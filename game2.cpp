#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1_g2, scene2_g2;
ObjectID startbutton, restartbutton, endbutton, heart1, heart2, heart3;
ObjectID target[20] = { 0, };
TimerID timer1;
SoundID bgm, finalHit, finalHit1, normalHit, normalHit1, normalHit2, normalHit3, normalHit4;
int arrX[20] = { 0, }, arrY[20] = { 0, };
int count = 0, clear = 0, life = 3, targetNum = 4, round = 0;
char info[50] = { 0, };
float duration = 1.0f;

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
			} while (x - arrX[i - 1] > -50 && x - arrX[i - 1] < 50);
			do {
				y = rand() % 500;
			} while (y - arrX[i - 1] > -50 && y - arrX[i - 1] < 50);
		}

		arrX[i] = 100 + x;
		arrY[i] = 100 + y;

		target[i] = createObject("images/5.png", scene2_g2, arrX[i], arrY[i], false);
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
	//bgmloop = false;

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

			if (clear == targetNum) {		// ������������ ���̵� ��� ���ǵ�

				for (int j = 0; j < targetNum; j++) {	// �� �� Ÿ�� ġ���
					hideObject(target[j]);
				}

				if (targetNum < 12) {	// Ÿ�� �� ���� (�ִ� 12��)
					targetNum++;
				}

				if (duration > 0.3f) {	// Ÿ�� ���� �ֱ� ���� (�ּ� 0.3��)
					duration -= 0.05f;
				}

				round++;

				clearing();
			}
		}

		else {
			minusHeart();
		}
	}
}


void Game2_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startbutton) {

		enterScene(scene2_g2);

		locationMaker(targetNum);
		setTimer(timer1, duration);
		startTimer(timer1);
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

	showObject(target[count]);

	count++;

	if (count != targetNum) {
		setTimer(timer1, duration);
		startTimer(timer1);
	}


}
void Game2_main() {

	scene1_g2 = createScene("�غ� ȭ��", "image/game2/���.png");
	scene2_g2 = createScene("�޸� ����", "image/game2/���.png");

	startbutton = createObject("image/game2/����.png", scene1_g2, 610, 70, true);
	restartbutton = createObject("image/game2/�ٽý���.png", scene2_g2, 610, 400, false);
	endbutton = createObject("image/game2/Ȯ��.png", scene2_g2, 610, 350, false);

	heart1 = createObject("image/game2/heart.png", scene2_g2, 830, 650, true);
	scaleObject(heart1, 0.05f);
	heart2 = createObject("image/game2/heart.png", scene2_g2, 900, 650, true);
	scaleObject(heart2, 0.05f);
	heart3 = createObject("image/game2/heart.png", scene2_g2, 970, 650, true);
	scaleObject(heart3, 0.05f);

	timer1 = createTimer(duration);

	
}