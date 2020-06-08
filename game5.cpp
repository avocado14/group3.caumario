#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SPEED				20
#define fire_SPEED				    20
#define PLAYER_ANIMATION_TIME		0.05f

SceneID scene_g5;
ObjectID player_g5, koopa;
ObjectID attackButton, startButton_g5;
ObjectID fire1[4], fire2[4];
ObjectID HPbar, HPbarLine, HPbarBlank, HPright;
TimerID playTimer_g5, jumpTimer_g5, fire1MoveTimer, fire2MoveTimer, koopaDamageTimer;

int dx_g5 = 0, dy_g5 = 0;

float playerSize_g5 = 100;
float koopaSizeX = 180, koopaSizeY = 240;
float fireSize = 45;

int playerX_g5 = 100, playerY_g5 = 150, playerBottom = playerY_g5, playerTop = 500, gravity_g5 = 25;
bool isRising_g5, isLanded = true;

int koopaX = 800, koopaY = 200;
int koopaCount = 0;
int koopaHP = 500;

int fire1X[4], fire1Y, fire2X[4], fire2Y;
int fire1Num = 0, fire2Num = 0;
bool attacking1 = false, attacking2 = false;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

void jump_g5() {

    if (isRising_g5)    //���
    {
        playerY_g5 += gravity_g5;
       
        if (playerY_g5 > playerTop)  {     //��� ������ -> else
            playerY_g5 -= gravity_g5;
            isRising_g5 = false;
        }

        locateObject(playerX_g5, scene_g5, playerX_g5, playerY_g5);

        setTimer(jumpTimer_g5, 0.01f);
        startTimer(jumpTimer_g5);
    }

    else  //����
    {
        playerY_g5 -= gravity_g5;

        if (playerY_g5 < playerBottom) {    //���� ������
            playerY_g5 += gravity_g5;
            isLanded = true;
        }

        locateObject(playerX_g5, scene_g5, playerX_g5, playerY_g5);

        if (isLanded == false) {        //���� �������� Ÿ�̸� ���
            setTimer(jumpTimer_g5, 0.01f);
            startTimer(jumpTimer_g5);
        }

    }

}

void attack1() {

    for (int i = 0; i < 4; i++) {
        fire1X[i] = playerX_g5 + 50 - 40 * i;
        fire1Y = playerY_g5 + 20;
        locateObject(fire1[i], scene_g5, fire1X[i], fire1Y);
    }
    showObject(fire1[0]);

    fire1Num = 0;

    startTimer(fire1MoveTimer);

}

void attack2() {

    for (int i = 0; i < 4; i++) {
        fire2X[i] = playerX_g5 + 50 - 40 * i;
        fire2Y = playerY_g5 + 20;
        locateObject(fire2[i], scene_g5, fire2X[i], fire2Y);

    }
    showObject(fire2[0]);

    fire2Num = 0;

    startTimer(fire2MoveTimer);

}

void koopaDamage() {

    //������ �Դ� �ִϸ��̼�
    koopaCount = 0;
    setTimer(koopaDamageTimer, 0.05f);
    startTimer(koopaDamageTimer);

    koopaHP -= 20;  //(20/500) 25�� ������ ����
    
    //ü�¹� �پ��� (Ⱥ���� �������� �ű��)
    //x : hp + 640 
    locateObject(HPbarBlank, scene_g5, koopaHP + 640, 640);

    if (koopaHP == 0) {
        showMessage("Ŭ����");
        hideObject(koopa);
        stopTimer(playTimer_g5);
    }


}

void Game5_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

   if (object == startButton_g5) {
        startTimer(playTimer_g5);
        hideObject(startButton_g5);
    }

    else if (object == attackButton) {
        if (attacking1 == false) {
            attack1();
            attacking1 = true;
        }
        
        else if (attacking2 == false) {
            attack2();
            attacking2 = true;
        }
    }
}


void Game5_timerCallback(TimerID timer) {

    if (timer == jumpTimer_g5) {
        jump_g5();
        setTimer(jumpTimer_g5, 0.01f);
        startTimer(jumpTimer_g5);
    }

    if (timer == playTimer_g5) {
        if (playerX_g5 + dx_g5 > 1280 - playerSize_g5 || playerX_g5 + dx_g5 < 0 || playerY_g5 + dy_g5 > 720 - playerSize_g5 || playerY_g5 + dy_g5 < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

        else {
            playerX_g5 += dx_g5; playerY_g5 += dy_g5;
            locateObject(player_g5, scene_g5, playerX_g5, playerY_g5);

            if (koopaX - playerX_g5 <= playerSize_g5 && koopaX - playerX_g5 >= -koopaSizeX &&		//�÷��̾ ���Ķ� x�� �ε�����
                koopaY - playerY_g5 <= playerSize_g5 && koopaY - playerY_g5 >= -koopaSizeY) {       //y�൵ �ε�����
            }

        }

        setTimer(timer, PLAYER_ANIMATION_TIME);
        startTimer(timer);
    }

    if (timer == fire1MoveTimer) {

        //�� ���ʷ� ����
        if (fire1Num == 2)
            showObject(fire1[1]);
        else if (fire1Num == 4)
            showObject(fire1[2]);
        else if (fire1Num == 6)
            showObject(fire1[3]);

        if (fire1Num < 6)
            fire1Num++;

        //�� �̵�
        for (int i = 0; i < 4; i++) {
            fire1X[i] += fire_SPEED;
            locateObject(fire1[i], scene_g5, fire1X[i], fire1Y);
        }

        //�� ������ ����
        if (fire1X[0] > 1200)               //ȭ�� ������ ������ ���� ��
            attacking1 = false;

        if (koopaX - fire1X[0] >= -koopaSizeX && koopaX - fire1X[0] <= fireSize &&
            fire1Y - koopaY >= -30 && fire1Y - koopaY <= koopaSizeY){    //���� ���Ķ� �ε�����

            koopaDamage();

            attacking1 = false;
        }

        //���� ���� ��
        if (attacking1 == false) {

            for (int i = 0; i < 4; i++)
                hideObject(fire1[i]);
        }

        //���� ����
        else {
            setTimer(fire1MoveTimer, 0.05f);
            startTimer(fire1MoveTimer);
        }

    }

    if (timer == fire2MoveTimer) {

        if (fire2Num == 2)
            showObject(fire2[1]);
        else if (fire2Num == 4)
            showObject(fire2[2]);
        else if (fire2Num == 6)
            showObject(fire2[3]);

        if (fire2Num < 6)
            fire2Num++;

        
        for (int i = 0; i < 4; i++) {      
            fire2X[i] += fire_SPEED;
            locateObject(fire2[i], scene_g5, fire2X[i], fire2Y);
        }

        
        if (fire2X[0] > 1200)               //ȭ�� ������ ������ ���� ��
            attacking2 = false;

        if (koopaX - fire2X[0] >= -koopaSizeX && koopaX - fire2X[0] <= fireSize &&
            fire2Y - koopaY >= -30 && fire2Y - koopaY <= koopaSizeY) {    //���� ���Ķ� �ε�����

            koopaDamage();

            attacking2 = false;
        }

        
        if (attacking2 == false) {

            for (int i = 0; i < 4; i++)
                hideObject(fire2[i]);
        }
        
        else {
            setTimer(fire2MoveTimer, 0.05f);
            startTimer(fire2MoveTimer);
        }

    }

    if (timer == koopaDamageTimer) {
        
        if (koopaCount % 2 == 0)
            setObjectImage(koopa, "image/game5/��������.png");
        else
            setObjectImage(koopa, "image/game5/����.png");
        
        if (koopaCount < 3) {
            koopaCount++;
            setTimer(koopaDamageTimer, 0.05f);
            startTimer(koopaDamageTimer);
        }
    }
}

void Game5_keyboardCallback(KeyCode code, KeyState state) {

    if (code == 4) {		// RIGHT
        dx_g5 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
    }
    else if (code == 1) {		// LEFT
        dx_g5 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
    }

    if (code == 23) {			// Jump
        if (isLanded == true) {//�ٴڿ� ���� ���� ����
            isRising_g5 = true;
            isLanded = false;
            startTimer(jumpTimer_g5);
        }

    }
}



void Game5_soundCallback(SoundID sound) {



}




void Game5_main() {

	scene_g5 = createScene("���� ��", "image/game5/���.png");

    startButton_g5 = createObject("image/game5/���ӽ���.png", scene_g5, 500, 100, true, 1.0f);
    attackButton = createObject("image/game5/����.png", scene_g5, 300, 100, true, 1.0f);
    //ü�¹� : 500 x 40
    HPbar = createObject("image/game5/ü�¹�.png", scene_g5, 640, 640, true, 1.0f);
    HPbarBlank = createObject("image/game5/ü�¹����.png", scene_g5, 1140, 640, true, 1.0f);
    HPright = createObject("image/game5/ü�¹ٹ��.png", scene_g5, 1140, 640, true, 1.0f);
    HPbarLine = createObject("image/game5/ü�¹��׵θ�.png", scene_g5, 640, 640, true, 1.0f);

	player_g5 = createObject("image/game5/������.png", scene_g5, playerX_g5, playerBottom, true, 1.0f);
	koopa = createObject("image/game5/����.png", scene_g5, koopaX, koopaY, true, 1.0f);

    char path_g5[256];

    for (int i = 0; i < 4; i++) {
        sprintf_s(path_g5, "image/game5/��%d.png", i + 1);
        fire1[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
        fire2[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
    }

    playTimer_g5 = createTimer(PLAYER_ANIMATION_TIME);
    jumpTimer_g5 = createTimer(0.01f);
    fire1MoveTimer = createTimer(0.05f);
    fire2MoveTimer = createTimer(0.05f);
    koopaDamageTimer = createTimer(0.05f);


}