// file: Map_Data.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/way.hpp>
#include <unordered_set>
#include <chrono>
#include <iomanip>
#include <sstream>

// Structure to hold merged road info
struct Road {
    std::string name;
    std::string type;
    std::vector<std::vector<osmium::object_id_type>> segments; // Each "Way" is one segment
};

class MyHandler : public osmium::handler::Handler {
public:
    std::map<std::pair<std::string, std::string>, Road> mergedRoads;

    void way(const osmium::Way& way) {
        const char* highway = way.tags()["highway"];
        const char* name = way.tags()["name"];

        static const std::unordered_set<std::string> major_roads = {
            "motorway", "trunk", "primary", "secondary", "tertiary"
        };

        if (highway && name && major_roads.count(highway)) {
            std::pair<std::string, std::string> key(name, highway);

            std::vector<osmium::object_id_type> nodes;
            for (const auto& node_ref : way.nodes()) {
                nodes.push_back(node_ref.ref());
            }

            auto& road = mergedRoads[key];
            if (road.name.empty()) {
                road.name = name;
                road.type = highway;
            }
            road.segments.push_back(nodes);
        }
    }

    void printMergedData(std::ostream& out) const {
        for (const auto& entry : mergedRoads) {
            const auto& road = entry.second;

            out << "Road: " << road.name
                << " | Type: " << road.type
                << " | Segments: " << road.segments.size()
                << "\n";

            for (size_t i = 0; i < road.segments.size(); ++i) {
                const auto& seg = road.segments[i];
                if (!seg.empty()) {
                    out << "  Segment " << (i + 1)
                        << " → Nodes: " << seg.front()
                        << " ... " << seg.back()
                        << " (" << seg.size() << " nodes)\n";
                }
            }
            out << "------------------------------------\n";
        }
    }
};

void parseMap() {
    const std::string input_file = "./data/karachi.osm.pbf";

    try {
        osmium::io::Reader reader(input_file);
        MyHandler handler;

        osmium::apply(reader, handler);
        reader.close();

        // Generate txt file
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream filename;
        filename << "./data/output_"
                 << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S")
                 << ".txt";

        std::ofstream outfile(filename.str());
        if (!outfile) {
            std::cerr << "Failed to create output file.\n";
            return;
        }

        handler.printMergedData(outfile);
        outfile.close();

        std::cout << "Map data successfully written to: " << filename.str() << "\n";
        std::cout << "Map data successfully merged and displayed.\n";

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }
}