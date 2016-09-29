#include "CGraph.h"
#include <stdio.h>
#include <stdlib.h>
#include "GlobalVar.h"
void printList(Cell* head)
{
	Cell* temp = head;

	while (temp != NULL)
	{
		printf("%d ", temp->x);
		temp = temp->next;
	}
	printf("\n");
}
void push_back(Cell** head, unsigned short value)
{
	//DOES NOT CHECK FOR DUPLICATES

	Cell* current = *head;
	Cell* newNode = malloc(sizeof(Cell));
	newNode->x = value;
	newNode->next = NULL;

	if (current == NULL)
	{
		*head = newNode;
	}
	else
	{

		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = newNode;

	}

}

Cell* InitializeList(unsigned short value)
{
	Cell* temp = malloc(sizeof(Cell));
	temp->x = value;
	temp->next = malloc(sizeof(Cell));
	temp->next = NULL;
	return temp;
}
void push_front(Cell** head, unsigned short value)
{
	//if (Contains(head, value) == incorrect)
	//	{
	Cell* temp = malloc(sizeof(Cell));
	temp->x = value;
	temp->next = malloc(sizeof(Cell));
	temp->next = *head;
	*head = temp;
	//}
}
unsigned short pop(Cell** head)
{
	unsigned short returnvalue = 0;
	Cell* next_Cell = NULL;
	if (*head == NULL)
	{
		return 0;
	}
	next_Cell = (*head)->next;
	returnvalue = (*head)->x;
	free(*head);
	*head = next_Cell;
	return returnvalue;
}
nbool Contains(Cell** head, unsigned short searchValue)
{
	nbool ret = incorrect;
	Cell* current = *head;
	while (current != NULL)
	{
		if (current->x == searchValue)
		{
			ret = correct;
		}
		current = current->next;
	}
	return ret;
}
unsigned short getSizeAtIndex(Cell** currentGraph, int size, int searchIndex)
{
	int temp = 0;
	if (searchIndex <= size)
	{
		Cell* currentCell = currentGraph[searchIndex];
		temp = getSize(currentCell);
	}
	return temp;
}
unsigned short getSize(Cell* head)
{

	Cell* temp = head;
	unsigned short count = 0;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	return count;
}
Cell** createGraph(int size)
{
	//create an array of linked cells
	Cell** head = (Cell**)malloc(size * sizeof(Cell));
	int i;
	for (i = 0; i < size; i++)
	{
		head[i] = NULL;
	}
	return head;
}
//this can also be seen as add edge
void addAdj(Cell** currentgraph, unsigned short root, unsigned short dest)
{
	Cell* temp = InitializeList(dest);
	if (root != dest)
	{
		Cell* temp2 = currentgraph[root];
		if (Contains(&temp2, dest) == incorrect)
		{
			temp->next = currentgraph[root];
			currentgraph[root] = temp;
		}

		temp2 = currentgraph[dest];
		if (Contains(&temp2, root) == incorrect)
		{

			temp = InitializeList(root);
			temp->next = currentgraph[dest];
			currentgraph[dest] = temp;
		}
	}

}
void printGraph(Cell** head, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("head %d -> ", i);
		Cell* temp = head[i];
		printList(temp);
		printf("\n");
	}
}
void DFT(Cell** currentGraph, int size, unsigned short index)
{
	Cell* stack = NULL;
	Cell* path = NULL;


	nbool visited[SIZE];
	int i;
	for (i = 0; i < size; i++)
	{
		visited[i] = incorrect;
	}
	int prev = 0;
	int current = index;
	push_front(&stack, index);
	while (stack != NULL)
	{


		current = pop(&stack);

		Cell* currentAdjList = currentGraph[current];

		visited[current] = correct;
		while (currentAdjList != NULL)
		{
			if (visited[currentAdjList->x] == incorrect)
			{
				push_front(&stack, currentAdjList->x);

			}


			currentAdjList = currentAdjList->next;
		}
		push_back(&path, current);




	}
	printList(path);
}


unsigned short peek(Cell** head)
{
	Cell* tempcell = *head;
	unsigned short temp = 0;
	if ((tempcell) != NULL)
	{
		temp = tempcell->x;
	}

	return temp;
}
unsigned short getFirstValueAtIndex(Cell** currentGraph, int size, int Index)
{
	unsigned short temp = 0;
	if (Index < size)
	{
		if (currentGraph[Index] != NULL)
		{
			Cell* tempcell = currentGraph[Index];
			temp = tempcell->x;
		}
	}
	return temp;
}
Cell* searchStack(Cell** head, unsigned short searchValue)
{
	Cell* current = *head;
	Cell* newList = NULL;
	nbool found = incorrect;
	if (Contains(&current, searchValue) == correct)
	{

		while (current != NULL && found == incorrect)
		{
			if (peek(&current) == searchValue)
			{
				found = correct;


			}
			push_front(&newList, pop(&current));

		}
	}
	else
	{

		//current = NULL;
		newList = NULL;
	}
	return newList;
}
Cell* searchList(Cell** head, unsigned short searchValue)
{

	Cell* current = *head;
	Cell* newList = NULL;
	nbool found = incorrect;
	if (Contains(&current, searchValue) == correct)
	{

		while (current != NULL && found == incorrect)
		{
			if (current->x == searchValue)
			{
				found = correct;


			}

			push_back(&newList, current->x);
			current = current->next;

		}
	}
	else
	{

		//current = NULL;
		newList = NULL;
	}
	return newList;

}
void BFT(Cell** currentGraph, int size, unsigned short index)
{
	Cell* queue = InitializeList(index);





	nbool*visited = (nbool*)malloc(size * sizeof(nbool));
	int i;
	for (i = 0; i < size; i++)
	{
		visited[i] = incorrect;
	}

	int current = 0;


	while (queue != NULL)
	{

		current = pop(&queue);


		Cell* currentAdjList = currentGraph[current];

		visited[current] = correct;
		int j;
		for (j = 0; j < getSizeAtIndex(currentGraph, size, current); j++)
		{


			if (visited[currentAdjList->x] == incorrect && currentAdjList->x != current)
			{

				visited[currentAdjList->x] = correct;
				push_back(&queue, currentAdjList->x);

			}


			currentAdjList = currentAdjList->next;
		}
		printList(queue);

		printf("\n");
	}


}
void PrintNodesDepth(Cell** currentGraph, Cell** Distances, int size)
{
	printf("With Respect Start Cell \n");
	int i;
	for (i = 0; i < size; i++)
	{
		Cell* temp3 = currentGraph[i];
		Cell* temp2 = Distances[i];
		if (temp3 != NULL && temp2 != NULL)
		{
			printf("Values: ");
			printList(temp3);
			printf("\n");
			printf("Depth: ");
			printList(temp2);
			printf("\n");
		}
		//printf("\n");
	}
}
void PrintPath(Cell** currentGraph, Cell** Distances, int size)
{



}
Cell* CriticalPath(Cell** currentGraph, int size, unsigned short* start, unsigned short end)
{
	Cell* stack = NULL;
	Cell* path = NULL;


	nbool visited[SIZE];
	int i;
	for (i = 0; i < size; i++)
	{
		visited[i] = incorrect;
	}

	unsigned short current = *start;
	push_front(&stack, *start);
	//to find the exit we would search for the unique cell ID that pertains to it

	nbool neighborfound = incorrect;
	while (stack != NULL)
	{
		neighborfound = incorrect;
		current = peek(&stack);
		Cell* adjList = currentGraph[current];
		//check neighbors
		visited[current] = correct;
		if (current == end)
		{

			path = Reverse(stack);
			break;

		}
		while (adjList != NULL)
		{
			if (visited[adjList->x] == incorrect)
			{
				push_front(&stack, adjList->x);
				neighborfound = correct;
				//break needed to ensure one node looked at a time -- in combination with peek previous issues solved
				break;
			}
			adjList = adjList->next;
		}


		if (neighborfound == incorrect)
		{
			pop(&stack);
		}




	}



	return path;

}

void removeCell(Cell** head, unsigned short* searchValue)
{

	//if the value is found
	Cell* current = (*head)->next;
	Cell* prev= *head;
	if (Contains(head, *searchValue) == correct)
	{
		while (current != NULL && prev != NULL)
		{

			if (prev->x == *searchValue)
			{
				Cell* tmp = prev;
				prev = current;
				free(tmp);
				*head = prev;
				break;
			}
			if (current->x == searchValue)
			{
				prev->next = current->next;
				Cell* tmp = current;
				current = current->next;
				free(tmp);
				break;
			}
			prev = current;
			current = current->next;
		}
		
	}
}
Cell* Reverse(Cell* head)
{
	Cell* temp = head;
	Cell* newList = NULL;
	while (temp != NULL)
	{

		push_front(&newList, pop(&temp));
	}

	return newList;
}
void freeList(Cell* head)
{
	Cell* tmp;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
