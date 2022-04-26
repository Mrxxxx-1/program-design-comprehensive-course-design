#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include"md5.h"

#define VERTEXNUM 266
#define HEADER1 "---------------武汉地铁线路----------------\n"
#define HEADER2 "| 线路名称 | 站点数 | 满员人数 | 线路长度 |\n"
#define HEADER3 "|----------|--------|----------|----------|\n"
#define HEADER4 "-------------------------------------------\n"
#define HEADER5 "--------------------------------------------\n"
#define HEADER6 "|  站点名称  | 是否可换乘 |  所属线路编号  |\n"

#define FORMAT "|%-10s|%-8d|%-10d|%-10g|\n"
using namespace  std;

namespace Encryption
{
	string md5(string strPlain)
	{
		MD5_CTX mdContext;


		MD5Init(&mdContext);
		MD5Update(&mdContext, (unsigned char*)const_cast<char*>(strPlain.c_str()), strPlain.size());
		MD5Final(&mdContext);

		string md5;
		char buf[3];
		for (int i = 0; i < 16; i++)
		{
			sprintf(buf, "%02x", mdContext.digest[i]);
			md5.append(buf);
		}
		return md5;
	}
}
int LINENUM = 8;               //地铁线总数//
int istrans[VERTEXNUM] = { 0 };//换乘点信息存储数组//
double a[3] = { 0 };                ////
int no = 0;                         ////  
double cfactor[8] = { 0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6 };//更改拥挤度信息函数//

typedef struct MetroLine
{
	int NO;							//线路编号//
	char name[15];					//线路名称//
	int number;						//站点数//
    int fullnumber;                 //满员人数//
	double linelength;              //线路长度//
}LINE;

LINE Metro[8];

typedef struct transstation
{
	char name[15];                  //换乘站点名称//
	int line[3];                    //换乘站点线路信息保存//
	int triple;                     //判断是否为三换乘站点//
}transstation;

transstation trans[27];

typedef struct enode
{
	int avex;                       //邻接点的位置//
	int line;                       //线路编号//
	struct enode* next;             //指向下一个邻接点//
}enode;

typedef struct vnode
{
	int id;                         //顶点的编号//
	char name[20];                  //站的名字//
	enode* fadj;					//指向第一个点//
}vnode;

typedef struct Graph
{
	vnode ver[VERTEXNUM];           //顶点集合(vertex)//
	int vexnum;                     //图的顶点数目//
	int arcnum;                     //图的边数目//

}Graph;

typedef struct path
{
	int pnum;                       //路径的数目//
	int path[VERTEXNUM];            //路径信息//
	int trans[VERTEXNUM];		    //换乘点信息,0为非换乘点，1为换乘点//
	int counter;                    //换乘的次数//
	double distance;                //路径距离//
	double time;                    //路径花费时间//
	double factor;                  //路径平均拥挤度因子//
	struct path* next;              //指向下一个路径信息节点//
}pathnode;


typedef struct LINK
{
	pathnode* head;                 //路径接口//
}LINK;


//菜单
void menu();

//线路读取
void readline(LINE a[]);

//换乘站点读取
void readtrans(transstation a[]);

//换乘站点判断
int judgetrans(char a[]);

//线路显示
int showline(LINE a[], int i);

//无向图的建立
void create_graph(Graph* G);

//功能一显示线路站点
void funcone();

//获取站点编号
int locatevex(Graph* G, char a[],int n);

//站点路径显示
int showstation(Graph* G, int num);

//功能二线路查询
void functwo();

//计算车辆到站离站时间
void arrivaldeparture(int line, int num, double *time0, double *time1);

//时间转换函数
double timechange(int a, int b);

//功能三车辆运行时刻表
void functhree();

//DFS深度优先搜索出所有可能的路线走法//
void DFS(Graph* G, int stack[], int visited[], int v, int destination, int pnum, LINK* L, int no);

//输出最少换乘路径信息//
void print_lesstransfer(Graph* G, LINK* L);

//获取换乘点//
void gettrans(Graph* G, pathnode* all);

//最少换乘的函数接口，参数1：起始站，参数2：终点站//
void lesstransfer(char start[], char destination[],double time);

//创建一个路径链表//
void create_path(LINK* L);

//插入一个路径信息//
void addpath(LINK* L, pathnode* p);

//计算路径距离时间//
void distanceandtime(Graph* G, LINK* L);

//计算路径距离,加权时间//
void distanceandtime1(Graph* G, LINK* L, double time);

//排序找出两条推荐路径(距离)//
void sortoutpath(LINK* L, double a[]);

//排序找出两条推荐路径(时间)//
void sortoutpath1(LINK* L, double a[]);

//功能四乘车路线推荐//
void funcfour();

//计算乘车距离//
double caculate_distance(int a[]);

//计算乘车时间//
double caculate_time(int sitenumber, int transfernumber, double waitingtime, double factor);

//计算票价//
double caculate_price(double distance);

//功能五拥挤度设置//
void funcfive();

//获得拥挤程度系数//
double getfactor(int line, double time);

//功能六综合拥挤度线路推荐
void funcsix();

//功能七更改拥挤度后线路推荐及避开拥挤
void funcseven();

int main()
{
	string passwd;
	char c;
	ifstream in("passwd.txt");
	in >> passwd;
	while (1)
	{
		cout << "请输入登录密码:\n";
		string x;
		while (1)
		{
			c = getch();
			if (c == '\r') {
				cout << endl;
				break;
			}
			cout << "*";
			x.push_back(c);
		}
		if (Encryption::md5(x) == passwd)
		{
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              进入系统中.";
			Sleep(500);
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              进入系统中..";
			Sleep(500);
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              进入系统中...";
			Sleep(500);
			system("cls");
			break;
		}
		else cout << "密码错误！" << endl;
	}

	int label;
	int i=1;
	char ch;
	char start[20], destination[20];
	int road[3] = {-1};
	readline(Metro);
	while (1)
	{
		menu();
		printf("请输入<0-7>:");
		scanf("%d", &label);
		if (label == 0) { printf("感谢您的使用！\n"); Sleep(1500);exit(0); }
		switch (label)
		{
		case 1:
			funcone();
			break;
		case 2:
			functwo();
			break;
		case 3:
			functhree();
			break;
		case 4:
			funcfour();
			break;
		case 5:
			funcfive();
			break;
		case 6:
			funcsix();
			break;
		case 7:
			funcseven();
			break;
		default:
			fflush(stdin);
			printf("您输入的指令有误！\n");
		}
		printf("\n按任意键继续...\n");
		_getch();
	}
	return 0;
}

//菜单
void menu()
{

	printf("\t|-----------------------------------------------|\n");
	printf("\t|            武汉地铁线路出行推荐               |\n");
	printf("\t|-----------------------------------------------|\n");
	printf("\t|       0:退出系统                              |\n");
	printf("\t|       1:显示线路站点                          |\n");
	printf("\t|       2:线路查询                              |\n");
	printf("\t|       3:车辆运行时刻表                        |\n");
	printf("\t|       4:乘车线路推荐                          |\n");
	printf("\t|       5:拥挤度设置                            |\n");
	printf("\t|       6:综合拥挤度线路推荐                    |\n");
	printf("\t|       7:更改拥挤度后线路推荐及避开拥挤        |\n");
	printf("\t|-----------------------------------------------|\n");

}

//线路读取
void readline(LINE a[])
{
	int NO;							//线路编号//
	char name[20];					//线路名称//
	int number;						//站点数//
	int fullnumber;                 //满员人数//
	double linelength;              //线路长度//
	int counter=8;                  //计数器//
	int i;
	FILE* fp;
	fp = fopen("WHline.txt", "r");
	for (i = 0;i < counter;i++)
	{
		fscanf(fp, "%d\n", &NO);
		fscanf(fp, "%s\n", name);
		fscanf(fp, "%d\n", &fullnumber);
		fscanf(fp, "%lf\n", &linelength);
		fscanf(fp, "%d\n", &number);

		a[i].NO = NO;
		strcpy(a[i].name, name);
		a[i].number = number;
		a[i].fullnumber = fullnumber;
		a[i].linelength = linelength;
	}
	fclose(fp);
}

//换乘站点读取
void readtrans(transstation a[])
{
	char name[15];                  //换乘站点名称//
	int line1, line2, line3;        //换乘站点线路信息保存//
	int triple;                     //判断是否为三换乘站点//
	int counter = 27;
	int i;
	FILE* fp;
	fp = fopen("transinfo.txt", "r");
	for (i = 0;i < counter;i++)
	{
		fscanf(fp, "%s\n", name);
		fscanf(fp, "%d\n", &line1);
		fscanf(fp, "%d\n", &line2);
		fscanf(fp, "%d\n", &line3);
		fscanf(fp, "%d\n", &triple);

		strcpy(a[i].name, name);
		a[i].line[0] = line1;
		a[i].line[1] = line2;
		a[i].line[2] = line3;
		a[i].triple = triple;
	}
	fclose(fp);

}

//换乘站点判断
int judgetrans(char a[])
{
	int i=0;
	int j;
	while (i<27)
	{
		if (strcmp(a, trans[i].name) == 0)
			break;
		i++;
	}
	if (i < 27)
		return i;
	else
		return -1;
}
//无向图的建立
void create_graph(Graph* G)
{
	enode* p;
	int i, j, m, n, line;
	G->vexnum = 227;
	G->arcnum = 220;
	FILE* fp;
	fp = fopen("station.txt", "r");
	for (n = 1;n <= G->vexnum;n++)
	{
		fscanf(fp, "%s", G->ver[n].name);
		G->ver[n].id = n;
		G->ver[n].fadj = NULL;
	}
	fclose(fp);
	fp = fopen("stationinfo.txt", "r");
	for (m = 1;m <= G->arcnum;m++)
	{
		fscanf(fp, "%d %d %d", &i, &j, &line);
		p = (enode*)malloc(sizeof(enode));
		p->avex = j;
		p->line = line;
		p->next = G->ver[i].fadj;
		G->ver[i].fadj = p;
		//双向连接
		p = (enode*)malloc(sizeof(enode));
		p->avex = i;
		p->line = line;
		p->next = G->ver[j].fadj;
		G->ver[j].fadj = p;
	}
	fclose(fp);
}

//线路显示
int showline(LINE a[], int i)
{
	int temp, num=0;
	if (i == 5)
		return 0;
	if (i > 0 && i < 9)
	{
		temp=i;
		temp--;
		printf(HEADER1);
		printf(HEADER2);
		printf(HEADER3);
		printf(FORMAT, a[temp].name,a[temp].number,a[temp].fullnumber,a[temp].linelength);
		printf(HEADER4);
		return 1;
	}
	else 
		return 0;
}

//功能一显示线路站点
void funcone()
{


	int i, j, n, temp, k;
	int a[VERTEXNUM] = { 0 };
	int b[VERTEXNUM] = { 0 };
	int c[VERTEXNUM] = { 0 };
	char ch;
	LINE p;
	Graph G;
	FILE* fp;
	readline(Metro);
	do {
		printf("请输入要查询的地铁线路");
		scanf("%d", &i);
		if (showline(Metro, i) == 0)
			printf("输入线路错误\n");
		else 
		{
			create_graph(&G);

			fp = fopen("stationinfo.txt", "r");
			for (k = 1;k <= VERTEXNUM;k++)
			{
				fscanf(fp, "%d %d %d", &a[k], &b[k], &c[k]);
			}
			temp = 1;
			i--;
			for (int n = i;n > 0;n--)
			{
				temp = temp + Metro[n-1].number;
			}
			temp = temp - i;
				printf("-----------------------------------地铁%d号线-----------------------------------\n",Metro[i].NO);
				i++;
				for (j = temp;c[j] == i;j++)
				{
					printf("%s->", G.ver[a[j]].name);
				}
				printf("%s\n", G.ver[b[--j]].name);
				temp = ++j;
				fclose(fp);
			}
			
		printf("是否继续查询线路y/n");
		getchar();
		ch = getchar();
	} while (ch == 'y' || ch == 'Y');
	
}

//获取站点编号
int locatevex(Graph* G, char a[],int n)
{
	int i = 1;
	int j = 0;
	int m = 0;
	enode* p = NULL;
	while ( i <= G->vexnum)
	{
		j = strcmp(G->ver[i].name, a);

		if (j == 0 )
		{
			p = G->ver[i].fadj;
			m = p->line;
			if(m==n)
			break;
		}
			
		i++;
	}
	if (i <= G->vexnum)
		return i;
	else
		return -1;
}

//站点路径显示
int showstation(Graph* G, int num)
{

	int a[VERTEXNUM] = { 0 };
	int b[VERTEXNUM] = { 0 };
	int c[VERTEXNUM] = { 0 };
	int j, t;
	int line;
	char ch;
	int counter = 0;
	FILE* fp;
	int direction;
	printf("查询该站点前/后站点（请输入0/1）\n");
	scanf("%d", &direction);
	if (direction > 1 || direction < 0)
	{
		printf("输入错误\n");
		return 0;
	}
	fp = fopen("stationinfo.txt", "r");
	for (int k = 1;k <= VERTEXNUM;k++)
	{
		fscanf(fp, "%d %d %d", &a[k], &b[k], &c[k]);
	}
	fclose(fp);
	line = c[num];
	if (direction == 0)
	{
		for (j = num-line+1;c[j-1] == line;j--)
		{
			printf("%s->", (*G).ver[a[j]].name);
			t = judgetrans((*G).ver[a[j]].name);
			counter++;
			if (t != -1)
			{
				if (trans[t].triple == 1)
				{
					if (trans[t].line[2] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[0], trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[0], trans[t].line[2]);
					if (trans[t].line[0] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[1], trans[t].line[2]);
				}
				else
				{
					if (trans[t].line[0] == line)
						printf("该站点为换乘站，可换乘线路为%d\n", trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("该站点为换乘站，可换乘线路为%d\n", trans[t].line[0]);
				}

				printf("是否换乘y/n\n");
				getchar();
				ch = getchar();
				if (ch == 'y' || ch == 'Y')
					return a[j];

			}
		}
		if (counter != 0)
			printf("%s\n", (*G).ver[a[j]].name);
	}
	if (direction == 1)
	{
		for (j = num-line+1;c[j] == line;j++)
		{
			printf("%s->", (*G).ver[a[j]].name);
			t = judgetrans((*G).ver[a[j]].name);
			counter++;
			if (t != -1)
			{
				if (trans[t].triple == 1)
				{
					if (trans[t].line[2] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[0], trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[0], trans[t].line[2]);
					if (trans[t].line[0] == line)
						printf("该站点为换乘站，可换乘线路为%d,%d\n", trans[t].line[1], trans[t].line[2]);
				}
				else
				{
					if (trans[t].line[0] == line)
						printf("该站点为换乘站，可换乘线路为%d\n", trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("该站点为换乘站，可换乘线路为%d\n", trans[t].line[0]);
				}

				printf("是否换乘y/n\n");
				getchar();
				ch = getchar();
				if (ch == 'y' || ch == 'Y')
					return a[j];

			}
		}
		if(counter!=0)
		printf("%s\n", (*G).ver[b[--j]].name);
	}
	return 0;
}
//功能二线路查询
void functwo()
{
	int i=1, num;
	int flag = 0;
	char name[20];
	Graph G;
	enode* p;
	create_graph(&G);
	readtrans(trans);
	printf("请输入要查询的线路\n");
	scanf("%d", &i);
	if (i < 1 || i>8 || i == 5)
	{
		printf("输入线路不存在\n");
		return;
	}
	else
	{
		printf("请输入要查询的站点\n");
		getchar();	
		gets_s(name);
		num = locatevex(&G, name, i);
		if (num == -1)
		{
			printf("输入站点不存在\n");
			return;
		}
		flag = showstation(&G,num);
		while (flag)
		{
			printf("请输入要换乘的线路\n");
			scanf("%d", &i);
			num=locatevex(&G, G.ver[flag].name, i);
			if (num == -1)
			{
				printf("输入线路中站点不存在\n");
				break;
			}
			flag=showstation(&G, num);
		}
			
	}
	
}

//计算车辆到站离站时间
void arrivaldeparture(int line,int num, double* time0, double* time1)
{
	int ar0,ar1;
	int sum = 0;
	if (line > 5)
		sum--;
	int i=line-1;
	for (;i > 0;--i)
	{
		sum = sum + Metro[i-1].number;
	}
	ar0 = num - sum-1;
	ar1 = Metro[line - 1].number - ar0-1;
	*time0 = (ar0 - 1) * 3 + 2;
	*time1 = (ar1 - 1) * 3 + 2;
}

//功能三车辆运行时刻表
void functhree()
{
	Graph G;
	int hour, min, hour0, min0, hour1, min1;
	double t1, t2;
	char op;
	int line, num;
	char name[20];
	double time0, time1;
	double ar, de;
	create_graph(&G);
	printf("请输入当前时刻\n");
	scanf("%d%c%d", &hour, &op, &min);
	printf("请输入要查询的线路\n");
	scanf("%d", &line);
	if (line < 1 || line > 8 || line == 5)
	{
		printf("输入线路不存在\n");
		return;
	}
	else
	{
		printf("请输入要查询的站点\n");
		getchar();
		gets_s(name);
		num = locatevex(&G, name, line);
		if (num == -1)
		{
			printf("输入站点不存在\n");
			return;
		}
		arrivaldeparture(line, num, &time0, &time1);
		t1 = (hour - 6) * 60 + min;
		t2 = (hour - 23) * 60 + min;
		if (t1 < time0||t2 > time0)
			printf("向终点站方向此时无车辆\n");
		else
		{
			min0 = t1 - time0;
			min0 = min0 % 5;
			min0 = min0 + min;
			if (min0 >= 59)
			{
				min0 = min0 - 60;
				hour0 = hour + 1;
			}
			else
				hour0 = hour;
			printf("向终点站方向车辆到站时间%02d:%02d\n", hour0, min0);
			printf("向终点站方向车辆离开时间%02d:%02d\n", hour0, min0 + 1);
		}
		
		if (t1 < time1 || t2 > time1)
			printf("向起始站方向此时无车辆\n");
		else
		{
			min1 = t1 - time1;
			min1 = min1 % 5;
			min1 = min1 + min;
			if (min1 >= 59)
			{
				min1 = min1 - 60;
				hour1 = hour + 1;
			}
			else
				hour1 = hour;
			printf("向起始站方向车辆到站时间%02d:%02d\n", hour1, min1);
			printf("向起始站方向车辆离开时间%02d:%02d\n", hour1, min1 + 1);
		}
		
	}
}

//时间转换函数
double timechange(int a, int b)
{
	double time;
	time = (a-6) * 60 + b;
	return time;
}
//1
int locatevex1(Graph* G, char a[])
{
	int i = 1;
	while (strcmp(G->ver[i].name, a) && i <= G->vexnum)
		i++;
	if (i <= G->vexnum)
		return i;
	else
		return -1;
}

 //2无向图的建立
void create_graph1(Graph* G)
{
	enode* p;
	int i, j, m, n, line;
	G->vexnum = 198;
	G->arcnum = 220;
	FILE* fp;
	fp = fopen("station1.txt", "r");
	for (n = 1;n <= G->vexnum;n++)
	{
		fscanf(fp, "%s", G->ver[n].name);
		G->ver[n].id = n;
		G->ver[n].fadj = NULL;
	}
	fclose(fp);
	fp = fopen("stationinfo1.txt", "r");
	for (m = 1;m <= G->arcnum;m++)
	{
		fscanf(fp, "%d %d %d", &i, &j, &line);
		p = (enode*)malloc(sizeof(enode));
		p->avex = j;
		p->line = line;
		p->next = G->ver[i].fadj;
		G->ver[i].fadj = p;
		//双向连接
		p = (enode*)malloc(sizeof(enode));
		p->avex = i;
		p->line = line;
		p->next = G->ver[j].fadj;
		G->ver[j].fadj = p;
	}
	fclose(fp);
}


//最少换乘的函数接口，参数1：起始站，参数2：终点站//
void lesstransfer(char start[], char destination[],double time)
{
	Graph G;
	LINK L;
	int v1, v2;
	int no=0;
	int stack[VERTEXNUM] = { 0 };     //初始化
	int  visited[VERTEXNUM] = { 0 };
	create_graph1(&G);	 //创建一个无向网络//
	create_path(&L);	//创建保存路径的链表//
	v1 = locatevex1(&G, start);
	v2 = locatevex1(&G, destination);

	if (v1 == -1 || v2 == -1)
	{
		printf("您输入的站名有误！\n");
		return;
	}

	DFS(&G, stack, visited, v1, v2, 0, &L,no);
	distanceandtime(&G, &L);
	sortoutpath(&L, a);
	print_lesstransfer(&G, &L);

}

/*利用深度搜索的递归找出所有的路径
参数一：图  参数2：临时存放路径顺序数组  参数3：是否访问  参数4：当前位置  参数5：到达的位置  参数6：栈顶  参数7：所有路径的链表*/
void DFS(Graph* G, int stack[], int visited[], int v, int destination, int pnum, LINK* L,int no)
{
	int j, i;
	enode* p;
	pathnode* all;
	stack[pnum] = v;
	if (v == destination)
	{
		all = (pathnode*)malloc(sizeof(pathnode));
		all->counter = 0;
		all->pnum = pnum;
		for (i = 0;i <= pnum;i++)
		{
			all->trans[i] = 0;
			all->path[i] = stack[i];
		}
		gettrans(G, all);
		addpath(L, all);
		return;//递归边界条件
	}
	else
		visited[v] = 1;
	p = G->ver[v].fadj;
	while (p)
	{
		if (!visited[p->avex])
			DFS(G, stack, visited, p->avex, destination, pnum + 1, L, ++no);
		p = p->next;
	}
	stack[pnum] = 0;
	visited[v] = 0;
}

//路径距离，时间计算//
void distanceandtime (Graph* G, LINK* L)
{
	int line;
	int k;
	double factor = 0.5;
	double waittime = 0;
	pathnode * r;
	r = L->head;
	while (r->next != NULL)
	{
		int stationnum[8] = { 0 };
		r = r->next;
		int i = 0;
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					i++;
					i--;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					i++;
					stationnum[line - 1]++;
				}
			}
			r->distance = caculate_distance(stationnum);
			r->time = caculate_time(i, r->counter, waittime, factor);
	}
}

//路径距离，加权时间计算//
void distanceandtime1(Graph* G, LINK* L, double time)
{
	int line;
	int k;
	double factor = 0.5;
	double waittime = 0;
	pathnode* r;
	r = L->head;
	while (r->next != NULL)
	{
		double sum = 0;
		int stationnum[8] = { 0 };
		r = r->next;
		int i = 0;
		for (k = 0;k < (r->pnum);k++)
		{
			if (r->trans[k])
			{
				time = time + 3.5;
			}
			else
			{
				line = G->ver[r->path[k]].fadj->line;
				sum = sum + getfactor(line, time);
				i++;
				stationnum[line - 1]++;
			}
		}
		sum = sum / i;
		factor = sum;
		r->factor = factor;
		r->distance = caculate_distance(stationnum);
		r->time = caculate_time(i, r->counter, waittime, factor);
	}
}

//2
void distanceandtime2(Graph* G, LINK* L)
{
	int line;
	int k;
	double factor = 0.5;
	double waittime = 0;
	pathnode* r;
	r = L->head;
	while (r->next != NULL)
	{
		double sum = 0;
		int stationnum[8] = { 0 };
		r = r->next;
		int i = 0;
		for (k = 0;k < (r->pnum);k++)
		{
			if (r->trans[k])
			{
				i++;
				i--;
			}
			else
			{
				line = G->ver[r->path[k]].fadj->line;
				sum = sum + cfactor[line - 1];
				i++;
				stationnum[line - 1]++;
			}
		}
		sum = sum / i;
		factor = sum;
		r->factor = factor;
		r->distance = caculate_distance(stationnum);
		r->time = caculate_time(i, r->counter, waittime, factor);
	}
}

//3
void distanceandtime3(Graph* G, LINK* L,double vfactor)
{
	int line;
	int flag;
	int k;
	double factor = 0.5;
	double waittime = 0;
	pathnode* r;
	r = L->head;
	while (r->next != NULL)
	{
		flag = 0;
		double sum = 0;
		int stationnum[8] = { 0 };
		r = r->next;
		int i = 0;
		for (k = 0;k < (r->pnum);k++)
		{
			if (r->trans[k])
			{
				i++;
				i--;
			}
			else
			{
				line = G->ver[r->path[k]].fadj->line;
				if (judgetrans(G->ver[r->path[k]].name)!= -1)
				{
					sum = sum + cfactor[line - 1];
					i++;
					stationnum[line - 1]++;
				}
				else if (cfactor[line - 1] > vfactor)
				{
					flag=1;
					break;
				}
				else
				{
					sum = sum + cfactor[line - 1];
					i++;
					stationnum[line - 1]++;
				}
			}
		}
		if (flag == 1)
		{
			r->time = 999;
				sum = sum / i;
			factor = sum;
			r->factor = factor;
			r->distance = caculate_distance(stationnum);
		}
		else
		{
			sum = sum / i;
			factor = sum;
			r->factor = factor;
			r->distance = caculate_distance(stationnum);
			r->time = caculate_time(i, r->counter, waittime, factor);
		}
	}
}

//输出路径//
void print_lesstransfer(Graph* G, LINK* L)
{
	int maximum = 999;
	int line, k;
	int last;
	int count = 1;
	int i = 0, j;
	int flag = 0;
	pathnode* p, * r;
	p = L->head;
	r = L->head;
	while (r->next != NULL)
	{
		r = r->next;
		last = 1;
		if (r->distance == a[0])
		{
			i = 0;
			printf("------------------------->-------------------------->------------------------->\n");
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("不需要换乘\n");
			else
			printf("经过站点:%d个\n", i);
			printf("乘车距离为:%g千米\n", r->distance);
			printf("乘车时间为:%g分钟\n", r->time);
			printf("票价为:%g元\n", caculate_price(r->distance));
			break;//为了避免重复，直接跳出
		}
	}
		if (a[1] == 0)
			return;
		else
		{
			last = 1;
			r = L->head;
			while (r->next != NULL)
			{
				r = r->next;
				if (r->distance == a[1])
				{
					i = 0;
					printf("------------------------->-------------------------->------------------------->\n");
					for (k = 0;k < (r->pnum);k++)
					{
						if (r->trans[k])
						{
							printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
							i++;
						}
						else
						{
							line = G->ver[r->path[k]].fadj->line;
							last = line;
							printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
							i++;
						}
					}
					printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
					printf("-------------------------------------------------------------------------------\n");
					if (r->counter == 0)
						printf("不需要换乘\n");
					else
					printf("经过站点:%d个\n", i);
					printf("乘车距离为:%g千米\n", r->distance);
					printf("乘车时间为:%g分钟\n", r->time);
					printf("票价为:%g元\n", caculate_price(r->distance));
					break;//为了避免重复，直接跳出
				}
			}
		}
		while (p->next != NULL)
		{
			last = 1;
			p = p->next;
			if (p->counter <= maximum)
				maximum = p->counter;
		}
		r = L->head;
		while (r->next != NULL)
		{
			r = r->next;
			if (r->counter == maximum)
			{
				i = 0;
				printf("最少换乘路线:\n");
				printf("------------------------->-------------------------->------------------------->\n");
				for (k = 0;k < (r->pnum);k++)
				{
					if (r->trans[k])
					{
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
				printf("-------------------------------------------------------------------------------\n");
				if (r->counter == 0)
					printf("不需要换乘\n");
				else
				printf("经过站点:%d个\n", i);
				printf("乘车距离为:%g千米\n", r->distance);
				printf("乘车时间为:%g分钟\n", r->time);
				printf("票价为:%g元\n", caculate_price(r->distance));
				break;//为了避免重复，直接跳出
			}
		}
	}

//1输出路径//
void print_lesstransfer1(Graph* G, LINK* L)
{
	int maximum = 999;
	int line, k;
	int last;
	int count = 1;
	int i = 0, j;
	int flag = 0;
	pathnode* p, * r;
	p = L->head;
	r = L->head;
	while (r->next != NULL)
	{
		last = 1;
		r = r->next;
		if (r->time == a[0])
		{
			i = 0;
			printf("------------------------->-------------------------->------------------------->\n");
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("不需要换乘\n");
			else
			printf("经过站点:%d个\n", i);
			printf("乘车距离为:%g千米\n", r->distance);
			printf("乘车加权时间为:%g分钟\n", r->time);
			printf("乘车平均拥挤程度为:%g\n", r->factor);
			printf("票价为:%g元\n", caculate_price(r->distance));
			break;//为了避免重复，直接跳出
		}
	}
	if (a[1] == 0)
		return;
	else
	{
		r = L->head;
		while (r->next != NULL)
		{
			last = 1;
			r = r->next;
			if (r->time == a[1])
			{
				i = 0;
				printf("------------------------->-------------------------->------------------------->\n");
				for (k = 0;k < (r->pnum);k++)
				{
					if (r->trans[k])
					{
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
				printf("-------------------------------------------------------------------------------\n");
				if (r->counter == 0)
					printf("不需要换乘\n");
				else
				printf("经过站点:%d个\n", i);
				printf("乘车距离为:%g千米\n", r->distance);
				printf("乘车加权时间为:%g分钟\n", r->time);
				printf("乘车平均拥挤程度为:%g\n", r->factor);
				printf("票价为:%g元\n", caculate_price(r->distance));
				break;//为了避免重复，直接跳出
			}
		}
	}
	while (p->next != NULL)
{
	p = p->next;
	if (p->counter <= maximum)
		maximum = p->counter;
}
	r = L->head;
	while (r->next != NULL)
	{
		last = 1;
		r = r->next;
		if (r->counter == maximum)
		{
			i = 0;
			printf("最少换乘路线:\n");
			printf("------------------------->-------------------------->------------------------->\n");
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("不需要换乘\n");
			else
			printf("经过站点:%d个\n", i);
			printf("乘车距离为:%g千米\n", r->distance);
			printf("乘车加权时间为:%g分钟\n", r->time);
			printf("乘车平均拥挤程度为:%g\n", r->factor);
			printf("票价为:%g元\n", caculate_price(r->distance));
			break;//为了避免重复，直接跳出
		}
	}
}

//2输出路径//
void print_lesstransfer2(Graph* G, LINK* L)
{
	int maximum = 999;
	int line, k;
	int last;
	int count = 1;
	int i = 0, j;
	int flag = 0;
	pathnode* p, * r;
	p = L->head;
	r = L->head;
	while (r->next != NULL)
	{
		last = 1;
		r = r->next;
		if (r->time == a[0])
		{
			i = 0;
			printf("------------------------->-------------------------->------------------------->\n");
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			break;//为了避免重复，直接跳出
		}
	}
	if (a[1] == 0)
		return;
	else
	{
		r = L->head;
		while (r->next != NULL)
		{
			last = 1;
			r = r->next;
			if (r->time == a[1])
			{
				i = 0;
				printf("------------------------->-------------------------->------------------------->\n");
				for (k = 0;k < (r->pnum);k++)
				{
					if (r->trans[k])
					{
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d号线)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(终点)\n", G->ver[r->path[(r->pnum)]].name);
			    printf("-------------------------------------------------------------------------------\n");
				break;
			}
		}
	}
}
void create_path(LINK* L)
{
	//新建链表操作//
	L->head = (pathnode*)malloc(sizeof(pathnode));
	L->head->next = NULL;
}

void addpath(LINK* L, pathnode* p)
{
	//链表的插入操作//
	p->next = L->head->next;
	L->head->next = p;
}

//求出同一条边上的两点间的线路号//
int getline(Graph* G, int i, int j)
{
	enode* p = G->ver[i].fadj;
	while (p)
	{
		if (p->avex == j)
			return p->line;
		p = p->next;
	}
	return 0;
}

//求出每条路线上的转乘次数//
void gettrans(Graph* G, pathnode* all)
{
	int i;
	for (i = 0;i < all->pnum - 1;i++)
		if (getline(G, all->path[i], all->path[i + 1]) != getline(G, all->path[i + 1], all->path[i + 2]))
		{
			all->trans[i + 1] = 1;
			all->counter++;
		}
}

//排序找出两条推荐路径（距离）
void sortoutpath(LINK* L, double a[])
{
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	int i = 0;
	pathnode* p;
	double max = 999;
	p = L->head;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->distance <= max)
		
			max = p->distance;
		
	}
	        a[0] = max;
			max = 999;
			p = L->head;
			while (p->next != NULL)
			{
				p = p->next;
				if (p->distance != a[0])
				{
					if (p->distance <= max)
						max = p->distance;
				}
			}
			a[1] = max;
			p = L->head;

}

//排序找出两条推荐路径(时间)//
void sortoutpath1(LINK* L, double a[])
{
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	int i = 0;
	pathnode* p;
	double max = 999;
	p = L->head;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->time <= max)

			max = p->time;

	}
	a[0] = max;
	max = 999;
	p = L->head;
	while (p->next != NULL)
	{
		p = p->next;
		if (p->time != a[0])
		{
			if (p->time <= max)
				max = p->time;
		}
	}
	a[1] = max;
	p = L->head;

}

//功能四乘车路线推荐
void funcfour()
{
	int hour, min;
	char op;
	double time;
	char start[20],  destination[20];
	printf("请输入当前时刻\n");
	scanf("%d%c%d", &hour, &op, &min);
	if (hour >= 23 || hour < 6)
	{
		printf("当前时间地铁已停运\n");
		return;
	}
	time = timechange(hour, min);
	//printf("请输入起始站所在线路：");
	//scanf("%d", &m);
	printf("请输入起始站：");
	scanf("%s", start);
	//printf("请输入终点站站所在线路：");
	//scanf("%d", &n);
	printf("请输入终点站：");
	scanf("%s", destination);

	if (strcmp(start, destination) == 0)
		printf("您输入的站点名相同！");
	else
		lesstransfer(start, destination,time);//最小换乘接口//
}

//计算乘车距离
double caculate_distance(int a[])
{
	double distance=0;
	int i;
	for (i = 0;i < 8;i++)
	{
		distance = distance + a[i] * (Metro[i].linelength / Metro[i].number);
	}
	return distance;
}

//计算乘车时间
double caculate_time(int sitenumber, int transfernumber, double waitingtime, double factor)
{
	double time;
	double factor1;
	if (factor < 0.2)
		factor1 = 0.6;
	else if (factor > 0.75)
		factor1 = 1.5;
	else factor1 = 1;
	time = 3 * sitenumber * factor1  + 3.5 * transfernumber + waitingtime ;
	return time;
}

//计算票价
double caculate_price(double distance)
{
	if (distance < 0)
		return -1;

	if (distance < 4 || distance == 4)
		return 2;

	else if (distance < 12 || distance == 12)
		return (2 + ceil((distance - 4) / 4));

	else if (distance < 24 || distance == 24)
		return (4 + ceil((distance - 12) / 6));

	else if (distance < 40 || distance == 40)
		return (6 + ceil((distance - 24) / 8));

	else if (distance < 50 || distance == 50)
		return 9;

	else
		return(9 + ceil((distance - 50) / 20 + 1));
}

//获得拥挤程度系数
double getfactor(int line, double time)
{
	double s = time;
	double e = time + 2;//通过两站间路程所需时间
	if (!line)return 1;
	if (line == 1 || line == 3)
	{
		if (s >= 0 && s <= 90 && e >= 0 && e <= 90)
		{
			return 0.4;
		}
		if (s > 0 && s < 90 && e>90 && e < 180)
		{
			return (0.4 * (90 - s) + 0.8 * (e - 90)) / 2;
		}
		if (s >= 90 && s <= 180 && e >= 90 && e <= 180)
		{
			return 0.8;
		}
		if (s > 90 && s < 180 && e>180 && e < 630)
		{
			return (0.8 * (180 - s) + 0.4 * (e - 180)) / 2;
		}
		if (s > 180 && s < 630 && e>180 && e < 630)
		{
			return 0.4;
		}
		if (s > 180 && s < 630 && e>630 && e < 750)
		{
			return (0.4 * (630 - s) + 0.75 * (e - 630)) / 2;
		}
		if (s >= 630 && s <= 750 && e >= 630 && e <= 750)
		{
			return 0.75;
		}
		if (s > 630 && s < 750 && e>750 && e < 1020)
		{
			return (0.75 * (750 - s) + 0.4 * (e - 750)) / 2;
		}
		if (s > 750 && s <= 1020 && e > 750 && e <= 1020)
		{
			return 0.4;
		}
	}
	if (line == 6 || line == 7)
	{
		if (s >= 0 && s <= 780 && e >= 0 && e <= 780)
		{
			return 0.15;
		}
		if (s > 0 && s < 780 && e>780 && e < 960)
		{
			return (0.15 * (780 - s) + 0.65 * (e - 780)) / 2;
		}
		if (s >= 780 && s <= 960 && e >= 780 && e <= 960)
		{
			return 0.65;
		}
		if (s > 780 && s < 960 && e>960 && e < 1020)
		{
			return (0.65 * (960 - s) + 0.15 * (e - 960)) / 2;
		}
		if (s >= 960 && s <= 1020 && e >= 960 && e <= 1020)
		{
			return 0.15;
		}
	}
	if (line == 2 || line == 4)
	{
		return 0.5;
	}
	if (line == 8)
	{
		if (s >= 0 && s <= 210 && e >= 0 && e <= 210)
		{
			return 0.20;
		}
		if (s > 0 && s < 210 && e > 210 && e < 540)
		{
			return (0.2 * (210 - s) + 0.65 * (e - 210)) / 2;
		}
		if (s >= 210 && s <= 540 && e >= 210 && e <= 540)
		{
			return 0.65;
		}
		if (s > 210 && s < 540 && e > 540 && e < 1020)
		{
			return (0.65 * (540 - s) + 0.2 * (e - 540)) / 2;
		}
		if (s >= 540 && s <= 1020 && e >= 540 && e <= 1020)
		{
			return 0.2;
		}
	}
	return 1;
}

//功能六综合拥挤度线路推荐
void funcsix()
{
	int hour, min;
	char op;
	double time=0;
	char start[20], destination[20];
	printf("请输入当前时刻\n");
	scanf("%d%c%d", &hour, &op, &min);
	if (hour >= 23 || hour < 6)
	{
		printf("当前时间地铁已停运\n");
		return;
	}
	time = timechange(hour, min);
	//printf("请输入起始站所在线路：");
	//scanf("%d", &m);
	printf("请输入起始站：");
	scanf("%s", start);
	//printf("请输入终点站站所在线路：");
	//scanf("%d", &n);
	printf("请输入终点站：");
	scanf("%s", destination);

	if (strcmp(start, destination) == 0)
		printf("您输入的站点名相同！\n");
	else
	{
		Graph G;
		LINK L;
		int v1, v2;
		int no = 0;
		int stack[VERTEXNUM] = { 0 };     //初始化
		int  visited[VERTEXNUM] = { 0 };
		create_graph1(&G);	 //创建一个无向网络//
		create_path(&L);	//创建保存路径的链表//
		v1 = locatevex1(&G, start);
		v2 = locatevex1(&G, destination);

		if (v1 == -1 || v2 == -1)
		{
			printf("您输入的站名有误！\n");
			return;
		}

		DFS(&G, stack, visited, v1, v2, 0, &L, no);
		distanceandtime1(&G, &L, time);
		sortoutpath1(&L, a);
		print_lesstransfer1(&G, &L);
	}
}

//功能五拥挤度设置//
void funcfive()
{
	char ch;
	int line;
	double cfactor1;
	do
	{
		printf("请输入要更改拥挤度的线路:\n");
		scanf("%d", &line);
		printf("拥挤度更改为:\n");
		scanf("%lf", &cfactor1);
		cfactor[line - 1] = cfactor1;
		printf("是否继续更改拥挤度y/n\n");
		getchar();
		ch = getchar();
	} while (ch == 'y' || ch == 'Y');
	return;
}

//功能七更改拥挤度后线路推荐及避开拥挤
void funcseven()
{
	char ch;
	printf("是否需要设置避开拥挤y/n\n");
	getchar();
	ch = getchar();
	if (ch == 'y' || ch == 'Y')
	{
		char start[20], destination[20];
		double time;
		printf("请输入起始站：");
		scanf("%s", start);
		printf("请输入终点站：");
		scanf("%s", destination);
		printf("请输入要避开的拥挤度系数0~1:");
		scanf("%lf", &time);
		if (strcmp(start, destination) == 0)
			printf("您输入的站点名相同！\n");
		else
		{
			Graph G;
			LINK L;
			int v1, v2;
			int no = 0;
			int stack[VERTEXNUM] = { 0 };     //初始化
			int  visited[VERTEXNUM] = { 0 };
			create_graph1(&G);	 //创建一个无向网络//
			create_path(&L);	//创建保存路径的链表//
			v1 = locatevex1(&G, start);
			v2 = locatevex1(&G, destination);

			if (v1 == -1 || v2 == -1)
			{
				printf("您输入的站名有误！\n");
				return;
			}

			DFS(&G, stack, visited, v1, v2, 0, &L, no);
			distanceandtime3(&G, &L, time);
			sortoutpath1(&L, a);
			print_lesstransfer2(&G, &L);
		}
	}

	else if (ch == 'n' || ch == 'N')
	{
		char start[20], destination[20];
		printf("请输入起始站：");
		scanf("%s", start);
		printf("请输入终点站：");
		scanf("%s", destination);

		if (strcmp(start, destination) == 0)
			printf("您输入的站点名相同！\n");
		else
		{
			Graph G;
			LINK L;
			int v1, v2;
			int no = 0;
			int stack[VERTEXNUM] = { 0 };     //初始化
			int  visited[VERTEXNUM] = { 0 };
			create_graph1(&G);	 //创建一个无向网络//
			create_path(&L);	//创建保存路径的链表//
			v1 = locatevex1(&G, start);
			v2 = locatevex1(&G, destination);

			if (v1 == -1 || v2 == -1)
			{
				printf("您输入的站名有误！\n");
				return;
			}

			DFS(&G, stack, visited, v1, v2, 0, &L, no);
			distanceandtime2(&G, &L);
			sortoutpath1(&L, a);
			print_lesstransfer1(&G, &L);
		}
	}
	else
	{
		printf("输入指令有误\n");
		return;
	}
}