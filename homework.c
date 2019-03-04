#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

struct pcb {
    char name[10];   //进程名字 
    int priorNumber; //进程优先数 
    int needTime;	 //要求服务的时间 
    int buildTime;   //进程建立的时间 
    int useTime;     //进程已执行时间 
    char state;      //进程的状态 W就绪  R运行  F完成 
    struct pcb* link; 
}*p;
typedef struct pcb PCB;

/*
 *the time of cpu do the homework 
*/
int time = 0;

/**
 * create a random number
 */
int getRandomNumber() {
    return rand() % 10 + 1;
}


int space() {

}

sort(PCB * insert) {
    int insertNeedTime = insert->needTime; 
    int insertTemp = 0;
    PCB *head;
    head = (PCB *) malloc (sizeof(PCB));
    head = p;

    if(head->link == NULL) {
        insertNeedTime = 0;
    } else {
        while (head != NULL) {
            if(head->link == NULL) {
                break;
            }else if(head->link->needTime >= insertNeedTime) {
                insert->link = head->link;
                head->link = insert;
                insertTemp = 1;
                break;
            }
            head = head->link;
        }
    }

    if(insertTemp == 0) {
        while(head != NULL) {
            if(head->link == NULL) {
                head->link = insert;
                insert->link = NULL;
            }
            head = head->link;
        }
    }
}

input(char name[]) {
    
    PCB *first = (PCB *) malloc (sizeof(PCB));
    strcpy(first->name, name);
    first->needTime = getRandomNumber();
    first->buildTime = 0;
    first->useTime = 0;
    first->state = 'W';
    first->link = NULL;

    sort(first);
}

initP() {
    p = (PCB *) malloc (sizeof(PCB));   
    strcpy(p->name, "head");
    p->needTime = -1;
    p->useTime = -1;
    p->buildTime = -1;
    p->state = 'W';
    p->link = NULL;
}

initBuild(int index, char inputName[]) {
    // printf("%d", index);
    PCB *buildInsert;
    buildInsert = (PCB *) malloc (sizeof(PCB));
    strcpy(buildInsert->name, inputName);
    buildInsert->needTime = getRandomNumber();
    buildInsert->buildTime = index;
    buildInsert->useTime = 0;
    buildInsert->link = NULL;
    buildInsert->state = 'W';
    sort(buildInsert);
}


show(PCB* head) {
    printf("-----------------------------------------------\n");
    printf("    name    needTime  buildTime  useTime  state\n");
    while(head->link != NULL) {
        printf("  %s\t", head->link->name);
        printf("  %d\t", head->link->needTime);
        printf("  %d\t", head->link->buildTime);
        printf("  %d\t", head->link->useTime);
        printf("  %c", head->link->state);
        printf("\n");
        head = head->link;
    }
    printf("------------------------------------------------\n\n");
}

check(PCB *a) {
    printf("time:%d  now the homework is: %s\n", time, a->name);
}

start() {

    PCB *head, *change, *checkPcb;
    head = (PCB *) malloc (sizeof(PCB));
    change = (PCB *)malloc (sizeof(PCB));

    //in order to printf the name when not exist homework doing
    checkPcb = (PCB *)malloc (sizeof(PCB));   
    strcpy(checkPcb->name, "null");

    head = p;
    while(1) {

        if(head->link == NULL) {
            printf("End\n");
            break;
        } else {

            if(head->link->buildTime > time && head->link->link != NULL) {

                change = head->link->link;
                head->link->link = change->link;
                change->link = head->link;
                head->link = change;
                /**
                 * lian xu liang ge wei dao da de 
                */

            } else {
                if(head->link->needTime > 0) {
                    if(head->link->buildTime > time) {

                        time++;
                        check(checkPcb);  
                    } else {
                        time++;
                        check(head->link);
                        head->link->needTime--;
                        head->link->useTime++;
                        if(head->link->needTime == 0) head->link->state = 'F';
                        else if(head->link->state == 'W') head->link->state = 'R';
                    }
                   
                    show(p);
                } else {
                    head = head->link;
                }
            }


            
        }
    }
}

main() {
    //-------------
    int index; char inputName[11];
    char name1[11] = {'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', '_', '1', '\0'};
    char name2[11] = {'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', '_', '2', '\0'};
    char name3[11] = {'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', '_', '3', '\0'};
    char name4[11] = {'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', '_', '4', '\0'};
    char name5[11] = {'h', 'o', 'm', 'e', 'w', 'o', 'r', 'k', '_', '5', '\0'};

    initP();
    input(name1); input(name2); input(name3); input(name4); input(name5);
    show(p);


    printf("if you want to stop input, please enter '-1'\n");
    

    while (1) {

        printf("please enter the homework's buildTime: "); 
        scanf("%d", &index);   
        printf("please enter the homework's name : ");
        scanf("%s", inputName);
        if(index == -1) {
            printf("hei\n");
            start();
            break;
        } else if(index >= 0 && index <= 100){
            initBuild(index, inputName);
        } else {
            printf("invalid input!\n");
        }
    }

    //-------------
    scanf("%s", inputName);
}
