/*
Escribir tres funciones que se encarguen de revisar 
que todos los numeros del uno al nueve esten>

1. En cada columna de un arreglo de 9x9.
2. En cada fila de un arreglo de un arreglo de 9x9.
3. En un subarreglo de 3x3 dentro de un arreglo de 9x9.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

int matrix[9][9];

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


//int checkColumn(int arr[][9], int col) {
    /*
        Este metodo revisa que los numeros esten en una 
        columna especifica del arreglo que se le pasa.
    */
//    int nums[10] = {0};
//    for (int i = 0; i < 9; i++) {
//        if (nums[arr[i][col]] == 1) {
//            return 0;
//        }
//        nums[arr[i][col]] = 1;
//    }
//    return 1;
//}

void *checkColumn(void *arg) {
    /*
        Este metodo revisa que los numeros esten en una 
        columna especifica del arreglo que se le pasa.
    */
    int (*arr)[9] = matrix;
    int i,j,k;
    int valido = 1;

    for (j = 0; j < 9; j++) {
        int nums[9] = {0};
        
        for (i = 0; i < 9; i++) {
            k = arr[i][j] - 1;
            if (nums[k] == 1) {
                valido = 0;
                break;
            }

            nums[k] = 1;
        }
    }

    if (valido == 1) {
        printf("El sudoku es valido\n");
    } else {
        printf("El sudoku no es valido\n");
    }

    return NULL;
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

// Funcion para los procesos y algunos threads.

int threadss() {

        // Este metodo se encarga de crear los procesos y threads
        // necesarios para validar el sudoku.
    // Imprimiendo el PID del proceso padre.

    int parent_pid = getpid();
    printf("Estoy en el proceso padre con PID=%d\n", parent_pid);


    int pid = fork();

    if (pid == 0) { // proceso hijo
        char parent_pid_str[10];
        sprintf(parent_pid_str, "%d", parent_pid);
        execlp("ps", "ps", "-p", parent_pid_str, "-lLf", NULL);

        // Ejecutando un fork para crear un proceso hijo.
        pid_t pid = fork();

        // Esperando a que el proceso hijo termine.
        wait(NULL);

        return 0;
    }
    
    if (pid > 0) { // proceso padre
        //wait(NULL);
        pthread_t thread;
        //pthread_create(&thread, NULL, checkColumn(), NULL);
        // Creando un thread que revisa las columnas.
        pthread_create(&thread, NULL, checkColumn, matrix);

        pthread_join(thread, NULL); // Haciendo un join al thread.

        // Obteniendo el thread en ejecucion y desplegandolo.
        pid_t tid = syscall(SYS_gettid);
        printf("El thread con ID=%d esta en ejecucion\n", (long) tid);

        wait(NULL); // Haciendo un wait al proceso hijo.

        // Revisando las filas.
        for (int i = 0; i < 9; i++) {
            if (checkRow(matrix, i) == 0) {
                printf("El sudoku no es valido\n");
                return 0;
            }else {
                printf("El sudoku es valido\n");
                return 1;
            }
        }
    }
    
    return 0;
}


// Funcion principal.

int main() {

    /*
        Arreglo temporal para revisar que los 
        metodos esten bien. 
    */

    // Abriendo el archivo.
    int fd; // File descriptor.
    struct stat sb; // Informacion del archivo.
    char *addr; // Apuntador al archivo.


    fd = open("sudoku", O_RDONLY);
    if (fd == -1) { // Error al abrir el archivo.
        perror("Hubo un error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &sb) == -1) { // Error al leer el archivo.
        perror("Error al obtener la informacion del archivo");
        exit(EXIT_FAILURE);
    }

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) { // Mappeo fallido.
        perror("Error al mapear el archivo");
        exit(EXIT_FAILURE);
    }

    // Copiando el archivo a un arreglo de 9x9.
    int index = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matrix[i][j] = addr[index] - '0';
            index++;
        }
    }

    // Imprimiendo el arreglo.
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Cerrando el archivo.
    close(fd);

    // Llamando al metodo threadss.
    threadss();

    /*
    // Validando el array.
    int valid = 1;
    for (int i = 0; i < 9; i++) {
        valid = valid && checkRow(matrix, i) && checkColumn(matrix, i);
    }
    for (int i = 0; i < 9; i += 3) {
        for (int j = 0; j < 9; j += 3) {
            valid = valid && checkSubgrid(matrix, i, j);
        }
    }
     if (valid) {
        printf("El sudoku es válido\n");
    } else {
        printf("El sudoku es inválido\n");
    }*/


    return 0;
}
