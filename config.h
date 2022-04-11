#include<fstream>
#include<cstring>
namespace Settings{
#define CONFIG_FILE "config.ini"
	int MazeType=1; //nature single
	int MazeShape=1; //rect
	int MazeHeight=21,MazeWeight=31;
	bool isCover=1;
	void ReadinSettings(){
		std::ifstream file(CONFIG_FILE);
		if(file.eof()){file.close(); return;}
		std::string str;
		while(file>>str){
			if(str=="//") getline(file,str);
			else{
				if(str==">MazeType:"){
					int x; file>>x;
					if(x==1||x==2) MazeType=x;
				}
				else if(str==">MazeShape:"){
					int x; file>>x;
					if(x==1||x==2) MazeShape=x;
				}
				else if(str==">MazeWeight:"){
					int x; file>>x;
					if(x>=5&&x<=31) MazeWeight=x;
				}
				else if(str==">MazeHeight:"){
					int x; file>>x;
					if(x>=11&&x<=41) MazeHeight=x;
				}
				else if(str==">Covered:"){
					file>>str;
					if(str=="true") isCover=true;
					if(str=="false") isCover=false;
				}
			}
		}
		if(MazeShape==2){ //is heart
			MazeHeight=37; MazeWeight=37;
		}
		file.close();
	}
	void WriteoutSettings(){
		std::ofstream file(CONFIG_FILE);
		file<<"// MazeType 控制迷宫的生成算法:"<<std::endl;
		file<<"// 1 : 主路扭曲型 , 2 : 自然分岔型"<<std::endl;
		file<<">MazeType: "<<MazeType<<std::endl;
		file<<"// MazeShape 控制迷宫的形状:"<<std::endl;
		file<<"// 1 : 矩形 , 2 : 心形"<<std::endl;
		file<<">MazeShape: "<<MazeShape<<std::endl;
		file<<"// MazeHeight & Weight 控制迷宫的高度和宽度:"<<std::endl;
		file<<"// 为保证显示效果, 它们应处于 [11,41] 之间"<<std::endl;
		file<<"// 如果要生成非矩形的迷宫, 该项设置可能不起作用"<<std::endl;
		file<<">MazeHeight: "<<MazeHeight<<std::endl;
		file<<">MazeWeight: "<<MazeWeight<<std::endl;
		file<<"// Covered 决定是否要在迷宫上覆盖遮罩"<<std::endl;
		file<<"Covered: "<<(isCover?"true":"false")<<std::endl;
		file.close();
	}
}
