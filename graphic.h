#include<windows.h>
#include<cstring>
#include<cstdlib>
#include<cstdio>
const int DROW=2;
const int DCOL=4;
void gotoxy(int x,int y){ //(0,0) is top-left
	COORD coord={(short)y,(short)x}; //swap row and col
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
void MazeCursor(int x,int y){ //cursor in maze
	gotoxy(x+DROW,y*2+DCOL);
}
void hideCursor(){
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=FALSE;
	SetConsoleCursorInfo(hOut,&cci);
}
void setColor(int x){
	if(x>9) return;
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle,x);
}
int ConHeight,ConWeight;
void setConsoleSize(int height,int weight){ //don't change it
	ConHeight=height>43?43:height;
	ConWeight=weight>180?180:weight;
	char command[100]="Mode Con Cols=     Lines=     ";
	int x=ConWeight;
	command[14]='0'+x/100;command[15]='0'+(x/10)%10;command[16]='0'+x%10;
	x=ConHeight;
	command[25]='0'+x/100;command[26]='0'+(x/10)%10;command[27]='0'+x%10;
	system(command);
}
void MediatePrint(const char *s){
	int len=strlen(s);
	if(len<ConWeight){
		int spc=ConWeight/2-len/2;
		while(spc-->0) putchar(' ');
	}
	puts(s);
}

void PrintHeart(){
	for(double x=1.3;x>=-0.9;x-=0.09){
		for(double y=1.2;y>=-1.2;y-=0.05){
			double p=x*x+y*y-1;
			double res=p*p*p-y*y*x*x*x;
			if(res<=0) putchar('#');
			else putchar(' ');
		}
		putchar('\n');
	}
}
