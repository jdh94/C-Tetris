#include <time.h>
#include "clsTetris.h"
#include <Windows.h>

using namespace std;
using namespace sf;


int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

    srand((unsigned int)time(NULL));	 

	Tetris *g_Tetris = new Tetris();
	bool rotate = false, drop = false;
	int dx = 0;
	float dwTime = 0, delay = 1.0f;

	Clock time_clock;
	Event e;

start:
	g_Tetris->Init();
	g_Tetris->InitMap();
	g_Tetris->InitTetrimino();
	g_Tetris->sound_play();

    while ((g_Tetris->Game_Window.isOpen()) && (g_Tetris->get_state() == GAME_INFO::GAME_STATE::PLAY))
    {
		float time = time_clock.getElapsedTime().asSeconds();
		time_clock.restart();
		dwTime += time;

		//윈도우 이벤트 처리 루틴
        while (g_Tetris->Game_Window.pollEvent(e))
        {
			if (e.type == Event::KeyPressed) {
				switch (e.key.code) {
					case Keyboard::Up:
						rotate = true;
						break;
					case Keyboard::Left:
						dx = -1;
						break;
					case Keyboard::Right:
						dx = +1;
						break;
					case Keyboard::Space:
						drop = true;
						break;
				}
			}

			//종료버튼을 누른 경우
			if (e.type == Event::Closed) {
				g_Tetris->Game_Window.close();
				return 0;
			}
		}

		//방향키 아래를 누르실 딜레이를 감소시켜 내려가는 속도 증가
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05f;

		//블럭 이동하기
		for (int Index = 0; Index < 4; Index++) {
			// 현재 테트리미노 정보를 백업해두는 부분
			g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
			// 현재 테트리미노 위치 변경
			g_Tetris->getTetrimino_now(Index).addX(dx);
		}

		// 옮겨진 테트리미노 위치가 정상적이지 않다면
		if (COL_TYPE::OK != g_Tetris->collision_check()) {
			for (int Index = 0; Index < 4; Index++)
				g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
				// 테트리미노 정보를 이전 테트리미노 정보로 백업하기.
		}

		// 테트리미노 회전 파트
		if (rotate)
		  {
				// 테트리미노 회전의 중심은 1번째 블럭
				block p = g_Tetris->getTetrimino_now(1);
				for (int Index = 0 ; Index < 4; Index++) {
					block t_a = g_Tetris->getTetrimino_now(Index);
					int x = t_a.getY() - p.getY();
					int y = t_a.getX() - p.getX();

					g_Tetris->getTetrimino_now(Index).setX(p.getX() - x);
					g_Tetris->getTetrimino_now(Index).setY(p.getY() + y);
	 			  }

				// 회전된 테트리미노의 위치가 정상적이지 않다면
				if (COL_TYPE::OK != g_Tetris->collision_check()) {
					for (int Index = 0; Index < 4; Index++) {
						//현재 테트리미노 정보를 이전 테트리미노 정보로 백업하기.
						g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
					}
				}
		  }

		if (drop == true) {
			while (true) {

				for (int Index = 0; Index < 4; Index++) {
					// 현재 테트리미노 정보를 백업해두는 부분
					g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
				}
				for (int Index = 0; Index < 4; Index++) {
					// 현재 테트리미노 위치 변경
					g_Tetris->getTetrimino_now(Index).addY(1);
				}

				// 옮겨질 테트리미노의 위치가 정상적이지 않다면
				if (g_Tetris->collision_check() != COL_TYPE::OK) {
					for (int Index = 0; Index < 4; Index++)
						g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
					break;
				}
			}
			drop = false;
		}


		//지정된 단위 시간이 지나면 테트리미노를 자동으로 1칸씩 다운
		if (dwTime > delay){

			//현재 테트리미노 1칸 내리는 부분
			for (int Index = 0; Index < 4; Index++) { 
				// 현재 테트리미노 정보를 백업
				g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
				// 현재 테트리미노 1칸 내리기
				g_Tetris->getTetrimino_now(Index).addY(1);
			}

			// 옮겨질 테트리미노의 위치가 정상적이지 않다면
			if (COL_TYPE::OK != g_Tetris->collision_check()){

				//맵 정보를 이전 맵 정보로 교체
				for (int Index = 0; Index < 4; Index++) {
					block b_Tetrimino = g_Tetris->getTetrimino_backup(Index);
					g_Tetris->field[b_Tetrimino.getY()][b_Tetrimino.getX()] = b_Tetrimino.get_colorNum();
				}

				//다음에 나와야 하는 블럭을 현재 블록으로 미리 설정
				for (int Index = 0; Index < 4; Index++) {
					g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_next(Index);
				}

				//다음에 나올 테트리미노 랜덤 추출
				TETRIMINO_INFO::TYPE nextType = (TETRIMINO_INFO::TYPE)(rand() % 7);
				int nextColor = 1 + (rand() % 7);

				//다음 테트리미노 정보 셋팅
				for (int Index = 0; Index < 4; Index++) {
					g_Tetris->getTetrimino_next(Index).setX(TETRIMINO_INFO::TETRIMINO[nextType][Index] % 2);
					g_Tetris->getTetrimino_next(Index).setY(TETRIMINO_INFO::TETRIMINO[nextType][Index] / 2 );
					g_Tetris->getTetrimino_next(Index).addX(2);
					g_Tetris->getTetrimino_next(Index).get_colorNum() = nextColor;
					g_Tetris->getTetrimino_next(Index).get_type() = nextType;
				}
			}

	  		dwTime = 0;
		 }

		//원래 게임 정보로 셋팅
		dx = 0; rotate = false; delay = 0.3f;

		//꽉찬 라인이 있는지 판별 후, 삭제 및 보정
		int t_Index = GAME_INFO::HEIGHT - 1;
		for (int hIndex = GAME_INFO::HEIGHT - 1; hIndex > 0; hIndex--)
		{
			int fill_count = 0;
			for (int wIndex = 0; wIndex < GAME_INFO::WIDTH ; wIndex++)
			{
				if (g_Tetris->field[hIndex][wIndex]) fill_count++;
				g_Tetris->field[t_Index][wIndex] = g_Tetris->field[hIndex][wIndex];
			}
			if (fill_count < GAME_INFO::WIDTH)
				t_Index--;
			else
				g_Tetris->get_score() += 100;
		}

		//배경화면 하얀색으로 초기화
		g_Tetris->Game_Window.clear(Color::White);	
		//배경화면 위치 보정
		g_Tetris->get_BackGroundSprite().setPosition(28,31);
		//배경화면 그리기
		g_Tetris->Game_Window.draw(g_Tetris->get_BackGroundSprite());

		//현재 점수와 저작권에 대한 메세지를 출력하기 위한 Text 객체 정보 설정
		g_Tetris->get_Text().setColor(Color::Color(48, 142, 40));
		g_Tetris->get_Text().setCharacterSize(17);
		g_Tetris->get_Text().setPosition(25, 400);
		g_Tetris->get_Text().setString(L"현재 점수 : " + to_string(g_Tetris->get_score()) + "\n" + L"Copyright 2018 원종운, 조동후" + "\n" + "All rights reserved.");
		//Text 객체 그리기
		g_Tetris->Game_Window.draw(g_Tetris->get_Text());


		//다음에 나올 테트리미노를 표시해주는 패널의 뒷 배경 위치 보정
		g_Tetris->get_PanelSprite().setPosition(220, 34);
		//패널의 뒷 배경 위치 보정
		g_Tetris->Game_Window.draw(g_Tetris->get_PanelSprite());



		//게임이 종료되는 시점
		if (COL_TYPE::OK != g_Tetris->collision_check()) {
			//게임이 끝났다는 메세지를 출력하기 위한 Text 객체 정보 설정
			g_Tetris->get_Text().setPosition(55, 200);
			g_Tetris->get_Text().setColor(Color::Red);
			g_Tetris->get_Text().setCharacterSize(20);
			g_Tetris->get_Text().setString("THE END OF \n GAME");

			//Text 객체 그리기
			g_Tetris->Game_Window.draw(g_Tetris->get_Text());
			//화면에 그리기
			g_Tetris->Game_Window.display();

			g_Tetris->get_state() = GAME_INFO::GAME_STATE::END;
			g_Tetris->sound_stop();
			continue;
		}

		//다음에 나올 테트리미노 그리기
		for (int Index = 0; Index < 4; Index++) {
			//테트리미노의 개별 블럭 색에 맞게 텍스쳐 설정
			g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->getTetrimino_next(Index).get_colorNum() * 18, 0, 18, 18));
			//테트리미노의 개별 블럭 위치 지정
			g_Tetris->get_TileSprite().setPosition((float)g_Tetris->getTetrimino_next(Index).getX() * 18, (float)g_Tetris->getTetrimino_next(Index).getY() * 18);
			//테트리미노 전체적인 위치 보정
			g_Tetris->get_TileSprite().move(210, 45);
			//테트리미노 그리기
			g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
		}


		//맵에 쌓인 테트리미노 그리기
		for (int hIndex = 0; hIndex < GAME_INFO::HEIGHT; hIndex++) {
			for (int wIndex = 0; wIndex < GAME_INFO::WIDTH; wIndex++){
				if (g_Tetris->field[hIndex][wIndex] == 0) continue; //빈 블럭이면 그리지 않고 넘기기
				//맵 개별 정보에 맞는 블럭 Texture 설정
				g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->field[hIndex][wIndex] * 18, 0, 18, 18));
				//맵 개별 정보에 맞는 블럭의 위치 지정
				g_Tetris->get_TileSprite().setPosition((float)wIndex * 18, (float)hIndex * 18);
				//맵 개별 정보에 맞는 블럭의 위치 보정
				g_Tetris->get_TileSprite().move(28, 31); //offset
				//맵 개별 정보에 맞는 블럭 그리기
				g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
			}
		}


		//현재 테트리미노 그리기
		for (int Index = 0; Index < 4; Index++) {
			//테트리미노의 개별 블럭 색에 맞게 Texture 설정
			g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->getTetrimino_now(Index).get_colorNum() * 18, 0, 18, 18));
			//테트리미노의 개별 블럭 위치 지정
			g_Tetris->get_TileSprite().setPosition((float)g_Tetris->getTetrimino_now(Index).getX() * 18, (float)g_Tetris->getTetrimino_now(Index).getY() * 18);
			//테트리미노의 개별 블럭 위치 보정
			g_Tetris->get_TileSprite().move(28, 31);
			//테트리미노 그리기
			g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
		}
		
		//모든 Texture 그리기
		g_Tetris->Game_Window.display();
	}

	while (true) {
		if (GetAsyncKeyState('R') & 0x8000) {
			goto start;
		}
	}
    return 0;
}