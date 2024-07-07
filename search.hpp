#pragma once

#include "knowledge.hpp"
#include "vectorChess.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>
#include <utility>

using namespace std;

const int MateScore = 1e6;
const int TT_miss = 404000;
extern int TT_size;  // Declare TT_size as extern

enum EvalType { Exact, LowerBound, UpperBound };

struct TTEntry {
    int age;
    int depth;
    int score;
    EvalType eval_type;
    uint64_t hash;
};

using TT_t = TTEntry*;

class Move {
public:
    int fromRow, fromCol, toRow, toCol;
    Move(int fr = 0, int fc = 0, int tr = 0, int tc = 0) : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
    string to_uci() const {
        return string(1, 'a' + fromCol) + to_string(fromRow + 1) + string(1, 'a' + toCol) + to_string(toRow + 1);
    }
};

class Search {
public:
    Search();
    void set_clock(int _wtime, int _btime, int _winc, int _binc);
    pair<Move, int> search();
    template <bool debug> int eval();
    int negamax(int depth);
    int alphabeta(int depth, int alpha, int beta);
    int quiesce(int depth, int alpha, int beta);
    bool is_repetition();

private:
    vector<pair<int, Move>> iterative_search();
    int wtime, btime, winc, binc;
    bool searching = false;
    int nodes_searched = 0;
    int ply = 0;
    int max_depth = 64;
    vector<uint64_t> repetitions;
    Board board;
};

void init_TT(TT_t& TT, int size);
int normalize_score(int score, int depth);
inline int TT_probe(TT_t TT, uint64_t hash, int depth, int alpha, int beta);
inline void TT_store(TT_t TT, uint64_t hash, int depth, int score, EvalType eval_type);
int get_mate_score(int score);
void print_score(int score);
void print_info(string infostring, int depth, int score, int nodes_searched, int time_taken, string move);