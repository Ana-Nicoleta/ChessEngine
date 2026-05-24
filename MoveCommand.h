#pragma once
#include "Piece.h"
#include <memory>

class Board;

class MoveCommand {
private:
    Position start;
    Position end;
    std::unique_ptr<Piece> capturedPiece;
    bool wasFirstMove;

public:
    MoveCommand() = default;
    MoveCommand(Position startPos, Position endPos, bool firstMove, std::unique_ptr<Piece> captured = nullptr);
    void undo(Board& board);
    Position getStart() const;
    Position getEnd() const;
    void setWasFirstMove(bool b);
};