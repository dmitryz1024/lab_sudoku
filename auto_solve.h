#ifndef AUTO_SOLVE
#define AUTO_SOLVE
// Функция для автоматического решения и визуализации (брутфорс)
int auto_solve_sudoku(int size, int (*board)[size], int (*original_board)[size]) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (*(*(board + row) + col) == 0) { // Если ячейка пустая
                for (int num = 1; num <= size; num++) { // Пробуем все возможные значения от 1 до size
                    if (is_valid(size, board, row, col, num)) { // Если решение верно
                        *(*(board + row) + col) = num;
                        clear_terminal();
                        print_board(size, board, original_board); // Выводим текущее состояние поля
                        set_matrix_position(row, col); // Перемещаем курсор на текущую ячейку
                        usleep(DELAY); // Задержка для визуализации
                        // Рекурсивно пытаемся решить дальше
                        if (auto_solve_sudoku(size, board, original_board)) {
                            return 1;
                        }
                        *(*(board + row) + col) = 0; // Откат, если не получилось решить
                    }
                }
                return 0; // Если не нашли решение для этой ячейки, возвращаем false
            }
        }
    }
    return 1; // Если все ячейки заполнены
}
#endif
