#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "clsGameInfo.h"
#include "clsBlock.h"

using namespace sf;

enum COL_TYPE { OK, RANGE_OUT, NOT_SPACE };

class Tetris {

private:
	Texture Tile, BackGround, BlockPanel;
	Sprite TileSprite, bSprite, pSprite;
	Font font;
	Text text;
	Music mPlayer;

private:
	block now_Tetrimino[4], backup_Tetrimino[4], next_Tetrimino[4];
	int g_score;
	GAME_INFO::GAME_STATE g_state;

public:
	RenderWindow Game_Window;
	int field[GAME_INFO::HEIGHT][GAME_INFO::WIDTH];

public:
	Tetris();

	COL_TYPE collision_check();

	void Init();
	void InitTetrimino();
	void InitMap();

	block& getTetrimino_now(int Index);
	block& getTetrimino_backup(int Index);
	block& getTetrimino_next(int Index);

	Sprite& get_TileSprite();
	Sprite& get_BackGroundSprite();
	Sprite& get_PanelSprite();
	Text& get_Text();

	int& get_score();
	GAME_INFO::GAME_STATE& get_state();
	void sound_play();
	void sound_stop();
};
