/** David Amador & Mason Rabtzow
 CSC345-01
 Project 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> // For runtime output

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

    //checks each row by iterating through each row and checking for 1-9 and ducplicates
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

    //iterates through each column and checks for 1-9 and duplicates
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

    //iterates through 1 3x3 box and checks for 1-9 and duplicates
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

    //checks 1 row for 1-9 and duplicates
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

    //checks 1 column for 1-9 and duplicates
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
    int i = 0;

    int mode = atoi(argv[1]);

    // Read board from file
    for(i = 0; i < 81; i++){
        fscanf(f, "%d", &board[i / 9][i % 9]);
    }
    int t = 0;

    fclose(f);

    //Print board
    printf("BOARD STATE IN input.txt:\n");
    for(i = 0; i < 81; i++){
        printf("%d ", board[i / 9][i % 9]);
        if((i + 1) % 9 == 0){
            printf("\n");
        }
    }

    // Start system timer
    clock_t start = clock();

    //Mode 1: 11 threads (1 for rows, 1 for columns, 9 for 3x3 boxes)
    if(mode == 1){
        //create threads for rows, columns, and 3x3 boxes
        pthread_t threads[11];
        pthread_create(&threads[t], NULL, check_rows, NULL);
        t++;
        pthread_create(&threads[t], NULL, check_cols, NULL);
        t++;

        for(int r = 0; r < 9; r += 3){
            for(int c = 0; c < 9; c += 3){
                //make struct for 3x3 box parameters and create thread
                params *p = malloc(sizeof(params));
                p->row = r;
                p->col = c;
                p->index = t;

                pthread_create(&threads[t], NULL, check_3x3, p);
                t++;
            }
        }

        //join all threads
        for(i = 0; i < 11; i++){
            pthread_join(threads[i], NULL);
        }

    //Mode 2: 27 threads (9 for rows, 9 for columns, 9 for 3x3 boxes)
    }else if(mode == 2){
        pthread_t threads[27];

        //make struct for each row
        for(i = 0; i < 9; i++){
            params *p_row = (params*) malloc(sizeof(params));
            p_row->row = i;
            p_row->index = t;
            pthread_create(&threads[t], NULL, check_row_thread, (void*) p_row);
            t++;
        }

        //make struct for each column
        for(i = 0; i < 9; i++){
            params *p_col = (params*) malloc(sizeof(params));
            p_col->col = i;
            p_col->index = t;
            pthread_create(&threads[t], NULL, check_col_thread, (void*) p_col);
            t++;
        }

        //make struct for each 3x3 box
        for(int r = 0; r < 9; r += 3){
            for(int c = 0; c < 9; c += 3){
                params *p = malloc(sizeof(params));
                p->row = r;
                p->col = c;
                p->index = t;

                pthread_create(&threads[t], NULL, check_3x3, p);
                t++;
            }
        }

        //join all threads
        for(i = 0; i < 27; i++){
            pthread_join(threads[i], NULL);
        }

    //if mode is not 1 or 2, print error and exit
    }else{
        printf("Invalid mode. Use 1 for full board check or 2 for row/column check.\n");
        fclose(f);
        return 1;
    }

    // Confirm result
    int correct = 1;

    for (int i = 0; i < t; i++){
        if(result[i] == 0){
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
