#include "clsBlock.h"

block::block(int px, int py) : colorNum(0) {

}
void block::addX(int x) {
	this->x += x;
}

void block::addY(int y) {
	this->y += y;
}

int block::getX() const {
	return x;
}
int block::getY() const {
	return y;
}

void block::setX(int px) {
	x = px;
}
void block::setY(int py) {
	y = py;
}

int& block::get_colorNum() {
	return colorNum;
}
TETRIMINO_INFO::TYPE& block::get_type() {
	return nType;
}