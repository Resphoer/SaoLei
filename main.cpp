#include<iostream>
#include<conio.h>
#include<easyx.h>
#include<graphics.h>
#include<Windows.h>
#define HEIGHT 450
#define WIDTH 450
using namespace std;

int dir[8][2] = { {1,0},{0,1},{-1,0},{0,-1},{-1,1},{-1,-1},{1,1},{1,-1} };

ExMessage info;

HWND hwnd;

int flag[9][9];//判断该区是否被标记

int map[9][9];
int total = 0;

void initmap() {
	srand((unsigned)time(0));
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			map[i][j] = rand() % 2;
			if (!map[i][j]) total++;
		}
	}
}

int count(int x,int y) {
	int ans = 0;
	for (int i = 0; i < 8; i++) {
		int cur_x = x + dir[i][0], cur_y = y + dir[i][1];
		if (cur_x < 0 || cur_y < 0 || cur_x >= 9 || cur_y >= 9) continue;
		ans += map[cur_x][cur_y]==1?1:0;
	}
	return ans;
}// 统计周围雷的个数

void identify() {
	info = getmessage(EM_MOUSE);
	int x = info.x / 50, y = info.y / 50;
	if (info.lbutton&&!flag[y][x]) {
		if (map[y][x] == 1) {
			MessageBox(hwnd, "你被炸死了！", "游戏结束", MB_ICONWARNING);
			closegraph();
			exit(1);
		}
		else if(map[y][x]==0) {
			char temp[10];
			int ans = count(y, x);
			setcolor(WHITE);
			setfont(50, 50, "楷体");
			sprintf_s(temp, 10, "%d", ans);
			outtextxy(x * 50, y * 50, temp);
			map[y][x] = 2;
			total--;
			if (total == 0) {
				MessageBox(hwnd, "挑战成功！", "游戏结束", MB_OK);
				closegraph();
				exit(0);
			}
		}
	}
	else if (info.rbutton) {
		if (!flag[y][x]&&map[y][x]!=2) {
			setfillcolor(RED);
			fillcircle(x * 50 + 25, y * 50 + 25, 20);
			flag[y][x] = 1;
		}
		else if(map[y][x]!=2) {
			setfillcolor(RGB(25, 35, 60));
			fillrectangle(x * 50, y * 50, x * 50 + 50, y * 50 + 50);
			flag[y][x] = 0;
		}
	}
}//输出周围雷个数

void drawGame() {
	setbkcolor(RGB(20, 60, 5));
	cleardevice();
	setlinecolor(WHITE);
	setfillcolor(RGB(25,35,60));
	for (int i = 0; i < 9; i++) {
		int x = i * 50;
		for (int j = 0; j < 9; j++) {
			int y = j * 50;
			fillrectangle(x, y, x + 50, y + 50);
		}
	}
}//初始化雷区

int main() {
	initgraph(WIDTH,HEIGHT);
	initmap();
	drawGame();
	memset(flag, 0, sizeof(flag));
	while (1) {
		identify();
		flushmessage();
	}
	_getch();
	return 0;
}