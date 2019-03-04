 /*首次适应*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#define getpch(type)(type*)malloc(sizeof(type))
#define Size 4

struct item {//定义一个表项
    int  size;            /* 区域大小 */
    int  sadd;            /* 起始地址 */
	int  state;           /* 状态,0表是未被分配,1表中已分配 */
    char name[10];        /* 分配作业 */
	int used;             /* 已分配空间 */
	int need;             /* 作业大小 */
	struct item *prior;   /* 前指针 */
	struct item *next;    /* 后指针 */
}*p;


typedef struct item ITEM;

int address=0;
char aname[10];  //作业名
int aneed;       //作业大小
char bname[10];

ITEM *head=NULL, *tail=NULL;   //空闲链头尾结点

//---------------------------disp1()------------------------------------------

void disp1()
{//打印出分区说明表
	int len=1;
	ITEM *pr;
	pr=head;
	printf("                    ******************可变分区*****************\n");
    printf("                    *                                         *\n");
    printf("                    *                 首次适应                *\n");
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
    printf("当前分区表分配情况如下:\n");
    printf("******************************************************************************\n");
    printf("* 分区号 *  起址(K) *  大小(K) *  状态  *  分配作业 *  作业大小 * 已分配空间 *\n");
    printf("******************************************************************************\n");
	while(pr!=NULL)
	{
		printf("|   %2d   |   %4d   |    %3d   |  %3d   |   %5s   |    %2d     |   %3d      |\n",len,pr->sadd,pr->size,pr->state,pr->name,pr->need,pr->used);
		printf("------------------------------------------------------------------------------\n");
		pr=pr->next;
		len++;
	}
	printf("\n------------------------------------------------------------------------------\n");
}



//--------------------------create()-------------------------------------------

void create()
{
	int i,n=rand()%6+1;
	for(i=0;i<n;i++)
	{
		p=getpch(ITEM);
		p->size=rand()%500+8;
		p->sadd=address;
		p->state=0;
		strcpy(p->name,"无");
		p->need=0;
		p->used=0;
		p->next=NULL;
		p->prior=NULL;
		if(head==NULL)
		{
			p->prior=head;
			p->next=head;
			head=p;
			tail=p;
		}
		else
		{
			tail->next=p;
			p->prior=tail;
			p->next=NULL;
			tail=p;
		}
		address=address+p->size;
	}
	disp1();
}


//-------------------------------input()---------------------------------------

void input()
{//添加作业
    printf("请输入作业名:");
    scanf("%s",aname);
    printf("请输入作业大小:");
    scanf("%d",&aneed);
    printf("\n");
}


//-------------------------------assign()--------------------------------------

void assign()
{//为新添加的作业分配存储空间，按首次适应算法
	ITEM *qr;
	int flag=0;
	qr=head;
    while(qr!=NULL&&flag!=1)
    {//作一个flag作标记，如果分配成功则flag=1，否则为0
		if(((qr->size-aneed)>=0)&&qr->state!=1)
		{//如果分区空间满足作业的大小要求，且分区尚未分配，就把分区分配给作业
			if((qr->size-aneed)<=Size)
			{  //空闲分区(qr->size-aneed)<=Size,直接把空闲分区分给作业
				qr->state=1;
				strcpy(qr->name,aname);
				qr->need=aneed;
				qr->used=qr->size;
				flag=1;
			}
			else
			{	//空闲分区(qr->size-aneed)>Size,把空闲分区分给作业大的空间，剩下的留在空闲链
				p=getpch(ITEM);	  //生成新的结点
				p->size=aneed;
				p->sadd=qr->sadd;
				p->state=1;
				strcpy(p->name,aname);
				p->need=aneed;
				p->used=aneed;
				p->next=NULL;
				p->prior=NULL;
				//将生成的p结点插入到结点qr前面
				if(qr==head)
				{   //
					qr->prior=p;
					p->next=qr;
					head=p;
				}
				else
				{
					p->prior=qr->prior;
				    qr->prior->next=p;
				    p->next=qr;
				    qr->prior=p;
				}
				//剩下的空间
				qr->sadd=qr->sadd+aneed;
				qr->size=qr->size-aneed;
			//	qr=qr->next;
				flag=1;
			}
		}
		else qr=qr->next;
	}
	if(flag==1) printf("分配成功!\n");
	else printf("内存不足,分配失败!\n");
}



//--------------------------------destroy()-------------------------------------

void destroy()
{//回收作业，如果要回收和作业名在分区说明表中有，就把分区回收
    int i=0,flag=0;
	ITEM *pr,*sec;
	pr=head;
	printf("请输入要回收的作业名:");
	scanf("%s",bname);
    while(flag!=1&&pr!=NULL)
    {
        if((strcmp(pr->name,bname)==0)&&pr->state!=0)
        {//如果在作业链中找到要回收的结点，则对其作回收操作
			if(pr==head)
			{//操作结点是第一个
				pr->state=0;
				strcpy(pr->name,"无");
				pr->need=0;
				pr->used=0;
				flag=1;
			}
			else
			{
				if((pr->sadd==pr->prior->sadd+pr->prior->size)&&pr->prior->state==0)
				{//回收空间与前一个空闲分区相连
					pr->prior->size=pr->prior->size+pr->size;
				    pr->prior->next=pr->next;
				    pr->next->prior=pr->prior;
				    free(pr);
				    flag=1;
				}
				else
				{
					if((pr->next->sadd==pr->sadd+pr->size)&&pr->next->state==0)
					{//回收空间与后一个空闲分区相连
						sec=pr->next;
						pr->size=pr->size+pr->next->size;
						pr->state=0;
						strcpy(pr->name,"无");
						pr->need=0;
						pr->used=0;
						sec->prior->next=sec->next;
						sec->next->prior=sec->prior;
						free(sec);
						flag=1;
					}
					else
					{//回收空间与前后空闲分区都不相连
						pr->state=0;
						strcpy(pr->name,"无");
						pr->need=0;
						pr->used=0;
						flag=1;
					}
				}
			}
		}
		else pr=pr->next;
	}
	if(flag==1) printf("回收成功!\n");
	else printf("作业不存在!\n");
}



//-------------------------------main()----------------------------------------

int main()
{
	char ch;
	int n=0;
	void clrscr(); /*清屏*/
	create();
	while(n!=3)
	{//循环的执行操作，直到操作为退出
		printf("  \n    1.添加作业            2.回收分区          3.退出quit\n");
		printf("\n------------------------------------------------------------------------------\n");
		printf("\n请选择操作:");
		scanf("%d",&n);
		switch(n)
		{
		case 1:{
			    input();
                assign();
				disp1();
				ch=getch();
			   };break;
		case 2:{
                destroy();
                disp1();
				ch=getch();
			   };break;
		case 3:break;
		}
	}
	ch=getch();
}
