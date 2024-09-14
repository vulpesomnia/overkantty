#include <map>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <map>

class Player {
    public: 
        int x, y;
        int spriteIndex;

        Player(int px, int py, int psprite) {
            x = px;
            y = py;
            spriteIndex = psprite;
        }
};
Player* player;

int tileHeight = 8;
int tileWidth = 16;
int height = 9 * tileHeight;
int width = 18 * tileWidth;


std::vector<std::vector<int>> spriteSizes = {
    {16,8},
    {16,8},
    {16,8},
    {16,8},
    {16,8},
    {16,8}
};

std::vector<std::vector<int>> sprites = {
    {
        4,4,0,3,5,0,6,6,6,0,3,5,0,0,4,4,
        5,5,8,0,0,10,0,0,0,10,0,0,9,3,3,0,
        0,5,0,5,0,0,11,1,12,0,0,3,0,3,0,0,
        0,0,5,0,0,0,0,0,0,0,0,0,0,3,0,0,
        0,0,3,0,0,0,0,0,0,0,0,0,0,5,0,0,
        0,4,0,0,0,0,0,0,0,0,0,0,0,0,4,0,
        0,0,5,0,0,0,0,0,0,0,0,0,0,3,0,0,
        0,0,0,3,3,3,0,0,0,0,3,3,3,0,0,0,
    },
    {
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
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
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

    },
    {
        0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
        0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
        0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
        0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,
        1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,
    },
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,

    },
};

std::vector<std::vector<short>> spriteColorFront = {
    {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
        {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
            {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
            {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
            {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },         
       {
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
};

std::vector<std::vector<short>> spriteColorBack = {
    {
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
        196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,196,196,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
        195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,
    },
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
};

/* 0: ilma
 * 1: pöytä
 * 2: tiski
 * 3: leikkuulauta
 * 4: juustoasema
 * 5: paninikone
 */


std::vector<int> tileMap = {
    2,2,2,4,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};


std::vector<char> characters = {' ', '#', 'b', '/', '|', '\\', '-', '_', '(', ')', 'o', '<', '>'};

std::map<int,int> colorIndexes;

void draw_sprite(WINDOW *win, int screenInfo[], int spriteIndex, int x, int y) {
    for (int y0 = 0 ; y0 < spriteSizes[spriteIndex][1] ; y0++) {
        for (int x0 = 0 ; x0 < spriteSizes[spriteIndex][0] ; x0++) {
            int characterIndex = sprites[spriteIndex][y0*spriteSizes[spriteIndex][0] + x0]; 
            int fc = spriteColorFront[spriteIndex][y0*spriteSizes[spriteIndex][0] + x0];
            int bc = spriteColorBack[spriteIndex][y0*spriteSizes[spriteIndex][0] + x0];
            int pairIndex = colorIndexes[(bc<<16)|fc];
            wattron(win, COLOR_PAIR(pairIndex));
            mvwaddch(win, y0+y, x0+x,characters[characterIndex]);
            wattroff(win, COLOR_PAIR(pairIndex));
        }
    }
}

void draw_tile(WINDOW *win, int screenInfo[],int tileIndex, int x, int y) {
    for (int y0 = 0 ; y0 < screenInfo[2] ; y0++) {
        for (int x0 = 0 ; x0 < screenInfo[3] ; x0++) {
            int characterIndex = sprites[tileIndex][y0*screenInfo[3] + x0]; 
            int fc = spriteColorFront[tileIndex][y0*spriteSizes[tileIndex][0] + x0];
            int bc = spriteColorBack[tileIndex][y0*spriteSizes[tileIndex][0] + x0];
            int pairIndex = colorIndexes[(bc<<16)|fc];
            wattron(win, COLOR_PAIR(pairIndex));
            mvwaddch(win, y0 + y*screenInfo[2], x0 + x*screenInfo[3], characters[characterIndex]);
            wattroff(win, COLOR_PAIR(pairIndex));
        }
    }
}

void update_screen(WINDOW *win, int screenInfo[]) {
    for (int y = 0 ; y < screenInfo[4] ; y++) {
        for (int x = 0 ; x < screenInfo[5] ; x++) {
            draw_tile(win, screenInfo, tileMap[y*screenInfo[5] + x], x, y);
        }
    }
    draw_sprite(win ,screenInfo, player->spriteIndex, player->x, player->y);
}

std::map<int, std::vector<int>> inputs = {{(int)'w', {0, -1}}, {(int)'a', {-2, 0}}, {(int)'s', {0, 1}}, {(int)'d', {2, 0}}};

void handleMovement(std::vector<int> movementVector) {
    int newPlayerX = movementVector[0] + player->x;
    int newPlayerY = movementVector[1] + player->y;
    if (!(newPlayerX < 0 or newPlayerX + tileWidth > width or newPlayerY < 0 or newPlayerY + tileHeight > height)) {
        for (int i=0;i<tileMap.size();i++){
            if (tileMap[i]!=1){
                if (newPlayerX<16*(i%18+1) && newPlayerX>16*(i%18-1) && newPlayerY<(i/18+1)*tileHeight && newPlayerY>(i/18-1)*tileHeight){
                    return;
                }
            }
        }
        player->x += movementVector[0];
        player->y += movementVector[1];
    }
}

void handleInput() {
    int input = getch();
    for (const auto& pair : inputs) {
        if (input == pair.first) {
            handleMovement(pair.second);
        }
    }
}

void utilize_colors(WINDOW *win) {
    int n = 0;
    for (int i = 0 ; i < sprites.size() ; i++) {
        //spriteColorIndex.push_back({});
        for (int j = 0 ; j < sprites[i].size() ; j++) {
            if (colorIndexes.find((spriteColorBack[i][j]<<16) | (spriteColorFront[i][j])) == colorIndexes.end()) {
                colorIndexes[(spriteColorBack[i][j]<<16)|(spriteColorFront[i][j])] = n;
                init_pair(n, spriteColorFront[i][j], spriteColorBack[i][j]);
                n++;
            }
        }
    }
}

int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    nodelay(stdscr, TRUE);
    player = new Player(tileHeight * 2, tileWidth * 2, 0);
    int heightTiles = height/tileHeight;
    int widthTiles = width/tileWidth;
    int screenInfo[6] = {height, width, tileHeight, tileWidth, heightTiles, widthTiles};
    WINDOW* win = newwin(height, width, 0, 0);
    utilize_colors(win);

    int ch;
    while (true) {
        update_screen(win, screenInfo);
        wrefresh(win);
        handleInput();
        ch = getch();
        if (ch != ERR) {
            if (ch == 27) {
                endwin();
                return 0;
            }
        }
    }
}