#include<windows.h>
#include<cstring>
#include<cstdlib>
const int DROW=2;
const int DCOL=4;
void gotoxy(int x,int y){ // (0,0) is left & up
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
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE); //use std_i/o handle
	SetConsoleTextAttribute(handle,x);
	//0:dark 1:deep-blue 2:green 3:blue 4:red 5:purple 6:yellow 7:white 8:dark-white
}
#include<cstdio>
void setConsoleSize(int height,int weight){ //don't change it
	height=height>43?43:height;
	weight=weight>180?180:weight;
	char command[100]="Mode Con Cols=     Lines=     ";
	int x=weight;
	command[14]='0'+x/100;command[15]='0'+(x/10)%10;command[16]='0'+x%10;
	x=height;
	command[25]='0'+x/100;command[26]='0'+(x/10)%10;command[27]='0'+x%10;
	system(command);
}

// Character Map
const char *L_A="¡û",*R_A="¡ú",*U_A="¡ü",*D_A="¡ý";
const char *STAR="¡î",*CROSS="¡Á";
const char *BLOCK="¨€ ";
const char *U_T="¡ø",*D_T="¨‹",*L_T="?",*R_T="?";
const char *MAN="¡á",*WOMAN="¡â";
const char *PLAYER="¡ô";
const char *ISPATH="¡¤"; //single char need add a space

//Keyboard Map
//*test on Lenovo laptop
const int UP=72;
const int DOWN=80;
const int LEFT=75;
const int RIGHT=77;
const int ESC=27;
const int K_A=97; //all characters is lower
const int K_B=98;
const int K_C=99;
const int K_D=100;
const int K_E=101;
const int K_F=102;
const int K_G=103;
const int K_H=104;
const int K_I=105;
const int K_J=106;
const int K_K=107;
const int K_L=108;
const int K_M=109;
const int K_N=110;
const int K_O=111;
const int K_P=112;
const int K_Q=113;
const int K_R=114;
const int K_S=115;
const int K_T=116;
const int K_U=117;
const int K_V=118;
const int K_W=119;
const int K_X=120;
const int K_Y=121;
const int K_Z=122;
const int TAB=9;
const int N_0=48;
const int N_1=49;
const int N_2=50;
const int N_3=51;
const int N_4=52;
const int N_5=53;
const int N_6=54;
const int N_7=55;
const int N_8=56;
const int N_9=57;
const int ENTER=13;
const int BACK=8;
const int SPACE=32;
