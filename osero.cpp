#include<iostream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<time.h>
using namespace std;

int roottable[10][10]={
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

class Pos{
public:
	int x;
	int y;
};

int width=20;
int tablevalue=1000;

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

int reverse(int x,int y,int i,vector<vector<int> > table,int playernumber){
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
	for(int i=0;i<ysize;i++){
		for(int j=0;j<table[i].size();j++){
			if(table[i][j]==0)a++;
			else if(table[i][j]==1)b++;
			else if(table[i][j]==-1)c++;
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
	for each (vector<int> vec in table)
	{
		for each (int ind in vec){
			if(table[i][j]==playernumber)
			{
				mikata++;
			}else{
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

int monte(vector<vector<int> > table,int playernumber,int color,int depth,int d,vector<int>& tabletimes){
	srand(time(NULL));
	vector<int> randlist;
	if(tabletimes[d]==tablevalue){
		return 0;
	}
	if(nullmap(table)==1||(getcanpos(table,1).size()==0 and getcanpos(table,-1).size()==0)){
		tabletimes[d]++;
		return judge(table,color);
	}
	vector<Pos> poslist=getcanpos(table,playernumber);
	int count=0;
	int wi=poslist.size();
	if(wi==0){
		return monte(table,-playernumber,playernumber,depth,d,tabletimes);
	}

	for(int i=0;i<min(wi,width);i++){
		int randindex=0;
		while(1){
			int flg=0;
			randindex=rand()%wi;
			for(int j=0;j<randlist.size();j++){
				if(randlist[j]==randindex){
					flg=1;
					break;
				}
			}
			if(flg==0)break;
		}
		vector<vector<int> > tablemap=table;
		tablemap=turned(poslist[i],tablemap,playernumber);
		count+=monte(tablemap,-playernumber,playernumber,depth+1,d,tabletimes);
	}
	return count;

}


int launchmonte(vector<vector<int> > table,int playernumber,Pos pos,int i,vector<double>& valuelist){
	int depth=1;
	vector<vector<int> > tablemap=table;
	tablemap=turned(pos,tablemap,playernumber);
	vector<int> tabletimes(tablevalue,0);
	valuelist[i]=monte(tablemap,-playernumber,playernumber,depth,i,tabletimes)/tablevalue;
	return 0;
}






int drawmap(vector<vector<int> > table,int playernumber){
	int y=table.size();
	vector<Pos> can=getcanpos(table,playernumber);
	printf("   ");
	for(int i=0;i<10;i++){
		printf("%3d",i+1);
	}
	printf("\n");
	for (int i = 0; i < y; i++){
		printf("%3d",i+1);
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
	vector<long double> value(n);
	vector<long double> trycount(n);
	vector<int> winpercent(n);
	for(int i=0;i<n;i++)
	{
		trycount[i]+=1;

	}
	return posli[0];
}



int main(){
	vector<vector<int> > table(10);
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			table[i].push_back(roottable[i][j]);
		}
	}
	return 0;
}