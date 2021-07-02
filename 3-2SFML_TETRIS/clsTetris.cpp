#include "clsTetris.h"


Tetris::Tetris() : g_score(0) {

		font.loadFromFile("font/ClearGodic.ttf");
		text.setFont(font);
		text.setStyle(Text::Bold);

		Tile.loadFromFile("images/tiles.png");
		BackGround.loadFromFile("images/background.png");
		BlockPanel.loadFromFile("images/blockpanel.png");

		TileSprite.setTexture(Tile);
		bSprite.setTexture(BackGround);
		pSprite.setTexture(BlockPanel);

		mPlayer.openFromFile("sound/Tetris.wav");
		Game_Window.create(VideoMode(320, 480), "Team Project Tetris Game!");
}

COL_TYPE Tetris::collision_check() {
	for (int Index = 0; Index < 4; Index++) {
		//맵에서 이탈한 경우
		if (now_Tetrimino[Index].getX() < 0 || now_Tetrimino[Index].getX() >= GAME_INFO::WIDTH || now_Tetrimino[Index].getY() >= GAME_INFO::HEIGHT)
			return COL_TYPE::RANGE_OUT;
		else if (field[now_Tetrimino[Index].getY()][now_Tetrimino[Index].getX()])
			return COL_TYPE::NOT_SPACE;
	}
	//그렇지 않은 경우
	return COL_TYPE::OK;
}

void Tetris::Init() {
	g_score = 0;
	g_state = GAME_INFO::GAME_STATE::PLAY;
	Game_Window.clear(Color::White);
}

void Tetris::InitMap() {
	for (int hIndex = 0; hIndex < GAME_INFO::HEIGHT; hIndex++)
		for (int wIndex = 0; wIndex < GAME_INFO::WIDTH; wIndex++)
			field[hIndex][wIndex] = 0;
}

void Tetris::InitTetrimino() {

	TETRIMINO_INFO::TYPE nowType = (TETRIMINO_INFO::TYPE)(rand() % 7);
	TETRIMINO_INFO::TYPE nextType = (TETRIMINO_INFO::TYPE)(rand() % 7);

	int nowColor = 0, nextColor = 0;
	nowColor = 1 + (rand() % 7);
	nextColor = 1 + (rand() % 7);

	//현재 테트리미노 정보 셋팅
	for (int Index = 0; Index < 4; Index++) {
		now_Tetrimino[Index].setX(TETRIMINO_INFO::TETRIMINO[nowType][Index] % 2);
		now_Tetrimino[Index].setY(TETRIMINO_INFO::TETRIMINO[nowType][Index] / 2);
		now_Tetrimino[Index].addX(2);
		now_Tetrimino[Index].get_colorNum() = nowColor;
		now_Tetrimino[Index].get_type() = nowType;
	}

	//다음 테트리미노 정보 셋팅
	for (int Index = 0; Index < 4; Index++) {
		next_Tetrimino[Index].setX(TETRIMINO_INFO::TETRIMINO[nextType][Index] % 2);
		next_Tetrimino[Index].setY(TETRIMINO_INFO::TETRIMINO[nextType][Index] / 2);
		next_Tetrimino[Index].addX(2);
		next_Tetrimino[Index].get_colorNum() = nextColor;
		next_Tetrimino[Index].get_type() = nextType;
	}
}



block& Tetris::getTetrimino_now(int Index) {
	return now_Tetrimino[Index];
}
block& Tetris::getTetrimino_backup(int Index) {
	return backup_Tetrimino[Index];
}
block& Tetris::getTetrimino_next(int Index) {
	return next_Tetrimino[Index];
}

Sprite& Tetris::get_TileSprite() {
	return TileSprite;
}
Sprite& Tetris::get_BackGroundSprite() {
	return bSprite;
}

Sprite& Tetris::get_PanelSprite() {
	return pSprite;
}
Text& Tetris::get_Text() {
	return text;
}

int& Tetris::get_score() {
	return g_score;
}
GAME_INFO::GAME_STATE& Tetris::get_state() {
	return g_state;
}
void Tetris::sound_play() {
	mPlayer.setLoop(true);
	mPlayer.play();
}
void Tetris::sound_stop() {
	mPlayer.stop();
}