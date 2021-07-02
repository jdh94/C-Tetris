#pragma once
namespace TETRIMINO_INFO {
	enum TYPE { I, Z, S, T, L, J, O }; // 블럭의 종류 7가지
	const int TETRIMINO[7][4] =
	{
		1,3,5,7, // I
		2,4,5,7, // Z
		3,5,4,6, // S
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
	};
}