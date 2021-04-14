#ifndef MAP_H
#define MAP_H
#include <SDL.h>
#include <iostream>
#include <vector>
using namespace std;

typedef struct
{
	char car;
	bool pass;
	bool fg;
	bool regen;
	bool king;
	bool side;
	bool nonspawn;
	SDL_Surface *img;
	unsigned char c[3];
} BlockType;

extern vector<BlockType> blockTypes;
void loadBlockTypes();


typedef struct
{
	SDL_Surface *pass, *nonspawn, *wall, *regen;
	vector<int> odds;
	int boss;
} Level;

BlockType * getBlock(char c); // Obtenir le bloc correspondant au caractère c

class Map
{
	private:
		vector<BlockType *> blocks;
		int w, h;
		SDL_Surface *bg;
		SDL_Surface *miniMap;

		int placeBlock(int x, int y, int i, int n);
	public:
		Map(char *filename);
		Map();
		~Map() {SDL_FreeSurface(bg); SDL_FreeSurface(miniMap);}
		void genBG(int s, Level *level);
		inline const int width() {return w;};
		inline const int height() {return h;};
		SDL_Surface *getMiniMap() {return miniMap;};
		BlockType *blockType(int x, int y) {return blocks[y*w+x];};
		SDL_Surface *getBG() {return bg;};
		inline bool inMap(int x, int y) {return (x > 0 && x < w-1 && y > 0 && y < h-1);};
};



#endif
