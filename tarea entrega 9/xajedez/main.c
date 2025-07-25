#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define clear_screen() system("cls")
    #define get_key() getch()
#else
    #include <termios.h>
    #include <unistd.h>
    #define clear_screen() system("clear")

    int get_key() {
        struct termios old, new;
        int ch;
        tcgetattr(STDIN_FILENO, &old);
        new = old;
        new.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        return ch;
    }
#endif

#define EMPTY 0
#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6

#define WHITE 0
#define BLACK 1

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE_C "\033[37m"
#define BG_RED  "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"

typedef struct {
    int type;
    int color;
    bool has_moved;
} Piece;

typedef struct {
    int from_row, from_col;
    int to_row, to_col;
} Move;

Piece board[8][8];
int selected_row = -1, selected_col = -1;
bool valid_moves[8][8];
int current_player = WHITE;
bool game_over = false;
char *piece_symbols[2][7] = {
    {".", "P", "R", "N", "B", "Q", "K"},
    {".", "p", "r", "n", "b", "q", "k"}
};

int en_passant_file = -1;

void init_board();
void print_board();
void print_menu();
bool is_valid_position(int row, int col);
bool is_enemy_piece(int row, int col, int color);
bool is_ally_piece(int row, int col, int color);
void calculate_valid_moves(int row, int col, int active_color);
bool is_path_clear(int from_row, int from_col, int to_row, int to_col);
bool is_in_check(int color);
bool would_be_in_check(int from_row, int from_col, int to_row, int to_col, int color);
bool is_checkmate(int color);
bool is_stalemate(int color);
void make_move(int from_row, int from_col, int to_row, int to_col);
void handle_promotion(int row, int col);
bool can_castle_kingside(int color);
bool can_castle_queenside(int color);
int evaluate_position();
Move get_best_move(int color, int depth);
int minimax(int depth, bool is_maximizing, int alpha, int beta, int color);
void play_1v1();
void play_vs_cpu();
void flush_input_buffer();
bool parse_coords(const char* input, int* row, int* col);

void init_board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Piece){EMPTY, 0, false};
            valid_moves[i][j] = false;
        }
    }

    board[0][0] = (Piece){ROOK, BLACK, false};
    board[0][1] = (Piece){KNIGHT, BLACK, false};
    board[0][2] = (Piece){BISHOP, BLACK, false};
    board[0][3] = (Piece){QUEEN, BLACK, false};
    board[0][4] = (Piece){KING, BLACK, false};
    board[0][5] = (Piece){BISHOP, BLACK, false};
    board[0][6] = (Piece){KNIGHT, BLACK, false};
    board[0][7] = (Piece){ROOK, BLACK, false};
    for (int j = 0; j < 8; j++) board[1][j] = (Piece){PAWN, BLACK, false};

    board[7][0] = (Piece){ROOK, WHITE, false};
    board[7][1] = (Piece){KNIGHT, WHITE, false};
    board[7][2] = (Piece){BISHOP, WHITE, false};
    board[7][3] = (Piece){QUEEN, WHITE, false};
    board[7][4] = (Piece){KING, WHITE, false};
    board[7][5] = (Piece){BISHOP, WHITE, false};
    board[7][6] = (Piece){KNIGHT, WHITE, false};
    board[7][7] = (Piece){ROOK, WHITE, false};
    for (int j = 0; j < 8; j++) board[6][j] = (Piece){PAWN, WHITE, false};

    current_player = WHITE;
    game_over = false;
    selected_row = selected_col = -1;
    en_passant_file = -1;
    memset(valid_moves, 0, sizeof(valid_moves));
}

void print_board() {
    clear_screen();
    printf("\n  Juego de Ajedrez - Turno: %s\n", current_player == WHITE ? "Blancas" : "Negras");
    if (is_in_check(current_player)) {
        printf("  %s¡JAQUE!%s\n", RED, RESET);
    }

    printf("\n    a   b   c   d   e   f   g   h\n");
    printf("  +---+---+---+---+---+---+---+---+\n");
    for (int i = 0; i < 8; i++) {
        printf("%d |", 8 - i);
        for (int j = 0; j < 8; j++) {
            bool is_selected = (i == selected_row && j == selected_col);
            bool is_valid_move = valid_moves[i][j];
            bool is_dark_square = (i + j) % 2 == 1;

            if (is_selected) printf("%s", BG_YELLOW);
            else if (is_valid_move) printf("%s", BG_RED);
            else if (is_dark_square) printf("%s", "\033[100m");
            else printf("%s", RESET);

            if (board[i][j].type != EMPTY) {
                printf(" %s ", piece_symbols[board[i][j].color][board[i][j].type]);
            } else if (is_valid_move) {
                printf(" * ");
            } else {
                printf("   ");
            }
            printf("%s|", RESET);
        }
        printf(" %d\n", 8 - i);
        printf("  +---+---+---+---+---+---+---+---+\n");
    }
    printf("    a   b   c   d   e   f   g   h\n\n");
}

bool parse_coords(const char* input, int* row, int* col) {
    if (strlen(input) != 2) return false;
    char in_col = tolower(input[0]);
    char in_row = input[1];

    if (in_col < 'a' || in_col > 'h') return false;
    if (in_row < '1' || in_row > '8') return false;

    *col = in_col - 'a';
    *row = '8' - in_row;
    return true;
}

bool is_valid_position(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool is_enemy_piece(int row, int col, int color) {
    return is_valid_position(row, col) && board[row][col].type != EMPTY && board[row][col].color != color;
}

bool is_ally_piece(int row, int col, int color) {
    return is_valid_position(row, col) && board[row][col].type != EMPTY && board[row][col].color == color;
}

bool is_path_clear(int from_row, int from_col, int to_row, int to_col) {
    int row_dir = (to_row > from_row) ? 1 : (to_row < from_row) ? -1 : 0;
    int col_dir = (to_col > from_col) ? 1 : (to_col < from_col) ? -1 : 0;
    int row = from_row + row_dir;
    int col = from_col + col_dir;

    while (row != to_row || col != to_col) {
        if (board[row][col].type != EMPTY) return false;
        row += row_dir;
        col += col_dir;
    }
    return true;
}

void calculate_valid_moves(int row, int col, int active_color) {
    memset(valid_moves, 0, sizeof(valid_moves));
    if (board[row][col].type == EMPTY || board[row][col].color != active_color) return;

    int piece_type = board[row][col].type;
    int piece_color = board[row][col].color;

    switch (piece_type) {
        case PAWN: {
            int direction = (piece_color == WHITE) ? -1 : 1;
            int start_row = (piece_color == WHITE) ? 6 : 1;
            if (is_valid_position(row + direction, col) && board[row + direction][col].type == EMPTY) {
                if (!would_be_in_check(row, col, row + direction, col, piece_color)) valid_moves[row + direction][col] = true;
                if (row == start_row && is_valid_position(row + 2 * direction, col) && board[row + 2 * direction][col].type == EMPTY) {
                    if (!would_be_in_check(row, col, row + 2 * direction, col, piece_color)) valid_moves[row + 2 * direction][col] = true;
                }
            }
            for (int dc = -1; dc <= 1; dc += 2) {
                if (is_valid_position(row + direction, col + dc)) {
                    if (is_enemy_piece(row + direction, col + dc, piece_color)) {
                        if (!would_be_in_check(row, col, row + direction, col + dc, piece_color)) valid_moves[row + direction][col + dc] = true;
                    } else if (en_passant_file == col + dc && row == (piece_color == WHITE ? 3 : 4)) {
                        if (!would_be_in_check(row, col, row + direction, col + dc, piece_color)) valid_moves[row + direction][col + dc] = true;
                    }
                }
            }
            break;
        }
        case KNIGHT: {
            int moves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
            for (int i = 0; i < 8; i++) {
                int new_row = row + moves[i][0], new_col = col + moves[i][1];
                if (is_valid_position(new_row, new_col) && !is_ally_piece(new_row, new_col, piece_color)) {
                    if (!would_be_in_check(row, col, new_row, new_col, piece_color)) valid_moves[new_row][new_col] = true;
                }
            }
            break;
        }
        case KING: {
            int moves[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
            for (int i = 0; i < 8; i++) {
                int new_row = row + moves[i][0], new_col = col + moves[i][1];
                if (is_valid_position(new_row, new_col) && !is_ally_piece(new_row, new_col, piece_color)) {
                    if (!would_be_in_check(row, col, new_row, new_col, piece_color)) valid_moves[new_row][new_col] = true;
                }
            }
            if (!board[row][col].has_moved && !is_in_check(piece_color)) {
                if (can_castle_kingside(piece_color)) valid_moves[row][col + 2] = true;
                if (can_castle_queenside(piece_color)) valid_moves[row][col - 2] = true;
            }
            break;
        }
        default: {
            int dirs[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};
            int start_dir = (piece_type == ROOK) ? 0 : (piece_type == BISHOP) ? 4 : 0;
            int end_dir = (piece_type == ROOK) ? 4 : (piece_type == BISHOP) ? 8 : 8;
            for (int d = start_dir; d < end_dir; d++) {
                for (int i = 1; i < 8; i++) {
                    int new_row = row + dirs[d][0] * i, new_col = col + dirs[d][1] * i;
                    if (!is_valid_position(new_row, new_col)) break;
                    if (board[new_row][new_col].type == EMPTY) {
                        if (!would_be_in_check(row, col, new_row, new_col, piece_color)) valid_moves[new_row][new_col] = true;
                    } else if (is_enemy_piece(new_row, new_col, piece_color)) {
                        if (!would_be_in_check(row, col, new_row, new_col, piece_color)) valid_moves[new_row][new_col] = true;
                        break;
                    } else break;
                }
            }
            break;
        }
    }
}

bool is_in_check(int color) {
    int king_row = -1, king_col = -1;
    for (int i = 0; i < 8 && king_row == -1; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].type == KING && board[i][j].color == color) {
                king_row = i; king_col = j; break;
            }
        }
    }
    if (king_row == -1) return false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_enemy_piece(i, j, color)) {
                int p_type = board[i][j].type, p_color = board[i][j].color;
                switch (p_type) {
                    case PAWN: {
                        int dir = (p_color == WHITE) ? -1 : 1;
                        if (king_row == i + dir && (king_col == j - 1 || king_col == j + 1)) return true;
                        break;
                    }
                    case ROOK: if ((i == king_row || j == king_col) && is_path_clear(i, j, king_row, king_col)) return true; break;
                    case KNIGHT: {
                        if ((abs(i-king_row)==2 && abs(j-king_col)==1) || (abs(i-king_row)==1 && abs(j-king_col)==2)) return true;
                        break;
                    }
                    case BISHOP: if (abs(i-king_row) == abs(j-king_col) && is_path_clear(i, j, king_row, king_col)) return true; break;
                    case QUEEN: if (((i==king_row || j==king_col) || (abs(i-king_row)==abs(j-king_col))) && is_path_clear(i,j,king_row,king_col)) return true; break;
                    case KING: if (abs(i-king_row) <= 1 && abs(j-king_col) <= 1) return true; break;
                }
            }
        }
    }
    return false;
}

bool would_be_in_check(int from_row, int from_col, int to_row, int to_col, int color) {
    Piece temp_to = board[to_row][to_col];
    Piece temp_from = board[from_row][from_col];
    board[to_row][to_col] = temp_from;
    board[from_row][from_col] = (Piece){EMPTY, 0, false};
    bool check = is_in_check(color);
    board[from_row][from_col] = temp_from;
    board[to_row][to_col] = temp_to;
    return check;
}

bool has_legal_moves(int color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (is_ally_piece(i, j, color)) {
                calculate_valid_moves(i, j, color);
                for (int ni = 0; ni < 8; ni++) {
                    for (int nj = 0; nj < 8; nj++) {
                        if (valid_moves[ni][nj]) {
                            memset(valid_moves, 0, sizeof(valid_moves));
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool is_checkmate(int color) {
    return is_in_check(color) && !has_legal_moves(color);
}

bool is_stalemate(int color) {
    return !is_in_check(color) && !has_legal_moves(color);
}

bool can_castle_kingside(int color) {
    int row = (color == WHITE) ? 7 : 0;
    return !board[row][4].has_moved && !board[row][7].has_moved &&
           board[row][5].type == EMPTY && board[row][6].type == EMPTY &&
           !is_in_check(color) && !would_be_in_check(row, 4, row, 5, color) &&
           !would_be_in_check(row, 4, row, 6, color);
}

bool can_castle_queenside(int color) {
    int row = (color == WHITE) ? 7 : 0;
    return !board[row][4].has_moved && !board[row][0].has_moved &&
           board[row][1].type == EMPTY && board[row][2].type == EMPTY && board[row][3].type == EMPTY &&
           !is_in_check(color) && !would_be_in_check(row, 4, row, 3, color) &&
           !would_be_in_check(row, 4, row, 2, color);
}

void make_move(int from_row, int from_col, int to_row, int to_col) {
    Piece moving_piece = board[from_row][from_col];
    en_passant_file = -1;
    if (moving_piece.type == PAWN && abs(to_row - from_row) == 2) {
        en_passant_file = to_col;
    }

    if (moving_piece.type == KING && abs(to_col - from_col) == 2) {
        int row = (moving_piece.color == WHITE) ? 7 : 0;
        if (to_col > from_col) {
            board[row][6] = board[row][4]; board[row][6].has_moved = true;
            board[row][4] = (Piece){EMPTY,0,false};
            board[row][5] = board[row][7]; board[row][5].has_moved = true;
            board[row][7] = (Piece){EMPTY,0,false};
        } else {
            board[row][2] = board[row][4]; board[row][2].has_moved = true;
            board[row][4] = (Piece){EMPTY,0,false};
            board[row][3] = board[row][0]; board[row][3].has_moved = true;
            board[row][0] = (Piece){EMPTY,0,false};
        }
    } else {
        if (moving_piece.type == PAWN && to_col != from_col && board[to_row][to_col].type == EMPTY) {
            board[from_row][to_col] = (Piece){EMPTY, 0, false};
        }
        board[to_row][to_col] = moving_piece;
        board[to_row][to_col].has_moved = true;
        board[from_row][from_col] = (Piece){EMPTY, 0, false};
        if (board[to_row][to_col].type == PAWN && (to_row == 0 || to_row == 7)) {
            handle_promotion(to_row, to_col);
        }
    }
    current_player = (current_player == WHITE) ? BLACK : WHITE;
}

void flush_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void handle_promotion(int row, int col) {
    print_board();
    printf("\n¡Promocion de peon! Elige pieza (1:Q, 2:R, 3:B, 4:N): ");
    int choice = 0;
    scanf("%d", &choice);
    flush_input_buffer();
    switch (choice) {
        case 2: board[row][col].type = ROOK; break;
        case 3: board[row][col].type = BISHOP; break;
        case 4: board[row][col].type = KNIGHT; break;
        default: board[row][col].type = QUEEN; break;
    }
}

void play_game(bool is_cpu_game) {
    init_board();
    char from_str[10], to_str[10];
    int from_row, from_col, to_row, to_col;
    int human_color = WHITE, cpu_color = BLACK;

    if (is_cpu_game) {
        printf("¿Quieres jugar con blancas (1) o negras (2)? ");
        int choice;
        scanf("%d", &choice);
        flush_input_buffer();
        human_color = (choice == 1) ? WHITE : BLACK;
        cpu_color = 1 - human_color;
    }

    while (!game_over) {
        print_board();
        if (selected_row != -1) {
             printf("Pieza en %s seleccionada. Movimientos válidos marcados con *.\n", from_str);
        }

        if (is_checkmate(current_player)) {
            printf("\n%s¡JAQUE MATE! Ganan las %s%s\n", RED, current_player == WHITE ? "NEGRAS" : "BLANCAS", RESET);
            game_over = true;
        } else if (is_stalemate(current_player)) {
            printf("\n%s¡TABLAS POR AHOGADO!%s\n", YELLOW, RESET);
            game_over = true;
        }
        if (game_over) {
            printf("Presiona cualquier tecla para volver al menu...");
            get_key();
            return;
        }

        if (is_cpu_game && current_player == cpu_color) {
            printf("\nLa CPU está pensando...\n");
            Move cpu_move = get_best_move(cpu_color, 3);
            if (cpu_move.from_row != -1) {
                make_move(cpu_move.from_row, cpu_move.from_col, cpu_move.to_row, cpu_move.to_col);
            }
            continue;
        }

        printf("\nTurno de %s.\n", current_player == WHITE ? "Blancas" : "Negras");
        printf("Mover desde (ej: e2) o 'reiniciar': ");
        scanf("%s", from_str);
        flush_input_buffer();

        if (strcmp(from_str, "reiniciar") == 0) { init_board(); continue; }

        if (!parse_coords(from_str, &from_row, &from_col) || !is_ally_piece(from_row, from_col, current_player)) {
            printf("Posición inválida o no es tu pieza. Presiona una tecla para reintentar.\n");
            get_key();
            continue;
        }

        calculate_valid_moves(from_row, from_col, current_player);
        selected_row = from_row;
        selected_col = from_col;
        print_board();
        printf("Pieza en %s seleccionada. Movimientos válidos marcados con *.\n", from_str);

        printf("Mover hacia (o 'cancelar'): ");
        scanf("%s", to_str);
        flush_input_buffer();

        selected_row = -1; selected_col = -1;

        if (strcmp(to_str, "cancelar") == 0) {
            memset(valid_moves, 0, sizeof(valid_moves));
            continue;
        }

        if (!parse_coords(to_str, &to_row, &to_col) || !valid_moves[to_row][to_col]) {
            printf("Movimiento inválido. Presiona una tecla para reintentar.\n");
            memset(valid_moves, 0, sizeof(valid_moves));
            get_key();
            continue;
        }

        make_move(from_row, from_col, to_row, to_col);
        memset(valid_moves, 0, sizeof(valid_moves));
    }
}

void print_menu() {
    clear_screen();
    printf("\n  +======================================+\n");
    printf("  |           JUEGO DE AJEDREZ           |\n");
    printf("  +======================================+\n");
    printf("  |  1. Jugar 1 vs 1 (Dos jugadores)    |\n");
    printf("  |  2. Jugar vs CPU                     |\n");
    printf("  |  3. Salir                            |\n");
    printf("  +======================================+\n");
    printf("\n  Selecciona una opcion: ");
}

int main() {
    srand(time(NULL));
    int choice;
    while (1) {
        print_menu();
        scanf("%d", &choice);
        flush_input_buffer();
        switch (choice) {
            case 1: play_game(false); break;
            case 2: play_game(true); break;
            case 3: printf("\n¡Gracias por jugar!\n"); return 0;
            default:
                printf("\nOpción inválida. Presiona una tecla para continuar...");
                get_key();
                break;
        }
    }
    return 0;
}

int evaluate_position() {
    int score = 0;
    int piece_values[7] = {0, 100, 500, 320, 330, 900, 20000};
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
        if (board[i][j].type != EMPTY) {
            int value = piece_values[board[i][j].type];
            if (board[i][j].color == WHITE) score += value;
            else score -= value;
        }
    }
    return score;
}

int minimax(int depth, bool is_maximizing, int alpha, int beta, int color) {
    if (depth == 0 || is_checkmate(color) || is_stalemate(color)) {
        if (is_checkmate(color)) return is_maximizing ? -10000 - depth : 10000 + depth;
        if (is_stalemate(color)) return 0;
        return evaluate_position();
    }

    int best_eval = is_maximizing ? -99999 : 99999;
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
        if (is_ally_piece(i, j, color)) {
            calculate_valid_moves(i, j, color);
            for (int ni = 0; ni < 8; ni++) for (int nj = 0; nj < 8; nj++) {
                if (valid_moves[ni][nj]) {
                    Piece temp_to = board[ni][nj], temp_from = board[i][j];
                    board[ni][nj] = temp_from; board[i][j] = (Piece){EMPTY, 0, false};
                    int eval = minimax(depth - 1, !is_maximizing, alpha, beta, 1 - color);
                    board[i][j] = temp_from; board[ni][nj] = temp_to;
                    if (is_maximizing) {
                        best_eval = (eval > best_eval) ? eval : best_eval;
                        alpha = (alpha > best_eval) ? alpha : best_eval;
                    } else {
                        best_eval = (eval < best_eval) ? eval : best_eval;
                        beta = (beta < best_eval) ? beta : best_eval;
                    }
                    if (beta <= alpha) goto end_minimax;
                }
            }
        }
    }
    end_minimax: return best_eval;
}

Move get_best_move(int color, int depth) {
    Move best_move = {-1, -1, -1, -1};
    bool is_maximizing = (color == WHITE);
    int best_eval = is_maximizing ? -99999 : 99999;

    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) {
        if (is_ally_piece(i, j, color)) {
            calculate_valid_moves(i, j, color);
            for (int ni = 0; ni < 8; ni++) for (int nj = 0; nj < 8; nj++) {
                if (valid_moves[ni][nj]) {
                    Piece temp_to = board[ni][nj], temp_from = board[i][j];
                    board[ni][nj] = temp_from; board[i][j] = (Piece){EMPTY, 0, false};
                    int eval = minimax(depth - 1, !is_maximizing, -99999, 99999, 1 - color);
                    board[i][j] = temp_from; board[ni][nj] = temp_to;

                    if (is_maximizing ? (eval > best_eval) : (eval < best_eval)) {
                        best_eval = eval;
                        best_move = (Move){i, j, ni, nj};
                    }
                }
            }
        }
    }
    return best_move;
}
