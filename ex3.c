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

int isInBounds(int, int, int, int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, int, char);

int checkVictory(char[][COLS], int, int, char);//int, int, char);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int, char);

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int);

// place the choosen token in its free rows space at the choosen column
void initBoard(char[][COLS], int, int);

void printBoard(char[][COLS], int, int);

// my functions
void boardInitialize(char[][COLS], int, int);
void actPlayerTurn(char[][COLS], int, int, int, char, char);
void printHumanPrompt(char, int, int);
int humanValidationInput(int, char);
int diagonalVictoryCheck(char[][COLS], int, int, int, int, char);
int reverseDiagonalVictoryCheck(char[][COLS], int, int, int, char);

int getPlayerType(int);





//Global vars
int isGameOver=0;

int main() {
    char board[ROWS][COLS];
    boardInitialize(board, ROWS, COLS);
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    // Note computer side need to redefine!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
    // if (p1Type == COMPUTER || p2Type == COMPUTER) {    
    //     scanf(" %d", &columnNumber);
    //     printf("Enter column (1-%d):", columnNumber);
    // }
    // initBoard(board, ROWS, COLS);
    
    // printBoard(board, ROWS, COLS);
    // runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void runConnectFour(char board[][COLS], int rows, int columns, int p1Type, int p2Type){
    printBoard(board, rows, columns);
    // initialize the player first turn to 0
    int playerTurn = 0;
    while(!isGameOver){
        if(!playerTurn){
            actPlayerTurn(board, rows, columns, p1Type, TOKEN_P1, TOKEN_P2);
            playerTurn++;
        }
        else{
            actPlayerTurn(board, rows, columns, p2Type, TOKEN_P2, TOKEN_P1);
            playerTurn--;
        }
    }
    // printf("Game Over\n"); and who won? or tie
}

void actPlayerTurn(char board[][COLS], int rows, int columns, int playerType, char playerToken, char opponentToken){
    int choosenColumn=-1;
    int isVictory=0;
    if(playerType == HUMAN){
        choosenColumn=humanChoose(board, columns, rows, playerToken);
        makeMove(board, columns, rows, choosenColumn, playerToken);
        printf("---------------\ncheck\n %c %d %d", opponentToken, rows, choosenColumn);
        
        isVictory=checkVictory(board, columns, rows, playerToken);
        if(!isVictory){
            if(isBoardFull(board, columns, rows)){
                printf("Board full and no winner. It's a tie!\n");
                isGameOver=1;
            }
        }
        else{
            isGameOver=1;
        }
    }
    else{
        // complete computer turn here
        // computerChoose(board, columns, rows, playerToken, opponentToken);
        // makeMove(board, columns, rows, choosenColumn, playerToken);
        // isVictory=checkVictory(board, columns, rows, playerToken);
        // if(!isVictory){
        //     if(isBoardFull(board, columns, rows)){
        //         printf("Board full and no winner. It's a tie!\n");
        //         isGameOver=1;
        //     }
        // }
        // else{
        //     isGameOver=1;
        // }
    }
}

int humanChoose(char board[][COLS], int columns, int rows, char playerToken){
    int choosenColumn=-1;
    int result=0;
    int nextChar;

    // first prompt for human player (so give 1 as third arg)
    printHumanPrompt(playerToken, columns, 1);
    while(1){
        result = scanf(" %d", &choosenColumn);
        nextChar = getchar();
        if (humanValidationInput(result, nextChar)!= 1){
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }
 
        if(choosenColumn<1 || choosenColumn>columns){
            printf("Invalid column. Choose between 1 and %d.\n", columns);
            // not first try in turn anymore so give 0 as third arg
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }
        if(isColumnFull(board, rows, choosenColumn-1)){
            printf("Column %d is full. Choose another column.\n", choosenColumn);
            // not first try in turn anymore so give 0 as third arg
            printHumanPrompt(playerToken, columns, 0);
            continue;
        }

        printf("You chose column %d\n", choosenColumn);
        return (choosenColumn-1);
    }
    
}

// validate if the human input is really a number
int humanValidationInput(int choosenColumn, char nextChar){
    int result=0;
    if(nextChar == '\n'){
        result=choosenColumn;
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

int isColumnFull(char board[][COLS], int rows, int choosenColumn){ 
    for(int i=0; i<rows; i++){
        if(board[i][choosenColumn]==EMPTY){
            return 0;
        }
    }
    return 1;
}

int makeMove(char board[][COLS], int columns, int rows, int choosenColumn, char playerToken){
    int choosenRow=getFreeRow(board, columns, rows, choosenColumn);
    if(choosenRow!=-1){
        board[choosenRow][choosenColumn]=playerToken;
        printBoard(board, rows, columns);
        return choosenRow;
    }
    return -1;
}

int getFreeRow(char board[][COLS], int columns, int rows, int choosenColumn){
    if(isColumnFull(board, rows, choosenColumn)){
        return -1;
    }
    for(int i=rows-1; i>=0; i--){
        if(board[i][choosenColumn]==EMPTY){
            return i;
        }
    }
    return -1;
    printf("%d", columns);
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
                    printf("Player %c wins vertically!\n", playerToken);
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
                    printf("Player %c wins horizontally!\n", playerToken);
                    return 1;
                }
            }
            else{
                sameTokenCountInRow=0;
            }
        }
    }

    // diagonal check
    for (int c = 0; c < columns; c++) {
        if(diagonalVictoryCheck(board, columns, rows,c,0,playerToken)){
            return 1;
        }
        if(reverseDiagonalVictoryCheck(board, rows,c,0,playerToken)){
            return 1;
        }
    }
    for (int r = 1; r < rows; r++) {
        if(diagonalVictoryCheck(board, columns, rows,0,r,playerToken)){
            return 1;
        }
        if(reverseDiagonalVictoryCheck(board, rows,columns-1,r,playerToken)){
            return 1;
        }
    }
    return 0;
}

int diagonalVictoryCheck(char board[][COLS], int columns, int rows, int startColumn, int startRow, char playerToken){
    int sameTokenCountInDiagonal=0;
    for(int r=startRow, c=startColumn; r<rows && c<columns; r++, c++){
        if(board[r][c]==playerToken){
            sameTokenCountInDiagonal++;
            if(sameTokenCountInDiagonal==CONNECT_N){
                printf("Player %c wins diagonally (left to right)!\n", playerToken);
                return 1;
            }
        }
        else{
            sameTokenCountInDiagonal=0;
        }
    }
    return 0;
}

int reverseDiagonalVictoryCheck(char board[][COLS], int rows, 
    int startColumn, int startRow, char playerToken)
{
    int sameTokenCountInDiagonal=0;
    for(int r=startRow, c=startColumn; r<rows && c>=0; r++, c--){
        if(board[r][c]==playerToken){
            sameTokenCountInDiagonal++;
            if(sameTokenCountInDiagonal==CONNECT_N){
                printf("Player %c wins diagonally (right to left)!\n", playerToken);
                return 1;
            }
        }
        else{
            sameTokenCountInDiagonal=0;
        }
    }
    return 0;
}

// int initBoard(char board[][COLS], int p1Type, int p2Type){
 
// }
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
