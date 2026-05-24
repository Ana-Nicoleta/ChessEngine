#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "Game.h"

enum class AppState { Player1Input, Player2Input, Playing };

class ChessUI {
private:
    sf::RenderWindow window;
    Game chessGame;
    AppState currentState;

    sf::Texture pieceTexture;
    sf::Sprite pieceSprite;

    sf::Font font;

    std::string player1Name;
    std::string player2Name;
    int tileSize = 75;
    sf::Vector2i selectedSquare;

    sf::FloatRect undoButtonBounds;

    bool showNameWarning = false;

public:
    ChessUI() : window(sf::VideoMode(900, 750), "Amazing Chess"), currentState(AppState::Player1Input) {
        window.setFramerateLimit(60);
        if (!pieceTexture.loadFromFile("assets/pieces.png")) {
            std::cerr << "Error loading pieces.png!" << std::endl;
        }
        pieceSprite.setTexture(pieceTexture);

        if (!font.loadFromFile("assets/font.ttf")) {
            std::cerr << "Error loading font.ttf! (Make sure you put a .ttf file in your folder)" << std::endl;
        }

        selectedSquare = sf::Vector2i(-1, -1);
        undoButtonBounds = sf::FloatRect(660, 660, 200, 35);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

private:
    void handleMouseClick(int x, int y) {
        int boardOffsetX = 40;
        int boardOffsetY = 50;

        if (undoButtonBounds.contains(x, y)) {
            chessGame.undoTurn();
            selectedSquare = sf::Vector2i(-1, -1);
            return;
        }

        if (x >= boardOffsetX && x < boardOffsetX + (8 * tileSize) &&
            y >= boardOffsetY && y < boardOffsetY + (8 * tileSize)) {

            int col = (x - boardOffsetX) / tileSize;
            int row = 7 - ((y - boardOffsetY) / tileSize);

            if (selectedSquare.x == -1) {
                Piece* p = chessGame.getBoard().getPieceAt({row, col});
                if (p != nullptr && p->getColor() == chessGame.getTurn()) {
                    selectedSquare = sf::Vector2i(col, row);
                }
            } else {
                Position start = {selectedSquare.y, selectedSquare.x};
                Position end = {row, col};

                chessGame.playTurn(start, end);
                selectedSquare = sf::Vector2i(-1, -1);
            }
    }

}
    void drawGameStatus() {
        GameState state = chessGame.getState();
        std::string statusMessage = "";
        sf::Color textColor = sf::Color::Black;
        int fontSize = 26;

        if (state == GameState::Active) {
            Color turn = chessGame.getTurn();
            std::string currentPlayer = (turn == Color::white) ?
                                        (player1Name.empty() ? "Player 1" : player1Name) :
                                        (player2Name.empty() ? "Player 2" : player2Name);

            std::string colorName = (turn == Color::white) ? " (White)" : " (Black)";
            statusMessage = "Turn: " + currentPlayer + colorName;
            const Board& currentBoard = chessGame.getBoard();
            Position kingPos = currentBoard.getKingPosition(turn);
            Color enemyColor = (turn == Color::white) ? Color::black : Color::white;

            if (currentBoard.isSquareAttacked(kingPos, enemyColor)) {
                statusMessage += "  *** IN CHECK! ***";
                textColor = sf::Color::Red;
            }
        }
        else if (state == GameState::WhiteWins) {
            std::string p1 = player1Name.empty() ? "Player 1" : player1Name;
            statusMessage = "CHECKMATE! " + p1 + " (White) Wins!";
            textColor = sf::Color::Red;
            fontSize = 32;
        }
        else if (state == GameState::BlackWins) {
            std::string p2 = player2Name.empty() ? "Player 2" : player2Name;
            statusMessage = "CHECKMATE! " + p2 + " (Black) Wins!";
            textColor = sf::Color::Red;
            fontSize = 32;
        }
        else if (state == GameState::Stalemate) {
            statusMessage = "DRAW! Stalemate.";
            textColor = sf::Color::Blue;
            fontSize = 32;
        }

        sf::Text statusText(statusMessage, font, fontSize);
        statusText.setFillColor(textColor);
        statusText.setPosition(40, 10);

        window.draw(statusText);
    }
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (currentState == AppState::Player1Input) {
                    processTextInput(event.text.unicode, player1Name, AppState::Player2Input);
                } else if (currentState == AppState::Player2Input) {
                    processTextInput(event.text.unicode, player2Name, AppState::Playing);
                }
            }

            if (currentState == AppState::Playing && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                     handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            }
        }
    }

   void processTextInput(sf::Uint32 unicode, std::string& currentString, AppState nextState) {
        if (unicode == 8) {
            if (!currentString.empty()) {
                currentString.pop_back();
            }
            showNameWarning = false;
        }
        else if (unicode == 13) {
            if (!currentString.empty()) {

                if (currentState == AppState::Player2Input && currentString == player1Name) {
                    showNameWarning = true;
                    currentString = "";
                    return;
                }

                showNameWarning = false;
                currentState = nextState;
            }
        }
        else if (unicode >= 32 && unicode < 128 && currentString.length() < 15) {
            currentString += static_cast<char>(unicode);
            showNameWarning = false;
        }
    }

    void drawWelcomeScreen() {
        sf::Text titleText("Welcome to\nAmazing Chess", font, 60);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(window.getSize().x / 2.0f - titleText.getGlobalBounds().width / 2.0f, 100);
        window.draw(titleText);

        std::string promptStr = (currentState == AppState::Player1Input) ? "Write your name Player 1:" : "Write your name Player 2:";
        sf::Text promptText(promptStr, font, 35);
        promptText.setFillColor(sf::Color::Black);
        promptText.setPosition(window.getSize().x / 2.0f - promptText.getGlobalBounds().width / 2.0f, 300);
        window.draw(promptText);

        sf::RectangleShape inputBox(sf::Vector2f(400, 50));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Blue);
        inputBox.setOutlineThickness(2);
        inputBox.setPosition(window.getSize().x / 2.0f - 200, 360);
        window.draw(inputBox);

        std::string displayStr = (currentState == AppState::Player1Input) ? player1Name : player2Name;
        if (displayStr.length() < 15) {
            displayStr += "_";
        }

        sf::Text inputText(displayStr, font, 35);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 5);
        window.draw(inputText);
        window.draw(inputText);

        if (showNameWarning) {
            sf::Text warningText("Name already taken! Please choose a different name.", font, 22);
            warningText.setFillColor(sf::Color::Red);
            warningText.setPosition(window.getSize().x / 2.0f - warningText.getGlobalBounds().width / 2.0f, 430);
            window.draw(warningText);
        }
    }

void drawBoard() {
    sf::RectangleShape square(sf::Vector2f(tileSize, tileSize));

    int boardOffsetX = 40;
    int boardOffsetY = 50;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (selectedSquare.x == col && selectedSquare.y == row) {
                square.setFillColor(sf::Color(100, 255, 255, 200));
            }
            else {
                if ((row + col) % 2 == 0) {
                    square.setFillColor(sf::Color(255, 182, 193));
                } else {
                    square.setFillColor(sf::Color::White);
                }
            }
            int renderY = 7 - row;
            square.setPosition(boardOffsetX + (col * tileSize), boardOffsetY + (renderY * tileSize));
            window.draw(square);
        }
    }
}

   void drawPieces() {
    int boardOffsetX = 40;
    int boardOffsetY = 50;

    int spriteWidth = pieceTexture.getSize().x / 6;
    int spriteHeight = pieceTexture.getSize().y / 2;

    const Board& currentBoard = chessGame.getBoard();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* p = currentBoard.getPieceAt({row, col});

            if (p != nullptr) {
                int sheetRow = (p->getColor() == Color::black) ? 0 : 1;
                int sheetCol = 0;
                string type = p->getType();
                if (type == "Queen") sheetCol = 0;
                else if (type == "King") sheetCol = 1;
                else if (type == "Rook") sheetCol = 2;
                else if (type == "Knight") sheetCol = 3;
                else if (type == "Bishop") sheetCol = 4;
                else if (type == "Pawn") sheetCol = 5;

                pieceSprite.setTextureRect(sf::IntRect(sheetCol * spriteWidth, sheetRow * spriteHeight, spriteWidth, spriteHeight));

                float scaleX = (float)tileSize / spriteWidth;
                float scaleY = (float)tileSize / spriteHeight;
                pieceSprite.setScale(scaleX, scaleY);

                int renderY = 7 - row;
                pieceSprite.setPosition(boardOffsetX + (col * tileSize), boardOffsetY + (renderY * tileSize));
                window.draw(pieceSprite);
            }
        }
    }
}
   void render() {
        window.clear(sf::Color(173, 216, 230));

        if (currentState == AppState::Player1Input || currentState == AppState::Player2Input) {
            drawWelcomeScreen();
        } else if (currentState == AppState::Playing) {
            drawBoard();
            drawPieces();
            drawMoveHistory();
            drawUndoButton();
            drawGameStatus();
        }

        window.display();
    }
    void drawUndoButton() {
        sf::RectangleShape button(sf::Vector2f(undoButtonBounds.width, undoButtonBounds.height));
        button.setPosition(undoButtonBounds.left, undoButtonBounds.top);
        button.setFillColor(sf::Color(100, 149, 237));
        button.setOutlineColor(sf::Color::Black);
        button.setOutlineThickness(2);

        sf::Text text("Undo Last Move", font, 18);
        text.setFillColor(sf::Color::White);

        text.setPosition(undoButtonBounds.left + 30, undoButtonBounds.top + 6);

        window.draw(button);
        window.draw(text);
    }
   void drawMoveHistory() {
    int panelX = 660;
    int panelY = 50;
    int panelWidth = 200;

    sf::RectangleShape panel(sf::Vector2f(panelWidth, 600));
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(255, 182, 193, 150));
    panel.setOutlineColor(sf::Color::White);
    panel.setOutlineThickness(3);
    window.draw(panel);

    sf::Text title("Move History", font, 26);
    title.setFillColor(sf::Color::Black);
    title.setPosition(panelX + 25, panelY + 10);
    window.draw(title);

    sf::Text p1Text(player1Name.empty() ? "Player 1" : player1Name, font, 18);
    p1Text.setFillColor(sf::Color::Black);
    p1Text.setPosition(panelX + 15, panelY + 55);
    window.draw(p1Text);

    sf::Text p2Text(player2Name.empty() ? "Player 2" : player2Name, font, 18);
    p2Text.setFillColor(sf::Color::Black);
    p2Text.setPosition(panelX + 115, panelY + 55);
    window.draw(p2Text);

    sf::RectangleShape line(sf::Vector2f(panelWidth, 2));
    line.setFillColor(sf::Color::White);
    line.setPosition(panelX, panelY + 85);
    window.draw(line);

    const auto& history = chessGame.getMoveHistory();
    int yOffset = panelY + 95;

    for (size_t i = 0; i < history.size(); ++i) {
        const MoveCommand& m = history.get(i);

        char startCol = 'a' + m.getStart().column;
        char startRow = '1' + m.getStart().row;
        char endCol = 'a' + m.getEnd().column;
        char endRow = '1' + m.getEnd().row;

        std::string moveString = std::string(1, startCol) + startRow + "-" + endCol + endRow;
        sf::Text moveText(moveString, font, 18);
        moveText.setFillColor(sf::Color::Black);

        if (i % 2 == 0) {
            std::string turnNum = std::to_string((i / 2) + 1) + ". ";
            moveText.setString(turnNum + moveString);
            moveText.setPosition(panelX + 15, yOffset);
            window.draw(moveText);
        } else {
            moveText.setPosition(panelX + 115, yOffset);
            window.draw(moveText);
            yOffset += 25;
        }

        if (yOffset > 630) break;
    }
}
};
