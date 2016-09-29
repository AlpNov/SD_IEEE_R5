#include "MazeAlg.h"
#include "CGraph.h"
#include "GlobalVar.h"


/* DirectionFnc will help use wall distance sensors in order to determine
 * the next movement operation
 */
int DirectionFnc(uint32_t* SensorLeft, uint32_t* SensorRight, uint32_t* SensorRear, uint32_t* SensorFront, int* Tolerance)
{
	int temp = -1;
	if(*SensorLeft < *Tolerance)
	{
			temp =LEFT;
	}
	else if(*SensorFront < *Tolerance)
		{
			temp=FORWARD;
		}
	else if (*SensorRight < *Tolerance)
		{
			temp= RIGHT;
		}
	else if (*SensorRear < *Tolerance)
		{
			temp= REVERSE;
		}
return temp;

}
/* this function will use the orientationValue to determine all
 * cells adjacent to the current cell
 */
void CurrentAdjacent(uint32_t* SensorLeft, uint32_t* SensorRight, uint32_t* SensorRear, uint32_t* SensorFront, int* Tolerance, int* OrientationValue, unsigned short* currentCellValue, int* Dimension, Cell** graph)
{

	switch(*OrientationValue)
	{

	case NORTH:

	if(*SensorLeft < *Tolerance)
	{
		if((*currentCellValue - 1 < SIZE )&&(*currentCellValue - 1 > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue - 1);
		}
	}

	if (*SensorFront < *Tolerance)
	{
		if((*currentCellValue - *Dimension < SIZE )&&(*currentCellValue - *Dimension > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue - *Dimension);
		}
		
	}
	if (*SensorRight < *Tolerance)
	{
		if((*currentCellValue + 1 < SIZE )&&(*currentCellValue + 1 > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue + 1);
		}

	}
	if(*SensorRear < *Tolerance )
	{
		if((*currentCellValue + *Dimension < SIZE )&&(*currentCellValue + *Dimension > 0))
		{
	addAdj(graph,*currentCellValue, *currentCellValue + *Dimension);
		}

	}
break;

		case SOUTH:

	if(*SensorLeft < *Tolerance)
	{
		if((*currentCellValue + 1 < SIZE )&&(*currentCellValue + 1 > 0))
		{
			addAdj(graph,*currentCellValue, *currentCellValue + 1);
		}
	}
	if (*SensorFront < *Tolerance )
	{
		if((*currentCellValue + *Dimension < SIZE )&&(*currentCellValue + *Dimension > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue + *Dimension);
		}
	}
	if (*SensorRight < *Tolerance)
	{
	
		if((*currentCellValue - 1 < SIZE )&&(*currentCellValue - 1 > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue - 1);
		}
	}
	if(*SensorRear < *Tolerance )
	{
		if((*currentCellValue - *Dimension < SIZE )&&(*currentCellValue - *Dimension > 0))
		{
		addAdj(graph,*currentCellValue, *currentCellValue - *Dimension);
		}
	}
		break;

		case EAST:
		if(*SensorLeft < *Tolerance)
		{
			
			if((*currentCellValue - *Dimension < SIZE )&&(*currentCellValue - *Dimension > 0))
			{
			addAdj(graph,*currentCellValue, *currentCellValue - *Dimension);
			}
			
		}
		if (*SensorFront < *Tolerance)
		{
			
			if((*currentCellValue + 1 < SIZE )&&(*currentCellValue + 1 > 0))
			{
				addAdj(graph,*currentCellValue, *currentCellValue + 1);
			}
		}
		if (*SensorRight < *Tolerance)
		{
		
			if((*currentCellValue + *Dimension < SIZE )&&(*currentCellValue + *Dimension > 0))
					{
				addAdj(graph,*currentCellValue, *currentCellValue + *Dimension);
					}
		
		}
		if(*SensorRear < *Tolerance)
		{
			
			if((*currentCellValue - 1 < SIZE )&&(*currentCellValue - 1 > 0))
			{
			addAdj(graph,*currentCellValue, *currentCellValue - 1);
			}
			
		}
		break;

		case WEST:
		if(*SensorLeft < *Tolerance)
		{
			
			if((*currentCellValue + *Dimension < SIZE )&&(*currentCellValue + *Dimension > 0))
					{
				addAdj(graph,*currentCellValue, *currentCellValue + *Dimension);
					}
			
		}
		if (*SensorFront < *Tolerance)
		{
			
			if((*currentCellValue - 1 < SIZE )&&(*currentCellValue - 1 > 0))
			{
			addAdj(graph,*currentCellValue, *currentCellValue - 1);
			}
			
		}
		if (*SensorRight < *Tolerance)
		{
			if((*currentCellValue - *Dimension < SIZE )&&(*currentCellValue - *Dimension > 0))
			{
			addAdj(graph,*currentCellValue, *currentCellValue - *Dimension);

		}
		}
		if(*SensorRear < *Tolerance)
		{
			
				if((*currentCellValue + 1 < SIZE )&&(*currentCellValue + 1 > 0))
			      {
				addAdj(graph,*currentCellValue, *currentCellValue + 1);
			      }
			
		}
		break;

		}


}

int ReturnDirectionFromCellValue(unsigned short* currentCellValue, Cell** criticalPath, int* OrientationValue, int* Dimension)
{
	unsigned int next = 0;
	if (*criticalPath != NULL)
	{

		  pop(criticalPath);
		  next = peek(criticalPath);
		//printf("%d \n", next);
	}


	if (next != 0) //head is not NULL
	{
		//Orientation state machine
		switch (*OrientationValue)
		{

		case NORTH:

			if (*currentCellValue - 1 == next)
			{

				return LEFT;
			}
			else if (*currentCellValue - *Dimension == next)
			{

				return FORWARD;
			}
			else if (*currentCellValue + 1 == next)
			{

				return RIGHT;
			}
			else if (*currentCellValue + *Dimension == next)
			{
				return REVERSE;
			}
			break;

		case SOUTH:

			if (*currentCellValue + 1 == next)
			{

				return LEFT;
			}
			else if (*currentCellValue + *Dimension == next)
			{

				return FORWARD;
			}
			else if (*currentCellValue - 1 == next)
			{

				return RIGHT;
			}
			else if (*currentCellValue - *Dimension == next)
			{

				return REVERSE;
			}
			break;

		case EAST:
			if (*currentCellValue - *Dimension == next)
			{

				return LEFT;
			}
			else if (*currentCellValue + 1 == next)
			{

				return FORWARD;
			}
			else if (*currentCellValue + *Dimension == next)
			{

				return RIGHT;
			}
			else if (*currentCellValue - 1 == next)
			{

				return REVERSE;
			}
			break;

		case WEST:
			if (*currentCellValue + *Dimension == next)
			{

				return LEFT;
			}
			else if (*currentCellValue - 1 == next)
			{

				return FORWARD;
			}
			else if (*currentCellValue - *Dimension == next)
			{

				return RIGHT;
			}
			else if (*currentCellValue + 1 == next)
			{
				return REVERSE;
			}
			break;

		}


	}
	else
	{
		return 999;
	}

}
void updateCurrentCell(int* OrientationValue, unsigned short* currentCellValue, int* movement, int* Dimension)
{
	switch(*OrientationValue)
		{

		case NORTH:

			switch(*movement)
			{
			case LEFT:
				*currentCellValue = *currentCellValue - 1;
				break;
			case FORWARD:
				*currentCellValue = *currentCellValue - *Dimension;
				break;
			case RIGHT:
				*currentCellValue = *currentCellValue + 1;
				break;
			case REVERSE:
				*currentCellValue = *currentCellValue + *Dimension;
				break;
			}
	break;

			case SOUTH:
				switch(*movement)
				{
				case LEFT:
					*currentCellValue = *currentCellValue + 1;
					break;
				case FORWARD:
					*currentCellValue = *currentCellValue + *Dimension;
					break;
				case RIGHT:
					*currentCellValue = *currentCellValue - 1;
					break;
				case REVERSE:
					*currentCellValue = *currentCellValue - *Dimension;
					break;
				}


			break;

			case EAST:
				switch(*movement)
				{
				case LEFT:
					*currentCellValue = *currentCellValue - *Dimension;
					break;
				case FORWARD:
					*currentCellValue = *currentCellValue + 1;
					break;
				case RIGHT:
					*currentCellValue = *currentCellValue + *Dimension;
					break;
				case REVERSE:
					*currentCellValue = *currentCellValue - 1;
					break;
				}

			break;

			case WEST:
				switch(*movement)
				{
				case LEFT:
					*currentCellValue = *currentCellValue + *Dimension;
					break;
				case FORWARD:
					*currentCellValue = *currentCellValue - 1;
					break;
				case RIGHT:
					*currentCellValue = *currentCellValue - *Dimension;
					break;
				case REVERSE:
					*currentCellValue = *currentCellValue + 1;
					break;
				}

			break;

			}
}
void updateOrientation(int* OrientationValue, int* movement)
{
	switch(*OrientationValue)
	{

	case NORTH:
		switch(*movement)
		{
		case FORWARD:
			*OrientationValue = NORTH;
			break;
		case LEFT:
			*OrientationValue = WEST;
			break;
		case REVERSE:
			*OrientationValue = SOUTH;
			break;
		case RIGHT:
			*OrientationValue = EAST;
			break;

		}

          break;
		case SOUTH:
			switch(*movement)
				{
				case FORWARD:
					*OrientationValue = SOUTH;
					break;
				case LEFT:
					*OrientationValue = EAST;
					break;
				case REVERSE:
					*OrientationValue = NORTH;
					break;
				case RIGHT:
					*OrientationValue = WEST;
					break;

				}

			break;
		case EAST:
			switch(*movement)
				{
				case FORWARD:
					*OrientationValue = EAST;
					break;
				case LEFT:
					*OrientationValue = NORTH;
					break;
				case REVERSE:
					*OrientationValue = WEST;
					break;
				case RIGHT:
					*OrientationValue = SOUTH;
					break;

				}
			break;
		case WEST:
			switch(*movement)
				{
				case FORWARD:
					*OrientationValue = WEST;
					break;
				case LEFT:
					*OrientationValue = SOUTH;
					break;
				case REVERSE:
					*OrientationValue = EAST;
					break;
				case RIGHT:
					*OrientationValue = NORTH;
					break;

				}


              break;
		}
}


