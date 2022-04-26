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
#define HEADER1 "---------------�人������·----------------\n"
#define HEADER2 "| ��·���� | վ���� | ��Ա���� | ��·���� |\n"
#define HEADER3 "|----------|--------|----------|----------|\n"
#define HEADER4 "-------------------------------------------\n"
#define HEADER5 "--------------------------------------------\n"
#define HEADER6 "|  վ������  | �Ƿ�ɻ��� |  ������·���  |\n"

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
int LINENUM = 8;               //����������//
int istrans[VERTEXNUM] = { 0 };//���˵���Ϣ�洢����//
double a[3] = { 0 };                ////
int no = 0;                         ////  
double cfactor[8] = { 0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6 };//����ӵ������Ϣ����//

typedef struct MetroLine
{
	int NO;							//��·���//
	char name[15];					//��·����//
	int number;						//վ����//
    int fullnumber;                 //��Ա����//
	double linelength;              //��·����//
}LINE;

LINE Metro[8];

typedef struct transstation
{
	char name[15];                  //����վ������//
	int line[3];                    //����վ����·��Ϣ����//
	int triple;                     //�ж��Ƿ�Ϊ������վ��//
}transstation;

transstation trans[27];

typedef struct enode
{
	int avex;                       //�ڽӵ��λ��//
	int line;                       //��·���//
	struct enode* next;             //ָ����һ���ڽӵ�//
}enode;

typedef struct vnode
{
	int id;                         //����ı��//
	char name[20];                  //վ������//
	enode* fadj;					//ָ���һ����//
}vnode;

typedef struct Graph
{
	vnode ver[VERTEXNUM];           //���㼯��(vertex)//
	int vexnum;                     //ͼ�Ķ�����Ŀ//
	int arcnum;                     //ͼ�ı���Ŀ//

}Graph;

typedef struct path
{
	int pnum;                       //·������Ŀ//
	int path[VERTEXNUM];            //·����Ϣ//
	int trans[VERTEXNUM];		    //���˵���Ϣ,0Ϊ�ǻ��˵㣬1Ϊ���˵�//
	int counter;                    //���˵Ĵ���//
	double distance;                //·������//
	double time;                    //·������ʱ��//
	double factor;                  //·��ƽ��ӵ��������//
	struct path* next;              //ָ����һ��·����Ϣ�ڵ�//
}pathnode;


typedef struct LINK
{
	pathnode* head;                 //·���ӿ�//
}LINK;


//�˵�
void menu();

//��·��ȡ
void readline(LINE a[]);

//����վ���ȡ
void readtrans(transstation a[]);

//����վ���ж�
int judgetrans(char a[]);

//��·��ʾ
int showline(LINE a[], int i);

//����ͼ�Ľ���
void create_graph(Graph* G);

//����һ��ʾ��·վ��
void funcone();

//��ȡվ����
int locatevex(Graph* G, char a[],int n);

//վ��·����ʾ
int showstation(Graph* G, int num);

//���ܶ���·��ѯ
void functwo();

//���㳵����վ��վʱ��
void arrivaldeparture(int line, int num, double *time0, double *time1);

//ʱ��ת������
double timechange(int a, int b);

//��������������ʱ�̱�
void functhree();

//DFS����������������п��ܵ�·���߷�//
void DFS(Graph* G, int stack[], int visited[], int v, int destination, int pnum, LINK* L, int no);

//������ٻ���·����Ϣ//
void print_lesstransfer(Graph* G, LINK* L);

//��ȡ���˵�//
void gettrans(Graph* G, pathnode* all);

//���ٻ��˵ĺ����ӿڣ�����1����ʼվ������2���յ�վ//
void lesstransfer(char start[], char destination[],double time);

//����һ��·������//
void create_path(LINK* L);

//����һ��·����Ϣ//
void addpath(LINK* L, pathnode* p);

//����·������ʱ��//
void distanceandtime(Graph* G, LINK* L);

//����·������,��Ȩʱ��//
void distanceandtime1(Graph* G, LINK* L, double time);

//�����ҳ������Ƽ�·��(����)//
void sortoutpath(LINK* L, double a[]);

//�����ҳ������Ƽ�·��(ʱ��)//
void sortoutpath1(LINK* L, double a[]);

//�����ĳ˳�·���Ƽ�//
void funcfour();

//����˳�����//
double caculate_distance(int a[]);

//����˳�ʱ��//
double caculate_time(int sitenumber, int transfernumber, double waitingtime, double factor);

//����Ʊ��//
double caculate_price(double distance);

//������ӵ��������//
void funcfive();

//���ӵ���̶�ϵ��//
double getfactor(int line, double time);

//�������ۺ�ӵ������·�Ƽ�
void funcsix();

//�����߸���ӵ���Ⱥ���·�Ƽ����ܿ�ӵ��
void funcseven();

int main()
{
	string passwd;
	char c;
	ifstream in("passwd.txt");
	in >> passwd;
	while (1)
	{
		cout << "�������¼����:\n";
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
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              ����ϵͳ��.";
			Sleep(500);
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              ����ϵͳ��..";
			Sleep(500);
			system("cls");
			cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << "                                                              ����ϵͳ��...";
			Sleep(500);
			system("cls");
			break;
		}
		else cout << "�������" << endl;
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
		printf("������<0-7>:");
		scanf("%d", &label);
		if (label == 0) { printf("��л����ʹ�ã�\n"); Sleep(1500);exit(0); }
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
			printf("�������ָ������\n");
		}
		printf("\n�����������...\n");
		_getch();
	}
	return 0;
}

//�˵�
void menu()
{

	printf("\t|-----------------------------------------------|\n");
	printf("\t|            �人������·�����Ƽ�               |\n");
	printf("\t|-----------------------------------------------|\n");
	printf("\t|       0:�˳�ϵͳ                              |\n");
	printf("\t|       1:��ʾ��·վ��                          |\n");
	printf("\t|       2:��·��ѯ                              |\n");
	printf("\t|       3:��������ʱ�̱�                        |\n");
	printf("\t|       4:�˳���·�Ƽ�                          |\n");
	printf("\t|       5:ӵ��������                            |\n");
	printf("\t|       6:�ۺ�ӵ������·�Ƽ�                    |\n");
	printf("\t|       7:����ӵ���Ⱥ���·�Ƽ����ܿ�ӵ��        |\n");
	printf("\t|-----------------------------------------------|\n");

}

//��·��ȡ
void readline(LINE a[])
{
	int NO;							//��·���//
	char name[20];					//��·����//
	int number;						//վ����//
	int fullnumber;                 //��Ա����//
	double linelength;              //��·����//
	int counter=8;                  //������//
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

//����վ���ȡ
void readtrans(transstation a[])
{
	char name[15];                  //����վ������//
	int line1, line2, line3;        //����վ����·��Ϣ����//
	int triple;                     //�ж��Ƿ�Ϊ������վ��//
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

//����վ���ж�
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
//����ͼ�Ľ���
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
		//˫������
		p = (enode*)malloc(sizeof(enode));
		p->avex = i;
		p->line = line;
		p->next = G->ver[j].fadj;
		G->ver[j].fadj = p;
	}
	fclose(fp);
}

//��·��ʾ
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

//����һ��ʾ��·վ��
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
		printf("������Ҫ��ѯ�ĵ�����·");
		scanf("%d", &i);
		if (showline(Metro, i) == 0)
			printf("������·����\n");
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
				printf("-----------------------------------����%d����-----------------------------------\n",Metro[i].NO);
				i++;
				for (j = temp;c[j] == i;j++)
				{
					printf("%s->", G.ver[a[j]].name);
				}
				printf("%s\n", G.ver[b[--j]].name);
				temp = ++j;
				fclose(fp);
			}
			
		printf("�Ƿ������ѯ��·y/n");
		getchar();
		ch = getchar();
	} while (ch == 'y' || ch == 'Y');
	
}

//��ȡվ����
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

//վ��·����ʾ
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
	printf("��ѯ��վ��ǰ/��վ�㣨������0/1��\n");
	scanf("%d", &direction);
	if (direction > 1 || direction < 0)
	{
		printf("�������\n");
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
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[0], trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[0], trans[t].line[2]);
					if (trans[t].line[0] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[1], trans[t].line[2]);
				}
				else
				{
					if (trans[t].line[0] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d\n", trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d\n", trans[t].line[0]);
				}

				printf("�Ƿ񻻳�y/n\n");
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
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[0], trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[0], trans[t].line[2]);
					if (trans[t].line[0] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d,%d\n", trans[t].line[1], trans[t].line[2]);
				}
				else
				{
					if (trans[t].line[0] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d\n", trans[t].line[1]);
					if (trans[t].line[1] == line)
						printf("��վ��Ϊ����վ���ɻ�����·Ϊ%d\n", trans[t].line[0]);
				}

				printf("�Ƿ񻻳�y/n\n");
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
//���ܶ���·��ѯ
void functwo()
{
	int i=1, num;
	int flag = 0;
	char name[20];
	Graph G;
	enode* p;
	create_graph(&G);
	readtrans(trans);
	printf("������Ҫ��ѯ����·\n");
	scanf("%d", &i);
	if (i < 1 || i>8 || i == 5)
	{
		printf("������·������\n");
		return;
	}
	else
	{
		printf("������Ҫ��ѯ��վ��\n");
		getchar();	
		gets_s(name);
		num = locatevex(&G, name, i);
		if (num == -1)
		{
			printf("����վ�㲻����\n");
			return;
		}
		flag = showstation(&G,num);
		while (flag)
		{
			printf("������Ҫ���˵���·\n");
			scanf("%d", &i);
			num=locatevex(&G, G.ver[flag].name, i);
			if (num == -1)
			{
				printf("������·��վ�㲻����\n");
				break;
			}
			flag=showstation(&G, num);
		}
			
	}
	
}

//���㳵����վ��վʱ��
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

//��������������ʱ�̱�
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
	printf("�����뵱ǰʱ��\n");
	scanf("%d%c%d", &hour, &op, &min);
	printf("������Ҫ��ѯ����·\n");
	scanf("%d", &line);
	if (line < 1 || line > 8 || line == 5)
	{
		printf("������·������\n");
		return;
	}
	else
	{
		printf("������Ҫ��ѯ��վ��\n");
		getchar();
		gets_s(name);
		num = locatevex(&G, name, line);
		if (num == -1)
		{
			printf("����վ�㲻����\n");
			return;
		}
		arrivaldeparture(line, num, &time0, &time1);
		t1 = (hour - 6) * 60 + min;
		t2 = (hour - 23) * 60 + min;
		if (t1 < time0||t2 > time0)
			printf("���յ�վ�����ʱ�޳���\n");
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
			printf("���յ�վ��������վʱ��%02d:%02d\n", hour0, min0);
			printf("���յ�վ�������뿪ʱ��%02d:%02d\n", hour0, min0 + 1);
		}
		
		if (t1 < time1 || t2 > time1)
			printf("����ʼվ�����ʱ�޳���\n");
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
			printf("����ʼվ��������վʱ��%02d:%02d\n", hour1, min1);
			printf("����ʼվ�������뿪ʱ��%02d:%02d\n", hour1, min1 + 1);
		}
		
	}
}

//ʱ��ת������
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

 //2����ͼ�Ľ���
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
		//˫������
		p = (enode*)malloc(sizeof(enode));
		p->avex = i;
		p->line = line;
		p->next = G->ver[j].fadj;
		G->ver[j].fadj = p;
	}
	fclose(fp);
}


//���ٻ��˵ĺ����ӿڣ�����1����ʼվ������2���յ�վ//
void lesstransfer(char start[], char destination[],double time)
{
	Graph G;
	LINK L;
	int v1, v2;
	int no=0;
	int stack[VERTEXNUM] = { 0 };     //��ʼ��
	int  visited[VERTEXNUM] = { 0 };
	create_graph1(&G);	 //����һ����������//
	create_path(&L);	//��������·��������//
	v1 = locatevex1(&G, start);
	v2 = locatevex1(&G, destination);

	if (v1 == -1 || v2 == -1)
	{
		printf("�������վ������\n");
		return;
	}

	DFS(&G, stack, visited, v1, v2, 0, &L,no);
	distanceandtime(&G, &L);
	sortoutpath(&L, a);
	print_lesstransfer(&G, &L);

}

/*������������ĵݹ��ҳ����е�·��
����һ��ͼ  ����2����ʱ���·��˳������  ����3���Ƿ����  ����4����ǰλ��  ����5�������λ��  ����6��ջ��  ����7������·��������*/
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
		return;//�ݹ�߽�����
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

//·�����룬ʱ�����//
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

//·�����룬��Ȩʱ�����//
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

//���·��//
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
					printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("����Ҫ����\n");
			else
			printf("����վ��:%d��\n", i);
			printf("�˳�����Ϊ:%gǧ��\n", r->distance);
			printf("�˳�ʱ��Ϊ:%g����\n", r->time);
			printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
			break;//Ϊ�˱����ظ���ֱ������
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
							printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
							i++;
						}
						else
						{
							line = G->ver[r->path[k]].fadj->line;
							last = line;
							printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
							i++;
						}
					}
					printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
					printf("-------------------------------------------------------------------------------\n");
					if (r->counter == 0)
						printf("����Ҫ����\n");
					else
					printf("����վ��:%d��\n", i);
					printf("�˳�����Ϊ:%gǧ��\n", r->distance);
					printf("�˳�ʱ��Ϊ:%g����\n", r->time);
					printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
					break;//Ϊ�˱����ظ���ֱ������
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
				printf("���ٻ���·��:\n");
				printf("------------------------->-------------------------->------------------------->\n");
				for (k = 0;k < (r->pnum);k++)
				{
					if (r->trans[k])
					{
						printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
				printf("-------------------------------------------------------------------------------\n");
				if (r->counter == 0)
					printf("����Ҫ����\n");
				else
				printf("����վ��:%d��\n", i);
				printf("�˳�����Ϊ:%gǧ��\n", r->distance);
				printf("�˳�ʱ��Ϊ:%g����\n", r->time);
				printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
				break;//Ϊ�˱����ظ���ֱ������
			}
		}
	}

//1���·��//
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
					printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("����Ҫ����\n");
			else
			printf("����վ��:%d��\n", i);
			printf("�˳�����Ϊ:%gǧ��\n", r->distance);
			printf("�˳���Ȩʱ��Ϊ:%g����\n", r->time);
			printf("�˳�ƽ��ӵ���̶�Ϊ:%g\n", r->factor);
			printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
			break;//Ϊ�˱����ظ���ֱ������
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
						printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
				printf("-------------------------------------------------------------------------------\n");
				if (r->counter == 0)
					printf("����Ҫ����\n");
				else
				printf("����վ��:%d��\n", i);
				printf("�˳�����Ϊ:%gǧ��\n", r->distance);
				printf("�˳���Ȩʱ��Ϊ:%g����\n", r->time);
				printf("�˳�ƽ��ӵ���̶�Ϊ:%g\n", r->factor);
				printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
				break;//Ϊ�˱����ظ���ֱ������
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
			printf("���ٻ���·��:\n");
			printf("------------------------->-------------------------->------------------------->\n");
			for (k = 0;k < (r->pnum);k++)
			{
				if (r->trans[k])
				{
					printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			if (r->counter == 0)
				printf("����Ҫ����\n");
			else
			printf("����վ��:%d��\n", i);
			printf("�˳�����Ϊ:%gǧ��\n", r->distance);
			printf("�˳���Ȩʱ��Ϊ:%g����\n", r->time);
			printf("�˳�ƽ��ӵ���̶�Ϊ:%g\n", r->factor);
			printf("Ʊ��Ϊ:%gԪ\n", caculate_price(r->distance));
			break;//Ϊ�˱����ظ���ֱ������
		}
	}
}

//2���·��//
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
					printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
					i++;
				}
				else
				{
					line = G->ver[r->path[k]].fadj->line;
					last = line;
					printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
					i++;
				}
			}
			printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
			printf("-------------------------------------------------------------------------------\n");
			break;//Ϊ�˱����ظ���ֱ������
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
						printf("%s(%d����)-->", G->ver[r->path[k]].name, last);
						i++;
					}
					else
					{
						line = G->ver[r->path[k]].fadj->line;
						last = line;
						printf("%s(%d����)-->", G->ver[r->path[k]].name, line);
						i++;
					}
				}
				printf("%s(�յ�)\n", G->ver[r->path[(r->pnum)]].name);
			    printf("-------------------------------------------------------------------------------\n");
				break;
			}
		}
	}
}
void create_path(LINK* L)
{
	//�½��������//
	L->head = (pathnode*)malloc(sizeof(pathnode));
	L->head->next = NULL;
}

void addpath(LINK* L, pathnode* p)
{
	//����Ĳ������//
	p->next = L->head->next;
	L->head->next = p;
}

//���ͬһ�����ϵ���������·��//
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

//���ÿ��·���ϵ�ת�˴���//
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

//�����ҳ������Ƽ�·�������룩
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

//�����ҳ������Ƽ�·��(ʱ��)//
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

//�����ĳ˳�·���Ƽ�
void funcfour()
{
	int hour, min;
	char op;
	double time;
	char start[20],  destination[20];
	printf("�����뵱ǰʱ��\n");
	scanf("%d%c%d", &hour, &op, &min);
	if (hour >= 23 || hour < 6)
	{
		printf("��ǰʱ�������ͣ��\n");
		return;
	}
	time = timechange(hour, min);
	//printf("��������ʼվ������·��");
	//scanf("%d", &m);
	printf("��������ʼվ��");
	scanf("%s", start);
	//printf("�������յ�վվ������·��");
	//scanf("%d", &n);
	printf("�������յ�վ��");
	scanf("%s", destination);

	if (strcmp(start, destination) == 0)
		printf("�������վ������ͬ��");
	else
		lesstransfer(start, destination,time);//��С���˽ӿ�//
}

//����˳�����
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

//����˳�ʱ��
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

//����Ʊ��
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

//���ӵ���̶�ϵ��
double getfactor(int line, double time)
{
	double s = time;
	double e = time + 2;//ͨ����վ��·������ʱ��
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

//�������ۺ�ӵ������·�Ƽ�
void funcsix()
{
	int hour, min;
	char op;
	double time=0;
	char start[20], destination[20];
	printf("�����뵱ǰʱ��\n");
	scanf("%d%c%d", &hour, &op, &min);
	if (hour >= 23 || hour < 6)
	{
		printf("��ǰʱ�������ͣ��\n");
		return;
	}
	time = timechange(hour, min);
	//printf("��������ʼվ������·��");
	//scanf("%d", &m);
	printf("��������ʼվ��");
	scanf("%s", start);
	//printf("�������յ�վվ������·��");
	//scanf("%d", &n);
	printf("�������յ�վ��");
	scanf("%s", destination);

	if (strcmp(start, destination) == 0)
		printf("�������վ������ͬ��\n");
	else
	{
		Graph G;
		LINK L;
		int v1, v2;
		int no = 0;
		int stack[VERTEXNUM] = { 0 };     //��ʼ��
		int  visited[VERTEXNUM] = { 0 };
		create_graph1(&G);	 //����һ����������//
		create_path(&L);	//��������·��������//
		v1 = locatevex1(&G, start);
		v2 = locatevex1(&G, destination);

		if (v1 == -1 || v2 == -1)
		{
			printf("�������վ������\n");
			return;
		}

		DFS(&G, stack, visited, v1, v2, 0, &L, no);
		distanceandtime1(&G, &L, time);
		sortoutpath1(&L, a);
		print_lesstransfer1(&G, &L);
	}
}

//������ӵ��������//
void funcfive()
{
	char ch;
	int line;
	double cfactor1;
	do
	{
		printf("������Ҫ����ӵ���ȵ���·:\n");
		scanf("%d", &line);
		printf("ӵ���ȸ���Ϊ:\n");
		scanf("%lf", &cfactor1);
		cfactor[line - 1] = cfactor1;
		printf("�Ƿ��������ӵ����y/n\n");
		getchar();
		ch = getchar();
	} while (ch == 'y' || ch == 'Y');
	return;
}

//�����߸���ӵ���Ⱥ���·�Ƽ����ܿ�ӵ��
void funcseven()
{
	char ch;
	printf("�Ƿ���Ҫ���ñܿ�ӵ��y/n\n");
	getchar();
	ch = getchar();
	if (ch == 'y' || ch == 'Y')
	{
		char start[20], destination[20];
		double time;
		printf("��������ʼվ��");
		scanf("%s", start);
		printf("�������յ�վ��");
		scanf("%s", destination);
		printf("������Ҫ�ܿ���ӵ����ϵ��0~1:");
		scanf("%lf", &time);
		if (strcmp(start, destination) == 0)
			printf("�������վ������ͬ��\n");
		else
		{
			Graph G;
			LINK L;
			int v1, v2;
			int no = 0;
			int stack[VERTEXNUM] = { 0 };     //��ʼ��
			int  visited[VERTEXNUM] = { 0 };
			create_graph1(&G);	 //����һ����������//
			create_path(&L);	//��������·��������//
			v1 = locatevex1(&G, start);
			v2 = locatevex1(&G, destination);

			if (v1 == -1 || v2 == -1)
			{
				printf("�������վ������\n");
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
		printf("��������ʼվ��");
		scanf("%s", start);
		printf("�������յ�վ��");
		scanf("%s", destination);

		if (strcmp(start, destination) == 0)
			printf("�������վ������ͬ��\n");
		else
		{
			Graph G;
			LINK L;
			int v1, v2;
			int no = 0;
			int stack[VERTEXNUM] = { 0 };     //��ʼ��
			int  visited[VERTEXNUM] = { 0 };
			create_graph1(&G);	 //����һ����������//
			create_path(&L);	//��������·��������//
			v1 = locatevex1(&G, start);
			v2 = locatevex1(&G, destination);

			if (v1 == -1 || v2 == -1)
			{
				printf("�������վ������\n");
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
		printf("����ָ������\n");
		return;
	}
}