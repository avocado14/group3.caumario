#include<bangtal.h>
#include <iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>



SceneID scene_g1;
ObjectID g1c1, g1obj1[6][10],g1startbutton, g1restartbutton;//[세로][가로][j][i]
TimerID g1timer1, g1c1move,g1levelupgrade,g1score;
SoundID theme;

int obj1x[6][10], obj1y[6][10];
int obj1speed=10;
double g1obj1moveblockx[6][10], g1obj1moveblocky[6][10];



int speed = 25;
double changex, changey;

int g1c1x=500, g1c1y=200;
int g1dx = 0, g1dy = 0;

ObjectID g1obj1_1[30];
int g1difficulty, obj1_1x[30], obj1_1y[30];
int obj1_1speed = 15;
double g1obj1_1moveblockx[30], g1obj1_1moveblocky[30];


ObjectID g1createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return(object);
}

void g1obj1firstposition() {// 리턴 해주어야 하나?.. i가 가로j가 세로
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {			
			obj1x[j][i] =0;
			obj1y[j][i] =0;
		}
	}
	for (int i = 0; i < 10; i++) {
		obj1x[0][i] = i * 128 ;
		obj1y[0][i] = 721; //-------------
		g1obj1[0][i] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[0][i], obj1y[0][i], true);
	}
	for (int j = 0; j < 6; j++) {
		//|           |
		//|           |
		obj1x[j][0] = 0 - 11;
		obj1y[j][0] = 720 - j * 120;
		g1obj1[j][0] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[j][0], obj1y[j][0], true);
		obj1x[j][9] = 1281;
		obj1y[j][9] = 720 - j * 120;
		g1obj1[j][9] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[j][9], obj1y[j][9], true);
	}
	for (int i = 0; i < 10; i++) {
		obj1x[5][i] = i * 128;
		obj1y[5][i] = 0 - 11; //-------------
		g1obj1[5][i] = g1createObject("image/game1/10pixel.png", scene_g1, obj1x[5][i], obj1y[5][i], true);
	}
}
void g1obj1_1firstposition() {
	srand((unsigned int)time(NULL) );
	for (int i = 0; i < 30; i++) {

		
		int num = rand();
		int g1obj1_1place = num % 4;
		int g1obj1_1placernd = num % 7;
		if (g1obj1_1place == 0) { //동
			obj1_1x[i] = 1281;
			obj1_1y[i] = 30+50* g1obj1_1placernd;
		}
		else if (g1obj1_1place == 1) {//서
			obj1_1x[i] = -15;
			obj1_1y[i] = 30 + 50 * g1obj1_1placernd;
		}
		else if (g1obj1_1place == 2) {//남
			obj1_1x[i] = 50 + 150 * g1obj1_1placernd;
			obj1_1y[i] = -15;
		}
		else if (g1obj1_1place == 3) {//북
			obj1_1x[i] = 50 + 150 * g1obj1_1placernd;
			obj1_1y[i] = 721;
		}
		g1obj1_1[i] = g1createObject("image/game1/10pixel1.png", scene_g1, obj1_1x[i], obj1_1y[i], true);

	}
}

void g1clearobj() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			hideObject(g1obj1[j][i]);

		}
	}
	for (int i = 0; i < 30; i++) {		
			hideObject(g1obj1_1[i]);		
	}
	
}

void g1obj1movepinpoint() {//중앙의 540*360 안의 사각형을 무조건 지나면서 이동
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if (obj1x[j][i] == 0 && obj1y[j][i] == 0) {//pass 요거는 가장자리에 있는 거 만 포인트를 정해주고 안쪽에 있는거는 무시
			}
			else if (obj1x[j][i] > 0 && obj1y[j][i]<720 && obj1x[j][i] < 1280 && obj1y[j][i] > 0) {//화면 안에 있을때는 할 필요 없음
			}
			else {
				
				int num = rand();
				int rndvaluex = (num % 18)*30;//가로 540
				int rndvaluey = (num % 12)*30;//세로 360
				double distance;
				double ratio;

				distance = sqrt((rndvaluex+270 - obj1x[j][i]) * (rndvaluex+270 - obj1x[j][i]) + (rndvaluey+180 - obj1y[j][i]) * (rndvaluey+180 - obj1y[j][i]));
				ratio = distance / obj1speed;
				g1obj1moveblockx[j][i] = (rndvaluex+270 - obj1x[j][i]) / ratio;
				g1obj1moveblocky[j][i] = (rndvaluey+180 - obj1y[j][i]) / ratio;
				
			}
		}
	}
	for (int i = 0; i < 30; i++) {

		if (obj1_1x[i] > 0 && obj1_1y[i] < 720 && obj1_1x[i] < 1280 && obj1_1y[i] > 0) {
		}
		else {
			
			int num = rand();
			int rndvaluex = (num % 9) * 60;//가로 540
			int rndvaluey = (num % 6) * 60;//세로 360
			double distance;
			double ratio;

			distance = sqrt((rndvaluex + 270 - obj1_1x[i]) * (rndvaluex + 270 - obj1_1x[i]) + (rndvaluey + 180 - obj1_1y[i]) * (rndvaluey + 180 - obj1_1y[i]));
			ratio = distance / obj1_1speed;
			g1obj1_1moveblockx[i] = (rndvaluex + 270 - obj1_1x[i]) / ratio;
			g1obj1_1moveblocky[i] = (rndvaluey + 180 - obj1_1y[i]) / ratio;

		}
	}
}
void g1obj1move() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if (obj1x[j][i] == 0 && obj1y[j][i]==0) {//pass
			}
			else {
				obj1x[j][i] += g1obj1moveblockx[j][i];
				obj1y[j][i] += g1obj1moveblocky[j][i];
				locateObject(g1obj1[j][i], scene_g1, obj1x[j][i], obj1y[j][i]);
			}
		}
	}
	for (int i = 0; i < 30; i++) {
		obj1_1x[i] += g1obj1_1moveblockx[i];
		obj1_1y[i] += g1obj1_1moveblocky[i];
		locateObject(g1obj1_1[i], scene_g1, obj1_1x[i], obj1_1y[i]);
	}
	
}

void score(){
	stopTimer(g1score);
	double g1result;
	g1result = getTimer(g1score);
	char buf[256];
	sprintf_s(buf, "버틴시간 : %0.2f초", (100-g1result), scene_g1);
	showMessage(buf);
}

void g1death() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			if ((g1c1x < (obj1x[j][i] + 10)) && (g1c1x + 30 > obj1x[j][i]) && (g1c1y < (obj1y[j][i] + 10)) && (g1c1y + 40 > obj1y[j][i])) {
				stopTimer(g1timer1);
				stopTimer(g1c1move);
				showMessage("end");
				score();
			}
			else if (g1c1x < 0 || g1c1x > 1280 || g1c1y < 0 || g1c1y > 720) {
				stopTimer(g1timer1);
				stopTimer(g1c1move);
				showMessage("out of bounds");
			}
		}
	}
	for (int i = 0; i < 30; i++) {

		if ((g1c1x < (obj1_1x[i] + 10)) && (g1c1x + 30 > obj1_1x[i]) && (g1c1y < (obj1_1y[i] + 10)) && (g1c1y + 40 > obj1_1y[i])) {
			stopTimer(g1timer1);
			stopTimer(g1c1move);
			showMessage("end");
			score();
		}
	}
}
	



//g1difficulty++;
//setTimer(g1levelupgrade, 0.01f);


	void Game1_mouseCallback(ObjectID object, int x, int y, MouseAction action) {


		if (object == g1startbutton) {
			startTimer(g1c1move);
			//g1objmovepinpoint();
			startTimer(g1timer1);




			setTimer(g1score, 100);
			showTimer(g1score);
			startTimer(g1score);


		}
		else if (object == g1restartbutton) {
			g1clearobj();
			g1obj1firstposition();
			g1obj1_1firstposition();
			g1difficulty = 0;
			startTimer(g1c1move);

			startTimer(g1timer1);



			setTimer(g1score, 100);
			showTimer(g1score);
			startTimer(g1score);
		}
	}
		

void Game1_timerCallback(TimerID timer) {// 크리에이트 타이머!!!!!!!!!!!
	if (timer == g1timer1) {//장애물움직이는거 전용
		setTimer(g1timer1, 0.01f);
		startTimer(g1timer1);
		g1obj1movepinpoint();
		g1obj1move();
		g1death();
		

	}
	if (timer == g1c1move) {//캐릭터움직이는거 전용
		g1c1x += g1dx; g1c1y += g1dy;
		locateObject(g1c1, scene_g1, g1c1x, g1c1y);
		setTimer(g1c1move, 0.01f);
		startTimer(g1c1move);
	}
	if (timer == g1levelupgrade) {//캐릭터움직이는거 전용		
		startTimer(g1levelupgrade);
	}
	if (timer == g1score) {
		
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
	scaleObject(g1c1, 0.5f);
	
	
	g1obj1firstposition();
	g1obj1_1firstposition();

	g1startbutton = g1createObject("image/game1/확인.png", scene_g1, 500, 110, true);
	g1restartbutton = g1createObject("image/game1/다시시작.png", scene_g1, 600, 500, true);


	
	
	g1timer1 = createTimer(0.01f);
	g1c1move = createTimer(0.01f);
	g1score = createTimer(100);
	
}