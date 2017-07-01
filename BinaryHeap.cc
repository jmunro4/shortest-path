#include "BinaryHeap.h"
#include <iostream>

using namespace std;

BinaryHeap::BinaryHeap(const double* priorities, int numItems)
{
  this->heapsize = numItems;
  this->origsize = numItems;
  this->priorities = new double[numItems];
  this->items = new int[numItems];
  this->location = new int[numItems];

  for(int i = 0; i < numItems; i++)
    {
      this->priorities[i] = priorities[i];
      this->items[i] = i;
      this->location[i] = i;
    }

  for(int i = numItems/2 - 1; i >= 0; i--)
    {
      this->minHeapify(i);
    }
}

BinaryHeap::~BinaryHeap()
{
  delete[] this->priorities;
  delete[] this->items;
  delete[] this->location;
}

void BinaryHeap::minHeapify(int index)
{
  int smallest = index;
  int left = 2*(index+1)-1;
  int right = 2*(index+1);

  if(left <= this->heapsize-1 && this->priorities[this->items[left]] < this->priorities[this->items[smallest]])
    {
      smallest = left;
    }
  if(right <= this->heapsize-1 && this->priorities[this->items[right]] < this->priorities[this->items[smallest]])
    {
      smallest = right;
    }

  if(smallest != index)
    {
      int holdItem = this->items[index];
      this->items[index] = this->items[smallest];
      this->items[smallest] = holdItem;

      this->minHeapify(smallest);
    }
}

int BinaryHeap::getMin() const
{
  return this->items[0];
}

void BinaryHeap::popMin()
{
  if(this->heapsize > 0)
    {
      this->location[this->items[0]] = -1;
      this->items[0] = this->items[this->heapsize-1];
      this->location[this->items[this->heapsize-1]] = 0;
      this->heapsize--;
      this->minHeapify(0);
    }
}

bool BinaryHeap::contains(int item) const
{
  if(item > 0 && item < origsize)
    {
      return this->location[item] >= 0;
    }
  return false;
}
    
double BinaryHeap::getPriority(int item) const
{
  return this->priorities[item];
}

void BinaryHeap::decreasePriority(int item, double newPriority)
{
  if(this->contains(item) && newPriority < this->priorities[item])
    {
      this->priorities[item] = newPriority;
      for(int i = this->heapsize/2 - 1; i >= 0; i--)
	{
	  this->minHeapify(i);
	}
    }
}

void BinaryHeap::printBH()
{
  cout << "BH: " << endl;
  for(int i = 0; i < this->heapsize; i++)
    {
      cout << this->items[i] << ' ';
    }
  cout << endl;
}
