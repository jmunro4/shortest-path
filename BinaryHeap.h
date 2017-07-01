#ifndef BINARY_HEAP_1_H
#define BINARY_HEAP_1_H

class BinaryHeap
{
 protected:
  int heapsize;
  int origsize;
  double* priorities;
  int* items;
  int* location;
  void minHeapify(int index);
 public:
  BinaryHeap(const double* priorities, int numItems);
  ~BinaryHeap();
  int getMin() const;
  void popMin();
  bool contains(int item) const;
  double getPriority(int item) const;
  void decreasePriority(int item, double newPriority);
  void printBH();
};

#endif
  
