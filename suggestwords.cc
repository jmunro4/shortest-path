#include <iostream>
#include <fstream>

using namespace std;

void editDist(string word1, string word2, int**& matrix)
{
  // create matrix
  matrix = new int*[word1.size()+1];
  for(int i = 0; i <= word1.size(); i++)
    {
      matrix[i] = new int[word2.size()+1];
      matrix[i][word2.size()] = word1.size()-i;
    }
  for(int i = 0; i < word2.size(); i++)
    {
      matrix[word1.size()][i] = word2.size()-i;
    }

  for(int i = word1.size()-1; i >=0; i--)
    {
      for(int j = word2.size()-1; j >= 0; j--)
	{
	  if(word1[i] == word2[j])
	    {
	      matrix[i][j] = matrix[i+1][j+1];
	    }
	  else
	    {
	      
	      int min = matrix[i][j+1];
	      if(matrix[i+1][j+1] < min)
		{
		  min = matrix[i+1][j+1];
		}
	      if(matrix[i+1][j] < min)
		{
		  min = matrix[i+1][j];
		}
	      matrix[i][j] = 1 + min;
	    }
	}
    }
}



int main(int argc, char** argv)
{
  ifstream fin(argv[1]);
  string s = argv[2];
  int dist = atoi(argv[3]);

  while(! fin.eof())
    {
      int** matrix;
      string s2;

      fin >> s2;
      editDist(s, s2, matrix);

      if(matrix[0][0] <= dist)
	{
	  cout << s2 << endl;
	}

      
      for(int i = 0; i <= s.size(); i++)
	{
	  delete[] matrix[i];
	}
      delete[] matrix;
    }
}
