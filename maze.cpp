#include<cstdio>
#include<iostream>
#include<algorithm>
#include<conio.h>
#include<vector>
#include<queue>
#include<ctime>
#include<cctype>
#include"constant.h"
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
inline void wait(long long x){
	long long res=clock();
	while(clock()-res<x);
}
namespace Maze{
#define MAXSIZ 207
#define BOUND -1
#define WALL 1
#define ROAD 2
	const int RANGE=3;
	int G[MAXSIZ][MAXSIZ],n,m,step;
	bool vis[MAXSIZ][MAXSIZ],ispath[MAXSIZ][MAXSIZ];
	bool iscover[MAXSIZ][MAXSIZ];
	bool covered;
	vector<pair<int,int> > path;
	pair<int,int> start,end,player;
	struct CheatCode{
		char s[17];
		int matched;
		CheatCode(const char *str){strcpy(s,str);matched=0;}
	};
	CheatCode ShowPath("PATH");
	CheatCode RemoveCover("REMOVE");
	CheatCode Teleport("TELEPORT");
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
		if(isout(x,y)||G[x][y]!=ROAD) return 0;
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
				G[tx][ty]=ROAD;
				tx+=dx[tp[p]]; ty+=dy[tp[p]];
			}while(G[tx][ty]==WALL);
			NatureSingleDFS(tx,ty);
		}
	}
	void GenerateNatureSingleMaze(){ //recursive-backtracking
		for(int i=0;i<=n+1;i++){ //initial
			for(int j=0;j<=m+1;j++){
				if(i==0||j==0||i>n||j>m) G[i][j]=BOUND;
				else if((i&1)&&(j&1)) G[i][j]=ROAD;
				else G[i][j]=WALL;
			}
		}
		for(int cnt=1;cnt<=2;++cnt){ //get initial point(2)
			pair<int,int> tmp=start;
			if(cnt==2) tmp=end;
			for(int i=0;i<TWD;i++){
				int x=tmp.first+dx[i],y=tmp.second+dy[i];
				if(isout(x,y)) continue;
				G[x][y]=ROAD;
				if(tmp==start) player=make_pair(x,y); //set player
				NatureSingleDFS(x,y);
				break; //only one toward is legal
			}
		}
		//embellish
		if(n%2==0){
			for(int i=1;i<=m;i++) if(rand()&1) G[n][i]=ROAD;
		}
		if(m%2==0){
			for(int i=1;i<=n;i++) if(rand()&1) G[i][m]=ROAD;
		}
	}
	void GeneratePrimNatureMaze(){ //random prim algorithm
		int prim[MAXSIZ][MAXSIZ]={0};
		for(int i=0;i<=n+1;i++){
			for(int j=0;j<=m+1;j++){
				if(i==0||j==0||i>n||j>m) prim[i][j]=BOUND;
				else if((i&1)&&(j&1)) prim[i][j]=1; //yellow
				else prim[i][j]=0; //dark
			}
		}
		for(int cnt=1;cnt<=2;++cnt){
			priority_queue<pair<int,pair<int,int> > > q;
			pair<int,int> tmp=start,inp;
			if(cnt==2) tmp=end;
			for(int i=0;i<TWD;i++){
				int x=tmp.first+dx[i],y=tmp.second+dy[i];
				if(isout(x,y)) continue;
				if(tmp==start) player=make_pair(x,y); //set player
				inp=make_pair(x,y);
				prim[x][y]=11;
				break; //only one toward is legal
			}
			for(int i=0;i<TWD;i++){
				int x=inp.first+dx[i],y=inp.second+dy[i];
				if(isout(x,y)) continue;
				if(prim[x][y]==0){
					q.push(make_pair(rand(),make_pair(x,y)));
					prim[x][y]=10; //blue
				}
			}
			while(!q.empty()){
				pair<int,int> p=q.top().second; q.pop();
				pair<int,int> p1=make_pair(-1,-1),p2=p1;
				bool flag=0;
				for(int i=0;i<TWD;i++){
					int x=p.first+dx[i],y=p.second+dy[i];
					if(isout(x,y)) continue;
					if(prim[x][y]==1) flag^=1;
					if(prim[x][y]==11) flag^=0; //red
					if(prim[x][y]==1||prim[x][y]==11){
						if(p1.first==-1) p1=make_pair(x,y);
						else p2=make_pair(x,y);
					}
				}
				if(!flag){
					prim[p.first][p.second]=0;
					continue;
				}
				if(prim[p1.first][p1.second]==1) swap(p1,p2);
				prim[p2.first][p2.second]=11;
				prim[p.first][p.second]=11;
				for(int i=0;i<TWD;i++){
					int x=p2.first+dx[i],y=p2.second+dy[i];
					if(isout(x,y)) continue;
					if(prim[x][y]==0){
						prim[x][y]=10;
						q.push(make_pair(rand(),make_pair(x,y)));
					}
				}
			}
		}
		for(int i=0;i<=n+1;i++){
			for(int j=0;j<=m+1;j++){
				if(prim[i][j]==BOUND) G[i][j]=BOUND;
				else if(prim[i][j]==0) G[i][j]=WALL;
				else G[i][j]=ROAD;
			}
		}
		//embellish
		if(n%2==0){
			for(int i=1;i<=m;i++) if(rand()&1) G[n][i]=ROAD;
		}
		if(m%2==0){
			for(int i=1;i<=n;i++) if(rand()&1) G[i][m]=ROAD;
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
			if(G[tx][ty]!=ROAD&&make_pair(tx,ty)!=end) continue;
			SearchNext(tx,ty);
			if(vis[end.first][end.second]){
				path.push_back(make_pair(x,y));
				return;
			}
		}
	}
	void CalculatePath(int x,int y){ //calc path between player and end
		memset(vis,0,sizeof(vis)); path.clear();
		memset(ispath,0,sizeof(ispath));
		SearchNext(x,y);
		//path.pop_back(); // pop player, not necessary
		reverse(path.begin(),path.end());
	}
	void GenerateMaze(int height,int weight){
		system("cls");
		n=height; m=weight; step=0;
		covered=Settings::isCover;
		memset(ispath,0,sizeof(ispath));
		memset(iscover,covered,sizeof(iscover));
		memset(vis,0,sizeof(vis));
		memset(G,0,sizeof(G));
		player=start=end=make_pair(0,0);
		path.clear();
		GeneratePoint();
		if(Settings::MazeType==1) GenerateNatureSingleMaze();
		else if(Settings::MazeType==2) GeneratePrimNatureMaze();
	}
	void PrintMaze(){ //no delay
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
					if(G[i][j]==WALL) printf(BLOCK);
					else if(G[i][j]==BOUND) printf(BLOCK);
					else printf("  "); //ROAD
				}
			}
		}
	}
	void ShowMaze(){
		vector<pair<int,int> > v;
		for(int i=0;i<=n+1;i++){
			for(int j=0;j<=m+1;j++){
				if(!isescape(i,j)&&G[i][j]!=ROAD) v.push_back(make_pair(i,j));
			}
		}
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
				else printf("  "); //ROAD
			}
			//wait(1);
			for(int tmp_cnt=0;tmp_cnt<=400000;tmp_cnt++); //make program slow down
		}
		MazeCursor(start.first,start.second);
		setColor(START_COLOR); printf(start.second?D_A:R_A);
		MazeCursor(end.first,end.second);
		setColor(END_COLOR); printf(end.first==n+1?D_A:R_A);
		MazeCursor(player.first,player.second);
		setColor(PLAYER_COLOR); printf(PLAYER);
		if(covered){
			Sleep(1000);
			for(int i=0;i<=n+1;i++){
				for(int j=0;j<=m+1;j++){
					if(G[i][j]==BOUND) iscover[i][j]=0;
					if(getdis(player,make_pair(i,j))<=RANGE) iscover[i][j]=0;
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
			wait(1);
		}
	}
	void PrintInfo(double cost=-1){
		setColor(TEXT_COLOR);
		int row=2,stp=2,r=6;
		if(n<=13) row=1;
		MazeCursor(row,m+r); printf("%s/W: Up",U_A);
		MazeCursor(row+=stp,m+r); printf("%s/S: Down",D_A);
		MazeCursor(row+=stp,m+r); printf("%s/A: Left",L_A);
		MazeCursor(row+=stp,m+r); printf("%s/D: Right",R_A);
		MazeCursor(row+=stp,m+r); printf("Esc: Exit");
		if(cost<=0) return;
		if(row+stp>n-9){
			for(int i=0;i<=n;i++){
				MazeCursor(i,m+r); printf("            ");
			}
			row=2;
		}
		else row=n-8;
		r=4;
		MazeCursor(row,m+4); printf("G A M E     O V E R");
		if(player==end){
			MazeCursor(row+=stp,m+r); setColor(2);
			printf("You Reached the End");
		}
		else{
			setColor(8); MazeCursor(row+=stp,m+r);
			printf(" You Quit the Game");
		}
		r=6; setColor(TEXT_COLOR);
		MazeCursor(row+=3,m+r);
		printf("Time: %.1f s",cost);
		MazeCursor(row+=stp,m+r);
		printf("Step:  %d",step);
	}
	void ReflushMaze(){
		PrintMaze();
		PrintInfo();
		PrintPath();
	}
	int toToward(char ch){ //get toward in dxdy array
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
		else{ //is ROAD
			printf("  ");
		}
	}
	void MatchCheatCode(char ch){
		if(!isalpha(ch)) return;
		ch=toupper(ch);
		if(ShowPath.s[ShowPath.matched]==ch) ++ShowPath.matched;
		else ShowPath.matched=0;
		if(ShowPath.matched==(int)strlen(ShowPath.s)){
			CalculatePath(player.first,player.second);
			ReflushMaze(); PrintPath();
			ShowPath.matched=0;
		}
		if(RemoveCover.s[RemoveCover.matched]==ch) ++RemoveCover.matched;
		else RemoveCover.matched=0;
		if(RemoveCover.matched==(int)strlen(RemoveCover.s)){
			for(int i=0;i<=n+1;i++)
				for(int j=0;j<=m+1;j++) if(iscover[i][j]) RestoreBlock(i,j);
			memset(iscover,0,sizeof(iscover)); covered=0;
			RemoveCover.matched=0;
		}
		if(Teleport.s[Teleport.matched]==ch) ++Teleport.matched;
		else Teleport.matched=0;
		if(Teleport.matched==(int)strlen(Teleport.s)){
			for(int i=0;i<TWD;++i){
				int x=end.first+dx[i],y=end.second+dy[i];
				if(isout(x,y)||G[x][y]!=ROAD) continue;
				RestoreBlock(player.first,player.second);
				player=make_pair(x,y); ++step;
				MazeCursor(x,y); setColor(PLAYER_COLOR);
				printf(PLAYER);
				break;
			}
			Teleport.matched=0;
		}
	}
	pair<int,int> NextStep(int x,int y,int i){
		if(isout(x+dx[i],y+dy[i])) return make_pair(x,y);
		if(G[x+dx[i]][y+dy[i]]!=ROAD) return make_pair(x,y);
		return make_pair(x+dx[i],y+dy[i]);
	}
	void RunGame(){
		while(1){
			char ch=getch();
			if(ch==ESC) break; //exit game
			if(ch==TAB) ReflushMaze(); //for debug
			int twd=toToward(ch);
			if(twd!=-1){ //go to next step
				if(make_pair(player.first+dx[twd],player.second+dy[twd])==end){
					player=end; break; // arrive end
				}
				RestoreBlock(player.first,player.second);
				pair<int,int> nxt_stp=NextStep(player.first,player.second,twd);
				if(nxt_stp!=player) ++step;
				player=nxt_stp;
				MazeCursor(player.first,player.second);
				setColor(PLAYER_COLOR); printf(PLAYER);
			}
			MatchCheatCode(ch);
			if(covered){
				for(int di=-RANGE;di<=RANGE;++di){
					for(int dj=-RANGE;dj<=RANGE;++dj){
						int i=player.first,j=player.second;
						if(isescape(i+di,j+dj)) continue;
						if(!iscover[i+di][j+dj]) continue;
						if(make_pair(i+di,j+dj)==player) continue;
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
		}
	}
	void ShowEnd(double cost){
		PrintInfo(cost);
		gotoxy(Maze::n+DROW+3,0); setColor(7);
		MediatePrint(">>> Press Enter to Play Again or Press Esc to Exit <<<");
	}
#undef MAXSIZ
#undef BOUND
#undef WALL
#undef ROAD
}
int main(){
	system("chcp 65001"); //use utf-8
	srand(time(0));
	hideCursor();
	Settings::ReadinSettings();
	setConsoleSize(Settings::MazeHeight+8,Settings::MazeWeight*2+35);
	while(1){
		Maze::GenerateMaze(Settings::MazeHeight,Settings::MazeWeight);
		Maze::PrintInfo();
		Maze::ShowMaze();
		double start_time=RUNTIME;
		Maze::RunGame();
		Maze::ShowEnd(RUNTIME-start_time);
		int press=getch();
		while(press!=ESC&&press!=ENTER) press=getch();
		if(press==ESC) break;
	}
	Settings::WriteoutSettings();
	setColor(7); system("cls");
	return 0;
}
