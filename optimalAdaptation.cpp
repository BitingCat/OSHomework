/*动态分区分配-最佳适应算法*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#define getpch(type)(type*)malloc(sizeof(type))
#define Size 4

//定义一个空闲分区链表
struct item
{
    int  size;            /* 区域大小 */
    int  startaddress;    /* 起始地址 */
    int  state;           /* 状态,0表是未被分配,1表中已分配 */
    char name[10];        /* 分配作业 */
    int used;             /* 已分配空间 */
    int need;             /* 作业大小 */
    struct item *prior;   /* 前指针 */
    struct item *next;    /* 后指针 */
}*p;

typedef struct item ITEM;

int address = 0;
char aname[10];  //作业名
int aneed;       //作业大小
char bname[10];

ITEM *head = NULL, *tail = NULL;   //空闲链头尾结点

//---------------------------disp()------------------------------------------

void disp()
{
    //打印出分区说明表
    int len = 1;
    ITEM *pr;
    pr = head;
    printf("                    ******************动态分区分配***************\n");
    printf("                    *                                           *\n");
    printf("                    *                 最佳适应算法              *\n");
    printf("                    *                                           *\n");
    printf("                    *********************************************\n");
    printf("当前分区表分配情况如下:\n");
    printf("******************************************************************************\n");
    printf("* 分区号 *  起址(K) *  大小(K) *  状态  *  分配作业 *  作业大小 * 已分配空间 *\n");
    printf("******************************************************************************\n");
    while (pr != NULL)
    {
        printf("|   %2d   |   %4d   |    %3d   |  %3d   |   %5s   |    %2d     |   %3d      |\n", len, pr->startaddress, pr->size, pr->state, pr->name, pr->need, pr->used);
        printf("------------------------------------------------------------------------------\n");
        pr = pr->next;
        len++;
    }
    printf("\n------------------------------------------------------------------------------\n");
}

//--------------------------create()-------------------------------------------

void create()
{
    int i, n = rand() % 6 + 1;
    for (i = 0; i < n; i++)
    {
        p = getpch(ITEM);
        p->size = rand() % 500 + 8;
        p->startaddress = address;
        p->state = 0;
        strcpy(p->name, "无");
        p->need = 0;
        p->used = 0;
        p->next = NULL;
        p->prior = NULL;
        if (head == NULL)
        {
            p->prior = head;
            p->next = head;
            head = p;
            tail = p;
        }
        else
        {
            tail->next = p;
            p->prior = tail;
            p->next = NULL;
            tail = p;
        }
        address = address + p->size;
    }
    disp();
}

//添加作业
void input()
{
    printf("请输入作业名:");
    scanf("%s", aname);
    printf("请输入作业大小:");
    scanf("%d", &aneed);
    printf("\n");
}

//为新添加的作业分配存储空间，最佳适应
void assign()
{
    ITEM *qr, *best_position;
    int flag = 0;
    qr = head;
    int value = qr->size - aneed;
    if (qr != NULL && flag != 1)
    {
        do
        {
            if((qr->size - aneed >= 0) && (qr->state == 0))
            {
                if((qr->size - aneed) < value)
                {
                    best_position = qr;
                    value = qr->size - aneed;
                }
            }
            qr = qr->next;
        }while(qr != NULL);

        if(best_position != NULL)
        {
            //printf("%d\n",best_position->size);
            if(best_position->size == aneed)
            {
                best_position->state = 1;
                strcpy(best_position->name, aname);
                best_position->need = aneed;
                best_position->used = best_position->size;
                flag = 1;
            }
            else
            {
                p = getpch(ITEM); //生成新的结点
                p->size = aneed;
                p->startaddress = best_position->startaddress;
                p->state = 1;
                strcpy(p->name, aname);
                p->need = aneed;
                p->used = aneed;
                p->next = NULL;
                p->prior = NULL;

                //把新生成的节点插入best_position前面
                if (best_position == head)
                {
                    //
                    best_position->prior = p;
                    p->next = best_position;
                    head = p;
                }
                else
                {
                    //printf("ceshi");
                    p->prior = best_position->prior;
                    best_position->prior->next = p;
                    p->next = best_position;
                    best_position->prior = p;

                }
                //剩下的空间
                best_position->startaddress = best_position->startaddress + aneed;
                best_position->size = best_position->size - aneed;
                flag = 1;
            }
        }
        if (flag == 1)
        {
            printf("分配成功!\n");
        }
        else
        {
            printf("内存不足,分配失败!\n");
        }
    }
}

//回收作业，如果要回收和作业名在分区说明表中有，就把分区回收
void destroy()
{
    int i = 0, flag = 0;
    ITEM *pr, *sec;
    pr = head;
    printf("请输入要回收的作业名:");
    scanf("%s", bname);
    while (flag != 1 && pr != NULL)
    {
        if ((strcmp(pr->name, bname) == 0) && pr->state != 0)
        {
            //如果在作业链中找到要回收的结点，则对其作回收操作
            if (pr == head)
            {
                //操作结点是第一个
                pr->state = 0;
                strcpy(pr->name, "无");
                pr->need = 0;
                pr->used = 0;
                flag = 1;
            }
            else
            {
                if ((pr->startaddress == pr->prior->startaddress + pr->size) && pr->prior->state == 0)
                {
                    //回收空间与前一个空闲分区相连
                    pr->prior->size = pr->prior->size + pr->size;
                    pr->prior->next = pr->next;
                    pr->next->prior = pr->prior;
                    free(pr);
                    flag = 1;
                }
                else
                {
                    if ((pr->next->startaddress == pr->startaddress + pr->size) && pr->next->state == 0)
                    {
                        //回收空间与后一个空闲分区相连
                        sec = pr->next;
                        pr->size = pr->size + pr->next->size;
                        pr->state = 0;
                        strcpy(pr->name, "无");
                        pr->need = 0;
                        pr->used = 0;
                        sec->prior->next = sec->next;
                        sec->next->prior = sec->prior;
                        free(sec);
                        flag = 1;
                    }
                    else
                    {
                        //回收空间与前后空闲分区都不相连
                        pr->state = 0;
                        strcpy(pr->name, "无");
                        pr->need = 0;
                        pr->used = 0;
                        flag = 1;
                    }
                }
            }
        }
        else
        {
            pr = pr->next;
        }
    }
    if (flag == 1)
    {
        printf("回收成功!\n");
    }
    else
    {
        printf("作业不存在!\n");
    }
}

int main()
{
    char ch;
    int n = 0;
    void clrscr(); /*清屏*/
    create();
    while (n != 3)
    {
        //循环的执行操作，直到操作为退出
        printf("  \n    1.添加作业            2.回收分区          3.退出quit\n");
        printf("\n------------------------------------------------------------------------------\n");
        printf("\n请选择操作:");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
        {
            input();
            assign();
            disp();
            ch = getch();
        };
        break;
        case 2:
        {
            destroy();
            disp();
            ch = getch();
        };
        break;
        case 3:
            break;
        }
    }
    ch = getch();
}
