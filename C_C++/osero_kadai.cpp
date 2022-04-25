#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;

//0:ユーザー、1:AI
int turn;

//ボードの縦横の大きさ
const int board_size = 8;
//マスの状態
enum class square_state { EMPTY, DARK, LIGHT };
//１行分のデータを表す型
typedef vector<square_state> board_line;
//オセロのゲーム盤を表す型
typedef vector<board_line> othello_board;

//自分の駒
square_state my_side;
//相手の駒
square_state enemy;

//ゲーム盤を表す変数
//マスは8×8列分用意し、周りは空けておく。
othello_board board(board_size, board_line(board_size, square_state::EMPTY));
//            変数名　ボードの周　　　　　　　　　ボードの周を含める 全部を空にする

//pairは２つの値を持つコンテナ
//方向を表す型（行・列）
typedef pair<int, int> direction;

//定数のときは、ベクタではなく配列にする
const direction direction_list[] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
};

//裏返す方向を保存するコンテナ
vector<int> rev_list = {};

void disp(vector<vector<square_state>> board_disp) {
    //場所指定のための指標
    cout << "  . a. b. c. d. e. f. g. h." << endl;
    for (size_t line = 0; line < 8; line++) {
        for (size_t row = 0; row < 8; row++) {
            if (row == 0) {
                //場所指定のための指標
                cout << line + 1 << " ";
            }
            //表示方法
            switch (board[line][row])
            {
            //白の時
            case square_state::LIGHT:
                cout << ".●";
                break;
            //黒の時
            case square_state::DARK:
                cout << ".○";
                break;
            //空の時
            case square_state::EMPTY:
                cout << ".  ";
                break;
            default:
                break;
            }
            //右端の壁
            if (row == 7) {
                cout << "." << endl;
            }
        }
    }
}

//
bool is_turnable(othello_board& board, int row, int col, square_state my_side, const direction& d) {
    //置くマスが空いているかどうか
    if (board[row][col] == square_state::EMPTY) {
        //8方のいずれかのマス
        int tr = row + d.first;
        int tc = col + d.second;
        //boardから外れていたらfalse
        if (tr < 0 || tc < 0 || tr >= 8 || tc >= 8) {
            return false;
        }
        //隣のマスに相手の駒があるかどうか
        if (board[tr][tc] != my_side && board[tr][tc] != square_state::EMPTY) {
            //tr,tcが盤面内なら、ループする
            for (tr += d.first,tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                //自分の駒があれば
                if (board[tr][tc] == my_side) {
                    return true;
                }
                //空の時
                else if (board[tr][tc] == square_state::EMPTY) {
                    return false;
                }
            }
        }
    }
    return false;
}

//row行col列の位置にmy_sideの石が置けるか？
bool is_puttable(othello_board& board, int row, int col, square_state my_side) {
    //返す方向のdirection_listのindexを計算する
    int i = 0;
    //各方向に置けるか？
    //8方向のマスを一つずつdに代入
    for (const direction& d : direction_list) {
        //1方向において、敵コマの先に自コマがあるとき
        if (is_turnable(board, row, col, my_side, d)) {
            //返したい方向を保存
            rev_list.push_back(i);
        }
        i++;
    }
    //8方向すべて検査し終えて、そのマスに置くことができるなら
    if (rev_list.size() > 0) {
        return true; // １方向でも裏返せたら置ける.
    }
    //どの方向も裏返せなかった。
    return false;
}

//盤上のsの数を数える
int count_piece(const othello_board& board, square_state s) {
    int c = 0;
    for (auto it = board.begin() + 1; it != board.end() - 1; ++it) {
        //１行分数える
        c += count(it->begin() + 1, it->end() - 1, s);
    }
    return c;
}
// it.begin()　は　it->begin()と書くことも出来る

//石を置いたときにひっくりかえす関数
//引数board(場面の状態), row(置く行), col(置く列), my_side(自分の駒の色), dir(ひっくりかえす方向)
void turn_over(othello_board& board,int row, int col, square_state my_side, vector<int> dir) {
    for (int i = 0; i < dir.size(); i++) {
        //rowの値を保存
        int row_temp = row;
        //colの値を保存
        int col_temp = col;
        //方向の座標移動を保存
        auto reverce_dir = direction_list[dir[i]];
        cout << reverce_dir.first << endl;
        //自分の駒が来るまでループ
        for (row_temp += reverce_dir.first, col_temp += reverce_dir.second;
            board[row_temp][col_temp] != my_side;
            row_temp += reverce_dir.first, col_temp += reverce_dir.second) {
            //自分の駒に変更
            board[row_temp][col_temp] = my_side;
        }
    }
}

//列番号をアルファベットから数字に変換
int culcu_cul(char alph) {
    switch (alph)
    {
    case 'a':
        return 0;
    case 'b':
        return 1;
    case 'c':
        return 2;
    case 'd':
        return 3;
    case 'e':
        return 4;
    case 'f':
        return 5;
    case 'g':
        return 6;
    case 'h':
        return 7;
    }
}

bool puttable_area(othello_board& board, square_state turn_) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == square_state::EMPTY) {
                //各方向に置けるか？
                //8方向のマスを一つずつdに代入
                for (const direction& d : direction_list) {

                    //8方のいずれかのマス
                    int tr = i + d.first;
                    int tc = j + d.second;
                    if (tr < 0 || tr > 7 || tc < 0 || tc > 7)continue;
                    //隣のマスに相手の駒があるかどうか
                    if (board[tr][tc] != turn_ && board[tr][tc] != square_state::EMPTY) {
                        //tr,tcが盤面内なら、ループする
                        for (tr += d.first, tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                            //自分の駒があれば
                            if (board[tr][tc] == turn_) {
                                return false;
                            }
                            else if (board[tr][tc] == square_state::EMPTY) {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

//AIが置けるところを探す
pair<int, int> search_puttable_position() {
    //AIの駒が置けるところを保存
    pair<int, int> ai_posi;
    int ec = 1000;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //i,jのemptyの数を保存
            int temp_ec = 0;
            //ひっくりかえす時の周りのemptyの座標を保存
            vector<pair<int, int> > empty_point;

            //場所が空いていれば
            if (board[i][j] == square_state::EMPTY) {
                //各方向に置けるか？
                //8方向のマスを一つずつdに代入
                for (const direction& d : direction_list) {
                    //8方のいずれかのマス
                    int tr = i + d.first;
                    int tc = j + d.second;

                    //boardの範囲外なら
                    if (tr < 0 || tr > 7 || tc < 0 || tc > 7)continue;//判定を中止し、次の判定へ
                    //隣のマスに相手の駒があるかどうか
                    if (board[tr][tc] == my_side) {
                        //tr,tcが盤面内なら、ループする
                        for (tr += d.first, tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                            //自分の駒があれば
                            if (board[tr][tc] == enemy) {
                                //置きたい位置
                                int ttr = i;
                                int ttc = j;

                                //置きたい位置から自分の駒になるまで繰り返し
                                for (ttr, ttc; board[ttr][ttc] != enemy; ttr += d.first, ttc += d.second) {
                                    //全方向にemptyがあるかを検査
                                    for (const direction& n : direction_list) {
                                        //各方向の座標を保存
                                        int temp_ttr = ttr + n.first, temp_ttc = ttc + n.second;
                                        //boardの範囲外なら
                                        if (temp_ttr < 0 || temp_ttr > 7 || temp_ttc < 0 || temp_ttc > 7)continue;//判定を中止し、次の判定へ

                                        //座標の地点がemptyなら
                                        if (board[temp_ttr][temp_ttc] == square_state::EMPTY) {
                                            //まだempty_pointに座標が登録されていなかったら
                                            if (empty_point.size() == 0) {
                                                empty_point.push_back({ temp_ttr,temp_ttc });
                                            }
                                            for (int k = 0; k < empty_point.size(); k++) {
                                                if (empty_point[k].first != temp_ttr && empty_point[k].second != temp_ttc) {
                                                    //最後まで合致する要素が無ければ
                                                    if (k == empty_point.size() - 1) {
                                                        //座標を登録
                                                        empty_point.push_back({ temp_ttr,temp_ttc });
                                                    }
                                                }
                                                //あったら
                                                else {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                            else if (board[tr][tc] == square_state::EMPTY) {
                                break;
                            }
                        }
                    }
                }
                //emptyの数を数える -1は置こうとしている場所
                temp_ec = empty_point.size() - 1;
                if (ec > temp_ec && temp_ec > 0) {
                    ai_posi = { i,j };
                    ec = temp_ec;
                }
            }
        }
    }
    return ai_posi;
}

void game() {
    disp(board);
    while (true) {

        //ユーザーの番の時
        if (turn == 0) {
            //置くことが出来なかったら
            if (puttable_area(board, my_side)) {
                cout << "\nPASS\n" << endl;
                turn = 1;
                rev_list = {};
                continue;
            }
            if (my_side == square_state::DARK) {
                cout << "\n       you are : " << "○\n" << endl;
            }
            else if (my_side == square_state::LIGHT) {
                cout << "\n       you are : " << "●\n" << endl;
            }
            //行番号
            int row;
            //列番号
            char alph;
            cout << "置く座標を指定してください\n\n"
                "縦の数字を入力してください = ";
            cin >> row;
            row--;
            cout << "横のalphabetを入力してください = ";
            cin >> alph;
            //列番号を数字に変換
            int col = culcu_cul(alph);

            //駒を置くことが出来るなら
            if (is_puttable(board, row, col, my_side)) {
                //ひっくりかえす
                board[row][col] = my_side;
                //cout << rev_list.size() << "rev" << endl;
                turn_over(board, row, col, my_side, rev_list);
                //ひっくりかえすリストを空に
                rev_list = {};
            }
            else {
                cout << "そのマスには置けません\n" << endl;
                continue;
            }
        }
        else {
            //置くことが出来なかったら
            if (puttable_area(board, enemy)) {
                cout << "\nPASS\n" << endl;
                turn = 0;
                rev_list = {};
                continue;
            }
            //if (enemy == square_state::DARK) {
            //    cout << "\n       now turn : " << "○\n" << endl;
            //}
            //else if (enemy == square_state::LIGHT) {
            //    cout << "\n       now turn : " << "●\n" << endl;
            //}
            ////行番号
            //int row;
            ////列番号
            //char alph;
            //cout << "置く座標を指定してください\n"
            //    "num = ";
            //cin >> row;
            //row--;
            //cout << "alphabet = ";
            //cin >> alph;
            ////列番号を数字に変換
            //int col = culcu_cul(alph);

            auto posi = search_puttable_position();
            int row = posi.first;
            int col = posi.second;

            //駒を置くことが出来るなら
            if (is_puttable(board, row, col, enemy)) {
                //ひっくりかえす
                board[row][col] = enemy;
                cout << rev_list.size() << "rev" << endl;
                turn_over(board, row, col, enemy, rev_list);
                rev_list = {};
            }
            else {
                cout << "そのマスには置けません\n" << endl;
                continue;
            }

        }

        //順番を制御
        //0:ユーザー、1:AI
        if (turn == 0)turn = 1;
        else turn = 0;
        system("cls");
        disp(board);
        //両方の置くことが出来なかったら
        if (puttable_area(board, enemy) && puttable_area(board, my_side)) {
            break;
        }
    }
    int dark = count_piece(board, square_state::DARK);
    int white = count_piece(board, square_state::LIGHT);
    cout << "○ : " << dark << endl;
    cout << "● : " << white << endl;
    if (dark == white) {
        cout << "\n draw" << endl;
    }
    else if (dark > white) {
        cout << "\n ○ win" << endl;
    }
    else {
        cout << " \n ● win" << endl;
    }
}

int main() {
    
    cout << "Game Start!!" << endl;
    cout << "どちらか選んで番号を入力してください\n"
        "先行 : 0\n"
        "後攻 : 1" << endl;
    //先行か後攻か
    int my;
    //先攻後攻の入力
    cin >> my;

    if (my == 0) {
        my_side = square_state::DARK;
        enemy = square_state::LIGHT;
        turn = 0;
    }
    else {
        my_side = square_state::LIGHT;
        enemy = square_state::DARK;
        turn = 1;
    }

    //初期配置
    board[3][3] = square_state::LIGHT;
    board[3][4] = square_state::DARK;
    board[4][3] = square_state::DARK;
    board[4][4] = square_state::LIGHT;
    

    game();

    return 0;
}