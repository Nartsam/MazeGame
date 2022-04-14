#include<fstream>
#include<cstring>
namespace Settings{
#define CONFIG_FILE "config.ini"
	int MazeType=1; //nature-single
	int MazeShape=1; //rectangle
	int MazeHeight=21,MazeWeight=31;
	bool isCover=1;
	void ReadinSettings(){
		std::ifstream file(CONFIG_FILE);
		if(file.eof()){file.close(); return;}
		std::string str;
		while(file>>str){
			if(str.size()>=2&&str[0]=='/'&&str[1]=='/') getline(file,str);
			else{
				if(str==">MazeType:"){
					int x; file>>x;
					if(x==1||x==2) MazeType=x;
				}
				else if(str==">MazeShape:"){
					int x; file>>x;
					if(x==1||x==2) MazeShape=x;
				}
				else if(str==">MazeHeight:"){
					int x; file>>x;
					if(x>=11&&x<=31) MazeHeight=x;
				}
				else if(str==">MazeWeight:"){
					int x; file>>x;
					if(x>=11&&x<=51) MazeWeight=x;
				}
				else if(str==">Covered:"){
					file>>str;
					if(str=="true") isCover=true;
					if(str=="false") isCover=false;
				}
			}
		}
		if(MazeShape==2){ //is heart
			MazeHeight=27; MazeWeight=47;
		}
		file.close();
	}
	void WriteoutSettings(){
		std::ofstream file(CONFIG_FILE);
		file<<"// MazeType �����Թ��������㷨: {1: ��·Ť����, 2: ��Ȼ�ֲ���}"<<std::endl;
		file<<">MazeType:   "<<MazeType<<std::endl;
//		file<<"// MazeShape �����Թ�����״: {1: ����, 2: ����}"<<std::endl;
//		file<<">MazeShape:   "<<MazeShape<<std::endl;
		file<<"// MazeHeight & Weight �����Թ��ĸ߶ȺͿ��, ��Χ: 11~31/11~51"<<std::endl;
		file<<"// Ϊ��֤��ʾЧ��, ���Ҫ���ɷǾ��ε��Թ�, �������ÿ��ܲ�������"<<std::endl;
		file<<">MazeHeight:   "<<MazeHeight<<std::endl;
		file<<">MazeWeight:   "<<MazeWeight<<std::endl;
		file<<"// Covered �����Ƿ�Ҫ���Թ��ϸ�������"<<std::endl;
		file<<">Covered:   "<<(isCover?"true":"false")<<std::endl;
		file.close();
	}
}
