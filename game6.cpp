#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_ANIMATION_TIME		0.05f
#define MONSTER_ANIMATION_TIME		0.2f
#define PLAYER_SPEED				20
#define MONSTER_SPEED				7
#define MONSTER_NUMBER				10
#define ENDMONSTER_STATE			2	//마지막 몬스터 레벨(게임 끝나는 레벨)
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

int playerState_g6 = 1;	//플레이어 상태(크기) : 처음은 1단계
int monsterState[MONSTER_NUMBER] = { 1,1,1,1,2,2,2,2,2,2 };	//몬스터 상태
int monsterNumber[ENDMONSTER_STATE] = { 4,6 };		//state별 몬스터 마리수
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

	if (object == startButton_g6) {		//타이머 켜지면서(오브젝트 이동 시작) 게임시작
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

	//공 조작
	if (timer == playTimer_g6) {
		if (playerX_g6 + dx_g6 > 1280 - playerSize_g6 || playerX_g6 + dx_g6 < 0 || playerY_g6 + dy_g6 > 720 - playerSize_g6 || playerY_g6 + dy_g6 < 0) {}	//테두리 나가면 이동 안시키기

		else {
			playerX_g6 += dx_g6; playerY_g6 += dy_g6;
			locateObject(player_g6, scene_g6, playerX_g6, playerY_g6);
		}
		for (int i = 0; i < MONSTER_NUMBER; i++) {
			if (monsterIsShown[i] == true) {			//죽은(안보이는) 몬스터에는 반응x
				if (monsterX[i] - playerX_g6 <= playerSize_g6 && monsterX[i] - playerX_g6 >= -monsterSize[i] &&		//플레이어가 몬스터랑 x축 부딪히고
					monsterY[i] - playerY_g6 <= playerSize_g6 && monsterY[i] - playerY_g6 >= -monsterSize[i]) {		//y축도 부딪히면

					if (playerState_g6 == monsterState[i]) {		//부딪혔는데 레벨이 똑같으면
						hideObject(monster[i]);					//몬스터 죽임
						monsterIsShown[i] = false;				//안보이는 상태임을 배열에 저장
						monsterCount++;

						if (monsterCount == monsterNumber[playerState_g6 - 1]) {				//지금 레벨 몬스터 다 죽이면
							if (playerState_g6 != ENDMONSTER_STATE) {						//클리어가 아니면
								playerSize_g6 = monsterSize[monsterNumber[playerState_g6]];			//플레이어 사이즈 다음 레벨 몬스터와 같게 만들고
								playerState_g6++;													//플레이어 레벨 올림
								scaleObject(player_g6, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);		//플레이어 사이즈 바꿔줌
								monsterCount = 0;
							}
							else {															//	<게임클리어>
								stopTimer(monsterTimer_g6);
								stopTimer(playTimer_g6);
								showMessage("게임클리어");
								return;
							}
						}
					}

					else {										//부딪혔는데 몬스터 레벨이 더 크면
						hideObject(player_g6);						//플레이어 죽음		<게임오버>
						stopTimer(monsterTimer_g6);
						stopTimer(playTimer_g6);
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
	if (timer == monsterTimer_g6) {

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

void Game6_soundCallback(SoundID sound) {

	if (sound == bgm_g6)
		playSound(bgm_g6);

}


void Game6_main()
{
	scene_g6 = createScene("하늘 맵", "image/game6/하늘배경.png");
	player_g6 = createObject("image/game6/날개마리오.png", scene_g6, playerX_g6, playerY_g6, true, (float)playerSize_g6 / PLAYER_IMAGE_SIZE);

	//레벨1 몬스터 스폰
	for (int i = 0; i < monsterNumber[0]; i++) {
		monster[i] = createObject("image/game6/날개굼바.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}
	//레벨2 몬스터 스폰	
	for (int i = monsterNumber[0]; i < monsterNumber[0] + monsterNumber[1]; i++) {
		monster[i] = createObject("image/game6/날개초록굼바.png", scene_g6, monsterX[i], monsterY[i], true, (float)monsterSize[i] / MONSTER_IMAGE_SIZE);
		monsterIsShown[i] = true;
	}

	startButton_g6 = createObject("image/game6/게임시작.png", scene_g6, 500, 150, true, 2.0f);
	countDown = createObject("image/game6/3.png", scene_g6, 600, 300, false, 5.0f);

	playTimer_g6 = createTimer(PLAYER_ANIMATION_TIME);
	monsterTimer_g6 = createTimer(MONSTER_ANIMATION_TIME);
	countDownTimer = createTimer(0.7f);

	bgm_g6 = createSound("image/game6/하늘브금.mp3");

}