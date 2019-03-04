/*��̬��������-�����Ӧ�㷨*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <math.h>
#define getpch(type)(type*)malloc(sizeof(type))
#define Size 4

//����һ�����з�������
struct item
{
    int  size;            /* �����С */
    int  startaddress;    /* ��ʼ��ַ */
    int  state;           /* ״̬,0����δ������,1�����ѷ��� */
    char name[10];        /* ������ҵ */
    int used;             /* �ѷ���ռ� */
    int need;             /* ��ҵ��С */
    struct item *prior;   /* ǰָ�� */
    struct item *next;    /* ��ָ�� */
}*p;

typedef struct item ITEM;

int address = 0;
char aname[10];  //��ҵ��
int aneed;       //��ҵ��С
char bname[10];

ITEM *head = NULL, *tail = NULL;   //������ͷβ���

//---------------------------disp()------------------------------------------

void disp()
{
    //��ӡ������˵����
    int len = 1;
    ITEM *pr;
    pr = head;
    printf("                    ******************��̬��������***************\n");
    printf("                    *                                           *\n");
    printf("                    *                 �����Ӧ�㷨              *\n");
    printf("                    *                                           *\n");
    printf("                    *********************************************\n");
    printf("��ǰ����������������:\n");
    printf("******************************************************************************\n");
    printf("* ������ *  ��ַ(K) *  ��С(K) *  ״̬  *  ������ҵ *  ��ҵ��С * �ѷ���ռ� *\n");
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
        strcpy(p->name, "��");
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

//�����ҵ
void input()
{
    printf("��������ҵ��:");
    scanf("%s", aname);
    printf("��������ҵ��С:");
    scanf("%d", &aneed);
    printf("\n");
}

//Ϊ����ӵ���ҵ����洢�ռ䣬�����Ӧ
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
                p = getpch(ITEM); //�����µĽ��
                p->size = aneed;
                p->startaddress = best_position->startaddress;
                p->state = 1;
                strcpy(p->name, aname);
                p->need = aneed;
                p->used = aneed;
                p->next = NULL;
                p->prior = NULL;

                //�������ɵĽڵ����best_positionǰ��
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
                //ʣ�µĿռ�
                best_position->startaddress = best_position->startaddress + aneed;
                best_position->size = best_position->size - aneed;
                flag = 1;
            }
        }
        if (flag == 1)
        {
            printf("����ɹ�!\n");
        }
        else
        {
            printf("�ڴ治��,����ʧ��!\n");
        }
    }
}

//������ҵ�����Ҫ���պ���ҵ���ڷ���˵�������У��Ͱѷ�������
void destroy()
{
    int i = 0, flag = 0;
    ITEM *pr, *sec;
    pr = head;
    printf("������Ҫ���յ���ҵ��:");
    scanf("%s", bname);
    while (flag != 1 && pr != NULL)
    {
        if ((strcmp(pr->name, bname) == 0) && pr->state != 0)
        {
            //�������ҵ�����ҵ�Ҫ���յĽ�㣬����������ղ���
            if (pr == head)
            {
                //��������ǵ�һ��
                pr->state = 0;
                strcpy(pr->name, "��");
                pr->need = 0;
                pr->used = 0;
                flag = 1;
            }
            else
            {
                if ((pr->startaddress == pr->prior->startaddress + pr->size) && pr->prior->state == 0)
                {
                    //���տռ���ǰһ�����з�������
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
                        //���տռ����һ�����з�������
                        sec = pr->next;
                        pr->size = pr->size + pr->next->size;
                        pr->state = 0;
                        strcpy(pr->name, "��");
                        pr->need = 0;
                        pr->used = 0;
                        sec->prior->next = sec->next;
                        sec->next->prior = sec->prior;
                        free(sec);
                        flag = 1;
                    }
                    else
                    {
                        //���տռ���ǰ����з�����������
                        pr->state = 0;
                        strcpy(pr->name, "��");
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
        printf("���ճɹ�!\n");
    }
    else
    {
        printf("��ҵ������!\n");
    }
}

int main()
{
    char ch;
    int n = 0;
    void clrscr(); /*����*/
    create();
    while (n != 3)
    {
        //ѭ����ִ�в�����ֱ������Ϊ�˳�
        printf("  \n    1.�����ҵ            2.���շ���          3.�˳�quit\n");
        printf("\n------------------------------------------------------------------------------\n");
        printf("\n��ѡ�����:");
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
