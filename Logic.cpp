#include "gomoku.hpp"

bool Gomoku::isOpenFourAt(int r, int c, int dr, int dc, Cell color){
    int startR = r;
    int startC = c;
    
    while(inBounds(startR - dr, startC - dc) && board[startR - dr][startC - dc] == color ) {
        startR -= dr;
        startC -= dc;
    }
    
    int count = 1 + countDirection(startR, startC, dr, dc, color);

    if(count != 4) {
        return false;
    }
    int beforeR = startR - dr;
    int beforeC = startC - dc;
    bool openBefore = inBounds(beforeR, beforeC) && board[beforeR][beforeC] == EMPTY;
    int afterR = startR + 4 * dr;
    int afterC = startC + 4 * dc;
    bool openAfter = inBounds(afterR, afterC) && board[afterR][afterC] == EMPTY;
    return openBefore && openAfter;
}

bool Gomoku::isFreeThreeAt(int r, int c, int dr, int dc, Cell color)  {
    int tr, tc;     
    
    for(int offset = -3; offset <= 3; offset++) {
        tr = r + (offset * dr);
        tc = c + (offset * dc);

        if (inBounds(tr, tc) && board[tr][tc] == EMPTY) {

            board[tr][tc] = color;
            
            if (isOpenFourAt(tr, tc, dr, dc, color)) {
                board[tr][tc] = EMPTY; 
                return true;
            }
            
            board[tr][tc] = EMPTY;
        }
    }    
    return false;
}

bool Gomoku::isDoubleThree(int r, int c, Cell color)  {
    board[r][c] = color;
    int count = 0;
    int dr[] = {0,1,1,1};
    int dc[] = {1,0,1,-1};
    for(int i = 0 ; i <4;i++)
    {
        if(isFreeThreeAt(r,c,dr[i],dc[i],color))
            count++;
        else if(isFreeThreeAt(r,c,-dr[i],-dc[i],color))  
            count++;
    }
    board[r][c] = EMPTY;
    if(count>1)
        return true;
    return false;
}

bool Gomoku::checkIfCorrectFive(int r , int c , Cell opponentColor,Cell winnerColor)
{
    board[r][c] = opponentColor;
    std::vector<Point> caps = checkCaptures(r, c, opponentColor);
    for(const auto &p : caps) {
        board[p.row][p.col] = EMPTY;
        }
            
    bool isBroken = !hasFive(winnerColor);
                
    for(const auto &p : caps) {
        board[p.row][p.col] = winnerColor;
    }         
    board[r][c] = EMPTY;
    return isBroken;
}

bool Gomoku::isFiveBreakable(Cell winnerColor){

    Cell opponentColor = opponent(winnerColor);
    for(int r = 0; r < BOARD_SIZE; r++) 
    {
        for(int c = 0 ; c < BOARD_SIZE;c++)
        {
            if(board[r][c] != EMPTY)
                continue;
            if(checkIfCorrectFive(r,c,opponentColor,winnerColor))
                return true;
        }
    }
    return false;
}