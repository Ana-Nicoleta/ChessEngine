#pragma once
#include <iostream>
#include <memory>
#include "Board.h"
using namespace std;
class Pawn: public Piece {
public:
    Pawn(Color c, Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;
};