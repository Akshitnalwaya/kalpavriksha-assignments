
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_MATRIX_SIZE 2
#define MAX_MATRIX_SIZE 10
#define MAX_PIXEL_INTENSITY 256

void swapValues(unsigned short int*, unsigned short int*);
void generateRandomMatrix(unsigned short int**, int);
void printMatrix(unsigned short int**, int);
void transposeMatrix(unsigned short int**, int);
void reverseRows(unsigned short int**, int);
void rotateMatrixClockwise(unsigned short int**, int);
void applySmoothingFilter(unsigned short int**, int);
void shiftMatrixLeft(unsigned short int**, int);
void clearUpperByte(unsigned short int**, int);

int main() {
    int matrixSize;

    do {
        printf("Enter matrix size [%d-%d]: ", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
        scanf("%d", &matrixSize);
        if (matrixSize < MIN_MATRIX_SIZE || matrixSize > MAX_MATRIX_SIZE) {
            printf("ERROR: Invalid Matrix Size. Please re-enter.\n");
        }
        getchar(); 
    } while (matrixSize < MIN_MATRIX_SIZE || matrixSize > MAX_MATRIX_SIZE);

    unsigned short int **pixelMatrix = (unsigned short int **)malloc(matrixSize * sizeof(unsigned short int *));
    if (!pixelMatrix) {
        printf("Memory allocation failed for matrix rows.\n");
        exit(1);
    }

    generateRandomMatrix(pixelMatrix, matrixSize);
    printf("\nOriginal Matrix:\n");
    printMatrix(pixelMatrix, matrixSize);

    rotateMatrixClockwise(pixelMatrix, matrixSize);
    printf("\nRotated Matrix:\n");
    printMatrix(pixelMatrix, matrixSize);

    shiftMatrixLeft(pixelMatrix, matrixSize);
    applySmoothingFilter(pixelMatrix, matrixSize);
    clearUpperByte(pixelMatrix, matrixSize);

    printf("\nFinal Matrix:\n");
    printMatrix(pixelMatrix, matrixSize);

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        free(*(pixelMatrix + rowIndex));
    }
    free(pixelMatrix);

    return 0;
}

void swapValues(unsigned short int *firstValue, unsigned short int *secondValue) {
    unsigned short int tempValue = *firstValue;
    *firstValue = *secondValue;
    *secondValue = tempValue;
}

void generateRandomMatrix(unsigned short int **pixelMatrix, int matrixSize) {
    srand((unsigned int)time(NULL));

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        *(pixelMatrix + rowIndex) = (unsigned short int *)malloc(matrixSize * sizeof(unsigned short int));
        if (!*(pixelMatrix + rowIndex)) {
            printf("Memory allocation failed for row %d.\n", rowIndex);
            exit(1);
        }
    }

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++) {
            *(*(pixelMatrix + rowIndex) + colIndex) = rand() % MAX_PIXEL_INTENSITY;
        }
    }
}

void printMatrix(unsigned short int **pixelMatrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++) {
            printf("%3d ", *(*(pixelMatrix + rowIndex) + colIndex));
        }
        printf("\n");
    }
}

void transposeMatrix(unsigned short int **pixelMatrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int colIndex = rowIndex + 1; colIndex < matrixSize; colIndex++) {
            unsigned short int *firstElement = *(pixelMatrix + rowIndex) + colIndex;
            unsigned short int *secondElement = *(pixelMatrix + colIndex) + rowIndex;
            swapValues(firstElement, secondElement);
        }
    }
}

void reverseRows(unsigned short int **pixelMatrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        unsigned short int *leftPointer = *(pixelMatrix + rowIndex);
        unsigned short int *rightPointer = *(pixelMatrix + rowIndex) + matrixSize - 1;
        while (leftPointer < rightPointer) {
            swapValues(leftPointer, rightPointer);
            leftPointer++;
            rightPointer--;
        }
    }
}

void rotateMatrixClockwise(unsigned short int **pixelMatrix, int matrixSize) {
    transposeMatrix(pixelMatrix, matrixSize);
    reverseRows(pixelMatrix, matrixSize);
}


void shiftMatrixLeft(unsigned short int **pixelMatrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++) {
            *(*(pixelMatrix + rowIndex) + colIndex) <<= 8;
        }
    }
}

void applySmoothingFilter(unsigned short int **pixelMatrix, int matrixSize) {
    for (int targetRow = 0; targetRow < matrixSize; targetRow++) {
        for (int targetCol = 0; targetCol < matrixSize; targetCol++) {
            int sumUpperBytes = 0;
            int neighborCounter = 0;

            for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                for (int colOffset = -1; colOffset <= 1; colOffset++) {
                    int neighborRow = targetRow + rowOffset;
                    int neighborCol = targetCol + colOffset;

                    if (neighborRow >= 0 && neighborRow < matrixSize &&
                        neighborCol >= 0 && neighborCol < matrixSize) {

                        int upperByte = *(*(pixelMatrix + neighborRow) + neighborCol) >> 8;
                        sumUpperBytes += upperByte;
                        neighborCounter++;
                    }
                }
            }

            if (neighborCounter > 0) {
                sumUpperBytes /= neighborCounter;
            }

            *(*(pixelMatrix + targetRow) + targetCol) += sumUpperBytes;
        }
    }
}

void clearUpperByte(unsigned short int **pixelMatrix, int matrixSize) {
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++) {
        for (int colIndex = 0; colIndex < matrixSize; colIndex++) {
            *(*(pixelMatrix + rowIndex) + colIndex) <<= 8;
            *(*(pixelMatrix + rowIndex) + colIndex) >>= 8;
        }
    }
}
