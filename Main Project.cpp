#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<string>
#include"bag/cdk.cpp"

using namespace std;
SYSTEMTIME sys;

//显示设置
const string title="兮辞_xicios";//标题
const char *lab="Main Project";//副标题
const string ibate="sv1.7.0",t_cx="2025.01.21 12:00";//程序详情
string set_link="items/set.ini";//配置链接

//基础配置
int base=60,tplus=600,stpl=74,maxpl=90;//五星概率
int fase=510,flus=5100,ftpl=9,faxpl=10;//四星概率
string bate,t_day,t_time,made_by,made_co;//卡池内容
int ten=10;//十连次数
int card=8,page=1;//当前选择
int all=0,res=0;//卡池总数,常驻数
bool resp[300];//常驻判定
const int cpmax=200;
string from[cpmax],name[cpmax],five[cpmax],four[cpmax][3];//卡池内容
int mfive=0,mfour=0,mthree=0;//通用数量
string afive[10],afour[150],athree[50];//通用内容
bool first=true;//读入限制
int wait=1220;//十连等待时间
char musicPath[20]="items\\set*.ini";//配置文件路径

//功能开关
bool outset=false;//set加密
bool gls=false;//概率公示
bool boom=true;//改变出率
bool copyright=false;//校验文件
bool creadin=true;//切换配置文件

//抽卡记录
const int alto=10000;//存储容量
bool big=false;//保底机制
int small=1;//四星统计
int alt=0,now=0;//统计
string history[alto],level[alto];//存储

//公告栏
int ads_max=6;
string ads[20]= {"原作者不对任何配置文件负责",\
                 "广告位招租！！！",\
                 "и'(?)_∵д*?(古神の低语)",\
                 "2025新春快乐喵~",\
                 "会自动打开大写锁定了哦~",\
                 "copyright 2023~2025 兮辞_xicios"\
                };

//调用区
void sets();
void ads_show();
void save();
void setCursorPosition(int x, int y);

int main () {
	//初始化
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
	SetConsoleTitle(lab);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//隐藏控制台光标
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);
	if (!GetKeyState(VK_CAPITAL)) {//设置大写锁定
		keybd_event(VK_CAPITAL, 0, 0, 0);
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
	}
	//#通行证校验
	int pass=cdk();
	if (pass!=1&&copyright==true) {
		//if (pass!=3) remove("items/set.ini");
		exit(0);
	}
	system("cls");
	//读入
	if (first==true) {
		//读入临时文件
		ifstream tin("items/temp.ini");
		tin>>page>>card>>ten;
		tin>>base>>tplus>>stpl>>maxpl;
		tin>>fase>>flus>>ftpl>>faxpl;
		tin>>set_link;
		tin.close();
		//校验配置文件
		WIN32_FIND_DATA findFileData;
		HANDLE hFind=FindFirstFile(musicPath,&findFileData);
		if (hFind==INVALID_HANDLE_VALUE&&creadin==false&&outset==false) {
			cout<<"[Error]未查询到配置文件"<<endl;
			Sleep(3000);
			exit(0);
		}
		//读入广告
		ifstream ain("items/ads.ini");
		int ads_plus=0;
		ain>>ads_plus;
		if (ads_plus>0&&(ads_plus+ads_max)<=20) for (int ls=0; ls!=ads_plus; ls++,ads_max++) ain>>ads[ads_max];
		ain.close();
		//读入卡池
		//if (outset==true) set_main();
		ifstream fin(set_link);
		fin>>made_by>>made_co>>bate>>t_day>>t_time;
		if (bate=="\0") sets();//配置文件为空
		fin>>mfive;
		for (int ls=0; ls!=mfive; ls++) {//五星
			fin>>afive[ls];
		}
		fin>>mfour;
		for (int ls=0; ls!=mfour; ls++) {//四星
			fin>>afour[ls];
		}
		fin>>mthree;
		for (int ls=0; ls!=mthree; ls++) {//三星
			fin>>athree[ls];
		}
		fin>>all>>res;
		for (int ls=0; ls!=res; ls++) {//读入常驻
			int temp=0;
			fin>>temp;
			resp[temp]=true;
		}
		for (int ls=1; ls!=all+1; ls++) {//读入卡池
			fin>>from[ls]>>name[ls]>>five[ls]>>four[ls][0]>>four[ls][1]>>four[ls][2];
		}
		//校验
		if (made_by=="\0") {	//原配置文件失效
			set_link="items/set.ini";
			save();
			main();
		}
		bool wrong=false;
		if (page<1||page>all-8&&all>8||card<1||card>all||ten<1||ten>1000) wrong=true;
		if (base<1||base>10000||tplus<1||tplus>10000||stpl<1||stpl>maxpl) wrong=true;
		if (fase<1||fase>10000||flus<1||flus>10000||ftpl<1||ftpl>faxpl) wrong=true;
		if (wrong==true) {
			int tcin[11]= {1,1,10,60,600,74,90,510,5100,9,10};
			for (int ls=0; ls!=11; ls++) fin>>tcin[ls];
			page=tcin[0],card=tcin[1],ten=tcin[2];
			base=tcin[3],tplus=tcin[4],stpl=tcin[5],maxpl=tcin[6];
			fase=tcin[7],flus=tcin[8],ftpl=tcin[9],faxpl=tcin[10];
			save();
			cout<<"[#请勿修改配置文件]"<<endl;
		}
		fin.close();
		//if (outset==true) set_del();
		first=false;
	}
	//显示
	system("cls");
	cout<<"「"<<title<<"」抽卡模拟器"<<endl;
	cout<<"制作者: 兮辞"<<endl;
	cout<<"卡池版本: "<<bate<<endl;
	ads_show();
	cout<<"--------------------------------------"<<endl;
	cout<<from[card]<<endl;
	if (resp[card]==false) cout<<"限定";
	else cout<<"常驻";
	cout<<"-"<<name[card]<<endl;
	cout<<"已抽:"<<now<<"发"<<endl;
	cout<<"--------------------------------------"<<endl;
	cout<<"本期 UP↑五星为:"<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
	cout<<" S "<<five[card]<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
	cout<<"本期 UP↑四星为:"<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
	for (int ls=0; ls!=3; ls++) cout<<" R "<<four[card][ls]<<endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
	cout<<"--------------------------------------"<<endl;
	cout<<">E.单次抽取"<<endl;
	cout<<">Q.多次抽取"<<endl;
	cout<<">A.查看记录"<<endl;
	cout<<">D.控制面板"<<endl;
	cout<<">S.程序配置"<<endl;
	cout<<"--------------------------------------"<<endl;
	char choose=getch();
	system("cls");
	if (choose=='S'||choose=='s') {
		cout<<"「"<<title<<"」抽卡模拟器"<<endl;
		cout<<"制作者: 兮辞"<<endl;
		cout<<"程序版本: "<<ibate<<endl;
		ads_show();
		cout<<"--------------------------------------"<<endl;
		cout<<"           「关于程序文件」"<<endl;
		cout<<"著作权人: 兮辞"<<endl;
		cout<<"版权归属: xicios.co"<<endl;
		cout<<"程序版本: "<<ibate<<endl;
		cout<<"程序更新时间: "<<t_cx<<endl;
		cout<<"--------------------------------------"<<endl;
		cout<<"           「关于卡池文件」"<<endl;
		cout<<"著作权人: "<<made_by<<endl;
		cout<<"版权归属: "<<made_co<<".co"<<endl;
		cout<<"卡池版本: "<<bate<<endl;
		cout<<"卡池更新时间: "<<t_day<<" "<<t_time<<endl;
		cout<<"--------------------------------------"<<endl;
		cout<<">按S查看更新内容"<<endl;
		if (creadin==true) cout<<">按X更改配置文件"<<endl;
		cout<<">按任意键退出本页面"<<endl;
		cout<<"--------------------------------------"<<endl;
		choose=getch();
		if (choose=='S'||choose=='s') {
			//变量
			int max[200],npage=1;
			string beta[200],day[200],time[200],out[200][15];
			//读取
			ifstream fin("items/news.ini");
			fin>>max[0];
			if (max[0]<=0) main();
			for (int ls=1; ls<=max[0]; ls++) {
				fin>>beta[ls]>>day[ls]>>time[ls]>>max[ls];
				for (int cs=0; cs<=max[ls]; cs++) {
					getline(fin,out[ls][cs]);
				}
			}
			fin.close();
			//显示
			while (true) {
				system("cls");
				cout<<beta[npage]<<"更新说明("<<npage<<"/"<<max[0]<<")"<<endl;
				cout<<"更新时间:"<<day[npage]<<" "<<time[npage]<<endl;
				cout<<"--------------------------------------"<<endl;
				for (int ls=1; ls<=max[npage]; ls++) {
					cout<<out[npage][ls]<<endl;
				}
				cout<<"--------------------------------------"<<endl;
				cout<<">按'A'或'D'切换页面"<<endl;
				cout<<">按任意键退出本页面..."<<endl;
				choose=getch();
				if (choose=='A'||choose=='a') {
					if (npage>1) npage-=1;
					//else npage=max[0];
				} else if (choose=='D'||choose=='d') {
					if (npage<max[0]) npage+=1;
					//else npage=1;
				} else main();
			}
		} else if (choose=='X'||choose=='x') sets();//更改配置文件
	} else if (choose=='A'||choose=='a') {//查看记录
		cout<<"历史记录"<<endl;
		cout<<"--------------------------------------"<<endl;
		int tj=1,S=0,R=0,A=0;
		if (alt<=0) cout<<"暂时还没有抽卡记录哦，快去抽卡吧。"<<endl;
		for (int ls=0; ls!=alt; ls++) {
			if (level[ls]=="S") {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
				S+=1;
			} else if (level[ls]=="R") {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
				R+=1;
			} else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
				A+=1;
			}
			if (tj<10) cout<<"0";
			cout<<tj<<" "<<level[ls]<<" "<<history[ls]<<endl;
			if (level[ls]=="S") {
				tj=0;
				Sleep(1500);
			}
			tj+=1;
			Sleep(20);
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
		cout<<"--------------------------------------"<<endl;
		cout<<"共计抽了:"<<alt<<"发"<<endl;
		cout<<"五星:"<<S<<"个"<<endl;
		cout<<"四星:"<<R<<"个"<<endl;
		cout<<"三星:"<<A<<"个"<<endl;
		char got=getch();
		main();
	} else if (choose=='D'||choose=='d') {//控制面板
		cout<<"控制面板"<<endl;
		cout<<"--------------------------------------"<<endl;
		cout<<"共计:"<<alt<<"抽"<<endl;
		if (big==true) cout<<"[大";
		else cout<<"[小";
		cout<<"保底]已垫:"<<now<<"抽"<<endl;
		cout<<"--------------------------------------"<<endl;
		cout<<">Q.退出程序"<<endl;
		cout<<">I.导出记录"<<endl;
		cout<<">O.清零保底"<<endl;
		cout<<">P.清空记录"<<endl;
		cout<<">J.抽取次数"<<endl;
		if (boom==true) cout<<">K.修改概率"<<endl;
		else cout<<"K.功能锁定"<<endl;
		cout<<">L.切换卡池"<<endl;
		cout<<"--------------------------------------"<<endl;
		choose=getch();
		system("cls");
		if (choose=='Q') { //退出程序
			save();
			exit(0);
		} else if (choose=='I') {//导出记录
			int best=0,R=0,S=0,A=0;
			if (alt!=0) {
				ofstream fout("history.txt",ios::app);
				GetLocalTime(&sys);
				fout<<sys.wYear<<"/"<<sys.wMonth<<"/"<<sys.wDay<<" "<<sys.wHour<<":"<<sys.wMinute<<":"<<sys.wSecond;
				fout<<" 的抽卡记录:"<<endl;
				fout<<"-------------------------"<<endl;
				if (alt<=0) cout<<"暂时还没有抽卡记录哦qaq。"<<endl;
				for (int ls=0; ls!=alt; ls++) {
					best+=1;
					if (level[ls]=="S") S+=1;
					if (level[ls]=="R") R+=1;
					if (level[ls]=="A") A+=1;
					if (best<10) fout<<"0";
					fout<<best<<" "<<level[ls]<<" "<<history[ls]<<endl;
					if (level[ls]=="S") best=0;
				}
				fout<<"-------------------------"<<endl;
				fout<<"共计抽了:"<<alt<<"发"<<endl;
				fout<<"五星:"<<S<<"个"<<endl;
				fout<<"四星:"<<R<<"个"<<endl;
				fout<<"三星:"<<A<<"个"<<endl;
				fout<<"--------------------------------------"<<endl;
				cout<<endl;
				fout.close();
			} else {
				MessageBox(NULL,"暂无抽卡记录可以导出","提示",MB_OK+16);
			}
			main();
		} else if (choose=='O') {//清零保底
			if (MessageBox(NULL,"确定要清零保底吗","提示",MB_YESNO+64)==6) {
				now=0;
				big=false;
				small=0;
			}
			main();
		} else if (choose=='P') {//清空记录
			if (MessageBox(NULL,"确定要清空记录吗","提示",MB_YESNO+64)==6) {
				alt=0,now=0;
				big=false;
				small=0;
			}
			main();
		} else if (choose=='J') {//修改次数
			cout<<"多次抽取"<<endl;
			cout<<"--------------------------------------"<<endl;
			cout<<"当前抽取次数:"<<ten<<endl;
			cout<<"抽取次数(可选择:1~100[默认:10]):";
			cin>>ten;
			if(ten<1||ten>100) ten=10;
			save();
			main();
		} else if (choose=='K'&&boom==true) {//修改概率
			cout<<"修改概率"<<endl;
			cout<<"--------------------------------------"<<endl;
			cout<<"当前出金率:"<<base<<endl;
			cout<<"修改出金率(0-10000[默认:60]):";
			cin>>base;
			if (base<0||base>10000) base=60;
			save();
			main();
		} else if (choose=='L') {//卡池切换
			bool tch[10]= {false};
			system("cls");
			cout<<"卡池切换"<<endl;
			cout<<"--------------------------------------"<<endl;
			if (all>9) setCursorPosition(0,20);
			else setCursorPosition(0,(1+all)*2);
			cout<<"--------------------------------------"<<endl;
			if (all<10) cout<<" W.上翻查    共计:"<<all<<"个卡池    下翻查.S"<<endl;
			else if (all<100) cout<<" W.上翻查   共计: "<<all<<"个卡池   下翻查.S"<<endl;
			else if (all<1000) cout<<" W.上翻查   共计:"<<all<<"个卡池   下翻查.S"<<endl;
			cout<<"--------------------------------------"<<endl;
			while (true) {
				//system("cls");
				setCursorPosition(0,2);
				for (int ls=0; ls!=9; ls++) {
					if(name[page+ls]!="\0"&&(page+ls)<=all) {
						cout<<ls+1<<".「(#"<<page+ls<<")"<<name[page+ls]<<"」               "<<endl;
						cout<<" >可获得: ";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
						cout<<five[page+ls]<<"               "<<endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
						tch[page+ls]=true;
					}
				}
				choose=getch();
				if (choose!='W'&&choose!='S'&&choose!='A'&&choose!='D'&&(choose<'1'||choose>'9')) {
					main();
				} else if (choose=='W'&&page>1) {
					page-=1;
					save();
				} else if (choose=='S'&&page<all-8) {
					page+=1;
					save();
				} else if (choose=='A'&&page>1) {
					if (page>9) page-=9;
					else page=1;
					save();
				} else if (choose=='D'&&page<all-8) {
					if (page<all-17) page+=9;
					else page=all-8;
					save();
				} else if (choose>='1'&&choose<='9') {
					if (tch[page-1+choose-'0']==true) {//校验卡池是否存在
						card=page-1+choose-'0';
						save();
						main();
					}
				}
			}
		} else main();
	} else if (choose=='E'||choose=='Q') {//抽卡
		int again=1;
		if (choose=='Q') again=ten;
		while (again!=0) {
			
			small+=1;
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			mt19937 generator (seed);
			int ls1=generator()%10000+1;
			double pro=base;
			if (now>=stpl) pro+=tplus*(now-stpl+1);
			double fro=fase,tro=10000-base-fase;
			if (small>=ftpl) fro+=flus*(small-ftpl+1);
			fro=(fro/10000)*(10000-pro);
			if (pro+fro>=10000) {
				tro=0;
				fro=10000-pro;
			} else tro=10000-pro-fro;
			if (ls1<=pro) {//五星
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				mt19937 generator (seed);
				int ls2=generator()%2+1;
				now=-1;
				level[alt]="S";
				if (ls2==1&&big==false) {//歪了
					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					mt19937 generator (seed);
					int ls3=generator()%mfive;
					big=true;
					history[alt]=afive[ls3];
				} else {//没歪
					big=false;
					history[alt]=five[card];
				}
			} else if (ls1<tro+pro) {//三星
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),1);
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				mt19937 generator (seed);
				int ls2=generator()%mthree;
				level[alt]="A";
				history[alt]=athree[ls2];
			} else {//四星
				small=0;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
				unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				mt19937 generator (seed);
				int ls2=generator()%2+1;
				level[alt]="R";
				if (ls2==1) {//歪了
					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					mt19937 generator (seed);
					int ls3=generator()%mfour;
					history[alt]=afour[ls3];
				} else {//没歪
					unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
					mt19937 generator (seed);
					int ls3=generator()%3;
					history[alt]=four[card][ls3];
				}
			}
			cout<<level[alt]<<" "<<history[alt]<<endl;
			alt+=1;
			now+=1;
			again-=1;
			if (choose=='Q') Sleep(16);
			if (gls==true) cout<<pro<<" "<<fro<<" "<<tro<<" "<<ls1<<endl;//[调试]概率显示
		}
		if (choose=='E') Sleep(200);
		else Sleep(wait);
		main();
	}
	main();
	return 0;
}

//调用区

void sets () {
	//更改配置文件
	if (creadin==false) {
		//移除使用本功能
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0c);
		cout<<"[Error] 相关功能已被开发者锁定。"<<endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
		Sleep(2000);
		main();
	}
	//路径搜索
	WIN32_FIND_DATA findFileData;
	HANDLE hFind=FindFirstFile(musicPath,&findFileData);
	//显示详细文件名用
	int i=0;
	int j=0;
	char path[55][50]= {};
	//显示
	system("cls");
	cout<<"「"<<title<<"」抽卡模拟器"<<endl;
	cout<<"制作者: 兮辞"<<endl;
	cout<<"程序版本: "<<ibate<<endl;
	ads_show();
	cout<<"--------------------------------------"<<endl;
	int set_all=0;//计数
	if(hFind==INVALID_HANDLE_VALUE) {
		cout<<"[Error]未查询到配置文件"<<endl;
		Sleep(3000);
		exit(0);
	} else {
		cout<<"配置文件列表："<<endl;
		while(1) {
			if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			} else {
				set_all+=1;
				char str[50] = "items/";
				memcpy(str+6, findFileData.cFileName, strlen(findFileData.cFileName));
				sprintf(path[i],str);
				//cout<<strlen(findFileData.cFileName)<<" "<<findFileData.cFileName<<endl;  //DEBUG
				i++;
				printf("%3d : %s\n",i,findFileData.cFileName);
			}
			if(!FindNextFile(hFind,&findFileData))	break;
		}
		FindClose(hFind);
	}
	cout<<"--------------------------------------"<<endl;
	bool fp=true;
	if (set_all!=1) {
		cout<<"输入序号读取配置文件：";
		cin>>j;
		while ('\n'!=getchar());
		if(j<1||j>i) fp=false;
		else set_link=path[j-1];
	} else set_link=path[0];
	save();
	cout<<endl;
	if (fp==false) cout<<"非法输入！"<<endl;
	else cout<<"配置文件'"<<set_link<<"'读取成功！"<<endl;
	first=true;
	Sleep(1500);
	main();
}

void ads_show() {
	//公告栏
	if (ads_max>0) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		mt19937 generator (seed);
		int ads_no=generator()%ads_max;
		cout<<"#"<<ads[ads_no]<<endl;
	}
}

void save() {
	//保存
	ofstream fout("items/temp.ini");
	fout<<page<<" "<<card<<" "<<ten<<endl;
	fout<<base<<" "<<tplus<<" "<<stpl<<" "<<maxpl<<endl;
	fout<<fase<<" "<<flus<<" "<<ftpl<<" "<<faxpl<<endl;
	fout<<set_link<<endl;
	fout.close();
}

void setCursorPosition(int x, int y) { //自定义光标定位
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
