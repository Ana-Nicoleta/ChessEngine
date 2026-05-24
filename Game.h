#pragma once
#include "Board.h"

enum class GameState { Active, WhiteWins, BlackWins, Stalemate };

class Game {

    Board& board=Board::getInstance();
    Color currentTurn;
    GameState state;

    void updateGameState();

public:
    Game();
    bool playTurn(Position start, Position end);
    GameState getState() const;
    Color getTurn() const;
    const Board& getBoard() const;
    const FixedStack<MoveCommand, 500>& getMoveHistory() const { return board.getMoveHistory(); }
    void undoTurn();
};