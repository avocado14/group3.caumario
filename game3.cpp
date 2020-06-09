#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1_g3, scene2_g3;
ObjectID startbutton, restartbutton, endbutton, normal, blue, red, yellow, heart1, heart2, heart3, damaged;
ObjectID enemy[150] = { 0, };
TimerID check, damageTime;
SoundID bgm, finalHit, finalHit1, normalHit, normalHit1, normalHit2, normalHit3, normalHit4;
int arr[150] = { 0, }, Elife[150] = { 0, }, locate[150] = { 0, };
int count = 0, clear = 0, life = 3, location = 60, NhitCount = 0, FhitCount = 0, score = 0;
char name[30] = { 0, }, info[30] = { 0, };
float duration = 1.0f;

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {

	ObjectID object = createObject(image);

	locateObject(object, scene, x, y);

	if (shown) {
		showObject(object);
	}

	return object;
}

void patternMaker() {		// 적 특성 저장할 정수 배열 만들기

	int i, x, y, temp;

	srand(time(NULL));

	//1~4까지는 목숨하나 몹 차례대로 N,B,R,Y
	//5~8까지는 목숨 여러개 몹 차례대로 N,B,R,Y

	for (i = 0; i < 32; i++) {
		if (i < 5) arr[i] = 1;
		else if (i < 10) arr[i] = 2;
		else if (i < 15) arr[i] = 3;
		else if (i < 20) arr[i] = 4;
		else if (i < 22) arr[i] = 5;
		else if (i < 24) arr[i] = 6;
		else if (i < 26) arr[i] = 7;
		else if (i < 28) arr[i] = 8;
		else if (i < 32) arr[i] = 0;
	}

	for (i = 0; i < 300; i++) {
		x = rand() % 32;
		y = rand() % 32;

		if (x != y) {
			temp = arr[x];
			arr[x] = arr[y];
			arr[y] = temp;
		}
	}
}

void enemyLife() {			// 적 목숨 랜덤 설정

	srand(time(NULL));

	for (int i = 0; i < 32; i++) {
		if (arr[i] == 5 || arr[i] == 6 || arr[i] == 7 || arr[i] == 8) {
			Elife[i] = rand() % 3 + 2;
		}
	}
}

void createEnemy() {		// 적 오브젝트 생성

	int i;

	for (i = 0; i < 32; i++) {

		if (arr[i] != 0) {
			sprintf_s(name, "images/%d.png", arr[i]);
			enemy[i] = createObject(name, scene2_g3, 610, 720, false);
		}
	}
}

void ending() {		//게임 클리어 후 처리

	stopTimer(check);

	for (int i = 0; i < 32; i++) {
		if (arr[i] != 0) hideObject(enemy[i]);
	}

	sprintf_s(info, "점수 : %d", score);
	showMessage(info);

	showObject(restartbutton);
	showObject(endbutton);

}

void minusHeart() {			// 오답 시 목숨 감소

	life--;
	showObject(damaged);

	setTimer(damageTime, 0.2f);
	startTimer(damageTime);

	if (life == 2) hideObject(heart3);
	else if (life == 1) hideObject(heart2);
	else if (life == 0) {
		hideObject(heart1);
		ending();
	}
}

void zeroCheck() {			// 다음 오브젝트가 없을 때 변수 clear 증가 함수
	if (clear != 32 && arr[clear] == 0) {
		clear++;
		if (arr[clear] != 0) return;
		zeroCheck();
	}
}
/*
void normalHitPlay() {		// 평타 효과음 재생
	switch (NhitCount) {
	case 0:
		playSound(normalHit, false);
		NhitCount++;
		break;
	case 1:
		playSound(normalHit1, false);
		NhitCount++;
		break;
	case 2:
		playSound(normalHit2, false);
		NhitCount++;
		break;
	case 3:
		playSound(normalHit3, false);
		NhitCount++;
		break;
	case 4:
		playSound(normalHit4, false);
		NhitCount = 0;
		break;
	}
}

void finalHitPlay() {		// 막타 효과음 재생
	switch (FhitCount) {
	case 0:
		playSound(finalHit, false);
		FhitCount++;
		break;
	case 1:
		playSound(finalHit1, false);
		FhitCount = 0;
		break;
	}
}
*/

void judge(int num1, int num2) {	// 클릭 시 정,오답 확인

	if (clear == 32) return;
	else if (life == 0) return;
	else if (arr[clear] == num1) {
		hideObject(enemy[clear]);
		clear++;
		zeroCheck();
		//normalHitPlay();
		score++;
	}
	else if (arr[clear] == num2) {
		Elife[clear]--;
		if (Elife[clear] != 0) {
			//normalHitPlay();
		}
		if (Elife[clear] == 0) {
			hideObject(enemy[clear]);
			clear++;
			zeroCheck();
			//finalHitPlay();
			score++;
		}
	}
	else {
		minusHeart();
	}
}

void Game3_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startbutton) {
		enterScene(scene2_g3);

		patternMaker();
		enemyLife();
		createEnemy();

		zeroCheck();

		playSound(bgm, true);

		setTimer(check, duration);
		startTimer(check);
	}

	else if (object == restartbutton) {
		for (int i = 0; i < 150; i++) enemy[i] = 0;
		for (int i = 0; i < 150; i++) arr[i] = 0;
		for (int i = 0; i < 150; i++) Elife[i] = 0;
		for (int i = 0; i < 150; i++) locate[i] = 0;
		for (int i = 0; i < 30; i++) name[i] = 0;
		count = 0;
		clear = 0;
		life = 3;
		score = 0;

		patternMaker();
		enemyLife();
		createEnemy();

		hideObject(restartbutton);
		hideObject(endbutton);

		showObject(heart1);
		showObject(heart2);
		showObject(heart3);

		zeroCheck();

		setTimer(check, duration);
		startTimer(check);
	}

	else if (object == endbutton) {

		endGame();
	}

	else if (object == normal) {

		judge(1, 5);
	}

	else if (object == blue) {

		judge(2, 6);
	}

	else if (object == red) {

		judge(3, 7);
	}

	else if (object == yellow) {

		judge(4, 8);
	}
}

void Game3_timerCallback(TimerID timer) {

	if (timer == check) {

		if (arr[count] != 0) {
			showObject(enemy[count]);
		}

		for (int i = 0; i < count + 1; i++) {
			if (arr[i] != 0 && i >= clear) {
				locate[i] += location;
				locateObject(enemy[i], scene2_g3, 610, 720 - locate[i]);
				if (locate[i] > 640) {
					life = 0;
					hideObject(heart1);
					hideObject(heart2);
					hideObject(heart3);
					ending();
					return;
				}
			}
		}

		count++;

		if (clear == 32) {

			for (int i = 0; i < 150; i++) enemy[i] = 0;
			for (int i = 0; i < 150; i++) arr[i] = 0;
			for (int i = 0; i < 150; i++) Elife[i] = 0;
			for (int i = 0; i < 150; i++) locate[i] = 0;
			for (int i = 0; i < 30; i++) name[i] = 0;
			count = 0;
			clear = 0;
			duration -= 0.1f;
			patternMaker();
			enemyLife();
			createEnemy();
		}

		setTimer(check, duration);
		startTimer(check);
	}

	if (timer == damageTime) {
		hideObject(damaged);
	}
}

int main() {

	scene1_g3 = createScene("스피드 스워드", "image/game3/cover.png");
	scene2_g3 = createScene("적을 모두 무찌르세요!", "image/game3/cover2.png");

	startbutton = createObject("image/game3/시작.png", scene1_g3, 610, 70, true);
	restartbutton = createObject("image/game3/다시시작.png", scene2_g3, 610, 400, false);
	endbutton = createObject("image/game3/취소.png", scene2_g3, 610, 350, false);

	heart1 = createObject("image/game3/heart.png", scene2_g3, 830, 650, true);
	scaleObject(heart1, 0.05f);
	heart2 = createObject("image/game3/heart.png", scene2_g3, 900, 650, true);
	scaleObject(heart2, 0.05f);
	heart3 = createObject("image/game3/heart.png", scene2_g3, 970, 650, true);
	scaleObject(heart3, 0.05f);

	normal = createObject("image/game3/N.png", scene2_g3, 370, 40, true);
	blue = createObject("image/game3/B.png", scene2_g3, 470, 40, true);
	red = createObject("image/game3/R.png", scene2_g3, 730, 40, true);
	yellow = createObject("image/game3/Y.png", scene2_g3, 830, 40, true);

	damaged = createObject("image/game3/damage.png", scene2_g3, 0, 0, false);

	bgm = createSound("image/game3/bgm.wav");
	normalHit = createSound("image/game3/normalHit.wav");
	normalHit1 = createSound("image/game3/normalHit1.wav");
	normalHit2 = createSound("image/game3/normalHit2.wav");
	normalHit3 = createSound("image/game3/normalHit3.wav");
	normalHit4 = createSound("image/game3/normalHit4.wav");
	finalHit = createSound("image/game3/finalHit.wav");
	finalHit1 = createSound("image/game3/finalHit1.wav");

	check = createTimer(duration);
	damageTime = createTimer(0.2f);
}