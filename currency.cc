#include "shortestpath.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

int readGraphCur(ifstream& fin, double**& matrix, string*& labels, float cost)
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
  double adjusted;
  double f;
  for(int i = 0; i < numVerts; i++)
    {
      double* weight = new double[numVerts];
      for(int j = 0; j < numVerts; j++)
	{
	  fin >> f;
	  adjusted = -1 * log (f * (1 - cost)) ; //-log(exchange rate * (1 - transaction cost))
	  weight[j] = adjusted;
	}
      matrix[i] = weight;
    }
  return numVerts;
}


int main(int argc, char** argv)
{
  double** matrix;
  string* labels;
  ifstream fin(argv[1]);
  string start = argv[2];
  float cost = atof(argv[3]);

  int numVerts = readGraphCur(fin, matrix, labels, cost);

  int startint;
  for(int i = 0; i < numVerts; i++)
    {
      if(labels[i] == start)
	{
	  startint = i;
	}
    }
  
  int lastIndex;
  double* dist;
  int* prev;
  
  lastIndex = bellmanFord(matrix, numVerts, startint, dist, prev);

  bool* in = new bool[numVerts];
  for(int i = 0; i < numVerts; i++)
    {
      in[i] = false;
    }
  string path;
  double rate;
  if(lastIndex == 0)
    {
      for(int i = 0; i < numVerts; i++)
	{
	  path = labels[i];
	  int cur = i;
	  rate = 1;
	  while(cur != startint)
	    {
	      rate *= exp(-1*matrix[prev[cur]][cur]);
	      cur = prev[cur];
	      // multiply each thing by weight like adding the weights from the matrix
	      path = labels[cur] + " -> " + path;
	    }
	  cout << labels[i] << ": " << path << " (effective rate: " << rate << ")" << endl;
	}
    }
  else
    {
      rate = 1;
      int cur = lastIndex - 1;
      while(in[cur] != true)
	{
	  in[cur] = true;
	  rate *= exp(-1*matrix[prev[cur]][cur]);
	  cur = prev[cur];
	}
      path = labels[cur];
      while(in[cur] != false)
	{
	  in[cur] = false;
	  cur = prev[cur];
	  path = labels[cur] + " -> " + path;
	}
      cout << "Arbitrage oppertunity detected!" << endl;
      cout << "Cycle: " << path << "(effective rate: " << rate << ")" << endl;
    }
}
