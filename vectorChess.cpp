/*
Project: VectorChess
Author: github.com/d3vboi
Start Date: 22/06/2024
Description:
    A c++ chess game, written for the hack club arcade.
Version: 0.2.3
*/

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>

#include "vectorChess.hpp"
#include "search.hpp"
#include "knowledge.hpp"

class ChessGame {
  public: 
    ChessGame() {
    board.initialize();
  }

  void play(bool algebraicMode) {
    std::cout << "\033[2J\033[1;1H";
    printBoard();
    if (algebraicMode) {
        playAlgebraic();
    } else {
        playVisual();
    }
  }

  private: 
    Board board;
    PieceColor currentTurn = PieceColor::WHITE;
    Search engine;

  /**
   * Prints the current state of the chess board to the console.
   *
   * @param selected Whether a piece is currently selected.
   * @param selectedCol The column index of the currently selected piece.
   * @param selectedRow The row index of the currently selected piece.
   * @param cursorCol The column index of the current cursor position.
   * @param cursorRow The row index of the current cursor position.
   * @param turn The current turn color.
   */
  void printBoard(bool selected = false, int selectedCol = 0, int selectedRow = 0, int cursorCol = 0, int cursorRow = 0, PieceColor turn = PieceColor::WHITE) const
  {
    // Clear the screen

    std::cout << "\033[2J\033[1;1H";

    // Colours
    std::string prefix = "\e[";
    std::string suffix = "m";
    std::string cReset = "\e[0m";
    std::string bgWhite = "0;107";
    std::string bgBlack = "0;100";
    std::string bgGreen = "0;102";
    std::string bgBlue = "0;104";
    std::string bgRed = "0;101";
    std::string bgPurple = "0;105";
    std::string bgYellow = "0;103";
    std::string fgWhite = ";1;97";
    std::string fgBlack = ";1;90";
    std::string fgGreen = ";1;92";
    std::string fgBlue = ";1;94";
    std::string fgRed = ";1;91";

    int colorIte = 0;
    std::string tempOut;
    std::vector<std::pair<int, int>> validMoves;
    if (selected && selectedRow != -1 && selectedCol != -1) {
        validMoves = board.getValidMoves(selectedRow, selectedCol);
    }
    for (int row = 7; row >= 0; --row)
    {
      std::cout << prefix + bgBlue + fgBlack + suffix << " " << row + 1 << " " << cReset;
      for (int col = 0; col < 8; ++col)
      {
        tempOut = "";
        tempOut += prefix;
        tempOut += ((colorIte % 2 == 0) ? bgWhite + fgBlack : bgBlack + fgWhite);
        if (cursorCol == col && cursorRow == row)
        {
          tempOut += bgPurple;
        }
        if (selected && selectedCol == col && selectedRow == row)
        {
          tempOut += bgBlue;
        }
        if (selected)
        {
          for (int i = 0; i < validMoves.size(); i++)
          {
            if (validMoves[i].first == row && validMoves[i].second == col)
            {
              if (cursorCol == col && cursorRow == row)
              {
                tempOut += bgYellow;
              }
              else if (board.getPiece(row, col).getColor() == ((board.getPiece(selectedRow, selectedCol).getColor() == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE))
              {
                tempOut += bgRed;
              }
              else
              {
                tempOut += bgGreen;
              }
            }
          }
        }

        tempOut += suffix;
        Piece piece = board.getPiece(row, col);
        /*
        White:
        Pawn      ♟
        Knight    ♞
        Bishop    ♝
        Rook      ♜
        King      ♚
        Queen     ♛

        Black:
        Pawn      ♙
        Knight    ♘
        Bishop    ♗
        Rook      ♖
        King      ♔
        Queen     ♕
        */
        switch (piece.getType()) {
        case PieceType::PAWN:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♟ " : " ♙ ");
          break;
        case PieceType::KNIGHT:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♞ " : " ♘ ");
          break;
        case PieceType::BISHOP:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♝ " : " ♗ ");
          break;
        case PieceType::ROOK:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♜ " : " ♖ ");
          break;
        case PieceType::QUEEN:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♛ " : " ♕ ");
          break;
        case PieceType::KING:
          tempOut += (piece.getColor() == PieceColor::WHITE ? " ♚ " : " ♔ ");
          break;
        default:
          tempOut += "   ";
          break;
        };

        // Finally print the output
        std::cout << tempOut << cReset;
        colorIte++;
      }
      std::cout << std::endl;
      colorIte++;
    }
    std::cout << prefix + bgBlue + fgBlack + suffix << "    a  b  c  d  e  f  g  h " << cReset << std::endl;

    std::cout << (turn == PieceColor::WHITE ? "White" : "Black") << "'s turn." << std::endl;
  }

  bool parseMove(const std::string & move, int & fromRow, int & fromCol, int & toRow, int & toCol) const {
    if (move.length() < 4) return false;

    fromCol = move[0] - 'a';
    fromRow = move[1] - '1';
    toCol = move[2] - 'a';
    toRow = move[3] - '1';

    return (fromCol >= 0 && fromCol < 8 && fromRow >= 0 && fromRow < 8 &&
      toCol >= 0 && toCol < 8 && toRow >= 0 && toRow < 8);
  }

  void switchTurn() {
    currentTurn = (currentTurn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
  }


  void playAlgebraic() {
    std::string move;
    while (true) {
          std::cout << "Enter move (e.g., e2e4): ";
          std::cin >> move;
          if (move == "exit") break;

          int fromRow, fromCol, toRow, toCol;
          if (parseMove(move, fromRow, fromCol, toRow, toCol)) {
            if (board.movePiece(fromRow, fromCol, toRow, toCol, currentTurn)) {
              std::cout << "\033[2J\033[1;1H";
              printBoard();

              int moveScore = engine.eval<false>();
              std::cout << "Move score: " << moveScore << std::endl;

              if (board.isCheckmate(currentTurn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE)) {
                std::cout << "Checkmate! " << (currentTurn == PieceColor::WHITE ? "White" : "Black") << " wins!" << std::endl;
                break;
              } else if (board.isKingInCheck(currentTurn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE)) {
                std::cout << (currentTurn == PieceColor::WHITE ? "Black" : "White") << " is in check." << std::endl;
              }

              switchTurn();
            } else {
              std::cout << "Invalid move. Try again." << std::endl;
            }
          } else {
            std::cout << "Invalid input. Try again." << std::endl;
          }
        }
  }

  void playVisual() {
    std::string move;
    int cursorRow = 0;
    int cursorCol = 0;
    int selectedRow = 0;
    int selectedCol = 0;
    bool selected = false;
    while (true) {
      char input = getch();
      switch (input) {
        case 'w':
          cursorRow = std::min(7, cursorRow + 1);
          printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
          break;
        case 'a':
          cursorCol = std::max(0, cursorCol - 1);
          printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
          break;
        case 's':
          cursorRow = std::max(0, cursorRow - 1);
          printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
          break;
        case 'd':
          cursorCol = std::min(7, cursorCol + 1);
          printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
          break;
        case ' ':
          if (selected == true) {
            if (cursorRow == selectedRow && cursorCol == selectedCol) {
              selected = false;
              printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
            } else {
              if (board.isValidMove(selectedRow, selectedCol, cursorRow, cursorCol, currentTurn)) {
                board.movePiece(selectedRow, selectedCol, cursorRow, cursorCol, currentTurn);
                selected = false;
                if (board.isCheckmate(currentTurn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE)) {
                  printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
                  std::cout << "Checkmate! " << (currentTurn == PieceColor::WHITE ? "White" : "Black") << " wins!" << std::endl;
                  break;
                } else if (board.isKingInCheck(currentTurn == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE)) {
                  switchTurn();
                  printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
                  std::cout << (currentTurn == PieceColor::BLACK ? "Black" : "White") << " is in check." << std::endl;
                
                } else {
                  switchTurn();
                  printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
                }
              };
            };
          } else {
            selectedRow = cursorRow;
            selectedCol = cursorCol;
            selected = true;
            printBoard(selected, selectedCol, selectedRow, cursorCol, cursorRow, currentTurn);
          }
          break;
      }
      // std::cout << "Current cursor: " << cursorRow << ", " << cursorCol << std::endl;
    }
  }

  char getch() {
    char buf = 0;
    struct termios old = { 0 };
    if (tcgetattr(0, & old) < 0)
      perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, & old) < 0)
      perror("tcsetattr ICANON");
    if (read(0, & buf, 1) < 0)
      perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, & old) < 0)
      perror("tcsetattr ~ICANON");
    return buf;
  }
};

void helpMenu() {
  std::cout << "Usage: vectorChess [--algebraic] " << std::endl;
  std::cout << "  --help: Display this menu" << std::endl;
  std::cout << "  --algebraic: Use algebraic notation for moves" << std::endl;
}

int main(int argc, char *argv[]) {
  bool algebraicMode = false;
  if (argc > 1 && std::string(argv[1]) == "--help") {
    helpMenu();
    return 0;

  } else if (argc > 1 && std::string(argv[1]) == "--algebraic") {
    algebraicMode = true;
  }
  try {
    ChessGame game;
    game.play(algebraicMode);
  } catch (const std::exception & e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Unknown error occurred" << std::endl;
  }

  return 0;
}