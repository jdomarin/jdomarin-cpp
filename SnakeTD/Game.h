#ifndef GAME_H
#define GAME_H
#include <limits.h>
#include "Map.h"
#include "Player.h"
#include "Projectile.h"
#include "Bonus.h"
#include "SpecialAttack.h"
#include "Slurp.h"
#include "Poulpofolie.h"
#include "Appel.h"
#include "Mob.h"

void getDirection (char c, int &x, int &y);

class Game
{
	public:
		Game(int nPlayers, SDL_Surface *screen);
		void draw(SDL_Surface *screen);
		void manageEvent(SDL_Event &ev);
		void update();
		int screenW, screenH;
		int viewW, viewH;
		vector<Player *> players;
		vector<Tower *> towers;
		vector<Bonus *> bonuses;
		Map *map;
		Tower **towerMap;

		bool blockTaken(int x, int y, bool spawn=true);
		int getBonus(int x, int y);
		void newBonus(int x, int y);
		void newSpecialAttack(SpecialAttack *specialAttack) {drawSpecialAttack = specialAttack;}
		Tower *inRange(int x, int y, int r);
		void launchNextLevel() {nextLevel = true;}
		SpecialAttack *drawSpecialAttack;
		int maxLevel;
		vector<Mob *> mobs;
		void removeTower(int i);
		int bonusExp;

		inline void occupied(Tower *t, int x, int y) {towerMap[y*map->width()+x] = t;}
		inline void notOccupied(int x, int y) {towerMap[y*map->width()+x] = NULL;}
		inline Tower *onBlock(int x, int y) {return towerMap[y*map->width()+x];}
		inline Tower *onBlockBC(int x, int y) {if(x >= 0 && y >= 0 && x < map->width() && y < map->height()) return onBlock(x, y); else return NULL;}

	protected:
	private:
		vector<Level *> levels;
		Level *level;
		int levelId;
		SDL_Surface *gameLayer;
		//SDL_Surface *infoLayer;

		SDL_Surface *gameMiniMap;
		int blinkCounter;
		int miniMapSize;

		int totalOdds;

		void loadLevels();
		void createBonus(Tower *t);

		void genGameMiniMap();
		void genGameMiniMap(int s); // Pour une minicarte avec des blocs de taille s
		void addTower(int level, bool appeared=false);
		void addTower(int x, int y, int level, bool appeared);
		void addTowerNearAnother(int level);

		void endLevel();
		void newLevel(int w, int h);
		bool nextLevel;
		bool inSight(int x, int y);
};

#endif // GAME_H
