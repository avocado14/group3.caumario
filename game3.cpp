#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID scene1_g3, scene2_g3;
ObjectID startbutton_g3, restartbutton_g3, endbutton_g3, normal, blue, red, yellow, heart1_g3, heart2_g3, heart3_g3, damaged_g3;
ObjectID enemy[150] = { 0, };
TimerID check, damageTime_g3;
SoundID bgm_g3, finalHit, finalHit1, normalHit, normalHit1, normalHit2, normalHit3, normalHit4;
int arr[150] = { 0, }, Elife_g3[150] = { 0, }, locate[150] = { 0, };
int count_g3 = 0, clear_g3 = 0, life_g3 = 3, location = 60, Nhitcount_g3 = 0, Fhitcount_g3 = 0, score = 0;
char name[30] = { 0, }, info_g3[30] = { 0, };
float duration_g3 = 1.0f;

extern ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown);

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

void enemylife_g3() {			// 적 목숨 랜덤 설정

	srand(time(NULL));

	for (int i = 0; i < 32; i++) {
		if (arr[i] == 5 || arr[i] == 6 || arr[i] == 7 || arr[i] == 8) {
			Elife_g3[i] = rand() % 3 + 2;
		}
	}
}

void createEnemy() {		// 적 오브젝트 생성

	int i;

	for (i = 0; i < 32; i++) {

		if (arr[i] != 0) {
			sprintf_s(name, "image/game3/%d.png", arr[i]);
			enemy[i] = createObject(name, scene2_g3, 610, 720, false);
		}
	}
}

void ending_g3() {		//게임 클리어 후 처리

	stopTimer(check);

	for (int i = 0; i < 32; i++) {
		if (arr[i] != 0) hideObject(enemy[i]);
	}

	sprintf_s(info_g3, "점수 : %d", score);
	showMessage(info_g3);

	showObject(restartbutton_g3);
	showObject(endbutton_g3);

}

void minusHeart_g3() {			// 오답 시 목숨 감소

	life_g3--;
	showObject(damaged_g3);

	setTimer(damageTime_g3, 0.2f);
	startTimer(damageTime_g3);

	if (life_g3 == 2) hideObject(heart3_g3);
	else if (life_g3 == 1) hideObject(heart2_g3);
	else if (life_g3 == 0) {
		hideObject(heart1_g3);
		ending_g3();
	}
}

void zeroCheck() {			// 다음 오브젝트가 없을 때 변수 clear_g3 증가 함수
	if (clear_g3 != 32 && arr[clear_g3] == 0) {
		clear_g3++;
		if (arr[clear_g3] != 0) return;
		zeroCheck();
	}
}
/*
void normalHitPlay() {		// 평타 효과음 재생
	switch (Nhitcount_g3) {
	case 0:
		playSound(normalHit, false);
		Nhitcount_g3++;
		break;
	case 1:
		playSound(normalHit1, false);
		Nhitcount_g3++;
		break;
	case 2:
		playSound(normalHit2, false);
		Nhitcount_g3++;
		break;
	case 3:
		playSound(normalHit3, false);
		Nhitcount_g3++;
		break;
	case 4:
		playSound(normalHit4, false);
		Nhitcount_g3 = 0;
		break;
	}
}

void finalHitPlay() {		// 막타 효과음 재생
	switch (Fhitcount_g3) {
	case 0:
		playSound(finalHit, false);
		Fhitcount_g3++;
		break;
	case 1:
		playSound(finalHit1, false);
		Fhitcount_g3 = 0;
		break;
	}
}
*/

void judge(int num1, int num2) {	// 클릭 시 정,오답 확인

	if (clear_g3 == 32) return;
	else if (life_g3 == 0) return;
	else if (arr[clear_g3] == num1) {
		hideObject(enemy[clear_g3]);
		clear_g3++;
		zeroCheck();
		//normalHitPlay();
		score++;
	}
	else if (arr[clear_g3] == num2) {
		Elife_g3[clear_g3]--;
		if (Elife_g3[clear_g3] != 0) {
			//normalHitPlay();
		}
		if (Elife_g3[clear_g3] == 0) {
			hideObject(enemy[clear_g3]);
			clear_g3++;
			zeroCheck();
			//finalHitPlay();
			score++;
		}
	}
	else {
		minusHeart_g3();
	}
}

void Game3_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == startbutton_g3) {
		enterScene(scene2_g3);

		patternMaker();
		enemylife_g3();
		createEnemy();

		zeroCheck();

		playSound(bgm_g3, true);

		setTimer(check, duration_g3);
		startTimer(check);
	}

	else if (object == restartbutton_g3) {
		for (int i = 0; i < 150; i++) enemy[i] = 0;
		for (int i = 0; i < 150; i++) arr[i] = 0;
		for (int i = 0; i < 150; i++) Elife_g3[i] = 0;
		for (int i = 0; i < 150; i++) locate[i] = 0;
		for (int i = 0; i < 30; i++) name[i] = 0;
		count_g3 = 0;
		clear_g3 = 0;
		life_g3 = 3;
		score = 0;

		patternMaker();
		enemylife_g3();
		createEnemy();

		hideObject(restartbutton_g3);
		hideObject(endbutton_g3);

		showObject(heart1_g3);
		showObject(heart2_g3);
		showObject(heart3_g3);

		zeroCheck();

		setTimer(check, duration_g3);
		startTimer(check);
	}

	else if (object == endbutton_g3) {

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

		if (arr[count_g3] != 0) {
			showObject(enemy[count_g3]);
		}

		for (int i = 0; i < count_g3 + 1; i++) {
			if (arr[i] != 0 && i >= clear_g3) {
				locate[i] += location;
				locateObject(enemy[i], scene2_g3, 610, 720 - locate[i]);
				if (locate[i] > 640) {
					life_g3 = 0;
					hideObject(heart1_g3);
					hideObject(heart2_g3);
					hideObject(heart3_g3);
					ending_g3();
					return;
				}
			}
		}

		count_g3++;

		if (clear_g3 == 32) {

			for (int i = 0; i < 150; i++) enemy[i] = 0;
			for (int i = 0; i < 150; i++) arr[i] = 0;
			for (int i = 0; i < 150; i++) Elife_g3[i] = 0;
			for (int i = 0; i < 150; i++) locate[i] = 0;
			for (int i = 0; i < 30; i++) name[i] = 0;
			count_g3 = 0;
			clear_g3 = 0;
			duration_g3 -= 0.1f;
			patternMaker();
			enemylife_g3();
			createEnemy();
		}

		setTimer(check, duration_g3);
		startTimer(check);
	}

	if (timer == damageTime_g3) {
		hideObject(damaged_g3);
	}
}

void Game3_main() {

	scene1_g3 = createScene("스피드 스워드", "image/game3/cover.png");
	scene2_g3 = createScene("적을 모두 무찌르세요!", "image/game3/cover2.png");

	startbutton_g3 = createObject("image/game3/시작.png", scene1_g3, 610, 70, true);
	restartbutton_g3 = createObject("image/game3/다시시작.png", scene2_g3, 610, 400, false);
	endbutton_g3 = createObject("image/game3/취소.png", scene2_g3, 610, 350, false);

	heart1_g3 = createObject("image/game3/heart.png", scene2_g3, 830, 650, true);
	scaleObject(heart1_g3, 0.05f);
	heart2_g3 = createObject("image/game3/heart.png", scene2_g3, 900, 650, true);
	scaleObject(heart2_g3, 0.05f);
	heart3_g3 = createObject("image/game3/heart.png", scene2_g3, 970, 650, true);
	scaleObject(heart3_g3, 0.05f);

	normal = createObject("image/game3/N.png", scene2_g3, 370, 40, true);
	blue = createObject("image/game3/B.png", scene2_g3, 470, 40, true);
	red = createObject("image/game3/R.png", scene2_g3, 730, 40, true);
	yellow = createObject("image/game3/Y.png", scene2_g3, 830, 40, true);

	damaged_g3 = createObject("image/game3/damage.png", scene2_g3, 0, 0, false);

	bgm_g3 = createSound("image/game3/bgm.wav");
	normalHit = createSound("image/game3/normalHit.wav");
	normalHit1 = createSound("image/game3/normalHit1.wav");
	normalHit2 = createSound("image/game3/normalHit2.wav");
	normalHit3 = createSound("image/game3/normalHit3.wav");
	normalHit4 = createSound("image/game3/normalHit4.wav");
	finalHit = createSound("image/game3/finalHit.wav");
	finalHit1 = createSound("image/game3/finalHit1.wav");

	check = createTimer(duration_g3);
	damageTime_g3 = createTimer(0.2f);
}