#include "Game.h"

Game::Game() : currentTurn(Color::white), state(GameState::Active) {}

bool Game::playTurn(Position start, Position end) {
    if (state != GameState::Active) return false;

    Piece* movingPiece = board.getPieceAt(start);

    if (movingPiece == nullptr || movingPiece->getColor() != currentTurn) {
        return false;
    }

    if (board.isLegalMove(start, end)) {
        board.movePiece(start, end);

        Piece* landedPiece = board.getPieceAt(end);
        if (landedPiece->getType() == "Pawn") {
            if ((currentTurn == Color::white && end.row == 7) ||
                (currentTurn == Color::black && end.row == 0)) {
                board.promoteToQueen(end);
                }
        }
        currentTurn = (currentTurn == Color::white) ? Color::black : Color::white;
        updateGameState();
        return true;
    }
    return false;
}

void Game::updateGameState() {
    bool hasMoves = board.findCheck(currentTurn);
    Position kingPos = board.getKingPosition(currentTurn);
    Color enemyColor = (currentTurn == Color::white) ? Color::black : Color::white;
    bool inCheck = board.isSquareAttacked(kingPos, enemyColor);

    if (!hasMoves) {
        if (inCheck) {
            state = (currentTurn == Color::white) ? GameState::BlackWins : GameState::WhiteWins;
        } else {
            state = GameState::Stalemate;
        }
    }
}
GameState Game::getState() const {
    return state;
}

Color Game::getTurn() const {
    return currentTurn;
}
const Board& Game:: getBoard() const {
    return board;
}
void Game::undoTurn() {
    if (!board.getMoveHistory().isEmpty()) {
        board.undoLastMove();
        currentTurn = (currentTurn == Color::white) ? Color::black : Color::white;
        state = GameState::Active;
    }
}