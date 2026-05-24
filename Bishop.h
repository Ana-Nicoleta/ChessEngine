#pragma once
#include <iostream>
#include "Board.h"
using namespace std;
class Bishop: public Piece {
public:
    Bishop(Color c, Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;
};