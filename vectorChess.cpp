/*
Project: VectorChess
Author: github.com/d3vboi
Start Date: 22/06/2024
Description:
    A c++ chess game, written for the hack club arcade.
Version: 0.2.1
*/

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

enum class PieceType {
  EMPTY,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING
};

enum class PieceColor {
  NONE,
  WHITE,
  BLACK
};

class Piece {
  public: Piece(PieceType type = PieceType::EMPTY, PieceColor color = PieceColor::NONE): type(type),
  color(color) {}

  PieceType getType() const {
    return type;
  }
  PieceColor getColor() const {
    return color;
  }

  void setType(PieceType newType) {
    type = newType;
  }
  void setColor(PieceColor newColor) {
    color = newColor;
  }

  private: PieceType type;
  PieceColor color;
};

class Board {
  public: Board() {
    initialize();
  }

  void initialize() {
    for (int col = 0; col < 8; ++col) {
      board[1][col] = Piece(PieceType::PAWN, PieceColor::WHITE);
    }
    board[0][0] = Piece(PieceType::ROOK, PieceColor::WHITE);
    board[0][1] = Piece(PieceType::KNIGHT, PieceColor::WHITE);
    board[0][2] = Piece(PieceType::BISHOP, PieceColor::WHITE);
    board[0][3] = Piece(PieceType::QUEEN, PieceColor::WHITE);
    board[0][4] = Piece(PieceType::KING, PieceColor::WHITE);
    board[0][5] = Piece(PieceType::BISHOP, PieceColor::WHITE);
    board[0][6] = Piece(PieceType::KNIGHT, PieceColor::WHITE);
    board[0][7] = Piece(PieceType::ROOK, PieceColor::WHITE);

    for (int col = 0; col < 8; ++col) {
      board[6][col] = Piece(PieceType::PAWN, PieceColor::BLACK);
    }
    board[7][0] = Piece(PieceType::ROOK, PieceColor::BLACK);
    board[7][1] = Piece(PieceType::KNIGHT, PieceColor::BLACK);
    board[7][2] = Piece(PieceType::BISHOP, PieceColor::BLACK);
    board[7][3] = Piece(PieceType::QUEEN, PieceColor::BLACK);
    board[7][4] = Piece(PieceType::KING, PieceColor::BLACK);
    board[7][5] = Piece(PieceType::BISHOP, PieceColor::BLACK);
    board[7][6] = Piece(PieceType::KNIGHT, PieceColor::BLACK);
    board[7][7] = Piece(PieceType::ROOK, PieceColor::BLACK);
  }

  Piece & getPiece(int row, int col) {
    return board[row][col];
  }

  const Piece & getPiece(int row, int col) const {
    return board[row][col];
  }

  bool movePiece(int fromRow, int fromCol, int toRow, int toCol, PieceColor currentTurn) {
    if (isValidMove(fromRow, fromCol, toRow, toCol, currentTurn)) {
      board[toRow][toCol] = board[fromRow][fromCol];
      board[fromRow][fromCol] = Piece();
      return true;
    }
    return false;
  }

  bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, PieceColor currentTurn) const {
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8) {
      return false;
    }

    Piece piece = board[fromRow][fromCol];

    if (piece.getType() == PieceType::EMPTY || piece.getColor() != currentTurn) {
      return false;
    }

    Piece destPiece = board[toRow][toCol];

    if (piece.getColor() == destPiece.getColor()) {
      return false;
    }

    // Movement rules
    switch (piece.getType()) {
    case PieceType::PAWN:
      if (piece.getColor() == PieceColor::WHITE) {
        if (fromCol == toCol && toRow == fromRow + 1 && destPiece.getType() == PieceType::EMPTY) {
          return true;
        }
        if (fromCol == toCol && fromRow == 1 && toRow == 3 && destPiece.getType() == PieceType::EMPTY &&
          board[2][toCol].getType() == PieceType::EMPTY) {
          return true;
        }
        if (std::abs(toCol - fromCol) == 1 && toRow == fromRow + 1 && destPiece.getType() != PieceType::EMPTY &&
          destPiece.getColor() == PieceColor::BLACK) {
          return true;
        }
      } else { // Black pawn
        if (fromCol == toCol && toRow == fromRow - 1 && destPiece.getType() == PieceType::EMPTY) {
          return true;
        }
        if (fromCol == toCol && fromRow == 6 && toRow == 4 && destPiece.getType() == PieceType::EMPTY &&
          board[5][toCol].getType() == PieceType::EMPTY) {
          return true;
        }
        if (std::abs(toCol - fromCol) == 1 && toRow == fromRow - 1 && destPiece.getType() != PieceType::EMPTY &&
          destPiece.getColor() == PieceColor::WHITE) {
          return true;
        }
      }
      break;

    case PieceType::KNIGHT:
      if ((std::abs(toRow - fromRow) == 2 && std::abs(toCol - fromCol) == 1) ||
        (std::abs(toRow - fromRow) == 1 && std::abs(toCol - fromCol) == 2)) {
        return true;
      }
      break;

    case PieceType::BISHOP:
      if (std::abs(toRow - fromRow) == std::abs(toCol - fromCol)) {
        int deltaRow = (toRow > fromRow) ? 1 : -1;
        int deltaCol = (toCol > fromCol) ? 1 : -1;
        for (int r = fromRow + deltaRow, c = fromCol + deltaCol; r != toRow; r += deltaRow, c += deltaCol) {
          if (board[r][c].getType() != PieceType::EMPTY) {
            return false;
          }
        }
        return true;
      }
      break;

    case PieceType::ROOK:
      if (fromRow == toRow || fromCol == toCol) {
        if (fromRow == toRow) {
          int deltaCol = (toCol > fromCol) ? 1 : -1;
          for (int c = fromCol + deltaCol; c != toCol; c += deltaCol) {
            if (board[fromRow][c].getType() != PieceType::EMPTY) {
              return false;
            }
          }
        } else {
          int deltaRow = (toRow > fromRow) ? 1 : -1;
          for (int r = fromRow + deltaRow; r != toRow; r += deltaRow) {
            if (board[r][fromCol].getType() != PieceType::EMPTY) {
              return false;
            }
          }
        }
        return true;
      }
      break;

    case PieceType::QUEEN:
      if ((fromRow == toRow || fromCol == toCol) || (std::abs(toRow - fromRow) == std::abs(toCol - fromCol))) {
        if (fromRow == toRow) {
          int deltaCol = (toCol > fromCol) ? 1 : -1;
          for (int c = fromCol + deltaCol; c != toCol; c += deltaCol) {
            if (board[fromRow][c].getType() != PieceType::EMPTY) {
              return false;
            }
          }
        } else if (fromCol == toCol) {
          int deltaRow = (toRow > fromRow) ? 1 : -1;
          for (int r = fromRow + deltaRow; r != toRow; r += deltaRow) {
            if (board[r][fromCol].getType() != PieceType::EMPTY) {
              return false;
            }
          }
        } else { // Diagonal movement
          int deltaRow = (toRow > fromRow) ? 1 : -1;
          int deltaCol = (toCol > fromCol) ? 1 : -1;
          for (int r = fromRow + deltaRow, c = fromCol + deltaCol; r != toRow; r += deltaRow, c += deltaCol) {
            if (board[r][c].getType() != PieceType::EMPTY) {
              return false;
            }
          }
        }
        return true;
      }
      break;

    case PieceType::KING:
      if (std::abs(toRow - fromRow) <= 1 && std::abs(toCol - fromCol) <= 1) {
        return true;
      }
      break;

    default:
      break;
    }
    // This bit caused a recursion error, still not quite sure why haha
    /*Board tempBoard = *this;
    tempBoard.movePiece(fromRow, fromCol, toRow, toCol, currentTurn);
    if (tempBoard.isKingInCheck(currentTurn)) {
        return false;
    }*/

    return false;
  }

  void findKing(PieceColor color, int& kingRow, int& kingCol) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board[row][col];
            if (piece.getType() == PieceType::KING && piece.getColor() == color) {
                kingRow = row;
                kingCol = col;
                return;
            }
        }
    }
    }

bool isKingInCheck(PieceColor color) const {
    int kingRow, kingCol;
    findKing(color, kingRow, kingCol);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board[row][col];
            if (piece.getColor() != color && piece.getType() != PieceType::EMPTY) {
                if (isValidMove(row, col, kingRow, kingCol, piece.getColor())) {
                    return true;
                }
            }
        }
    }

    return false; // Not in check
}

bool isCheckmate(PieceColor color) const {
    if (!isKingInCheck(color)) {
        return false; // Can't really be in checkmate if not in check
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece piece = board[row][col];
            if (piece.getColor() == color) {
                for (int toRow = 0; toRow < 8; ++toRow) {
                    for (int toCol = 0; toCol < 8; ++toCol) {
                        if (isValidMove(row, col, toRow, toCol, color)) {
                            return false; // Valid moves, not checkmate
                        }
                    }
                }
            }
        }
    }

    return true; // No valid moves, means it's a checkmate
}



  private: std::vector < std::vector < Piece >> board {
    8,
    std::vector < Piece > (8)
  };
};

class ChessGame {
  public: ChessGame() {
    board.initialize();
  }

  void play() {
    std::cout << "\033[2J\033[1;1H";
    printBoard();
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

  private: Board board;
  PieceColor currentTurn = PieceColor::WHITE;

  void printBoard() const {
    for (int row = 7; row >= 0; --row) {
      std::cout << row + 1 << " ";
      for (int col = 0; col < 8; ++col) {
        Piece piece = board.getPiece(row, col);
        switch (piece.getType()) {
        case PieceType::PAWN:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "P " : "p ");
          break;
        case PieceType::KNIGHT:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "N " : "n ");
          break;
        case PieceType::BISHOP:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "B " : "b ");
          break;
        case PieceType::ROOK:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "R " : "r ");
          break;
        case PieceType::QUEEN:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "Q " : "q ");
          break;
        case PieceType::KING:
          std::cout << (piece.getColor() == PieceColor::WHITE ? "K " : "k ");
          break;
        default:
          std::cout << ". ";
          break;
        }
      }
      std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
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
    std::cout << (currentTurn == PieceColor::WHITE ? "White" : "Black") << "'s turn." << std::endl;
  }
};

int main() {
    try {
        ChessGame game;
        game.play();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
    }

    return 0;
}
