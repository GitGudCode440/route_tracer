#include <iostream>
#include <unordered_map>
#include <unordered_set>
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
#include <algorithm>

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
        std::cout << "Map loaded successfully! Nodes: " << nodes.size()
                  << "  Adjacencies: " << adj.size() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error reading Karachi map: " << e.what() << "\n";
    }
}

std::vector<int64_t> astar(int64_t start, int64_t goal) {
    std::unordered_map<int64_t, double> gScore;
    std::unordered_map<int64_t, double> fScore;
    std::unordered_map<int64_t, int64_t> parent;

    // Initialize scores only as needed (more efficient)
    gScore[start] = 0.0;
    fScore[start] = haversine(nodes[start].lat, nodes[start].lon,
                              nodes[goal].lat, nodes[goal].lon);

    // Use min-heap with proper comparison
    auto cmp = [&](const std::pair<int64_t, double>& a, const std::pair<int64_t, double>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int64_t, double>, 
                       std::vector<std::pair<int64_t, double>>, 
                       decltype(cmp)> openSet(cmp);

    openSet.push({start, fScore[start]});
    std::unordered_set<int64_t> inOpenSet;
    inOpenSet.insert(start);

    int nodes_explored = 0;

    while (!openSet.empty()) {
        auto current_pair = openSet.top();
        int64_t current = current_pair.first;
        openSet.pop();
        inOpenSet.erase(current);

        nodes_explored++;

        if (current == goal) {
            std::vector<int64_t> path;
            for (int64_t at = goal; at != start; at = parent[at]) {
                path.push_back(at);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            std::cout << "Path found! Nodes explored: " << nodes_explored << "\n";
            return path;
        }

        if (!adj.count(current)) continue;

        for (const auto& edge : adj[current]) {
            double tentative_gScore = gScore[current] + edge.weight;
            
            // Initialize if not present
            if (!gScore.count(edge.to)) {
                gScore[edge.to] = std::numeric_limits<double>::infinity();
            }

            if (tentative_gScore < gScore[edge.to]) {
                parent[edge.to] = current;
                gScore[edge.to] = tentative_gScore;
                fScore[edge.to] = tentative_gScore + 
                    haversine(nodes[edge.to].lat, nodes[edge.to].lon,
                              nodes[goal].lat, nodes[goal].lon);
                
                // Only push to open set if not already there with better score
                if (inOpenSet.find(edge.to) == inOpenSet.end()) {
                    openSet.push({edge.to, fScore[edge.to]});
                    inOpenSet.insert(edge.to);
                }
            }
        }
    }

    std::cout << "No path found after exploring " << nodes_explored << " nodes.\n";
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

    // Calculate straight-line distance for comparison
    double straight_distance = haversine(nodes[start].lat, nodes[start].lon,
                                         nodes[goal].lat, nodes[goal].lon);
    std::cout << "Straight-line distance: " << straight_distance / 1000.0 << " km\n";

    // A*
    std::cout << "Calculating shortest path...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<int64_t> path = astar(start, goal);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    outfile << "Start Node ID: " << start << "\n";
    outfile << "Goal Node ID: " << goal << "\n";
    outfile << "Straight-line distance: " << straight_distance / 1000.0 << " km\n";
    outfile << "Google Maps distance: ~1.6 km\n";
    outfile << "Calculation time: " << duration.count() << " ms\n";
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
        outfile << "Path length: " << path.size() << " nodes\n";
        outfile << "Efficiency ratio: " << total / straight_distance << " (ideal: ~1.0)\n";

        std::cout << "Path saved successfully to: " << filename.str() << "\n";
        std::cout << "Total distance: " << total / 1000.0 << " km\n";
        std::cout << "Efficiency ratio: " << total / straight_distance << " (ideal: ~1.0)\n";
        
        if (total / straight_distance > 1.3) {
            std::cout << "WARNING: Path is significantly longer than straight-line distance!\n";
            std::cout << "This may indicate missing direct road connections in the OSM data.\n";
        }
    }

    outfile.close();
}