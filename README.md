코드가 복잡해질 것을 고려해 몇가지 공통 양식을 정해두면 좋을 것 같아 적어둡니다. (계속 추가해주세요!)

# main 함수이름: 소스파일이름_main()

ex) Title.cpp의 main 함수 = Title_main()


# CallBack 함수이름: 소스파일이름_~~CallBack()

ex) Title.cpp의 mouseCallBack 함수 = Title_mouseCallBack()

# Scene 이름, 특정 오브젝트 이름(button, player등)은 자주 겹치니 뒤에 소스파일이름을 간단히 붙여줍시다

ex) SceneID scene -> SceneID scene_g6

ex2) ObjectID startButton -> ObjectID startButton_title


# creatObject 오버로딩을 할 때는 main.cpp에 있는 함수를 가져다가 씁시다

여러 소스에서 creatObject 오버로딩을 하면 중복 정의로 오류가 발생하기 때문에 main에만 정의합시다

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

위 코드를 소스에 넣으면 바로 사용 가능합니다. (해당 함수는 6번째 인자로 size도 받고 있으니 1.0f라도 잊지 말고 넣어주세요)


# timerCallBack을 쓸 경우 꼭 본인의 Scene 안에서 작동과 정지가 이루어지게 해주세요

ex) Game6의 타이머지만 프로젝트 실행시 바로 타이머가 실행되게 만들 경우 타이틀 화면에 있어도 timer가 계속 돌아가고 있을 수도 있습니다..! (렉의 주범ㅠ)

-> 필요한 순간에만 타이머를 작동시킵시다

ex2) 0.05초 간격으로 플레이어를 이동시키는 playerTimer를 게임 클리어가 된 후에 정지시키지 않을 경우 다른 씬으로 이동해도 timer가 계속 플레이어를 이동시키고 있을 수도 있습니다..!

-> 현재 Scene을 나갈거면 현재 Scene의 모든 타이머를 정지시켜준 후에 탈출합시다
