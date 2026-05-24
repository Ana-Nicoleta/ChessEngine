#include "Piece.h"
Piece::Piece(Color color,Position position):color(color),position(position), hasMoved(false){};
Color Piece:: getColor()const {
    return color;
};
Position Piece:: getPosition() const{ return position; };
bool Piece:: getStatus() const { return hasMoved; };
void Piece:: setPosition(Position p) {
    this->position=p;
};
void Piece:: setStatus(bool s) {
    this->hasMoved=s;
};

