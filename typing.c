#include "typing.h"
#include <curses.h>

int main(int argc, char* argv[])
{
    init_curses();
    srand(time(NULL));

    run();

    end_curses();

    return 0;
}

void run()
{
    char* words[WORD_COUNT];
    read_file(words, "../words.csv");

    run_loop(words);
}

void run_loop(char* words[])
{
    char* lines[LINE_COUNT];
    for (int i = 0; i < LINE_COUNT; i++)
    {
        lines[i] = gen_random_line(words);
    }
    char* typed = malloc(sizeof(char[get_line_length(0)]));

    bool has_rotated = false;

    while (true)
    {
        char input = handle_input(typed);
        if (should_rotate(lines, typed, input, has_rotated))
        {
            rotate_lines(lines, words, has_rotated);
            has_rotated = true;
            empty_string(typed);
        }
        if (has_rotated && input == K_BACKSPACE && strlen(typed) == 0)
        {
            has_rotated = false;
        }
        print_lines(lines, typed, has_rotated);
    }
}

void empty_string(char* string)
{
    for (int i = 0; i < get_line_length(0); i++)
    {
        string[i] = '\0';
    }
}

char handle_input(char* typed)
{
    char input = getch();
    switch (input)
    {
        case ERR:
            break;
        case K_BACKSPACE:
            typed[strlen(typed) - 1] = '\0';
            break;
        default:
            typed[strlen(typed)] = input;
            break;
    }
    return input;
}

bool should_rotate(char* lines[], char* typed, char input, bool has_rotated)
{
    return input == K_ENTER && strlen(typed) >= strlen(lines[has_rotated]);
}
void rotate_lines(char* lines[], char* words[], bool has_rotated)
{
    if (has_rotated)
    {
        lines[0] = lines[1];
        lines[1] = lines[2];
        lines[2] = gen_random_line(words);
    }
}

void prepare_print()
{
    clear();
    move((int)(get_scrh() / 2) - 3, 0);
    attron(COLOR_PAIR(1));
}
void print_lines(char* lines[], char* typed, int cur_line)
{
    prepare_print();
    for (int i = 0; i < LINE_COUNT; i++)
    {
        print_line(lines[i], i == cur_line ? typed : NULL);
    }
}
void print_line(char* line, char* typed)
{
    to_center(strlen(line), get_scrw());
    for (int c = 0; c < strlen(line); c++)
    {
        handle_color(line, typed, c);
        line[c] == '%' ? printw("  ") : printw("%c", line[c]);
    }
    printw("\n\n\r");
}

void handle_color(char* line, char* typed, int c)
{
    if (typed == NULL) 
    {
        attroff(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    if (typed[c] == 0)
    {
        attroff(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    if (line[c] == typed[c])
    {
        attron(A_BOLD);
        attron(COLOR_PAIR(1));
        return;
    }
    attron(A_BOLD);
    attron(COLOR_PAIR(2));
}

char* get_random_word(char* words[])
{
    return words[rand() % WORD_COUNT];
}

char* gen_random_line(char* words[])
{
    int line_length = get_line_length(LINE_PADDING);
    char* line = malloc(sizeof(char[line_length]));
    while (strlen(line) < line_length)
    {
        strcat(line, get_random_word(words));
        strcat(line, " ");
    }
    return line;
}

int get_line_length(int padding)
{
    return get_scrw() - padding * 2;
}
int get_scrw()
{
    int scrw, scrh;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}
int get_scrh()
{
    int scrw, scrh;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

void init_curses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(REFRESH_MS);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_GREEN, -1);
}
void end_curses()
{
    endwin();
}

void read_file(char* dest[], char* filename)
{
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < WORD_COUNT; i++)
    {
        dest[i] = malloc(WORD_SIZE);
        fscanf(file_ptr, "%127s", dest[i]);
    }
    fclose(file_ptr);
}
