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
		file<<"// MazeType �����Թ��������㷨:"<<std::endl;
		file<<"// 1 : ��·Ť���� , 2 : ��Ȼ�ֲ���"<<std::endl;
		file<<">MazeType: "<<MazeType<<std::endl;
		file<<"// MazeShape �����Թ�����״:"<<std::endl;
		file<<"// 1 : ���� , 2 : ����"<<std::endl;
		file<<">MazeShape: "<<MazeShape<<std::endl;
		file<<"// MazeHeight & Weight �����Թ��ĸ߶ȺͿ��:"<<std::endl;
		file<<"// Ϊ��֤��ʾЧ��, ����Ӧ���� [11,41] ֮��"<<std::endl;
		file<<"// ���Ҫ���ɷǾ��ε��Թ�, �������ÿ��ܲ�������"<<std::endl;
		file<<">MazeHeight: "<<MazeHeight<<std::endl;
		file<<">MazeWeight: "<<MazeWeight<<std::endl;
		file<<"// Covered �����Ƿ�Ҫ���Թ��ϸ�������"<<std::endl;
		file<<"Covered: "<<(isCover?"true":"false")<<std::endl;
		file.close();
	}
}
