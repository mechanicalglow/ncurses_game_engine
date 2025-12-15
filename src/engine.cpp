#include <ncurses/ncurses.h>

typedef struct Vec2
{
    int x;
    int y;
} Vec2;

class Renderer
{
    public:
        Renderer()
        {
            
        }
    
    
};

class Engine
{
    public:
        Vec2 screen_dim;
        Vec2 current_pos;
        
        Engine()
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
            
            this->printsw();
            
            this->center_cursor();
            this->plot_triangle();
            
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
            this->current_pos.y -= y;
            this->current_pos.x += x;
            
            move(current_pos.y, current_pos.x);
            
            refresh();
        }
        
        void mvcursor_draw(int y, int x, char ch)
        {
            this->move_cursor(y,x);
            
            addch(ch);
            
            refresh();
        }
        
        void plot_triangle()
        {
            this->plot_point(0,0);
            this->plot_point(0,5);
            this->plot_point(2,3);
        }
        
        void plot_point(int y, int x)
        {
            this->mvcursor_draw(y,x,'.');
            this->center_cursor();
        }
        
        void draw_line(int rs, int rn, int len, char ch)
        {
            for (int i = 0; i < len; i++)
            {
                this->mvcursor_draw(rs, rn, ch);
            }
        }

        void draw_triangle()
        {
            this->draw_line(1, 1, 5, '/');
            this->move_cursor(1, 0);
            this->draw_line(-1, 1, 5, '\\');
            this->draw_line(0,-1, 8, '_');
        }
        
        void enable_move()
        {
            while(1)
            {
                int key = getch();
                
                switch(key)
                {
                    case KEY_UP:
                        this->move_cursor(1,0);
                        break;
                        
                    case KEY_DOWN:
                        this->move_cursor(-1,0);
                        break;
                    
                    case KEY_LEFT:
                        this->move_cursor(0,-1);
                        break;
                        
                    case KEY_RIGHT:
                        this->move_cursor(0,1);
                        break;
                    
                    case 99: // enable echo
                        echo();
                        break;
                    
                    case 100: // d for draw
                        this->draw_line(1, 1, 5, '/');
                        break;
                    
                    case 112: //p for printw
                        this->printcp();
                        break;
                    
                    case 116: // draw triangle 
                        this->draw_triangle();
                        break;
                }
            }
            
        }
        
        void printcp()
        {
            move(0,0);
            refresh();
            
            for (int i = 0; i < this->screen_dim.x; i++)
            {
                mvaddch(0, i, ' ');
                refresh();
            }
            
            printw("Y: %d, X: %d", this->current_pos.y, this->current_pos.x);
            
            this->move_cursor(0,0);
        }
        
        void printsw()
        {
            printw("Screen Y: %d, Screen X: %d", this->screen_dim.y, this->screen_dim.x);
        }
        
        void deinit()
        {
            endwin();
        }
};

int main()
{
    Engine engine;

    return 0;
}
