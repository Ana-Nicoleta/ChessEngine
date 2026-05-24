#pragma once
#include <iostream>
#include <memory>
#include "Board.h"

using namespace std;
class Knight: public Piece {
public:
    Knight(Color c,Position p);
    bool isValidMove(Position target, const Board& b) const override;
    string getType()const override;

};