#include<stdio.h>

#define false 0
#define true 1
int sourse = 3;
int process = 5;
int all_process[3] = {10, 5, 7};
int max_process[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
int have_process[5][3] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
int need_process[5][3];
int available_process[3];
int request_process[3];

getNeed() {
	for(int i = 0; i < process; i++) {
		for(int j = 0; j < sourse; j++) {
			need_process[i][j] = max_process[i][j] - have_process[i][j];
		}
	}
}

getAvailable() {
	int temp = 0;
	for(int i = 0; i < sourse; i++) {
		temp = 0;
		for(int j = 0; j < process; j++) {
			temp += have_process[j][i];
		}
		available_process[i] = all_process[i] - temp;
	}
}

showProcess(int a[5][3]) {

	printf("	资源0	资源1	资源2\n");
	for(int i = 0; i < process; i++) {
		printf("进程p%d:", i);
		for(int j = 0; j < sourse; j++) {	
			printf("\t %d ", a[i][j]);
		}
		printf("\n");
	}
	
}

showAllProcess(int a[3]) {
	printf("[");
	for(int i = 0; i < 3; i++) {
		printf(" %d ", a[i]);
	}
	printf("]\n");
}

distribute(int k) {
	for(int i = 0; i < sourse; i++) {
		available_process[i] = available_process[i] - request_process[i];
		have_process[k][i] = have_process[k][i] + request_process[i];
		need_process[k][i] = need_process[k][i] - request_process[i];
	}
}


back(int k) {
	for(int i = 0; i < sourse; i++) {
		available_process[i] = available_process[i] + available_process[i];
		have_process[k][i] = have_process[k][i] - request_process[i];
		need_process[k][i] = need_process[k][i] + request_process[i];
	}
}

int isExist() {

		
	int temp = 0;
	int flag = 1;
	
	int index[5] = {0, 0, 0, 0, 0};
	int k = 5;
	
	
	while(k > 0) {
		
		temp = 0;
		
		for(int i = 0; i < 5; i++) {
			
			if(index[i] == 1) {
				continue;
			}
			
			flag = 1;

			for(int j = 0; j < 3; j++) {
				if(available_process[j] >= need_process[i][j]) {
					continue;
				} else {
					flag = -1;
					break;
				}
			}
			
			if(flag == 1) {
				for(int k = 0; k < 3; k++) {
					available_process[k] += have_process[i][k];	
				}
				index[i] = 1;
			}
		}
		
		
		k--;
	}

	
	
	for(int i = 0; i < 5; i++) {
		if(index[i] == 1) {
			
			continue;
		}else {
			printf("不存在安全序列\n");
			return -1;
		}
		
	}
	printf("存在安全序列\n");
	return 1;

}
int bank(int num_process) {
	int index = 1;
	int result;
	char input;
	
	int available_process_test[3];
	
	for(int i = 0; i < 3; i++) {
		if(available_process[i] < request_process[i]) {
			index = -1;
			break;
		} else if(request_process[i] > need_process[num_process][i]) {
			index = -2;
			break;
		}
	} 
	if(index == -1) {
		printf("要求资源大于可用资源。\n");
	} else if(index == -2) {
		printf("要求资源大于进程所需资源。\n");
	}else {
		
		distribute(num_process);
				
		result = isExist();
		
		if(result == 1) {
			printf("分配资源成功！\n");
		} else if(result == -1) {
		
		}				
	}
	
}

int main() {
	int num_process;
	int result;
	
	while(true) {
		getNeed();
		getAvailable();
		
		printf("总资源数：");
		showAllProcess(all_process);
		
		printf("目前可用资源：\n");
		showAllProcess(available_process);
		
		printf("\n进程得到的资源数：\n");
		showProcess(have_process);
		
		printf("\n需要的资源数：\n");
		showProcess(need_process);
		
		printf("\n请输入需要申请的进程号：p");
		
		scanf("%d", &num_process);
		printf("\n资源0：");
		scanf("%d", &request_process[0]);
		printf("\n资源1：");
		scanf("%d", &request_process[1]);
		printf("\n资源2：");
		scanf("%d", &request_process[2]);
		
		result = bank(num_process);
	}
}

