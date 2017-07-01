Jonathan Munro

Files:
shortestPath.h
shortestPath.cc
BinaryHeap.h
BinaryHeap.cc
sptest.cc
currency.cc
suggestwords.cc
mapqueue.cc
Makefile
analysis.pdf
mapqueuefix.txt

This project contains one main program and other smaller programs. The 
command 'make all' will compile every program in the project. Alternatively,
there are instructions for individual compiling below.

---sptest--- 
To compile: make sptest 
To run: ./sptest <filename containing graph> <start vertex name> <end vertex name> <# of threads> 
Description: 
The purpose of the sptest program is to test, and record the timing of different methods 
to find the shortest path in a graph. The graph is given from a file, and read in as both
an Adjacency Matrix and an Adjacency List. To find the shortest path dijkstra and bellman-
ford algorithms are implemented. The shortest path is printed to the terminal with relevant 
timing data.

Known issues:

- If the thread count is greater than the number of vertices in the graph there is a chance
that a "Bus error: 10" is triggered. This is due to the load balancing, the threads are split
and the split point will be equal to the start value. (For example: 3 vertex graph & 5 threads) 

---currency---
To compile: make currency
To run: ./currency <filename containing graph> <start currency name> <cost per transaction>
Description:
The goal of this program is to find the cheapest method of converting currency. The program
receives exchange rates from a provided text file. The starting currency is given and the 
shortest path from that currrency to any other in the text file is printed to the terminal. 
If there is an instance of a negative loop (the opertunity to make more money by exchanging) 
the pathway is given. The program uses the bellman-ford algorithm with a conversion factor
applied to the exchange rate to be used easily.

Known issues:

---suggestwords---
To compile: make suggestwords
To run: ./suggestwords <filename containing words> <initial word> <minimum edit distance>
Description:
The program takes an initial file containing a large number of potential words, as well as 
an initial word (spelled correctly or incorrectly) and prints words that are potentially 
similar (like a spell check). The program only returns words within a given edit distance
which determines how different the suggested words can be. This is done by a method editDist.
The method creates a matrix, and fills it systematically to find the minimum number of edits
that would need to be made to make one word the other.

Known issues:

- The program is not efficent with memory. Maintaining two arrays, the current line of the 
array and the line previous could solve this.

---mapqueue---
To compile: make mapqueue
To run: ./mapqueue <*optional* number of consumers>
Description:
The goal of this program is to simulate the automated process of jobs being generated and
completed. The program runs in parallel and accesses a shared queue so it must be regulated
using mutex. In the program a thread is generating jobs based on a wait period, and adding
to the job queue. Other threads are removing from the job queue and simulating the jobs. 
The jobs have an associated wait period that tells the simulation how long it would take.

Known issues:

