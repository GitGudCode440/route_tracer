🚀 Route Tracer – Shortest Path Finder Using A* (C++ + Libosmium + Protozero + OpenGL)

This repository contains our Data Structures & Algorithms (DSA) project, where we built a system to find the shortest path and shortest distance between two points on a real-world map using OpenStreetMap (OSM) data.

We implemented the A* algorithm as the core pathfinding method and used Libosmium, Protozero, and OpenGL for data parsing and visualization.

📌 Features
✔ Shortest path calculation using A*
✔ Real OpenStreetMap data parsing
✔ Map rendering using OpenGL
✔ Node & coordinate handling
✔ Efficient data structures for graph representation
✔ Interactive visualization of the computed route

🧠 Technologies Used
Component	Technology
Language	C++
Pathfinding Algorithm	A*
Map Data Parsing	Libosmium, Protozero
Rendering	OpenGL
Data Source	OSM (.osm/.pbf)

🧩 How It Works
The program loads and parses the map using Libosmium + Protozero.
Nodes and ways are converted into a graph structure.
The A* algorithm calculates the shortest path between user-selected coordinates.
OpenGL renders the map and final path visually.

🛠 Installation & Setup
1. Clone the repository
git clone https://github.com/GitGudCode440/route_tracer
cd route_tracer

2. Install Dependencies
Make sure your system has:
C++17 or higher
CMake
Libosmium
Protozero
OpenGL & GLUT
OSM/PBF map file

3. Build the project
mkdir build
cd build
cmake ..
make

4. Run
./route_tracer
🗺 Input Requirements
Load an OSM map file
Enter start and goal coordinates (latitude & longitude)
The program calculates:
Shortest distance
Shortest path (sequence of nodes)

📚 What We Learned

Working on this project helped us understand:
How real-world map data is structure
How A* works with heuristics, costs, and priority queues
Graph traversal & optimization
Parsing and processing large datasets
Efficient rendering and memory management
Practical application of DSA concepts in real-world scenarios
This project significantly improved our understanding of data structures, algorithms, and system design.

👨‍💻 Team Members-(with github profiles)

1. M. Mahad Hashmi (GitGudCode440)
2. Mohammad Bilal (MHTBEELAL)
3. Muhammad Usman Murtaza (MuhammadUsman-Murtaza)

📄 License
This project is open-source under the MIT License.

⭐ Contribute
Feel free to fork the repository, open issues, or submit pull requests!
