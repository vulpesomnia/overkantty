#include <ncurses.h>
#include <vector>
#include <iostream>

std::vector<std::vector<int>> tiles= {
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

    },
};

std::vector<std::vector<int>> sprites= {
    {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
    },
    {
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
    },
};

/* 0: ilma
 * 1: pöytä
 * 2: tiski
 */
std::vector<int> tileMap = {
  1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


std::vector<char> characters = {'a', ' '};

/*void draw_sprite(WINDOW *win, int screenInfo[], int spriteIndex, int x, int y){
    for (int y0=0; y0<screenInfo[2];y0++){
        for (int x0=0; x0<screenInfo[3];x0++){
            int characterIndex = sprites[spriteIndex][y0*screenInfo[2]+x0]; 
            //mvwaddch(win, x0+x, y0+y,characters[characterIndex]);
            //mvwaddch(win, 1, 1,'a');

        }
    }
}*/

void draw_tile(WINDOW *win, int screenInfo[],int tileIndex, int x, int y){
    //std::cout<<screenInfo[2]<<" "<<screenInfo[3]<<std::endl;
    for (int y0=0; y0<screenInfo[2];y0++){
        for (int x0=0; x0<screenInfo[3];x0++){
            int characterIndex = tiles[tileIndex][y0*screenInfo[2]+x0]; 
            mvwaddch(win, y0+y*screenInfo[2], x0+x*screenInfo[3], characters[characterIndex]);
        }
    }
}

void update_screen(WINDOW *win, int screenInfo[]){
    //std::cout<<"hi"<<std::endl;
    for (int y=0;y<screenInfo[4];y++){
        for (int x=0; x<screenInfo[5];x++){
            //std::cout<<"hi"<<std::endl;
            draw_tile(win, screenInfo, tileMap[y*screenInfo[4] + x], x, y);
        }
    }
}

int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    int tileHeight = 8;
    int tileWidth = 16;
    int height=9 * tileHeight;
    int width=18 * tileWidth;
    int heightTiles = height/tileHeight;
    int widthTiles = width/tileWidth;
    //heightTiles-=5;
    //widthTiles-=5;
    int screenInfo[6]={height, width, tileHeight, tileWidth, heightTiles, widthTiles};
    WINDOW* win = newwin(height, width, 0, 0);
    int ch;
    while(true){
        update_screen(win, screenInfo);
        wrefresh(win);
        ch=getch();
        if (ch!=ERR){
            if (ch==27){
                endwin();
                return 0;
            }
        }
    }

}