#include "game.h"
using namespace sf;



int main(){	

 	
	Game *g = new Game(800, 800, 20, 20, 10);
	g->init();
	cerr << "LMB - draw wall\n";
	cerr << "RMB - delete wall\n";
	cerr << "X - set start point(can't be changed after you started)\nZ - chenge end point\nS - start procces\n";
	g->run();


	return 0;	
}