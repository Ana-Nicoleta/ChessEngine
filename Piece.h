#pragma once
#include <iostream>
using namespace std;

enum class Color{ white, black};

struct Position {int row; int column;};
class Board;
class Piece {
    Color color;
    Position position;
    bool hasMoved;
public:
    Piece(Color color,Position position);
    virtual ~Piece() = default;
    virtual bool isValidMove(Position target, const Board& board) const = 0;
    Color getColor()const;
    Position getPosition() const;
    bool getStatus() const;
    virtual string getType()const = 0;

    void setPosition(Position p);
    void setStatus(bool s);
};