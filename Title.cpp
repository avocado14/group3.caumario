#define _CRT_SECURE_NO_WARNINGS
#include<bangtal.h>
#include <stdio.h>

SceneID titleScene;
ObjectID GameButton[6], Titleimage, Titlestart, Titlesave;
TimerID Titletimer1;
int Titleimagex=0;

char path[256];

extern SceneID scene_g1, scene1_g2, scene1_g3, scene_g4, scene_g5, scene_g6, scene_g62, scene_g63;

extern SoundID buttonClickSound, gameClearSound, gameOverSound, gameEnterSound;
extern SoundID bgm_g5, bgm_g6,g4theme, bgm_g1;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

int nowGameSceneNum = 0;	//0은 타이틀
extern int nowGame6Stage;


int coin;


void Titleimageanimation() {
	
	Titleimagex += 50;
	locateObject(Titleimage, titleScene, Titleimagex, 0);
	if (Titleimagex >1280) {
		stopTimer(Titletimer1);
	}
}

void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action) {


	if (object == GameButton[0]) {
		nowGameSceneNum = 3;

		playSound(buttonClickSound);
		enterScene(scene1_g3);
	}

	else if (object == GameButton[1]) {
		nowGameSceneNum = 4;

		playSound(buttonClickSound);
		enterScene(scene_g4);
		playSound(g4theme);
	}

	else if (object == GameButton[2]) {
		nowGameSceneNum = 2;

		playSound(buttonClickSound);
		enterScene(scene1_g2);
	}

	else if (object == GameButton[3]) {
		nowGameSceneNum = 1;

		playSound(buttonClickSound);
		enterScene(scene_g1);
		
		playSound(bgm_g1);
	}

	else if (object == GameButton[4]) {
		nowGameSceneNum = 6;
	
		switch (nowGame6Stage) {
			case 1: 
				enterScene(scene_g6);
				break;
			case 2:
				enterScene(scene_g62);
				break;
			case 3:
				enterScene(scene_g63);
				break;
		}
		playSound(buttonClickSound);
		playSound(bgm_g6);
	}

	else if (object == GameButton[5]) {
		nowGameSceneNum = 5;

		enterScene(scene_g5);
		playSound(buttonClickSound);
		playSound(bgm_g5);
	}

	else if (object == Titlestart) {
		playSound(buttonClickSound);
		startTimer(Titletimer1);
		hideObject(Titlestart);
	}
	else if (object == Titlesave) {
		
	}
	
}

void Title_timerCallback(TimerID timer) {
	if (timer == Titletimer1) {
		Titleimageanimation();
		setTimer(Titletimer1, 0.01f);
		startTimer(Titletimer1);
	}
}


void Title_main() {

	titleScene = createScene("타이틀(임시)", "image/Title/map ui.png");

	
	GameButton[0] = createObject("image/Title/game1.png", titleScene, 78, 80, true, 1.f);
	GameButton[1] = createObject("image/Title/game2.png", titleScene, 215, 405, true, 1.f);
	GameButton[2] = createObject("image/Title/game3.png", titleScene, 560, 155, true, 1.3f);
	GameButton[3] = createObject("image/Title/game4.png", titleScene, 810, 380, true, 1.3f);
	GameButton[4] = createObject("image/Title/game5.png", titleScene, 1130, 250, true, 1.3f);
	GameButton[5] = createObject("image/Title/game6.png", titleScene, 1070, 450, true, 1.3f);

	Titlesave = createObject("image/Title/save.png", titleScene, 420, 200, true, 1.f);
	Titleimage = createObject("image/Title/title.png", titleScene, 0, 0, true, 1.f);
	Titlestart = createObject("image/Title/start.png", titleScene, 420, 200, true, 1.f);
	Titletimer1 = createTimer(0.01f);
}