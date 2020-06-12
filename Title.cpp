#include<bangtal.h>
#include <stdio.h>

#define MARIO_ANIMATION_TIME	0.1f


extern SceneID scene_g1, scene1_g2, scene1_g3, scene_g4, scene_g5, scene_g6, scene_g62, scene_g63;

extern SoundID buttonClickSound, gameClearSound, gameOverSound, GameEnterSound;
extern SoundID bgm_g5, bgm_g6;
SoundID bgmList[6] = { bgm_g5, bgm_g5, bgm_g5, bgm_g5, bgm_g5, bgm_g6 };

SceneID titleScene;
ObjectID GameIcon[6], GamePopup[6], GameEnterButton[6];
ObjectID Mario;
TimerID marioAnimationTimer;
SoundID bgm_title;

int IconX[6] = { 30,22,570,560,1050,1170 };
int IconY[6] = { 115,385,215,565,275,465 };

bool stageLocked[6] = { 0,1,0,1,1,1 };
bool stageBlack[6] = { 0,0,0,0,0,0 };		//0,0,1,1,1,0
int stageUnlockCost[6] = { 0, 300, 0,300,1500,0 };	//6번째 : 열쇠 2개 필요
int coin = 5000;
int key = 2;

int nowMarioOn = 0;
int nowGameSceneNum = 0;	//0은 타이틀
extern int nowGame6Stage;

char path[256];

int marioAnimationCount = 0;
const char* marioAnimationImage[9] =
{ "image/Title/마리오애니/1.png","image/Title/마리오애니/2.png","image/Title/마리오애니/3.png","image/Title/마리오애니/4.png",
"image/Title/마리오애니/5.png","image/Title/마리오애니/6.png","image/Title/마리오애니/7.png","image/Title/마리오애니/8.png","image/Title/마리오애니/9.png" };



extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);


void exitTitle() {

	stopTimer(marioAnimationTimer);
	stopSound(bgm_title);

}

void hideUI() {

	for (int i = 0; i < 6; i++) {
		hideObject(GameEnterButton[i]);
		hideObject(GamePopup[i]);
	}
}

//0은 클리어x 2스테이지(사막), 5스테이지(하늘) 받음 
void enterTitle(int clearScene) {

	hideUI();
	setTimer(marioAnimationTimer, MARIO_ANIMATION_TIME);
	startTimer(marioAnimationTimer);

	playSound(bgm_title);

	//사막지대 클리어했으면
	if (clearScene == 2) {
		setObjectImage(GameIcon[1], "image/Title/초록아이콘.png");
		setObjectImage(GameIcon[2], "image/Title/파란아이콘.png");
		setObjectImage(GameIcon[3], "image/Title/파란아이콘.png");
		stageBlack[2] = false;
		stageBlack[3] = false;
		
		key = 1;
	}

	//하늘지대 클리어했으면
	else if (clearScene == 5) {
		setObjectImage(GameIcon[4], "image/Title/초록아이콘.png");
		
		key = 2;
	}

	enterScene(titleScene);
}

//0번 2번은 실행 안됨
void stageUnlock(int stage) {

	if (stage != 5 ) {
		if (coin >= stageUnlockCost[stage]) {

			coin -= stageUnlockCost[stage];		//돈 내고
			stageLocked[stage] = false;			//열림으로 바꿔주고
			
			sprintf_s(path, "image/Title/팝업/%d-2.png", stage + 1);
			setObjectImage(GamePopup[stage], path);
			setObjectImage(GameEnterButton[stage], "image/Title/enter.png");
			locateObject(GameEnterButton[stage], titleScene, 950, 100);

			if(stage == 4)
				locateObject(GameEnterButton[stage], titleScene, 70, 100);

		}

		else
			showMessage("돈이 부족합니다");
	}

	//쿠파성
	else  {
		if (key == 2) {

			stageLocked[5] = false;

			setObjectImage(GamePopup[5], "image/Title/팝업/6-2.png");
			setObjectImage(GameEnterButton[5], "image/Title/enter.png");
			locateObject(GameEnterButton[stage], titleScene, 70, 100);
		}
		
		else
			showMessage("열쇠가 부족합니다");
	}
}

void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	

	//검은아이콘(클릭불가)이 아니면 팝업창 띄움
	for (int i = 0; i < 6; i++) {
		
		if (object == GameIcon[i] && stageBlack[i] == false) {
			playSound(buttonClickSound);
			hideUI();
			showObject(GamePopup[i]);
			showObject(GameEnterButton[i]);

			locateObject(Mario, titleScene, IconX[i], IconY[i]);
			nowMarioOn = i;
		}

		else if (object == GameIcon[i] && stageBlack[i] == true) {
			showMessage("이전 스테이지를 클리어 해주세요");
		}
	}

	////팝업창 클릭시 사라짐//////
	for (int i = 0; i < 6; i++) {
		playSound(buttonClickSound);

		if (object == GamePopup[i]) {
			hideObject(GamePopup[i]);
			hideObject(GameEnterButton[i]);
		}
	}
	
	if (object == Mario) {
		
		if (stageBlack[nowMarioOn] == false) {
			playSound(buttonClickSound);
			hideUI();
			showObject(GamePopup[nowMarioOn]);
			showObject(GameEnterButton[nowMarioOn]);
		}
	}

	
	///////게임 입장 버튼//////
	//평원
	else if (object == GameEnterButton[0]) {
		nowGameSceneNum = 3;
		
		hideObject(GamePopup[0]);
		hideObject(GameEnterButton[0]);

		playSound(buttonClickSound);
		exitTitle();
		enterScene(scene1_g3);
	}

	//사막
	else if (object == GameEnterButton[1]) {

		//잠겨있으면 해금
		if (stageLocked[1]) {
			stageUnlock(1);
		}

		//열려있으면 이동
		else {
			nowGameSceneNum = 4;

			hideObject(GamePopup[1]);
			hideObject(GameEnterButton[1]);

			playSound(buttonClickSound);
			exitTitle();
			enterScene(scene_g4);
		}
	}

	//바다 (바로 입장가능)
	else if (object == GameEnterButton[2]) {

		nowGameSceneNum = 2;

		hideObject(GamePopup[2]);
		hideObject(GameEnterButton[2]);

		playSound(buttonClickSound);
		exitTitle();
		enterScene(scene1_g2);

	}

	//귀신의집
	else if (object == GameEnterButton[3]) {

		if (stageLocked[3]) {
			stageUnlock(3);

			if (stageLocked[3] == true) {	//열렸으면
				stageBlack[4] = false;
				setObjectImage(GameIcon[4], "image/Title/빨간아이콘.png");
			}
		}

		else {
			nowGameSceneNum = 1;

			hideObject(GamePopup[3]);
			hideObject(GameEnterButton[3]);

			playSound(buttonClickSound);
			exitTitle();
			enterScene(scene_g1);
		}
	}

	//하늘섬
	else if (object == GameEnterButton[4]) {

		if (stageLocked[4]) {
			stageUnlock(4);
		}

		else {
			nowGameSceneNum = 6;

			hideObject(GamePopup[4]);
			hideObject(GameEnterButton[4]);

			exitTitle();
			playSound(buttonClickSound);
			playSound(bgm_g6);

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
		}
	}

	//쿠파성
	else if (object == GameEnterButton[5]) {

		if (stageLocked[5]) {
			stageUnlock(5);
		}

		else {
			nowGameSceneNum = 5;

			hideObject(GamePopup[5]);
			hideObject(GameEnterButton[5]);

			exitTitle();
			playSound(buttonClickSound);
			playSound(bgm_g5);
			enterScene(scene_g5);
		}
	}	

}

void Title_timerCallback(TimerID timer) {

	if (timer == marioAnimationTimer) {

		setObjectImage(Mario, marioAnimationImage[marioAnimationCount % 9]);
		marioAnimationCount++;

		setTimer(marioAnimationTimer, MARIO_ANIMATION_TIME);
		startTimer(marioAnimationTimer);
	}


}


void Title_soundCallback(SoundID sound) {

}


void Title_main() {

	titleScene = createScene("전체 맵", "image/Title/worldmap.png");
	
	for (int i = 0; i < 6; i++)
		GameIcon[i] = createObject("image/Title/검은아이콘.png", titleScene, IconX[i] +10 , IconY[i] - 15, true, 1.0f);
	setObjectImage(GameIcon[0], "image/Title/파란아이콘.png");
	setObjectImage(GameIcon[1], "image/Title/빨간아이콘.png");
	setObjectImage(GameIcon[5], "image/Title/빨간아이콘.png");

	Mario = createObject("image/Title/마리오애니/1.png", titleScene, IconX[0], IconY[0], true, 1.4f);

	GamePopup[0] = createObject("image/Title/팝업/1.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[1] = createObject("image/Title/팝업/2.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[2] = createObject("image/Title/팝업/3.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[3] = createObject("image/Title/팝업/4.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[4] = createObject("image/Title/팝업/5.png", titleScene, 0, 0, false, 1.0f);
	GamePopup[5] = createObject("image/Title/팝업/6.png", titleScene, 0, 0, false, 1.0f);



	GameEnterButton[0] = createObject("image/Title/enter.png", titleScene, 950, 100, false, 1.0f);
	GameEnterButton[1] = createObject("image/Title/unlock.png", titleScene, 950, 70, false, 1.0f);
	GameEnterButton[2] = createObject("image/Title/enter.png", titleScene, 950, 100, false, 1.0f);
	GameEnterButton[3] = createObject("image/Title/unlock.png", titleScene, 950, 70, false, 1.0f);
	GameEnterButton[4] = createObject("image/Title/unlock.png", titleScene, 70, 70, false, 1.0f);
	GameEnterButton[5] = createObject("image/Title/unlock.png", titleScene, 70, 70, false, 1.0f);



	marioAnimationTimer = createTimer(MARIO_ANIMATION_TIME);
	startTimer(marioAnimationTimer);

	bgm_title = createSound("image/Title/메인맵브금.mp3");
	playSound(bgm_title, true);

}