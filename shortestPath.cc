#include <limits>
#include <vector>
#include <iostream>
#include <thread>
#include "BinaryHeap.h"

using namespace std;

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  bool* in = new bool[numVertices];
  for(int i = 0; i < numVertices; i++)
    {
      dist[i] = numeric_limits<double>::infinity();
      prev[i] = 0;
      in[i] = false;
    }

  int start = source;
  in[start] = true;
  dist[start] = 0;
  int next = start;

  for(int i = 1; i < numVertices; i++)
    {
      for(int vert = 0; vert < numVertices; vert++)
	{
	  if(not in[vert] && ((dist[next] + graph[next][vert]) < dist[vert]))
	    {
	      dist[vert] = dist[next] + graph[next][vert];
	      prev[vert] = next;
	    }
	}
      double min = numeric_limits<double>::infinity();
      for(int vert = 0; vert < numVertices; vert++)
	{
	  if(not in[vert] && dist[vert] < min)
	    {
	      next = vert;
	      min = dist[vert];
	    }
	}
      cout << "min " << min << endl;
      in[next] = true;
    }
  delete[] in;
}


void dijkstra(const int* const * adj, const double* const * weights, const int* lengths, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  for(int i = 0; i < numVertices; i++)
    {
      dist[i] = numeric_limits<double>::infinity();
      prev[i] = 0;
    }

  BinaryHeap heap = BinaryHeap(dist, numVertices);
  heap.decreasePriority(source, 0);
  int min = heap.getMin();
  for(int i = 0; i < numVertices; i++)
    {
      for(int j = 0; j < lengths[i]; j++)
	{
	  if(heap.contains(adj[i][j]) and dist[min] + weights[i][j] < heap.getPriority(adj[i][j]))
	    {
	      heap.decreasePriority(adj[i][j], dist[min]+weights[i][j]);
	      prev[i] = min;
	    }
	}
      min = heap.getMin();
    }
}

int bellmanFord(const double* const * graph, int numVertices, int source, double*& dist, int*& prev)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  // make edges vector
  vector<pair<int, int>> edges;
  for(int i = 0; i < numVertices; i++)
    {
      for(int j = 0; j < numVertices; j++)
	{
	  if(graph[i][j] != 0 && graph[i][j] != numeric_limits<double>::infinity())
	    {
	      pair<int, int> pair (i, j);
	      edges.push_back(pair);
	    }
	}
    }
  // make dist and prev
  for(int i = 0; i < numVertices; i++)
    {
      dist[i] = numeric_limits<double>::infinity();
      prev[i] = 0;
    }
  dist[source] = 0;

  int lastUpdate;
  for(int i = 0; i < numVertices - 1; i++)
    {
      for(int j = 0; j < edges.size(); j++)
	{
	  if(dist[edges[j].second] > dist[edges[j].first] + graph[edges[j].first][edges[j].second])
	    {
	      dist[edges[j].second] = dist[edges[j].first] + graph[edges[j].first][edges[j].second];
	      prev[edges[j].second] = edges[j].first;
	      lastUpdate = edges[j].second;
	    }
	}
    }
  for(int i = 0; i < edges.size(); i++)
    {
      if(dist[edges[i].second] > dist[edges[i].first] + graph[edges[i].first][edges[i].second])
	{
	  return lastUpdate + 1;
	}
    }
  return 0;
}

void initialize(double* dist, int* prev, bool* in, int start, int end, int numThreads)
{ 
  if(end == start + 1)
    {
      (dist)[start] = numeric_limits<double>::infinity();
      (prev)[start] = 0;
      (in)[start] = false;

    }
  else
    {
      if(numThreads == 1)
	{
	  int mid = (start+end)/2;
	  initialize(dist, prev, in, start, mid, 1);
	  initialize(dist, prev, in, mid, end, 1);
	}
      else
	{
	  int half = numThreads/2;
	  int mid = start + (half*(end-start))/numThreads;
	  thread t(initialize, dist, prev, in, start, mid, half);
	  initialize(dist, prev, in, mid, end, numThreads-half);
	  t.join();

	}
    }
}

void getMinVert(double* dist, bool* in, int start, int end, int numThreads, int* min)
{
  if(end == start+1)
    {
      *min = start;
    }
  else
    {
      int min1 = 0;
      int min2 = 0;
      if(numThreads == 1)
  	{
  	  int mid = (end+start)/2;
  	  getMinVert(dist, in, start, mid, 1, &min1);
	  getMinVert(dist, in, mid, end, 1, &min2);
  	}
      else
  	{
  	  int half = numThreads/2;
  	  int mid = start + (half*(end-start))/numThreads;
	  thread t(getMinVert, dist, in, start, mid, half, &min1);
	  getMinVert(dist, in, mid, end, (numThreads-half), &min2);
  	  t.join();
  	}
      double minDist = numeric_limits<double>::infinity();
      if((dist)[min1] < minDist && not (in)[min1])
	{
	  (*min) = min1;
	  minDist = (dist)[min1];
	}
      if((dist)[min2] < minDist && not (in)[min2])
	{
	  (*min) = min2;
	}      
    }
}

void update(const double* const * graph, double* dist, int* prev, bool* in, int start, int end, int numThreads, int vert)
{
  if(end == start+1)
    {
      if(not (in)[start] && (dist)[start] > (dist)[vert] + (graph)[vert][start])
  	{
  	  (dist)[start] = (dist)[vert] + (graph)[vert][start];
  	  (prev)[start] = vert;
  	}
    }
  else
    {
      if(numThreads == 1)
  	{
  	  int mid = (end+start)/2;
  	  update(graph, dist, prev, in, start, mid, 1, vert);
  	  update(graph, dist, prev, in, mid, end, 1, vert);
  	}
      else
  	{
  	  int half = numThreads/2;
  	  int mid = start + (half*(end-start))/numThreads;
  	  thread t(update, graph, dist, prev, in, start, mid, half, vert);
  	  update(graph, dist, prev, in, mid, end, numThreads-half, vert);
	  t.join();
  	}
    }
}

void dijkstra(const double* const * graph, int numVertices, int source, double*& dist, int*& prev, int numThreads)
{
  dist = new double[numVertices];
  prev = new int[numVertices];
  bool* in = new bool[numVertices];
  initialize(dist, prev, in, 0, numVertices, numThreads);
  dist[source] = 0;
  for(int i = 0; i < numVertices; i++)
    {
      int recMin = 0;
      getMinVert(dist, in, 0, numVertices, numThreads, &recMin);
      in[recMin] = true;
      update(graph, dist, prev, in, 0, numVertices, numThreads, recMin);
    }
  delete[] in;
}
