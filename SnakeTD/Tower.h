#ifndef TOWER_H
#define TOWER_H
#include "Sprite.h"
#include "Explosion.h"
#include "Map.h"
#include "SDL_mixer.h"
#define playChunk(chunk) Mix_PlayChannel(-1, chunk, 0)

#define P 8
enum Category {OFFENSIVE, SHIELD, SLOW, SLOW_WPN, ABOMINATION, HEAD};
enum FollowMode {NONE, ACCEL, SPEED};
class Fluid;
#define BASE_EXP 1500
#define SHIELD_FACTOR(power) powf(powf(1.3, 2)+(float)power/1000, 0.5)

#define TOWER_FOURMI 14
typedef struct
{
	enum Category cat;
	int power;
	int life;
	int speed;
	int baseLevel;
	bool proj;
	bool ray;
	int rayThickness;
	unsigned char ray_r, ray_g, ray_b;
	int slowWpn;
	int slowWpnFactor;
	int slow;
	int slowFactor;
	int range;
	int proj_speed;
	int xoff, yoff;
	bool fluid;
	int fluidDensity;
	Fluid *fluidType;
	bool splash;
	bool alterHeading;
	bool multipleTargets;
	bool damageShields;
	bool blind;
	bool drain;
	int preferredAngle;
	FollowMode followMode;
	ImageList *towerImage;
	ImageList *projImage;
	ExplosionType *explType;
	ImageList *canonImage;
	Mix_Chunk *fireChunk;
	Mix_Chunk *impactChunk;
	int specialAttackTypeId;
	int nextForm;
//	Mix_Chunk *takeDamageChunk;
	int odds;
	char nom[20];
	char description[4][60];
} TowerType;

extern vector<TowerType> towerTypes;
extern ExplosionType *death, *wall;
extern vector<SDL_Surface *> *regenAnim;
extern vector<SDL_Surface *> *newLevelAnim;
extern Mix_Chunk *deathChunk, *newLevelChunk;


void loadTowerTypes();
class Tower;
class Player;
class Game;
class SpecialAttack;

class Thing
{
	public:
		virtual int getX() = 0;
		virtual int getY() = 0;
		int x, y;
};

class Target : public Thing
{
	public:
		int ownerId;
		int r;
		vector<Explosion *> explosions;
		virtual void takeDamage(int dmg) = 0;
};

class Projectile;

class Tower : public Target
{
	private:

		TowerType *type;
		Sprite *towerSprite;
		Sprite *canonSprite;

		vector<Projectile *> projectiles;



		int exp;
		int expNextLevel;

		float theta;
		float dtheta;
		float otheta;

		void updateDTheta(); // Mise à jour de l'angle en cas de changement de direction
		bool toAttach; // Doit-on attacher la tour suivante ?

		int nextRegen; // Prochaine récupération après destruction


		int brokenAnimationCounter;

		//Tower * _blockTaken(Map *map, vector<Tower *> *towers, int x, int y);
		Tower *onBlock(vector<Tower *> *towers, int x, int y);

		int lvlSpeed, lvlPower, lvlLife;
		bool shieldsEnabled;
		int appearedCounter;
		int slowedWpn;
		int slowedWpnFactor;

		bool isFiring;
		int soundChannel;
		void haltSound();

		//vector<Explosion *> explosions;

	public:
		Game *game;
		int ox, oy;
		int dx, dy;
		Tower *next;


		Tower(Game *game, int id, int x, int y, char _deltax, char _deltay, Tower *next, TowerType *type, int ownerId, bool appeared);
		//int x, y;
		int id;
		int ownerId;
		int life, speed, power;
		int level;
		int reload;
		int maxLife;
		int slow;
		int slowWpn;
		char deltax, deltay;
		Player *owner;

		int distance(Tower *t);


		inline TowerType *getType() {return type;}
		int nChilds() {if(next == NULL) return 1; else return 1+next->nChilds();}
		int sumLevels() {if(next != NULL) return level+next->sumLevels(); else return level;}



		void heading(char _deltax, char _deltay) {theta = dtheta; deltax = _deltax ; deltay = _deltay; updateDTheta(); };// Imposer la direction
		void attachTower(Tower *t);
		void setAttached(int _ownerId, Player *owner, int _x, int _y, char _deltax, char _deltay);
		void setPos(int ix, int iy) {x = P*(16+ix*32); y=P*(16+iy*32); ox = ix; oy = iy; deltax = 0; deltay = 1; dx = ox+deltax; dy = oy+deltay; if(next != NULL) next->setPos(ix, iy-1);}

		const int getOx() {return ox;}
		const int getOy() {return oy;}
		const int getDx() {return dx;}
		const int getDy() {return dy;}
		inline int getX() {return x;}
		inline int getY() {return y;}

		void update(bool move, bool *newTower);
		bool updatePos(bool move, bool *newTower);
		void updateTheta();
		void updateProjectiles();
		void updateFire();


		void takeDamage(int damage);
		void globalDamage(int damage);
		void die(); // Bouhouhou :'(
		void globalDie();
		void fullLife(); // Youpi ! :-)
		void destroyShieldGenerators();
		void regen(int l) {life += l; if(life > maxLife) fullLife();}
		void globalRegen(int l) {if(life > 0) regen(l); if(next != NULL) next->globalRegen(l);}
		void fire(Tower *target);
		void doSlowWpn(int factor, int dmg);

		void draw(SDL_Surface *gameLayer);
		void drawInfo(SDL_Surface *infoLayer);
		void drawProjectiles(SDL_Surface *gameLayer);
		void drawSpecialAttack(SDL_Surface *gameLayer);


		void gainExp(int n, bool bonusExp = false);
		void nextLevel();
		void gainLevels(int n);
		void globalGainExp(int n, bool bonusExp = false) {gainExp(n, bonusExp); if(next != NULL) next->globalGainExp(n, bonusExp);}

		bool shieldsReloading();


		void printPos(SDL_Surface *screen, int y);
		void removeTarget(Tower *target);


		~Tower();

		Tower *getEq(TowerType *t) {if(type == t) return this; if(next != NULL) return next->getEq(t); else return NULL;}
		inline int appeared() {return appearedCounter;}
		bool toDelete;

		SpecialAttack *specialAttack;
		int specialAttackAvailableId;
		void resetReloadCounter() {reload = 0;}

		void engageSpecialAttack();
		void endSpecialAttack();

		void playContinuousDamageAnimation();
		void newExplosion(Explosion *e) {explosions.push_back(e);}
};


Tower *onBlock(int x, int y);
#endif
