#include "Game.h"
vector<BlockType> blockTypes;

void loadBlockTypes()
{
	FILE *in;
	char filename[20], pass[20], fg[20], status[20], c;
	int r, g, b;
	BlockType bt;
	in = fopen("blocktypes.dat", "r");

	while(!feof(in))
	{
		fscanf(in, "%c %s %s %s %s %d %d %d\n", &c, pass, status, fg, filename, &r, &g, &b);
		bt.car = c;
		bt.pass = (strcmp(pass, "pass") == 0 ? true : false);
		bt.fg = (strcmp(fg, "fg") == 0 ? true : false);
		bt.regen = (strcmp(status, "regen") == 0 ? true : false);
		bt.king = (strcmp(status, "king") == 0 ? true : false);
		bt.nonspawn = (strcmp(status, "nonspawn") == 0 ? true : false);
		bt.img = SDL_LoadBMP(filename);
		bt.c[0] = r;
		bt.c[1] = g;
		bt.c[2] = b;
		cout << "Bloc "<<bt.car<<" "<<(bt.pass ? "traversable" : "bloquant")<<" "<<(bt.fg ? "avant-plan" : "arrière-plan")<<" chargé."<< endl;
		blockTypes.push_back(bt);
	}
	fclose(in);
}

BlockType *getBlock(char c)
{
    int i;
    for(i=0; i<blockTypes.size(); i++)
        if(blockTypes[i].car == c) break;
    return &(blockTypes[i]);
}

Map::Map(char *filename)
{
	FILE *in;
	int x, y, i;
	char c;
	in = fopen(filename, "r");
	cout << "Chargement de "<<filename << endl;
	x = 0; y = 0;
	w = 100000; h = 0;
	while(!feof(in))
	{
		c = fgetc(in);
		//cout << "Caractère "<<c<<" ; x = "<<x<<endl;
		if(c == '\n')
		{
			h++;
			y++;
			if(x < w) w = x;
			x = 0;
			continue;
		}
		for(i=0; i<blockTypes.size(); i++)
			if(blockTypes[i].car == c) break;

		if(i == blockTypes.size())
		{
			//cout << "Bloc non reconnu : '"<<c<<"'"<<endl;
			continue;
		}

		blocks.push_back(&blockTypes[i]);
		x++;
	}
	cout << "Taille de la carte : "<<w<<"x"<<h<<endl;
}

Map::Map()
{
    w = 50; h = 50;
    for(int x=0; x<w; x++)
    for(int y=0; y<h; y++)
        blocks.push_back(getBlock('#'));

	/* On génère la base de la carte */
	int x = 1+(rand() % (w-3));
	int y = 1+(rand() % (h-3));

	int n = 3*w*h/10+(rand() % (7*w*h/10-3*w*h/10));
	placeBlock(x, y, 0, n);

	for(x=w-3; x>=0; x--)
	{
		for(y=h-3; y>=0; y--)
		{
			if(blockType(x, y)->pass)
			{
				blocks[(y+1)*w+x] = getBlock('g');
				blocks[y*w+x+1] = getBlock('g');
			}
		}
	}
	/* On place le roi */
	int kx, ky;
	do
	{
		kx = 3+(rand() % (w-4));
		ky = 3+(rand() % (h-4));
	} while(! blockType(kx, ky)->pass);

	blocks[ky*w+kx] = getBlock('k');
	blocks[(ky+1)*w+kx] = getBlock('s');
	blocks[ky*w+kx+1] = getBlock('s');
	blocks[(ky-1)*w+kx] = getBlock('s');
	blocks[ky*w+kx-1] = getBlock('s');
	blocks[(ky-1)*w+kx-1] = getBlock('s');
	blocks[(ky-1)*w+kx+1] = getBlock('s');
	blocks[(ky+1)*w+kx+1] = getBlock('s');
	blocks[(ky+1)*w+kx-1] = getBlock('s');
	/* Et on empêche les gens de se rallier aux alentours */
	for(x=kx-10; x<=kx+10; x++)
	for(y=ky-10; y<=ky+10; y++)
	{
		if(x == kx && y == ky) continue;
		if(x >= w-1 || x < 1 || y >= h-1 || y < 1) continue;
		if((x-kx)*(x-kx)+(y-ky)*(y-ky) > 100) continue;
		if(blocks[y*w+x]->pass || (x-kx)*(x-kx)+(y-ky)*(y-ky) <= 10) blocks[y*w+x] = getBlock('s');
	}

	/* Et enfin, les barres de vie ! */
	for(int i=0; i<3; i++)
	{
		int bx, by;
		do
		{
			bx = 1+(rand() % (w-2));
			by = 1+(rand() % (h-2));
		} while((! blockType(bx, by)->pass) || (bx-kx)*(bx-kx)+(by-ky)*(by-ky) <= 110 );
		int o = rand() % 2;

		for(x=0; abs(x)<=5; x = (x > 0 ? -x : -x+1))
		{
			if(inMap(bx+o*x, by+(1-o)*x))
				if(blockType(bx+o*x, by+(1-o)*x)->pass) blocks[(by+(1-o)*x)*w+bx+o*x] = getBlock('c');
		}
	}


}

int Map::placeBlock(int x, int y, int i, int n)
{
	blocks[y*w+x] = getBlock('g');

	i++;
	if(i >= n) return n;
	bool tried[4] = {false, false, false, false};


	while((!tried[0]) || (!tried[1]) || (!tried[2]) || (!tried[3]))
	{
		int direction = rand() % 4;
		if(tried[direction]) continue;
		tried[direction] = true;
		int dx, dy;
		switch(direction)
		{
			case 0: dy = 1; dx = 0; break;
			case 1: dy = -1; dx = 0; break;
			case 2: dy = 0; dx = 1; break;
			case 3: dy = 0; dx = -1; break;
		}
		if(x+dx >= w-2 || x+dx < 1 || y+dy >= h-2 || y+dy < 1) continue;
		if(blockType(x+dx, y+dy)->pass)
			continue;

		i = placeBlock(x+dx, y+dy, i, n);

//		i += 3;

		if(i >= n)
			return n;
	}
	return i;
}

void getMainColor(SDL_Surface *surface, unsigned char *c)
{
	unsigned char r, g, b;
	int tr=0, tg=0, tb=0;
	for(int x=0; x<surface->w; x++)
		for(int y=0; y<surface->h; y++)
		{
			getPixel(surface, x, y, r, g, b);
			tr += r;
			tg += g;
			tb += b;
		}
	c[0] = tr/surface->h/surface->w;
	c[1] = tg/surface->h/surface->w;
	c[2] = tb/surface->h/surface->w;
}

void Map::genBG(int s, Level *level)
{
	bg = SDL_CreateRGBSurface(SDL_HWSURFACE, 32*w, 32*h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	//SDL_SetColorKey(bg, SDL_SRCCOLORKEY, SDL_MapRGB(bg->format, 1, 0, 254));
	SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, 1, 0, 254));

	getMainColor(level->pass, getBlock('g')->c);
	getMainColor(level->nonspawn, getBlock('s')->c);
	getMainColor(level->nonspawn, getBlock('k')->c);
	getMainColor(level->wall, getBlock('#')->c);
	getMainColor(level->regen, getBlock('c')->c);
	miniMap = createSurface(s*w+2, s*h+2, true, false);
	rectangleRGBA(miniMap, 0, 0, s*w+1, s*h+1, 0, 0, 0, 255);
	for(int x=0; x<w; x++)
		for(int y=0; y<h; y++)
		{
			SDL_Rect r;
			r.x = 32*x;
			r.y = 32*y;
			SDL_Surface *i = NULL;
			switch(blockType(x, y)->car)
			{
				case 'g': i = level->pass; break;
				case 's': i = level->nonspawn; break;
				case 'k': i = level->nonspawn; break;
				case '#': i = level->wall; break;
				case 'c': i = level->regen; break;
			}
			if(i == NULL) i = blockType(x, y)->img;
			SDL_BlitSurface(i, 0, bg, &r);
			if(s == 1) pixelRGBA(miniMap, x+1, y+1, blockType(x, y)->c[0], blockType(x, y)->c[1], blockType(x, y)->c[2], 255);
			else filledRectangleRGB(miniMap, 1+s*x, 1+s*y, s*x+s+1, s*y+s+1, blockType(x, y)->c[0], blockType(x, y)->c[1], blockType(x, y)->c[2]);
		}
		#define L 2
	for(int x=0; x<w-1; x++)
		for(int y=0; y<h; y++)
		{
			for(int dy=0; dy<32; dy++)
			{
				if(blockType(x, y) != blockType(x+1, y))
				{
					unsigned char ir, ig, ib, nr, ng, nb;
					getPixel(bg, 32*x+32-L, 32*y+dy, ir, ig, ib);
					getPixel(bg, 32*x+32+L, 32*y+dy, nr, ng, nb);
					for(int i=-L; i<=L; i++)
					{
						unsigned char r, g, b;
						r = ir+((int)nr-ir)*(i+L)/L/2;
						g = ig+((int)ng-ig)*(i+L)/L/2;
						b = ib+((int)nb-ib)*(i+L)/L/2;
						pixelRGBA(bg, 32*(x+1)+i, 32*y+dy, r, g, b, 255);
					}
				}
			}
		}

	for(int x=0; x<w; x++)
		for(int y=0; y<h-1; y++)
		{
			if(blockType(x, y) != blockType(x, y+1))
			{
				for(int dx=0; dx<32; dx++)
				{
					unsigned char ir, ig, ib, nr, ng, nb;
					getPixel(bg, 32*x+dx, 32*y+32-L, ir, ig, ib);
					getPixel(bg, 32*x+dx, 32*y+32+L, nr, ng, nb);
					for(int i=-L; i<=L; i++)
					{
						unsigned char r, g, b;
						r = ir+((int)nr-ir)*(i+L)/L/2;
						g = ig+((int)ng-ig)*(i+L)/L/2;
						b = ib+((int)nb-ib)*(i+L)/L/2;
						pixelRGBA(bg, 32*x+dx, 32*(y+1)+i, r, g, b, 255);
					}
				}
			}
		}




}


