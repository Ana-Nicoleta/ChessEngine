#include "Knight.h"
Knight::Knight(Color c,Position p):Piece(c,p){};
bool Knight:: isValidMove(Position target, const Board& b) const {
    if (target.row < 0 || target.row >= 8 || target.column < 0 || target.column >= 8) return false;
    int rowDiff = std::abs(target.row - getPosition().row);
    int colDiff = std::abs(target.column - getPosition().column);

    bool isLShape = (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    if (!isLShape) return false;

    const Piece* pieceOnTarget= b.getPieceAt(target);
    if (pieceOnTarget!= nullptr && pieceOnTarget->getColor()==this->getColor()) return false;
    return true;
};
string Knight::getType() const {
    return "Knight";
}