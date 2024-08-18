# Kruskal-Tree
This program is designed to illustrate the process of finding the minimum cost to connect all cities in a graph using Kruskal's algorithm, which is particularly efficient for sparse graphs. The graph is statically defined, and the code demonstrates basic memory management, graph traversal, and union-find techniques in C.

### Key Components:
1. **Graph Representation**: 
   - `Grafo` structure represents a city (vertex) and a road (edge) with a pointer to the next edge in the adjacency list.
   - `Aresta` structure represents an edge with an origin city, destination city, and weight (distance).

2. **Subsets for Union-Find**: 
   - `Subconjunto` structure is used for the union-find operations to manage the connected components during Kruskal's algorithm, keeping track of each city's parent and tree height.

3. **Functions**:
   - `printaCidades()`: Prints the adjacency list representation of the graph.
   - `liberaMemoria()`: Frees the allocated memory for the graph.
   - `find()`: A recursive function to find the root of a subset using path compression.
   - `Union()`: Merges two subsets into one based on their tree heights.
   - `comparaArestas()`: Compares the weights of two edges to assist in sorting.
   - `KruskalMST()`: Implements Kruskal's algorithm by sorting edges and selecting the smallest ones that don't form a cycle to build the MST.

4. **Main Function**:
   - Initializes the graph by creating cities and connecting them with roads.
   - Prints the initial graph.
   - Runs Kruskal's algorithm to find the MST.
   - Prints the roads included in the MST.
   - Frees the allocated memory before exiting.
