#include "game.h"
using namespace sf;



int main(){	

 	
	Game *g = new Game(800, 800, 50, 50, 10);
	g->init();
	g->run();


	return 0;	
}