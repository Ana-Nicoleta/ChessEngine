#include "MoveCommand.h"
#include "Board.h"
#include <cmath>
MoveCommand::MoveCommand(Position startPos, Position endPos, bool firstMove, std::unique_ptr<Piece> captured)
    : start(startPos),
      end(endPos),
      wasFirstMove(firstMove),
      capturedPiece(std::move(captured))
{}
void MoveCommand::undo(Board& board) {
    board.board[start.row][start.column] = std::move(board.board[end.row][end.column]);
    board.board[start.row][start.column]->setPosition(start);

    if (wasFirstMove) {
        board.board[start.row][start.column]->setStatus(false);
    }

    if (capturedPiece != nullptr) {
        board.board[end.row][end.column] = std::move(capturedPiece);
    }

    if (board.board[start.row][start.column]->getType() == "King") {
        if (board.board[start.row][start.column]->getColor() == Color::white) {
            board.whiteKingPos = start;
        } else {
            board.blackKingPos = start;
        }

        if (std::abs(end.column - start.column) == 2) {
            int direction = (end.column > start.column) ? 1 : -1;
            int rookStartCol = (direction == 1) ? 7 : 0;
            int rookEndCol = (direction == 1) ? end.column - 1 : end.column + 1;

            board.board[start.row][rookStartCol] = std::move(board.board[end.row][rookEndCol]);
            board.board[start.row][rookStartCol]->setPosition({start.row, rookStartCol});
            board.board[start.row][rookStartCol]->setStatus(false);
        }
    }
}
Position MoveCommand:: getStart() const {
    return start;
};
Position MoveCommand:: getEnd() const {
    return end;
};
void MoveCommand::setWasFirstMove(bool b) {
    this->wasFirstMove=b;
}

