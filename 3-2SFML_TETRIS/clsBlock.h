#pragma once
#include "clsTetriminoInfo.h"

class block {
	private:
		int x; 	// 블럭의 x 위치
		int y; // 블럭의 y 위치
		int colorNum; // 블럭의 색깔
		TETRIMINO_INFO::TYPE nType; // 해당 블럭으로 구성될 테트리미노의 종류

	public:
		block(int px = 0, int py = 0);

		void addX(int x);
		void addY(int y);

		int getX() const;
		int getY() const;

		void setX(int px);
		void setY(int py);

		int& get_colorNum();
		TETRIMINO_INFO::TYPE& get_type();
};