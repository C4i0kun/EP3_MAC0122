#include <stdio.h>
#include <Stdlib.h>

typedef enum {
    SIZE_OF_MATRIX,
    C_MATRIX,
    R_MATRIX
} reading_mode; 


/**************************************************/
/*************** USEFUL FUNCTIONS *****************/
/**************************************************/

int char_to_int(char c) {
    int integer = c - '0';
    return integer;
}

int** allocate_matrix(int lines, int columns) {
    int **array = (int **)malloc(lines * sizeof(int *));
    for (int i = 0; i < lines; i++) {
        array[i] = (int *)malloc(columns * sizeof(int));
    }
    return array;
}


/**************************************************/
/********** MATRIX CREATION FUNCTIONS *************/
/**************************************************/

int get_number_of_lines(char file[]) {
    FILE *fp;
    int number_of_lines;

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Erro de leitura!\n");
        exit(-1);
    }

    fscanf(fp, "%d", &number_of_lines);
    fclose(fp);

    return number_of_lines;
}

int get_number_of_columns(char file[]) {
    FILE *fp;
    char symbol_read;
    int number_of_columns;

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Erro de leitura!\n");
        exit(-1);
    }
    
    do {
        fscanf(fp, "%c", &symbol_read);
    } while (symbol_read != ' ');

    fscanf(fp, "%d", &number_of_columns);
    fclose(fp);

    return number_of_columns;    
}

int** create_c_matrix (char file[], int lines, int columns) {
    FILE *fp;
    char symbol_read;
    int value;

    int newlines_read = 0;
    int newlines_to_read = 2;

    int** c_matrix = allocate_matrix(lines, columns);

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Erro de leitura!\n");
        exit(-1);
    }

    do {
        fscanf(fp, "%c", &symbol_read);
        if (symbol_read == '\n') {
            newlines_read++;
        }
    } while (newlines_read < newlines_to_read);

    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < columns; j++) {
            fscanf(fp, "%d", &value);
            fseek(fp, 1, SEEK_CUR);
            c_matrix[i][j] = value;
        }
    }

    return c_matrix;
}

int** create_r_matrix (char file[], int size) {
    FILE *fp;
    char symbol_read;
    int value;

    int newlines_read = 0;
    int newlines_to_read = 2 + size + 1;

    int** r_matrix = allocate_matrix(size, size);

    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Erro de leitura!\n");
        exit(-1);
    }

    do {
        fscanf(fp, "%c", &symbol_read);
        if (symbol_read == '\n') {
            newlines_read++;
        }
    } while (newlines_read < newlines_to_read);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fscanf(fp, "%d", &value);
            fseek(fp, 1, SEEK_CUR);
            r_matrix[i][j] = value;
        }
    }

    return r_matrix;
}


/**************************************************/
/********** MATRIX COMPARISON FUNCTIONS ***********/
/**************************************************/




/**************************************************/
/********************* MAIN ***********************/
/**************************************************/

int main() {
    char file[100];
    printf("Entre com o nome do arquivo a ser analisado: \n");
    scanf("%s", file);

    int lines = get_number_of_lines(file);
    int columns = get_number_of_columns(file);
    int **C = create_c_matrix(file, lines, columns)
    int **R = create_r_matrix(file, lines);



    return 0;
}