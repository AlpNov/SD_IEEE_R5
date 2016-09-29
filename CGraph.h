#ifndef CGRAPH_H
#define CGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef enum { incorrect, correct } nbool;

typedef struct Cell {
	unsigned short x;
	struct Cell *next;
} Cell;
typedef struct Graph{
	nbool* visited;
	Cell** body;
}Graph;

Cell* InitializeList(unsigned short value);
Cell** createGraph(int size);
void printList(Cell* head);
void push_back(Cell** head, unsigned short value);
void push_front(Cell** head, unsigned short value);
nbool Contains(Cell**head, unsigned short searchValue);
void printGraph(Cell**head, int size);
void addAdj(Cell** currentgraph, unsigned short root, unsigned short dest);
unsigned short getSize(Cell* head);
unsigned short getSizeAtIndex(Cell** head, int size, int searchIndex);
unsigned short pop(Cell** head);
unsigned short getFirstValueAtIndex(Cell** currentGraph, int size, int Index);
void DFT(Cell** currentGraph, int size, unsigned short index);
void BFT(Cell** currentGraph, int size, unsigned short index);
Cell* searchList(Cell** head, unsigned short searchValue);
unsigned short peek(Cell** head);
void removeCell(Cell** head, unsigned short* searchValue);
void PrintNodesDepth(Cell** currentGraph, Cell** Distances, int size);
void PrintPath(Cell** currentGraph, Cell** Distances, int size);
Cell* CriticalPath(Cell** currentGraph, int size, unsigned short* start, unsigned short end);
Cell* Reverse(Cell* head);
void freeList(Cell* head);
#endif

