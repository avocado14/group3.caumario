#include <bangtal.h>
#include<stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define MONSTER_ANIMATION_TIME		0.2f
#define PLAYER_SPEED				20
#define MONSTER_SPEED				7
#define MONSTER_NUMBER				10
#define ENDMONSTER_STATE			2	//마지막 몬스터 레벨(게임 끝나는 레벨)
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

int playerState = 1;	//플레이어 상태(크기) : 처음은 1단계
int monsterState[MONSTER_NUMBER] = { 1,1,1,1,2,2,2,2,2,2 };	//몬스터 상태
int monsterNumber[ENDMONSTER_STATE] = { 4,6 };		//state별 몬스터 마리수
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

	if (object == startButton_g6) {		//타이머 켜지면서(오브젝트 이동 시작) 게임시작
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
	//공 조작
	if (timer == playerTimer) {
		if (playerX + dx > 1280 - playerSize || playerX + dx < 0 || playerY + dy > 720 - playerSize || playerY + dy < 0) {}	//테두리 나가면 이동 안시키기

		else {
			playerX += dx; playerY += dy;
			locateObject(player, scene_g6, playerX, playerY);
		}
		for (int i = 0; i < MONSTER_NUMBER; i++) {
			if (monsterIsShown[i] == true) {			//죽은(안보이는) 몬스터에는 반응x
				if (monsterX[i] - playerX <= playerSize && monsterX[i] - playerX >= -monsterSize[i] &&		//플레이어가 몬스터랑 x축 부딪히고
					monsterY[i] - playerY <= playerSize && monsterY[i] - playerY >= -monsterSize[i]) {		//y축도 부딪히면

					if (playerState == monsterState[i]) {		//부딪혔는데 레벨이 똑같으면
						hideObject(monster[i]);					//몬스터 죽임
						monsterIsShown[i] = false;				//안보이는 상태임을 배열에 저장
						monsterCount++;

						if (monsterCount == monsterNumber[playerState - 1]) {				//지금 레벨 몬스터 다 죽이면
							if (playerState != ENDMONSTER_STATE) {						//클리어가 아니면
								playerSize = monsterSize[monsterNumber[playerState]];			//플레이어 사이즈 다음 레벨 몬스터와 같게 만들고
								playerState++;													//플레이어 레벨 올림
								scaleObject(player, (float)playerSize / PLAYER_IMAGE_SIZE);		//플레이어 사이즈 바꿔줌
								monsterCount = 0;
							}
							else {															//	<게임클리어>
								stopTimer(monsterTimer);
								stopTimer(playerTimer);
								showMessage("게임클리어");
								return;
							}
						}
					}

					else {										//부딪혔는데 몬스터 레벨이 더 크면
						hideObject(player);						//플레이어 죽음		<게임오버>
						stopTimer(monsterTimer);
						stopTimer(playerTimer);
						showMessage("게임오버");
						return;

					}
				}
			}
		}
		setTimer(timer, PLAYER_ANIMATION_TIME);
		startTimer(timer);
	}

	//몬스터 랜덤 이동
	if (timer == monsterTimer) {

		for (int j = 0; j < MONSTER_NUMBER; j++) {
			if (monsterIsShown[j] == true) {			//죽은(안보이는) 몬스터에는 반응x

				random();

				if (num == 0)	//왼쪽 이동
					dmx -= MONSTER_SPEED;
				else if (num == 1)	//오른쪽 이동
					dmx += MONSTER_SPEED;
				else if (num == 2)	//아래 이동
					dmy -= MONSTER_SPEED;
				else if (num == 3)	//위 이동
					dmy += MONSTER_SPEED;

				for (int i = 0; i < 4; i++) {
					if (monsterX[j] + dmx > 1185 || monsterX[j] + dmx < 15 || monsterY[j] + dmy > 635 || monsterY[j] + dmy < 15)	//테두리 나가면 이동 안시키기
						break;

					else {
						monsterX[j] += dmx, monsterY[j] += dmy;
						locateObject(monster[j], scene_g6, monsterX[j], monsterY[j]);			//4번 쪼개서 이동
					}
				}
				dmx = 0, dmy = 0;	//이동 끝나면 dmx, dmy 초기화
			}
		}
		setTimer(timer, MONSTER_ANIMATION_TIME);
		startTimer(timer);
	}
}


void Game6_main()
{
	scene_g6 = createScene("게임6", "image/game6/배경.png");
	player = createObject("image/game6/player.png", scene_g6, playerX, playerY, true, (float)playerSize / PLAYER_IMAGE_SIZE);

	//레벨1 몬스터 스폰
	for (int i = 0; i < monsterNumber[0]; i++) {
		monster[i] = createObject("image/game6/monster1.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}
	//레벨2 몬스터 스폰	
	for (int i = monsterNumber[0]; i < monsterNumber[0] + monsterNumber[1]; i++) {
		monster[i] = createObject("image/game6/monster2.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}

	startButton_g6 = createObject("image/game6/게임시작.png", scene_g6, 500, 250, true, 2.0f);

	playerTimer = createTimer(PLAYER_ANIMATION_TIME);
	monsterTimer = createTimer(MONSTER_ANIMATION_TIME);

}