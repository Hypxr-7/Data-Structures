#include <iostream>

#include "../data_structures/Graph.hpp"
#include "../data_structures/GraphGenerator.hpp"

int main() {
    try {
        // Create various types of graphs

        // 1. Complete graph with 5 vertices
        std::cout << "=== Complete Graph (5 vertices) ===" << std::endl;
        Graph completeGraph = GraphGenerator::complete(5);
        std::cout << "Vertices: " << completeGraph.V() << std::endl;
        std::cout << "Edges: " << completeGraph.E() << std::endl;
        std::cout << completeGraph << std::endl;

        // 2. Random simple graph with 6 vertices and 8 edges
        std::cout << "\n=== Random Simple Graph (6 vertices, 8 edges) ===" << std::endl;
        Graph randomGraph = GraphGenerator::simple(6, 8);
        std::cout << "Vertices: " << randomGraph.V() << std::endl;
        std::cout << "Edges: " << randomGraph.E() << std::endl;
        std::cout << randomGraph << std::endl;

        // 3. Erdős–Rényi random graph (probability model)
        std::cout << "\n=== Erdős–Rényi Graph (8 vertices, p=0.3) ===" << std::endl;
        Graph erdosRenyi = GraphGenerator::simple(8, 0.3);
        std::cout << "Vertices: " << erdosRenyi.V() << std::endl;
        std::cout << "Edges: " << erdosRenyi.E() << std::endl;
        std::cout << erdosRenyi << std::endl;

        // 4. Complete bipartite graph
        std::cout << "\n=== Complete Bipartite Graph (3,4) ===" << std::endl;
        Graph bipartiteGraph = GraphGenerator::completeBipartite(3, 4);
        std::cout << "Vertices: " << bipartiteGraph.V() << std::endl;
        std::cout << "Edges: " << bipartiteGraph.E() << std::endl;
        std::cout << bipartiteGraph << std::endl;

        // 5. Path graph
        std::cout << "\n=== Path Graph (7 vertices) ===" << std::endl;
        Graph pathGraph = GraphGenerator::path(7);
        std::cout << "Vertices: " << pathGraph.V() << std::endl;
        std::cout << "Edges: " << pathGraph.E() << std::endl;
        std::cout << pathGraph << std::endl;

        // 6. Cycle graph
        std::cout << "\n=== Cycle Graph (6 vertices) ===" << std::endl;
        Graph cycleGraph = GraphGenerator::cycle(6);
        std::cout << "Vertices: " << cycleGraph.V() << std::endl;
        std::cout << "Edges: " << cycleGraph.E() << std::endl;
        std::cout << cycleGraph << std::endl;

        // 7. Binary tree
        std::cout << "\n=== Binary Tree (7 vertices) ===" << std::endl;
        Graph binaryTree = GraphGenerator::binaryTree(7);
        std::cout << "Vertices: " << binaryTree.V() << std::endl;
        std::cout << "Edges: " << binaryTree.E() << std::endl;
        std::cout << binaryTree << std::endl;

        // 8. Star graph
        std::cout << "\n=== Star Graph (6 vertices) ===" << std::endl;
        Graph starGraph = GraphGenerator::star(6);
        std::cout << "Vertices: " << starGraph.V() << std::endl;
        std::cout << "Edges: " << starGraph.E() << std::endl;
        std::cout << starGraph << std::endl;

        // 9. Wheel graph
        std::cout << "\n=== Wheel Graph (7 vertices) ===" << std::endl;
        Graph wheelGraph = GraphGenerator::wheel(7);
        std::cout << "Vertices: " << wheelGraph.V() << std::endl;
        std::cout << "Edges: " << wheelGraph.E() << std::endl;
        std::cout << wheelGraph << std::endl;

        // 10. Random tree
        std::cout << "\n=== Random Tree (8 vertices) ===" << std::endl;
        Graph randomTree = GraphGenerator::tree(8);
        std::cout << "Vertices: " << randomTree.V() << std::endl;
        std::cout << "Edges: " << randomTree.E() << std::endl;
        std::cout << randomTree << std::endl;

        // 11. Regular graph (each vertex has degree 3)
        std::cout << "\n=== 3-Regular Graph (6 vertices) ===" << std::endl;
        Graph regularGraph = GraphGenerator::regular(6, 3);
        std::cout << "Vertices: " << regularGraph.V() << std::endl;
        std::cout << "Edges: " << regularGraph.E() << std::endl;
        std::cout << regularGraph << std::endl;

        // 12. Eulerian cycle
        std::cout << "\n=== Eulerian Cycle (5 vertices, 8 edges) ===" << std::endl;
        Graph eulerianCycle = GraphGenerator::eulerianCycle(5, 8);
        std::cout << "Vertices: " << eulerianCycle.V() << std::endl;
        std::cout << "Edges: " << eulerianCycle.E() << std::endl;
        std::cout << eulerianCycle << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}