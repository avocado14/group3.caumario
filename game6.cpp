#include <bangtal.h>
#include<stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define MONSTER_ANIMATION_TIME		0.2f
#define PLAYER_SPEED				20
#define MONSTER_SPEED				7
#define MONSTER_NUMBER				10
#define ENDMONSTER_STATE			2	//������ ���� ����(���� ������ ����)
#define PLAYER_IMAGE_SIZE			78
#define MONSTER_IMAGE_SIZE			78

SceneID scene_g6;
ObjectID startButton_g6;
ObjectID player;
ObjectID monster[MONSTER_NUMBER];
TimerID playerTimer, monsterTimer;

int playerX = 640, playerY = 360;
int monsterX[MONSTER_NUMBER] = { 500,900,500,900,400,700,1000,400,700,1000 };
int monsterY[MONSTER_NUMBER] = { 200,200,400,400,150,150,150,500,500,500 };
int dx = 0, dy = 0, dmx = 0, dmy = 0;

int playerState = 1;	//�÷��̾� ����(ũ��) : ó���� 1�ܰ�
int monsterState[MONSTER_NUMBER] = { 1,1,1,1,2,2,2,2,2,2 };	//���� ����
int monsterNumber[ENDMONSTER_STATE] = { 4,6 };		//state�� ���� ������
float monsterSize[MONSTER_NUMBER] = { 54.6f,54.6f,54.6f,54.6f,78,78,78,78,78,78 };
float playerSize = 54.6f;

bool monsterIsShown[MONSTER_NUMBER];

int num;
int monsterCount = 0;


ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size) {

	ObjectID object = createObject(name);
	locateObject(object, scene, x, y);
	scaleObject(object, size);

	if (shown == true)
		showObject(object);

	return object;
}

int random() {

	//0~3
	num = rand() % 4;

	return num;
}

void Game6_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startButton_g6) {		//Ÿ�̸� �����鼭(������Ʈ �̵� ����) ���ӽ���
		hideObject(startButton_g6);
		startTimer(playerTimer);
		startTimer(monsterTimer);	
	}

}

void Game6_keyboardCallback(KeyCode code, KeyState state)
{
	if (code == 23) {			// UP
		dy += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 19) {		// DOWN
		dy -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 4) {		// RIGHT
		dx += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}
	else if (code == 1) {		// LEFT
		dx -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
	}

}

void Game6_timerCallback(TimerID timer)
{
	//�� ����
	if (timer == playerTimer) {
		if (playerX + dx > 1280 - playerSize || playerX + dx < 0 || playerY + dy > 720 - playerSize || playerY + dy < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

		else {
			playerX += dx; playerY += dy;
			locateObject(player, scene_g6, playerX, playerY);
		}
		for (int i = 0; i < MONSTER_NUMBER; i++) {
			if (monsterIsShown[i] == true) {			//����(�Ⱥ��̴�) ���Ϳ��� ����x
				if (monsterX[i] - playerX <= playerSize && monsterX[i] - playerX >= -monsterSize[i] &&		//�÷��̾ ���Ͷ� x�� �ε�����
					monsterY[i] - playerY <= playerSize && monsterY[i] - playerY >= -monsterSize[i]) {		//y�൵ �ε�����

					if (playerState == monsterState[i]) {		//�ε����µ� ������ �Ȱ�����
						hideObject(monster[i]);					//���� ����
						monsterIsShown[i] = false;				//�Ⱥ��̴� �������� �迭�� ����
						monsterCount++;

						if (monsterCount == monsterNumber[playerState - 1]) {				//���� ���� ���� �� ���̸�
							if (playerState != ENDMONSTER_STATE) {						//Ŭ��� �ƴϸ�
								playerSize = monsterSize[monsterNumber[playerState]];			//�÷��̾� ������ ���� ���� ���Ϳ� ���� �����
								playerState++;													//�÷��̾� ���� �ø�
								scaleObject(player, (float)playerSize / PLAYER_IMAGE_SIZE);		//�÷��̾� ������ �ٲ���
								monsterCount = 0;
							}
							else {															//	<����Ŭ����>
								stopTimer(monsterTimer);
								stopTimer(playerTimer);
								showMessage("����Ŭ����");
								return;
							}
						}
					}

					else {										//�ε����µ� ���� ������ �� ũ��
						hideObject(player);						//�÷��̾� ����		<���ӿ���>
						stopTimer(monsterTimer);
						stopTimer(playerTimer);
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
	if (timer == monsterTimer) {

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


void Game6_main()
{
	scene_g6 = createScene("����6", "image/game6/���.png");
	player = createObject("image/game6/player.png", scene_g6, playerX, playerY, true, (float)playerSize / PLAYER_IMAGE_SIZE);

	//����1 ���� ����
	for (int i = 0; i < monsterNumber[0]; i++) {
		monster[i] = createObject("image/game6/monster1.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}
	//����2 ���� ����	
	for (int i = monsterNumber[0]; i < monsterNumber[0] + monsterNumber[1]; i++) {
		monster[i] = createObject("image/game6/monster2.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}

	startButton_g6 = createObject("image/game6/���ӽ���.png", scene_g6, 500, 250, true, 2.0f);

	playerTimer = createTimer(PLAYER_ANIMATION_TIME);
	monsterTimer = createTimer(MONSTER_ANIMATION_TIME);

}