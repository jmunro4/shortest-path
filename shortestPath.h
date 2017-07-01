#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev);

void dijkstra(const int* const * adj, const double* const * weights, const int* lengths, int numVertices, int source, double*& dist, int*& prev);

int bellmanFord(const double* const * graph, int numVertices, int source, double*& dist, int*& prev);

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev, int numThreads);

void initialize(double* dist, int* prev, bool* in, int start, int end, int numThreads);

void getMinVert(double* dist, bool* in, int start, int end, int numThreads, int* min);

void update(const double* const * graph, double* dist, int* prev, bool* in, int start, int end, int numThreads, int vert);

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev, int numThreads);

#endif
