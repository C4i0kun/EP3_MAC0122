#include <stdio.h>
#include <stdlib.h>

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
int* g_work_array;


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

void change_array_positions(int *array, int pos_a, int pos_b) {
    int tmp = array[pos_a];
    array[pos_a] = array[pos_b];
    array[pos_b] = tmp;
}

void print_result() {
    for (int i = 0; i < g_number_of_works; i++) {
        printf("(T:%d F:%d) ", i, g_work_array[i]);
    }
    
    printf("= ");
    printf("%d\n", g_cost);
}

void free_matrix(int **matrix, int lines, int columns) {
    for (int i = 0; i < lines; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void free_array(int *array) {
    free(array);
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

void minimize_cost(int** C, int** R, int* work_array, int index) {
    int i = 0;
    int cost = 0;

    if (index == g_number_of_works-1) {
        for (int j = 0; j < g_number_of_works; j++) {
            cost += C[work_array[j]][j];
        }
        if (g_cost == 0) {
            g_cost = cost;
            for (int k = 0; k < g_number_of_works; k++) {
                g_work_array[k] = work_array[k];
            }
        } else {
            if (cost < g_cost) {
                g_cost = cost;
                for (int k = 0; k < g_number_of_works; k++) {
                    g_work_array[k] = work_array[k];
                }
            }
        }

    } else {
        for (i = index; i < g_number_of_works; i++) {
            change_array_positions(work_array, i, index);
            minimize_cost(C, R, work_array, index + 1);
            change_array_positions(work_array, i, index);
        }
    }


}

int valid_combination(int* work_array, int** R) {
    for (int i = 0; i < g_number_of_works; i++) {
        for (int j = i + 1; (j < g_number_of_works) && (j > i); j++) {
            if(R[work_array[i]][work_array[j]] == 1) {
                return 0;
            }
        }
    }

    return 1;
}

void all_combinations(int** C, int** R, int* people_array, int* work_array, int start, int end, int index, int comb_size) {
    if (index == comb_size) {
        if (valid_combination(work_array, R) == 1) {
            minimize_cost(C, R, work_array, 0);
        }
        return;
    }

    for (int j = start; (j < end) && ((end - j + 1) >= (comb_size - index)); j++) {
        work_array[index] = people_array[j];
        all_combinations(C, R, people_array, work_array, j+1, end, index+1, comb_size);
    }
}

/**************************************************/
/********** MATRIX DESTRUCTION FUNCTIONS **********/
/**************************************************/

void destroy_c_matrix(int** c_matrix) {
    free_matrix(c_matrix, g_number_of_people, g_number_of_works);
}

void destroy_r_matrix(int** r_matrix) {
    free_matrix(r_matrix, g_number_of_people, g_number_of_people);
}

void destroy_work_array(int* work_array) {
    free_array(work_array);
}

void destroy_people_array(int* people_array) {
    free_array(people_array);
}

void destroy_all(int **c_matrix, int** r_matrix, int* work_array, int* g_work_array, int* people_array) {
    destroy_c_matrix(c_matrix);
    destroy_r_matrix(r_matrix);
    destroy_work_array(work_array);
    destroy_work_array(g_work_array);
    destroy_people_array(people_array);
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

    g_work_array = create_work_array(g_number_of_works);

    all_combinations(C, R, people_array, work_array, 0, g_number_of_people, 0, g_number_of_works);
    print_result();

    destroy_all(C, R, work_array, g_work_array, people_array);

    return 0;
}