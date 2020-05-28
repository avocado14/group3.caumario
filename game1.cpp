#include<bangtal.h>
#include <iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>



SceneID scene_g1;
ObjectID g1c1, g1obj1[6][10],g1startbutton, g1restartbutton;//[세로][가로]
TimerID g1timer1, g1c1move;
SoundID theme;

int obj1x[6][10], obj1y[6][10];
int objspeed=10;
double g1objmoveblockx, g1objmoveblocky;



int speed = 20;
double changex, changey;

int g1c1x=500, g1c1y=200;
int g1dx = 0, g1dy = 0;

ObjectID g1createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return(object);
}

void g1objfirstposition() {// 리턴 해주어야 하나?.. i가 가로j가 세로
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			
			obj1x[j][i] =0;
			obj1y[j][i] =0;

		}
	}
	for (int i = 0; i < 10; i++) {
		obj1x[0][i] = i * 128;
		obj1y[0][i] = 320; //-------------
		g1obj1[0][i] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[0][i], obj1y[0][i], true);
	}
	for (int j = 0; j < 6; j++) {
		obj1x[j][0] = 0 - 10;
		obj1y[j][0] = 720 - j * 120;
		g1obj1[j][0] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[j][0], obj1y[j][0], true);
		obj1x[j][9] = 1280;
		obj1y[j][9] = 720 - j * 120;//|       |
		g1obj1[j][9] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[j][9], obj1y[j][9], true);
	}
	for (int i = 0; i < 10; i++) {
		obj1x[5][i] = i * 128;
		obj1y[5][i] = 0 - 10; //-------------
		g1obj1[5][i] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[5][i], obj1y[5][i], true);
	}
}


void g1objmovepinpoint() {//중앙의 540*360 안의 사각형을 무조건 지나면서 이동
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if (obj1x[j][i] == 0 && obj1y[j][i]) {//pass
			}
			else {
				srand((unsigned int)time(NULL));
				int num = rand();
				int rndvaluex = num % 540;
				int rndvaluey = num % 360;
				double distance;
				double ratio;

				distance = sqrt((rndvaluex+270 - obj1x[j][i]) * (rndvaluex+270 - obj1x[j][i]) + (rndvaluey+180 - obj1y[j][i]) * (rndvaluey+180 - obj1y[j][i]));
				ratio = distance / objspeed;
				g1objmoveblockx = (rndvaluex+270 - obj1x[j][i]) / ratio;
				g1objmoveblocky = (rndvaluey+180 - obj1y[j][i]) / ratio;
				locateObject(g1obj1[j][i], scene_g1, obj1x[j][i], obj1y[j][i]);
			}
		}
	}
}
void g1objmove() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if (obj1x[j][i] == 0 && obj1y[j][i]) {//pass
			}
			else {
				obj1x[j][i] += g1objmoveblockx;
				obj1y[j][i] += g1objmoveblocky;
				locateObject(g1obj1[j][i], scene_g1, obj1x[j][i], obj1y[j][i]);
			}
		}
	}
}


void Game1_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	
	if (object == g1startbutton) {
		startTimer(g1c1move);
		g1objmovepinpoint();
		startTimer(g1timer1);
	}
		
}
void Game1_timerCallback(TimerID timer) {
	if (timer == g1timer1) {//장애물움직이는거 전용
		g1objmove();
			
		setTimer(g1timer1, 0.01f);
		startTimer(g1timer1);
	}
	if (timer == g1c1move) {//캐릭터움직이는거 전용
		g1c1x += g1dx; g1c1y += g1dy;
		locateObject(g1c1, scene_g1, g1c1x, g1c1y);
		setTimer(g1c1move, 0.01f);
		startTimer(g1c1move);
	}
}
void Game1_soundCallback(SoundID sound) {
	if (sound == theme) {

	}

}
void Game1_keyboardCallback(KeyCode code, KeyState state)
{
	if (code == 84) {			// UP
		g1dy += (state == KeyState::KEYBOARD_PRESSED ? speed : -speed);
	}
	else if (code == 85) {		// DOWN
		g1dy -= (state == KeyState::KEYBOARD_PRESSED ? speed : -speed);
	}
	else if (code == 83) {		// RIGHT
		g1dx += (state == KeyState::KEYBOARD_PRESSED ? speed : -speed);
	}
	else if (code == 82) {		// LEFT
		g1dx -= (state == KeyState::KEYBOARD_PRESSED ? speed : -speed);
	}
}

	


void Game1_main() {
	
	
	scene_g1 = createScene("game1", "image/game1/배경.png");

	g1c1 = g1createObject("image/game1/c1.png", scene_g1, g1c1x, g1c1y, true);	
	
	
	g1objfirstposition();
	
	g1startbutton = g1createObject("image/game1/확인.png", scene_g1, 500, 110, true);
	g1restartbutton = g1createObject("image/game1/다시시작.png", scene_g1, 600, 500, true);


	
	
	g1timer1 = createTimer(0.01f);
	g1c1move = createTimer(0.01f);
	
	
}