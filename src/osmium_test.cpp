// file: osm_test.cpp

#include <iostream>
#include <string>
#include <osmium/io/any_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/node.hpp>

class MyHandler : public osmium::handler::Handler {
public:
    void node(const osmium::Node& node) {
        const char* name = node.tags()["name"];
        if (name) {
            std::cout << "Node id=" << node.id()
                      << " lat=" << node.location().lat()
                      << " lon=" << node.location().lon()
                      << " name=\"" << name << "\"\n";
        }
    }
};

void parseMap() {
   

    const std::string input_file = "/home/mmaha/Documents/repos/route_tracer/data/andorra-251022.osm.pbf";

    try {
        osmium::io::Reader reader(input_file);
        MyHandler handler;
        osmium::apply(reader, handler);
        reader.close();
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
    }

}
