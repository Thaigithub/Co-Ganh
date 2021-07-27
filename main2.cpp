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
    int count = 0, limit = 50;
    int player=1;
    int** board = init_board();
    int** pre_board = nullptr;
    print_board(board);cout<<endl;
    while(count < limit)
    {
        if (count%2==first) {
            vector<Move> valid_moves = get_valid_moves(board, pre_board, player);
            if (valid_moves.size() != 0)
            {
                srand (time(NULL));
                int index = rand()% valid_moves.size();
                Move new_move = valid_moves[index];
                pre_board = copy_board(board);
                act_move(board, new_move, player);
            }
            else {
                break;
            }
        }
        else {
            Move m=select_move(board,pre_board,player);
            if (m.pos_start.x==-1 || m.pos_start.y==-1 || m.pos_end.x==-1 || m.pos_end.y==-1) {
                break;
            }
            else {
                pre_board = copy_board(board);
                act_move(board, m, player);
            }
        }
        player *= -1;
        count++;
    }
    print_board(board);
    count=0;
    for (int i=0;i<5;++i) {
        for (int j=0;j<5;++j) {
            if (board[i][j]==1) ++count;
        }
    }
    for (int i=0;i<5;++i) {
        delete [] board[i];
    }
    delete [] board;
    for (int i=0;i<5;++i) {
        delete [] pre_board[i];
    }
    delete [] pre_board;
    if (first) {
        if (count>=8) cout<<"You win!"<<endl;
        else cout<<"You lose!"<<endl;
    }
    else {
        if (count>8) cout<<"You lose!"<<endl;
        else cout<<"You win!"<<endl;
    }
}

int main()
{
    int first=1;
    cin>>first;
    play(first);
    system("pause");
    return 0;
}

class minimax{
private:
    float point;
    vector<minimax> descendent;
public:
    minimax(int** current_board, int** previous_board, int player, int degree,int& real_player) {
        this->point=0;
        if (degree<=1) {
            for (int i=0;i<5;++i) {
                for (int j=0;j<5;++j) {
                    if (current_board[i][j]==1) this->point+=1;
                    if (current_board[i][j]==-1) this->point-=1;
                }
            }
        }
        else {
            vector<Move> check=get_valid_moves(current_board,previous_board,player);
            if (check.size()!=0) {
                for (int i=0;i<check.size();++i) {
                    int **newboard=copy_board(current_board);
                    act_move(newboard,check[i],player);
                    descendent.push_back(minimax(newboard,current_board,-player,degree-1,real_player));
                    for (int j=0;j<5;++j) delete[] newboard[j];
                    delete[] newboard;
                }
                if (real_player>0) {
                    for (int i=0;i<descendent.size();++i) {
                        if (descendent[i].getpoint()>0) this->point+=1;
                    }
                    this->point=(this->point)/descendent.size();
                }
                else {
                    for (int i=0;i<descendent.size();++i) {
                        if (descendent[i].getpoint()<0) this->point-=1;
                    }
                    this->point=(this->point)/descendent.size();
                }
            }
            else {
                for (int i=0;i<5;++i) {
                    for (int j=0;j<5;++j) {
                        if (current_board[i][j]==1) ++(this->point);
                        if (current_board[i][j]==-1) --(this->point);
                    }
                }
            }
        }
    }
    float getpoint() {return this->point;}
    float getaverage() {
        float out=0;
        for (int i=0;i<descendent.size();++i) {
            out+=descendent[i].getpoint();
        }
        out=out/descendent.size();
        return out;
    }
};

bool comparepos(Position a, Position b) { //check if the positions are the same
    return (a.x==b.x && a.y==b.y);
}

void filter(vector<Position> &a, int** board,int player) { //remove the positons of the board at which are not eual to player
    for (int i=0;i<a.size();) {
        if (board[a[i].x][a[i].y]!=player) a.erase(a.begin()+i);
        else ++i;
    }
}

vector<Position> connection(Position now) { //return the positions that are connected with the input on the board
    vector <Position> out;
    if (now.x>0) out.push_back(Position(now.x-1,now.y));
    if (now.y>0) out.push_back(Position(now.x,now.y-1));
    if (now.x<4) out.push_back(Position(now.x+1,now.y));
    if (now.y<4) out.push_back(Position(now.x,now.y+1));
    if ((now.x+now.y)%2==0) {
        if (now.x>0 && now.y>0) out.push_back(Position(now.x-1,now.y-1));
        if (now.x<4 && now.y>0) out.push_back(Position(now.x+1,now.y-1));
        if (now.x<4 && now.y<4) out.push_back(Position(now.x+1,now.y+1));
        if (now.x>0 && now.y<4) out.push_back(Position(now.x-1,now.y+1));
    }
    return out;
}

vector<Position> ganh(int** board, Move m, int player) {
    vector <Position> out;
    if (comparepos(m.pos_end,m.pos_start)) return out;
    vector <Position> check=connection(m.pos_end);
    for (int i=0;i<check.size()-1;++i) {
        if (board[check[i].x][check[i].y]==-player) {
            for (int j=i+1;j<check.size();++j) {
                if (board[check[j].x][check[j].y]==-player && check[j].x==2*m.pos_end.x-check[i].x && check[j].y==2*m.pos_end.y-check[i].y) {
                    out.push_back(check[i]);
                    out.push_back(check[j]);
                    check.erase(check.begin()+i);
                    check.erase(check.begin()+j);
                    --i;
                    break;
                }
            }
        }
    }
    int **new_board=copy_board(board);
    for (int i=0;i<out.size();++i) new_board[out[i].x][out[i].y]=player;
    int size=out.size();
    for (int i=0;i<size;++i) {
        check.clear();
        check=vay(new_board,Move(Position(),out[i]),player);
        for (int j=0;j<check.size();++j) {
            bool flag=true;
            for (int k=0;k<out.size();++k) {
                if (comparepos(out[k],check[j])) flag=false;
            }
            if (flag) out.push_back(check[j]);
        }
    }
    return out;
}

vector<Position> vay(int** board, Move m, int player) {
    vector <Position> out,check;
    if (comparepos(m.pos_end,m.pos_start)) return out;
    check=connection(m.pos_end);
    filter(check,board,-player);
    bool flag=true;
    while (flag) {
        flag=false;
        for (int i=0;i<check.size();++i) {
            bool flag1=true;
            for (int j=0;j<out.size();++j) {
                if (comparepos(out[j],check[i])) {
                    flag1=false;
                    break;
                }
            }
            if (flag1) {
                out.push_back(check[i]);
                flag=true;
            }
        }
        int size=check.size();
        for (int i=0;i<size;++i) {
            vector <Position> insert=connection(check[0]);
            filter(insert,board,-player);
            bool flag1=true;
            for (int j=0;j<insert.size();++j) {
                for (int k=0;k<out.size();++k) {
                    if (comparepos(insert[j],out[k])) {
                        flag1=false;
                        break;
                    }
                }
                if (flag1) check.push_back(insert[j]);
            }
            check.erase(check.begin());
        }
    }
    flag=true;
    for (int i=0;i<out.size();++i) {
        check=connection(out[i]);
        filter(check,board,0);
        if (check.size()!=0) {
            out.clear();
            return out;
        }
        check.clear();
    }
    return out;
}

vector<Move> bay_or_mo(int** current_board, int** previous_board, int player) {
    vector <Move> out;
    if (previous_board!=NULL) {
        vector <Position> change;
        for (int i=0;i<5;++i) {
            for (int j=0;j<5;++j) {
                if (current_board[i][j]!=previous_board[i][j]) change.push_back(Position(i,j));
            }
        }
        if (change.size()==2) {
            vector <Position> check;
            vector <vector<Position>> possible;
            if (current_board[change[0].x][change[0].y]==-player) check=connection(change[0]);
            else check=connection(change[1]);
            filter(check,current_board,0);
            for (int i=0;i<check.size();) {
                possible.push_back(connection(check[i]));
                filter(possible[possible.size()-1],current_board,player);
                if (possible[possible.size()-1].size()==0) {
                    check.erase(check.begin()+i);
                    possible.erase(possible.end());
                }
                else ++i;
            }
            for (int i=0;i<check.size();++i) {
                vector<Position> g=ganh(current_board,Move(possible[i][0],check[i]),player);
                if (g.size()!=0) {
                    for (int j=0;j<possible[i].size();++j) {
                        out.push_back(Move(possible[i][j],check[i]));
                    }
                }
            }
        }
    }
    return out;
}

vector<Move> get_valid_moves(int** current_board, int** previous_board, int player) {
    vector <Move> out;
    if (previous_board!=NULL) {
        vector <Position> change;
        for (int i=0;i<5;++i) {
            for (int j=0;j<5;++j) {
                if (current_board[i][j]!=previous_board[i][j]) change.push_back(Position(i,j));
            }
        }
        if (change.size()==2) {
            vector <Position> check;
            vector <vector<Position>> possible;
            if (current_board[change[0].x][change[0].y]==-player) check=connection(change[0]);
            else check=connection(change[1]);
            filter(check,current_board,0);
            for (int i=0;i<check.size();) {
                possible.push_back(connection(check[i]));
                filter(possible[possible.size()-1],current_board,player);
                if (possible[possible.size()-1].size()==0) {
                    check.erase(check.begin()+i);
                    possible.erase(possible.end());
                }
                else ++i;
            }
            for (int i=0;i<check.size();++i) {
                vector<Position> g=ganh(current_board,Move(possible[i][0],check[i]),player);
                if (g.size()!=0) {
                    for (int j=0;j<possible[i].size();++j) {
                        out.push_back(Move(possible[i][j],check[i]));
                    }
                }
            }
            if (out.size()!=0) return out;
        }
    } else {
        for (int i=1;i<4;++i) {
            for (int j=1;j<4;++j) {
                current_board[i][j]=0;
            }
        }
    }
    for (int i=0;i<5;++i) {
        for (int j=0;j<5;j++) {
            if (current_board[i][j]==player) {
                vector <Position> check=connection(Position(i,j));
                for (int k=0;k<check.size();++k) {
                    if (current_board[check[k].x][check[k].y]==0) {
                        out.push_back(Move(Position(i,j),check[k]));
                    }
                }
            }
        }
    }
    return out;
}

Move select_move(int** current_board, int** previous_board, int player) {
    vector<Move> check=get_valid_moves(current_board,previous_board,player);
    vector<minimax> probabilities;
    if (check.size()==0) return Move(Position(-1,-1),Position(-1,-1));
    for (int i=0;i<check.size();++i) {
        int **newboard=copy_board(current_board);
        act_move(newboard,check[i],player);
        probabilities.push_back(minimax(newboard,current_board,-player,6,player));
        for (int j=0;j<5;++j) delete[] newboard[j];
        delete[] newboard;
    }
    int choice=0;
    if (player>0) {
        for (int i=1;i<probabilities.size();++i) {
            if (probabilities[i].getpoint()>probabilities[choice].getpoint()) choice=i;
        }
    }
    else {
        for (int i=1;i<probabilities.size();++i) {
            if (probabilities[i].getpoint()<probabilities[choice].getpoint()) choice=i;
        }
    }
    for (int i=0;i<probabilities.size();) {
        if (probabilities[i].getpoint()!=probabilities[choice].getpoint()) {
            check.erase(check.begin()+i);
            probabilities.erase(probabilities.begin()+i);
        }
        else ++i;
    }
    choice=0;
    if (player>0) {
        for (int i=1;i<probabilities.size();++i) {
            if (probabilities[i].getaverage()>probabilities[choice].getaverage()) choice=i;
        }
    }
    else {
        for (int i=1;i<probabilities.size();++i) {
            if (probabilities[i].getaverage()<probabilities[choice].getaverage()) choice=i;
        }
    }
    return check[choice];
}

void act_move(int** current_board, Move m, int player) {
    vector <Position> g, v;
    g=ganh(current_board,m,player);
    v=vay(current_board,m,player);
    for (int i=0;i<g.size();++i) current_board[g[i].x][g[i].y]=player;
    for (int i=0;i<v.size();++i) current_board[v[i].x][v[i].y]=player;
    current_board[m.pos_start.x][m.pos_start.y]=0;
    current_board[m.pos_end.x][m.pos_end.y]=player;
}