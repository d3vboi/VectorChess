#include "search.hpp"

int TT_size = 1 << 16;  // Define TT_size

Search::Search() {
    // Initialize the board or any other necessary components
}

void Search::set_clock(int _wtime, int _btime, int _winc, int _binc) {
    wtime = _wtime;
    btime = _btime;
    winc = _winc;
    binc = _binc;
}

pair<Move, int> Search::search() {
    searching = true;
    auto movelist = iterative_search();
    searching = false;

    auto bestmove = movelist.front().second;
    auto bestscore = movelist.front().first;

    // choose random move out of same-scoring moves
    vector<pair<int, Move>> bestmoves;
    for (auto& score_move : movelist)
        if (score_move.first == bestscore) bestmoves.emplace_back(score_move);

    if (bestmoves.size() > 1) {
        random_device rd;
        uniform_int_distribution<int> dist(0, bestmoves.size() - 1);
        auto& best = bestmoves[dist(rd)];
        bestmove = best.second;
        bestscore = best.first;
    }

    cout << "info bestmove: " << bestscore << " = " << bestmove.to_uci()
         << " out of " << movelist.size() << " legal, " << bestmoves.size()
         << " best" << endl;
    cout << "bestmove " << bestmove.to_uci() << endl;
    return {bestmove, bestscore};
}

template <bool debug>
int Search::eval() {
    int material_score = 0;
    int pst_score = 0;
    int endgame_score = 0;
    int material_count[7] = {0};

    // Calculate material and piece-square table score
    for (int i = 0; i < 64; i++) {
        const Piece piece = board.getPiece(i / 8, i % 8);
        material_count[static_cast<int>(piece.getType())]++;
        material_score += piece_val[static_cast<int>(piece.getType()) + 6];
        if (piece.getType() != PieceType::EMPTY) {
            pst_score += pst[static_cast<int>(piece.getType()) - 1][piece.getColor() == PieceColor::WHITE ? i : 63 - i] * (piece.getColor() == PieceColor::WHITE ? 1 : -1);
        }
    }

    // Calculate phase
    const int PawnPhase = 0;
    const int KnightPhase = 1;
    const int BishopPhase = 1;
    const int RookPhase = 2;
    const int QueenPhase = 4;
    const int TotalPhase = PawnPhase * 16 + KnightPhase * 4 + BishopPhase * 4 + RookPhase * 4 + QueenPhase * 2;

    int phase = TotalPhase;
    phase -= material_count[static_cast<int>(PieceType::PAWN)] * PawnPhase;
    phase -= material_count[static_cast<int>(PieceType::KNIGHT)] * KnightPhase;
    phase -= material_count[static_cast<int>(PieceType::BISHOP)] * BishopPhase;
    phase -= material_count[static_cast<int>(PieceType::ROOK)] * RookPhase;
    phase -= material_count[static_cast<int>(PieceType::QUEEN)] * QueenPhase;
    phase = (phase * 256 + TotalPhase / 2) / TotalPhase;

    // Calculate endgame score
    int kingRow, kingCol, oppKingRow, oppKingCol;
    board.findKing(PieceColor::WHITE, kingRow, kingCol);
    board.findKing(PieceColor::BLACK, oppKingRow, oppKingCol);
    endgame_score += pst_k_end[kingRow * 8 + kingCol] - pst_k_end[63 - (oppKingRow * 8 + oppKingCol)];

    const int score = material_score + pst_score;
    const int eval = (score * (256 - phase) + endgame_score * phase) / 256;

    if (debug) {
        cout << "material: " << material_score << endl;
        cout << "position: " << pst_score << endl;
        cout << "opening score: " << score << endl;
        cout << "endgame score: " << endgame_score << endl;
        cout << "phase: " << phase << endl;
        cout << "adjusted opening score: " << score * (256 - phase) / 256 << endl;
        cout << "adjusted endgame score: " << endgame_score * phase / 256 << endl;
    }

    return eval;
}

// Explicit template instantiation
template int Search::eval<true>();
template int Search::eval<false>();

vector<pair<int, Move>> Search::iterative_search() {
    // Implement the iterative search logic here
    return {};
}

// Other methods (negamax, alphabeta, quiesce, etc.) can be defined here as needed