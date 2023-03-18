

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

#define N 13
#define BOX 2
#define MENUNUM 4
#define MENULEN 20
#define LEVEL_NUM 5
void menu();                                  //菜单
void run();                                   //加载
void write_config();                            //存文件
void read_config();                             //读取数据
void play(int map[N][N]);
void playgame(int a[N][N]);
void draw(int a, int i, int j);
void draw_all(int map[N][N]);
bool judge(int map[N][N]);                   //是否过关

int readmap(int map[N][N]);

//用来标记当前界面
int current_mode = 0;
//0 主界面
//1 游戏中
//2 选择关卡
bool replay = false;

int pass_level = 0;
int direction = 0;	//箱子的移动方向
int map[N][N];
int level = 0;	//关卡数
int steps = 0; //步数

int min_step[LEVEL_NUM];

PIMAGE menu_background_img;
PIMAGE play_background_img;
PIMAGE path_img;
PIMAGE human_img_1;
PIMAGE human_img_2;
PIMAGE human_img_3;
PIMAGE human_img_4;
PIMAGE wall_img;
PIMAGE box_img;
PIMAGE red_box_img;
PIMAGE dest_img;


void read_config()
{
	FILE* fp;
	fp = fopen("config.txt", "rt+");
	char buffer[1000];
	if (fp == NULL)
	{
		//文件不存在，把以通关的关卡和步数写入
		write_config();
	}
	else
	{
		//文件存在，读取每关最小步数到变量中
		fgets(buffer, 1000, fp);
		sscanf(buffer, "%d", &pass_level);
		while (!feof(fp))
		{
			fgets(buffer, 1000, fp);
			int i;
			int s;
			sscanf(buffer, "level:%d,min_step:%d\n", &i, &s);
			min_step[i] = s;
		}

		fclose(fp);
	}

}

void write_config()
{

	//把步数和关卡写到文件config.txt
	FILE* fp;
	char buffer[1000];
	fp = fopen("config.txt", "w+");
	sprintf(buffer, "%d\n", pass_level);
	fputs(buffer, fp);
	for (int i = 0; i < LEVEL_NUM; i++)
	{
		sprintf(buffer, "level:%d,min_step:%d\n", i, min_step[i]);
		fputs(buffer, fp);
	}
	fflush(fp);
	fclose(fp);
}
//从文件中读取地图
int readmap(int map[N][N])
{
	FILE *fp = NULL;
	//以只读形式打开文件MAP.txt
	switch (level)
	{
	case 0:
		fp = fopen("map/MAP1.txt", "rb");
		break;
	case 1:
		fp = fopen("map/MAP2.txt", "rb");
		break;
	case 2:
		fp = fopen("map/MAP3.txt", "rb");
		break;
	case 3:
		fp = fopen("map/MAP4.txt", "rb");
		break;
	case 4:
		fp = fopen("map/MAP5.txt", "rb");
		break;
	}
	if (!fp) {
		printf("打开文件失败，文件位置你可能放错了\n");
		exit(1);//打开失败则结束程序
	}
	int i, j;
	char str[10];//用于存储从文本文件读取的字符串信息
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
		{
			fscanf(fp, "%s", str); //从文件中读取一个字符串，遇到空格结束
			map[i][j] = atoi(str);//将字符串转变为整型，并赋值给map[i][j]			
		}
	return 0;
}
//开始玩游戏
void play(int map[N][N])
{

	current_mode = 1;
	while (1)
	{
		readmap(map);
		//遍历绘制每个小矩形
		draw_all(map);
		while (judge(map))
		{
			playgame(map);
			if (replay)
			{
				replay = false;
				break;
			}
		}
		steps = 0;
	}

}
//检查是否过关
bool judge(int map[N][N])
{
	int judge = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (map[i][j] == 4)
				judge++;
		}
	if (judge == 0)
	{
		//小于当前关卡的最小步数，记录下来
		if (steps < min_step[level])
		{
			min_step[level] = steps;
		}
		//关卡数加一
		//清空步数
		steps = 0;
		//关卡加一
		level++;
		//当关卡大于或者等于已通关的关卡时，更新已通关的关卡，并写入。
		if (level >= pass_level)
			pass_level = level;

		write_config();
		//绘制全部
		draw_all(map);
		return false;
	}

	return true;
}
//绘制所有的格子以及背景图
void draw_all(int map[N][N])
{
	cleardevice();
	//绘制背景
	putimage(0, 0, play_background_img);
	char buffer[100];
	setfont(25, 0, "幼圆");

	//绘制记录和步数
	outtextrect(50, 570, 170, 590, "z/返回主界面");
	outtextrect(100, 120, 200, 100, "r/R重玩");
	sprintf(buffer, "已经走的步数:%d", steps);
	outtextrect(50, 50, 250, 30, buffer);
	sprintf(buffer, "最少使用的步数记录:%d", min_step[level]);
	outtextrect(50, 90, 300, 30, buffer);

	//绘制格子
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			draw(map[i][j], i, j);
}
//绘制某个格子
void draw(int a, int i, int j)
{
	switch (a)
	{
	case 0:
		putimage(i * 50, j * 50, path_img);
		break;
	case 1:
		putimage(i * 50, j * 50, wall_img);//在ii行jj列绘制一个50*50像素的矩形图像
		break;
	case 2:
		break;
	case 3:
		switch (direction)
		{
		case 0:
			putimage(i * 50, j * 50, human_img_1);
			break;
		case 1:
			putimage(i * 50, j * 50, human_img_2);
			break;
		case 3:
			putimage(i * 50, j * 50, human_img_3);
			break;
		case 4:
			putimage(i * 50, j * 50, human_img_4);
			break;
		}
		break;
	case 4:
		putimage(i * 50, j * 50, box_img);
		break;
	case 5:
		putimage(i * 50, j * 50, dest_img);
		break;
	case 8:
		putimage(i * 50, j * 50, human_img_1);
		break;
	case 9:
		putimage(i * 50, j * 50, red_box_img);
		break;
	default:
		break;
	}
}
//1.█墙 0.空白 3.人♀  4.箱子卐  5.目的地★   
//8.人+目的地♀  9.箱子+目的地⊙")
void playgame(int a[N][N])
{
	int man_i, man_j;
	//寻找人的位置
	for (man_i = 0; man_i < N; ++man_i)
	{
		for (man_j = 0; man_j < N; man_j++)
		{
			if (a[man_i][man_j] == 3 || a[man_i][man_j] == 8)
				break;
		}
		if (a[man_i][man_j] == 3 || a[man_i][man_j] == 8)
			break;
	}
	char ch = getch();
	switch (ch)
	{
	case 'A':
	case 'a':
	case 37:

		//"1.█墙 0.空白 3.人♀  4.箱子卐  5.目的地★   2.沒有東西
		//8.人+目的地♀  9.箱子+目的地⊙")
		direction = 0;
		if (a[man_i - 1][man_j] == 0 || a[man_i - 1][man_j] == 5)
		{
			a[man_i - 1][man_j] += 3;
			a[man_i][man_j] -= 3;
			steps++;
		}
		else if (a[man_i - 1][man_j] == 4 || a[man_i - 1][man_j] == 9)
		{
			if (a[man_i - 2][man_j] == 0 || a[man_i - 2][man_j] == 5)
			{
				a[man_i - 2][man_j] += 4;
				a[man_i - 1][man_j] -= 1;
				a[man_i][man_j] -= 3;
				steps++;
			}
		}
		break;
	case 'W':
	case 'w':
	case 38:
		direction = 3;
		if (a[man_i][man_j - 1] == 0 || a[man_i][man_j - 1] == 5)
		{
			a[man_i][man_j - 1] += 3;
			a[man_i][man_j] -= 3;
			steps++;
		}
		else if (a[man_i][man_j - 1] == 4 || a[man_i][man_j - 1] == 9)
		{
			if (a[man_i][man_j - 2] == 0 || a[man_i][man_j - 2] == 5)
			{
				a[man_i][man_j - 2] += 4;
				a[man_i][man_j - 1] -= 1;
				a[man_i][man_j] -= 3;
				steps++;
			}
		}
		break;
	case 'D':
	case 'd':
	case 39:
		direction = 1;
		if (a[man_i + 1][man_j] == 0 || a[man_i + 1][man_j] == 5)
		{
			a[man_i][man_j] -= 3;
			a[man_i + 1][man_j] += 3;
			steps++;
		}
		else if (a[man_i + 1][man_j] == 4 || a[man_i + 1][man_j] == 9)
		{
			if (a[man_i + 2][man_j] == 0 || a[man_i + 2][man_j] == 5)
			{
				a[man_i + 2][man_j] += 4;
				a[man_i + 1][man_j] -= 1;
				a[man_i][man_j] -= 3;
				steps++;
			}
		}
		break;
	case 'S':
	case 's':
	case 40:
		direction = 4;
		if (a[man_i][man_j + 1] == 0 || a[man_i][man_j + 1] == 5)
		{
			a[man_i][man_j + 1] += 3;
			a[man_i][man_j] -= 3;
			steps++;
		}
		else if (a[man_i][man_j + 1] == 4 || a[man_i][man_j + 1] == 9)
		{
			if (a[man_i][man_j + 2] == 0 || a[man_i][man_j + 2] == 5)
			{
				a[man_i][man_j + 2] += 4;
				a[man_i][man_j + 1] -= 1;
				a[man_i][man_j] -= 3;
				steps++;
			}
		}
		break;
	case 'R':
	case 'r':
		replay = true;
		break;

	case 'Z':
	case 'z':
		run();

		break;


	default:
		break;
	}
	draw_all(a);
}

void menu()
{
	current_mode = 0;
	putimage(0, 0, menu_background_img);

	char menuDisplay[MENUNUM][MENULEN] = {
	  "开始游戏"
	, "选择关卡"
	, "退出游戏"
	};
	//选项输出
	//setcolor(BLACK);
	setcolor(GREEN);
	setfont(80, 0, "幼圆");
	setbkmode(TRANSPARENT);


	setfont(50, 0, "幼圆");
	int i;

	//主菜单显示及用户选择阶段
	for (i = 0; i < MENUNUM; i++)
		outtextrect(300, 300 + i * 60, 200, 50, menuDisplay[i]);


}

//选关
void choose_level()
{
	current_mode = 2;

	cleardevice();
	putimage(0, 0, menu_background_img);
	outtextrect(250, 150, 200, 50, "选择关卡");

	char buffer[100];
	//根据关卡数量绘制
	for (int i = 0; i < LEVEL_NUM; ++i)
	{
		_itoa(i + 1, buffer, 10);                 //关卡数，数字转成字符串
		outtextrect(250 + i * 60, 250, 50, 50, buffer);
	}

	setfont(20, 0, "幼圆");
	sprintf(buffer, "已通关:%d", pass_level);
	outtextrect(300, 480, 240, 40, buffer);

}

void run()
{
	initgraph(800, 650);

	//加载图片
	play_background_img = newimage();
	getimage(play_background_img, "pic/背景.jpg");

	menu_background_img = newimage();
	getimage(menu_background_img, "pic/背景1.jpg");

	path_img = newimage();
	getimage(path_img, "pic/路.jpg");

	human_img_1 = newimage();
	getimage(human_img_1, "pic/人1.jpg");

	human_img_2 = newimage();
	getimage(human_img_2, "pic/人2.jpg");

	human_img_3 = newimage();
	getimage(human_img_3, "pic/人3.jpg");

	human_img_4 = newimage();
	getimage(human_img_4, "pic/人4.jpg");

	wall_img = newimage();
	getimage(wall_img, "pic/墙.jpg");

	box_img = newimage();
	getimage(box_img, "pic/箱子.png");

	red_box_img = newimage();
	getimage(red_box_img, "pic/红箱子.png");

	dest_img = newimage();
	getimage(dest_img, "pic/目的地.png");




	//加载图片

	//进入主菜单
	menu();

	mouse_msg m = { 0 };
	for (; is_run(); delay_fps(60))
	{
		//获取鼠标消息，这个函数会等待，等待到有消息为止
		//类似地，有和kbhit功能相近的函数MouseHit，用于检测有没有鼠标消息
		while (mousemsg())
		{
			m = getmouse();
		}
		if (current_mode == 0)//主菜单模式
		{
			//判断鼠标位置和点击事件
			if (m.msg == mouse_msg_down)
			{
				if (m.x > 300 && m.x < 500)
				{
					if (m.y > 300 && m.y < 350)
					{
						play(map);
					}
					if (m.y > 360 && m.y < 410)
					{
						choose_level();
					}
					if (m.y > 420 && m.y < 470)
					{
						break;
					}
				}
			}
		}
		if (current_mode == 2)//选择关卡模式
		{
			for (int i = 0; i < LEVEL_NUM; i++)
			{
				//根据鼠标位置判断选中的关卡
				if (m.msg == mouse_msg_down)
				{
					if (m.y > 250 && m.y < 300)
					{
						if (m.x > 250 + 60 * i && m.x < 300 + 60 * i)
						{
							if (i <= pass_level)
							{
								level = i;
								play(map);
							}
							else
							{
								outtextrect(300, 540, 240, 40, "尚未通关，无法选择");
							}
						}
					}
				}
			}
		}

	}
}

int main()
{
	//初始化每关的最小步数
	for (int i = 0; i < LEVEL_NUM; ++i)
	{
		min_step[i] = 9999;
	}
	//"1.墙 0.路 3.人♀  4.箱子  5.   2.沒有東西
	//8.人+目的地♀  9.箱子+目的地⊙")
	//读取配置
	read_config();
	run();

	write_config();

	closegraph();//关闭窗口
	return 0;
}