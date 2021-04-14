#ifndef BONUS_H
#define BONUS_H
#include "Sprite.h"

enum BonusType {BONUS_ATK, BONUS_EXP, BONUS_REGEN, BONUS_SPECATK};
extern vector<ImageList> bonusImages;
#define BONUS_ATK_FACTOR 40
#define BONUS_EXP_FACTOR 30
#define BONUS_REGEN_FACTOR 400


void loadBonusImages();

class Bonus
{
	public:
		enum BonusType type;
		Bonus(int x, int y);
		void draw(SDL_Surface *gameLayer);
		//Sprite *
		virtual ~Bonus();
		int x, y;
		int specAtkId;
	protected:
	private:

	unsigned char c;
	char dc;
};

#endif // BONUS_H
