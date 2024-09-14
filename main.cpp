#include <map>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <map>



class Tile {
public:
  int spriteIndex;
  int heldItem = -1;
  bool isActive = false;
  int x, y;
  Tile(int tileX, int tileY, int tileSprite) {
    x = tileX;
    y = tileY;
    spriteIndex = tileSprite;

  }
};

class Player {
public: 
  int x, y;
  int spriteIndex;
  int heldItem = 1;
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

int heightTiles = height/tileHeight;
int widthTiles = width/tileWidth;
int screenInfo[6] = {height, width, tileHeight, tileWidth, heightTiles, widthTiles};

int dx=0;
int dy=0;
//0 nothing
//1 käntty


Tile* tileInFront;
std::vector<Tile*> levelTiles;

std::vector<uint64_t> alphabet = {
  0b111001111101111, //a
  0b100100111101111, //b
  0b000111100100111, //c
  0b001001111101111, //d
  0b111101111100111, //e
  0b111100110100100, //f
  0b111101111001011, //g
  0b100100111101101, //h
  0b010000010010010, //i
  0b010000010010110, //j
  0b100101110101101, //k
  0b100100100100110, //l
  0b000101111101101, //m
  0b000111101101101, //n
  0b000111101101111, //o
  0b111101111100100, //p
  0b111101111001001, //q
  0b000110100100100, //r
  0b111100010001111, //s
  0b000010111010011, //t
  0b000101101101111, //u
  0b000101101101010, //v
  0b000101101111101, //w
  0b101101010101101, //x
  0b000101111001011, //y
  0b000111001010111, //z
  0b111101101101111, //0
  0b110010010010111, //1
  0b111001111100111, //2
  0b111001111001111, //3
  0b101101111001001, //4
  0b111100111001111, //5
  0b111100111101111, //6
  0b111001001001001, //7
  0b111101111101111, //8
  0b111101111001111, //9
  0b010010010000010, //!
  0b111001011000010, //?
  0b000000000000100, //.
  0b000000000010110, //,
  0b000010000010110, //;
  0b000000100000100, //:
  0b000000111000000, //-
  0b000000000000111 //_
};

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

/* 1: ilma
 * 2: pöytä
 * 3: tiski
 * 4: leikkuulauta
 * 5: juustoasema
 * 6: känttyasema
 * 7: paninikone
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

void createLevel(std::vector<int> map) {
  int x = 0;
  int y = 0;
  for (int i = 0; i < (int)map.size(); i++) {
    if (i % (width/tileWidth) == 0 and i != 0) {
      y++;
      x = 0;
    }
    levelTiles.push_back(new Tile(x, y, map[i]));
    x++;
  }
} 

std::map<int, std::vector<int>> inputs = {{(int)'w', {0, -1}}, {(int)'a', {-2, 0}}, {(int)'s', {0, 1}}, {(int)'d', {2, 0}}};

void handleMovement(std::vector<int> movementVector) {
  int newPlayerX = movementVector[0] + player->x;
  int newPlayerY = movementVector[1] + player->y;

  dx=movementVector[0]/2;
  dy=movementVector[1];
  if (!(newPlayerX < 0 or newPlayerX + tileWidth > width or newPlayerY < 0 or newPlayerY + tileHeight > height)) {
    for (int i=0;i<(int)tileMap.size();i++){
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


Tile* getTile(int x, int y, WINDOW* win) {
  int tileX = x/tileWidth;
  int tileY = y/tileHeight;
  //DEBUG:
  //draw_sprite(win, screenInfo, 0, tileX * tileWidth, tileY * tileHeight);
  return levelTiles[tileY * widthTiles + tileX];
}


/* 1: ilma
 * 2: pöytä
 * 3: tiski
 * 4: leikkuulauta
 * 5: juustoasema
 * 6: känttyasema
 * 7: paninikone
 */
void useTile(WINDOW* win) {
  //Gets the center of player.
  int x = player->x + tileWidth/2;
  int y = player->y + tileHeight/2;

  tileInFront = getTile(x + tileWidth*dx, y + tileHeight*dy, win);
  //Temporary oletus: 1 on käntty, 2 on leikattu käntty, 3 on juusto, 4 on juustolla täytetty leikattu käntty, 5 on panini käntty ja 6 on juustolla täytetty panini käntty
  if (tileInFront->spriteIndex == 2) { //Pöytä
    if (player->heldItem != -1)  {
      if (tileInFront->heldItem == -1) {
        tileInFront->heldItem = player->heldItem;
        player->heldItem = -1;
      }
      else if (tileInFront->heldItem == 2 and player->heldItem == 3){
        tileInFront->heldItem = -1;
        player->heldItem = 4;
      }
    }
    else if (tileInFront->heldItem != -1) {
      player->heldItem = tileInFront->heldItem;
      tileInFront->heldItem = -1;
    }
  }
  else if (tileInFront->spriteIndex == 4) { //Leikkuulauta
    if(player->heldItem == 1) {
      player->heldItem = 2;
    }
  }
  else if (tileInFront->spriteIndex == 5) {//Juustoasema
    if (player->heldItem == -1) {
      player->heldItem = 3;
    }
  }
  else if (tileInFront->spriteIndex == 6) {//Känttyasema
    if (player->heldItem == -1) {
      player->heldItem = 1;
    }
  }
  else if (tileInFront->spriteIndex == 7) {//Paninikone
    if ((player->heldItem == 1 or player->heldItem == 4) and tileInFront->isActive == false and tileInFront->heldItem == -1) {
      tileInFront->heldItem = player->heldItem;
      player->heldItem = -1;
      tileInFront->isActive = true;
      napms(1000 * 10);
      if (tileInFront->heldItem == 1) {
        tileInFront->heldItem = 5;
      }
      else {
        tileInFront->heldItem = 6;
      }
      tileInFront->isActive = false;
    } else if (tileInFront->heldItem != -1 and tileInFront->isActive == false and player->heldItem == -1) {
      player->heldItem = tileInFront->heldItem;
      tileInFront->heldItem = -1;
    }
  }
}

void handleInput(WINDOW* win) {
  int input = getch();
  for (const auto& pair : inputs) {
    if (input == pair.first) {
      handleMovement(pair.second);
    }
  }
  if (input == ' '){
    useTile(win);
  }
}

void utilize_colors(WINDOW *win) {
  int n = 0;
  for (int i = 0 ; i < (int)sprites.size() ; i++) {
    //spriteColorIndex.push_back({});
    for (int j = 0 ; j < (int)sprites[i].size() ; j++) {
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
  createLevel(tileMap);
  player = new Player(tileHeight * 2, tileWidth * 2, 0);

  WINDOW* win = newwin(height, width, 24, 0);
  utilize_colors(win);
  int ch;
  while (true) {
    update_screen(win, screenInfo, levelTiles);
    handleInput(win);

    ch = getch();
    if (ch != ERR) {
      if (ch == 27) {
        endwin();
        return 0;
      }
    }

    wrefresh(win);
    napms(1000 / 30);
  }
}
