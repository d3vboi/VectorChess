#pragma once

#include <vector>
#include <string>

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
public:
    Piece(PieceType type = PieceType::EMPTY, PieceColor color = PieceColor::NONE)
        : type(type), color(color) {}

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

private:
    PieceType type;
    PieceColor color;
};

class Board {
public:
    Board() : turn(PieceColor::WHITE) {
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

    Piece& getPiece(int row, int col) {
        return board[row][col];
    }

    const Piece& getPiece(int row, int col) const {
        return board[row][col];
    }

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol, PieceColor currentTurn) {
        if (isValidMove(fromRow, fromCol, toRow, toCol, currentTurn)) {
            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = Piece();
            turn = (turn == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
            return true;
        }
        return false;
    }

    PieceColor getTurn() const {
        return turn;
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

        return false;
    }

    std::vector<std::pair<int, int>> getValidMoves(int row, int col) const {
        std::vector<std::pair<int, int>> validMoves;
        Piece piece = board[row][col];
        PieceColor color = piece.getColor();

        for (int toRow = 0; toRow < 8; ++toRow) {
            for (int toCol = 0; toCol < 8; ++toCol) {
                if (isValidMove(row, col, toRow, toCol, color)) {
                    validMoves.emplace_back(toRow, toCol);
                }
            }
        }

        return validMoves;
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

private:
    std::vector<std::vector<Piece>> board{8, std::vector<Piece>(8)};
    PieceColor turn;
};