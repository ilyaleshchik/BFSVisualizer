#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
using namespace sf;
using namespace std;
#pragma once


class Game{

private:
	int nScreenWidth, nScreenHeight;//screen quality
	int nFieldWidth, nFieldHeight;//field size
	int VisualSpeed = 0;//speed of visualization
	int **field;//field of points use to set START and FINIS and walls and for BFS

	bool started;//is GAME started
	bool isFinished;//is window closed

	float TIME = 0;//time var to manege speed
	float lastTime = 0;//to view FPS
	
	//sf objects for game
	Clock clock;
	RenderWindow *window;
	RectangleShape **rects;

	queue<pair<int, int> > q;//queue for BFS

	int dx[4] = {0, -1, 0, 1};//transitionts for BFS
	int dy[4] = {1, 0, -1, 0};
	int pathI, pathJ;//to find a path
	int I = -1, J = -1;//last drawing position
	int startI = 0, startJ = 0;//BFS Starting position
	int endI = 1, endJ = 1;//BFS ending position

	pair<int, int> **p;//parents array for BFS

	void BFS();
	void getPath();//path reconstruction
	void delPath();//delete old path when end point changed

public:
	Game(int nSW, int nSH, int nFW, int nFH, int VS);
	~Game();
	void init();
	void clear();
	void run();
	void handleEvent();
	void update();//next move in algorithm
	void render();//drawing all stuff
	void updateTime();//update var TIME
	float getFPS();//returns FPS in float
};