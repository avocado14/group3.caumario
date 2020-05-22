#include<bangtal.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>



SceneID game1b1;
ObjectID c1;
TimerID timer1;
SoundID theme;





ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return(object);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == c1) {

	}
}
void timerCallback(TimerID timer) {
	if (timer == timer1) {

	}
}
void soundCallback(SoundID sound) {
	if (sound == theme) {

	}

}

int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);
	game1b1 = createScene("타이틀화면", "image/title.png");

	c1 = createObject("image/배경 수정.png", game1b1, -520, -1030, true);
	timer1 = createTimer(0.01f);

	startGame(game1b1);
}