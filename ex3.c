/*

*/
#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char[][COLS], int, int);

int isBoardFull(char[][COLS], int, int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, int, char);

int checkVictory(char[][COLS], int, int, char);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int, char);

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int);

void printBoard(char[][COLS], int, int);

void boardInitialize(char[][COLS], int, int);

void actPlayerTurn(char[][COLS], int, int, int, char, char);

void printHumanPrompt(char, int, int);

int humanValidationInput(int, char);

int diagonalVictoryCheck(char[][COLS], int, int, int, int, char);

int reverseDiagonalVictoryCheck(char[][COLS], int, int, int, char);

int distanceFromMiddle(int, int);

int checkStepsBeforeCanHappen(char[][COLS], int, int, int, char);

int possibleComputerMove(char[][COLS], int, int);

int getPlayerType(int);

int main() {
    char board[ROWS][COLS];
    boardInitialize(board, ROWS, COLS);
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void runConnectFour(char board[][COLS], int rows, int columns, int p1Type, int p2Type){
    // initialize the game over checking var
    int isGameOver=0;

    printBoard(board, rows, columns);
    
    // initialize the player first turn to 0
    int playerTurn = 0;
    
    // to be sure that we did clean the board right and its not full
    if(isBoardFull(board, columns, rows)){
        printf("Board full and no winner. It's a tie!\n");
        isGameOver=1;
    }

    while(!isGameOver){
        if(!playerTurn){
            isGameOver=actPlayerTurn(board, rows, columns, p1Type, TOKEN_P1, TOKEN_P2);
            playerTurn++;
        }
        else{
            isGameOver=actPlayerTurn(board, rows, columns, p2Type, TOKEN_P2, TOKEN_P1);
            playerTurn--;
        }
    }
}

int actPlayerTurn(char board[][COLS], int rows, int columns, int playerType, char playerToken, char opponentToken){
    int chosenColumn=-1;
    int isVictory=0;

    // human turn
    if(playerType == HUMAN){ 

        // find human chosen column
        chosenColumn=humanChoose(board, columns, rows, playerToken);
        
        // if valid column chosen, make the move
        if(chosenColumn!=-1){
            makeMove(board, columns, rows, chosenColumn, playerToken);
            
            // check if human won
            isVictory=checkVictory(board, columns, rows, playerToken);
        }
        
        // check if human won
        if(!isVictory){
            if(isBoardFull(board, columns, rows)){
                printf("Board full and no winner. It's a tie!\n");
                return 1;
            }
        }
        else{
            printf("Player %c (%c) wins!\n", (playerToken == TOKEN_P1 ? '1' : '2'), playerToken);
            return 1;
        }
    }
    // computer turn
    else{
        // check if board full

        printf("Player %c (%c) turn.\n", (playerToken == TOKEN_P1 ? '1' : '2'), playerToken);

        // find computer chosen column
        chosenColumn=computerChoose(board, columns, rows, playerToken, opponentToken);
        
        printf("Computer chose column %d\n", chosenColumn + 1);

        // if valid column chosen, make the move
        if(chosenColumn!=-1){
            makeMove(board, columns, rows, chosenColumn, playerToken);
            isVictory=checkVictory(board, columns, rows, playerToken);
        }

        // check if computer won
        if(!isVictory){
            if(isBoardFull(board, columns, rows)){
                printf("Board full and no winner. It's a tie!\n");
                return 1;
            }
        }
        else{
            printf("Player %c (%c) wins!\n", (playerToken == TOKEN_P1 ? '1' : '2'), playerToken);
            return 1;
        }
    }
    return 0;
}

int humanChoose(char board[][COLS], int columns, int rows, char playerToken){
    int chosenColumn=-1;
    int result=0;
    int nextChar;

    // first prompt for human player (so give 1 as third arg)
    printHumanPrompt(playerToken, columns, 1);
    while(1){
        result = scanf(" %d", &chosenColumn);
        nextChar = getchar();
        if (humanValidationInput(result, nextChar)!= 1){
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }
 
        if(chosenColumn<1 || chosenColumn>columns){
            printf("Invalid column. Choose between 1 and %d.\n", columns);
            // not first try in turn anymore so give 0 as third arg
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }
        if(isColumnFull(board, rows, chosenColumn-1)){
            printf("Column %d is full. Choose another column.\n", chosenColumn);
            // not first try in turn anymore so give 0 as third arg
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }

        printf("You chose column %d\n", chosenColumn);
        
        // return the chosen column (0-based)
        return (chosenColumn-1);
    }
    return -1;
}

// validate if the human input is really a number
int humanValidationInput(int chosenColumn, char nextChar){
    int result=0;
    if(nextChar == '\n'){
        result=chosenColumn;
    }
    if (result!= 1 || nextChar != '\n'){
        printf("Invalid input. Enter a number.\n");
        while (getchar() != '\n'); // clear input buffer
    }
    return result;
}

// print the prompt for human player (player 1 or player 2)
void printHumanPrompt(char playerToken, int columns, int firstTryInTurn){
    if (playerToken == TOKEN_P1){
        if(firstTryInTurn){
            printf("Player 1 (%c) turn.\n", playerToken);
        }
        printf("Enter column (1-%d):", columns);
    }
    else{
        if(firstTryInTurn){
            printf("Player 2 (%c) turn.\n", playerToken);
        }
        
        printf("Enter column (1-%d):", columns);
    }
}

int isColumnFull(char board[][COLS], int rows, int chosenColumn){ 
    for(int i=0; i<rows; i++){
        if(board[i][chosenColumn]==EMPTY){
            return 0;
        }
    }
    return 1;
}

int makeMove(char board[][COLS], int columns, int rows, int chosenColumn, char playerToken){
    int chosenRow=getFreeRow(board, rows, chosenColumn);
    if(chosenRow!=-1){
        board[chosenRow][chosenColumn]=playerToken;
        printBoard(board, rows, columns);
        return chosenRow;
    }
    return -1;
}

int getFreeRow(char board[][COLS], int rows, int chosenColumn){
    if(isColumnFull(board, rows, chosenColumn)){
        return -1;
    }
    for(int i=rows-1; i>=0; i--){
        if(board[i][chosenColumn]==EMPTY){
            return i;
        }
    }
    return -1;
}

int isBoardFull(char board[][COLS], int columns, int rows){
    for(int i=0; i<columns; i++){
        if(!isColumnFull(board, rows, i)){
            return 0;
        }
    }
    return 1;
}

int checkVictory(char board[][COLS], int columns, int rows, char playerToken){
    // vertically check
    for(int c=0; c<columns; c++){
        int sameTokenCountInColumn=0;
        for(int r=0; r<rows; r++){
            if(board[r][c]==playerToken){
                sameTokenCountInColumn++;
                if(sameTokenCountInColumn==CONNECT_N){
                    return 1;
                }
            }
            else{
                sameTokenCountInColumn=0;
            }
        }
    }   
    // horizontally check
    for(int r=0; r<rows; r++){
        int sameTokenCountInRow=0;
        for(int c=0; c<columns; c++){
            if(board[r][c]==playerToken){
                sameTokenCountInRow++;
                if(sameTokenCountInRow==CONNECT_N){
                    return 1;
                }
            }
            else{
                sameTokenCountInRow=0;
            }
        }
    }

    // diagonal check
    
    // Check diagonals starting from the top row (row 0) for each column
    for (int c = 0; c < columns; c++) {
        // Check main diagonal (\) starting at (0, c) moving down-right
        if(diagonalVictoryCheck(board, columns, rows,c,0,playerToken)){
            return 1;
        }
        // Check reverse diagonal (/) starting at (0, c) moving down-left
        if(reverseDiagonalVictoryCheck(board, rows,c,0,playerToken)){
            return 1;
        }
    }
    // Check diagonals starting from the side edges (from row 1 downwards)
    for (int r = 1; r < rows; r++) {
        // Check main diagonal (\) starting at the left edge (col 0) moving down-right
        if(diagonalVictoryCheck(board, columns, rows,0,r,playerToken)){
            return 1;
        }
        // Check reverse diagonal (/) starting at the right edge (last col) moving down-left
        if(reverseDiagonalVictoryCheck(board, rows,columns-1,r,playerToken)){
            return 1;
        }
    }
    return 0;
}

// Check diagonal (\)
int diagonalVictoryCheck(char board[][COLS], int columns, int rows, int startColumn, int startRow, char playerToken){
    int sameTokenCountInDiagonal=0;
    for(int r=startRow, c=startColumn; r<rows && c<columns; r++, c++){
        if(board[r][c]==playerToken){
            sameTokenCountInDiagonal++;
            if(sameTokenCountInDiagonal==CONNECT_N){
                return 1;
            }
        }
        else{
            sameTokenCountInDiagonal=0;
        }
    }
    return 0;
}

// Check diagonal (/)
int reverseDiagonalVictoryCheck(char board[][COLS], int rows, 
    int startColumn, int startRow, char playerToken)
{
    int sameTokenCountInDiagonal=0;
    for(int r=startRow, c=startColumn; r<rows && c>=0; r++, c--){
        if(board[r][c]==playerToken){
            sameTokenCountInDiagonal++;
            if(sameTokenCountInDiagonal==CONNECT_N){
                return 1;
            }
        }
        else{
            sameTokenCountInDiagonal=0;
        }
    }
    return 0;
}

int computerChoose(char board[][COLS], int columns, int rows, char playerToken, char opponentToken){
    int moveOption=-1;
    for(int i=1; i<=CONNECT_N-2; i++){
        // check if computer can create sequance of CONNECT_N-i in next move
        moveOption=checkStepsBeforeCanHappen(board, columns, rows, i, playerToken);
        if(moveOption!=-1){
            return moveOption;
        }
        // then check if opponent can create sequance of CONNECT_N-i in next move, and block it
        moveOption=checkStepsBeforeCanHappen(board, columns, rows, i, opponentToken);
        if(moveOption!=-1){
            return moveOption;
        }
    }

    // else, choose possible move
    moveOption=possibleComputerMove(board, columns, rows);
    if(moveOption!=-1){
        return moveOption;
    }
    return moveOption;
}

/* Checks if placing 'token' creates a sequence of 
size (CONNECT_N - stepAmountBefore) and returns the best column*/
int checkStepsBeforeCanHappen(char board[][COLS], int columns, int rows, int stepAmountBefore, char token){

    // initialize variables
    int minDistanceToMiddle = columns + 1; // set to max possible distance +1(thus any distance will be smaller)
    int bestPossibleMoveColumn = -1;
    int wantedTokenValue = CONNECT_N - stepAmountBefore;

    // run through all columns
    for(int c=0; c < columns; c++){
        // initialize variables for each column
        int isFoundPossibleMove = 0;
        int rowIndexOption = -1;
        
        // if column is full continue to next column
        rowIndexOption = getFreeRow(board, rows, c);
        if(rowIndexOption == -1){
            continue;
        }
        
        // vertically check
        int sameTokenCountInColumn = 0;
        for(int r=rowIndexOption + 1; r < rows; r++){
            // check how many same tokens are in column
            if(board[r][c]==token){
                sameTokenCountInColumn++;
            }
            else{
                break;
            }
        }

        // if enough same tokens found in column
        if(sameTokenCountInColumn>=wantedTokenValue){
            isFoundPossibleMove = 1;
        }
        
        // horizontally check
        if(!isFoundPossibleMove){
            int sameTokenCountInRow=0;
            
            // check right side
            for(int i=c + 1; i < columns; i++){
                // check how many same tokens are in row
                if(board[rowIndexOption][i]==token){
                    sameTokenCountInRow++;
                }
                else{
                    break;
                }
            }
            
            // check left side
            for(int i = c-1; i >= 0; i--){
                // check how many same tokens are in row
                if(board[rowIndexOption][i]==token){
                    sameTokenCountInRow++;
                }
                else{
                    break;
                }
            }

            // if enough same tokens found in row
            if(sameTokenCountInRow>=wantedTokenValue){
                // possibleMoveColumn = c;
                isFoundPossibleMove = 1;
            }   
        }
        
        // diagonal check
        
        int sameTokenCountInDiagonal=0;
        
        // check right down and left up diagonal
        if(!isFoundPossibleMove){    
            
            /* check right down diagonal
            do row +1 and col +1 because we check from the free row position */
            for (int i = rowIndexOption + 1, j = c + 1; i < rows && j < columns; i++, j++) {
                
                // check how many same tokens are in diagonal
                if (board[i][j] == token){
                    sameTokenCountInDiagonal++;       
                } 
                else{
                    break;
                }
            }

            /* check left up diagonal
            do row -1 and col -1 because we check from the free row position*/
            for (int i = rowIndexOption - 1, j = c - 1; i >=0 && j >=0; i--, j--) {
                
                // check how many same tokens are in diagonal
                if (board[i][j] == token){
                    sameTokenCountInDiagonal++;
                }
                else{
                    break;
                }   
            }

            // if enough same tokens found in diagonal
            if(sameTokenCountInDiagonal>=wantedTokenValue){
                isFoundPossibleMove = 1;
            }
        }
        
        // reverse diagonal check
        if(!isFoundPossibleMove){    
            int sameTokenCountInDiagonal=0;

            /* check left down diagonal
            do row +1 and col -1 because we check from the free row position */
            for (int i = rowIndexOption + 1, j = c - 1; i < rows && j >=0; i++, j--) {
                
                // check how many same tokens are in diagonal
                if (board[i][j] == token){
                    sameTokenCountInDiagonal++;
                } 
                else{
                    break;
                }
            }

            /* check right up diagonal 
            do row -1 and col +1 because we check from the free row position*/
            for (int i = rowIndexOption - 1, j = c + 1; i >=0 && j < columns; i--, j++) {
                
                // check how many same tokens are in diagonal
                if (board[i][j] == token){
                    sameTokenCountInDiagonal++;
                } 
                else{
                    break;
                }
            }

            // if enough same tokens found in diagonal
            if(sameTokenCountInDiagonal>=wantedTokenValue){
                isFoundPossibleMove = 1;
            }
        }
        // if possible move found, check if it's the best one (closest to middle)
        if(isFoundPossibleMove){
            int currentDistanceToMiddle=distanceFromMiddle(columns, c);
            // if first possible move found, set it as best
            if(bestPossibleMoveColumn==-1 || currentDistanceToMiddle<minDistanceToMiddle){
                bestPossibleMoveColumn=c;
                minDistanceToMiddle=currentDistanceToMiddle;
            }
        }

    }
    return bestPossibleMoveColumn;
}

// calculate the distance of the possible move column from the middle column
int distanceFromMiddle(int columns, int possibleMoveColumn){
    
    /* for not dealing with even division in int ((columns-1)/2)
    we multiplying by 2 the chosen column, using that we get the middle of the columns like it the end
    because of that if we do (3*2)-6 we get that the destance from the middle is 0
    and it keeping perfect symmetry for whole other numbers*/
    int distance=2*possibleMoveColumn-(columns - 1);

    // if negative distance, get the absolute value
    if(distance<0){
        distance= -distance;
    }
    return distance;
}

// find possible move for computer when no immediate win or block needed
int possibleComputerMove(char board[][COLS], int columns, int rows){
    // initialize variables
    int possibleMoveColumn=-1;
    int minDistanceToMiddle=columns+1;
    
    // run through all columns to find possible move
    for(int c=0; c<columns; c++){
        if(isColumnFull(board, rows, c)){
            continue;
        }
        
        int currentDistanceToMiddle=distanceFromMiddle(columns, c);
        
        // if closer to middle, update possible move
        if(currentDistanceToMiddle<minDistanceToMiddle){
            minDistanceToMiddle=currentDistanceToMiddle;
            possibleMoveColumn=c;
        }
    }

    return possibleMoveColumn;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

// initialize the boards places for fresh start ('.' in all of them)
void boardInitialize(char board[][COLS], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            board[r][c] = EMPTY;
        }
    }
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}
