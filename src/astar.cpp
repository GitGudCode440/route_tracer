#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

struct Node {
    double lat, lon;
};
struct Edge {
    int64_t to;
    double weight;
};

std::unordered_map<int64_t, Node> nodes;
std::unordered_map<int64_t, std::vector<Edge>> adj;

double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000.0;
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1 * M_PI / 180.0) * std::cos(lat2 * M_PI / 180.0) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
}

void loadKarachiMap(const std::string& filename) {
    struct MapHandler : public osmium::handler::Handler {
        void node(const osmium::Node& node) {
            if (node.location().valid()) {
                nodes[node.id()] = {node.location().lat(), node.location().lon()};
            }
        }
        void way(const osmium::Way& way) {
            const osmium::WayNodeList& wnl = way.nodes();
            for (auto it = wnl.begin(); std::next(it) != wnl.end(); ++it) {
                int64_t id1 = it->ref();
                int64_t id2 = std::next(it)->ref();
                if (nodes.count(id1) && nodes.count(id2)) {
                    double d = haversine(nodes[id1].lat, nodes[id1].lon,
                                         nodes[id2].lat, nodes[id2].lon);
                    adj[id1].push_back({id2, d});
                    adj[id2].push_back({id1, d});
                }
            }
        }
    };

    try {
        osmium::io::Reader reader(filename);
        MapHandler handler;
        osmium::apply(reader, handler);
        reader.close();
        std::cout << "Map loaded successfully! "
                  << "Nodes: " << nodes.size()
                  << "  Adjacencies: " << adj.size() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error reading Karachi map: " << e.what() << "\n";
    }
}

std::vector<int64_t> astar(int64_t start, int64_t goal) {
    std::unordered_map<int64_t, double> gScore, fScore;
    std::unordered_map<int64_t, int64_t> parent;

    for (const auto& p : nodes) {
        gScore[p.first] = std::numeric_limits<double>::infinity();
        fScore[p.first] = std::numeric_limits<double>::infinity();
    }

    auto cmp = [&](int64_t a, int64_t b) { return fScore[a] > fScore[b]; };
    std::priority_queue<int64_t, std::vector<int64_t>, decltype(cmp)> open(cmp);

    gScore[start] = 0.0;
    fScore[start] = haversine(nodes[start].lat, nodes[start].lon,
                              nodes[goal].lat, nodes[goal].lon);
    open.push(start);

    while (!open.empty()) {
        int64_t current = open.top();
        open.pop();

        if (current == goal) {
            std::vector<int64_t> path;
            for (int64_t at = goal; parent.find(at) != parent.end(); at = parent[at])
                path.push_back(at);
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& edge : adj[current]) {
            double tentative = gScore[current] + edge.weight;
            if (tentative < gScore[edge.to]) {
                parent[edge.to] = current;
                gScore[edge.to] = tentative;
                fScore[edge.to] = tentative +
                    haversine(nodes[edge.to].lat, nodes[edge.to].lon,
                              nodes[goal].lat, nodes[goal].lon);
                open.push(edge.to);
            }
        }
    }

    return {};
}

void Astar() {
    const std::string map_file = "/home/kali/source/repos/route_tracer/data/karachi.osm.pbf";
    loadKarachiMap(map_file);

    int64_t start, goal;
    std::cout << "Enter start node ID: ";
    std::cin >> start;
    std::cout << "Enter goal node ID: ";
    std::cin >> goal;

    if (!nodes.count(start) || !nodes.count(goal)) {
        std::cerr << "Invalid node IDs.\n";
        return;
    }

    // Generate output file name
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::stringstream filename;
    filename << "/home/kali/source/repos/route_tracer/data/path_output_"
             << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S") << ".txt";

    std::ofstream outfile(filename.str());
    if (!outfile) {
        std::cerr << "Failed to create output file.\n";
        return;
    }

    // A*
    std::vector<int64_t> path = astar(start, goal);

    outfile << "Start Node ID: " << start << "\n";
    outfile << "Goal Node ID: " << goal << "\n";
    outfile << "------------------------------------\n";

    if (path.empty()) {
        outfile << "No path found between given nodes.\n";
        std::cout << "No path found between given nodes.\n";
    } else {
        double total = 0;
        outfile << "Shortest path:\n";
        for (size_t i = 0; i < path.size(); ++i) {
            outfile << path[i];
            if (i + 1 < path.size()) {
                double d = haversine(nodes[path[i]].lat, nodes[path[i]].lon,
                                     nodes[path[i + 1]].lat, nodes[path[i + 1]].lon);
                total += d;
                outfile << " -> ";
            }
        }
        outfile << "\nTotal distance: " << total / 1000.0 << " km\n";

        std::cout << "Path saved successfully to: " << filename.str() << "\n";
        std::cout << "Total distance: " << total / 1000.0 << " km\n";
    }

    outfile.close();
}