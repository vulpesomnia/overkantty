#include <cstdlib>
#include <map>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <map>
#include <string>



class Tile {
public:
  int spriteIndex;
  int heldItem = -1;
  bool isActive = false;
  int timeStamp = 0;
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

int FPS = 30;
int globalTime = 0;
int tileHeight = 8;
int tileWidth = 16;
int height = 9 * tileHeight;
int width = 18 * tileWidth;

int heightTiles = height/tileHeight;
int widthTiles = width/tileWidth;
int screenInfo[6] = {height, width, tileHeight, tileWidth, heightTiles, widthTiles};

int dx=0;
int dy=0;

int score=1000;

Tile* tileInFront;
std::vector<Tile*> levelTiles;

std::vector<uint64_t> alphabet = {
  0b111101111101101, //a
  0b111101111101111, //b
  0b111100100100111, //c
  0b001001111101111, //d
  0b111100111100111, //e
  0b111100110110100, //f
  0b111100100101111, //g
  0b101101111101101, //h
  0b010010010010010, //i
  0b001001001101111, //j
  0b101101110101101, //k
  0b100100100100111, //l
  0b101101111101101, //m
  0b111101101101101, //n
  0b111101101101111, //o
  0b111101111100100, //p
  0b111101111001001, //q
  0b111101110101101, //r
  0b111100010001111, //s
  0b111010010010010, //t
  0b101101101101111, //u
  0b101101101101010, //v
  0b101101101111101, //w
  0b101101010101101, //x
  0b101101010010010, //y
  0b111001010100111, //z
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
  0b000000000000000, // 
  0b000000111000000, //-
  0b000000100000100, //:
  0b000000000000100, //.
  0b010010010000010, //!
  0b111001011000010, //?
  0b000000000010110, //,
  0b000010000010110, //;
  0b000000000000111 //_
};

std::vector<std::vector<int>> spriteSizes = {
  {16,8},
  {16,8},
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
  2,2,7,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
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
      tileInFront->timeStamp = globalTime;
    
    } else if (tileInFront->heldItem != -1 and player->heldItem == -1 and abs(globalTime-tileInFront->timeStamp) > FPS * 10) {
      if (tileInFront->heldItem == 1) {
        tileInFront->heldItem = 5;
      }
      else {
        tileInFront->heldItem = 6;
      }
      tileInFront->isActive = false;
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

void draw_character(WINDOW *topWin, int index,int x, int y){
    for (int i = 0; i < 16; i++){
        if (((index>>(i))&1)==1){
            mvwaddch(topWin, 1+5-(i)/3 + 7*y,1+3-(i)%3+5*x,'#');
        }
        else{
            mvwaddch(topWin, 1+5-(i)/3 + 7*y,1+3-(i)%3+5*x,' ');
        }
    }
}

void draw_string(WINDOW *topWin, std::string text, int x, int y){
    std::map<char, int> letterToNumber={{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}, {'i', 8}, {'j', 9}, {'k',10}, {'l', 11}, {'m', 12}, {'n', 13}, {'o', 14}, {'p', 15}, {'q', 16}, {'r', 17}, {'s', 18}, {'t', 19}, {'u', 20}, {'v', 21}, {'w', 22}, {'x', 23}, {'y', 24}, {'z', 25}, {'0', 26}, {'1', 27}, {'2', 28}, {'3', 29}, {'4', 30}, {'5', 31}, {'6', 32}, {'7', 33}, {'8', 34}, {'9', 35}, {' ', 36}, {'-',37}, {':', 38}, {'.', 39}};
    for (int i=0; i<text.size(); i++){
        draw_character(topWin, alphabet[letterToNumber[text[i]]],i+x, y);
    }
}

void update_top_screen(WINDOW *topWin)
{ 
    draw_string(topWin, "overkantty", 0,0);   
    draw_string(topWin, "panini juusto kantty x 99", 0,1);
    draw_string(topWin, "panini juusto kantty x 99", 0,2);
    draw_string(topWin, "panini juusto kantty x 99",27,1);
    draw_string(topWin, "panini juusto kantty x 99",27,2);
}

void update_right_screen(WINDOW *rightWin)
{ 
    std::string text;
    text="score "+std::to_string(score);
    draw_string(rightWin, text,0,0);
    draw_string(rightWin, "ohjeet:", 0,1);   
    draw_string(rightWin, "...", 0,2);
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
    int topHeight, topWidth;
    getmaxyx(stdscr, topHeight, topWidth);
    WINDOW* topWin = newwin(24, width, 0, 0);
    utilize_colors(win);
    WINDOW* rightWin = newwin(height,topWidth-width, 0, width);

    int ch;
    while (true) {
        score--;
        update_screen(win, screenInfo, levelTiles);
        wrefresh(win);
        update_top_screen(topWin);
        wrefresh(topWin);
        update_right_screen(rightWin);
        wrefresh(rightWin);
        handleInput(win);
        ch = getch();
        if (ch != ERR) {
            if (ch == 27) {
                endwin();
                return 0;
            }
        }
    }

    wrefresh(win);
    napms(1000 / 30);
}
