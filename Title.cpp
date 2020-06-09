#include<bangtal.h>
#include <stdio.h>

SceneID titleScene;
ObjectID GameButton[6];

char path[256];

extern SceneID scene_g1, scene1_g2, scene_g4, scene_g5, scene_g6;

extern SoundID bgm_g6;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if (object == GameButton[0])
		enterScene(scene_g1);

	else if (object == GameButton[1])
		enterScene(scene1_g2);

	else if (object == GameButton[3])
		enterScene(scene_g4);

	else if (object == GameButton[4]) {
		enterScene(scene_g5);
	}

	else if (object == GameButton[5]) {
		enterScene(scene_g6);
		playSound(bgm_g6);
	}

}


void Title_main() {

	titleScene = createScene("타이틀(임시)", "image/Title/map ui.png");
	GameButton[0] = createObject("image/Title/game1.png", titleScene, 78, 80, true, 1.f);
	GameButton[1] = createObject("image/Title/game2.png", titleScene, 215, 405, true, 1.f);
	GameButton[2] = createObject("image/Title/game3.png", titleScene, 560, 155, true, 1.3f);
	GameButton[3] = createObject("image/Title/game4.png", titleScene, 810, 380, true, 1.3f);
	GameButton[4] = createObject("image/Title/game5.png", titleScene, 1130, 250, true, 1.3f);
	GameButton[5] = createObject("image/Title/game6.png", titleScene, 1070, 450, true, 1.3f);
}