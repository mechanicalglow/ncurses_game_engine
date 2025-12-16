// make vec3 a class, clean up. use other formula to calculate z axis using projection
#include <ncurses/ncurses.h>
#include <iostream>
#include <cmath>
#include "vec3.h"

const int ANGLE = 12;
const int Z_LENGTH = 48;

float Vec3::half_tan(int angle)
{
  float half = angle / 2.0f;
  float htan = tan(half);

  return htan;
}

float Vec3::get_temp(int coord, float htan)
{
    if (z == 0)
    {
      return static_cast<float>(coord);
    }

    float temp = (coord / (z * htan));
    
    if (temp > 0)
    {
      temp += 0.5f;
    }
    else 
    {
      temp -= 0.5f;
    }

    return temp;
}

void Vec3::xz(int angle)
{
    float htan = Vec3::half_tan(angle);
    float temp = Vec3::get_temp(x, htan);

    x = static_cast<int>(temp);
}

void Vec3::yz(int angle)
{
    float htan = Vec3::half_tan(angle);
    float temp = Vec3::get_temp(y, htan);

    y = static_cast<int>(temp);
}

class Engine
{
    public:
        Vec3 screen_dim;
        Vec3 current_pos;

        Engine() : screen_dim(0,0,0), current_pos(0,0,0)
        {   
            
            initscr();
            noecho();
            cbreak();
            keypad(stdscr, TRUE);
            
            refresh();

            screen_dim.x = getmaxx(stdscr);
            screen_dim.y = getmaxy(stdscr);
            screen_dim.z = Z_LENGTH;

            this->printsw();
            
            this->center_cursor();
            this->plot_pyramid();
            
            this->enable_move();
        }
        
        void center_cursor()
        {
            int midy = screen_dim.y / 2.0f;
            int midx = screen_dim.x / 2.0f;
            
            current_pos.y = midy;
            current_pos.x = midx;
            current_pos.z = 0;

            move(midy, midx);
        }
        
        void move_cursor(int y, int x)
        {
            current_pos.y -= y;
            current_pos.x += x;
            
            move(current_pos.y, current_pos.x);
            
            refresh();
        }
        
        void mvcursor_draw(int y, int x, char ch)
        {
            move_cursor(y,x);
            
            addch(ch);
            
            refresh();
        }
        
        void plot_pyramid()
        {
            int midx = screen_dim.x / 2.0f;
            int midy = screen_dim.y / 2.0f;

            Vec3 point1(midx, midy, 0);
            Vec3 point2(midx + 5, midy, 0);
            Vec3 point3(midx, midy, 5);
            Vec3 point4(midx + 5, midy, 5);
            Vec3 point5(midx, midy + 5, 0);
            Vec3 point6(midx, midy + 5, 5);
            Vec3 point7(midx + 5, midy + 5, 0);
            Vec3 point8(midx + 5, midy + 5, 5);

            plot_vec3(point1);
            plot_vec3(point2);
            plot_vec3(point3);
            plot_vec3(point4);
            plot_vec3(point5);
            plot_vec3(point6);
            plot_vec3(point7);
            plot_vec3(point8);
        }
        
        void plot_point(int y, int x)
        {
            mvaddch(y, x, '.');

            center_cursor();
        }
        
        void plot_vec3(Vec3 vec)
        {
          static int count = 1;

          std::cout << "Point " << count << " Before translation" << " X: " << vec.x << " Y: " << vec.y << std::endl;

          vec.xz(ANGLE);
          vec.yz(ANGLE);

          std::cout << "Point " << count << " X: " << vec.x << " Y: " << vec.y << std::endl;

          count++;

          plot_point(vec.y, vec.x);
        }

        void draw_line(int rs, int rn, int len, char ch)
        {
            for (int i = 0; i < len; i++)
            {
                mvcursor_draw(rs, rn, ch);
            }
        }

        void draw_triangle()
        {
            draw_line(1, 1, 5, '/');
            move_cursor(1, 0);
            draw_line(-1, 1, 5, '\\');
            draw_line(0,-1, 8, '_');
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
            
            printw("Y: %d, X: %d, Z: %d", this->current_pos.y, this->current_pos.x, this->current_pos.z);
            
            this->move_cursor(0,0);
        }
        
        void printsw()
        {
            printw("Screen Y: %d, Screen X: %d, Screen Z: %d", this->screen_dim.y, this->screen_dim.x, this->screen_dim.z);
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
