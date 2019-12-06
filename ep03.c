#include <stdio.h>
#include <Stdlib.h>

typedef enum {
    SIZE_OF_MATRIX,
    C_MATRIX,
    R_MATRIX
} reading_mode; 

/**************************************************/
/*************** GLOBAL VARIABLES *****************/
/**************************************************/

int g_cost = 0;
int g_number_of_works = 0;
int g_number_of_people = 0;


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

int* allocate_array(int size) {
    int *array = (int *)malloc(size * sizeof(int));
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

int* create_work_array(int size) {
    int* array = allocate_array(size);
    return array;
}

int* create_people_array(int size) {
    int *array = allocate_array(size);

    for (int i = 0; i < size; i++) {
        array[i] = i;
    }

    return array;
}


/**************************************************/
/********** MATRIX COMPARISON FUNCTIONS ***********/
/**************************************************/

/* Returns 1 if the work restriction is accomplished */
int restriction_accomplished(int line, int column, int **R) {
    if (R[line-1][column-1] == 1) {
        return 0;
    }

    return 1;
}

void all_combinations(int* people_array, int* work_array, int start, int end, int index, int comb_size) {
    if (index == comb_size) {
        for (int i = 0; i < comb_size; i++) {
            printf("%d ", work_array[i]);
        }
        printf("\n");
        return;
    }

    for (int j = start; (j < end) && ((end - j + 1) >= (comb_size - index)); j++) {
        work_array[index] = people_array[j];
        all_combinations(people_array, work_array, j+1, end, index+1, comb_size);
    }
}


/**************************************************/
/********************* MAIN ***********************/
/**************************************************/

int main() {
    char file[100];
    printf("Entre com o nome do arquivo a ser analisado: \n");
    scanf("%s", file);

    int lines = get_number_of_lines(file);
    int columns = get_number_of_columns(file);

    g_number_of_people = lines;   
    g_number_of_works = columns;

    int **C = create_c_matrix(file, lines, columns);
    int **R = create_r_matrix(file, lines);
    int *work_array = create_work_array(g_number_of_works);
    int *people_array = create_people_array(g_number_of_people);

    printf("%d\n", restriction_accomplished(3,1, R));
    all_combinations(people_array, work_array, 0, g_number_of_people, 0, g_number_of_works);

    return 0;
}