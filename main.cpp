#include "game.h"
using namespace sf;



int main(){	

 	
	Game *g = new Game(800, 800, 20, 20, 10);
	g->init();
	g->run();


	return 0;	
}