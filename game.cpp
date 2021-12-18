#include "game.h"

using namespace sf;
using namespace std;

Game::Game(int nSW, int nSH, int nFW, int nFH, int VS) { 
	//initialization of all data and memory
	nScreenWidth = nSW;
	nScreenHeight = nSH;
	nFieldWidth = nFW;
	nFieldHeight = nFH;
	field = new int*[nFieldHeight];
	isFinished = 0;
	VisualSpeed = VS;

	//memory allocation for field
	for(int i = 0; i < nFieldHeight; i++) {
		field[i] = new int[nFieldWidth];
	}
	for(int i = 0; i < nFieldHeight; i++) {
		for(int j = 0; j < nFieldWidth; j++) {
			field[i][j] = 0;
		}
	}
	//memory allocation for rectangles
	rects = new RectangleShape*[nFieldHeight];
	for(int i = 0; i < nFieldHeight; i++)
		rects[i] = new RectangleShape[nFieldWidth];
	for(int i = 0; i < nFieldHeight; i++) {
		for(int j = 0; j < nFieldWidth; j++) {
			rects[i][j] = RectangleShape(Vector2f(int(nScreenHeight / nFieldHeight) - 1.f, int(nScreenWidth / nFieldHeight) - 1.f));
			rects[i][j].move(i * int(nScreenHeight / nFieldHeight), j * int(nScreenWidth / nFieldWidth));
			rects[i][j].setFillColor(Color(255, 255, 255));
		}
	}

	//memory allocation for perents array
	p = new pair<int, int>*[nFieldHeight];
	for(int i = 0; i < nFieldHeight; i++)
		p[i] = new pair<int, int>[nFieldWidth];

	for(int i = 0; i < nFieldHeight; i++) {
		for(int j = 0; j < nFieldWidth; j++) {
			p[i][j] = {-1, -1};
		}
	}

}


Game::~Game() {

	for(int i = 0; i < nFieldHeight; i++) {
		delete[] field[i];
		delete[] rects[i];
		delete[] p[i];
	}
	delete[] field;
	delete[] rects;
	delete[] p;
	delete window;

}

void Game::init() {
	//initialization of gameobjects
	window = new RenderWindow(VideoMode(nScreenWidth, nScreenHeight), "BFS");
}


void Game::clear() {
	startI = 0; startJ = 0;
	endI = 1; endJ = 1;
	for(int i = 0; i < nFieldHeight; i++)
		for(int j = 0; j < nFieldWidth; j++) {
			rects[i][j].setFillColor(Color(255, 255, 255));
			field[i][j] = 0;
			p[i][j] = {-1, -1};
		}
	I = -1; J = -1;
	started = 0;
	while(!q.empty()) q.pop();
}



void Game::run() {

	while(!isFinished) {

		handleEvent();
		//cerr << getFPS() << endl;
		updateTime();
		if(started && TIME >= VisualSpeed)
			update();
		render();
	}

}

void Game::setStartPosition() {
	if(started) return;
	Vector2i pos = Mouse::getPosition(*window);
	int i = int(pos.x) / int(nScreenHeight / nFieldHeight);
	int j = int(pos.y) / int(nScreenWidth / nFieldWidth);
	if(!(i >= nFieldHeight || j >= nFieldWidth || i < 0 || j < 0) && field[i][j] != 2) {
		field[startI][startJ] = 0;
		rects[startI][startJ].setFillColor(Color(255, 255, 255));
		startI = i; startJ = j;
		rects[startI][startJ].setFillColor(Color(242, 8, 53));
		field[startI][startJ] = 2;
	}
}
void Game::setEndPosition() {

	Vector2i pos = Mouse::getPosition(*window);
	int i = pos.x / int(nScreenHeight / nFieldHeight);
	int j = pos.y / int(nScreenWidth / nFieldWidth);
	if(!(i >= nFieldHeight || j >= nFieldWidth || i < 0 || j < 0) && field[i][j] != 2 && field[i][j] != 3) {
       	if(started) delPath();
       	else {
			field[endI][endJ] = 0;
	       	rects[endI][endJ].setFillColor(Color(255, 255, 255));
       	}
		endI = i;  endJ = j;
		rects[endI][endJ].setFillColor(Color(168, 0, 252));
		field[endI][endJ] = 2;
	}
}

void Game::handleEvent() {


	Event ev;
	while(window->pollEvent(ev)) {	
		if(ev.type == Event::Closed)
        	isFinished = 1;
        else if(ev.type == Event::KeyPressed) {
        	if(ev.key.code == Keyboard::S) {
        		started = 1;
        		q.push({startI, startJ});
        		field[startI][startJ] = 1;
        	}else if(ev.key.code == Keyboard::X) {
        		setStartPosition();
        	}else if(ev.key.code == Keyboard::Z) {
        		setEndPosition();
        	}else if(ev.key.code == Keyboard::C) {
        		clear();
        	}
        }

        //MOUSE CLICKS
        if (Mouse::isButtonPressed(Mouse::Left) && !started) {
        	Vector2i pos = Mouse::getPosition(*window);
            int i = pos.x / int(nScreenHeight / nFieldHeight);
            int j = pos.y / int(nScreenWidth / nFieldWidth);
            if(!(i >= nFieldHeight || j >= nFieldWidth || i < 0 || j < 0) && field[i][j] != 2) {
            	rects[i][j].setFillColor(Color(73, 78, 78));
            	field[i][j] = 3;
            }
        }else if(Mouse::isButtonPressed(Mouse::Right) && !started) {
        	Vector2i pos = Mouse::getPosition(*window);
            int i = pos.x / int(nScreenHeight / nFieldHeight);
            int j = pos.y / int(nScreenWidth / nFieldWidth);
            if(!(i >= nFieldHeight || j >= nFieldWidth || i < 0 || j < 0) && field[i][j] != 2) {
	            rects[i][j].setFillColor(Color(255, 255, 255));
	            field[i][j] = 0;
            }
        }
	}
}

void Game::BFS() {
	if(I >= 0 && !(I == startI && J == startJ) && !(I == endI && J == endJ)) {
		rects[I][J].setFillColor(Color(137, 151, 229));
	}//changing prev cell color

	pair<int, int> cur = q.front();
	q.pop();

	//changing current cell color
	if(!(cur.first == startI && cur.second == startJ) && !(cur.first == endI && cur.second == endJ))
		rects[cur.first][cur.second].setFillColor(Color(52, 212, 203));


	I = cur.first; J = cur.second; //save this cell to previous
	//transitions in 4 ways
	for(int i = 0; i < 4; i++) {
		int curX = cur.first + dx[i], curY = cur.second + dy[i];
		if(curX < 0 || curX >= nFieldHeight || curY < 0 || curY >= nFieldWidth) continue;
		if(field[curX][curY] == 1 || field[curX][curY] == 3) continue;
		q.push({curX, curY});
		field[curX][curY] = 1;
		p[curX][curY] = {cur.first, cur.second};
	}
}


void Game::delPath() {

	//delet old path
	for(pair<int, int>& cur: path) {
		rects[cur.first][cur.second].setFillColor(Color(137, 151, 229));
	}
	if(endI != startI && endJ != startJ)
		rects[endI][endJ].setFillColor(Color(255, 255, 255));
	else
		field[path[0].first][path[0].second] = 1;
	path.clear();			
}

void Game::getPath() {

	if(p[endI][endJ] == make_pair(-1, -1)) return;//if it's end point

	rects[endI][endJ].setFillColor(Color(95, 217, 85));
	path.push_back({endI, endJ});
	int nI = p[endI][endJ].first;
	int nJ = p[endI][endJ].second;
	endI = nI;//transition to parent point
	endJ = nJ;//transition to parent point
}

void Game::update() {
	//BFS code here

	TIME = 0;
	if(!q.empty()) BFS();
	else {
		if(I >= 0 && !(I == startI && J == startJ)) {
			rects[I][J].setFillColor(Color(137, 151, 229));
			I = -1; J = -1;
		}
		if(p[endI][endJ] != make_pair(-1, -1)) //if we need didn't find all path already
			getPath();
	}

}
void Game::render() {

	window->clear(Color(0, 0, 0, 0));//clear window
		
	for(int i = 0; i < nFieldHeight; i++) {
		for(int j = 0; j < nFieldWidth; j++) {
			window->draw(rects[i][j]);//draw rects
		}
	}
	window->display();

}

void Game::updateTime() {
	float time = clock.getElapsedTime().asMicroseconds();
	clock.restart();
	TIME += time / 500.0;
}

float Game::getFPS() { 
	float curTime = clock.getElapsedTime().asSeconds();
	float fps = 1.f / (curTime - lastTime);
	lastTime = curTime;
	return fps;
}