#include <ncurses/ncurses.h>

typedef struct Vec2
{
    int x;
    int y;
} Vec2;

class Curses
{
    public:
        Vec2 screen_dim;
        Vec2 current_pos;
        
        Curses()
        {   
            int x;
            int y;
            
            initscr();
            noecho();
            cbreak();
            keypad(stdscr, TRUE);
            
            refresh();
            
            getmaxyx(stdscr, y, x);
            
            this->screen_dim.y = y;
            this->screen_dim.x = x;
            
            this->center_cursor();
            this->enable_move();
        }
        
        void center_cursor()
        {
            int midy = this->screen_dim.y / 2.0f;
            int midx = this->screen_dim.x / 2.0f;
            
            this->current_pos.y = midy;
            this->current_pos.x = midx;
            
            move(midy, midx);
        }
        
        void move_cursor(int y, int x)
        {
            this->current_pos.y += y;
            this->current_pos.x += x;
            
            move(current_pos.y, current_pos.x);
            
            refresh();
        }
        
        void enable_move()
        {
            while(1)
            {
                int key = getch();
                
                switch(key)
                {
                    case KEY_UP:
                        this->move_cursor(-1,0);
                        break;
                        
                    case KEY_DOWN:
                        this->move_cursor(1,0);
                        break;
                    
                    case KEY_LEFT:
                        this->move_cursor(0,-1);
                        break;
                        
                    case KEY_RIGHT:
                        this->move_cursor(0,1);
                        break;
                }
            }
            
        }
        
        void deinit()
        {
            endwin();
        }
};

int main()
{
    Curses ncurses;

    return 0;
}
