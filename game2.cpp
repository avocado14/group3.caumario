#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1_g2, scene2_g2;
ObjectID startbutton, restartbutton, endbutton, heart1, heart2, heart3, hiteffect, damaged;
ObjectID target[20] = { 0, };
TimerID timer1, hitting, damageTime;
SoundID bgm;
int arrX[20] = { 0, }, arrY[20] = { 0, };
int count = 0, clear = 0, life = 3, targetNum = 4, round = 0;
char info[50] = { 0, };
float duration = 1.0f;
bool lock = false;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {

	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}

	return object;
}

void locationMaker(int num) {		// ǥ�� ��ġ ������ ���� �迭 ����� (ǥ�� ���� �Է�)

	int i, x, y, temp;

	srand(time(NULL));

	x = rand() % 1000;
	y = rand() % 500;

	for (i = 0; i < num; i++) {
		if (i != 0) {
			do {
				x = rand() % 1000;
			} while (x - arrX[i - 1] > -200 && x - arrX[i - 1] < 200);
			do {
				y = rand() % 500;
			} while (y - arrX[i - 1] > -200 && y - arrX[i - 1] < 200);
		}

		arrX[i] = 100 + x;
		arrY[i] = 100 + y;

		target[i] = createObject("images/target.png", scene2_g2, arrX[i], arrY[i], false);
	}
}

void clearing() {

	for (int i = 0; i < 20; i++) target[i] = 0;
	for (int i = 0; i < 20; i++) arrX[i] = 0;
	for (int i = 0; i < 20; i++) arrY[i] = 0;

	clear = 0;
	count = 0;

	locationMaker(targetNum);
	setTimer(timer1, duration);
	startTimer(timer1);
}

void ending() {		// ���� ����

	for (int j = 0; j < targetNum; j++) {	// �� �� Ÿ�� ġ���
		hideObject(target[j]);
	}

	sprintf_s(info, "���� �� : %d", round);
	showMessage(info);

	showObject(restartbutton);
	showObject(endbutton);

}

void minusHeart() {			// ���� �� ��� ����
	life--;
	if (life == 2) hideObject(heart3);
	else if (life == 1) hideObject(heart2);
	else if (life == 0) {
		hideObject(heart1);
		ending();
	}
}

void judge(ObjectID object, int i) {

	if (object == target[i]) {

		if (clear == i) {

			hideObject(target[i]);

			clear++;

			locateObject(hiteffect, scene2_g2, arrX[i], arrY[i]);
			showObject(hiteffect);

			setTimer(hitting, 0.5f);
			startTimer(hitting);

			if (clear == targetNum) {		// ������������ ���̵� ��� ���ǵ�

				for (int j = 0; j < targetNum; j++) {	// �� �� Ÿ�� ġ���
					hideObject(target[j]);
				}

				if (targetNum < 7) {	// Ÿ�� �� ���� (�ִ� 7��)
					targetNum++;
				}

				if (duration > 0.5f) {	// Ÿ�� ���� �ֱ� ���� (�ּ� 0.5��)
					duration -= 0.05f;
				}

				round++;

				clearing();
			}
		}

		else {

			showObject(damaged);
			setTimer(damageTime, 0.2f);
			startTimer(damageTime);

			minusHeart();
		}
	}
}


void Game2_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (lock == true) {
		return;
	}
	
	if (object == startbutton) {

		enterScene(scene2_g2);

		locationMaker(targetNum);
		setTimer(timer1, duration);
		startTimer(timer1);

		playSound(bgm, true);
	}

	else if (object == restartbutton) {

		hideObject(restartbutton);
		hideObject(endbutton);

		clearing();
	}

	else if (object == endbutton) {

		endGame();
	}
	else {											// ���� Ÿ���� Ŭ���ߴ��� �˻�
		for (int i = 0; i < targetNum; i++) {
			judge(object, i);
		}
	}

}

void Game2_timerCallback(TimerID timer) {

	if (timer == timer1) {
	
		showObject(target[count]);

		count++;
		lock = true;

		if (count != targetNum) {
			setTimer(timer1, duration);
			startTimer(timer1);
		}
		else lock = false;
	}

	if (timer == hitting) {
		hideObject(hiteffect);
	}

	if (timer == damageTime) {
		hideObject(damaged);
	}

}
void Game2_main() {

	setMouseCallback(Game2_mouseCallback);
	setTimerCallback(Game2_timerCallback);

	scene1_g2 = createScene("�غ� ȭ��", "image/game2/���.png");
	scene2_g2 = createScene("�޸� ����", "image/game2/���1.png");

	startbutton = createObject("image/game2/����.png", scene1_g2, 610, 70, true);
	restartbutton = createObject("image/game2/�ٽý���.png", scene2_g2, 610, 400, false);
	endbutton = createObject("image/game2/Ȯ��.png", scene2_g2, 610, 350, false);

	hiteffect = createObject("images/game2/hit.png", scene2_g2, 610, 400, false);
	damaged = createObject("images/game2/damage.png", scene2_g2, 0, 0, false);

	heart1 = createObject("image/game2/heart.png", scene2_g2, 830, 650, true);
	scaleObject(heart1, 0.05f);
	heart2 = createObject("image/game2/heart.png", scene2_g2, 900, 650, true);
	scaleObject(heart2, 0.05f);
	heart3 = createObject("image/game2/heart.png", scene2_g2, 970, 650, true);
	scaleObject(heart3, 0.05f);

	bgm = createSound("image/game2/bgm.wav");

	timer1 = createTimer(duration);
	hitting = createTimer(0.5f);
	damageTime = createTimer(0.2f);

	
}