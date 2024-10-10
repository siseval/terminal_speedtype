#include "menu.h"

void draw_button(struct button b, struct menu m, int col) 
{
    attron(COLOR_PAIR(col));
  
    move_center_h(-((int)(strlen(b.text) + strlen(m.left) + strlen(m.right)) / 2));
    printw("%s%s%s\n\r", m.left, b.text, m.right);

    attroff(COLOR_PAIR(col));
}

void draw_buttons(struct menu m, int gaps[]) 
{
    for (int i = 0; i < m.num_buttons; i++) 
    {
        struct button b = m.buttons[i];
        for (int j = 0; j < gaps[i]; j++) 
        {
            printw("\n");
        }
        draw_button(b, m, m.selected == i ? m.selected_col : m.normal_col);
    }
}

int do_menu(struct menu *m, int gaps[], bool clear_screen)
{
    m->bold ? attron(A_BOLD) : attroff(A_BOLD);
    int selection = m->selected;
    while (!m->has_selected)
    {
        if (clear_screen)
        {
            clear();
        }
        move_center_v(-((int)(get_height(*m, gaps) / 2)));
        move_center_h(-((int)(strlen(m->top_text) / 2)));

        printw("%s", m->top_text);
        draw_buttons(*m, gaps);

        selection = menu_input(m);
    }
    attroff(A_BOLD);
    return selection;
}

int get_height(struct menu m, int gaps[])
{
    int height = m.num_buttons + 1;
    for (int i = 0; i < m.num_buttons; i++)
    {
        height += gaps[i];
    }
    return height;
}

struct button get_button(struct menu m, int i) { return m.buttons[i]; }

int menu_input(struct menu *m) 
{
    switch (getch()) 
    {
    case 'k':
    case 'w':
        m->selected -= m->selected <= 0 ? 0 : 1;
        break;

    case 'j':
    case 's':
        m->selected += m->selected >= m->num_buttons - 1 ? 0 : 1;
        break;

    case ' ':
    case 'f':
        m->has_selected = true;
        break;
    }
    return m->selected;
}

int get_cur_x()
{
    int y, x;
    getyx(stdscr, y, x);
    return x;
}
int get_cur_y()
{
    int y, x;
    getyx(stdscr, y, x);
    return y;
}

int get_scrw()
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}

int get_scrh()
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

void move_center_v(int dy)
{
    move((int)(get_scrh() / 2) + dy, get_cur_x());
}

void move_center_h(int dx)
{
    move(get_cur_y(), (int)(get_scrw() / 2) + dx);
}
