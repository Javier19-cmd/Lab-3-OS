/*
Escribir tres funciones que se encarguen de revisar 
que todos los numeros del uno al nueve esten>

1. En cada columna de un arreglo de 9x9.
2. En cada fila de un arreglo de un arreglo de 9x9.
3. En un subarreglo de 3x3 dentro de un arreglo de 9x9.
*/

#include <stdio.h>

#define N 9

int arr[9][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {4, 5, 6, 7, 8, 9, 1, 2, 3},
        {7, 8, 9, 1, 2, 3, 4, 5, 6},
        {2, 3, 4, 5, 6, 7, 8, 9, 1},
        {5, 6, 7, 8, 9, 1, 2, 3, 4},
        {8, 9, 1, 2, 3, 4, 5, 6, 7},
        {3, 4, 5, 6, 7, 8, 9, 1, 2},
        {6, 7, 8, 9, 1, 2, 3, 4, 5},
        {9, 1, 2, 3, 4, 5, 6, 7, 8}
    };

int checkRow(int arr[][9], int row) {
    /*
        Este metodo revisa si los numeros del 1 al 9
        estan en una fila especifica del arreglo.
    */
    int nums[10] = {0};
    for (int i = 0; i < 9; i++) {
        if (nums[arr[row][i]] == 1) {
            return 0;
        }
        nums[arr[row][i]] = 1;
    }
    return 1;
}

int checkColumn(int arr[][9], int col) {
    /*
        Este metodo revisa que los numeros esten en una 
        columna especifica del arreglo que se le pasa.
    */
    int nums[10] = {0};
    for (int i = 0; i < 9; i++) {
        if (nums[arr[i][col]] == 1) {
            return 0;
        }
        nums[arr[i][col]] = 1;
    }
    return 1;
}

int checkSubgrid(int arr[][9], int row, int col) {
    /*
        Este funcion revisa que los numeros del 1 al 9
        esten en un subarreglo de 3x3 dentro del arreglo de 
        9x9. Aqui se reciben dos parametros, row y col, los cuales
        corresponden a la fila y columna de la esquina superior izquierda
        del subarreglo que se esta revisando.
    */
    int nums[10] = {0};
    for (int i = row; i < row + 3; i++) {
        for (int j = col; j < col + 3; j++) {
            if (nums[arr[i][j]] == 1) {
                return 0;
            }
            nums[arr[i][j]] = 1;
        }
    }
    return 1;
}

int main() {

    /*
        Arreglo temporal para revisar que los 
        metodos esten bien. 
    */

    

    // Validando el array.
    int valid = 1;
    for (int i = 0; i < 9; i++) {
        valid = valid && checkRow(arr, i) && checkColumn(arr, i);
    }
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            valid = valid && checkSubgrid(arr, i, j);
        }
    }
     if (valid) {
        printf("El sudoku es válido\n");
    } else {
        printf("El sudoku es inválido\n");
    }

    return 0;
}
