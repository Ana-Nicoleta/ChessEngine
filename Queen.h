#pragma once
#include <iostream>
#include "Board.h"
using namespace std;
class Queen: public Piece {
public:
    Queen(Color c, Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;
};