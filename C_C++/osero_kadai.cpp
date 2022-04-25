#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;

//0:���[�U�[�A1:AI
int turn;

//�{�[�h�̏c���̑傫��
const int board_size = 8;
//�}�X�̏��
enum class square_state { EMPTY, DARK, LIGHT };
//�P�s���̃f�[�^��\���^
typedef vector<square_state> board_line;
//�I�Z���̃Q�[���Ղ�\���^
typedef vector<board_line> othello_board;

//�����̋�
square_state my_side;
//����̋�
square_state enemy;

//�Q�[���Ղ�\���ϐ�
//�}�X��8�~8�񕪗p�ӂ��A����͋󂯂Ă����B
othello_board board(board_size, board_line(board_size, square_state::EMPTY));
//            �ϐ����@�{�[�h�̎��@�@�@�@�@�@�@�@�@�{�[�h�̎����܂߂� �S������ɂ���

//pair�͂Q�̒l�����R���e�i
//������\���^�i�s�E��j
typedef pair<int, int> direction;

//�萔�̂Ƃ��́A�x�N�^�ł͂Ȃ��z��ɂ���
const direction direction_list[] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    { 0, -1},          { 0, 1},
    { 1, -1}, { 1, 0}, { 1, 1}
};

//���Ԃ�������ۑ�����R���e�i
vector<int> rev_list = {};

void disp(vector<vector<square_state>> board_disp) {
    //�ꏊ�w��̂��߂̎w�W
    cout << "  . a. b. c. d. e. f. g. h." << endl;
    for (size_t line = 0; line < 8; line++) {
        for (size_t row = 0; row < 8; row++) {
            if (row == 0) {
                //�ꏊ�w��̂��߂̎w�W
                cout << line + 1 << " ";
            }
            //�\�����@
            switch (board[line][row])
            {
            //���̎�
            case square_state::LIGHT:
                cout << ".��";
                break;
            //���̎�
            case square_state::DARK:
                cout << ".��";
                break;
            //��̎�
            case square_state::EMPTY:
                cout << ".  ";
                break;
            default:
                break;
            }
            //�E�[�̕�
            if (row == 7) {
                cout << "." << endl;
            }
        }
    }
}

//
bool is_turnable(othello_board& board, int row, int col, square_state my_side, const direction& d) {
    //�u���}�X���󂢂Ă��邩�ǂ���
    if (board[row][col] == square_state::EMPTY) {
        //8���̂����ꂩ�̃}�X
        int tr = row + d.first;
        int tc = col + d.second;
        //board����O��Ă�����false
        if (tr < 0 || tc < 0 || tr >= 8 || tc >= 8) {
            return false;
        }
        //�ׂ̃}�X�ɑ���̋���邩�ǂ���
        if (board[tr][tc] != my_side && board[tr][tc] != square_state::EMPTY) {
            //tr,tc���Ֆʓ��Ȃ�A���[�v����
            for (tr += d.first,tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                //�����̋�����
                if (board[tr][tc] == my_side) {
                    return true;
                }
                //��̎�
                else if (board[tr][tc] == square_state::EMPTY) {
                    return false;
                }
            }
        }
    }
    return false;
}

//row�scol��̈ʒu��my_side�̐΂��u���邩�H
bool is_puttable(othello_board& board, int row, int col, square_state my_side) {
    //�Ԃ�������direction_list��index���v�Z����
    int i = 0;
    //�e�����ɒu���邩�H
    //8�����̃}�X�������d�ɑ��
    for (const direction& d : direction_list) {
        //1�����ɂ����āA�G�R�}�̐�Ɏ��R�}������Ƃ�
        if (is_turnable(board, row, col, my_side, d)) {
            //�Ԃ�����������ۑ�
            rev_list.push_back(i);
        }
        i++;
    }
    //8�������ׂČ������I���āA���̃}�X�ɒu�����Ƃ��ł���Ȃ�
    if (rev_list.size() > 0) {
        return true; // �P�����ł����Ԃ�����u����.
    }
    //�ǂ̕��������Ԃ��Ȃ������B
    return false;
}

//�Տ��s�̐��𐔂���
int count_piece(const othello_board& board, square_state s) {
    int c = 0;
    for (auto it = board.begin() + 1; it != board.end() - 1; ++it) {
        //�P�s��������
        c += count(it->begin() + 1, it->end() - 1, s);
    }
    return c;
}
// it.begin()�@�́@it->begin()�Ə������Ƃ��o����

//�΂�u�����Ƃ��ɂЂ����肩�����֐�
//����board(��ʂ̏��), row(�u���s), col(�u����), my_side(�����̋�̐F), dir(�Ђ����肩��������)
void turn_over(othello_board& board,int row, int col, square_state my_side, vector<int> dir) {
    for (int i = 0; i < dir.size(); i++) {
        //row�̒l��ۑ�
        int row_temp = row;
        //col�̒l��ۑ�
        int col_temp = col;
        //�����̍��W�ړ���ۑ�
        auto reverce_dir = direction_list[dir[i]];
        cout << reverce_dir.first << endl;
        //�����̋����܂Ń��[�v
        for (row_temp += reverce_dir.first, col_temp += reverce_dir.second;
            board[row_temp][col_temp] != my_side;
            row_temp += reverce_dir.first, col_temp += reverce_dir.second) {
            //�����̋�ɕύX
            board[row_temp][col_temp] = my_side;
        }
    }
}

//��ԍ����A���t�@�x�b�g���琔���ɕϊ�
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
                //�e�����ɒu���邩�H
                //8�����̃}�X�������d�ɑ��
                for (const direction& d : direction_list) {

                    //8���̂����ꂩ�̃}�X
                    int tr = i + d.first;
                    int tc = j + d.second;
                    if (tr < 0 || tr > 7 || tc < 0 || tc > 7)continue;
                    //�ׂ̃}�X�ɑ���̋���邩�ǂ���
                    if (board[tr][tc] != turn_ && board[tr][tc] != square_state::EMPTY) {
                        //tr,tc���Ֆʓ��Ȃ�A���[�v����
                        for (tr += d.first, tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                            //�����̋�����
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

//AI���u����Ƃ����T��
pair<int, int> search_puttable_position() {
    //AI�̋�u����Ƃ����ۑ�
    pair<int, int> ai_posi;
    int ec = 1000;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //i,j��empty�̐���ۑ�
            int temp_ec = 0;
            //�Ђ����肩�������̎����empty�̍��W��ۑ�
            vector<pair<int, int> > empty_point;

            //�ꏊ���󂢂Ă����
            if (board[i][j] == square_state::EMPTY) {
                //�e�����ɒu���邩�H
                //8�����̃}�X�������d�ɑ��
                for (const direction& d : direction_list) {
                    //8���̂����ꂩ�̃}�X
                    int tr = i + d.first;
                    int tc = j + d.second;

                    //board�͈̔͊O�Ȃ�
                    if (tr < 0 || tr > 7 || tc < 0 || tc > 7)continue;//����𒆎~���A���̔����
                    //�ׂ̃}�X�ɑ���̋���邩�ǂ���
                    if (board[tr][tc] == my_side) {
                        //tr,tc���Ֆʓ��Ȃ�A���[�v����
                        for (tr += d.first, tc += d.second; tr >= 0 && tc >= 0 && tr < 8 && tc < 8; tr += d.first, tc += d.second) {
                            //�����̋�����
                            if (board[tr][tc] == enemy) {
                                //�u�������ʒu
                                int ttr = i;
                                int ttc = j;

                                //�u�������ʒu���玩���̋�ɂȂ�܂ŌJ��Ԃ�
                                for (ttr, ttc; board[ttr][ttc] != enemy; ttr += d.first, ttc += d.second) {
                                    //�S������empty�����邩������
                                    for (const direction& n : direction_list) {
                                        //�e�����̍��W��ۑ�
                                        int temp_ttr = ttr + n.first, temp_ttc = ttc + n.second;
                                        //board�͈̔͊O�Ȃ�
                                        if (temp_ttr < 0 || temp_ttr > 7 || temp_ttc < 0 || temp_ttc > 7)continue;//����𒆎~���A���̔����

                                        //���W�̒n�_��empty�Ȃ�
                                        if (board[temp_ttr][temp_ttc] == square_state::EMPTY) {
                                            //�܂�empty_point�ɍ��W���o�^����Ă��Ȃ�������
                                            if (empty_point.size() == 0) {
                                                empty_point.push_back({ temp_ttr,temp_ttc });
                                            }
                                            for (int k = 0; k < empty_point.size(); k++) {
                                                if (empty_point[k].first != temp_ttr && empty_point[k].second != temp_ttc) {
                                                    //�Ō�܂ō��v����v�f���������
                                                    if (k == empty_point.size() - 1) {
                                                        //���W��o�^
                                                        empty_point.push_back({ temp_ttr,temp_ttc });
                                                    }
                                                }
                                                //��������
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
                //empty�̐��𐔂��� -1�͒u�����Ƃ��Ă���ꏊ
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

        //���[�U�[�̔Ԃ̎�
        if (turn == 0) {
            //�u�����Ƃ��o���Ȃ�������
            if (puttable_area(board, my_side)) {
                cout << "\nPASS\n" << endl;
                turn = 1;
                rev_list = {};
                continue;
            }
            if (my_side == square_state::DARK) {
                cout << "\n       you are : " << "��\n" << endl;
            }
            else if (my_side == square_state::LIGHT) {
                cout << "\n       you are : " << "��\n" << endl;
            }
            //�s�ԍ�
            int row;
            //��ԍ�
            char alph;
            cout << "�u�����W���w�肵�Ă�������\n\n"
                "�c�̐�������͂��Ă������� = ";
            cin >> row;
            row--;
            cout << "����alphabet����͂��Ă������� = ";
            cin >> alph;
            //��ԍ��𐔎��ɕϊ�
            int col = culcu_cul(alph);

            //���u�����Ƃ��o����Ȃ�
            if (is_puttable(board, row, col, my_side)) {
                //�Ђ����肩����
                board[row][col] = my_side;
                //cout << rev_list.size() << "rev" << endl;
                turn_over(board, row, col, my_side, rev_list);
                //�Ђ����肩�������X�g�����
                rev_list = {};
            }
            else {
                cout << "���̃}�X�ɂ͒u���܂���\n" << endl;
                continue;
            }
        }
        else {
            //�u�����Ƃ��o���Ȃ�������
            if (puttable_area(board, enemy)) {
                cout << "\nPASS\n" << endl;
                turn = 0;
                rev_list = {};
                continue;
            }
            //if (enemy == square_state::DARK) {
            //    cout << "\n       now turn : " << "��\n" << endl;
            //}
            //else if (enemy == square_state::LIGHT) {
            //    cout << "\n       now turn : " << "��\n" << endl;
            //}
            ////�s�ԍ�
            //int row;
            ////��ԍ�
            //char alph;
            //cout << "�u�����W���w�肵�Ă�������\n"
            //    "num = ";
            //cin >> row;
            //row--;
            //cout << "alphabet = ";
            //cin >> alph;
            ////��ԍ��𐔎��ɕϊ�
            //int col = culcu_cul(alph);

            auto posi = search_puttable_position();
            int row = posi.first;
            int col = posi.second;

            //���u�����Ƃ��o����Ȃ�
            if (is_puttable(board, row, col, enemy)) {
                //�Ђ����肩����
                board[row][col] = enemy;
                cout << rev_list.size() << "rev" << endl;
                turn_over(board, row, col, enemy, rev_list);
                rev_list = {};
            }
            else {
                cout << "���̃}�X�ɂ͒u���܂���\n" << endl;
                continue;
            }

        }

        //���Ԃ𐧌�
        //0:���[�U�[�A1:AI
        if (turn == 0)turn = 1;
        else turn = 0;
        system("cls");
        disp(board);
        //�����̒u�����Ƃ��o���Ȃ�������
        if (puttable_area(board, enemy) && puttable_area(board, my_side)) {
            break;
        }
    }
    int dark = count_piece(board, square_state::DARK);
    int white = count_piece(board, square_state::LIGHT);
    cout << "�� : " << dark << endl;
    cout << "�� : " << white << endl;
    if (dark == white) {
        cout << "\n draw" << endl;
    }
    else if (dark > white) {
        cout << "\n �� win" << endl;
    }
    else {
        cout << " \n �� win" << endl;
    }
}

int main() {
    
    cout << "Game Start!!" << endl;
    cout << "�ǂ��炩�I��Ŕԍ�����͂��Ă�������\n"
        "��s : 0\n"
        "��U : 1" << endl;
    //��s����U��
    int my;
    //��U��U�̓���
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

    //�����z�u
    board[3][3] = square_state::LIGHT;
    board[3][4] = square_state::DARK;
    board[4][3] = square_state::DARK;
    board[4][4] = square_state::LIGHT;
    

    game();

    return 0;
}