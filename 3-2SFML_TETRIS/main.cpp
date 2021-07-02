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

		//������ �̺�Ʈ ó�� ��ƾ
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

			//�����ư�� ���� ���
			if (e.type == Event::Closed) {
				g_Tetris->Game_Window.close();
				return 0;
			}
		}

		//����Ű �Ʒ��� ������ �����̸� ���ҽ��� �������� �ӵ� ����
		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05f;

		//�� �̵��ϱ�
		for (int Index = 0; Index < 4; Index++) {
			// ���� ��Ʈ���̳� ������ ����صδ� �κ�
			g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
			// ���� ��Ʈ���̳� ��ġ ����
			g_Tetris->getTetrimino_now(Index).addX(dx);
		}

		// �Ű��� ��Ʈ���̳� ��ġ�� ���������� �ʴٸ�
		if (COL_TYPE::OK != g_Tetris->collision_check()) {
			for (int Index = 0; Index < 4; Index++)
				g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
				// ��Ʈ���̳� ������ ���� ��Ʈ���̳� ������ ����ϱ�.
		}

		// ��Ʈ���̳� ȸ�� ��Ʈ
		if (rotate)
		  {
				// ��Ʈ���̳� ȸ���� �߽��� 1��° ��
				block p = g_Tetris->getTetrimino_now(1);
				for (int Index = 0 ; Index < 4; Index++) {
					block t_a = g_Tetris->getTetrimino_now(Index);
					int x = t_a.getY() - p.getY();
					int y = t_a.getX() - p.getX();

					g_Tetris->getTetrimino_now(Index).setX(p.getX() - x);
					g_Tetris->getTetrimino_now(Index).setY(p.getY() + y);
	 			  }

				// ȸ���� ��Ʈ���̳��� ��ġ�� ���������� �ʴٸ�
				if (COL_TYPE::OK != g_Tetris->collision_check()) {
					for (int Index = 0; Index < 4; Index++) {
						//���� ��Ʈ���̳� ������ ���� ��Ʈ���̳� ������ ����ϱ�.
						g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
					}
				}
		  }

		if (drop == true) {
			while (true) {

				for (int Index = 0; Index < 4; Index++) {
					// ���� ��Ʈ���̳� ������ ����صδ� �κ�
					g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
				}
				for (int Index = 0; Index < 4; Index++) {
					// ���� ��Ʈ���̳� ��ġ ����
					g_Tetris->getTetrimino_now(Index).addY(1);
				}

				// �Ű��� ��Ʈ���̳��� ��ġ�� ���������� �ʴٸ�
				if (g_Tetris->collision_check() != COL_TYPE::OK) {
					for (int Index = 0; Index < 4; Index++)
						g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_backup(Index);
					break;
				}
			}
			drop = false;
		}


		//������ ���� �ð��� ������ ��Ʈ���̳븦 �ڵ����� 1ĭ�� �ٿ�
		if (dwTime > delay){

			//���� ��Ʈ���̳� 1ĭ ������ �κ�
			for (int Index = 0; Index < 4; Index++) { 
				// ���� ��Ʈ���̳� ������ ���
				g_Tetris->getTetrimino_backup(Index) = g_Tetris->getTetrimino_now(Index);
				// ���� ��Ʈ���̳� 1ĭ ������
				g_Tetris->getTetrimino_now(Index).addY(1);
			}

			// �Ű��� ��Ʈ���̳��� ��ġ�� ���������� �ʴٸ�
			if (COL_TYPE::OK != g_Tetris->collision_check()){

				//�� ������ ���� �� ������ ��ü
				for (int Index = 0; Index < 4; Index++) {
					block b_Tetrimino = g_Tetris->getTetrimino_backup(Index);
					g_Tetris->field[b_Tetrimino.getY()][b_Tetrimino.getX()] = b_Tetrimino.get_colorNum();
				}

				//������ ���;� �ϴ� ���� ���� ������� �̸� ����
				for (int Index = 0; Index < 4; Index++) {
					g_Tetris->getTetrimino_now(Index) = g_Tetris->getTetrimino_next(Index);
				}

				//������ ���� ��Ʈ���̳� ���� ����
				TETRIMINO_INFO::TYPE nextType = (TETRIMINO_INFO::TYPE)(rand() % 7);
				int nextColor = 1 + (rand() % 7);

				//���� ��Ʈ���̳� ���� ����
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

		//���� ���� ������ ����
		dx = 0; rotate = false; delay = 0.3f;

		//���� ������ �ִ��� �Ǻ� ��, ���� �� ����
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

		//���ȭ�� �Ͼ������ �ʱ�ȭ
		g_Tetris->Game_Window.clear(Color::White);	
		//���ȭ�� ��ġ ����
		g_Tetris->get_BackGroundSprite().setPosition(28,31);
		//���ȭ�� �׸���
		g_Tetris->Game_Window.draw(g_Tetris->get_BackGroundSprite());

		//���� ������ ���۱ǿ� ���� �޼����� ����ϱ� ���� Text ��ü ���� ����
		g_Tetris->get_Text().setColor(Color::Color(48, 142, 40));
		g_Tetris->get_Text().setCharacterSize(17);
		g_Tetris->get_Text().setPosition(25, 400);
		g_Tetris->get_Text().setString(L"���� ���� : " + to_string(g_Tetris->get_score()) + "\n" + L"Copyright 2018 ������, ������" + "\n" + "All rights reserved.");
		//Text ��ü �׸���
		g_Tetris->Game_Window.draw(g_Tetris->get_Text());


		//������ ���� ��Ʈ���̳븦 ǥ�����ִ� �г��� �� ��� ��ġ ����
		g_Tetris->get_PanelSprite().setPosition(220, 34);
		//�г��� �� ��� ��ġ ����
		g_Tetris->Game_Window.draw(g_Tetris->get_PanelSprite());



		//������ ����Ǵ� ����
		if (COL_TYPE::OK != g_Tetris->collision_check()) {
			//������ �����ٴ� �޼����� ����ϱ� ���� Text ��ü ���� ����
			g_Tetris->get_Text().setPosition(55, 200);
			g_Tetris->get_Text().setColor(Color::Red);
			g_Tetris->get_Text().setCharacterSize(20);
			g_Tetris->get_Text().setString("THE END OF \n GAME");

			//Text ��ü �׸���
			g_Tetris->Game_Window.draw(g_Tetris->get_Text());
			//ȭ�鿡 �׸���
			g_Tetris->Game_Window.display();

			g_Tetris->get_state() = GAME_INFO::GAME_STATE::END;
			g_Tetris->sound_stop();
			continue;
		}

		//������ ���� ��Ʈ���̳� �׸���
		for (int Index = 0; Index < 4; Index++) {
			//��Ʈ���̳��� ���� �� ���� �°� �ؽ��� ����
			g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->getTetrimino_next(Index).get_colorNum() * 18, 0, 18, 18));
			//��Ʈ���̳��� ���� �� ��ġ ����
			g_Tetris->get_TileSprite().setPosition((float)g_Tetris->getTetrimino_next(Index).getX() * 18, (float)g_Tetris->getTetrimino_next(Index).getY() * 18);
			//��Ʈ���̳� ��ü���� ��ġ ����
			g_Tetris->get_TileSprite().move(210, 45);
			//��Ʈ���̳� �׸���
			g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
		}


		//�ʿ� ���� ��Ʈ���̳� �׸���
		for (int hIndex = 0; hIndex < GAME_INFO::HEIGHT; hIndex++) {
			for (int wIndex = 0; wIndex < GAME_INFO::WIDTH; wIndex++){
				if (g_Tetris->field[hIndex][wIndex] == 0) continue; //�� ���̸� �׸��� �ʰ� �ѱ��
				//�� ���� ������ �´� �� Texture ����
				g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->field[hIndex][wIndex] * 18, 0, 18, 18));
				//�� ���� ������ �´� ���� ��ġ ����
				g_Tetris->get_TileSprite().setPosition((float)wIndex * 18, (float)hIndex * 18);
				//�� ���� ������ �´� ���� ��ġ ����
				g_Tetris->get_TileSprite().move(28, 31); //offset
				//�� ���� ������ �´� �� �׸���
				g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
			}
		}


		//���� ��Ʈ���̳� �׸���
		for (int Index = 0; Index < 4; Index++) {
			//��Ʈ���̳��� ���� �� ���� �°� Texture ����
			g_Tetris->get_TileSprite().setTextureRect(IntRect(g_Tetris->getTetrimino_now(Index).get_colorNum() * 18, 0, 18, 18));
			//��Ʈ���̳��� ���� �� ��ġ ����
			g_Tetris->get_TileSprite().setPosition((float)g_Tetris->getTetrimino_now(Index).getX() * 18, (float)g_Tetris->getTetrimino_now(Index).getY() * 18);
			//��Ʈ���̳��� ���� �� ��ġ ����
			g_Tetris->get_TileSprite().move(28, 31);
			//��Ʈ���̳� �׸���
			g_Tetris->Game_Window.draw(g_Tetris->get_TileSprite());
		}
		
		//��� Texture �׸���
		g_Tetris->Game_Window.display();
	}

	while (true) {
		if (GetAsyncKeyState('R') & 0x8000) {
			goto start;
		}
	}
    return 0;
}