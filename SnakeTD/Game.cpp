#include "Game.h"
#include <time.h>

vector<SDL_Surface *> *newLevelAnim;

void getDirection (char c, int &x, int &y)
{
	switch(c)
	{
		case 0: x = 1; y = 0; break;
		case 1: x = -1; y = 0; break;
		case 2: x = 0; y = 1; break;
		case 3: x = 0; y = -1; break;
	}
}


Game::Game(int nPlayers, SDL_Surface *screen)
{
	srand(time(NULL));
	levelId = -1;
	loadLevels();
	//SDL_LockSurface(infoLayer);
	for(int i=0; i<nPlayers; i++)
	{
		switch(nPlayers)
		{
			case 1: players.push_back(new Player(this, i+1, screen->w, screen->h, 0, 0, i*255, i*255, (1-i)*255)); break;
			case 2: players.push_back(new Player(this, i+1, screen->w/2, screen->h, screen->w/2*(1-i), 0, i*255, i*255, (1-i)*255)); break;
			case 4:
			case 3: if(i == 1) {players.push_back(new Player(this, i+1, screen->w/2, screen->h/2, 0, 0, 0, 0, 255)); break;}
					if(i == 0) {players.push_back(new Player(this, i+1, screen->w/2, screen->h/2, screen->w/2, 0, 255, 255, 0)); break;}
					if(i == 2) {players.push_back(new Player(this, i+1, screen->w/2, screen->h/2, 0, screen->h/2, 255, 0, 255)); break;}
					if(i == 3) {players.push_back(new Player(this, i+1, screen->w/2, screen->h/2, screen->w/2, screen->h/2, 0, 255, 255)); break;}
		}
		towers.push_back(players[i]->getHead());

	}
	//players[0]->getHead()->gainLevels(30);



    //miniMapSize = 1;
	newLevel(screen->w, screen->h);

    newLevelAnim = loadAnimation("newlevel");
	maxLevel = 7;
	bonusExp = 100;
	screenW = screen->w;
	screenH = screen->h;
	switch(nPlayers)
	{
		case 1: viewH = screenH; viewW = screenW; break;
		case 2: viewH = screenH; viewW = screenW/2; break;
		case 4:
		case 3: viewH = screenH/2; viewW = screenW/2; break;
	}
}

bool match(FILE *f, char *match)
{
	char read[50];
	int i, n;
	n = strlen(match);
	for(i=0; i<n; i++)
		read[i] = fgetc(f);
	read[n] = 0;

	if(strcmp(match, read) == 0) return true;
	fseek(f, -n, SEEK_CUR);
	return false;
}

void Game::loadLevels()
{
	FILE *leveldat;
	Level *curlevel;
	char filename[40];
	leveldat = fopen("levels.dat", "r");
	if(leveldat == NULL)
	{
		printf("Erreur lors de l'ouverture de levels.dat\n");
		exit(1);
	}
	printf("Lecture des niveaux...\n");

	int ntowers;
	fscanf(leveldat, "NTowers = %d\n", &ntowers);

	curlevel = NULL;
	while(!feof(leveldat))
	{
		if(match(leveldat, "[Level]\n"))
		{
			if(curlevel != NULL) levels.push_back(curlevel);
			curlevel = new Level;
			printf("Nouveau niveau !\n");
		}
		if(match(leveldat, "odds = "))
		{
			int o;
			for(int i=0; i<ntowers-1; i++)
			{
				fscanf(leveldat, "%d,", &o);
				curlevel->odds.push_back(o);
			}
			fscanf(leveldat, "%d\n", &o);
			curlevel->odds.push_back(o);
			printf("Probabilités lues : %d %d %d\n", curlevel->odds[0], curlevel->odds[4], curlevel->odds[6]);
		}
		if(fscanf(leveldat, "pass = %s\n", filename) > 0)
		{
			curlevel->pass = SDL_LoadBMP(filename);
			printf("Lu pass %s\n", filename);
		}
		if(fscanf(leveldat, "regen = %s\n", filename) > 0)
		{
			curlevel->regen = SDL_LoadBMP(filename);
			printf("Lu regen %s\n", filename);
		}
		if(fscanf(leveldat, "wall = %s\n", filename) > 0)
		{
			curlevel->wall = SDL_LoadBMP(filename);
			printf("Lu wall %s\n", filename);
		}
		if(fscanf(leveldat, "nonspawn = %s\n", filename) > 0)
		{
			curlevel->nonspawn = SDL_LoadBMP(filename);
			printf("Lu nonspawn %s\n", filename);
		}
		fscanf(leveldat, "boss = %d\n", &(curlevel->boss));
		fscanf(leveldat, "\n");
	}
	levels.push_back(curlevel);
	printf("%d niveaux lus\n", levels.size());
}

void Game::endLevel()
{
	for(int i=0; i<towers.size(); i++)
	{
		if(towers[i]->ownerId == -1)
		{
			delete towers[i];
			towers.erase(towers.begin()+i);
			i--;
		}
		else towers[i]->endSpecialAttack();
	}
	while(bonuses.size() != 0)
	{
		delete bonuses[0];
		bonuses.erase(bonuses.begin());
	}
	delete[] towerMap;
	delete map;
	SDL_FreeSurface(gameLayer);
//	SDL_FreeSurface(infoLayer);
	SDL_FreeSurface(gameMiniMap);
}

 void Game::newLevel(int w, int h)
 {
 	nextLevel = false;
 	bonusExp *= 3;
	blinkCounter = 0;
	maxLevel += 7;
    drawSpecialAttack = NULL;
	map = new Map();
	levelId++;
	level = levels[levelId];
	totalOdds = 0;

	towerMap = new Tower * [map->height()*map->width()];
	for(int i=0; i<map->height()*map->width(); i++) towerMap[i] = NULL;

	for(int i=0; i<level->odds.size(); i++) totalOdds += level->odds[i];

	gameLayer = SDL_CreateRGBSurface(SDL_HWSURFACE, 32*map->width(), 32*map->height(), fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	SDL_SetColorKey(gameLayer, SDL_SRCCOLORKEY, SDL_MapRGB(gameLayer->format, 1, 0, 254));
	SDL_FillRect(gameLayer, 0, SDL_MapRGB(gameLayer->format, 1, 0, 254));

	/*infoLayer = SDL_CreateRGBSurface(SDL_SWSURFACE, 32*map->width(), 32*map->height(), fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	SDL_SetColorKey(infoLayer, SDL_SRCCOLORKEY, SDL_MapRGB(infoLayer->format, 1, 0, 254));
	SDL_FillRect(infoLayer, 0, SDL_MapRGB(infoLayer->format, 1, 0, 254));*/

	for(int x=0; x<map->width(); x++)
	{
		for(int y=0; y<map->height(); y++)
		{
			if(map->blockType(x, y)->king)
			{
				int i;
				for(i=0; i<towerTypes.size(); i++)
					if(towerTypes[i].cat == ABOMINATION) break;
				towers.push_back(new Tower(this, towers.size()+players.size()+1, x*32+16, y*32+16, 0, 0, NULL, &(towerTypes[level->boss]), -1, true));
			}
		}
	}
	for(int i=0; i<players.size(); i++)
	{
		int ix, iy;
		do
		{
			ix = rand() % map->width();
			iy = rand() % (map->height()-1);
		} while((!map->blockType(ix, iy)->pass) || (!map->blockType(ix, iy+1)->pass) || (map->blockType(ix, iy)->king) || map->blockType(ix, iy)->nonspawn);

		players[i]->getHead()->setPos(ix, iy);
	}

    for(int i=0; i<30; i++)
    {
		addTower(0, false);
    }

	switch(players.size())
    {
    	case 1: miniMapSize = (h/4-4)/map->width(); break;
    	case 2: miniMapSize = 1; break;
    	case 3: miniMapSize = (h/2-4)/map->width(); break;
    	case 4: miniMapSize = 1; break;
    }

   	map->genBG(miniMapSize, level);
	gameMiniMap = createSurface(miniMapSize*map->width(), miniMapSize*map->height(), true, true);

 }

#define Dist(x, y) (x)*(x)+(y)*(y)
void Game::addTower(int level, bool appeared)
{
	int x, y;
	int c = 0;
	bool ok = false;
	bool near = false;
    do
    {
    	near = false;
    	c++;
    	x = rand() % map->width();
    	y = rand() % map->height();

    	for(int it=0; it<players.size(); it++)
    	{
			if(Dist(x-players[it]->getHead()->getOx(), y-players[it]->getHead()->getOy()) <= 36)
			{
				near = true;
				break;
			}
    	}
		if(near) continue;

		if(! (blockTaken(x, y) || map->blockType(x, y)->regen))
			ok = true;
    } while(!ok && c <= 1000);

	if(ok) addTower(x, y, level, appeared);
}


void Game::addTower(int x, int y, int reqLevel, bool appeared)
{
	int type;
    int n = rand() % totalOdds;
    int cursor = 0;
    for(type=0; type<towerTypes.size(); type++)
    {
		if(n >= cursor && n < cursor+level->odds[type]) break;
		cursor += level->odds[type];
    }
    if(type == towerTypes.size()) type = 0;
    towers.push_back(new Tower(this, towers.size()+players.size()+1, x*32+16, y*32+16, 0, 0, NULL, &(towerTypes[type]), -1, appeared));
    towers[towers.size()-1]->gainLevels(reqLevel-towerTypes[type].baseLevel);
}

void Game::addTowerNearAnother(int level)
{
	if(rand() % 10 == 0)
	{
		addTower(level); // Sinon toutes les tours finiraient au même endroit
		return;
	}
	bool *triedTowers = new bool[towers.size()];
	for(int i=0; i<towers.size(); i++) triedTowers[i] = false;
	bool ok = false;
	bool end = true;
	do
	{
		for(int i=0; i<towers.size(); i++)
		{
			if(triedTowers[i] == false)
			{
				end = false;
				break;
			}
		}
		if(end) break;

		int i;
		do i = rand() % towers.size(); while(triedTowers[i]);
		triedTowers[i] = true;

		if(towers[i]->ownerId != -1) continue;

		bool cont = true;
		for(int it=0; it<players.size(); it++)
		{
			if(towers[i]->distance(players[it]->getHead()) <= 32*10*32*10)
			{
				cont = false;
				break;
			}
		}
		if(!cont) continue;


		bool triedDirections[4] = {false, false, false, false};
		int direction;
		do
		{
			direction = rand() % 4;
			if(triedDirections[direction]) continue;
			triedDirections[direction] = true;
			int dx, dy;
			switch(direction)
			{
				case 0: dx = 1; dy = 0; break;
				case 1: dx = -1; dy = 0; break;
				case 2: dx = 0; dy = 1; break;
				case 3: dx = 0; dy = -1; break;
			}
			if(map->inMap(towers[i]->x/32/P+dx, towers[i]->y/32/P+dy))
				if(! (blockTaken(towers[i]->x/32/P+dx, towers[i]->y/32/P+dy) || map->blockType(towers[i]->x/32/P+dx, towers[i]->y/32/P+dy)->regen))
				{
					ok = true;
					addTower(towers[i]->x/32/P+dx, towers[i]->y/32/P+dy, level, true);
					break;
				}
		} while((!triedDirections[0]) || (!triedDirections[1]) || (!triedDirections[2]) || (!triedDirections[3]));
	} while(!ok);
	if(! ok) addTower(level);
	delete[] triedTowers;
}

void Game::newBonus(int x, int y)
{
	for(int i=0; i<bonuses.size(); i++)
	{
		if(bonuses[i]->x == x && bonuses[i]->y == y) return;
	}
	if(map->blockType(x, y)->regen) return;

	bonuses.push_back(new Bonus(x, y));
}


void Game::draw(SDL_Surface *screen)
{
	SDL_Rect pos;



	SDL_Rect screenPart;
	bool drawInfo = false;

	if(drawSpecialAttack == NULL)
		for(int i=0; i<players.size(); i++)
		{
			SDL_Rect p;
			p = players[i]->pos();
			SDL_FillRect(gameLayer, &p, SDL_MapRGB(screen->format, 1, 0, 254));
		}
	else
		SDL_FillRect(gameLayer, 0, SDL_MapRGB(screen->format, 1, 0, 254));

	for(int i=0; i<players.size(); i++) if(players[i]->displayInfo)
	{
		drawInfo = true;
		//SDL_FillRect(infoLayer, 0, SDL_MapRGB(screen->format, 1, 0, 254));
		break;
	}

	for(int i=0; i<towers.size(); i++)
	{
		if(drawSpecialAttack == NULL && !inSight(towers[i]->x/P, towers[i]->y/P)) continue;
		towers[i]->draw(gameLayer);
		if(drawInfo) towers[i]->drawInfo(gameLayer);
	}

	for(int i=0; i<mobs.size(); i++) mobs[i]->draw(gameLayer);
	for(int i=0; i<bonuses.size(); i++) if(inSight(bonuses[i]->x*32+16, bonuses[i]->y*32+16)) bonuses[i]->draw(gameLayer);

	for(int i=0; i<towers.size(); i++) towers[i]->drawProjectiles(gameLayer);
	for(int i=0; i<towers.size(); i++) towers[i]->drawSpecialAttack(gameLayer);

	if(drawSpecialAttack != NULL)
	{
		pos.x = drawSpecialAttack->owner->x/P-16-screen->w/2;
		pos.y = drawSpecialAttack->owner->y/P-16-screen->h/2;
		pos.w = screen->w;
		pos.h = screen->h;
		SDL_BlitSurface(map->getBG(), &pos, screen, 0);
		SDL_BlitSurface(gameLayer, &pos, screen, 0);
		drawSpecialAttack->drawAnim(screen);
		return;
	}

	for(int i=0; i<players.size(); i++)
	{
		pos = players[i]->pos();
		screenPart = players[i]->screenPart();
		SDL_BlitSurface(map->getBG(), &pos, screen, &screenPart);
		screenPart = players[i]->screenPart();
		SDL_BlitSurface(gameLayer, &pos, screen, &screenPart);
		screenPart = players[i]->screenPart();
		/*if(players[i]->displayInfo)
		{
			SDL_BlitSurface(infoLayer, &pos, screen, &screenPart);
			screenPart = players[i]->screenPart();
		}*/
		for(int j=0; j<players[i]->splashes.size(); j++)
		{
			players[i]->splashes[j]->draw(screen);
		}
		rectangleRGBA(screen, screenPart.x, screenPart.y, screenPart.x+screenPart.w-1, screenPart.y+screenPart.h-1, 0xff, 0xff, 0xff, 0xff);
		char message[80] = "";
		sprintf(message, "ATK: %.0f%% | EXP: %.0f%% | REGEN: %.1f%% ", 100*(1.+(float)players[i]->bonusAtk/BONUS_ATK_FACTOR), 100*(1.+(float)players[i]->bonusExp/BONUS_EXP_FACTOR), 100*((float)players[i]->bonusRegen)/BONUS_REGEN_FACTOR);
		stringRGBA(screen, screenPart.x+10, screenPart.y+10, message, 255, 255, 255, 255);
		if(players[i]->friendly) sprintf(message, "GENTIL");
		else sprintf(message, "MOINS GENTIL");
		if(players[i]->getHead()->specialAttackAvailableId != -1) strcat(message, " S ");
		stringRGBA(screen, screenPart.x+10, screenPart.y+20, message, 255, 255, 255, 255);
		if(players[i]->shields > 0)
		{
			sprintf(message, "BOUCLIERS: %.0f%% | CHARGE: %.0f%% ", 100*SHIELD_FACTOR(players[i]->shields), 100*players[i]->shieldsLoadR());
			stringRGBA(screen, screenPart.x+10, screenPart.y+30, message, 255, 255, 255, 255);
		}



		//SDL_BlitSurface(map->getFG(), &pos, screen, 0);
	}


	if(players.size() == 3)
	{
		pos.x = 3*screen->w/4-map->getMiniMap()->w/2;
		pos.y = 3*screen->h/4-map->getMiniMap()->h/2;
		SDL_BlitSurface(map->getMiniMap(), 0, screen, &pos);
		pos.x = 3*screen->w/4-gameMiniMap->w/2;
		pos.y = 3*screen->h/4-gameMiniMap->h/2;
		genGameMiniMap(miniMapSize);
		SDL_BlitSurface(gameMiniMap, 0, screen, &pos);
	}
	else if(players.size() == 1)
	{
		pos.x = screen->w-map->getMiniMap()->w;
		pos.y = screen->h-map->getMiniMap()->h;
		SDL_BlitSurface(map->getMiniMap(), 0, screen, &pos);
		pos.x = screen->w-gameMiniMap->w-1;
		pos.y = screen->h-gameMiniMap->h-1;
		genGameMiniMap(miniMapSize);
		SDL_BlitSurface(gameMiniMap, 0, screen, &pos);
	}
	else
	{
		pos.x = screen->w/2-map->getMiniMap()->w/2;
		pos.y = screen->h/2-map->getMiniMap()->h/2;
		SDL_BlitSurface(map->getMiniMap(), 0, screen, &pos);
		pos.x = screen->w/2-gameMiniMap->w/2;
		pos.y = screen->h/2-gameMiniMap->h/2;
		genGameMiniMap(miniMapSize);
		SDL_BlitSurface(gameMiniMap, 0, screen, &pos);
	}
}

void Game::update()
{
	if(drawSpecialAttack != NULL)
	{
		if(drawSpecialAttack->updateAnim())
		{
			drawSpecialAttack = NULL;
		}
		return;
	}

	bool newTower;
	for(int i=0; i<players.size(); i++)
	{
		newTower = false;
		players[i]->update(&newTower);
		if(newTower) addTowerNearAnother(players[i]->getHead()->level+2-(rand() % 3));
	}
	for(int i=0; i<towers.size(); i++)
	{
		if(towers[i]->ownerId == -1) towers[i]->update(false, &newTower);
	}

	for(int iT = 0; iT<towers.size(); iT++)
	{
		if(towers[iT]->toDelete)
		{
			int dx, dy;
			if(towers[iT]->getType()->cat == ABOMINATION && towers[iT]->getType()->nextForm != -1)
			{
				towers.push_back(new Tower(this, towers[iT]->id, towers[iT]->x/P, towers[iT]->y/P, 0, 0, NULL, &(towerTypes[towers[iT]->getType()->nextForm]), -1, true));
			}
			getDirection(rand() % 4, dx, dy);
			if(! blockTaken(towers[iT]->x/P/32+dx, towers[iT]->y/P/32+dy)) bonuses.push_back(new Bonus(towers[iT]->x/P/32+dx, towers[iT]->y/P/32+dy));
			removeTower(iT);
			iT--;
		}
	}

	for(int i=0; i<mobs.size(); i++)
	{
		if(mobs[i]->update())
		{
			delete mobs[i];
			mobs.erase(mobs.begin()+i);
			i--;
		}
	}

	if(nextLevel)
	{
		endLevel();
		newLevel(800, 600);
	}
}

void Game::manageEvent(SDL_Event &ev)
{
	for(int i=0; i<players.size(); i++)
		players[i]->manageEvent(ev);
}

bool Game::blockTaken(int x, int y, bool spawn)
{
	for(int i=0; i<towers.size(); i++)
	{
		if(towers[i]->getOx() == x && towers[i]->getOy() == y) return true;
		if(towers[i]->getDx() == x && towers[i]->getDy() == y) return true;
	}
	return (! map->blockType(x, y)->pass) || (spawn && map->blockType(x, y)->nonspawn);
}

void Game::genGameMiniMap()
{
	blinkCounter++;
	if(blinkCounter >= 15) blinkCounter = 0;
	SDL_FillRect(gameMiniMap, 0, SDL_MapRGB(gameMiniMap->format, 1, 0, 254));
	for(int i=0; i<towers.size(); i++)
	{
		if(towers[i]->ownerId == -1 || blinkCounter < 5)
			filledRectangleRGB(gameMiniMap, towers[i]->x/P/32-1, towers[i]->y/P/32-1, towers[i]->x/P/32+1, towers[i]->y/P/32+1, 255, 255, 255);
		else
			filledRectangleRGB(gameMiniMap, towers[i]->x/P/32-1, towers[i]->y/P/32-1, towers[i]->x/P/32+1, towers[i]->y/P/32+1, towers[i]->owner->c[0], towers[i]->owner->c[1], towers[i]->owner->c[2]);

		if(towers[i]->appeared() > 0)
		{
			if(towers[i]->ownerId == -1) circleRGBA(gameMiniMap, towers[i]->x/P/32, towers[i]->y/P/32, towers[i]->appeared(), 255, 255, 255, 255);
			else circleRGBA(gameMiniMap, towers[i]->x/P/32, towers[i]->y/P/32, towers[i]->appeared(), towers[i]->owner->c[0], towers[i]->owner->c[1], towers[i]->owner->c[2], 255);
		}
	}
}

void Game::genGameMiniMap(int s)
{
	if(miniMapSize == 1)
	{
		genGameMiniMap();
		return;
	}
	blinkCounter++;
	if(blinkCounter >= 15) blinkCounter = 0;
	SDL_FillRect(gameMiniMap, 0, SDL_MapRGB(gameMiniMap->format, 1, 0, 254));
	for(int i=0; i<towers.size(); i++)
	{
		if(towers[i]->ownerId == -1 || blinkCounter < 5)
			filledRectangleRGB(gameMiniMap, s*towers[i]->x/P/32-s/2, s*towers[i]->y/P/32-s/2, s*towers[i]->x/P/32+s-s/2, s*towers[i]->y/P/32+s-s/2, 255, 255, 255);
		else
			filledRectangleRGB(gameMiniMap, s*towers[i]->x/P/32-s/2, s*towers[i]->y/P/32-s/2, s*towers[i]->x/P/32+s-s/2, s*towers[i]->y/P/32+s-s/2, towers[i]->owner->c[0], towers[i]->owner->c[1], towers[i]->owner->c[2]);

		if(towers[i]->appeared() > 0)
		{
			if(towers[i]->ownerId == -1) circleRGBA(gameMiniMap, s*towers[i]->x/P/32+s/2, s*towers[i]->y/P/32+s/2, towers[i]->appeared(), 255, 255, 255, 255);
			else circleRGBA(gameMiniMap, s*towers[i]->x/P/32+s/2, s*towers[i]->y/P/32+s/2, towers[i]->appeared(), towers[i]->owner->c[0], towers[i]->owner->c[1], towers[i]->owner->c[2], 255);
		}
	}
}

int Game::getBonus(int x, int y)
{
	for(int i=0; i<bonuses.size(); i++)
		if(bonuses[i]->x == x && bonuses[i]->y == y) return i;
	return -1;
}

Tower *Game::inRange(int x, int y, int r)
{
	for(int i=0; i<towers.size(); i++)
	{
		int rt;
		rt = (towers[i]->getType()->cat == ABOMINATION ? 96 : 32) + r;
		if((towers[i]->x/P-x)*(towers[i]->x/P-x)+(towers[i]->y/P-y)*(towers[i]->y/P-y) <= rt*rt) return towers[i];
	}
	return NULL;
}

void Game::removeTower(int idTower)
{
	/*for(int i=0; i<towers.size(); i++)
	{
		if(i != idTower)
			towers[i]->removeTarget(towers[idTower]);
	}*/

	delete towers[idTower];
	towers.erase(towers.begin()+idTower);
}

bool Game::inSight(int x, int y)
{
	for(int i=0; i<players.size(); i++)
	{
		if(x+48 > players[i]->getHead()->x/P-viewW/2 && x-48 < players[i]->getHead()->x/P+viewW/2
			&& y+48 > players[i]->getHead()->y/P-viewH/2 && y-48 < players[i]->getHead()->y/P+viewH/2)
			return true;
	}
	return false;
}
