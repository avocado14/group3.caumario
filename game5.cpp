#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SPEED				20
#define FIRE_SPEED				    25
#define KOOPA_FIRE_SPEED            15
#define KFIRE_COOLTIME              4.0f
#define KBALL_COOLTIME              4.0f
#define PLAYER_ANIMATION_TIME		0.05f
#define GAP                         20

extern SceneID endingScene;
SceneID scene_g5;
ObjectID player_g5, koopa;
ObjectID startButton_g5, restartButton_g5, goMapButton_g5;
ObjectID fire1[4], fire2[4];
ObjectID koopaFire1, koopaFire2, koopaFire1Ball;
ObjectID HPbar, HPbarLine, HPbarBlank, HPright;
ObjectID playerHP[3], playerFrame, koopaFrame;
TimerID playTimer_g5, jumpTimer_g5, koopaJumpTimer, fire1MoveTimer, fire2MoveTimer, koopaDamageTimer;
TimerID kFireMoveTimer1, kFireMoveTimer2, kBallMoveTimer;
TimerID kFireCoolTimer1, kFireCoolTimer2, kBallCoolTimer, koopaJumpCoolTimer;
TimerID playerAnimationTimer_g5, koopaAnimationTimer;
SoundID bgm_g5, jumpSound_g5, jumpSound2_g5, playerFireSound1, playerFireSound2, koopaFireSound1, koopaFireSound2, koopaFireSound3;
SoundID koopaDamageSound1, koopaDamageSound2, playerDamageSound;
extern SoundID buttonClickSound, gameClearSound, gameOverSound;

extern void enterTitle(int clearScene);
extern void startEnding();
extern int nowGameSceneNum;
bool stage6Clear = false;

int dx_g5 = 0, dy_g5 = 0;
bool isJumping = false;
bool gameStart = false;

float playerSizeX_g5 = 110, playerSizeY_g5 = 130;
float koopaSizeX = 280, koopaSizeY = 220;
float fireSize = 45;

int playerX_g5 = 100, playerY_g5 = 140, playerBottom = playerY_g5, playerTop = 400, gravity_g5 = 25, koopaTop = 500;
int playerLife = 3;
bool playerDead = false;
bool isRising_g5, isLanded = true;

int koopaX = 900, koopaY = playerY_g5;
int koopaCount = 0;
int koopaHP = 500;

//������ �� ����
int fire1X[4], fire1Y, fire2X[4], fire2Y;
int fire1Num = 0, fire2Num = 0;
bool attacking1 = false, attacking2 = false;

//���� �� ����
int kFire1X, kFire1Y;
int kFire2X, kFire2Y;
int kBallX, kBallY;
float kFireSizeX = 100, kFireSizeY = 50;
float kBallSize = 70;
bool kBallisRising;
bool koopaisRising, koopaisLanded;


//ĳ���� �ִ�
const char* playerAnimationImage[10] =
{ "image/game5/������/������ �ִϸ��̼�1.png","image/game5/������/������ �ִϸ��̼�2.png","image/game5/������/������ �ִϸ��̼�3.png","image/game5/������/������ �ִϸ��̼�4.png",
"image/game5/������/������ �ִϸ��̼�5.png","image/game5/������/������ �ִϸ��̼�6.png" ,"image/game5/������/������ �ִϸ��̼�7.png" ,"image/game5/������/������ �ִϸ��̼�8.png"
,"image/game5/������/������ �ִϸ��̼�9.png" ,"image/game5/������/������ �ִϸ��̼�10.png" };

const char* setKoopaAnimationImage[7] =
{ "image/game5/����/���� �ִϸ��̼�1.png","image/game5/����/���� �ִϸ��̼�2.png","image/game5/����/���� �ִϸ��̼�3.png","image/game5/����/���� �ִϸ��̼�4.png",
"image/game5/����/���� �ִϸ��̼�5.png","image/game5/����/���� �ִϸ��̼�6.png" ,"image/game5/����/���� �ִϸ��̼�7.png" };

const char* setKoopaDamageAnimationImage[7] =
{ "image/game5/��������/�������� �ִϸ��̼�1.png","image/game5/��������/�������� �ִϸ��̼�2.png","image/game5/��������/�������� �ִϸ��̼�3.png","image/game5/��������/�������� �ִϸ��̼�4.png",
"image/game5/��������/�������� �ִϸ��̼�5.png","image/game5/��������/�������� �ִϸ��̼�6.png" ,"image/game5/��������/�������� �ִϸ��̼�7.png" };

const char* koopaAnimationImage[7];

int playerAnimationCount = 0;
int koopaAnimationCount = 0;


extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

void restart_g5() {
    
    dx_g5 = 0, dy_g5 = 0;
    isJumping = false;

    koopaSizeX = 230, koopaSizeY = 300;

    playerX_g5 = 100, playerY_g5 = 150;
    playerLife = 3;
    playerDead = false;
    isRising_g5, isLanded = true;

    koopaX = 900, koopaY = playerY_g5;
    koopaCount = 0;
    koopaHP = 500;

    playerAnimationCount = 0;
    koopaAnimationCount = 0;

    attacking1 = false, attacking2 = false;

    locateObject(HPbarBlank, scene_g5, 1140, 640);
    
    for (int i = 0; i < 3; i++) {
        showObject(playerHP[i]);
    }
    
    setObjectImage(player_g5, "image/game5/������/������ �ִϸ��̼�1.png");
    locateObject(player_g5,scene_g5, playerX_g5, playerBottom);
    showObject(player_g5);
    
    setObjectImage(koopa, "image/game5/����/���� �ִϸ��̼�1.png");
    locateObject(koopa, scene_g5, koopaX, koopaY);
    showObject(koopa);


    setTimer(playTimer_g5, PLAYER_ANIMATION_TIME);
    setTimer(koopaJumpCoolTimer, 4.0f);
    setTimer(kFireCoolTimer1, 2.0f);
    setTimer(kFireCoolTimer2, 2.0f + KFIRE_COOLTIME / 2);
    setTimer(kBallCoolTimer, 3.0f);

}

void gameClear_g5() {

    gameStart = false;
    stopSound(bgm_g5);
    playSound(gameClearSound);

    showMessage("���ĸ� �����ƽ��ϴ�!");
    stage6Clear = true;
    setObjectImage(goMapButton_g5, "image/game5/goEnding.png");
    locateObject(goMapButton_g5, scene_g5, 450, 340);


    
    //������Ʈ �� �����
    hideObject(player_g5);
    hideObject(koopa);

    hideObject(koopaFire1), hideObject(koopaFire2);
    hideObject(koopaFire1Ball);

    for (int i = 0; i < 4; i++) {
        hideObject(fire1[i]);
        hideObject(fire2[i]);
    }

    showObject(goMapButton_g5);

    stopTimer(playTimer_g5);
    stopTimer(playerAnimationTimer_g5);
    stopTimer(koopaAnimationTimer);
    stopTimer(kFireCoolTimer1);
    stopTimer(kFireCoolTimer2);
    stopTimer(kBallCoolTimer);
    stopTimer(koopaJumpCoolTimer);
    stopTimer(jumpTimer_g5);
    stopTimer(fire1MoveTimer);
    stopTimer(fire2MoveTimer);
    stopTimer(kFireMoveTimer1);
    stopTimer(kFireMoveTimer2);
    stopTimer(kBallMoveTimer);
    stopTimer(koopaDamageTimer);
}

void gameOver_g5() {

    gameStart = false;
    playSound(gameOverSound);

    showMessage("���� ����");

    //������Ʈ �� �����
    hideObject(player_g5);
    hideObject(koopa);
   
    hideObject(koopaFire1), hideObject(koopaFire2); 
    hideObject(koopaFire1Ball);

    for (int i = 0; i < 4; i++) {
        hideObject(fire1[i]);
        hideObject(fire2[i]);
    }

    showObject(restartButton_g5);
    showObject(goMapButton_g5);

    stopTimer(playTimer_g5);
    stopTimer(playerAnimationTimer_g5);
    stopTimer(koopaAnimationTimer);
    stopTimer(kFireCoolTimer1);
    stopTimer(kFireCoolTimer2);
    stopTimer(kBallCoolTimer);
    stopTimer(koopaJumpCoolTimer);
    stopTimer(jumpTimer_g5);
    stopTimer(fire1MoveTimer);
    stopTimer(fire2MoveTimer);
    stopTimer(kFireMoveTimer1);
    stopTimer(kFireMoveTimer2);
    stopTimer(kBallMoveTimer);
    stopTimer(koopaDamageTimer);
}

void jump_g5() {

    if (isRising_g5)    //���
    {
        playerY_g5 += gravity_g5;
       
        if (playerY_g5 > playerTop)  {     //��� ������ -> else
            playerY_g5 -= gravity_g5;
            isRising_g5 = false;
        }

        locateObject(player_g5, scene_g5, playerX_g5, playerY_g5);

        setTimer(jumpTimer_g5, 0.05f);
        startTimer(jumpTimer_g5);
    }

    else  //����
    {
        playerY_g5 -= gravity_g5;

        if (playerY_g5 < playerBottom) {    //���� ������
            playerY_g5 += gravity_g5;
            isLanded = true;
            stopTimer(jumpTimer_g5);
        }

        locateObject(player_g5, scene_g5, playerX_g5, playerY_g5);

        if (isLanded == false) {        //���� �������� Ÿ�̸� ���
            setTimer(jumpTimer_g5, 0.05f);
            startTimer(jumpTimer_g5);
        }
    }
}

void koopaJump() {

    if (koopaisRising)    //���
    {
        koopaY += gravity_g5;

        if (koopaY > koopaTop) {     //��� ������ -> else
            koopaY -= gravity_g5;
            koopaisRising = false;
        }

        locateObject(koopa, scene_g5, koopaX, koopaY);

        setTimer(koopaJumpTimer, 0.05f);
        startTimer(koopaJumpTimer);
    }

    else  //����
    {
        koopaY -= gravity_g5;

        if (koopaY < playerBottom) {    //���� ������
            koopaY += gravity_g5;
            koopaisLanded = true;
            stopTimer(koopaJumpTimer);
        }

        locateObject(koopa, scene_g5, koopaX, koopaY);

        if (koopaisLanded == false) {        //���� �������� Ÿ�̸� ���
            setTimer(koopaJumpTimer, 0.05f);
            startTimer(koopaJumpTimer);
        }
    }
}

void attack1() {

    playSound(playerFireSound1);

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

    playSound(playerFireSound2);

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
    if (koopaHP >= 0) {
        locateObject(HPbarBlank, scene_g5, koopaHP + 640, 640);
    }

    if (koopaHP == 0) {
        gameClear_g5();
    }

}

void playerDamage() {

    playSound(playerDamageSound);

    playerLife--;

    if (playerLife >= 0) {
        for (int i = playerLife; i < 3; i++) {
            hideObject(playerHP[i]);
        }
    }

    if (playerLife <= 0) {

        for (int i = 0; i < 3; i++) {
            hideObject(playerHP[i]);
        }

        gameOver_g5();
    }

}

//�÷��̾� ���ؼ� �� �߻�
void koopaFire1Attack() {

    playSound(koopaFireSound1);

    kFire1X = koopaX;
    kFire1Y = playerY_g5;

    locateObject(koopaFire1, scene_g5, kFire1X, kFire1Y);
    showObject(koopaFire1);

    startTimer(kFireMoveTimer1);

}

void koopaFire2Attack() {

    playSound(koopaFireSound2);

    kFire2X = koopaX;
    kFire2Y = playerY_g5;

    locateObject(koopaFire2, scene_g5, kFire2X, kFire2Y);
    showObject(koopaFire2);

    startTimer(kFireMoveTimer2);

}

//ƨ��� �Ұ� �ٴ����� ������
void koopaBallAttack() {
    
    playSound(koopaFireSound3);

    kBallX = koopaX;
    kBallY = koopaY + 200;

    locateObject(koopaFire1Ball, scene_g5, kBallX, kBallY);
    showObject(koopaFire1Ball);

    startTimer(kBallMoveTimer);

}



void Game5_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

   if (object == startButton_g5) {
       
       gameStart = true;
       playSound(buttonClickSound);

       hideObject(startButton_g5);
       hideObject(goMapButton_g5);
       
       startTimer(playTimer_g5);
       startTimer(playerAnimationTimer_g5);
       startTimer(koopaAnimationTimer);
       startTimer(kFireCoolTimer1);
       startTimer(kFireCoolTimer2);
       startTimer(kBallCoolTimer);
       startTimer(koopaJumpCoolTimer);
    }

   else if (object == restartButton_g5) {

       gameStart = true;
       playSound(buttonClickSound);

       hideObject(restartButton_g5);
       hideObject(goMapButton_g5);

       restart_g5();

       startTimer(playTimer_g5);
       startTimer(playerAnimationTimer_g5);
       startTimer(koopaAnimationTimer);
       startTimer(kFireCoolTimer1);
       startTimer(kFireCoolTimer2);
       startTimer(kBallCoolTimer);
       startTimer(koopaJumpCoolTimer);
   }

   else if (object == goMapButton_g5) {
       
       playSound(buttonClickSound);
       stopSound(bgm_g5);
       
       if(stage6Clear == false)
           enterTitle(0);

       //Ŭ���� ������
       else {
           setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
           startEnding();
           enterScene(endingScene);
       }
   }
}


void Game5_timerCallback(TimerID timer) {
    
    if (timer == jumpTimer_g5) {
        jump_g5();
    }

    if (timer == playTimer_g5) {
        if (playerX_g5 + dx_g5 > 1280 - playerSizeX_g5 || playerX_g5 + dx_g5 < 0 || playerY_g5 + dy_g5 > 720 - playerSizeY_g5 || playerY_g5 + dy_g5 < 0) {}	//�׵θ� ������ �̵� �Ƚ�Ű��

        else {
            playerX_g5 += dx_g5; playerY_g5 += dy_g5;
            locateObject(player_g5, scene_g5, playerX_g5, playerY_g5);

        }

        if (playerX_g5 >= koopaX - playerSizeX_g5 && playerX_g5 <= koopaX + koopaSizeX &&
            playerY_g5 >= koopaY - playerSizeY_g5 && playerY_g5 <= koopaY + koopaSizeY) {     //������ ���Ķ� �ε�����
        
            playerLife = 0;
            playerDamage();

            return;
        }

        if (isJumping == true) {
            
            if (isLanded == true) {//�ٴڿ� ���� ���� ����
                playSound(jumpSound_g5);
                
                isRising_g5 = true;
                isLanded = false;
                startTimer(jumpTimer_g5);
            }
        }

         setTimer(timer, PLAYER_ANIMATION_TIME);
         startTimer(timer);
        
    }

    if (timer == playerAnimationTimer_g5) {

        setObjectImage(player_g5, playerAnimationImage[playerAnimationCount % 10]);
        playerAnimationCount++;

        setTimer(playerAnimationTimer_g5, 0.07f);
        startTimer(playerAnimationTimer_g5);

    }

    if (timer == koopaAnimationTimer) {

        setObjectImage(koopa, koopaAnimationImage[koopaAnimationCount % 7]);
        koopaAnimationCount++;

        setTimer(koopaAnimationTimer, 0.2f);
        startTimer(koopaAnimationTimer);

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
            fire1X[i] += FIRE_SPEED;
            locateObject(fire1[i], scene_g5, fire1X[i], fire1Y);
        }

        //�� ������ ����
        if (fire1X[0] > 1200)               //ȭ�� ������ ������ ���� ��
            attacking1 = false;

        if (koopaX - fire1X[0] >= -koopaSizeX && koopaX - fire1X[0] <= fireSize &&
            fire1Y - koopaY >= -30 && fire1Y - koopaY <= koopaSizeY){    //���� ���Ķ� �ε�����

            koopaDamage();
            playSound(koopaDamageSound1);

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
            fire2X[i] += FIRE_SPEED;
            locateObject(fire2[i], scene_g5, fire2X[i], fire2Y);
        }

        
        if (fire2X[0] > 1200)               //ȭ�� ������ ������ ���� ��
            attacking2 = false;

        if (koopaX - fire2X[0] >= -koopaSizeX && koopaX - fire2X[0] <= fireSize &&
            fire2Y - koopaY >= -30 && fire2Y - koopaY <= koopaSizeY) {    //���� ���Ķ� �ε�����

            koopaDamage();
            playSound(koopaDamageSound2);

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
        
        if (koopaCount == 0) {
            for (int i = 0; i < 7; i++) {
                koopaAnimationImage[i] = setKoopaDamageAnimationImage[i];
            }
        }
        else if (koopaCount == 5) {
            for (int i = 0; i < 7; i++) {
                koopaAnimationImage[i] = setKoopaAnimationImage[i];
            }
        }
            
        
        if (koopaCount < 5) {
            koopaCount++;
            setTimer(koopaDamageTimer, 0.05f);
            startTimer(koopaDamageTimer);
        }
    }


    if (timer == kFireMoveTimer1) {

        kFire1X -= KOOPA_FIRE_SPEED;
        locateObject(koopaFire1, scene_g5, kFire1X, kFire1Y);

        if (kFire1X < 10)        //ȭ�� ������ ������ �����
            hideObject(koopaFire1);

        else if (kFire1X >= playerX_g5 - kFireSizeX + GAP && kFire1X <= playerX_g5 + playerSizeX_g5 - GAP &&
                 kFire1Y >= playerY_g5 - kFireSizeY + GAP && kFire1Y <= playerY_g5 + playerSizeY_g5 - GAP) {     //�������� �ε�����

            playerDamage();

            hideObject(koopaFire1);
        }

        else {
            setTimer(kFireMoveTimer1, 0.05f);
            startTimer(kFireMoveTimer1);
        }

    }

    if (timer == kFireMoveTimer2) {

        kFire2X -= KOOPA_FIRE_SPEED + 10;
        locateObject(koopaFire2, scene_g5, kFire2X, kFire2Y);

        if (kFire2X < 10)        //ȭ�� ������ ������ �����
            hideObject(koopaFire2);

        else if (kFire2X >= playerX_g5 - kFireSizeX + GAP && kFire2X <= playerX_g5 + playerSizeX_g5 - GAP &&
                 kFire2Y >= playerY_g5 - kFireSizeY + GAP && kFire2Y <= playerY_g5 + playerSizeY_g5 - GAP) {     //�������� �ε�����

            playerDamage();

            hideObject(koopaFire2);

       }

        else {
            setTimer(kFireMoveTimer2, 0.05f);
            startTimer(kFireMoveTimer2);
        }

    }

    if (timer == kBallMoveTimer) {

        kBallX -= KOOPA_FIRE_SPEED + 5;

        if (kBallisRising == false)
            kBallY -= KOOPA_FIRE_SPEED - 5;

        else
            kBallY += KOOPA_FIRE_SPEED - 5;

        if (kBallY <= playerBottom)         //�ٴڿ� ������ ���� ƨ���
            kBallisRising = true;

        locateObject(koopaFire1Ball, scene_g5, kBallX, kBallY);

        if (kBallX < 10 || kBallY > 600)        //ȭ�� ������ ������ �����
            hideObject(koopaFire1Ball);

        else if (kBallX >= playerX_g5 - kBallSize + GAP && kBallX <= playerX_g5 + playerSizeX_g5 - GAP &&
                 kBallY >= playerY_g5 - kBallSize + GAP && kBallY <= playerY_g5 + playerSizeY_g5 - GAP) {     //�������� �ε�����

            playerDamage();

            hideObject(koopaFire1Ball);
        }

        else {
            setTimer(kBallMoveTimer, 0.05f);
            startTimer(kBallMoveTimer);
        }
    }

    if (timer == koopaJumpTimer) {
        koopaJump();
    }

    if (timer == koopaJumpCoolTimer) {
        koopaisRising = true;
        koopaisLanded = false;
        setTimer(koopaJumpTimer, 0.05f);
        startTimer(koopaJumpTimer);
        setTimer(koopaJumpCoolTimer, 4.0f);
        startTimer(koopaJumpCoolTimer);
    }

    if (timer == kFireCoolTimer1) {
        koopaFire1Attack();
        setTimer(kFireCoolTimer1, KFIRE_COOLTIME);
        startTimer(kFireCoolTimer1);
    
    }

    if (timer == kFireCoolTimer2) {
        koopaFire2Attack();
        setTimer(kFireCoolTimer2, KFIRE_COOLTIME);
        startTimer(kFireCoolTimer2);

    }

    if (timer == kBallCoolTimer) {
        kBallisRising = false;
        koopaBallAttack();
        setTimer(kBallCoolTimer, KBALL_COOLTIME);
        startTimer(kBallCoolTimer);
    }
 
}

void Game5_keyboardCallback(KeyCode code, KeyState state) {

    if (nowGameSceneNum == 5) {

        if (code == 83) {		// RIGHT
            dx_g5 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
        }
        else if (code == 82) {		// LEFT
            dx_g5 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
        }

        if (code == 84) {			// Jump 
            isJumping += (state == KeyState::KEYBOARD_PRESSED ? 1 : -1);
        }

        if (code == 75) {           // �� �߻� : �����̽���
            if (state == KeyState::KEYBOARD_PRESSED && gameStart == true) {

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
    }
}



void Game5_soundCallback(SoundID sound) {

    if (sound == bgm_g5)
        playSound(bgm_g5);
}


void Game5_main() {

	scene_g5 = createScene("BOSS STAGE ���� ��", "image/game5/���ļ����.png");

    startButton_g5 = createObject("image/game6/start.png", scene_g5, 530, 350, true, 1.0f);
    restartButton_g5 = createObject("image/game6/restart.png", scene_g5, 470, 350, false, 1.0f);
    goMapButton_g5 = createObject("image/game6/goMap.png", scene_g5, 20, 20, true, 1.0f);

    //ü�¹� : 500 x 40
    HPbar = createObject("image/game5/ü�¹�.png", scene_g5, 640, 640, true, 1.0f);
    HPbarBlank = createObject("image/game5/ü�¹����.png", scene_g5, 1140, 640, true, 1.0f);
    HPright = createObject("image/game5/ü�¹ٹ��.png", scene_g5, 1140, 640, true, 1.0f);
    HPbarLine = createObject("image/game5/ü�¹��׵θ�.png", scene_g5, 640, 640, true, 1.0f);

    playerFrame = createObject("image/game5/�������׸�.png", scene_g5, 240, 640, true, 1.3f);
    koopaFrame = createObject("image/game5/���ı׸�.png", scene_g5, 560, 640, true, 1.3f);

    for (int i = 0; i < 3; i++) {
        playerHP[i] = createObject("image/game5/��Ʈ.png", scene_g5, 320 + 55 * i, 640, true, 1.0f);
    }

	player_g5 = createObject("image/game5/������/������ �ִϸ��̼�1.png", scene_g5, playerX_g5, playerBottom, true, 1.0f);
	koopa = createObject("image/game5/����/���� �ִϸ��̼�1.png", scene_g5, koopaX, koopaY, true, 1.0f);

    koopaFire1 = createObject("image/game5/���ĺ�.png", scene_g5, 1, 1, false, 1.0f);
    koopaFire2 = createObject("image/game5/���ĺ�.png", scene_g5, 1, 1, false, 1.0f);
    koopaFire1Ball = createObject("image/game5/���ĺҰ�.png", scene_g5, 1, 1, false, 1.0f);

    char path_g5[256];

    for (int i = 0; i < 4; i++) {
        sprintf_s(path_g5, "image/game5/��%d.png", i + 1);
        fire1[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
        fire2[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
    }

    for (int i = 0; i < 7; i++) {
        koopaAnimationImage[i] = setKoopaAnimationImage[i];
    }


    playTimer_g5 = createTimer(PLAYER_ANIMATION_TIME);
    jumpTimer_g5 = createTimer(0.05f);
    fire1MoveTimer = createTimer(0.05f);
    fire2MoveTimer = createTimer(0.05f);
    koopaDamageTimer = createTimer(0.05f);

    koopaJumpTimer = createTimer(0.05f);
    kFireMoveTimer1 = createTimer(0.05f);
    kFireMoveTimer2 = createTimer(0.05f);
    kBallMoveTimer = createTimer(0.05f);

    koopaJumpCoolTimer = createTimer(4.0f);
    kFireCoolTimer1 = createTimer(2.0f);
    kFireCoolTimer2 = createTimer(2.0f + KFIRE_COOLTIME / 2);
    kBallCoolTimer = createTimer(3.0f);

    playerAnimationTimer_g5 = createTimer(0.05f);
    koopaAnimationTimer = createTimer(0.2f);

    bgm_g5 = createSound("sounds/�����/���ļ�.mp3");

    jumpSound_g5 = createSound("sounds/����/����.mp3");
    jumpSound2_g5 = createSound("sounds/����/����2.mp3");

    playerFireSound1 = createSound("sounds/game5/�������Ұ���1.wav");
    playerFireSound2 = createSound("sounds/game5/�������Ұ���2.wav");
    koopaFireSound1 = createSound("sounds/game5/���ĺҰ���1.mp3");
    koopaFireSound2 = createSound("sounds/game5/���ĺҰ���2.mp3");
    koopaFireSound3 = createSound("sounds/game5/���ĺҰ���3.mp3");

    koopaDamageSound1 = createSound("sounds/game5/���ĵ�����1.mp3");
    koopaDamageSound2 = createSound("sounds/game5/���ĵ�����2.mp3");
    playerDamageSound = createSound("sounds/game5/������������.mp3");
}