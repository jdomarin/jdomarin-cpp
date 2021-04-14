#ifndef PLAYER_H
#define PLAYER_H
#include "Sprite.h"
#include "Map.h"
#include "Tower.h"
#include "Splash.h"

class Game;

class Player
{
	public:
		Player(Game *game, int id, int w, int h, int xs, int ys, unsigned char r, unsigned char g, unsigned char b);
		void manageEvent(SDL_Event ev);
		int id;
		inline Tower *getHead() {return head;};

		SDL_Rect pos();
		SDL_Rect screenPart();

		int shield;
		const int speed() {return _speed;}

		void increaseSpeed();
		void normalSpeed();
		void reduceSpeed(int factor, int time);

		void update(bool *newTower);

		bool displayInfo;

		int shields;
		void takeDamageOnShields(int damage);
		float shieldsLoadR() {return (float)shieldsLoad/shields;}
		int shieldsLoad;
		void regenSpeed();

		unsigned char c[3];


		int bonusAtk;
		int bonusRegen;
		int bonusExp;

		void newSplash(int ampl) {if(splashes.size() < 30) splashes.push_back(new Splash(ampl, screenPart()));}
		vector<Splash *> splashes;
		bool friendly;


	protected:
	private:
		Game *game;
		Tower *head;
		int w, h, xs, ys;
		int _speed;
		int oldSpeed;
		int requestSpeed;
		int timeLowSpeed;
		int timeLowSpeedElapsed;




		SDL_Rect clipRect;

};

#endif // PLAYER_H
