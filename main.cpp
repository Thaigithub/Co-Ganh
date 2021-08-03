#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>

using namespace std;

/*Assignment function********************************************************************************************************************/
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
    cout<<"  0 1 2 3 4 x"<<endl;
    for(int i = 0; i < 5; i++)
    {
        cout<<i<<" ";
        for(int j = 0; j < 5; j++)
        {
            if(board[i][j] == 1)
                cout << "X ";
            else if(board[i][j] == -1)
                cout << "O ";
            else
                cout << "- ";
        }
        cout << endl;
    }
    cout<<"y"<<endl;
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
void play(int player);
int main()
{
    cout<<"Welcome to Co Ganh on C++ by Thai Tran"<<endl;
    int first=-1;
    while (first!=1 && first!=0) {
        cout<<"Want to start first? (1 for yes and 0 for no): ";
        cin>>first;
        cout<<"You are X"<<endl;
    }
    play(first);
    cout<<endl<<"If you win, please take a screenshot and send me, I'll buy you a drink when we meet again"<<endl;
    cout<<"Please give me a feedback at: https://docs.google.com/forms/d/1k9DOVuHvqHjWDzmbX3Rc98qcRitrQv9yMZAGjC1rBRg/viewform?edit_requested=true#responses"<<endl;
    cout<<"Co Ganh - by Thai Tran - DSA Semester 203"<<endl;
    system("pause");
    return 0;
}
/*Assignment function********************************************************************************************************************/

/*Addition function**********************************************************************************************************************/
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
/*Addition function**********************************************************************************************************************/

/*Defining function**********************************************************************************************************************/
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
    int** new_board=copy_board(board);

    new_board[m.pos_start.x][m.pos_start.y]=0;
    new_board[m.pos_end.x][m.pos_end.y]=player;
    check=connection(m.pos_end);
    filter(check,new_board,-player);
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
            filter(insert,new_board,-player);
            for (int j=0;j<insert.size();++j) {
                bool flag1=true;
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
    check.clear();
    vector<vector<Position>> classification;
    for (int i=0;i<out.size();++i) {
        flag=false;
        for (int j=0;j<classification.size();++j) {
            for (int k=0;k<classification[j].size();++k) {
                check=connection(classification[j][k]);
                filter(check,new_board,-player);
                for (int l=0;l<check.size();++l) {
                    if (comparepos(check[l],out[i])) {
                        classification[j].push_back(out[i]);
                        flag=true;
                        break;
                    }
                }
                if (flag) break;
            }
            if (flag) break;
        }
        if (!flag) classification.push_back(vector<Position>{out[i]});
    }
    out.clear();
    check.clear();
    for (int i=0;i<classification.size();++i) {
        flag=true;
        for (int j=0;j<classification[i].size();++j) {
            check=connection(classification[i][j]);
            filter(check,new_board,0);
            if (check.size()!=0) {
                flag=false;
                break;
            }
        }
        if (flag) {
            for (int j=0;j<classification[i].size();++j) out.push_back(classification[i][j]);
        }
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
        probabilities.push_back(minimax(newboard,current_board,-player,4,player));
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

void play(int first)
{
    int player=1;
    int count = 0, limit = 50;
    int** board = init_board();
    int** pre_board = nullptr;
    print_board(board);
    while (count<limit) {
        count++;
        if (false) {
            cout<<"Your turn: "<<endl;
            vector<Move> valid_moves = get_valid_moves(board, pre_board, player);
            if(valid_moves.size() != 0) {
                cout<<"Moves you can go: "<<endl;
                for (int i=0;i<valid_moves.size();++i) {
                    cout<<"From: ("<<valid_moves[i].pos_start.y<<" "<<valid_moves[i].pos_start.x<<") to ("<<valid_moves[i].pos_end.y<<" "<<valid_moves[i].pos_end.x<<")"<<endl;
                }
                Move m(Position(-1,-1),Position(-1,-1));
                bool flag=true;
                while (flag) {
                    int x_old=-1,y_old=-1,x_new=-1,y_new=-1;
                    while ((0>x_old)||(x_old>4)||(0>y_old)||(y_old>4)||(0>x_new)||(x_new>4)||(0>y_new)||(y_new>4)) {
                        cout<<"Load the coordinate from (x y): ";
                        cin>>y_old>>x_old;
                        cout<<"Load the coordinate to (x y): ";
                        cin>>y_new>>x_new;
                    }
                    m=Move(Position(x_old,y_old),Position(x_new,y_new));
                    for (int i=0;i<valid_moves.size();++i) {
                        if (comparepos(m.pos_start,valid_moves[i].pos_start)&&comparepos(m.pos_end,valid_moves[i].pos_end)) flag=false;
                    }
                }
                pre_board = copy_board(board);
                act_move(board, m, player);
            }
            else {
                break;
            }
        } else {
            time_t current_time=time(NULL);
            Move m=select_move(board,pre_board,player);
            cout<<difftime(time(NULL),current_time)<<endl;
            if (m.pos_start.x==-1 || m.pos_start.y==-1 || m.pos_end.x==-1 || m.pos_end.y==-1) {
                break;
            }
            else {
                cout<<"My turn: "<<endl;
                pre_board = copy_board(board);
                act_move(board, m, player);
            }        
        }
        player *= -1;
        print_board(board);
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
/*Defining function**********************************************************************************************************************/