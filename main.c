#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int board[9][9]; // Sudoku board
int result[27]; // Stores result for rows, columns, and 3x3 boxes


typedef struct _params {
    int row;
    int col;
    int index;
} params;

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
void *check_cols(void *param)
{
    int i;
    int j;

    for (i = 0; i < 9; i++) {
        int check[10] = {0};

        for (j = 0; j < 9; j++) {
            int num = board[j][i];
            //check if valid
            if (num < 1 || num > 9 || check[num]) {
                result[1] = 0;
                pthread_exit(0);
            }
            check[num] = 1;
        }
    }

    result[1] = 1; // All cols = valid
    pthread_exit(0);
}

// Check 3x3
void *check_3x3(void *param)
{
    params *p = (params*) param;

    int row = p->row;
    int col = p->col;
    int index = p->index;

    int check[10] = {0};

    for (int i = row; i < row + 3; i++) {
        
        for (int j = col; j < col + 3; j++) {
            int num = board[i][j];
            //check if valid
            if (num < 1 || num > 9 || check[num]) {
                result[index] = 0;
                pthread_exit(0);
            }
            
            check[num] = 1;
        }
    }

    result[index] = 1; // 3x3 box = valid
    pthread_exit(0);
}

// Check 1 Row (Mode 2)
void *check_row_thread(void *param)
{
    params *p = (params*) param;

    int row = p->row;
    int index = p->index;

    int check[10] = {0};

    for (int j = 0; j < 9; j++) {
        int num = board[row][j];
        //check if valid
        if (num < 1 || num > 9 || check[num]) {
            result[index] = 0;
            pthread_exit(0);
        }
        check[num] = 1;
    }

    result[index] = 1; // Row = valid
    pthread_exit(0);
}

// Check 1 Column (Mode 2)
void *check_col_thread(void *param)
{
    params *p = (params*) param;

    int col = p->col;
    int index = p->index;

    int check[10] = {0};

    for (int i = 0; i < 9; i++) {
        int num = board[i][col];
        //check if valid
        if (num < 1 || num > 9 || check[num]) {
            result[index] = 0;
            pthread_exit(0);
        }
        check[num] = 1;
    }

    result[index] = 1; // Column = valid
    pthread_exit(0);
}

// Main
int main(int argc, char *argv[]){
    FILE *f;
    f = fopen("input.txt", "r");
    int nums[81];
    int num, i = 0;

    int mode = atoi(argv[1]);

    for(i = 0; i < 81; i++){
        fscanf(f, "%d", &board[i / 9][i % 9]);
    }
    int t = 0;

    fclose(f);

    printf("BOARD STATE IN input.txt:\n");
    for(i = 0; i < 81; i++){
        printf("%d ", board[i / 9][i % 9]);
        if((i + 1) % 9 == 0){
            printf("\n");
        }
    }
    // Start system timer
    clock_t start = clock();









    // Confirm result
    int correct = 1;

    for (int i = 0; i < t; i++) {
        if (result[i] == 0) {
            correct = 0;
            break;
        }
    }
    
    // End timer and print time taken and result
    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    if (correct)
        printf("SOLUTION: YES (%.4f seconds)\n", time_spent);
    else
        printf("SOLUTION: NO (%.4f seconds)\n", time_spent);

    return 0;
}

