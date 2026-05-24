#pragma once
#include <iostream>
#include <memory>
#include "Piece.h"
#include "FixedStack.h"
#include "MoveCommand.h"
using namespace std;

class Board {
    unique_ptr<Piece> board[8][8];
    FixedStack<MoveCommand, 500> moveHistory;
    Position whiteKingPos = {0, 4};
    Position blackKingPos = {7, 4};
    Board();
public:
    Board(const Board&)            = delete;
    Board& operator=(const Board&) = delete;

    static Board& getInstance() {
        static Board instance;
        return instance;
    }
    friend class MoveCommand;
    Piece* getPieceAt(Position p) const;
    void movePiece(Position start, Position end);
    bool isSquareAttacked(Position target, Color attackingColor) const;
    bool isLegalMove(Position start, Position target);
    bool findCheck(Color playerColor);
    void promoteToQueen(Position p);
    const MoveCommand* getLastMove() const;
    const FixedStack<MoveCommand, 500>& getMoveHistory() const;
    Position getKingPosition(Color playerColor) const;
    void undoLastMove();
};