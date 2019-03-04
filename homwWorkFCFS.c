#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

struct pcb {
    char name[10];	//作业名字 
    int needTime;	//要求服务的时间 
    int arriveTime; //作业到达时间 
    int useTime;    //周转时间 
    char state;     //作业的状态 
    struct pcb* link; 
}*p;
typedef struct pcb PCB;
int time = 0;

int getRandomNumber() {
    return rand() % 10 + 1;
}

sort(PCB * insert) {

    int insertArriveTime = insert->arriveTime;
    int insertTemp = 0;
    PCB *head = (PCB*) malloc (sizeof(PCB));
    head = p;

    if(head -> link == NULL) {
        insertTemp = 0;
    } else {
        while (head != NULL) {
            if(head->link == NULL) {
                break;
            } else if (head->link->arriveTime >= insertArriveTime) {
                insert -> link = head -> link;
                head->link = insert;
                insertTemp = 1;
                break;
            }
            head = head -> link;
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


initP() {
    p = (PCB *) malloc (sizeof(PCB));   
    strcpy(p->name, "head");
    p->needTime = -1;
    p->useTime = -1;
    p->arriveTime = -1;
    p->state = 'W';
    p->link = NULL;
}

input(char name[]) {
    
    PCB *first = (PCB *) malloc (sizeof(PCB));
    strcpy(first->name, name);
    first->needTime = getRandomNumber();
    first->arriveTime = getRandomNumber();
    first->useTime = 0;
    first->state = 'W';
    first->link = NULL;

    sort(first);
}


int space() {

}


show(PCB* head) {
    printf("-----------------------------------------------\n");
    printf("    name    needTime  arriveTime  useTime  state\n");
    while(head->link != NULL) {
        printf("  %s\t", head->link->name);
        printf("  %d\t", head->link->needTime);
        printf("  %d\t", head->link->arriveTime);
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
    PCB *head, *checkPcb;
    head = (PCB *) malloc (sizeof(PCB));
    head = p;

    //in order to printf the name when not exist homework doing
    checkPcb = (PCB *)malloc (sizeof(PCB));   
    strcpy(checkPcb->name, "null");

//     PCB *head, *change, *checkPcb;
//     head = (PCB *) malloc (sizeof(PCB));
//     change = (PCB *)malloc (sizeof(PCB));

    while(1) {
        time++;
        if(head->link == NULL) {
            printf("End\n");
            break;
        } else {
            checkPcb = head->link;
            if(checkPcb->needTime > 0) {
                if(checkPcb->arriveTime <= time) {
                
                    head->link->needTime--;
                    head->link->useTime++;
                    if(head->link->needTime == 0) head->link->state = 'F';
                    else if(head->link->state == 'W') head->link->state = 'R';
                    check(checkPcb);
                    show(head);
                } 
            } else {
                head = head -> link;
            }  
        }
    }

//     while(1) {

//         if(head->link == NULL) {
//             printf("End\n");
//             break;
//         } else {

//             if(head->link->arriveTime > time && head->link->link != NULL) {

//                 change = head->link->link;
//                 head->link->link = change->link;
//                 change->link = head->link;
//                 head->link = change;
//                 /**
//                  * lian xu liang ge wei dao da de 
//                 */

//             } else {
//                 if(head->link->needTime > 0) {
//                     if(head->link->arriveTime > time) {

//                         time++;
//                         check(checkPcb);  
//                     } else {
//                         time++;
//                         check(head->link);
//                         head->link->needTime--;
//                         head->link->useTime++;
//                         if(head->link->needTime == 0) head->link->state = 'F';
//                         else if(head->link->state == 'W') head->link->state = 'R';
//                     }
                   
//                     show(p);
//                 } else {
//                     head = head->link;
//                 }
//             }


            
//         }
//     }
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


    start();

    //-------------
    printf("");
    getchar();
}
