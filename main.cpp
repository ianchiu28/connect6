#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<conio.h>
#define M 19
int G[M][M]={0};
int ori_depth = 2;
int a = -999999;
int b =  999999;
int abx1,aby1,abx2,aby2;



//設定游標位置
void gotoxy(int xpos, int ypos)
{
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn); //設定游標位置
}

//設定顏色
void SetColor(int f=7,int b=0)
{
    unsigned short ForeColor=f+16*b;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);//取得目前文字視窗的輸出Handle
    SetConsoleTextAttribute(hCon,ForeColor);
}

//棋盤圖片&線路
void dir(int x,int y)
{
    gotoxy(x*2+6,y+3);
    if     (G[x][y]==1) { printf("●"); }
    else if(G[x][y]==2) { printf("○"); }
    else
    {
        if     (x == 0   && y == 0  ) { printf("┌"); }
        else if(x == 0   && y == M-1) { printf("└"); }
        else if(x == M-1 && y == 0  ) { printf("┐"); }
        else if(x == M-1 && y == M-1) { printf("┘"); }
        else if(x == 0  )             { printf("├"); }
        else if(x == M-1)             { printf("┤"); }
        else if(y == 0  )             { printf("┬"); }
        else if(y == M-1)             { printf("┴"); }
        else                          { printf("┼"); }
    }
}

bool victory(int color, int x, int y)
{
    // "|"
    int check = 0;
    for(int i=1;i<6;++i)
    {
        if(G[x][y-i] == color) { ++check; }
        else break;
    }
    for(int i=1;i<6;++i)
    {
        if(G[x][y+i] == color) { ++check; }
        else break;
    }
    if(check >= 5)
    {
        SetColor(0,8);
        dir(x,y);
        SetColor(15,0);
        gotoxy(56,5);
        if(color == 1) { printf("玩 家 獲勝!!"); }
        if(color == 2) { printf("電 腦 獲勝!!"); }
        return true;
    }

    // "-"
    check = 0;
    for(int i=1;i<6;++i)
    {
        if(G[x-i][y] == color) { ++check; }
        else break;
    }
    for(int i=1;i<6;++i)
    {
        if(G[x+i][y] == color) { ++check; }
        else break;
    }
    if(check >= 5)
    {
        SetColor(0,8);
        dir(x,y);
        SetColor(15,0);
        gotoxy(56,5);
        if(color == 1) { printf("玩 家 獲勝!!"); }
        if(color == 2) { printf("電 腦 獲勝!!"); }
        return true;
    }

    // "\"
    check = 0;
    for(int i=1;i<6;++i)
    {
        if(G[x-i][y-i] == color) { ++check; }
        else break;
    }
    for(int i=1;i<6;++i)
    {
        if(G[x+i][y+i] == color) { ++check; }
        else break;
    }
    if(check >= 5)
    {
        SetColor(0,8);
        dir(x,y);
        SetColor(15,0);
        gotoxy(56,5);
        if(color == 1) { printf("玩 家 獲勝!!"); }
        if(color == 2) { printf("電 腦 獲勝!!"); }
        return true;
    }

    // "/"
    check = 0;
    for(int i=1;i<6;++i)
    {
        if(G[x-i][y+i] == color) { ++check; }
        else break;
    }
    for(int i=1;i<6;++i)
    {
        if(G[x+i][y-i] == color) { ++check; }
        else break;
    }
    if(check >= 5)
    {
        SetColor(0,8);
        dir(x,y);
        SetColor(15,0);
        gotoxy(56,5);
        if(color == 1) { printf("玩 家 獲勝!!"); }
        if(color == 2) { printf("電 腦 獲勝!!"); }
        return true;
    }
    return false;
}


int evaluate(int color)
{
/*
棋型  我分  他分   圖

死二     0     0   @**_
斷二     0     0   @*_*_
眠二     2     4   _*_*_
活二     3     6   _**_

死三     0     3   @***_
斷三     0     5   @**_*_
眠三     5     9   _**_*_
活三     7    13   _***_

死四     2     5   @****_
斷四     1     6   @**_**_
眠四     3    13   _**_**_
活四     4    24   _****_

死五   306   106   @*****_
斷五   300   100   @***_**_
眠五   302   102   _***_**_
活五   330   130   _*****_

連六   900   200   ******
*/

    int s6 = 1000000,
        s5 = 4500,
        d5 = 2000,
        s4 = 4000,
        d4 = 2000,
        s3 = 1000,
        d3 = 300,
        s2 = 400,
        d2 = 100;

    int sum = 0;
    bool left,right;
    int enemy = 1;
    if(color == 1) enemy = 2;

    //*****me*****

    // "|"
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M;++j)
        {
            left = right = false;
            if(G[i][j] == color)
            {
                int k = 1;
                while(G[i][j+k] == color) ++k;
                if(j != 0 && G[i][j-1] != enemy) left = true;
                if(j+k != M && G[i][j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "-"
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M;++j)
        {
            left = right = false;
            if(G[j][i] == color)
            {
                int k = 1;
                while(G[j+k][i] == color) ++k;
                if(j != 0 && G[j-1][i] != enemy) left = true;
                if(j+k != M && G[j+k][i] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "\" bot part
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M && j+i<M;++j)
        {
            left = right = false;
            if(G[j][j+i] == color)
            {
                int k = 1;
                while(G[j+k][j+i+k] == color) ++k;
                if(j != 0 && G[j-1][j+i-1] != enemy) left = true;
                if(j+i+k != M && G[j+k][j+i+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
    for(int i=1;i<M;++i)
    {
        for(int j=0;j<M && j+i<M;++j)
        {
            left = right = false;
            if(G[j+i][j] == color)
            {
                int k = 1;
                while(G[j+i+k][j+k] == color) ++k;
                if(j != 0 && G[j+i-1][j-1] != enemy) left = true;
                if(j+i+k != M && G[j+i+k][j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "\" top part
    for(int i=M-1;i>=0;--i)
    {
        for(int j=0;j<M && i-j>=0;++j)
        {
            left = right = false;
            if(G[i-j][j] == color)
            {
                int k = 1;
                while(G[i-j-k][j+k] == color) ++k;
                if(j != 0 && G[i-j+1][j-1] != enemy) left = true;
                if(i-j-k+1 != 0 && G[i-j-k][j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
    for(int i=1;i<M;++i)
    {
        for(int j=M-1;j>=0 && i+18-j<M;--j)
        {
            left = right = false;
            if(G[j][i+18-j] == color)
            {
                int k = 1;
                while(G[j-k][i+18-j+k] == color) ++k;
                if(j != M-1 && G[j+1][i+18-j-1] != enemy) left = true;
                if(i+18-j+k != M && G[j-k][i+18-j+k] != enemy) right = true;
                switch(k)
                {
                case 6:
                    sum += s6; //連六
                    break;
                case 5:
                    if(left && right) sum += s5; //活五
                    else if(left || right) sum += d5; //死五
                    break;
                case 4:
                    if(left && right) sum += s4; //活四
                    else if(left || right) sum += d4; //死四
                    break;
                case 3:
                    if(left && right) sum += s3; //活三
                    else if(left || right) sum += d3; //死三
                    break;
                case 2:
                    if(left && right) sum += s2; //活二
                    else if(left || right) sum += d2; //死二
                    break;
                default:
                    if(k > 6) sum += s6; //連六
                    break;
                }
                j -= k;
            }
        }
    }

    //*****enemy*****

    // "|"
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M;++j)
        {
            left = right = false;
            if(G[i][j] == enemy)
            {
                int k = 1;
                while(G[i][j+k] == enemy) ++k;
                if(j != 0 && G[i][j-1] != color) left = true;
                if(j+k != M && G[i][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "-"
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M;++j)
        {
            left = right = false;
            if(G[j][i] == enemy)
            {
                int k = 1;
                while(G[j+k][i] == enemy) ++k;
                if(j != 0 && G[j-1][i] != color) left = true;
                if(j+k != M && G[j+k][i] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "\" bot part
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M && j+i<M;++j)
        {
            left = right = false;
            if(G[j][j+i] == enemy)
            {
                int k = 1;
                while(G[j+k][j+i+k] == enemy) ++k;
                if(j != 0 && G[j-1][j+i-1] != color) left = true;
                if(j+i+k != M && G[j+k][j+i+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
    for(int i=1;i<M;++i)
    {
        for(int j=0;j<M && j+i<M;++j)
        {
            left = right = false;
            if(G[j+i][j] == enemy)
            {
                int k = 1;
                while(G[j+i+k][j+k] == enemy) ++k;
                if(j != 0 && G[j+i-1][j-1] != color) left = true;
                if(j+i+k != M && G[j+i+k][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j += k;
            }
        }
    }

    // "\" top part
    for(int i=M-1;i>=0;--i)
    {
        for(int j=0;j<M && i-j>=0;++j)
        {
            left = right = false;
            if(G[i-j][j] == enemy)
            {
                int k = 1;
                while(G[i-j-k][j+k] == enemy) ++k;
                if(j != 0 && G[i-j+1][j-1] != color) left = true;
                if(i-j-k+1 != 0 && G[i-j-k][j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j += k;
            }
        }
    }
    // "\" top part
    for(int i=1;i<M;++i)
    {
        for(int j=M-1;j>=0 && i+18-j<M;--j)
        {
            left = right = false;
            if(G[j][i+18-j] == enemy)
            {
                int k = 1;
                while(G[j-k][i+18-j+k] == enemy) ++k;
                if(j != M-1 && G[j+1][i+18-j-1] != color) left = true;
                if(i+18-j+k != M && G[j-k][i+18-j+k] != color) right = true;
                switch(k)
                {
                case 6:
                    sum -= s6; //連六
                    break;
                case 5:
                    if(left && right) sum -= s5; //活五
                    else if(left || right) sum -= d5; //死五
                    break;
                case 4:
                    if(left && right) sum -= s4; //活四
                    else if(left || right) sum -= d4; //死四
                    break;
                case 3:
                    if(left && right) sum -= s3; //活三
                    else if(left || right) sum -= d3; //死三
                    break;
                case 2:
                    if(left && right) sum -= s2; //活二
                    else if(left || right) sum -= d2; //死二
                    break;
                default:
                    if(k > 6) sum -= s6; //連六
                    break;
                }
                j -= k;
            }
        }
    }

    return sum;
}

void mask()
{
    for(int i=0;i<M;++i)
    {
        for(int j=0;j<M;++j)
        {
            if(G[i][j] == 1 || G[i][j] == 2)
            {
                for(int k=1;k<3;++k)
                {
                    if(j-k>=0 &&           G[ i ][j-k] == 0) { G[ i ][j-k] = 7; } //上
                    if(j-k>=0 && i+k<M  && G[i+k][j-k] == 0) { G[i+k][j-k] = 7; } //右上
                    if(i+k<M  &&           G[i+k][ j ] == 0) { G[i+k][ j ] = 7; } //右
                    if(i+k<M  && j+k<M  && G[i+k][j+k] == 0) { G[i+k][j+k] = 7; } //右下
                    if(j+k<M  &&           G[ i ][j+k] == 0) { G[ i ][j+k] = 7; } //下
                    if(j+k<M  && i-k>=0 && G[i-k][j+k] == 0) { G[i-k][j+k] = 7; } //左下
                    if(i-k>=0 &&           G[i-k][ j ] == 0) { G[i-k][ j ] = 7; } //左
                    if(i-k>=0 && j-k>=0 && G[i-k][j-k] == 0) { G[i-k][j-k] = 7; } //左上
                }
            }
        }
    }
}

int alpha_beta_min(int alpha, int beta, int depth, int color);

int alpha_beta_max(int alpha, int beta, int depth, int color)
{
    int enemy = 1;
    if(color == 1) enemy = 2;
    int maxsc = -999999999;
    if(depth == 0) return evaluate(enemy);

    for(int i=0;i<M*M;++i)
    {
        if(G[i/M][i%M] == 7)
        {
            for(int j=i+1;j<M*M;++j)
            {
                if(G[j/M][j%M] == 7)
                {
                    G[i/M][i%M] = color;
                    G[j/M][j%M] = color;

                    /*gotoxy(0,30);
                    for(int i=0;i<M;++i)
                    {
                        for(int j=0;j<M;++j)
                        {
                            printf("%2d ",G[j][i]);
                        }
                        printf("\n");
                    }*/
                    int score = alpha_beta_min(alpha,beta,depth-1,color);
                    /*gotoxy(55,15);
                    printf("score:%d \n",score);
                    gotoxy(0,29);
                    printf("score:%d \n",score);*/
                    G[i/M][i%M] = 7;
                    G[j/M][j%M] = 7;
                    /*gotoxy(55,19);
                    printf("%d %d %d %d\n",i/M,i%M,j/M,j%M);
                    printf("\n\n\n\n\n\n\n\n\n\n\\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                    system("pause");*/

                    if(score >= beta) { return beta; }
                    if(score > alpha) { alpha = score; }
                    if(depth == ori_depth)
                    {
                        if(maxsc < score)
                        {
                            maxsc = score;
                            abx1 = i/M;
                            aby1 = i%M;
                            abx2 = j/M;
                            aby2 = j%M;
                        }
                    }
                }
            }
        }
    }

    return alpha;
}

int alpha_beta_min(int alpha, int beta, int depth, int color)
{
    int enemy = 1;
    if(color == 1) enemy = 2;
    if(depth == 0) return evaluate(color);

    for(int i=0;i<M*M;++i)
    {
        if(G[i/M][i%M] == 7)
        {
            for(int j=i+1;j<M*M;++j)
            {
                if(G[j/M][j%M] == 7)
                {
                    G[i/M][i%M] = enemy;
                    G[j/M][j%M] = enemy;
                    int score = alpha_beta_min(alpha,beta,depth-1,color);
                    G[i/M][i%M] = 7;
                    G[j/M][j%M] = 7;
                    /*gotoxy(55,20);
                    printf("%d %d %d %d\n",i/M,i%M,j/M,j%M);*/

                    if(score <= alpha) { return alpha; }
                    if(score < beta) { beta = score; }
                }
            }
        }
    }

    return beta;
}

//數字中文化
void nc(int n)
{
    char s1[10][5]={"  ","一","二","三","四","五","六","七","八","九"};

    if(n > 99) { printf("%s百",s1[n/100]); }
    n%=100;
    if(n > 19) { printf("%s",s1[n/10]); }
    if(n > 9)  { printf("十"); }
    n%=10;
    printf("%s",s1[n]);
}


int main()
{
Start:
    bool blackAI,whiteAI;
    int mode;
    do
    {
        system("cls");
        printf("Connect Six\n\n");
        printf("please choose a mode\n");
        printf("1. single player\n");
        printf("2. mutiple players\n");
        printf("3. AI battle\n");
        printf("...>");
        scanf("%d",&mode);
    }
    while(mode>3 || mode<1);
    system("cls");

    switch(mode)
    {
    case 1:
        do
        {
            system("cls");
            printf("you will pick...\n");
            printf("1. Black\n");
            printf("2. White\n");
            printf("...>");
            scanf("%d",&mode);
        }
        while(mode>2 || mode<1);
        if(mode == 1)
        {
            blackAI = false;
            whiteAI = true;
        }
        else
        {
            blackAI = true;
            whiteAI = false;
        }
        break;
    case 2:
        blackAI = false;
        whiteAI = false;
        break;
    case 3:
        blackAI = true;
        whiteAI = true;
    }

    system("cls");
    gotoxy(56,5);
    printf("               ");
    int i,j,l;
    int cMg;
    int nt=1;
    bool start_step = true;
    //畫棋盤
    for(i=0;i<M;i++)
    {
        for(j=0;j<M;j++)
        {
            G[i][j]=0;
            SetColor(0,15);
            dir(i,j);
        }

        SetColor(15,0);
        gotoxy(i*2+6,M+3);
        printf("%02d",i+1);
        gotoxy(M*2+1+6,i+3);
        printf("%c",'A'+i);
    }

    int key;
    int xi,yi;
    xi=yi=M/2;

    while(1)
    {
        SetColor(15,0);
        gotoxy(6,1);
        printf("六子棋實作");
        SetColor(15,0);
        gotoxy(55,3);
        printf("第");nc(nt);printf("回合    ");

        if(nt>=(M*M)/2)
        {
           SetColor(15,0);
           gotoxy(56,5);
           printf("平  手 !!");
        }

        nt++;

        //black
        if(blackAI)
        {
            if(start_step)
            {
                SetColor(0,15);
                G[xi][yi]=1;
                dir(xi,yi);
            }
            else
            {
                SetColor(0,15);
                mask();
                alpha_beta_max(a,b,ori_depth,1);
                G[abx1][aby1]=1;
                dir(abx1,aby1);
                if(victory(1,abx1,aby1)) break;
                G[abx2][aby2]=1;
                dir(abx2,aby2);
                if(victory(1,abx2,aby2)) break;
            }
        }
        else
        {
            //fisrt
            do{

                SetColor(0,8);dir(xi,yi);
                key=getch();

                SetColor(0,15);dir(xi,yi);

                        //輸入
                if(key==72){if(yi>0)yi--;   else yi=M-1; }   //上
                else if(key==80){if(yi<M-1)yi++; else yi=0;   }   //下
                else if(key==75){if(xi>0)xi--;   else xi=M-1; }   //左
                else if(key==77){if(xi<M-1)xi++; else xi=0;   }   //右
            }while(key!=13 || (G[xi][yi]!=0 && G[xi][yi]!=7)); // 選擇移動

            SetColor(0,15);
            G[xi][yi]=1;
            dir(xi,yi);

            if(victory(1,xi,yi)) break;

            //second
            if(!start_step)
            {
                do
                {
                    SetColor(0,8);dir(xi,yi);
                    key=getch();

                    SetColor(0,15);dir(xi,yi);

                            //輸入
                    if(key==72){if(yi>0)yi--;   else yi=M-1; }   //上
                    else if(key==80){if(yi<M-1)yi++; else yi=0;   }   //下
                    else if(key==75){if(xi>0)xi--;   else xi=M-1; }   //左
                    else if(key==77){if(xi<M-1)xi++; else xi=0;   }   //右
                }while(key!=13 || (G[xi][yi]!=0 && G[xi][yi]!=7)); // 選擇移動

                SetColor(0,15);
                G[xi][yi]=1;
                dir(xi,yi);

                if(victory(1,xi,yi)) break;
            }
        }

        //white
        if(whiteAI)
        {
            mask();
            alpha_beta_max(a,b,ori_depth,2);
            G[abx1][aby1]=2;
            dir(abx1,aby1);
            if(victory(2,abx1,aby1)) break;
            G[abx2][aby2]=2;
            dir(abx2,aby2);
            if(victory(2,abx2,aby2)) break;
        }
        else
        {
            //fisrt
            do{

                SetColor(0,8);dir(xi,yi);
                key=getch();

                SetColor(0,15);dir(xi,yi);

                        //輸入
                if(key==72){if(yi>0)yi--;   else yi=M-1; }   //上
                else if(key==80){if(yi<M-1)yi++; else yi=0;   }   //下
                else if(key==75){if(xi>0)xi--;   else xi=M-1; }   //左
                else if(key==77){if(xi<M-1)xi++; else xi=0;   }   //右
            }while(key!=13 || (G[xi][yi]!=0 && G[xi][yi]!=7)); // 選擇移動

            SetColor(0,15);
            G[xi][yi]=2;
            dir(xi,yi);

            if(victory(2,xi,yi)) break;

            //second
            do
            {
                SetColor(0,8);dir(xi,yi);
                key=getch();

                SetColor(0,15);dir(xi,yi);

                        //輸入
                if(key==72){if(yi>0)yi--;   else yi=M-1; }   //上
                else if(key==80){if(yi<M-1)yi++; else yi=0;   }   //下
                else if(key==75){if(xi>0)xi--;   else xi=M-1; }   //左
                else if(key==77){if(xi<M-1)xi++; else xi=0;   }   //右
            }while(key!=13 || (G[xi][yi]!=0 && G[xi][yi]!=7)); // 選擇移動

            SetColor(0,15);
            G[xi][yi]=2;
            dir(xi,yi);

            if(victory(2,xi,yi)) break;
        }

        start_step = false;
        Sleep(10);
    }//while(1)

    while(1)
    {
        SetColor(15,0);
        gotoxy(56,7);
        printf("是否繼續?!(Y/N)");
        key=getch();
        gotoxy(56,7);
        printf("               ");
        if(key=='n')break;
        else if(key=='y')goto Start;
    }
    return 0;
}
