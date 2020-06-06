#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define MONSTER_ANIMATION_TIME		0.2f
#define PLAYER_SPEED				20
#define MONSTER_SPEED				7
#define MONSTER_NUMBER				10
#define ENDMONSTER_STATE			2	//������ ���� ����(���� ������ ����)
#define PLAYER_IMAGE_SIZE			100
#define MONSTER_IMAGE_SIZE			100

SceneID scene_g6;
ObjectID startButton_g6;
ObjectID player_g6;
ObjectID monster[MONSTER_NUMBER];
ObjectID countDown;
TimerID countDownTimer, playTimer_g6, monsterTimer_g6;
SoundID bgm_g6;

int playerX_g6 = 640, playerY_g6 = 360;
int monsterX[MONSTER_NUMBER] = { 500,900,500,900,400,700,1000,400,700,1000 };
int monsterY[MONSTER_NUMBER] = { 200,200,400,400,150,150,150,500,500,500 };
int dx_g6 = 0, dy_g6 = 0, dmx = 0, dmy = 0;

int playerState_g6 = 1;	//�÷��̾� ����(ũ��) : ó���� 1�ܰ�
int monsterState[MONSTER_NUMBER] = { 1,1,1,1,2,2,2,2,2,2 };	//���� ����
int monsterNumber[ENDMONSTER_STATE] = { 4,6 };		//state�� ���� ������
float monsterSize[MONSTER_NUMBER] = { 70,70,70,70,100,100,100,100,100,100 };
float playerSize_g6 = monsterSize[0];

bool monsterIsShown[MONSTER_NUMBER];

int num;
int monsterCount = 0;
int countDownNum = 1;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

int random() {

	//0~3
	num = rand() % 4;

	return num;
}

void Game6_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startButton_g6) {		//Ÿ�̸� �����鼭(������Ʈ �̵� ����) ���ӽ���
		showObject(countDown);
		startTimer(countDownTimer);
		hideObject(startButton_g6);
	}

}

void Game6_keyboardCallback(KeyCode code, KeyState state)
{
	if (code == 23) {			// UP
		dy_g6 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 19) {		// DOWN
		dy_g6 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 4) {		// RIGHT
		dx_g6 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 1) {		// LEFT
		dx_g6 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}

}

void Game6_timerCallback(TimerID timer)
{

	if (timer == countDownTimer) {
		countDownNum++;

		if (countDownNum == 2) {
			setObjectImage(countDown, "image/game6/2.png");
			setTimer(timer, 1.0f);
			startTimer(timer);
		}
		else if (countDownNum == 3) {
			setObjectImage(countDown, "image/game6/1.png");
			setTimer(timer, 1.0f);
			startTimer(timer);
		}
		else {
			hideObject(countDown);
			startTimer(playTimer_g6);
			startTimer(monsterTimer_g6);
		}
	}

	//�� ����
	if (timer == playTimer_g6) {
		if (playerX_g6 + dx_g6 > 1280 - playerSize_g6 || playerX_g6 + dx_g6 < 0 || playerY_g6 + dy_g6 > 720 - playerSize_g6 || playerY_g6 + dy_g6 < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

		else {
			playerX_g6 += dx_g6; playerY_g6 += dy_g6;
			locateObject(player_g6, scene_g6, playerX_g6, playerY_g6);
		}
		for (int i = 0; i < MONSTER_NUMBER; i++) {
			if (monsterIsShown[i] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x
				if (monsterX[i] - playerX_g6 <= playerSize_g6 && monsterX[i] - playerX_g6 >= -monsterSize[i] &&		//�÷��̾ ���Ͷ� x�� �ε�����
					monsterY[i] - playerY_g6 <= playerSize_g6 && monsterY[i] - playerY_g6 >= -monsterSize[i]) {		//y�൵ �ε�����

					if (playerState_g6 == monsterState[i]) {		//�ε����µ� ������ �Ȱ�����
						hideObject(monster[i]);					//���� ����
						monsterIsShown[i] = false;				//�Ⱥ��̴� �������� �迭�� ����
						monsterCount++;

						if (monsterCount == monsterNumber[playerState_g6 - 1]) {				//���� ���� ���� �� ���̸�
							if (playerState_g6 != ENDMONSTER_STATE) {						//Ŭ��� �ƴϸ�
								playerSize_g6 = monsterSize[monsterNumber[playerState_g6]];			//�÷��̾� ������ ���� ���� ���Ϳ� ���� �����
								playerState_g6++;													//�÷��̾� ���� �ø�
								scaleObject(player_g6, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);		//�÷��̾� ������ �ٲ���
								monsterCount = 0;
							}
							else {															//	<����Ŭ����>
								stopTimer(monsterTimer_g6);
								stopTimer(playTimer_g6);
								showMessage("����Ŭ����");
								return;
							}
						}
					}

					else {										//�ε����µ� ���� ������ �� ũ��
						hideObject(player_g6);						//�÷��̾� ����		<���ӿ���>
						stopTimer(monsterTimer_g6);
						stopTimer(playTimer_g6);
						showMessage("���ӿ���");
						return;

					}
				}
			}
		}

		setTimer(timer, PLAYER_ANIMATION_TIME);
		startTimer(timer);

	}

	//���� ���� �̵�
	if (timer == monsterTimer_g6) {

		for (int j = 0; j < MONSTER_NUMBER; j++) {
			if (monsterIsShown[j] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x

				random();

				if (num == 0)	//���� �̵�
					dmx -= MONSTER_SPEED;
				else if (num == 1)	//������ �̵�
					dmx += MONSTER_SPEED;
				else if (num == 2)	//�Ʒ� �̵�
					dmy -= MONSTER_SPEED;
				else if (num == 3)	//�� �̵�
					dmy += MONSTER_SPEED;

				for (int i = 0; i < 4; i++) {
					if (monsterX[j] + dmx > 1185 || monsterX[j] + dmx < 15 || monsterY[j] + dmy > 635 || monsterY[j] + dmy < 15)	//�׵θ� ������ �̵� �Ƚ�Ű��
						break;

					else {
						monsterX[j] += dmx, monsterY[j] += dmy;
						locateObject(monster[j], scene_g6, monsterX[j], monsterY[j]);			//4�� �ɰ��� �̵�
					}
				}
				dmx = 0, dmy = 0;	//�̵� ������ dmx, dmy �ʱ�ȭ
			}
		}
		setTimer(timer, MONSTER_ANIMATION_TIME);
		startTimer(timer);
	}
}

void Game6_soundCallback(SoundID sound) {

	if (sound == bgm_g6)
		playSound(bgm_g6);

}


void Game6_main()
{
	scene_g6 = createScene("�ϴ� ��", "image/game6/�ϴù��.png");
	player_g6 = createObject("image/game6/����������.png", scene_g6, playerX_g6, playerY_g6, true, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);

	//����1 ���� ����
	for (int i = 0; i < monsterNumber[0]; i++) {
		monster[i] = createObject("image/game6/��������.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}
	//����2 ���� ����	
	for (int i = monsterNumber[0]; i < monsterNumber[0] + monsterNumber[1]; i++) {
		monster[i] = createObject("image/game6/�����ʷϱ���.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}

	startButton_g6 = createObject("image/game6/���ӽ���.png", scene_g6, 500, 150, true, 2.0f);
	countDown = createObject("image/game6/3.png", scene_g6, 600, 300, false, 5.0f);

	playTimer_g6 = createTimer(PLAYER_ANIMATION_TIME);
	monsterTimer_g6 = createTimer(MONSTER_ANIMATION_TIME);
	countDownTimer = createTimer(0.7f);

	bgm_g6 = createSound("image/game6/�ϴú��.mp3");

}