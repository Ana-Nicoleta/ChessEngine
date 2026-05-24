#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
Board::Board() {
    for (int col = 0; col < 8; col++) {
        board[1][col] = std::make_unique<Pawn>(Color::white, Position{1, col});
        board[6][col] = std::make_unique<Pawn>(Color::black, Position{6, col});
    }

    board[0][0] = std::make_unique<Rook>(Color::white, Position{0, 0});
    board[0][1] = std::make_unique<Knight>(Color::white, Position{0, 1});
    board[0][2] = std::make_unique<Bishop>(Color::white, Position{0, 2});
    board[0][3] = std::make_unique<Queen>(Color::white, Position{0, 3});
    board[0][4] = std::make_unique<King>(Color::white, Position{0, 4});
    board[0][5] = std::make_unique<Bishop>(Color::white, Position{0, 5});
    board[0][6] = std::make_unique<Knight>(Color::white, Position{0, 6});
    board[0][7] = std::make_unique<Rook>(Color::white, Position{0, 7});

    board[7][0] = std::make_unique<Rook>(Color::black, Position{7, 0});
    board[7][1] = std::make_unique<Knight>(Color::black, Position{7, 1});
    board[7][2] = std::make_unique<Bishop>(Color::black, Position{7, 2});
    board[7][3] = std::make_unique<Queen>(Color::black, Position{7, 3});
    board[7][4] = std::make_unique<King>(Color::black, Position{7, 4});
    board[7][5] = std::make_unique<Bishop>(Color::black, Position{7, 5});
    board[7][6] = std::make_unique<Knight>(Color::black, Position{7, 6});
    board[7][7] = std::make_unique<Rook>(Color::black, Position{7, 7});
}
Piece* Board:: getPieceAt(Position p) const{
    if (p.row >= 0 && p.row < 8 && p.column >= 0 && p.column < 8) {
        return board[p.row][p.column].get();
    }
    return nullptr;
}
void Board::movePiece(Position start, Position end) {
    if (this->isLegalMove(start, end)) {

        bool isFirstMove = !board[start.row][start.column]->getStatus();
        std::unique_ptr<Piece> captured = nullptr;

        if (board[end.row][end.column] != nullptr &&
            board[end.row][end.column]->getColor() != board[start.row][start.column]->getColor()) {
            captured = std::move(board[end.row][end.column]);
            }

        MoveCommand command(start, end, isFirstMove, std::move(captured));

        board[end.row][end.column] = std::move(board[start.row][start.column]);
        board[end.row][end.column]->setPosition(end);
        board[end.row][end.column]->setStatus(true);

        if (board[end.row][end.column]->getType() == "King") {
            if (board[end.row][end.column]->getColor() == Color::white) {
                whiteKingPos = end;
            } else {
                blackKingPos = end;
            }
        }

        if (board[end.row][end.column]->getType() == "King" && abs(end.column - start.column) == 2) {
            int direction = (end.column > start.column) ? 1 : -1;
            int rookStartCol = (direction == 1) ? 7 : 0;
            int rookEndCol = (direction == 1) ? end.column - 1 : end.column + 1;

            board[end.row][rookEndCol] = std::move(board[end.row][rookStartCol]);
            board[end.row][rookEndCol]->setPosition({end.row, rookEndCol});
            board[end.row][rookEndCol]->setStatus(true);
        }
        moveHistory.push(std::move(command));
    }
}
bool Board::isSquareAttacked(Position target, Color attackingColor) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = this->getPieceAt({row, col});
            if (piece != nullptr && piece->getColor() == attackingColor) {
                if (piece->getType() == "King") {
                    if (abs(row - target.row) <= 1 && abs(col - target.column) <= 1) {
                        return true;
                    }
                    continue;
                }
                if (piece->isValidMove(target, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::isLegalMove(Position start, Position target) {
    if (board[start.row][start.column] == nullptr) {
        return false;
    }

    if (!board[start.row][start.column]->isValidMove(target, *this)) {
        return false;
    }

    Color playerColor = board[start.row][start.column]->getColor();
    Color enemyColor = (playerColor == Color::white) ? Color::black : Color::white;
    Position originalPosition = start;

    std::unique_ptr<Piece> capturedPiece = std::move(board[target.row][target.column]);
    board[target.row][target.column] = std::move(board[start.row][start.column]);
    board[target.row][target.column]->setPosition(target);

    Position kingPos = (playerColor == Color::white) ? whiteKingPos : blackKingPos;
    if (board[target.row][target.column]->getType() == "King") {
        kingPos = target;
    }
    bool isKingSafe = !this->isSquareAttacked(kingPos, enemyColor);

    if (board[target.row][target.column] != nullptr &&
        board[target.row][target.column]->getType() == "King" &&
        abs(target.column - originalPosition.column) == 2) {

        int direction = (target.column > originalPosition.column) ? 1 : -1;
        Position crossedSquare = {originalPosition.row, originalPosition.column + direction};

        if (this->isSquareAttacked(originalPosition, enemyColor) ||
            this->isSquareAttacked(crossedSquare, enemyColor)) {
            isKingSafe = false;
            }
        }

    board[start.row][start.column] = std::move(board[target.row][target.column]);
    board[start.row][start.column]->setPosition(originalPosition);
    board[target.row][target.column] = std::move(capturedPiece);

    return isKingSafe;
}
bool Board::findCheck(Color playerColor) {
    int startIdx = (playerColor == Color::white) ? 0 : 7;
    int endIdx = (playerColor == Color::white) ? 8 : -1;
    int step = (playerColor == Color::white) ? 1 : -1;

    for (int startRow = startIdx; startRow != endIdx; startRow += step) {
        for (int startCol = 0; startCol < 8; ++startCol) {
            Piece* piece = this->getPieceAt({startRow, startCol});

            if (piece != nullptr && piece->getColor() == playerColor) {
                for (int targetRow = 0; targetRow < 8; ++targetRow) {
                    for (int targetCol = 0; targetCol < 8; ++targetCol) {
                        Piece* targetPiece = this->getPieceAt({targetRow, targetCol});
                        if (targetPiece != nullptr && targetPiece->getColor() == playerColor) {
                            continue;
                        }
                        if (this->isLegalMove({startRow, startCol}, {targetRow, targetCol})) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
void Board::promoteToQueen(Position p) {
    Color c = board[p.row][p.column]->getColor();
    board[p.row][p.column] = std::make_unique<Queen>(c, p);
}
const MoveCommand* Board::getLastMove() const {
    if (moveHistory.isEmpty()) {
        return nullptr;
    }
    return &moveHistory.peek();
}

const FixedStack<MoveCommand, 500>& Board:: getMoveHistory() const {
    return moveHistory;
}
Position Board::getKingPosition(Color playerColor) const {
    return (playerColor == Color::white) ? whiteKingPos : blackKingPos;
}
void Board::undoLastMove() {
    if (moveHistory.isEmpty()) {
        return;
    }
    MoveCommand lastCommand = moveHistory.pop();

    lastCommand.undo(*this);
}