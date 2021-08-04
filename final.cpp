#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>

using namespace std;

int** init_board()
{
    int** t = new int*[5];
    for(int i = 0; i < 5; i++)
    {
        t[i] = new int[5];
        if(i == 0)
        {
            for(int j = 0; j < 5; j++)
                t[i][j] = 1;
        }
        if(i == 4)
        {
            for(int j = 0; j < 5; j++)
                t[i][j] = -1;
        }
    }
    t[1][0] = 1; t[1][4] = 1; t[2][0] = 1;
    t[2][4] = -1; t[3][0] = -1; t[3][4] = -1;
    return t;
}

int** copy_board(int** board)
{
    int** n_b = new int*[5];
    for(int i = 0; i < 5; i++)
    {
        n_b[i] = new int[5];
        for(int j = 0; j < 5; j++)
        {
            n_b[i][j] = board[i][j];
        }
    }
    return n_b;
}

void print_board(int** board)
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            if(board[4-i][j] == 1)
                cout << "X ";
            else if(board[4-i][j] == -1)
                cout << "O ";
            else
                cout << "- ";
        }
        cout << endl;
    }
}

struct Position
{
    int x; int y;
    Position(){x = y = 0;}
    Position(int a, int b)
    {
        x = a; y = b;
    }
};

struct Move
{
    Position pos_start, pos_end;
    Move(Position s, Position e)
    {
        pos_start = s; pos_end = e;
    }
};

vector<Position> ganh(int** board, Move m, int player);

vector<Position> vay(int** board, Move m, int player);

vector<Move> bay_or_mo(int** current_board, int** previous_board, int player);

vector<Move> get_valid_moves(int** current_board, int** previous_board, int player);

Move select_move(int** current_board, int** previous_board, int player);

void act_move(int** current_board, Move m, int player);

void play(int first)
{
    int count = 0, limit = 70;
    int player;
    if(first == 1)
        player = 1;
    else
        player = -1;

    int** board = init_board();
    int** pre_board = nullptr;
    print_board(board);
    while(count < limit)
    {
        count++;
        vector<Move> valid_moves = get_valid_moves(board, pre_board, player);
        if(valid_moves.size() != 0)
        {
            srand (time(NULL));
            int index = rand()% valid_moves.size();
            Move new_move = valid_moves[index];
            pre_board = copy_board(board);
            act_move(board, new_move, player);
        }
        else
            break;
        player *= -1;
    }
}

int main()
{
    play(1);
    return 0;
}

/*Defining function**********************************************************************************************************************/
/*Defining function**********************************************************************************************************************/