
#pragma once
#include <iostream>
#include "Board.h"
class King: public Piece {
public:
    King(Color c, Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;
};