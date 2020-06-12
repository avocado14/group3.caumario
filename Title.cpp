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
int stageUnlockCost[6] = { 0, 300, 0,300,1500,0 };	//6��° : ���� 2�� �ʿ�
int coin = 5000;
int key = 2;

int nowMarioOn = 0;
int nowGameSceneNum = 0;	//0�� Ÿ��Ʋ
extern int nowGame6Stage;

char path[256];

int marioAnimationCount = 0;
const char* marioAnimationImage[9] =
{ "image/Title/�������ִ�/1.png","image/Title/�������ִ�/2.png","image/Title/�������ִ�/3.png","image/Title/�������ִ�/4.png",
"image/Title/�������ִ�/5.png","image/Title/�������ִ�/6.png","image/Title/�������ִ�/7.png","image/Title/�������ִ�/8.png","image/Title/�������ִ�/9.png" };



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

//0�� Ŭ����x 2��������(�縷), 5��������(�ϴ�) ���� 
void enterTitle(int clearScene) {

	hideUI();
	setTimer(marioAnimationTimer, MARIO_ANIMATION_TIME);
	startTimer(marioAnimationTimer);

	playSound(bgm_title);

	//�縷���� Ŭ����������
	if (clearScene == 2) {
		setObjectImage(GameIcon[1], "image/Title/�ʷϾ�����.png");
		setObjectImage(GameIcon[2], "image/Title/�Ķ�������.png");
		setObjectImage(GameIcon[3], "image/Title/�Ķ�������.png");
		stageBlack[2] = false;
		stageBlack[3] = false;
		
		key = 1;
	}

	//�ϴ����� Ŭ����������
	else if (clearScene == 5) {
		setObjectImage(GameIcon[4], "image/Title/�ʷϾ�����.png");
		
		key = 2;
	}

	enterScene(titleScene);
}

//0�� 2���� ���� �ȵ�
void stageUnlock(int stage) {

	if (stage != 5 ) {
		if (coin >= stageUnlockCost[stage]) {

			coin -= stageUnlockCost[stage];		//�� ����
			stageLocked[stage] = false;			//�������� �ٲ��ְ�
			
			sprintf_s(path, "image/Title/�˾�/%d-2.png", stage + 1);
			setObjectImage(GamePopup[stage], path);
			setObjectImage(GameEnterButton[stage], "image/Title/enter.png");
			locateObject(GameEnterButton[stage], titleScene, 950, 100);

			if(stage == 4)
				locateObject(GameEnterButton[stage], titleScene, 70, 100);

		}

		else
			showMessage("���� �����մϴ�");
	}

	//���ļ�
	else  {
		if (key == 2) {

			stageLocked[5] = false;

			setObjectImage(GamePopup[5], "image/Title/�˾�/6-2.png");
			setObjectImage(GameEnterButton[5], "image/Title/enter.png");
			locateObject(GameEnterButton[stage], titleScene, 70, 100);
		}
		
		else
			showMessage("���谡 �����մϴ�");
	}
}

void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	

	//����������(Ŭ���Ұ�)�� �ƴϸ� �˾�â ���
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
			showMessage("���� ���������� Ŭ���� ���ּ���");
		}
	}

	////�˾�â Ŭ���� �����//////
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

	
	///////���� ���� ��ư//////
	//���
	else if (object == GameEnterButton[0]) {
		nowGameSceneNum = 3;
		
		hideObject(GamePopup[0]);
		hideObject(GameEnterButton[0]);

		playSound(buttonClickSound);
		exitTitle();
		enterScene(scene1_g3);
	}

	//�縷
	else if (object == GameEnterButton[1]) {

		//��������� �ر�
		if (stageLocked[1]) {
			stageUnlock(1);
		}

		//���������� �̵�
		else {
			nowGameSceneNum = 4;

			hideObject(GamePopup[1]);
			hideObject(GameEnterButton[1]);

			playSound(buttonClickSound);
			exitTitle();
			enterScene(scene_g4);
		}
	}

	//�ٴ� (�ٷ� ���尡��)
	else if (object == GameEnterButton[2]) {

		nowGameSceneNum = 2;

		hideObject(GamePopup[2]);
		hideObject(GameEnterButton[2]);

		playSound(buttonClickSound);
		exitTitle();
		enterScene(scene1_g2);

	}

	//�ͽ�����
	else if (object == GameEnterButton[3]) {

		if (stageLocked[3]) {
			stageUnlock(3);

			if (stageLocked[3] == true) {	//��������
				stageBlack[4] = false;
				setObjectImage(GameIcon[4], "image/Title/����������.png");
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

	//�ϴü�
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

	//���ļ�
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

	titleScene = createScene("��ü ��", "image/Title/worldmap.png");
	
	for (int i = 0; i < 6; i++)
		GameIcon[i] = createObject("image/Title/����������.png", titleScene, IconX[i] +10 , IconY[i] - 15, true, 1.0f);
	setObjectImage(GameIcon[0], "image/Title/�Ķ�������.png");
	setObjectImage(GameIcon[1], "image/Title/����������.png");
	setObjectImage(GameIcon[5], "image/Title/����������.png");

	Mario = createObject("image/Title/�������ִ�/1.png", titleScene, IconX[0], IconY[0], true, 1.4f);

	GamePopup[0] = createObject("image/Title/�˾�/1.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[1] = createObject("image/Title/�˾�/2.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[2] = createObject("image/Title/�˾�/3.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[3] = createObject("image/Title/�˾�/4.png", titleScene, 884, 0, false, 1.0f);
	GamePopup[4] = createObject("image/Title/�˾�/5.png", titleScene, 0, 0, false, 1.0f);
	GamePopup[5] = createObject("image/Title/�˾�/6.png", titleScene, 0, 0, false, 1.0f);



	GameEnterButton[0] = createObject("image/Title/enter.png", titleScene, 950, 100, false, 1.0f);
	GameEnterButton[1] = createObject("image/Title/unlock.png", titleScene, 950, 70, false, 1.0f);
	GameEnterButton[2] = createObject("image/Title/enter.png", titleScene, 950, 100, false, 1.0f);
	GameEnterButton[3] = createObject("image/Title/unlock.png", titleScene, 950, 70, false, 1.0f);
	GameEnterButton[4] = createObject("image/Title/unlock.png", titleScene, 70, 70, false, 1.0f);
	GameEnterButton[5] = createObject("image/Title/unlock.png", titleScene, 70, 70, false, 1.0f);



	marioAnimationTimer = createTimer(MARIO_ANIMATION_TIME);
	startTimer(marioAnimationTimer);

	bgm_title = createSound("image/Title/���θʺ��.mp3");
	playSound(bgm_title, true);

}