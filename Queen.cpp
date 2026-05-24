#include "Queen.h"
Queen::Queen(Color c, Position p): Piece(c,p){};
bool Queen::isValidMove(Position target, const Board& b) const {
    int rowDiff = target.row - this->getPosition().row;
    int colDiff = target.column - this->getPosition().column;

    if ((rowDiff != 0 && colDiff != 0 && abs(rowDiff) != abs(colDiff)) ||
        (rowDiff == 0 && colDiff == 0)) {
        return false;
        }

    int rowStep = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
    int colStep = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

    int currentRow = this->getPosition().row + rowStep;
    int currentCol = this->getPosition().column + colStep;


    while (currentRow != target.row || currentCol != target.column) {
        if (b.getPieceAt({currentRow, currentCol}) != nullptr) {
            return false;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }

    const Piece* pieceAtTarget = b.getPieceAt(target);
    if (pieceAtTarget != nullptr && pieceAtTarget->getColor() == this->getColor()) {
        return false;
    }

    return true;
}
string Queen::getType() const {
    return "Queen";
}