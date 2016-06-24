#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdlib.h>
#include <sstream> 
#include <stdio.h> 
using namespace std;
using namespace cv;
int X1=0,X2=0,Y1=0,Y2=0;
int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
int getmin (int a, int b)
{
  if (a < b)
    return a;
  return b;
}

int
getmax (int a, int b)
{
  if (a > b)
    return a;
  return b;
}

int
mod (int a)
{
  if (a > 0)
    return a;
  return -a;
}

int getscore(int **);
void play (int **);
void PrintBoard (int **);
void cpy(int **,int **);

struct list
{
  int x;
  int y;
  list*next;
};

list *posmoves(int **,int ,int ,list *);
int **makeBoard(char *board_shit) {
		int **bd = (int**) malloc(sizeof(int *) * 8); 
		for(int i=0; i!=8; i++) {
				bd[i] = (int *) malloc(sizeof(int) * 8); 
				for(int j=0; j!=8; j++) {
						char key = board_shit[j*8+i]; 						
						//int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
						if(key=='K') bd[i][j] = K; 
						if(key=='k') bd[i][j] = -K; 
						if(key=='Q') bd[i][j] = Q; 
						if(key=='q') bd[i][j] = -Q; 
						if(key=='R') bd[i][j] = R; 
						if(key=='r') bd[i][j] = -R;	
						if(key=='B') bd[i][j] = B; 
						if(key=='b') bd[i][j] = -B;
						if(key=='N') bd[i][j] = N; 
						if(key=='n') bd[i][j] = -N; 
						if(key=='P') bd[i][j] = P; 
						if(key=='p') bd[i][j] = -P; 
						if(key=='0') bd[i][j] = 0; 
					}
			}
			return bd; 
	}
	int
King (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) < 2 && mod (y1 - y2) < 2)
    {
      if (a[x1][y1] * a[x2][y2] > 0)
	{
	  return 0;
	}
    }
  else
    return 0;
  //  if(inCheck())
  //return 0;
  return 1;
}

int
Rook (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (x1 == x2 && mod (y1 - y2) > 0 || y1 == y2 && mod (x1 - x2) > 0);
  else
    return 0;
  if (x1 - x2 != 0)
    {
      int i, maX = getmax (x1, x2);
      for (i = getmin (x1, x2) + 1;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[i][y1] != 0)
	    return 0;
	}
    }
  else
    {
      int i = getmin (y1, y2) + 1, maX = getmax (y1, y2);
      for (;; i++)
	{
	  if (i == maX)
	    return 1;
	  if (a[x1][i] != 0)
	    {
	      return 0;
	    }
	}
    }
  return 1;
}

int
Knight (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  if (mod (x1 - x2) + mod (y1 - y2) == 3)
    if (mod (x1 - x2) == 1 || mod (x1 - x2) == 2)
      return 1;
  return 0;
}

int
Bishop (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (mod (x1 - x2) != mod (y1 - y2))
    return 0;
  if (a[x1][y1] * a[x2][y2] > 1)
    return 0;
  int i = 1, j = mod (x1 - x2);
  if (y1 < y2 && x1 < x2)
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 < y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 + i] != 0)
	  return 0;
    }
  else if (y1 > y2 && x1 > x2)
    {
      for (; i < j; i++)
	if (a[x1 - i][y1 - i] != 0)
	  return 0;
    }
  else
    {
      for (; i < j; i++)
	if (a[x1 + i][y1 - i] != 0)
	  return 0;
    }
  return 1;
}

int
Pawn (int move, int **a)
{
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  if (a[x1][y1] == -P)
    {
      if (x1 == 6 && x2 == 4 && a[x2][y2] == 0)
	return 1;
      if (!(x1 - x2 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  else
    {
      if (x1 == 1 && x2 == 3 && a[x2][y2] == 0)
	return 1;
      if (!(x2 - x1 == 1))
	return 0;
      if (mod (y1 - y2) == 1)
	{
	  if (a[x1][y1] * a[x2][y2] >= 0)
	    return 0;
	  else
	    return 1;
	}
      if (y1 == y2)
	{
	  if (a[x1][y1] * a[x2][y2] == 0)
	    return 1;
	  else
	    return 0;
	}
    }
  return 0;
}

int
isValidMove (int move, int **a)	//return 0 if invalid   if move=1234 it means move piece from 1,2 to 3,4
{
  int x1, y1, x2, y2, tmpmove = move;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  cout<<"  "<<x1+1<<" "<<y1+1<<" "<<x2+1<<" "<<y2+1<<"\n"<<a[x1][y1]<<" "<<a[x2][y2]<<"\n";
  if (mod (a[x1][y1]) == Q)
    return Bishop (tmpmove, a) || Rook (tmpmove, a);
  if (mod (a[x1][y1]) == K)
    return King (tmpmove, a);
  if (mod (a[x1][y1]) == P)
    return Pawn (tmpmove, a);
  if (mod (a[x1][y1]) == R)
    return Rook (tmpmove, a);
  if (mod (a[x1][y1]) == N)
    return Knight (tmpmove, a);
  if (mod (a[x1][y1]) == B)
    return Bishop (tmpmove, a);
  return 0;
}

void
moveIt (int move, int **a)
{
  
  int x1, y1, x2, y2;
  y2 = move % 10;
  move /= 10;
  x2 = move % 10;
  move /= 10;
  y1 = move % 10;
  move /= 10;
  x1 = move % 10;
  a[x2][y2] = a[x1][y1];
  a[x1][y1] = 0;
  PrintBoard (a);
  
}

void
PrintBoard (int **a)
{
  int i, j;
  for (i = 1; i < 9; i++)
    cout<<"   "<<i<<"   ";
  cout<<"\n";
  for (i = 0; i < 8; i++)
    {
      cout<<" ";
      if (i & 1)
	for (j = 0; j < 4; j++)
	  cout<<"******      ";
      else
	for (j = 0; j < 4; j++)
	  cout<<"      ******";
      cout<<"\n"<< i + 1;
      for (j = 0; j < 8; j++)
	{
	  if ((i + j) & 1)
	    cout<<"**";
	  else
	    cout<<"  ";
	  if (mod (a[i][j]) == K)
	    {
	      if (a[i][j] > 0)
		cout<<"WK";
	      else
		cout<<"BK";
	    }
	  else if (mod (a[i][j]) == Q)
	    {
	      if (a[i][j] > 0)
		cout<<"WQ";
	      else
		cout<<"BQ";
	    }
	  else if (mod (a[i][j]) == R)
	    {
	      if (a[i][j] > 0)
		cout<<"WR";
	      else
		cout<<"BR";
	    }
	  else if (mod (a[i][j]) == B)
	    {
	      if (a[i][j] > 0)
		cout<<"WB";
	      else
		cout<<"BB";
	    }
	  else if (mod (a[i][j]) == N)
	    {
	      if (a[i][j] > 0)
		cout<<"WN";
	      else
		cout<<"BN";
	    }
	  else if (mod (a[i][j]) == P)
	    {
	      if (a[i][j] > 0)
		cout<<"WP";
	      else
		cout<<"BP";
	    }
	  else if (a[i][j] == 0)
	    {
	      if ((j + i) & 1)
		cout<<"**";
	      else
		cout<<"  ";
	    }
	  if ((i + j) & 1)
	    cout<<"**";
	  else
	    cout<<"  ";
	}
      cout<<"\n ";
      if (i & 1)
	for (j = 0; j < 4; j++)
	  cout<<"******      ";
      else
	for (j = 0; j < 4; j++)
	  cout<<"      ******";
      cout<<"\n";
    }
}
int getbestmove(int **board,int turn,int score,int depth)
{
 int bestscore=-30000*turn;
 int t1,t2,bestt;
 list*move=NULL;
 if(depth==1)
 {
   for(int i=0 ; i<8 ; i++)
   {
	for(int j=0 ; j<8 ; j++)
	  {
  	   if(board[i][j]*turn>0)
	     {
		move=posmoves(board,i,j,move);
		list*first=move;
		int flag=0;
		while(move!=NULL)
		{
		  t1=board[i][j];
		  t2=board[move->x][move->y];
		  board[move->x][move->y]=board[i][j];
		  board[i][j]=0;
		//  bestt=bestscore;
		  if(turn>0)
		  bestt=getmax (bestscore, getscore(board) +getbestmove (board,-turn, bestscore, depth + 1));
		  if(turn<0)
		  bestt=getmin(bestscore,getscore(board)+getbestmove(board,-turn,bestscore,depth+1));
		  if((bestt>bestscore&&turn>0)||(bestt<bestscore&&turn<0))
			{
		  	 bestscore=bestt;
			  X1=i+1;
			 Y1=j+1;
			 X2=move->x+1;
			 Y2=move->y+1;
			}
		  board[i][j]=t1;
		  board[move->x][move->y]=t2;
		  first=move;
		  move=move->next;
		  free(first);
		}
	      }
	   }   
    }
 }
 else if(depth<Depth)
 {
  for(int i=0 ; i<8 ; i++)
  {
	for(int j=0 ; j<8 ; j++)
	{
	  if(board[i][j]*turn>0)
	   {
	    move=posmoves(board,i,j,move);
        list *first=move;
	    while(move!=NULL)
	    { 
		t1=board[i][j];
		t2=board[move->x][move->y];
		board[move->x][move->y]=board[i][j];
		board[i][j]=0;
		int newscore= (getscore(board)+getbestmove(board,-turn,bestscore,depth+1));
		if(newscore>bestscore&&turn==1)
		  bestscore=newscore;
		else if(newscore<bestscore&&turn==-1)
		  bestscore=newscore;
		board[i][j]=t1;
		board[move->x][move->y]=t2;
		first=move;
		move=move->next;
		free(first);
	    }
	   }
	}
  }
return bestscore;
}
else return 0;
return(1000*X1+100*Y1+10*X2+Y2);
}
void play(int **a)
{
int m=1;
while(m!=-1111)
{
  cout<<"Your move :  ";
  cin>>m;
  m-=1111;
  if(m==-1111)
	break;
  if(m<0||m>7777)
  {
   cout<<"Invalid Move !!\n";
   continue;
  }
  if(isValidMove(m,a))
   moveIt(m,a);
  else
  {
   cout<<"Invalid Move !!\n";
   continue;
  }
  do {
  m = getbestmove (a, 1,0, 1);
} while(isValidMove(m,a)==0); 
      if (m == 0)
	{
	  PrintBoard (a);
	  cout << "\ncontinue\n";
	  continue;
	}
      moveIt (m, a);
      PrintBoard(a);
      cout<<"My Move "<<m+1111<<"\n";
  }
	cout<<m<<endl; 
	cout<<"Ending play"<<endl; 
}
 

void cpy(int **a,int **b)
{
  for(int i=0 ; i<8 ; i++)
  {
    for(int j=0 ; j<8 ; j++)
    {
	a[i][j]=b[i][j];
    }
  }
}



int getscore(int **a)
{
  int s=0 ;
  for(int i=0 ; i<8 ; i++)
  {
     for(int j=0 ; j<8 ; j++)
     {
	s+=a[i][j];
     }
  }
return s;
}

list * pop(list*start,int i,int j)
{
  if(start==NULL)
  {
    list*tmp=(list*)malloc(sizeof(list));
    tmp->x=i;
    tmp->y=j;
    tmp->next=NULL;
    return tmp;
  }
  
  list*tmp=(list*)malloc(sizeof(list));
  tmp->next=start;
  tmp->x=i;
  tmp->y=j;
  start=tmp;
  return start; 
}

int
Encode (int i, int j, int k, int l)
{
  return 1000 * i + 100 * j + 10 * k + l;
}

list *WhereToMoveKing (int **a, int i, int j, list * move)
{
  int x, y;
  for (x = -1; x < 2; x++)
    {
      for (y = -1; y < 2; y++)
	{
	  if (x + i > -1 && x + i < 8 && j + y > -1 && j + y < 8)
	    {
	      if (King (Encode (i, j, i + x, j + y), a))
		{
		  move = pop (move,i+x,j+y);
		}
	    }
	}
    }
  return move;
}

list *WhereToMoveRook (int **a, int i, int j, list * move)
{
  int x = 1, count = 0;
  while (i - x > -1)
    {
      if (Rook (Encode (i, j, i - x, j), a))
	{
	  move = pop (move, i - x, j);
	  count++;
	}
      x++;
    }
  x = 1;
  while (i + x < 8)
    {
      if (Rook (Encode (i, j, i + x, j), a))
	{
	  move = pop (move,i + x, j);
	  count++;
	}
      x++;
    }
  x = 1;
  while (j - x > -1)
    {
      if (Rook (Encode (i, j, i, j - x), a))
	{
	  move = pop (move, i, j - x);
	  count++;
	}
      x++;
    }
  x = 1;
  while (j + x < 8)
    {
      if (Rook (Encode (i, j, i, j + x), a))
	{
	  move = pop (move,i, j + x);
	  count++;
	}
      x++;
    }
  return move;
}

list *
WhereToMoveKnight (int **a, int i, int j, list * move)
{
  int x, y;
  if (i - 2 > -1)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i - 2, j - 1), a))
	    {
	      move = pop (move, i - 2, j - 1);
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i - 2, j + 1), a))
	    {
	      move = pop (move,i - 2, j + 1);
	    }
	}
    }
  if (i + 2 < 8)
    {
      if (j - 1 > -1)
	{
	  if (Knight (Encode (i, j, i + 2, j - 1), a))
	    {
	      move = pop (move,i + 2, j - 1);
	    }
	}
      if (j + 1 < 8)
	{
	  if (Knight (Encode (i, j, i + 2, j + 1), a))
	    {
	      move = pop (move, i + 2, j + 1);
	    }
	}
    }
  if (i - 1 > -1)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i - 1, j - 2), a))
	    {
	      move = pop (move, i - 1, j - 2);
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i - 1, j + 2), a))
	    {
	      move = pop (move, i - 1, j + 2);
	    }
	}
    }
  if (i + 1 < 8)
    {
      if (j - 2 > -1)
	{
	  if (Knight (Encode (i, j, i + 1, j - 2), a))
	    {
	      move = pop (move, i + 1, j - 2);
	    }
	}
      if (j + 2 < 8)
	{
	  if (Knight (Encode (i, j, i + 1, j + 2), a))
	    {
	      move = pop (move, i + 1, j + 2);
	    }
	}
    }
  return move;
}

list *
WhereToMoveBishop (int **a, int i, int j, list * move)
{
  int x;
  for (x = 1; x < 8; x++)
    {
      if (i - x > -1 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i - x, j - x), a))
	    {
	      move = pop (move,i - x, j - x);
	    }
	}
      if (i - x > -1 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i - x, j + x), a))
	    {
	      move = pop (move, i - x, j + x);
	    }
	}
      if (i + x < 8 && j - x > -1)
	{
	  if (Bishop (Encode (i, j, i + x, j - x), a))
	    {
	      move = pop (move, i + x, j - x);
	    }
	}
      if (i + x < 8 && j + x < 8)
	{
	  if (Bishop (Encode (i, j, i + x, j + x), a))
	    {
	      move = pop (move,i + x, j + x);
	    }
	}
    }
  return move;
}

list *
WhereToMovePawn (int **a, int i, int j, list * move)
{
  if (a[i][j] < 0)  {
      if (j > 0 && i > 0) {
		if (Pawn (Encode (i, j, i - 1, j - 1), a))    {
	      move = pop (move, i - 1, j - 1);
	    }
	}
      if (i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j), a))
	    {
	      move = pop (move, i - 1, j);
	    }
	}
      if (i == 6)
	{
	  if (Pawn (Encode (i, j, i - 2, j), a))
	    {
	      move = pop (move, i - 2, j);
	    }
	}
      if (j < 7 && i > 0)
	{
	  if (Pawn (Encode (i, j, i - 1, j + 1), a))
	    {
	      move = pop (move, i - 1, j + 1);
	    }
	}
    }
  if (a[i][j] > 0)
    {
      if (j < 7 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j + 1), a))
	    {
	      move = pop (move, i + 1, j + 1);
	    }
	}
      if (i == 1)
	{
	  if (Pawn (Encode (i, j, i + 2, j), a))
	    move = pop (move,i + 2, j);
	}
      if (i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j), a))
	    move = pop (move, i + 1, j);
	}
      if (j > 0 && i < 7)
	{
	  if (Pawn (Encode (i, j, i + 1, j - 1), a))
	    move = pop (move, i + 1, j - 1);
	}
    }
  return move;
}

list *posmoves(int **a,int i,int j,list *move)
{
  if(a[i][j]==0)
   return move;
  else if(mod(a[i][j])==K)
   return(WhereToMoveKing(a,i,j,move));
  else if(mod(a[i][j])==P)
   return(WhereToMovePawn(a,i,j,move));
  else if(mod(a[i][j])==B)
   return(WhereToMoveBishop(a,i,j,move));
  else if(mod(a[i][j])==N)
   return(WhereToMoveKnight(a,i,j,move));
  else if(mod(a[i][j])==R)
   return(WhereToMoveRook(a,i,j,move));
  else if(mod(a[i][j])==Q)
  {
   move=WhereToMoveBishop(a,i,j,move);
   return(WhereToMoveRook(a,i,j,move));
  }
}

list *posmoves(int **,int ,int ,list *);
int **makeBoard1(char *board_shit) {
		int **bd = (int**) malloc(sizeof(int *) * 8); 
		for(int i=0; i!=8; i++) {
				bd[i] = (int *) malloc(sizeof(int) * 8); 
				for(int j=0; j!=8; j++) {
						char key = board_shit[j*8+i]; 						
						//int K = 10000, Q = 79, R = 50, B = 33, N = 31, P = 10, Depth = 5;
						if(key=='K') bd[i][j] = K; 
						if(key=='k') bd[i][j] = -K; 
						if(key=='Q') bd[i][j] = Q; 
						if(key=='q') bd[i][j] = -Q; 
						if(key=='R') bd[i][j] = R; 
						if(key=='r') bd[i][j] = -R;	
						if(key=='B') bd[i][j] = B; 
						if(key=='b') bd[i][j] = -B;
						if(key=='N') bd[i][j] = N; 
						if(key=='n') bd[i][j] = -N; 
						if(key=='P') bd[i][j] = P; 
						if(key=='p') bd[i][j] = -P; 
						if(key=='0') bd[i][j] = 0; 
					}
			}
			return bd; 
	}
int min_area=1200, max_area=4500; 
int min_angle = 15, max_angle=75; 
float min_ratio = 0.8, max_ratio=1.2;

//To be used for proc
Mat src,hsv, holded, holded1; 

//To be used for printing
Mat final1, final2; 
int flag=0, flag2=0;
              
RotatedRect *label(VideoCapture); 
bool inside_rect(RotatedRect, Point2f); 
int shL=0;int shH=255;int ssL=0;int ssH=255;int svL=100;int svH=255;
int shL1=0;int shH1=120;int ssL1=0;int ssH1=75;int svL1=10;int svH1=70;
int output1[2];
int *populate_board(Mat original_board, Mat final_board, RotatedRect *rects ) {
	int *loc = (int *) calloc(64,sizeof(int)); 
	/* subtraction */
	Mat subtracted; 
	absdiff(original_board, final_board, subtracted); 
	
	waitKey(0); 
	cvtColor(subtracted, subtracted, CV_BGR2GRAY); 
	threshold(subtracted, subtracted, 15, 255, THRESH_BINARY); 
	medianBlur(subtracted, subtracted, 15); 
	dilate(subtracted,subtracted,getStructuringElement( MORPH_RECT,Size(3,3)));	
	("subtracted", subtracted); 
	
	//HERE I WILL MASK AND SHOW BECAUSE I CAN BRO
	Mat binaried;
	final_board.copyTo(binaried, subtracted); 
	waitKey(0);
	imshow("binaried", binaried); 
	waitKey(0); 
	namedWindow("source");
	namedWindow("source1");
	Mat goti1, goti2; 
	createTrackbar("shL", "source", &shL, 255);
	createTrackbar("shH", "source", &shH, 255);
	createTrackbar("ssL", "source", &ssL, 255);
	createTrackbar("ssH", "source", &ssH, 255);
	createTrackbar("svL", "source", &svL, 255);
	createTrackbar("svH", "source", &svH, 255);
	createTrackbar("shL1", "source1", &shL1, 255);
	createTrackbar("shH1", "source1", &shH1, 255);
	createTrackbar("ssL1", "source1", &ssL1, 255);
	createTrackbar("ssH1", "source1", &ssH1, 255);
	createTrackbar("svL1", "source1", &svL1, 255);
	createTrackbar("svH1", "source1", &svH1, 255);
	waitKey(10);
	while(1){
		inRange(binaried, Scalar(shL1,ssL1,svL1), Scalar(shH1,ssH1,svH1), goti1); 
		// threshold(goti1, goti1, 15, 255, THRESH_BINARY);
		medianBlur(goti1, goti1, 15); 	

		imshow("goti1", goti1); 
		
		inRange(binaried, Scalar(shL,ssL,svL), Scalar(shH,ssH,svH), goti2); 
		// threshold(goti2, goti2, 15, 255, THRESH_BINARY); 
		medianBlur(goti2, goti2, 15); 
		//erode(goti2,goti2,getStructuringElement( MORPH_RECT,Size(15,15)));	
		imshow("goti2", goti2); 
		char x = waitKey(10);
		if(x ==27)
			break;
	}
	
	waitKey(0); 
	/* end subtraction */ 
//	erode(goti1, goti1, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) );
  //dilate( goti1, goti1, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) ); 
  //dilate( goti1, goti1, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) ); 
  //erode(goti1, goti1, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) );
  //erode(goti2, goti2, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) );
  //dilate( goti2, goti2, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) ); 
  //dilate( goti2, goti2, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) ); 
  //erode(goti2, goti2, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)) );
	/* find contours and centre of masses for yellow and blue separately */ 
	vector<vector<Point> > contours_blue, contours_yellow; vector<Vec4i> hierarchy_b, hierarchy_y; 
	findContours(goti1, contours_blue, hierarchy_b, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(goti2, contours_yellow, hierarchy_y, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	Mat drawing= Mat::zeros(original_board.size(), CV_8UC3); 
	vector<Point2f> CoMs_blue(32), CoMs_yellow(32); 
	for(int i=0; i!=contours_blue.size(); i++) {
			Moments m = moments(contours_blue[i], false); 
			float CoM_x = m.m10/m.m00; 
			float CoM_y = m.m01/m.m00; 
			CoMs_blue[i] = Point2f(CoM_x, CoM_y); 
			circle(drawing, Point(CoM_x, CoM_y), 20, Scalar(255,0,0), -1,8,0); 
			//cerr<<"blue_com="<<CoM_x<<" "<<CoM_y<<endl;
		}	
		
	for(int i=0; i!=contours_yellow.size(); i++) {
			Moments m = moments(contours_yellow[i], false); 
			float CoM_x = m.m10/m.m00; 
			float CoM_y = m.m01/m.m00; 
			CoMs_yellow[i] = Point2f(CoM_x, CoM_y); 
			circle(drawing, Point(CoM_x, CoM_y), 20, Scalar(0,255,255), -1,8,0); 
			//cerr<<"yellow_com="<<CoM_x<<" "<<CoM_y<<endl;
		}	
	imshow("..", original_board+drawing); 
	
	waitKey(0); 
	for(int i = 0; i!=64; i++) { //Loop thru rects
			for(int j =0; j!=32; j++)  { //Loop thru all points
					//waitKey(0);
				//cerr<<"i("<<i<<") j("<<j<<")"<<endl;
					bool result1 = inside_rect(rects[i], CoMs_blue[j]); 
					//cerr<<CoMs_blue[j]<<endl;
					//if(result==true){ loc[i] = 1; 
					//cerr<<"a black piece was moved to "<<i<<endl;break;}
				
					bool result2 = inside_rect(rects[i], CoMs_yellow[j]);
					//cerr<<CoMs_blue[j]<<endl; 
					//if(result==true){ loc[i] = 3;
					//	cerr<<"it was moved from "<<i<<endl;break;}
					if(result2==true&&result1==true)	continue;
					if(result2==true)
					{
						loc[i]=1;
						cerr<<"a black piece. was moved from "<<i<<endl;
						output1[1]=i;continue;
						//cerr<<"yellow_com"<<CoMs_yellow[j]<<endl;
					}
					if(result1==true)	{loc[i] = 3;
						{cerr<<"a black piece was moved to "<<i<<endl;
						output1[0]=i;continue;
						//cerr<<"blue_com"<<CoMs_blue[j]<<endl;
						}
				}
		} 
	}
	return output1;	
}

bool checkContour(vector<Point> cont) {
	Moments m = moments(cont, false); 
	//Area: 
	if(m.m00>max_area || m.m00<min_area) return false; 
	//Angle
	RotatedRect r = minAreaRect( Mat(cont));
	float modAngle = r.angle>0?r.angle:-r.angle; 	
	if(modAngle>min_angle && modAngle<max_angle) return false; 
	//Sides Ratio
	float ht = r.size.height; 
	float wt = r.size.width; 
	float ratio = ht/wt;

	if(ratio<min_ratio || ratio>max_ratio) return false; 
	//It is alright
	return true;
}
/* This sorts the array of rotated rects */ 
RotatedRect *sorting(RotatedRect *original, int N) {
	int k = N/2; 
	int flag = 0; 
	int k2=0; 
	RotatedRect *sorted = (RotatedRect *) malloc(sizeof(RotatedRect) * N); 
	for(int i=0; i!=k; i++) {
		if(flag==0) {
		sorted[2*i] = original[32+i]; 
		sorted[2*i+1] = original[i]; 
		k2+=2;  
		} else{
		sorted[2*i] = original[i]; 
		sorted[2*i+1] = original[32+i]; 
		k2+=2;  
		}
		if(k2==8) {
			flag=!flag; 
			k2=0;
		}	
	}
	return sorted; 
}

bool inside_rect(RotatedRect r, Point2f p) {
		//Check if point lies inside the boundaries of the rectangle. 
		Point2f temp_array[4]; 
		r.points(temp_array); 
		vector<Point2f> rect_points(temp_array, temp_array+4); 
		double result = pointPolygonTest(rect_points, p, false);
		return result>0?true:false; 
	}

RotatedRect *label(VideoCapture cam) {
	char key; //to quit
	RotatedRect *rects = (RotatedRect *)malloc(64 * sizeof(RotatedRect)); 

	/* These are configuration trackbars. Will be removed once the chess board is final. */ 
	namedWindow("Value and Saturation"); 
	namedWindow("Value and Saturation1"); 
	int hH = 255,hL= 0,vL=104, vH=173, sL=71, sH=131; 
	createTrackbar("hH", "Value and Saturation", &hH, 255);
	createTrackbar("hL", "Value and Saturation", &hL, 255);  
	createTrackbar("sH", "Value and Saturation", &sH, 255); 
	createTrackbar("sL", "Value and Saturation", &sL, 255); 
	createTrackbar("vH", "Value and Saturation", &vH, 255); 
	createTrackbar("vL", "Value and Saturation", &vL, 255); 
	int hH1=126,hL1=13,vL1=0, vH1=121,sL1=0, sH1=80; 
	createTrackbar("hH1", "Value and Saturation1", &hH1, 255);
	createTrackbar("hL1", "Value and Saturation1", &hL1, 255);
	createTrackbar("sH1", "Value and Saturation1", &sH1, 255); 
	createTrackbar("sL1", "Value and Saturation1", &sL1, 255); 
	createTrackbar("vH1", "Value and Saturation1", &vH1, 255); 
	createTrackbar("vL1", "Value and Saturation1", &vL1, 255); 
	createTrackbar("minArea", "Value and Saturation", &min_area, 10000); 
	createTrackbar("maxArea", "Value and Saturation", &max_area, 10000); 
	/* end config */
	//Start main loop: 
	while(key!=27) {
		//Read and threshold image. Clean up. 
		cam.read(src);
		cvtColor(src,hsv, CV_BGR2HSV); 
		inRange(hsv, Scalar(hL, sL, vL), Scalar(hH, sH,vH),holded); 
		inRange(hsv, Scalar(hL1, sL1, vL1), Scalar(hH1, sH1,vH1),holded1); 	
		medianBlur(holded, holded, 15); 
		medianBlur(holded1, holded1, 15);
		erode(holded,holded,getStructuringElement( MORPH_RECT,Size(9,9)));
		erode(holded1,holded1,getStructuringElement( MORPH_RECT,Size(9,9)));
		//medianBlur(holded1, holded1, 9);
		//dilate(holded1, holded1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
		imshow("b", holded);
		imshow("c", holded1);

		//Fiond contours for black and white , find moments		
		vector<vector<Point> > contours; 		
		vector<Vec4i> hierarchy;	
		vector<vector<Point> > contours1; 		
		vector<Vec4i> hierarchy1;
		findContours(holded, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		vector<Moments> mu(contours.size() );
		findContours(holded1, contours1, hierarchy1, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );	
		vector<Moments> mu1(contours1.size() );
		for( int i = 0; i < contours.size(); i++ ) {
			mu[i] = moments( contours[i], false );
		}
		for( int i = 0; i < contours1.size(); i++ ) {
			mu1[i] = moments( contours1[i], false );
		}
				
		vector<Point2f> mc( contours.size() );
		vector<Point2f> mc1( contours1.size() );


		for( int i = 0; i < contours.size(); i++ ) {
			mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		}
		for( int i = 0; i < contours1.size(); i++ ) {
			mc1[i] = Point2f( mu1[i].m10/mu1[i].m00 , mu1[i].m01/mu1[i].m00 ); 
		}

		
		Mat drawing = Mat::zeros( holded.size(), CV_8UC3 );//for drawing

		
		int k=0; //k will count number of white/black boxes which are valid. 
		for( int i = 0; i< contours.size(); i++ )
     		{

			
			if(flag==1) continue; 
			if(checkContour(contours[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours[i])); 	
			/* start drawing functions */
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(255,255,0), 2, 8 ); 
			
     			}
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc[i], 1,1, Scalar(255,255,255)); 
			rects[k] = r1; 
			/* end drawing functions */ 
			k++;  			
			if(k==32) { //k=32 implies we are finished with the chess blocks
				flag=1; 
				drawing.copyTo(final1); 
			}
	
     		}	

		k=0;	//reset white/black counter for black 
		for( int i = 0; i< contours1.size(); i++ )
     		{

			if(flag2==1) continue;			
			if(checkContour(contours1[i])==false) continue; 
			RotatedRect r1 = minAreaRect( Mat(contours1[i])); 

			/* start drawing functions */	
			Point2f r[4]; r1.points(r);
			for( int j = 0; j < 4; j++ ){ 
          			line( drawing, r[j], r[(j+1)%4], Scalar(0,0,255), 2, 8 );
     			} 
			string s;
			stringstream out;
			out << k;
			s = out.str();
			putText(drawing, s, mc1[i], 1,1, Scalar(255,0,255)); 
			rects[k+32] = r1; 
			/* end drawing function */ 

			k++;  		
			if(k==32) {
				flag2=1;
				drawing.copyTo(final2);
			}	
     		}
		
		imshow("COM", src+drawing); //Display current drawn imgs
		key=waitKey(30); 
	}
	imshow("final", src+final2 + final1); //display the final contours 
	
	waitKey(0); 
	cerr<<"stored rectangles, press ESC to continue"<<endl;
	return sorting(rects, 64);
}
int main()
{
	Mat o,t;
	VideoCapture c(1);
	RotatedRect *rects=label(c);
	int i;
int **bd = (int **) malloc(sizeof(int *) * 8);
for( i=0; i!=8; i++) {
			bd[i] = (int *) calloc(8, sizeof(int)); }
	bd[0][0] = R;
	bd[0][1] = N;
	bd[0][2] = B;
	bd[0][3] = Q;	
	bd[0][4] = K;
	bd[0][5] = B;
	bd[0][6] = N;
	bd[0][7] = R;
	bd[1][0] = P;
	bd[1][1] = P;
	bd[1][2] = P;
	bd[1][3] = P;
	bd[1][4] = P;
	bd[1][5] = P;
	bd[1][6] = P;
	bd[1][7] = P;
	bd[6][0] =-P;
	bd[6][1] =-P;
	bd[6][2] =-P;
	bd[6][3] =-P;
	bd[6][4] =-P;
	bd[6][5] =-P;
	bd[6][6] =-P;
	bd[6][7] =-P;
	bd[7][0] =-R, 
	bd[7][1] =-N,
	bd[7][2] =-B, 
	bd[7][3] =-Q, 
	bd[7][4] =-K,
	bd[7][5] =-B,
	bd[7][6] =-N,
	bd[7][7] =-R;
	PrintBoard(bd);int a,b,mv,i_x,i_y,f_x,f_y,e,d;
	while(1){
		mv=getbestmove(bd, 1,0,1);
		cout<<mv<<endl;
		e=mv/100;d=mv%100;cout<<e<<"----->"<<d<<" is best move"<<endl;
		cout<<"arm has to move from "<<rects[8*((e/10)-1)+(e%10)-1].center<<" to "<<rects[8*((d/10)-1)+(d%10)-1].center<<endl;
		PrintBoard(bd);
			while(1)
		{
			c.read(o);
			imshow("o", o); 
			char k = waitKey(30);
			if(k==27) break; 
		} 
		while(1)
		{
			c.read(t);
			imshow("t", t); 
			char k = waitKey(30);
			if(k==27) break; 
		}
		int *output1 = populate_board(o,t, rects);
		cout<<"output[0]="<<output1[0]<<"  output[1]="<<output1[1]<<endl;
		a=output1[0];b=output1[1];
		if (bd[(e/10)-1][(e%10)-1]<0)
			cerr<<"white killed a black!!!"<<endl;
		bd[(d/10)-1][(d%10)-1]=bd[(e/10)-1][(e%10)-1];
	bd[(e/10)-1][(e%10)-1]=0;
	//PrintBoard(bd);
	cout<<"a="<<a<<" b="<<b<<endl;
	f_x=a/8;
	f_y=a%8;
	i_x=b/8;
	i_y=b%8;
	if (bd[i_x][i_y]>0)
	{
		cerr<<"black killed a white"<<endl;
	}
	bd[f_x][f_y]=bd[i_x][i_y];
	bd[i_x][i_y]=0;
	PrintBoard(bd);
	destroyAllWindows();
	}
return 0;
}