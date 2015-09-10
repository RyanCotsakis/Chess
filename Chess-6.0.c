#include <stdlib.h>
#include <stdio.h>

#define BP -1
#define BR -2
#define BN -3
#define BB -4
#define BQ -5
#define BK -6
#define WP 1
#define WR 2
#define WN 3
#define WB 4
#define WQ 5
#define WK 6
#define E 0

#define MOVELIMIT 5 //{3,5}
#define SENTINEL 3142

//OFFICIAL SETUP:
int officialBoard[8][8]={{BR,BN,BB,BQ,BK,BB,BN,BR},{BP,BP,BP,BP,BP,BP,BP,BP},
	{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},
	{WP,WP,WP,WP,WP,WP,WP,WP},{WR,WN,WB,WQ,WK,WB,WN,WR}};

int oldBoard[8][8]={{BR,BN,BB,BQ,BK,BB,BN,BR},{BP,BP,BP,BP,BP,BP,BP,BP},
	{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},{E,E,E,E,E,E,E,E},
	{WP,WP,WP,WP,WP,WP,WP,WP},{WR,WN,WB,E,WK,WB,WN,WR}};
	
int board[8][8];

int movePiece(int row, int col, int skipto);
void promptMove(void);//:goes to official board
int evalBoard(void);


int main(void){
	int count, row[MOVELIMIT], col[MOVELIMIT], moveNumber[MOVELIMIT], scoreToBeat[4][MOVELIMIT], score, i, j, move;

	printf("Welcome to Chess-6.0!\nIf you would like to move your pawn from A2 to A4, enter a2a4 as your move.\nTo castle, move your king to his end location.\nTo undo your last move, type UNDO.\nTo access these instructions during the game, type INFO.\nI think that is all... Good Luck!\n\n");

	while(1){
		promptMove();
		
		//setup "board":
		for(i=0; i<8; i++){
			for(j=0; j<8; j++)
				board[i][j]=officialBoard[i][j];
		}

		//everything 0:
		for(count=0;count<MOVELIMIT;count++){
			row[count]=0;
			col[count]=0;
			moveNumber[count]=0;
			scoreToBeat[0][count]=SENTINEL;
		}

		for(count=0;count>=0;count--){
			while(row[count]<8){
				while(col[count]<8){
					while(1){
						i=board[row[count]][col[count]];
						if((!(count%2) && i<E) || (count%2 && i>E)){
							move=movePiece(row[count],col[count],moveNumber[count]);
							if(move && count==MOVELIMIT-1){
								score=evalBoard();
								if(score<scoreToBeat[0][MOVELIMIT-1] || scoreToBeat[0][MOVELIMIT-1]==SENTINEL){
									scoreToBeat[0][MOVELIMIT-1]=score;
									scoreToBeat[1][MOVELIMIT-1]=row[0];
									scoreToBeat[2][MOVELIMIT-1]=col[0];
									scoreToBeat[3][MOVELIMIT-1]=moveNumber[0];
								}
								//reset board up to MOVELIMIT-2 and keep count at MOVELIMIT-1:
								for(i=0;i<8;i++){
									for(j=0;j<8;j++)
										board[i][j]=officialBoard[i][j];
								}
								for(i=0;i<MOVELIMIT-1;i++)
									movePiece(row[i],col[i],moveNumber[i]);
								moveNumber[count]++;
							}
							else if(move){
								count++;
								row[count]=0;
								col[count]=0;
								moveNumber[count]=0;
							}
							else
								break;
						}
						else
							break;
					}
					col[count]++;
					moveNumber[count]=0;
				}
				row[count]++;
				col[count]=0;
				moveNumber[count]=0;
			}

			//reset board up to count-2... idk but dont change count:
			for(i=0;i<8;i++){
				for(j=0;j<8;j++)
					board[i][j]=officialBoard[i][j];
			}
			for(i=0;i<count-1;i++)
				movePiece(row[i],col[i],moveNumber[i]);

			//do some best move sheit:
			if((!(count%2) && count && scoreToBeat[0][count-1]<scoreToBeat[0][count]) || (count%2 && scoreToBeat[0][count-1]>scoreToBeat[0][count]) || (scoreToBeat[0][count-1]==SENTINEL && count)){//count is even || odd || count-1 is emptcol
				for(i=0;i<4;i++)
					scoreToBeat[i][count-1]=scoreToBeat[i][count];
			}
			scoreToBeat[0][count]=SENTINEL;

			if(count)
				moveNumber[count-1]++;
		}//:end of count loop

		//make move on official board:
		movePiece(scoreToBeat[1][0],scoreToBeat[2][0],scoreToBeat[3][0]);
		for(i=0;i<8;i++){
			for(j=0;j<8;j++)
				officialBoard[i][j]=board[i][j];
		}

		for(i=0;i<8;i++){
			for(j=0;j<8;j++)
				printf("%2i ",officialBoard[i][j]);
			printf("\n");
		}
		printf("\n");
	}//:end of game
	return 0;
}

//returns 1 if successful, else 0:
int movePiece(int row, int col, int skipto){
	int i, moveNum=0;

	if(board[row][col]<E){
		if(board[row][col]==BP){
			if(row==6){
				if(board[7][col]==E){
					if(moveNum==skipto){
						board[7][col]=BQ;
						board[6][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=7 && board[7][col+1]>E){
					if(moveNum==skipto){
						board[7][col+1]=BQ;
						board[6][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=0 && board[7][col-1]>E && moveNum==skipto){
					board[7][col-1]=BQ;
					board[6][col]=E;
					return 1;
				}
			}
			else{
				if(board[row+1][col]==E){
					if(moveNum==skipto){
						board[row+1][col]=BP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=7 && board[row+1][col+1]>E){
					if(moveNum==skipto){
						board[row+1][col+1]=BP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=0 && board[row+1][col-1]>E){
					if(moveNum==skipto){
						board[row+1][col-1]=BP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(row==1 && board[3][col]==E && board[2][col]==E && moveNum==skipto){
					board[3][col]=BP;
					board[1][col]=E;
					return 1;
				}
			}
			return 0;
		}//BLACK PAWN
		
		if(board[row][col]==BR){
			i=1;
			//move down:
			while(board[row+i][col]==E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col]>E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move up:
			while(board[row-i][col]==E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col]>E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right:
			while(board[row][col+i]==E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col+i]>E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left:
			while(board[row][col-i]==E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col-i]>E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=BR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//BLACK ROOK

		if(board[row][col]==BB){
			i=1;
			//move right-down:
			while(board[row+i][col+i]==E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col+i]>E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-up:
			while(board[row-i][col+i]==E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col+i]>E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-down:
			while(board[row+i][col-i]==E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col-i]>E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-up:
			while(board[row-i][col-i]==E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col-i]>E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=BB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//BLACK BISHOP

		if(board[row][col]==BQ){
			i=1;
			//move down:
			while(board[row+i][col]==E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col]>E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move up:
			while(board[row-i][col]==E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col]>E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right:
			while(board[row][col+i]==E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col+i]>E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left:
			while(board[row][col-i]==E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col-i]>E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-down:
			while(board[row+i][col+i]==E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col+i]>E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-up:
			while(board[row-i][col+i]==E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col+i]>E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-down:
			while(board[row+i][col-i]==E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col-i]>E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}	
			i=1;
			//move left-up:
			while(board[row-i][col-i]==E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col-i]>E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=BQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//BLACK QUEEN

		if(board[row][col]==BK){
			//move down
			if(board[row+1][col]>=E && row<7){
				if(moveNum==skipto){
					board[row+1][col]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up
			if(board[row-1][col]>=E && row>0){
				if(moveNum==skipto){
					board[row-1][col]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right
			if(board[row][col+1]>=E && col<7){
				if(moveNum==skipto){
					board[row][col+1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left
			if(board[row][col-1]>=E && col>0){
				if(moveNum==skipto){
					board[row][col-1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right-down
			if(board[row+1][col+1]>=E && col<7 && row<7){
				if(moveNum==skipto){
					board[row+1][col+1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right-up
			if(board[row-1][col+1]>=E && col<7 && row>0){
				if(moveNum==skipto){
					board[row-1][col+1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left down
			if(board[row+1][col-1]>=E && col>0 && row<7){
				if(moveNum==skipto){
					board[row+1][col-1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left up
			if(board[row-1][col-1]>=E && col>0 && row>0){
				if(moveNum==skipto){
					board[row-1][col-1]=BK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//BLACK KING

		if(board[row][col]==BN){
			//move right then down
			if(board[row+1][col+2]>=E && row<7 && col<6){
				if(moveNum==skipto){
					board[row+1][col+2]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move down then right
			if(board[row+2][col+1]>=E && row<6 && col<7){
				if(moveNum==skipto){
					board[row+2][col+1]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right then up
			if(board[row-1][col+2]>=E && row>0 && col<6){
				if(moveNum==skipto){
					board[row-1][col+2]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up then right
			if(board[row-2][col+1]>=E && row>1 && col<7){
				if(moveNum==skipto){
					board[row-2][col+1]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left then down
			if(board[row+1][col-2]>=E && row<7 && col>1){
				if(moveNum==skipto){
					board[row+1][col-2]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move down then left
			if(board[row+2][col-1]>=E && row<6 && col>0){
				if(moveNum==skipto){
					board[row+2][col-1]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left then up
			if(board[row-1][col-2]>=E && row>0 && col>1){
				if(moveNum==skipto){
					board[row-1][col-2]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up then left
			if(board[row-2][col-1]>=E && row>1 && col>0){
				if(moveNum==skipto){
					board[row-2][col-1]=BN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//THE DARK KNIGHT

	}
	if(board[row][col]>E){

		if(board[row][col]==WP){
			if(row==1){
				if(board[0][col]==E){
					if(moveNum==skipto){
						board[0][col]=WQ;
						board[1][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=7 && board[0][col+1]<E){
					if(moveNum==skipto){
						board[0][col+1]=WQ;
						board[1][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=0 && board[0][col-1]<E && moveNum==skipto){
					board[0][col-1]=WQ;
					board[1][col]=E;
					return 1;
				}
			}
			else{
				if(board[row-1][col]==E){
					if(moveNum==skipto){
						board[row-1][col]=WP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=7 && board[row-1][col+1]<E){
					if(moveNum==skipto){
						board[row-1][col+1]=WP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(col!=0 && board[row-1][col-1]<E){
					if(moveNum==skipto){
						board[row-1][col-1]=WP;
						board[row][col]=E;
						return 1;
					}
					moveNum++;
				}
				if(row==6 && board[4][col]==E && board[5][col]==E && moveNum==skipto){
					board[4][col]=WP;
					board[6][col]=E;
					return 1;
				}
			}
			return 0;
		}//WHITE PAWN

		if(board[row][col]==WR){
			i=1;
			//move down:
			while(board[row+i][col]==E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col]<E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move up:
			while(board[row-i][col]==E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col]<E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right:
			while(board[row][col+i]==E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col+i]<E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left:
			while(board[row][col-i]==E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col-i]<E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=WR;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//WHITE ROOK

		if(board[row][col]==WB){
			i=1;
			//move right-down:
			while(board[row+i][col+i]==E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col+i]<E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-up:
			while(board[row-i][col+i]==E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col+i]<E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-down:
			while(board[row+i][col-i]==E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col-i]<E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-up:
			while(board[row-i][col-i]==E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col-i]<E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=WB;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//WHITE BISHOP

		if(board[row][col]==WQ){
			i=1;
			//move down:
			while(board[row+i][col]==E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col]<E && row+i<8){
				if(moveNum==skipto){
					board[row+i][col]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move up:
			while(board[row-i][col]==E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col]<E && row-i>=0){
				if(moveNum==skipto){
					board[row-i][col]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right:
			while(board[row][col+i]==E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col+i]<E && col+i<8){
				if(moveNum==skipto){
					board[row][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left:
			while(board[row][col-i]==E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row][col-i]<E && col-i>=0){
				if(moveNum==skipto){
					board[row][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-down:
			while(board[row+i][col+i]==E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col+i]<E && row+i<8 && col+i<8){
				if(moveNum==skipto){
					board[row+i][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move right-up:
			while(board[row-i][col+i]==E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col+i]<E && row-i>=0 && col+i<8){
				if(moveNum==skipto){
					board[row-i][col+i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			i=1;
			//move left-down:
			while(board[row+i][col-i]==E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row+i][col-i]<E && row+i<8 && col-i>=0){
				if(moveNum==skipto){
					board[row+i][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}	
			i=1;
			//move left-up:
			while(board[row-i][col-i]==E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
				i++;
			}
			if(board[row-i][col-i]<E && row-i>=0 && col-i>=0){
				if(moveNum==skipto){
					board[row-i][col-i]=WQ;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//WHITE QUEEN

		if(board[row][col]==WK){
			//move down
			if(board[row+1][col]<=E && row<7){
				if(moveNum==skipto){
					board[row+1][col]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up
			if(board[row-1][col]<=E && row>0){
				if(moveNum==skipto){
					board[row-1][col]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right
			if(board[row][col+1]<=E && col<7){
				if(moveNum==skipto){
					board[row][col+1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left
			if(board[row][col-1]<=E && col>0){
				if(moveNum==skipto){
					board[row][col-1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right-down
			if(board[row+1][col+1]<=E && col<7 && row<7){
				if(moveNum==skipto){
					board[row+1][col+1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right-up
			if(board[row-1][col+1]<=E && col<7 && row>0){
				if(moveNum==skipto){
					board[row-1][col+1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left down
			if(board[row+1][col-1]<=E && col>0 && row<7){
				if(moveNum==skipto){
					board[row+1][col-1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left up
			if(board[row-1][col-1]<=E && col>0 && row>0){
				if(moveNum==skipto){
					board[row-1][col-1]=WK;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//WHITE KING

		if(board[row][col]==WN){
			//move right then down
			if(board[row+1][col+2]<=E && row<7 && col<6){
				if(moveNum==skipto){
					board[row+1][col+2]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move down then right
			if(board[row+2][col+1]<=E && row<6 && col<7){
				if(moveNum==skipto){
					board[row+2][col+1]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move right then up
			if(board[row-1][col+2]<=E && row>0 && col<6){
				if(moveNum==skipto){
					board[row-1][col+2]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up then right
			if(board[row-2][col+1]<=E && row>1 && col<7){
				if(moveNum==skipto){
					board[row-2][col+1]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left then down
			if(board[row+1][col-2]<=E && row<7 && col>1){
				if(moveNum==skipto){
					board[row+1][col-2]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move down then left
			if(board[row+2][col-1]<=E && row<6 && col>0){
				if(moveNum==skipto){
					board[row+2][col-1]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move left then up
			if(board[row-1][col-2]<=E && row>0 && col>1){
				if(moveNum==skipto){
					board[row-1][col-2]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			//move up then left
			if(board[row-2][col-1]<=E && row>1 && col>0){
				if(moveNum==skipto){
					board[row-2][col-1]=WN;
					board[row][col]=E;
					return 1;
				}
				moveNum++;
			}
			return 0;
		}//WHIGHT KNIGHT

	}
	return 0;
}

int evalBoard(void){
	int row, col, score=0;

	for(row=0;row<8;row++){
		for(col=0;col<8;col++){
			if(board[row][col]>E){
				if(board[row][col]==WP){
					if(row==1)
						score+=20;
					else
						score+=10;
				}
				else if(board[row][col]==WR)
					score+=50;
				else if(board[row][col]==WB)
					score+=30;
				else if(board[row][col]==WN)
					score+=30;
				else if(board[row][col]==WQ)
					score+=80;
				else if(board[row][col]==WK)
					score+=1580;
			}
			else if(board[row][col]<E){
				if(board[row][col]==BP){
					if(row==6)
						score-=20;
					else
						score-=10;
				}
				else if(board[row][col]==BR)
					score-=50;
				else if(board[row][col]==BB){
					score-=30;
					if(row>1 && row<6 && col>1 && col<6)
						score-=1;
				}
				else if(board[row][col]==BN){
					score-=30;
					if(row>1 && row<6 && col>1 && col<6)
						score-=2;
				}
				else if(board[row][col]==BQ){
					score-=80;
					if(row>1 && row<6 && col>1 && col<6)
						score-=1;
				}
				else if(board[row][col]==BK)
					score-=1600;
			}
		}
	}
	return score;
}

void promptMove(void){
	char move[5];
	int row1, col1, row2, col2, newPiece, i, j;
	printf("\nWhite, it's your move.\n");
	scanf("%s",move);

	if(move[0]=='I' || move[0]=='i'){
		printf("\nIf you would like to move your pawn from A2 to A4, enter a2a4 as your move.\nTo castle, move your king to his end location.\nTo undo your last move, type UNDO.\n\n\nWhite, it's your move.\n");
		scanf("%s",move);
	}
	if(move[0]=='U' || move[0]=='u'){
		printf("\n");
		for(i=0;i<8;i++){
			for(j=0;j<8;j++){
				officialBoard[i][j]=oldBoard[i][j];
				printf("%2i ",officialBoard[i][j]);
			}
			printf("\n");
		}
		printf("\n\nWhite, it's your move. You cannot undo, or access INFO right now.\n");
		scanf("%s",move);
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++)
			oldBoard[i][j]=officialBoard[i][j];
	}

	if(officialBoard[7][4]==WK && move[0]=='e' && move[1]=='1' && move[2]=='g'){
		officialBoard[7][4]=E;
		officialBoard[7][5]=WR;
		officialBoard[7][6]=WK;
		officialBoard[7][7]=E;
	}
	else if(officialBoard[7][4]==WK && move[0]=='e' && move[1]=='1' && move[2]=='c'){
		officialBoard[7][4]=E;
		officialBoard[7][3]=WR;
		officialBoard[7][2]=WK;
		officialBoard[7][0]=E;
	}
	else{
		row1=56-move[1];
		col1=-97+move[0];
		row2=56-move[3];
		col2=-97+move[2];
		if(officialBoard[row1][col1]==WP && row2==0){
			printf("Please choose a piece by entering its corresponding number.\nWhich piece would you like?\nRook:   2\nKnight: 3\nBishop: 4\nQueen:  5\n");
			scanf("%i",&newPiece);
			officialBoard[0][col2]=newPiece;
		}
		else
			officialBoard[row2][col2]=officialBoard[row1][col1];
		officialBoard[row1][col1]=E;
	}
	printf("\nThinking...\n");
}