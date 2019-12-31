#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define WIDTH 10
#define MAX 4096
using namespace std;
random_device rnd;
int drawmap(int table[WIDTH][WIDTH], int playernumber);
int player[2] = { 1,-1 };
int playercolor;
int swi = 0;
double are = 0.333;
int roottable[WIDTH][WIDTH];
int judgenum(char a){
	if('0'<=a&&a<='9'){
		return 1;
	}else{
		return 0;
	}
}

int init()
{
	cout << "inputcolor(1 or -1)\nblack:1,white:-1:";
	string s;
	cin >> s;
	if(s=="1"){
		playercolor=1;
	}else if(s=="-1"){
		playercolor=-1;
	}else{
		cout<<"plaese type it again\n";
		return init();
	}
	return 0;
}

class Pos {
public:
	int x;
	int y;
};

int width = 20;
int tablevalue = 8000;

int dirx[8] = { -1, 0, 1, 1, 1, 0,-1,-1 };
int diry[8] = { -1,-1,-1, 0, 1, 1, 1, 0 };

int checkfield(int x, int y) {

	return (0 <= x && x <= 9 && 0 <= y && y <= 9) ? 1 : 0;
}

int deepcopy(int root[WIDTH][WIDTH], int table[WIDTH][WIDTH]) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			table[i][j] = root[i][j];
		}
	}
	return 0;
}

int canputflg(int x, int y, int table[WIDTH][WIDTH], int i, int playernumber) {
	if (checkfield(x, y) == 0) {
		return 0;
	}
	else if (table[y][x] == 0) {
		return 0;
	}
	else if (table[y][x] == playernumber) {
		return 1;
	}
	else {
		return canputflg(x + dirx[i], y + diry[i], table, i, playernumber);
	}
}

int canput(int x, int y, int table[WIDTH][WIDTH], int playernumber) {
	if (table[y][x] != 0) {
		return 0;
	}
	if (checkfield(x, y) == 0) {
		return 0;
	}
	for (int i = 0; i < 8; i++) {
		if (checkfield(x + dirx[i], y + diry[i]) == 1 && table[y + diry[i]][x + dirx[i]] == -1 * playernumber) {//範囲内&&向いている方向が敵だったら
			if (canputflg(x + dirx[i], y + diry[i], table, i, playernumber) == 1) {
				return 1;
			}
		}
	}
	return 0;
}

int reverse(int x, int y, int i, int table[WIDTH][WIDTH], int playernumber) {
	if (checkfield(x, y) == 0 || table[y][x] == 0)
	{
		return 0;
	}
	if (table[y][x] == playernumber)
	{
		return 1;
	}
	if (reverse(x + dirx[i], y + diry[i], i, table, playernumber) == 1)
	{
		table[y][x] = playernumber;
		return 1;
	}

	return 0;
}

int turn(Pos pos, int table[WIDTH][WIDTH], int playernumber) {
	if (canput(pos.x, pos.y, table, playernumber) == 1) {
		table[pos.y][pos.x] = playernumber;
		for (int i = 0; i < 8; i++) {
			reverse(pos.x + dirx[i], pos.y + diry[i], i, table, playernumber);
		}
	}
	return 0;
}



int nullmap(int table[WIDTH][WIDTH]) {
	int a = 0, b = 0, c = 0;
	int ysize = WIDTH;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (table[i][j] == 0)a++;
			else if (table[i][j] == 1)b++;
			else if (table[i][j] == -1)c++;
		}
	}
	if (a == 0) {
		return 1;
	}
	else if (b == 0 || c == 0) {
		return 1;
	}
	return 0;
}


vector<Pos> getcanpos(int table[WIDTH][WIDTH], int playernumber) {
	vector<Pos> list;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (canput(j, i, table, playernumber) == 1) {
				Pos pos;
				pos.x = j;
				pos.y = i;
				list.push_back(pos);
			}
		}
	}
	return list;
}



int judge(int table[WIDTH][WIDTH], int playernumber) {//勝ちだったら1,負けは0
	int mikata = 0;
	int teki = 0;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (table[i][j] == playernumber) {
				mikata++;
			}
			else if (table[i][j] == -playernumber) {
				teki++;
			}
		}
	}
	if (teki < mikata) {
		return 1;
	}
	else {
		return 0;
	}
}

string judgest(int table[WIDTH][WIDTH], int playernumber) {
	string re;
	if (judge(table, playernumber) == 1) {
		re = "Win";
		return re;
	}
	else {
		re = "Lose";
		return re;
	}
};



int drawmap(int table[WIDTH][WIDTH], int playernumber) {
	int y = WIDTH;
	vector<Pos> can = getcanpos(table, playernumber);
	printf("   ");
	for (int i = 0; i < 10; i++) {
		printf("%3d", i);
	}
	printf("\n");
	for (int i = 0; i < y; i++) {
		printf("%3d", i);
		for (int j = 0; j < WIDTH; j++) {
			int flg = 0;
			for (int k = 0; k < can.size(); k++) {
				if (can[k].y == i && can[k].x == j) {
					flg = 1;
					break;
				}
			}
			if (flg == 1) {
				printf("  *");
			}
			else if (table[i][j] == 0) {
				printf("  -");
			}
			else {
				if (table[i][j] == -1) {
					printf("  o");
				}
				else {
					printf("  x");
				}

			}

		}
		printf("\n");
	}
	return 0;
}


class montemachine{
	public:
	int flg=0;
	montemachine(int ta[WIDTH][WIDTH]);
	int table[WIDTH][WIDTH];

	int monte(int table[WIDTH][WIDTH], int playernumber, int color) {
		vector<Pos> poslist = getcanpos(table, playernumber);
		int n=poslist.size();
		if(n==0&&getcanpos(table,-playernumber).size()==0){
			return judge(table,color);
		}
		if (n == 0) {
			//pass
			return monte(table, -playernumber, color);
		}
		turn(poslist[rnd() % n], table, playernumber);
		return monte(table, -playernumber, color);
	};

	Pos runAI(int playernumber) {
		vector<Pos> posli = getcanpos(table, playernumber);
		int n = posli.size();
		int *valuelist;
		int *trycount;
		long double *ucbvalue;
		valuelist = new int[n];
		trycount = new int[n];
		ucbvalue = new long double[n];
		for (int i = 0; i < n; i++) {
			valuelist[i] = 0;
			trycount[i]=0;
			ucbvalue[i]=0;
		}


		
		for (int i = 0; i < n * 2; i++)
		{
			int ct[WIDTH][WIDTH];
			deepcopy(table, ct);
			turn(posli[i % n], ct, playernumber);
			valuelist[i % n] += monte(ct, -playernumber, playernumber);
			trycount[i % n] += 1;
		}
		int maxindex = 0;
		for (int i = n * 2; i <= tablevalue; i++) {
			maxindex = 0;
			for (int k = 0; k < n; k++) {
				ucbvalue[k] = (double)valuelist[k] / (double)trycount[k] + are * sqrt((log(i) / (double)trycount[k]));
			}
			for (int k = 1; k < n; k++) {
				if (ucbvalue[maxindex] < ucbvalue[k]) {
					maxindex = k;
				}
			}
			int ta[WIDTH][WIDTH];
			deepcopy(table, ta);
			turn(posli[maxindex], ta, playernumber);
			valuelist[maxindex] += monte(ta, -playernumber, playernumber);
			trycount[maxindex] += 1;
		}
		//探索完了

		maxindex = 0;
		for (int k = 1; k < n; k++) {
			if (ucbvalue[maxindex] < ucbvalue[k]) {
				maxindex = k;
			}
		}
		cout << "\nWinning percentage:" << 100 * (double)valuelist[maxindex] / (double)trycount[maxindex] << "%" << endl;
		delete[] valuelist;
		delete[] ucbvalue;
		delete[] trycount;
		return posli[maxindex];
	};
};
montemachine::montemachine(int ta[WIDTH][WIDTH]){
	deepcopy(ta,table);
};

int inittboard(int table[WIDTH][WIDTH]){
	int hoge[WIDTH][WIDTH] = {
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
	deepcopy(hoge,table);
	return 0;
};


string oserosocket(string senddata){
	char *sendc=new char[senddata.size()];
	for(int i=0;i<senddata.size();i++){
		sendc[i]=senddata[i];
	}
	char *recved=new char[MAX];
	string recvdata="";
	int sockfd;
	struct sockaddr_in addr;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket");
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(50000);
	addr.sin_addr.s_addr=inet_addr("127.0.1.1");
	connect(sockfd,(struct sockaddr *)&addr ,sizeof(sockaddr_in));
	send(sockfd,sendc,senddata.size(),0);
	int count=MAX;
	while(count==MAX){
		count=recv(sockfd,recved,)
	}
}


int main() {
	Pos pos;
	
	init();
	int nowplayer = player[swi % 2];
	int playerwin=0;
	int enemywin=0;
	inittboard(roottable);
	//tableの初期化
	for (;; swi++)
	{
		nowplayer = player[swi % 2];
		drawmap(roottable, nowplayer);
		if (getcanpos(roottable, 1).size() == 0 && getcanpos(roottable, -1).size() == 0) {
			break;
		}
		if (getcanpos(roottable, nowplayer).size() == 0) {
			continue;
		}
		if (nowplayer == playercolor)
		{
			cout << "input x,y:";
			while(1){
				string s;
				cin>>s;
				if(s.size()==3&&s[1]==','&&judgenum(s[0])==1&&judgenum(s[2])==1&&canput(s[0]-'0',s[2]-'0',roottable,nowplayer)==1&&canput(s[0]-'0',s[2]-'0',roottable,nowplayer)==1){
					pos.x=s[0]-'0';
					pos.y=s[2]-'0';
					turn(pos,roottable,nowplayer);
					break;
				}else{
					cout<<"plaese type it again\n";
				}
			}
		}
		else {
			montemachine ddddd(roottable);
			pos = ddddd.runAI(nowplayer);
			cout << "(x,y)=(" << pos.x << "," << pos.y << ")\n";
			turn(pos, roottable, nowplayer);
		}

	}
	
	cout << "black:" << judgest(roottable, 1) << " white:" << judgest(roottable, -1) << endl;
	return 0;
}
