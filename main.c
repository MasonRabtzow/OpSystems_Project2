#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int board[9][9]; // Sudoku board
int result[27]; // Stores result for rows, columns, and 3x3 boxes

typedef struct {
    int row;
    int columnl;
    int index;
} parameters;