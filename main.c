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

// Check Rows
void *check_rows(void *param)
{
    int i;
    int j;

    for (i = 0; i < 9; i++) {
        int seen[10] = {0};

        for (j = 0; j < 9; j++) {
            int num = board[i][j];
            //check if valid
            if (num < 1 || num > 9 || seen[num]) {
                result[0] = 0;
                pthread_exit(0);
            }
            seen[num] = 1;
        }
    }

    result[0] = 1; // All rows = valid
    pthread_exit(0);
}

// Check Columns


// Check 3x3


// Check 1 Row


// Check 1 Column


// Main


