#pragma once
#include "clsTetriminoInfo.h"

class block {
	private:
		int x; 	// ���� x ��ġ
		int y; // ���� y ��ġ
		int colorNum; // ���� ����
		TETRIMINO_INFO::TYPE nType; // �ش� ������ ������ ��Ʈ���̳��� ����

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