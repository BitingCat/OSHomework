#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct pcb{
    char name[10];     //锟斤拷锟斤拷锟斤�?
    int reach_time;  //锟斤拷锟教�?�拷锟斤拷时锟斤�?
    int need_time;      //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷时锟斤拷
    struct pcb * link;   //锟斤拷锟斤拷锟斤拷一锟斤拷pcb锟斤拷指锟斤�?
};

int times = 0;     //锟斤拷前时锟斤拷
int num = 0;       //锟斤拷锟斤拷慕锟斤拷锟斤拷锟�?
typedef struct pcb PCB;    //锟斤拷锟斤拷峁癸拷锟斤拷锟斤�?

struct pcb *p = NULL, *ready = NULL;


//锟斤拷锟�?函锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤�?
void input(){
    int i;
    struct pcb * q;    //锟斤拷锟斤拷峁癸拷锟斤拷锟斤�?
    p = (struct pcb*)malloc(sizeof(struct pcb));
    printf("锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�?:");
    scanf("%d", &num);
    for(i=1; i<=num; i++){
        p = (struct pcb*)malloc(sizeof(struct pcb));
        printf("\n锟斤�?%d锟斤拷锟斤拷锟斤�?:", i);
        printf("\n\n锟斤拷锟斤拷锟斤拷锟斤拷锟�:");
        scanf("%s", p->name);
        printf("\n锟斤拷锟斤拷锟斤拷锟斤拷痰锟斤拷锟绞憋拷锟�:");
        scanf("%d", &p->reach_time);
        printf("\n锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟�:");
        scanf("%d", &p->need_time);

        p->link = NULL;
        if(NULL == ready){       //锟斤拷锟斤拷锟斤拷一锟斤拷锟结构锟藉，使指锟斤拷p,q指锟斤拷锟斤�?
            ready = p;
            q = ready;
        }
        else{
            q->link = p;
            q = p;
        }
    }
}

//锟斤拷锟斤拷锟�?�帮拷锟斤拷�?锟斤拷锟�?
void output(struct pcb * p, int times){
    if(NULL == p){
        printf("锟斤拷前时锟斤拷:%d, 锟斤拷锟�?斤拷锟斤拷锟斤拷锟斤拷锟斤拷!\n", times);
    }
    else{
        printf("锟斤拷锟斤拷锟斤�?:%s,锟斤拷锟斤拷时锟斤拷:%d,锟斤拷锟斤拷锟斤拷�?�时锟斤�?:%d\n",p->name,p->reach_time,p->need_time);
    }
}


struct pcb * SJF(int times, int * after){
    int min_time = 0;   //锟斤拷锟绞憋拷洌拷锟斤拷锟斤拷锟斤拷锟斤拷械慕锟斤拷痰锟绞憋拷锟�
    struct pcb * now_progress = NULL, *p = ready;
    if (NULL != ready){
        while(NULL != p){
            if(times >= p->reach_time){          //锟斤拷锟斤拷锟斤拷丫锟斤拷锟斤拷锟�
                if(0 == min_time){          //锟斤拷锟斤拷锟绞憋拷涓�?�拷锟�?��
                    now_progress = p;
                    min_time = p->need_time;
                }
                else{
                    if(p->need_time < min_time){
                        now_progress = p;
                        min_time = p->need_time;
                    }
                }
            }
            p = p->link;
        }
    }
    *after = min_time + times;
    return now_progress;   //锟斤拷锟斤拷指锟斤拷锟斤拷锟斤拷锟斤拷锟�?斤拷锟教碉拷指锟斤拷
}

//锟斤拷锟窖撅拷锟斤拷锟斤拷锟斤拷傻慕锟斤拷锟斤拷锟接碉拷finish锟斤拷锟�?ｏ拷锟斤拷锟�?斤拷锟斤拷锟斤拷锟斤拷一
void destory(struct pcb * p, int times){
    struct pcb * q = ready;

    if(strcmp(p->name, ready->name) == 0){  //锟斤拷锟�?伙拷锟斤拷锟斤拷锟斤拷锟斤拷
        ready = ready->link;
    }
    //锟斤拷锟�?硷拷锟斤拷锟斤拷一锟斤拷锟斤拷锟斤拷锟斤拷锟�
    else{
        q = ready;
        while((strcmp(q->link->name,p->name) != 0) && NULL != q->link){
            q = q->link;
        }
        q->link = p->link;
    }

    num--;   //锟斤拷锟斤拷锟斤拷锟斤拷一
    printf("\n锟斤拷锟斤拷%s锟窖撅拷锟斤拷锟絓n");
    getchar();
}

int main(int argc, char *argv[]) {

    input();  //锟斤拷锟斤拷锟斤拷锟斤拷
    int after = 0;        //执锟斤拷锟斤拷一锟斤拷锟斤拷锟教猴拷锟绞憋拷锟�
    struct pcb *now_progress = NULL;    //now_progress指锟斤拷锟斤拷锟斤拷锟斤拷锟�?的斤拷锟斤拷

    while(num > 0){    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤�?0,每锟斤拷�?锟斤拷num锟斤拷锟�?�?
        now_progress = SJF(times, &after);  //锟斤拷锟斤拷SJF锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤�?
        if(NULL != now_progress){
            for(;times < after; times++){
                printf("\n锟斤拷前时锟斤拷:%d", times);
                printf("\n-----------锟斤拷前执锟�?斤拷锟斤�?------------\n");
                output(now_progress, times);     //锟斤拷锟斤拷output锟斤拷锟斤拷
                getchar();
            }
            //锟斤拷锟斤拷执锟斤拷锟斤拷锟斤拷锟斤拷destory锟斤拷锟斤拷
            destory(now_progress, times);

        }
        else{   //没锟�?斤拷锟斤拷锟斤拷锟斤拷锟斤拷
            output(now_progress, times);
            times++;
        }
    }
    return 0;
}
