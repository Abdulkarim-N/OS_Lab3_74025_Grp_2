#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define NUM_THREADS 27

int sudoku[9][9] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

bool row_valid[9] = {false};
bool col_valid[9] = {false};
bool subgrid_valid[9] = {false};

void* checkRow(void* param) {
    int row = *(int*)param;
    bool numbers[10] = {false};
    for (int i = 0; i < 9; i++) {
        int num = sudoku[row][i];
        if (numbers[num]) return NULL;
        numbers[num] = true;
    }
    row_valid[row] = true;
    return NULL;
}

void* checkColumn(void* param) {
    int col = *(int*)param;
    bool numbers[10] = {false};
    for (int i = 0; i < 9; i++) {
        int num = sudoku[i][col];
        if (numbers[num]) return NULL;
        numbers[num] = true;
    }
    col_valid[col] = true;
    return NULL;
}

void* checkSubGrid(void* param) {
    int subgrid = *(int*)param;
    int startRow = (subgrid / 3) * 3;
    int startCol = (subgrid % 3) * 3;
    bool numbers[10] = {false};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = sudoku[startRow + i][startCol + j];
            if (numbers[num]) return NULL;
            numbers[num] = true;
        }
    }
    subgrid_valid[subgrid] = true;
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int indices[9];

    for (int i = 0; i < 9; i++) { // just combined the creation of the threads to make it neater as each individual loop always went to 9, and just adding 9 to each column and 18 to each subgrid makes it the same (logic taken from the given sample code)
        indices[i] = i;
        pthread_create(&threads[i], NULL, checkRow, &indices[i]);
        pthread_create(&threads[i + 9], NULL, checkColumn, &indices[i]);
        pthread_create(&threads[i + 18], NULL, checkSubGrid, &indices[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) { //waiting for all the threads to complete
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 9; i++) {
        if (!row_valid[i] || !col_valid[i] || !subgrid_valid[i]) { //validation logic, if any one of the rows columns or subgrids are invalid, the entire grid is invalid.
            printf("Invalid Sudoku Solution\n");
            return 0;
        }
    }

    printf("Valid Sudoku Solution\n"); //if everything is valid, it will print valid solution and exit the program
    return 0;
}
