#include <limits>
#include <iostream>
#include <fstream>
#include <chrono>
#include "shortestPath.h"
#include "BinaryHeap.h"

using namespace std;

int readGraph(ifstream& fin, double**& matrix, string*& labels)
{
  int numVerts;
  fin >> numVerts;
  
  labels = new string[numVerts];
  matrix = new double*[numVerts];

  string s;
  for(int i = 0; i < numVerts; i++)
    {
      fin >> s;
      labels[i] = s;
    }

  double d;
  for(int i = 0; i < numVerts; i++)
    {
      double* weight = new double[numVerts];
      for(int j = 0; j < numVerts; j++)
	{
	  fin >> d;
	  if(d == 0 && i != j)
	    {
	      weight[j] = numeric_limits<double>::infinity();
	    }
	  else
	    {
	      weight[j] = d;
	    }
	}
      matrix[i] = weight;
    } 
  return numVerts;
}

int readGraph(ifstream& fin, int**& adj, double**& weights, int*& lengths, string*& labels)
{
  int numVerts;
  fin >> numVerts;

  adj = new int*[numVerts];
  weights = new double*[numVerts];
  lengths = new int[numVerts];
  labels = new string[numVerts];

  int count;

  string s;
  for(int i = 0; i < numVerts; i++)
    {
      fin >> s;
      labels[i] = s;
    }

  for(int i = 0; i < numVerts; i++)
    {
      double* line = new double[numVerts];
      double d;
      count = 0;
      for(int j = 0; j < numVerts; j++)
	{
	  fin >> d;
	  line[j] = d;
	  if(d != 0)
	    {
	      count++;
	    }
	}
      lengths[i] = count;

      int* subadj = new int[count];
      double* subweights = new double[count];
      count = 0;
      for(int j = 0; j < numVerts; j++)
	{
	  if(line[j] != 0)
	    {
	      subadj[count] = j;
	      subweights[count] = line[j];
	      count++;
	    }
	}
      adj[i] = subadj;
      weights[i] = subweights;
      delete[] line;
    }
  return numVerts;
}


int main(int argc, char** argv)
{
  // Command line args
  ifstream fin(argv[1]);
  string start = argv[2];
  string end = argv[3];
  int numThreads = atoi(argv[4]);


  double** matrix;
  string* labels;
  int numVerts = readGraph(fin, matrix, labels);
  fin.close();
  
  cout << "Adjacency Matrix" << endl;
  for(int i = 0; i < numVerts; i++)
    {
      cout << labels[i] << ": ";
      for(int j = 0; j < numVerts; j++)
	{
	  cout << matrix[i][j] << ' ';
	}
      cout << endl;
    }
  delete[] labels;


  
  int** adj;
  double** weights;
  int* lengths;

  ifstream fin1(argv[1]);
  
  numVerts = readGraph(fin1, adj, weights, lengths, labels);

  fin1.close();
  
  cout << endl << "Adjacency List" << endl;
  for(int i = 0; i < numVerts; i++)
    {
      cout << labels[i] << ": ";
      for(int j = 0; j < lengths[i]; j++)
	{
	  cout << labels[adj[i][j]] << "(" << weights[i][j] << ") ";
	}
      cout << endl;
    }

  
  cout << endl << "Dijkstra (Matrix)" << endl;
  
  int startint;
  int endint;

  for(int i = 0; i < numVerts; i++)
    {
      if(labels[i] == start)
	{
	  startint = i;
	}
      if(labels[i] == end)
	{
	  endint = i;
	}
    }
  double* dist;
  int* prev;

  auto ti = chrono::system_clock::now();
  dijkstra(matrix, numVerts, startint, dist, prev);
  auto tf = chrono::system_clock::now();
  auto dur = tf - ti;
  auto durms = chrono::duration_cast<chrono::microseconds>(dur);
  int elapsed = durms.count();

  string path = labels[endint];
  int cur = endint;
  while(cur != startint)
    {
      cur = prev[cur];
      path = labels[cur] + " -> " + path;
    }

  cout << "Path: " << path << " (dist: " << dist[endint] << ")" << endl;
  cout << "Runtime: " << elapsed << " microseconds" << endl;

  delete[] dist;
  delete[] prev;

  cout << endl << "Dijkstra (List)" << endl;
  
  ti = chrono::system_clock::now();
  dijkstra(matrix, numVerts, startint, dist, prev);
  tf = chrono::system_clock::now();
  dur = tf - ti;
  durms = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durms.count();
  
  path = labels[endint];
  cur = endint;
  while(cur != startint)
    {
      cur = prev[cur];
      path = labels[cur] + " -> " + path;
    }

  cout << "Path: " << path << " (dist: " << dist[endint] << ")" << endl;
  cout << "Runtime: " << elapsed << " microseconds" << endl << endl;

  delete[] dist;
  delete[] prev;

  cout << endl << "Bellman-Ford" << endl;
  int lastIndex;

  ti = chrono::system_clock::now();
  lastIndex = bellmanFord(matrix, numVerts, startint, dist, prev);
  tf = chrono::system_clock::now();
  dur = tf - ti;
  durms = chrono::duration_cast<chrono::microseconds>(dur);
  elapsed = durms.count();


  bool* in = new bool[numVerts];
  for(int i = 0; i < numVerts; i++)
    {
      in[i] = false;
    }

  if(lastIndex == 0)
    {
      path = labels[endint];
      cur = endint;
      while(cur != startint)
	{
	  cur = prev[cur];
	  path = labels[cur] + " -> " + path;
	}
      cout << "Path: " << path << " (dist: " << dist[endint] << ")" << endl;
      cout << "Runtime: " << elapsed << " microseconds" << endl << endl;
    }
  else
    {
      int cur = lastIndex - 1;
      int weight = 0;
      while(in[cur] != true)
      	{
      	  in[cur] = true;
      	  cur = prev[cur];
      	}
      path = labels[cur];
      while(in[cur] != false)
      	{
      	  in[cur] = false;
	  weight += matrix[prev[cur]][cur];
      	  cur = prev[cur];
      	  path = labels[cur] + " -> " + path;
      	}
      cout << "Cycle: " << path << " (weight: " << weight << ")" << endl;
      cout << "Runtime: " << elapsed << " microseconds" << endl << endl;
    }

  delete[] dist;
  delete[] prev;
  delete[] in;

  ti = chrono::system_clock::now();
  auto startCPU = clock();
  dijkstra(matrix, numVerts, startint, dist, prev, 1);
  tf = chrono::system_clock::now();
  auto endCPU = clock();
  dur = tf - ti;
  auto durCPU = endCPU - startCPU;
  durms = chrono::duration_cast<chrono::microseconds>(dur);
  long long serialTime = durms.count();

  path = labels[endint];
  cur = endint;
  while(cur != startint)
    {
      cur = prev[cur];
      path = labels[cur] + " -> " + path;
    }
  
  cout << "Parallel Dijkstra (1 thread)" << endl;
  cout << "Path: " << path << " (dist: " << dist[endint] << ")" << endl;
  cout << "CPU time: " << (durCPU)*(1000000/CLOCKS_PER_SEC) << " microseconds" << endl;
  cout << "Wall clock time: " << serialTime << " microseconds" << endl << endl;
  
  delete[] dist;
  delete[] prev;

  ti = chrono::system_clock::now();
  startCPU = clock();
  dijkstra(matrix, numVerts, startint, dist, prev, numThreads);
  tf = chrono::system_clock::now();
  endCPU = clock();
  dur = tf - ti;
  durCPU = endCPU - startCPU;
  durms = chrono::duration_cast<chrono::microseconds>(dur);
  long long parallelTime = durms.count();

  path = labels[endint];
  cur = endint;
  while(cur != startint)
    {
      cur = prev[cur];
      path = labels[cur] + " -> " + path;
    }

  cout << "Parallel Dijkstra (" << numThreads << " threads)" << endl;
  cout << "Path: " << path << " (dist: " << dist[endint] << ")" << endl;
  cout << "CPU time: " << (durCPU)*(1000000/CLOCKS_PER_SEC) << " microseconds" << endl;
  cout << "Wall clock time: " << parallelTime << " microseconds" << endl << endl;
  
  for(int i = 0; i < numVerts; i++)
    {
      delete[] matrix[i];
      delete[] adj[i];
      delete[] weights[i];
    }
  delete[] matrix;
  delete[] adj;
  delete[] weights;
  delete[] lengths;
  delete[] labels;
  delete[] dist;
  delete[] prev;
}
