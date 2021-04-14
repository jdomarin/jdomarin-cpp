#ifndef APPEL_H
#define APPEL_H

#include "SpecialAttack.h"

class Appel : public SpecialAttack
{
	public:
		Appel(Tower *owner);
		bool update();
		void draw(SDL_Surface *gameLayer);
		~Appel();
	protected:
	private:
		int appelCounter;
		bool preparation;
		int endCounter;
};

#endif // APPEL_H
