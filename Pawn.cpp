#include "Pawn.h"
Pawn::Pawn(Color c, Position p): Piece(c,p){};
bool Pawn:: isValidMove(Position target, const Board& b) const  {
    int direction = (this->getColor() == Color::white) ? 1 : -1;

    int rowDiff = target.row - this->getPosition().row;
    int colDiff = abs(target.column - this->getPosition().column);

    const Piece* pieceAtTarget= b.getPieceAt(target);

    if (colDiff == 0) {
        if (rowDiff == direction) {
            return pieceAtTarget == nullptr;
        }

        if (!this->getStatus() && rowDiff == 2 * direction) {
            Position intermediateSquare = {this->getPosition().row + direction, this->getPosition().column};

            bool intermediateEmpty = (b.getPieceAt(intermediateSquare) == nullptr);
            bool targetEmpty = (pieceAtTarget == nullptr);

            return intermediateEmpty && targetEmpty;
        }
    }
    else if (colDiff == 1 && rowDiff == direction) {
        if (pieceAtTarget != nullptr && pieceAtTarget->getColor() != this->getColor()) {
            return true;
        }
    }
    return false;
}
string Pawn::getType() const {
    return "Pawn";
}