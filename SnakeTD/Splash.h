#ifndef SPLASH_H
#define SPLASH_H
#include <SDL.h>

class Splash
{
	public:
		bool update();
		void draw(SDL_Surface *screen);
		Splash(int ampl, SDL_Rect screenPart);
	protected:
	private:
		int counter;
		int radius;
		int x, y;
};

#endif // SPLASH_H
