#include<cstdio>
#include<iostream>
#include<algorithm>
#include<conio.h>
#include<vector>
#include<ctime>
#include<cctype>
#include"graphic.h"
#include"config.h"
using namespace std;
#define RUNTIME clock()/(double)CLOCKS_PER_SEC
const int dx[]={-1,0,1,0},dy[]={0,1,0,-1},TWD=4;
inline int getRand(int l,int r){
	if(l>r) swap(l,r);
	return rand()%(r-l+1)+l;
}
inline int getOddRand(int l,int r){
	if(l==r&&l%2==0&&r%2==0) return 1;
	for(int i=getRand(l,r);1;i=getRand(l,r)){
		if(i&1) return i;
	}
}
namespace Maze{
#define MAXSIZ 507
#define BOUND -1
#define WALL 1
#define LAND 2
	const int PLAYER_COLOR=3;
	const int TEXT_COLOR=7;
	const int START_COLOR=7;
	const int END_COLOR=7;
	const int MAZE_COLOR=4;
	const int PATH_COLOR=6;
	const int COVER_COLOR=7;
	const int SCOPE=3;
	int G[MAXSIZ][MAXSIZ],n,m;
	bool vis[MAXSIZ][MAXSIZ],ispath[MAXSIZ][MAXSIZ];
	bool iscover[MAXSIZ][MAXSIZ];
	bool covered;
	vector<pair<int,int> > path;
	pair<int,int> start,end,player;
	struct CheatCode{
		char s[13];
		int match_pos;
		CheatCode(const char *str){strcpy(s,str);match_pos=0;}
	};
	CheatCode ShowPath("PATH");
	CheatCode RemoveCover("REMOVE");
	inline bool isout(int x,int y){
		return x<1||x>n||y<1||y>m;
	}
	inline bool isescape(int x,int y){
		if(x<0||x>n+1||y<0||y>m+1) return 1;
		return 0;
	}
	inline int getdis(pair<int,int> p1,pair<int,int> p2){
		return max(abs(p1.first-p2.first),abs(p1.second-p2.second));
	}
	bool two_step_expand(int x,int y,int i){
		x+=dx[i]; x+=dx[i]; y+=dy[i]; y+=dy[i];
		if(isout(x,y)||G[x][y]!=LAND) return 0;
		return !vis[x][y];
	}
	void NatureSingleDFS(int x,int y){
		if(vis[x][y]) return;
		vis[x][y]=1;
		int tp[TWD]; for(int i=0;i<TWD;i++) tp[i]=i;
		random_shuffle(tp,tp+TWD);
		for(int p=0;p<TWD;p++){
			if(!two_step_expand(x,y,tp[p])) continue;
			int tx=x,ty=y;
			do{
				G[tx][ty]=LAND;
				tx+=dx[tp[p]]; ty+=dy[tp[p]];
			}while(G[tx][ty]==WALL);
			NatureSingleDFS(tx,ty);
		}
	}
	void GenerateNatureSingleMaze(){ //zhu lu niu qu xing
		for(int cnt=1;cnt<=2;++cnt){ //get initial point(2)
			pair<int,int> tmp=start;
			if(cnt==2) tmp=end;
			for(int i=0;i<TWD;i++){
				int x=tmp.first+dx[i],y=tmp.second+dy[i];
				if(isout(x,y)) continue;
				G[x][y]=LAND;
				if(tmp==start) player=make_pair(x,y); //set player
				NatureSingleDFS(x,y);
				break; //only one toward is legal
			}
		}
		//embellish
		if(n%2==0){
			for(int i=1;i<=m;i++) if(rand()&1) G[n][i]=LAND;
		}
		if(m%2==0){
			for(int i=1;i<=n;i++) if(rand()&1) G[i][m]=LAND;
		}
	}
	void GeneratePoint(){
		if(getRand(0,3)) start=make_pair(getOddRand(1,n/2+1),0);
		else start=make_pair(0,getOddRand(1,m/2+1));
		if(getRand(0,3)) end=make_pair(getOddRand(n/2+1,n),m+1);
		else end=make_pair(n+1,getOddRand(m/2+1,m));
	}
	void SearchNext(int x,int y){ //dfs way-finding
		if(vis[x][y]) return;
		vis[x][y]=1;
		if(x==end.first&&y==end.second) return;
		for(int i=0;i<TWD;i++){
			int tx=x+dx[i],ty=y+dy[i];
			if(isout(tx,ty)&&make_pair(tx,ty)!=end) continue;
			if(G[tx][ty]!=LAND&&make_pair(tx,ty)!=end) continue;
			SearchNext(tx,ty);
			if(vis[end.first][end.second]){
				path.push_back(make_pair(x,y));
				return;
			}
		}
	}
	void CalculatePath(int x,int y){ //get path between player and end
		memset(vis,0,sizeof(vis)); path.clear();
		memset(ispath,0,sizeof(ispath));
		SearchNext(x,y);
		//path.pop_back(); // pop player
		reverse(path.begin(),path.end());
	}
	void GenerateMaze(int height,int weight){
		system("cls");
		n=height; m=weight;
		covered=Settings::isCover;
		memset(ispath,0,sizeof(ispath));
		memset(iscover,covered,sizeof(iscover));
		memset(vis,0,sizeof(vis));
		memset(G,0,sizeof(G));
		for(int i=0;i<=n+1;i++){
			for(int j=0;j<=m+1;j++){
				if(i==0||j==0||i>n||j>m) G[i][j]=BOUND;
				else if((i&1)&&(j&1)) G[i][j]=LAND;
				else G[i][j]=WALL;
			}
		}
		//initial ended
		GeneratePoint();
		if(Settings::MazeType==1) GenerateNatureSingleMaze();
	}
	void PrintMaze(){ //debug, no delay
		for(int i=0;i<=n+1;i++){
			for(int j=0;j<=m+1;j++){
				if(isescape(i,j)) continue;
				MazeCursor(i,j);
				pair<int,int> tmp=make_pair(i,j);
				if(covered&&iscover[i][j]){
					setColor(COVER_COLOR); printf(BLOCK);
					continue;
				}
				if(tmp==start){
					setColor(START_COLOR); printf(j?D_A:R_A);
				}
				else if(tmp==end){
					setColor(END_COLOR); printf(i==n+1?D_A:R_A);
				}
				else if(tmp==player){
					setColor(PLAYER_COLOR); printf(PLAYER);
				}
				else{
					setColor(MAZE_COLOR);
					if(G[i][j]==WALL) printf(BLOCK); //it's a multi-byte char
					else if(G[i][j]==BOUND) printf(BLOCK);
					else printf("  "); //land
				}
			}
		}
	}
	void ShowMaze(){
		vector<pair<int,int> > v;
		for(int i=0;i<=n+1;i++)
			for(int j=0;j<=m+1;j++) if(!isescape(i,j)) v.push_back(make_pair(i,j));
		random_shuffle(v.begin(),v.end());
		vector<pair<int,int> >::iterator it;
		for(it=v.begin();it!=v.end();++it){
			int i=(*it).first,j=(*it).second;
			MazeCursor(i,j);
			pair<int,int> tmp=make_pair(i,j);
			if(tmp==start||tmp==end||tmp==player) continue;
			else{
				setColor(MAZE_COLOR);
				if(G[i][j]==WALL) printf(BLOCK);
				else if(G[i][j]==BOUND) printf(BLOCK);
				else printf("  "); //land
			}
			for(int tmp_cnt=0;tmp_cnt<=50000;tmp_cnt++); //make program slow down
		}
		MazeCursor(start.first,start.second);
		setColor(START_COLOR); printf(start.second?D_A:R_A);
		MazeCursor(end.first,end.second);
		setColor(END_COLOR); printf(end.first==n+1?D_A:R_A);
		MazeCursor(player.first,player.second);
		setColor(PLAYER_COLOR); printf(PLAYER);
		if(covered){
			Sleep(400);
			for(int i=0;i<=n+1;i++){
				for(int j=0;j<=m+1;j++){
					if(G[i][j]==BOUND) iscover[i][j]=0;
					if(getdis(player,make_pair(i,j))<=SCOPE) iscover[i][j]=0;
					if(iscover[i][j]){
						MazeCursor(i,j); setColor(COVER_COLOR);
						printf(BLOCK);
					}
				}
			}
		}
	}
	void PrintPath(){
		setColor(PATH_COLOR);
		vector<pair<int,int> >::iterator it;
		pair<int,int> last=player;
		for(it=path.begin();it!=path.end();++it){
			pair<int,int> p=*it;
			ispath[p.first][p.second]=1;
			if(p!=player){
				MazeCursor(p.first,p.second);
				/*
				if(p.first==last.first+1) printf(D_A);
				else if(p.first==last.first-1) printf(U_A);
				else if(p.second==last.second+1) printf(R_A);
				else printf(L_A);
				*/
				printf(ISPATH);
			}
			last=p;
			Sleep(10);
		}
	}
	void PrintHelp(){
		setColor(TEXT_COLOR);
		int row=2,stp=2,r=6;
		if(n<=6) row=1,stp=1;
		MazeCursor(row,m+r); printf("¡ü/W: Up");
		MazeCursor(row+=stp,m+r); printf("¡ý/S: Down");
		MazeCursor(row+=stp,m+r); printf("¡û/A: Left");
		MazeCursor(row+=stp,m+r); printf("¡ú/D: Right");
		MazeCursor(row+=stp,m+r); printf("Esc: Exit");
	}
	void ReflushMaze(){
		PrintMaze();
		PrintHelp();
		PrintPath();
	}
	int toToward(char ch){ //get toward in d* array
		if(ch==UP||ch==K_W) return 0;
		if(ch==RIGHT||ch==K_D) return 1;
		if(ch==DOWN||ch==K_S) return 2;
		if(ch==LEFT||ch==K_A) return 3;
		return -1;
	}
	void RestoreBlock(int x,int y){
		MazeCursor(x,y);
		if(ispath[x][y]){
			setColor(PATH_COLOR); printf(ISPATH);
		}
		else if(G[x][y]==BOUND){
			setColor(MAZE_COLOR); printf(BLOCK);
		}
		else if(G[x][y]==WALL){
			setColor(MAZE_COLOR); printf(BLOCK);
		}
		else{ //is land
			printf("  ");
		}
	}
	void MatchCheatCode(char ch){
		if(!isalpha(ch)) return;
		ch=toupper(ch);
		if(ShowPath.s[ShowPath.match_pos]==ch) ++ShowPath.match_pos;
		else ShowPath.match_pos=0;
		if(ShowPath.match_pos==(int)strlen(ShowPath.s)){
			CalculatePath(player.first,player.second);
			ReflushMaze(); PrintPath();
			ShowPath.match_pos=0;
		}
		if(RemoveCover.s[RemoveCover.match_pos]==ch) ++RemoveCover.match_pos;
		else RemoveCover.match_pos=0;
		if(RemoveCover.match_pos==(int)strlen(RemoveCover.s)){
			for(int i=0;i<=n+1;i++)
				for(int j=0;j<=m+1;j++) if(iscover[i][j]) RestoreBlock(i,j);
			memset(iscover,0,sizeof(iscover)); covered=0;
			RemoveCover.match_pos=0;
		}
	}
	pair<int,int> NextStep(int x,int y,int i){
		if(isout(x+dx[i],y+dy[i])) return make_pair(x,y);
		if(G[x+dx[i]][y+dy[i]]!=LAND) return make_pair(x,y);
		return make_pair(x+dx[i],y+dy[i]);
	}
	void RunGame(){
		while(1){
			char ch=getch();
			if(ch==ESC) break; //exit game
			int twd=toToward(ch);
			if(twd!=-1){ //go to next step
				if(make_pair(player.first+dx[twd],player.second+dy[twd])==end){
					break; // arrive end
				}
				RestoreBlock(player.first,player.second);
				player=NextStep(player.first,player.second,twd); //go next
				MazeCursor(player.first,player.second);
				setColor(PLAYER_COLOR); printf(PLAYER);
			}
			if(covered){
				for(int di=-SCOPE;di<=SCOPE;++di){
					for(int dj=-SCOPE;dj<=SCOPE;++dj){
						int i=player.first,j=player.second;
						if(isescape(i+di,j+dj)) continue;
						if(!iscover[i+di][j+dj]) continue;
						iscover[i+di][j+dj]=0;
						MazeCursor(i+di,j+dj);
						if(G[i+di][j+dj]==WALL){
							setColor(MAZE_COLOR); printf(BLOCK);
						}
						else if(ispath[i+di][j+dj]){
							setColor(PATH_COLOR); printf(ISPATH);
						}
						else printf("  ");
					}
				}
			}
			MatchCheatCode(ch);
		}
	}
#undef MAXSIZ
#undef BOUND
#undef WALL
#undef LAND
}
void PrintSettings(){ //debug, can remove
	Settings::ReadinSettings();
	cout<<Settings::MazeType<<' '<<Settings::MazeShape<<' '<<endl;
	cout<<Settings::MazeHeight<<' '<<Settings::MazeWeight<<endl;
	cout<<Settings::isCover<<endl;
	Settings::WriteoutSettings();
}
int main(){
	srand(time(0));
	hideCursor();
	Settings::ReadinSettings();
	setConsoleSize(Settings::MazeHeight+8,Settings::MazeWeight*2+35);
	while(1){
		Maze::GenerateMaze(Settings::MazeHeight,Settings::MazeWeight);
		Maze::ShowMaze();
		//Maze::PrintPath();
		Maze::PrintHelp();
		double start_time=RUNTIME;
		Maze::RunGame();
		gotoxy(Maze::n+DROW+3,0);
		setColor(7); printf("> Game Over! ");
		printf("You Spend %.1f Seconds.\n",RUNTIME-start_time);
		puts("> Press Enter to Play Again or Press Esc to Exit Game.");
		int press=getch();
		while(press!=ESC&&press!=ENTER) press=getch();
		if(press==ESC) break;
	}
	Settings::WriteoutSettings();
	///////////////////////////////////////////////
	setColor(7); system("cls");
	return 0;
}
