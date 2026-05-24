#include "King.h"
King::King(Color c, Position p): Piece(c,p){};
bool King::isValidMove(Position target, const Board& b) const {
    int rowDiffRaw = target.row - this->getPosition().row;
    int colDiffRaw = target.column - this->getPosition().column;

    int rowDiff = abs(rowDiffRaw);
    int colDiff = abs(colDiffRaw);

    if (rowDiff <= 1 && colDiff <= 1 && (rowDiff != 0 || colDiff != 0)) {
        const Piece* pieceAtTarget = b.getPieceAt(target);
        if (pieceAtTarget != nullptr && pieceAtTarget->getColor() == this->getColor()) {
            return false;
        }

        return true;
    }

    //castling
    if (rowDiff == 0 && colDiff == 2) {
        if (this->getStatus()) {
            return false;
        }
        int currentRow = this->getPosition().row;
        int currentCol = this->getPosition().column;
        int direction = (colDiffRaw > 0) ? 1 : -1;

        if (direction == 1) { // Kingside
            if (b.getPieceAt({currentRow, currentCol + 1}) != nullptr ||
                b.getPieceAt({currentRow, currentCol + 2}) != nullptr) {
                return false;
            }
        } else {
            if (b.getPieceAt({currentRow, currentCol - 1}) != nullptr ||
                b.getPieceAt({currentRow, currentCol - 2}) != nullptr ||
                b.getPieceAt({currentRow, currentCol - 3}) != nullptr) {
                return false;
            }
        }

        int rookCol = (direction == 1) ? currentCol + 3 : currentCol - 4;
        const Piece* rookPiece = b.getPieceAt({currentRow, rookCol});

        if (rookPiece == nullptr || rookPiece->getColor() != this->getColor() || rookPiece->getStatus()) {
            return false;
        }

        return true;
    }

    return false;
}

string King::getType() const {
    return "King";
}