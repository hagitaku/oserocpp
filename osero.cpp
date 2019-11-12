#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
#include<cmath>
using namespace std;

vector<vector<int> > roottable(10,vector<int>(10));
int player[2]={1,-1};
int playercolor;
int turn=0;
double k=1;
int init()
{
	cout<<"inputcolor(1 or -1)\nblack:1,white:-1:";
	cin>>playercolor;
	roottable={
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 1,-1, 0, 0, 0, 0},
		{ 0, 0, 0, 0,-1, 1, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
		return 0;
}

class Pos{
public:
	int x;
	int y;
};

int width=20;
int tablevalue=100;

int dirx[8]={-1, 0, 1, 1, 1, 0,-1,-1};
int diry[8]={-1,-1,-1, 0, 1, 1, 1, 0};

int checkfield(int x,int y){

	return (0<=x&&x<=9&&0<y&&y<=9)?1:0;
}

int canputflg(int x,int y,vector<vector<int> > table,int i,int playernumber){
	if(checkfield(x,y)==0){
		return 0;
	}else if(table[y][x]==0){
		return 0;
	}else if(table[y][x]==playernumber){
		return 1;
	}else{
		return canputflg(x+dirx[i],y+diry[i],table,i,playernumber);
	}
}

int canput(int x,int y,vector<vector<int> > table,int playernumber){
	if(table[y][x]!=0){
		return 0;
	}
	if(checkfield(x,y)==0){
		return 0;
	}
	for(int i=0;i<8;i++){
		if(checkfield(x+dirx[i],y+diry[i])==1&&table[y+diry[i]][x+dirx[i]]==-1*playernumber){
			if(canputflg(x+dirx[i],y+diry[i],table,i,playernumber)==1){
				return 1;
			}
		}
	}
	return 0;
}

int reverse(int x,int y,int i,vector<vector<int> >& table,int playernumber){
	if(checkfield(x,y)==0||table[y][x]==0)
	{
		return 0;
	}
	if(table[y][x]==playernumber)
	{
		return 1;
	}
	if(reverse(x+dirx[i],y+diry[i],i,table,playernumber)==1)
	{
		table[y][x]=playernumber;
		return 1;
	}

	return 0;
}

vector<vector<int> > turned(Pos pos,vector<vector<int> > table,int playernumber){
	vector<vector<int> > tablemap=table;
	if(canput(pos.x,pos.y,tablemap,playernumber)==1){
		tablemap[pos.y][pos.x]=playernumber;
		for (int i = 0; i < 8; i++){
			reverse(pos.x+dirx[i],pos.y+diry[i],i,tablemap,playernumber);
		}
	}
	return tablemap;
}



int nullmap(vector<vector<int> > table){
	int a=0,b=0,c=0;
	int ysize=table.size();
	for(auto itr:table){
		for(auto idx:itr){
			if(idx==0)a++;
			else if(idx==1)b++;
			else if(idx==-1)c++;
		}
	}
	if(a==0){
		return 1;
	}else if(b==0 || c==0){
		return 1;
	}
	return 0;
}


vector<Pos> getcanpos(vector<vector<int> > table,int playernumber){
	vector<Pos> list;
	for(int i=0;i<table.size();i++){
		for(int j=0;j<table[i].size();j++){
			if(canput(j,i,table,playernumber)==1){
				Pos pos;
				pos.x=j;
				pos.y=i;
				list.push_back(pos);
			}
		}
	}
	return list;
}


int judge(vector<vector<int> > table,int playernumber){
	int mikata=0;
	int teki=0;
	for(auto itr:table){
		for(auto idx:itr){
			if(idx==playernumber){
				mikata++;
			}else if(idx==-playernumber){
				teki++;
			}
		}
	}

	if(teki<mikata){
		return 1;
	}else{
		return 0;
	}
}

int monte(vector<vector<int> > table,int playernumber,int color){
	if(nullmap(table)==1||(getcanpos(table,1).size()==0 && getcanpos(table,-1).size()==0)){
		return judge(table,color);
	}
	vector<Pos> poslist=getcanpos(table,playernumber);
	for(int i=0;i<poslist.size();i++){
		if((poslist[i].y==0&&poslist[i].x==0)||(poslist[i].y==0&&poslist[i].x==9)||(poslist[i].y==9&&poslist[i].x==0)||(poslist[i].y==9&&poslist[i].x==9))
		{
			return monte(turned(poslist[i],table,playernumber),-playernumber,playernumber);
		}
	}
	int count=0;
	int wi=poslist.size();
	if(wi==0){
		//pass
		return monte(table,-playernumber,color);
	}
	return monte(turned(poslist[rand()%wi],table,playernumber),-playernumber,playernumber);
}



int drawmap(vector<vector<int> > table,int playernumber){
	int y=table.size();
	vector<Pos> can=getcanpos(table,playernumber);
	printf("   ");
	for(int i=0;i<10;i++){
		printf("%3d",i);
	}
	printf("\n");
	for (int i = 0; i < y; i++){
		printf("%3d",i);
		for (int j = 0; j < table[i].size(); j++){
			int flg=0;
			for(int k=0;k<can.size();k++){
				if(can[k].y==i&&can[k].x==j){
					flg=1;
					break;
				}
			}
			if(flg==1){
				printf("  *");
			}else if(table[i][j]==0){
				printf("   ");
			}else{
				printf("%3d",table[i][j]);
			}
			
		}
		printf("\n");
	}
	return 0;
}


Pos runAI(vector<vector<int> > table,int playernumber){
	vector<Pos> posli=getcanpos(table,playernumber);
	int n=posli.size();
	for(int i=0;i<n;i++){
		if((posli[i].y==0&&posli[i].x==0)||(posli[i].y==0&&posli[i].x==9)||(posli[i].y==9&&posli[i].x==0)||(posli[i].y==9&&posli[i].x==9))
		{
			return posli[i];
		}
	}
	int valuelist[n];
	for(int i=0;i<n;i++){
		valuelist[i]=0;
	}
	vector<int> trycount(n);
	vector<long double> ucbvalue(n);
	for(int i=0;i<n;i++)
	{
		auto ta=turned(posli[i],table,playernumber);
		valuelist[i]+=monte(ta,-playernumber,playernumber);
		trycount[i]+=1;
	}
	int maxindex=0;
	for(int i=4;i<=tablevalue;i++){
		maxindex=0;
		for(int k=0;k<n;k++){
			ucbvalue[k]=(double)valuelist[k]/(double)trycount[k]+k*sqrt((log(i)/(double)trycount[k]));
		}
		for(int k=1;k<n;k++){
			if(ucbvalue[maxindex]<ucbvalue[maxindex]){
				maxindex=k;
			}
		}
		auto ta=turned(posli[maxindex],table,playernumber);
		valuelist[maxindex]+=monte(ta,-playernumber,playernumber);
		trycount[maxindex]+=1;
		cout<<i<<endl;
	}
	for(int k=1;k<n;k++){
		if(ucbvalue[maxindex]<ucbvalue[maxindex]){
			maxindex=k;
		}
	}
	return posli[maxindex];
}



int main(){
	srand(time(NULL));
	Pos pos;
	init();//盤面の初期化
	int nowplayer=player[turn%2];
	for(;;turn++)
	{
	nowplayer=player[turn%2];

	if(nowplayer==playercolor)
		{
			drawmap(roottable,nowplayer);
			cout<<"input x y:";
			while(true){
				char are;
				cin>>pos.x>>are>>pos.y;
				if(checkfield(pos.x,pos.y)==1&&canput(pos.x,pos.y,roottable,nowplayer)==1){
					roottable=turned(pos,roottable,nowplayer);
					break;
				}
				system("cls");
				cout<<"Invalid.\nPlaease type it again\n";
			}
		}else{
			pos=runAI(roottable,nowplayer);
			roottable=turned(pos,roottable,nowplayer);
		}
	}
	return 0;
}