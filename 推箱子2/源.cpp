

#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>

#define N 13
#define BOX 2
#define MENUNUM 4
#define MENULEN 20
#define LEVEL_NUM 5
void menu();                                  //�˵�
void run();                                   //����
void write_config();                            //���ļ�
void read_config();                             //��ȡ����
void play(int map[N][N]);
void playgame(int a[N][N]);
void draw(int a, int i, int j);
void draw_all(int map[N][N]);
bool judge(int map[N][N]);                   //�Ƿ����

int readmap(int map[N][N]);

//������ǵ�ǰ����
int current_mode = 0;
//0 ������
//1 ��Ϸ��
//2 ѡ��ؿ�
bool replay = false;

int pass_level = 0;
int direction = 0;	//���ӵ��ƶ�����
int map[N][N];
int level = 0;	//�ؿ���
int steps = 0; //����

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
		//�ļ������ڣ�����ͨ�صĹؿ��Ͳ���д��
		write_config();
	}
	else
	{
		//�ļ����ڣ���ȡÿ����С������������
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

	//�Ѳ����͹ؿ�д���ļ�config.txt
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
//���ļ��ж�ȡ��ͼ
int readmap(int map[N][N])
{
	FILE *fp = NULL;
	//��ֻ����ʽ���ļ�MAP.txt
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
		printf("���ļ�ʧ�ܣ��ļ�λ������ܷŴ���\n");
		exit(1);//��ʧ�����������
	}
	int i, j;
	char str[10];//���ڴ洢���ı��ļ���ȡ���ַ�����Ϣ
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
		{
			fscanf(fp, "%s", str); //���ļ��ж�ȡһ���ַ����������ո����
			map[i][j] = atoi(str);//���ַ���ת��Ϊ���ͣ�����ֵ��map[i][j]			
		}
	return 0;
}
//��ʼ����Ϸ
void play(int map[N][N])
{

	current_mode = 1;
	while (1)
	{
		readmap(map);
		//��������ÿ��С����
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
//����Ƿ����
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
		//С�ڵ�ǰ�ؿ�����С��������¼����
		if (steps < min_step[level])
		{
			min_step[level] = steps;
		}
		//�ؿ�����һ
		//��ղ���
		steps = 0;
		//�ؿ���һ
		level++;
		//���ؿ����ڻ��ߵ�����ͨ�صĹؿ�ʱ��������ͨ�صĹؿ�����д�롣
		if (level >= pass_level)
			pass_level = level;

		write_config();
		//����ȫ��
		draw_all(map);
		return false;
	}

	return true;
}
//�������еĸ����Լ�����ͼ
void draw_all(int map[N][N])
{
	cleardevice();
	//���Ʊ���
	putimage(0, 0, play_background_img);
	char buffer[100];
	setfont(25, 0, "��Բ");

	//���Ƽ�¼�Ͳ���
	outtextrect(50, 570, 170, 590, "z/����������");
	outtextrect(100, 120, 200, 100, "r/R����");
	sprintf(buffer, "�Ѿ��ߵĲ���:%d", steps);
	outtextrect(50, 50, 250, 30, buffer);
	sprintf(buffer, "����ʹ�õĲ�����¼:%d", min_step[level]);
	outtextrect(50, 90, 300, 30, buffer);

	//���Ƹ���
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			draw(map[i][j], i, j);
}
//����ĳ������
void draw(int a, int i, int j)
{
	switch (a)
	{
	case 0:
		putimage(i * 50, j * 50, path_img);
		break;
	case 1:
		putimage(i * 50, j * 50, wall_img);//��ii��jj�л���һ��50*50���صľ���ͼ��
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
//1.��ǽ 0.�հ� 3.�ˡ�  4.���Ӆe  5.Ŀ�ĵء�   
//8.��+Ŀ�ĵء�  9.����+Ŀ�ĵء�")
void playgame(int a[N][N])
{
	int man_i, man_j;
	//Ѱ���˵�λ��
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

		//"1.��ǽ 0.�հ� 3.�ˡ�  4.���Ӆe  5.Ŀ�ĵء�   2.�]�Ж|��
		//8.��+Ŀ�ĵء�  9.����+Ŀ�ĵء�")
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
	  "��ʼ��Ϸ"
	, "ѡ��ؿ�"
	, "�˳���Ϸ"
	};
	//ѡ�����
	//setcolor(BLACK);
	setcolor(GREEN);
	setfont(80, 0, "��Բ");
	setbkmode(TRANSPARENT);


	setfont(50, 0, "��Բ");
	int i;

	//���˵���ʾ���û�ѡ��׶�
	for (i = 0; i < MENUNUM; i++)
		outtextrect(300, 300 + i * 60, 200, 50, menuDisplay[i]);


}

//ѡ��
void choose_level()
{
	current_mode = 2;

	cleardevice();
	putimage(0, 0, menu_background_img);
	outtextrect(250, 150, 200, 50, "ѡ��ؿ�");

	char buffer[100];
	//���ݹؿ���������
	for (int i = 0; i < LEVEL_NUM; ++i)
	{
		_itoa(i + 1, buffer, 10);                 //�ؿ���������ת���ַ���
		outtextrect(250 + i * 60, 250, 50, 50, buffer);
	}

	setfont(20, 0, "��Բ");
	sprintf(buffer, "��ͨ��:%d", pass_level);
	outtextrect(300, 480, 240, 40, buffer);

}

void run()
{
	initgraph(800, 650);

	//����ͼƬ
	play_background_img = newimage();
	getimage(play_background_img, "pic/����.jpg");

	menu_background_img = newimage();
	getimage(menu_background_img, "pic/����1.jpg");

	path_img = newimage();
	getimage(path_img, "pic/·.jpg");

	human_img_1 = newimage();
	getimage(human_img_1, "pic/��1.jpg");

	human_img_2 = newimage();
	getimage(human_img_2, "pic/��2.jpg");

	human_img_3 = newimage();
	getimage(human_img_3, "pic/��3.jpg");

	human_img_4 = newimage();
	getimage(human_img_4, "pic/��4.jpg");

	wall_img = newimage();
	getimage(wall_img, "pic/ǽ.jpg");

	box_img = newimage();
	getimage(box_img, "pic/����.png");

	red_box_img = newimage();
	getimage(red_box_img, "pic/������.png");

	dest_img = newimage();
	getimage(dest_img, "pic/Ŀ�ĵ�.png");




	//����ͼƬ

	//�������˵�
	menu();

	mouse_msg m = { 0 };
	for (; is_run(); delay_fps(60))
	{
		//��ȡ�����Ϣ�����������ȴ����ȴ�������ϢΪֹ
		//���Ƶأ��к�kbhit��������ĺ���MouseHit�����ڼ����û�������Ϣ
		while (mousemsg())
		{
			m = getmouse();
		}
		if (current_mode == 0)//���˵�ģʽ
		{
			//�ж����λ�ú͵���¼�
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
		if (current_mode == 2)//ѡ��ؿ�ģʽ
		{
			for (int i = 0; i < LEVEL_NUM; i++)
			{
				//�������λ���ж�ѡ�еĹؿ�
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
								outtextrect(300, 540, 240, 40, "��δͨ�أ��޷�ѡ��");
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
	//��ʼ��ÿ�ص���С����
	for (int i = 0; i < LEVEL_NUM; ++i)
	{
		min_step[i] = 9999;
	}
	//"1.ǽ 0.· 3.�ˡ�  4.����  5.   2.�]�Ж|��
	//8.��+Ŀ�ĵء�  9.����+Ŀ�ĵء�")
	//��ȡ����
	read_config();
	run();

	write_config();

	closegraph();//�رմ���
	return 0;
}