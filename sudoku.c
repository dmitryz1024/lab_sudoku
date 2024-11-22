#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>

// Размеры полей
#define SIZE_4 4
#define SIZE_9 9

// Задержка для брутфорса (в мс)
#define DELAY 2000

// Псевдонимы размерностей полей судоку
typedef int Sudoku4x4[SIZE_4][SIZE_4];
typedef int Sudoku9x9[SIZE_9][SIZE_9];

// Функция очистки терминала
void clear_terminal() {
    printf("\033[H\033[J");
}

// Функция для перемещения курсора
void set_matrix_position(int row, int col) {
    printf("\033[%d;%dH", row + 1, (col + 1) * 2 - 1 );
}

// Функция для вывода игрового поля
void print_board(int size, int (*board)[size], int (*original_board)[size]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (*(*(board + i) + j) == 0) {
                printf(". ");
            } else {
                if (*(*(original_board + i) + j) != 0){
                    printf("%d ", *(*(board + i) + j));
                } else{
                    printf("\033[32m%d \033[0m", *(*(board + i) + j));
                }
            }
        }
        printf("\n");
    }
}

// Проверка, можно ли поставить число в ячейку
int is_valid(int size, int (*board)[size], int row, int col, int num) {
    // Проверяем строки и столбцы
    for (int i = 0; i < size; i++) {
        if (*(*(board + row) + i) == num || *(*(board + i) + col) == num) {
            return 0;  // Число уже есть в строке или столбце
        }
    }
    // Для поля 9x9 или 4x4 размер внутреннего квадрата
    int box_size = (size == 9) ? 3 : 2;  // Для 9x9 это 3, для 4x4 это 2
    int box_row = row / box_size * box_size;  // Начало блока по строкам
    int box_col = col / box_size * box_size;  // Начало блока по столбцам
    // Проверка чисел внутри блока
    for (int i = 0; i < box_size; i++) {
        for (int j = 0; j < box_size; j++) {
            if (*(*(board + box_row + i) + box_col + j) == num) {
                return 0;  // Число уже есть в блоке
            }
        }
    }
    return 1;  // Число можно поставить
}

// Функция для проверки решения
int check_solution(int size, int (*board)[size], int (*original_board)[size]) {
    // Переменные для отслеживания суммы
    int last_sum = 0, current_sum = 0;
    // Проверка сумм по строкам
    for (int row = 0; row < size; row++) {
        current_sum = 0;
        for (int col = 0; col < size; col++) {
            current_sum += *(*(board + row) + col);
        }
        if (current_sum != last_sum && row != 0) {
            return 0; // Суммы не совпадают
        }
        last_sum = current_sum; // Обновляем last_sum для следующей строки
    }
    // Проверка сумм по столбцам
    for (int col = 0; col < size; col++) {
        current_sum = 0;
        for (int row = 0; row < size; row++) {
            current_sum += *(*(board + row) + col);
        }
        if (current_sum != last_sum) {
            return 0; // Суммы не совпадают
        }
        last_sum = current_sum; // Обновляем last_sum для следующего столбца
    }
    // Для поля 9x9 или 4x4 размер внутреннего квадрата
    int box_size = (size == 9) ? 3 : 2;  // Для 9x9 это 3, для 4x4 это 2
    // Проверка сумм по блокам
    for (int box_row = 0; box_row < size; box_row += box_size) {
        for (int box_col = 0; box_col < size; box_col += box_size) {
            current_sum = 0;
            // Перебор элементов внутри блока
            for (int i = 0; i < box_size; i++) {
                for (int j = 0; j < box_size; j++) {
                    current_sum += *(*(board + box_row + i) + box_col + j);
                }
            }
            if (current_sum != last_sum) {
                return 0; // Суммы не совпадают
            }
            last_sum = current_sum; // Обновляем last_sum для следующего блока
        }
    }
    // Если все суммы совпали
    return 1;
}

// Основная функция
int main() {
    int flag = 1;
    while (flag) {
        int choice, mode;
        int row = 0, col = 0;
        // Выбор размерности поля
        clear_terminal();
        printf("Choose the board size (1 - 4x4, 2 - 9x9): ");
        scanf("%d", &choice);
        int size = (choice == 1) ? SIZE_4 : SIZE_9;
        // Динамическое выделение памяти под поле
        int (*original_board)[size] = (int (*)[size])malloc(size * sizeof(int[size]));
        int (*board)[size] = (int (*)[size])malloc(size * sizeof(int[size]));
        // Изначальные поля для игры
        Sudoku9x9 puzzle9x9 = {
            {1, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 7, 2, 6, 0, 4, 8, 0},
            {4, 0, 0, 9, 3, 5, 0, 0, 6},
            {0, 3, 0, 4, 8, 0, 2, 0, 0},
            {0, 4, 1, 6, 0, 9, 3, 0, 0},
            {0, 0, 6, 0, 0, 0, 8, 9, 0},
            {5, 7, 8, 0, 4, 0, 0, 0, 2},
            {0, 0, 0, 3, 0, 0, 0, 7, 0},
            {2, 0, 0, 0, 0, 0, 0, 0, 5}
        };
        Sudoku4x4 puzzle4x4 = {
            {0, 0, 0, 3},
            {4, 0, 0, 0},
            {0, 1, 3, 0},
            {3, 0, 2, 0}
        };
        // Изначальные поля для проверки
        Sudoku9x9 original9x9 = {
            {1, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 7, 2, 6, 0, 4, 8, 0},
            {4, 0, 0, 9, 3, 5, 0, 0, 6},
            {0, 3, 0, 4, 8, 0, 2, 0, 0},
            {0, 4, 1, 6, 0, 9, 3, 0, 0},
            {0, 0, 6, 0, 0, 0, 8, 9, 0},
            {5, 7, 8, 0, 4, 0, 0, 0, 2},
            {0, 0, 0, 3, 0, 0, 0, 7, 0},
            {2, 0, 0, 0, 0, 0, 0, 0, 5}
        };
        Sudoku4x4 original4x4 = {
            {0, 0, 0, 3},
            {4, 0, 0, 0},
            {0, 1, 3, 0},
            {3, 0, 2, 0}
        };
        // Инициализация игрового поля
        if (choice == 1) {
            memcpy(original_board, original4x4, sizeof(Sudoku4x4));
            memcpy(board, puzzle4x4, sizeof(Sudoku4x4));
        } else {
            memcpy(original_board, original9x9, sizeof(Sudoku9x9));
            memcpy(board, puzzle9x9, sizeof(Sudoku9x9));
        }
        // Вывод изначального поля
        clear_terminal();
        print_board(size, board, original_board);
        // Выбор режима
        printf("Choose the mode (1 - Manual, 2 - Auto): ");
        scanf("%d", &mode);
        if (mode == 1) {
            char key;
            while (1) {
                clear_terminal();
                print_board(size, board, original_board);
                set_matrix_position(row, col);
                printf("[%c]", *(*(board + row) + col) == 0 ? '.' : *(*(board + row) + col) + '0');
                key = _getch();  // Считываем символ с клавиатуры
                if (key == 'w' && row > 0) row--;
                if (key == 's' && row < size - 1) row++;
                if (key == 'a' && col > 0) col--;
                if (key == 'd' && col < size - 1) col++;
                if (key >= '1' && key <= '9' && *(*(original_board + row) + col) == 0) {
                    *(*(board + row) + col) = key - '0';
                }
                if (key == '\r') {  // Нажатие клавиши Enter
                    clear_terminal();
                    print_board(size, board, original_board);
                    if (check_solution(size, board, original_board)) {
                        printf("Great solution!\n");
                        break;
                    } else {
                        printf("Wrong solution!\n");
                        sleep(1);
                    }
                }
            }
        } else {
            // Автоматическое решение
            #ifndef AUTO_SOLVE
            #define AUTO_SOLVE
            #include "auto_solve.h"
            auto_solve_sudoku(size, board, original_board);
            #endif
            clear_terminal();
            print_board(size, board, original_board);
            if (check_solution(size, board, original_board)) {
                printf("Solution found!\n");
            } else {
                printf("Wrong solution!\n");
            }
        }
        // Освобождение памяти
        free(original_board);
        free(board);
        sleep(2);
        printf("Wanna play again? (y/n)\n");
        getchar();
        if (getchar() == 'n'){
            flag = 0;
        }
    }
    clear_terminal();
    return 0;
}