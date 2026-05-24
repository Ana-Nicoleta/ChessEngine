#include "Rook.h"
Rook::Rook(Color c, Position p): Piece(c,p){};
bool Rook:: isValidMove(Position target, const Board& b) const  {
    if (target.row < 0 || target.row >= 8 || target.column < 0 || target.column >= 8) return false;

    int rowDiff = target.row - this->getPosition().row;
    int colDiff = target.column - this->getPosition().column;

    if (rowDiff != 0 && colDiff != 0) {
        return false;
    }

    if (rowDiff != 0) {
        int step = (rowDiff > 0) ? 1 : -1;
        for (int r = this->getPosition().row + step; r != target.row; r += step) {
            if (b.getPieceAt({r, this->getPosition().column}) != nullptr) {
                return false;
            }
        }
    }
    else {
        int step = (colDiff > 0) ? 1 : -1;
        for (int c =this->getPosition().column + step; c != target.column; c += step) {
            if (b.getPieceAt({this->getPosition().row, c}) != nullptr) {
                return false;
            }
        }
    }

    const Piece* targetPiece = b.getPieceAt(target);
    if (targetPiece != nullptr && targetPiece->getColor() == this->getColor()) {
        return false;
    }
    return true;
}
string Rook::getType() const {
    return "Rook";
}
