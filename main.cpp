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
        int heldItem;

        Player(int px, int py, int psprite) {
            x = px;
            y = py;
            spriteIndex = psprite;
            heldItem = 0;
        }
};
class Item {
    public:
        int spriteIndex;
};

class Tile {
  public:
    int spriteIndex;
    Item* heldItem;
    int x, y;
    Tile(int tileX, int tileY, int tileSprite) {
      x = tileX;
      y = tileY;
      spriteIndex = tileSprite;
    }

Player* player;

int tileHeight = 8;
int tileWidth = 16;
int height = 9 * tileHeight;
int width = 18 * tileWidth;

int tileInFront=1;

int dx=0;
int dy=0;
//0 nothing
//1 käntty


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

/* 0: pelaaja
 * 1: ilma
 * 2: pöytä
 * 3: tiski
 * 4: leikkuulauta
 * 5: juustoasema
 * 6: paninikone
 */


std::vector<int> tileMap = {
    2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,
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

void update_screen(WINDOW *win, int screenInfo[], std::vector<Tile*> tiles) {
    for (int i = 0; i < (int)tiles.size(); i++) {
      draw_tile(win, screenInfo, tiles[i]->spriteIndex, tiles[i]->x, tiles[i]->y);
    };
    draw_sprite(win ,screenInfo, player->spriteIndex, player->x, player->y);
}

std::map<int, std::vector<int>> inputs = {{(int)'w', {0, -1}}, {(int)'a', {-2, 0}}, {(int)'s', {0, 1}}, {(int)'d', {2, 0}}};

void handleMovement(std::vector<int> movementVector) {
    int newPlayerX = movementVector[0] + player->x;
    int newPlayerY = movementVector[1] + player->y;
    if (!(newPlayerX < 0 or newPlayerX + tileWidth > width or newPlayerY < 0 or newPlayerY + tileHeight > height)) {
        for (int i = 0 ; i < (int) tileMap.size() ; i++) {
            if (tileMap[i] != 1){
                if (newPlayerX < 16*(i%(width/tileWidth) + 1) && newPlayerX > 16*(i%(width/tileWidth) - 1) && newPlayerY < (i/(width/tileWidth) + 1)*tileHeight && newPlayerY > (i/(width/tileWidth) - 1)*tileHeight) {
                    return;
                }
            }
        }
        dx=movementVector[0];
        dy=movementVector[1];
        player->x += movementVector[0];
        player->y += movementVector[1];
    }
}

void getItem(){
    int x=player->x;
    int y=player->y;
    tileInFront=tileMap[((y+4)/8+dy)*18 + ((x+8)/16+dx/2)];
}

void handleInput() {
    int input = getch();
    for (const auto& pair : inputs) {
        if (input == pair.first) {
            handleMovement(pair.second);
        }
    }
    if (input==' '){
        getItem();
    }
}

void utilize_colors(WINDOW *win) {
    int n = 0;
    for (int i = 0 ; i < (int) sprites.size() ; i++) {
        //spriteColorIndex.push_back({});
        for (int j = 0 ; j < (int) sprites[i].size() ; j++) {
            if (colorIndexes.find((spriteColorBack[i][j]<<16) | (spriteColorFront[i][j])) == colorIndexes.end()) {
                colorIndexes[(spriteColorBack[i][j]<<16)|(spriteColorFront[i][j])] = n;
                init_pair(n, spriteColorFront[i][j], spriteColorBack[i][j]);
                n++;
            }
        }
    }
}

std::vector<Tile*> levelTiles;

void createLevel(std::vector<int> map) {
  int x = 0;
  int y = 0;
  for (int i = 0; i <= (int)map.size(); i++) {
    if (i % width/tileWidth == 0 and i != 0) {
      y++;
      x = 0;
    }
    levelTiles.push_back(new Tile(x, y, map[i]));
    x++;
  }
} 

int main(){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    nodelay(stdscr, TRUE);

    createLevel(tileMap);
    player = new Player(tileHeight * 2, tileWidth * 2, 0);

    int heightTiles = height/tileHeight;
    int widthTiles = width/tileWidth;

    int screenInfo[6] = {height, width, tileHeight, tileWidth, heightTiles, widthTiles};
    WINDOW* win = newwin(height, width, 24, 0);
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
