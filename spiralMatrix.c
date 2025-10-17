#include <stdio.h>
#define EMPTY_CELL -1
int count = 0;
int row = 0, column = 0;

void moveRight(int numberOfCells, int *spiralMatrix);
void moveDown(int numberOfCells, int *spiralMatrix);
void moveLeft(int numberOfCells, int *spiralMatrix);
void moveUp(int numberOfCells, int *spiralMatrix);
void fillSpiral(int numberOfCells, int *spiralMatrix);

void moveRight(int numberOfCells, int *spiralMatrix) 
{
    if (count == numberOfCells * numberOfCells) 
    {
        return;
    }
    
    while (column < numberOfCells && *(spiralMatrix + row * numberOfCells + column) == EMPTY_CELL) 
    {
        count++;
        *(spiralMatrix + row * numberOfCells + column) = count;
        column++;
    }
    column--;
    row++;
    if (row < numberOfCells) 
    {
        moveDown(numberOfCells, spiralMatrix);
    }
}

void moveDown(int numberOfCells, int *spiralMatrix) 
{
    if (count == numberOfCells * numberOfCells) 
    {
        return;
    }
    
    while (row < numberOfCells && *(spiralMatrix + row * numberOfCells + column) == EMPTY_CELL) 
    {
        count++;
        *(spiralMatrix + row * numberOfCells + column) = count;
        row++;
    }
    row--;
    column--;
    if (column >= 0) 
    {
        moveLeft(numberOfCells, spiralMatrix);
    }
}

void moveLeft(int numberOfCells, int *spiralMatrix) 
{
    if (count == numberOfCells * numberOfCells) 
    {
        return;
    }
    
    while (column >= 0 && *(spiralMatrix + row * numberOfCells + column) == EMPTY_CELL) 
    {
        count++;
        *(spiralMatrix + row * numberOfCells + column) = count;
        column--;
    }
    column++;
    row--;
    if (row >= 0) 
    {
        moveUp(numberOfCells, spiralMatrix);
    }
}

void moveUp(int numberOfCells, int *spiralMatrix) 
{
    if (count == numberOfCells * numberOfCells) 
    {
        return;
    }
    
    while (row >= 0 && *(spiralMatrix + row * numberOfCells + column) == EMPTY_CELL) 
    {
        count++;
        *(spiralMatrix + row * numberOfCells + column) = count;
        row--;
    }
    row++;
    column++;
    if (column < numberOfCells) 
    {
        moveRight(numberOfCells, spiralMatrix);
    }
}

void fillSpiral(int numberOfCells, int *spiralMatrix) 
{
    moveRight(numberOfCells, spiralMatrix);
}

int main() 
{
    int numberOfCells;
    printf("Enter size of the square matrix: ");
    if (scanf("%d", &numberOfCells) != 1 || numberOfCells <= 0) 
    {
        printf("Invalid input Please enter a positive integer.\n");
        return 1;
    }

    int spiralMatrix[numberOfCells][numberOfCells];
    for (int i = 0; i < numberOfCells; i++) 
    {
        for (int j = 0; j < numberOfCells; j++) 
        {
            spiralMatrix[i][j] = EMPTY_CELL;
        }
    }

    fillSpiral(numberOfCells, (int*)spiralMatrix);

    for (int i = 0; i < numberOfCells; i++) 
    {
        for (int j = 0; j < numberOfCells; j++) 
        {
            printf("%d ", spiralMatrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}
