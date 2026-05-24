#pragma once
#include <iostream>
#include "Board.h"
using namespace std;
class Rook: public Piece {
public:
    Rook(Color c, Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;
};