#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h> 

#define false 0
#define true 1
#define commandNum 15
#define bitMapSize 10
#define RED 0
#define GREEN 1
#define BLUE 2
#define WHITE 3

//�û� 
typedef struct u {
	char *userName;											//�û����� 
	int userType;											//�û����� 
}User;

//��ǰ״̬ 
typedef struct cs {
	User *currentUser;
	char *currentPath;
	char currentDir[60];
}CurrentStatus;


//�ļ�Ŀ¼���ṹ 
typedef struct dirEntry {
	char fileName[60];
	int type;
	int fileAddr;
	int fileSize;
	char content[100];
	char creator[60];
	int state;
	struct dirEntry *firstChild, *nextSibling; 
}DirEntry;

User *user;
CurrentStatus *currentStatus;
DirEntry *dirEntry;
int Fat[bitMapSize*bitMapSize] = {0};						//fat�� 	-1��ʾ�ļ����� 
int	BitMap[bitMapSize][bitMapSize] = {0};					//λʾͼ 	0��ʾ���У�1��ʾ���� 



int isLogin = -1; 											//�û��Ƿ��¼��־ 
char path[60] = "~";										//�û����ʵ�·�� 
char dir[60];

HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

char allCommand[commandNum][10] = {"login", "help", "h", "ls", "create", "open", "read", "write", "close", "delete", "mkdir", "cd", "dir", "logout", "check"};
char allCommandMean[commandNum][20] = {"�û���¼", "��ѯ����", "��ѯ����", "��ѯ�ļ�Ŀ¼", "�����ļ�", "���ļ�", "�ļ��Ķ�", "�ļ���д", "�ر��ļ�",
											 "ɾ���ļ�", "����Ŀ¼", "�ı䵱ǰĿ¼", "��ѯ�ļ�Ŀ¼", "�˳���¼", "�鿴λʾͼ"};
															//���趨�õ����� 

char allUsername[4][20] = {"wen", "liu", "fu", "cai"};
char allPassword[4][20] = {"123", "123", "123", "123"};


/*
 *�ı䵱ǰ������ɫ
 */
changeColor(int color) {	
	switch(color) {
		case 0 : SetConsoleTextAttribute(hdl, FOREGROUND_RED); break;
		case 1 : SetConsoleTextAttribute(hdl, FOREGROUND_GREEN); break;
		case 2 : SetConsoleTextAttribute(hdl, FOREGROUND_BLUE); break;
		case 3 : SetConsoleTextAttribute(hdl, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
		default: ; break;
	}
}

welcomeFirst () {
	changeColor(GREEN);
	printf("\n");
	printf("                    *******************************************\n");
    printf("                    *                                         *\n");
    printf("                    *                 Welcome                 *\n");
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
}

welcomeUser () {
	changeColor(GREEN);
	printf("\n");
	printf("                    *******************************************\n");
    printf("                    *                                         *\n");
    printf("                    *       ��ӭ����%6s  ���ļ�ϵͳ        *\n", user->userName);
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
}



/*
 *����������� 
 */
int preOrderTraverseForest(DirEntry* D) {
	if(NULL == D) return 1;	
	if(D->type == 0) {
		printf("%s	", D->fileName);
	} else {		
		changeColor(RED);
		printf("%s	", D->fileName);
		changeColor(GREEN);
	}	
	if(-1 == preOrderTraverseForest(D->firstChild)) return -1;
	return preOrderTraverseForest(D->nextSibling);
}


/*
 *����Ƿ����û����� 
 */
 int checkUserLogined() {
 	if(isLogin == 1) return 1;
 	else return -1;
 }

/*
 *��ӡ��������ʾ 
 */
printHead() {
	changeColor(WHITE);
	if(checkUserLogined() == -1) {
		printf("\n[root@localhost]%s: $", path);					//�û�δ��¼ 
	} else {
		printf("\n[%s@localhost]%s: $", user->userName, path);	//�û��ѵ�¼ 
	}
	changeColor(GREEN);	
}

/*
 *��������Ƿ���Ч 
 */
int checkCommand(char* command) {
	if(strcmp(command, allCommand[0]) == 0 || strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0 
				|| strcmp(command, allCommand[3]) == 0 || strcmp(command, allCommand[4]) == 0 || strcmp(command, allCommand[5]) == 0 
				|| strcmp(command, allCommand[6]) == 0 || strcmp(command, allCommand[7]) == 0 || strcmp(command, allCommand[8]) == 0 
				|| strcmp(command, allCommand[9]) == 0 || strcmp(command, allCommand[10]) == 0 || strcmp(command, allCommand[11]) == 0
				|| strcmp(command, allCommand[12]) == 0 || strcmp(command, allCommand[13]) == 0 || strcmp(command, allCommand[14]) == 0){
		return 1;												//����1����������Ч�� 
	} else return -1;   										//����-1��������Ч 
}

/*
 *����¼ 
*/
int checkLogin(char* username, char* password) {
	if((strcmp(username, allUsername[0]) == 0 && strcmp(password, allPassword[0]) == 0) ||  (strcmp(username, allUsername[1]) == 0 && strcmp(password, allPassword[1]) == 0)
			|| (strcmp(username, allUsername[2]) == 0 && strcmp(password, allPassword[2]) == 0) || (strcmp(username, allUsername[3]) == 0 && strcmp(password, allPassword[3]) == 0) ) {
					
		user->userName = username;
		isLogin = 1;
				
		strcat(path, "/");
		strcat(path, user->userName);	

		currentStatus->currentUser = user;
		currentStatus->currentPath = path;
		strcpy(currentStatus->currentDir, username);			//��ǰĿ¼ 
		printf("login successful");								//��¼�ɹ� 
		welcomeUser();
	} else {
		printf("username or password is wrong");				//��¼ʧ�� 
	}
}

commandLogout() {
	free(user);
	user = (User *)malloc(sizeof(User));				//��ʼ���û� 
	strcpy(path, "~");
	isLogin = -1;
}

/*
 *��ӡ���е����� 
 */ 
commandHelp() {
	int i = 0; 
	for(i = 0; i < commandNum; i++) {
		printf("%s	", allCommand[i]);
		printf("%s\n", allCommandMean[i]);
	}
}

/*
 *ͨ��/��������ҳ���ǰ���ڵ�Ŀ¼ 
 */ 
char *getCurrentDir(char inputPath[]) {
	char *path1;
	char *path2;
	path1 = strtok(inputPath, "/");
	while(true) {
		path1 = strtok(NULL, "/");
		if(path1 == NULL) break;
		path2 = path1;
	}
	return path2;
}

/*
 *��Ŀ¼�ṹ�����ҵ���ǰĿ¼��� 
 */ 
DirEntry* search(DirEntry* D, char currentDir[], int type) {
	DirEntry* result = NULL;
	if(NULL == D) return NULL;
	if(strcmp(D->fileName,currentDir) == 0 && D->type == type) {
		return D;
	}
	if((result = search(D->firstChild, currentDir, 0)) != NULL) 
		return result;
	return search(D->nextSibling, currentDir, 0);
}

/*
 *��ָ��Ŀ¼���ҳ�Ŀ��Ŀ¼���ļ� 
 */
DirEntry* searchCurrentDir(DirEntry* D, char inputStr[], int type) {
	DirEntry* result = (DirEntry *)malloc(sizeof(DirEntry));
	
	
	if(type == 3) {
			
		if(NULL == D) return NULL;
		if(strcmp(D->firstChild->fileName, inputStr) == 0) return D->firstChild;
		else {
			result = D->firstChild;
			while (true) {
				if(NULL == result->nextSibling) {
					break;
				}
				if(strcmp(result->nextSibling->fileName, inputStr) == 0) {
					return result->nextSibling;
				}
				result = result->nextSibling;
			}
		}
			
	} else {
		if(NULL == D) return NULL;
		if(strcmp(D->firstChild->fileName, inputStr) == 0 && D->firstChild->type == type) return D->firstChild;
		else {
			result = D->firstChild;
			while (true) {
				if(NULL == result->nextSibling) {
					break;
				}
				if(strcmp(result->nextSibling->fileName, inputStr) == 0 && result->nextSibling->type == type) {
					return result->nextSibling;
				}
				result = result->nextSibling;
			}
		}
	}
	return NULL;
}

/*
 *�г���ǰĿ¼�������� 
 */ 
commandLsFile(DirEntry* D) {
	DirEntry* next = NULL;
//	preOrderTraverseForest(D);
	
	if(NULL == D->firstChild) {
		printf("\n");
	} else {
		next = D->firstChild;
		while(true) {
			
			if(next->type == 0) {
				printf("%s	", next->fileName);
			} else {
				changeColor(RED);
				printf("%s	", next->fileName);
				changeColor(GREEN);
			}
			
			if(NULL == next->nextSibling) break;
			next = next->nextSibling;
		}
	}
}

commandLs(char currentDir[]) {
	DirEntry *p = NULL;	
	
	p = search(dirEntry, currentDir, 0);
	
	if(NULL == p) printf("path is wrong");
	else if(p->type != 0) printf("%s not a directory", p->fileName);
	else {
		commandLsFile(p);
	}
} 


int createCheckBitMap(int fileSize) {
	int i, j, count = 0;
	for(i = 0; i < bitMapSize; i++) {			//����Ƿ����㹻�Ŀ����̿�
		for(j = 0; j < bitMapSize; j++) {
			if(BitMap[i][j] == 0){
				BitMap[i][j] = -1;
				count++;						//�ҵ�һ�������̿飬count++ 
				if(count == fileSize) break;	//�ҵ��㹻�Ŀռ�ʱ������ѭ�� 
			} 
		}
		if(count == fileSize) break;
	}
	 
	if(count < fileSize) {						//û���㹻�ռ�ʱ���������ǵ�-1�Ļ�0 
		for(i = 0; i < bitMapSize; i++) {
			for(j = 0; j < bitMapSize; j++) {
				if(BitMap[i][j] == -1) BitMap[i][j] = 0;
			}
		}	
		return -1;
	}
	else return 1;
}

int createChangeFat(int fileSize) {
	int i, j, index, firstBit, count = 0;
	for(i = 0; i < bitMapSize; i++) {				//�ҳ�Ѱ�ҿռ�ʱ��ǵ�-1 
		for(j = 0; j < bitMapSize; j++) {
			if(BitMap[i][j] == -1) {
				BitMap[i][j] = 1;
				count++;
				if(count == 1) {
					firstBit = i * bitMapSize + j;
					index = i * bitMapSize + j;
				}
				else {								//��fat���У����̿��������һ���̿� 
					Fat[index] = i * bitMapSize + j;
					index = i * bitMapSize + j;
				}
				if(count == fileSize) {				//���ҵ����һ����ǵ��̿�ʱ����fat��������-1����ʾ�ļ��Ľ����� 
					Fat[i * bitMapSize + j] = -1;
					break;
				}
			}
		}
	}
	return firstBit;
}

/*
 *�ڵ�ǰĿ¼�����ļ���� 
 */ 
int createCreateNode(char* fileName, int fileSize, int firstBit, int type) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *t = (DirEntry *)malloc(sizeof(DirEntry));
	
	p = search(dirEntry, currentStatus->currentDir, 0);
	if(NULL == p) printf("path is wrong");
	else if(p->type != 0) printf("%s not a directory", p->fileName);
	else {
		strcpy(q->fileName, fileName);
		q->fileSize = fileSize;
		q->fileAddr = firstBit;
		q->type = type;
		q->firstChild = NULL;
		q->nextSibling = NULL;
		
		//�����ǰĿ¼û���κ����ֱ�Ӻ���ָ��q 
		if(NULL == p->firstChild) {
			p->firstChild = q;
			
		} else {
			//�����ǰĿ¼���ӽڵ���ֵܽ��Ϊ�գ����ӽڵ���ֵܽ��ָ��q 
			if(NULL == p->firstChild->nextSibling) {
				p->firstChild->nextSibling = q;
				
			} else {
				//�����ҵ��ӽڵ������ֵܽ�㣬ָ��q 
				t = p->firstChild;
				while(true) {
					if(NULL == t->nextSibling) {
						t->nextSibling = q;
						break;
					}
					t = t->nextSibling;
				}
			}
		}
		
	}
	printf("create successful");
//	preOrderTraverseForest(dirEntry); 
}

/*
 *�ж�������ļ���С�Ƿ�Ϊ���� 
 */ 
int createCheckFileSize(int fileSize) {
	
}

/*
 *�ж��ļ��Ƿ�������� 
 *�������ķֱ����ļ���Ŀ¼ʱ���ɴ��� 
 */
int createCheckRepeatName(char* fileName, int type) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	
	p = search(dirEntry, currentStatus->currentDir, 0);
	
	if(NULL == p) printf("path is wrong");
	else if(p->type != 0) printf("%s not a directory", p->fileName);
	else {
		if(NULL == p->firstChild) return 1; 								//���ӽ��Ϊ�գ��ɴ��� 
		else if(strcmp(p->firstChild->fileName, fileName) == 0 && p->firstChild->type == type) return 0;	//�뺢�ӽ�����������ɴ��� 
		else {
			if(NULL == p->firstChild->nextSibling) return 1;				//���ӽ����ֵܽ��Ϊ�գ��ɴ��� 
			else {
				q = p->firstChild;
				while(true) {
					if(NULL == q->nextSibling) {
						return 1;											//�������ֵܽ������������ɴ��� 
					}
					
					if(strcmp(q->nextSibling->fileName, fileName) == 0 && q->nextSibling->type == type) {
						return 0;											//��ĳһ�ֵܽ�����������ɴ��� 
					} else {
						q = q->nextSibling;
					}
					
				}
				return 1;
			}
		} 
	}
}

/*
 *����û�������ļ������ļ���С 
 */
int createCheckInput(char* fileName, int fileSize) {
	if(NULL == fileName) {
		printf("fileName can't null'");
		return -1;
	} 
}

/*
 *�����ļ� 
 */
commandCreate(char * secondStr) {
	int fileSize;
	int tempCheckBitMap;
	int tempFirstBit;
	int tempCheckInput;
	int tempCheckRepeatName;
	printf("fileSize:");
	scanf("%d", &fileSize);
	
	//��һ����������������Ƿ�Ƿ�
//	tempCheckInput = createCheckInput(fileName, fileSize);
	//�ڶ���������ļ�����
	tempCheckRepeatName = createCheckRepeatName(secondStr, 1);
	if(tempCheckRepeatName == 1) {
		//�����������λʾͼ�����ҿ����̿�
		tempCheckBitMap = createCheckBitMap(fileSize);
		if(tempCheckBitMap == -1) {
			printf("not enough space");
		} else {
			//���Ĳ����������̿�����ӵ�fat��
			tempFirstBit = createChangeFat(fileSize);
			//���岽����Ŀ¼���д����ļ���� 
			createCreateNode(secondStr, fileSize, tempFirstBit, 1);
		}
	}
	else printf("filename can't repeat");
}

/*
 *����Ŀ¼ 
 */
commandMkdir(char * secondStr) {
	int tempCheckRepeatName;
	
	//��һ�����������	
	if(createCheckRepeatName(secondStr, 0) == 1) {			//�ڶ������������
		createCreateNode(secondStr, -1, -1, 0);				//�������������ļ���� 
	} else printf("fileName can't repeat");
}


/*
 *
 */
int checkRead(DirEntry * D) {
	int i;
	if(strcmp(D->fileName, "~") == 0) return 1;
	for(i = 0; i < 4; i++) {
		if(strcmp(allUsername[i], user->userName) == 0) {
			continue;
		} else {
			if(strcmp(D->fileName, allUsername[i]) == 0) {
				return 1;
			}
		}
	}
	return 0;
}

/*
 *��ȡ·�������һ��Ŀ¼ 
 */
getLastPath(char* newDir) {
	char *path1;
	char *path2;
	path1 = strtok(newDir, "/");
	while(true) {
		path1 = strtok(NULL, "/");
		if(path1 == NULL) break;
		path2 = path1;
	}
	newDir = path2;
}


/*
 *�ص�·�������һ��Ŀ¼ 
 */ 
getNewPath(char* newPath) {
	char path1[60];	
	int i, count = 0;
	for(i = 0; i < strlen(newPath); i++) {
		path1[i] = newPath[i];
	}
	for(i = strlen(path1); i >= 0; i--) {
		if(count == 1) newPath[i] = path1[i];
		else newPath[i] = '\0';
		if(path1[i] == '/' && count < 1) count++;
	}
	if(count == 0) strcpy(newPath, "~");
}


/*
 *�����û������Ŀ¼ 
 */
int cdFindDir(char* inputPath) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	
	char newPath[60] = "~";
	char newDir[60] = "~";
	
	int index = 0;
	
	char* path1;		
	char* path2;
	char path3[60] = "~";	
	strcpy(path3, inputPath);	
	path2 = strtok(path3, "/");
	
	//��һ�֣��Ӹ�Ŀ¼��ʼѰ��
	if(strcmp(path2, "~") == 0) {			
		p = search(dirEntry, path2, 0);
		if(NULL == p) {
			printf("path is not exist");
			return -1;
		}
		else {
			while(true) {								//path2ǰ��ÿһ��Ŀ¼ 
				path1 = path2;
				path2 = strtok(NULL, "/");
				if(NULL == path2) break;
				p = searchCurrentDir(p, path2, 0);
			}
			if(NULL == p) {
				printf("path is not exist");
				return -1;
			}
			
			if(checkRead(p) == 1) {
				printf("can't read");
				return -1;
			}
			
			strcpy(currentStatus->currentDir, path1);	//�޸ĵ�ǰĿ¼ 
			strcpy(path, inputPath);
			return 1;									//path1 ���һ��Ŀ¼ 
		}
				
	} else if(strcmp(path2, "..") == 0) {
		//�����֣��ӵ�ǰĿ¼����Ѱ�� 
		strcpy(newPath, path);
		getNewPath(newPath); 
		while(true) {
			path1 = path2;
			path2 = strtok(NULL, "/");
			if(NULL == path2) break;
			
			if(strcmp(path2, "..") == 0) {
				getNewPath(newPath); 
			} else {
				
				if(strcmp(newPath, "~") == 0) {
					p = search(dirEntry, path2, 0);	
					strcat(newPath, "/");
					strcat(newPath, path2);
					index = 1;						
				} else {
					
					strcpy(newDir, newPath);
				
					getLastPath(newDir);
					p = search(dirEntry, newDir, 0);
					p = search(p, path2, 0);
					strcat(newPath, "/");
					strcat(newPath, path2);				
				}
			}
		}
		
		if(NULL == p) {
			printf("path is not exist");
			return -1;
		}
		if(checkRead(p) == 1) {
			printf("can't read");
			return -1;
		}
		
		if(strcmp(newPath, "~") == 0) {
			printf("can't read");
			return -1;
		} else {
			if(index == 1) {
				strcpy(currentStatus->currentDir, path1);	//�޸ĵ�ǰĿ¼ 
				strcpy(path, newPath);
				return 1;									//path1 ���һ��Ŀ¼ 
			} else {
				strcpy(currentStatus->currentDir, path1);	//�޸ĵ�ǰĿ¼ 
				strcpy(path, newPath);
				return 1;									//path1 ���һ��Ŀ¼ 
			}
		}
		
	} else {		
		//�ڶ��֣��ӵ�ǰĿ¼����Ѱ��
		p = search(dirEntry, user->userName, 0);
		p = search(p, path2, 0);
		if(NULL == p) {
			printf("path is not exist");
			return -1;
		}
		else {
			while(true) {
				path1 = path2;
				path2 = strtok(NULL, "/");
				if(NULL == path2) break;
				p = search(p, path2, 0);
			}
			if(NULL == p) {
				printf("path is not exist");
				return -1;
			}		
			if(checkRead(p) == 1) {
				printf("can't read");
				return -1;
			}		
			strcpy(currentStatus->currentDir, path1);	//�޸ĵ�ǰĿ¼ 
			strcat(path, "/");
			strcat(path, inputPath);
			return 1;									//path1 ���һ��Ŀ¼ 
		}
	}
}

/*
 *�޸ĵ�ǰĿ¼ 
 */
commandCd(char* secondStr) {
	//��һ�����������
	//�ڶ��������Ŀ¼�Ƿ����
	if(cdFindDir(secondStr) == 1) {
		//������������û��Ƿ���Ȩ�޷���		
		//���Ĳ����޸�Ŀ¼	
	} 
}

/*
 *��õ�ǰĿ¼��ָ�� 
 */
DirEntry *deleteFineNode() {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	
	char operatePath[60] = "~";
	char* path1;
	strcpy(operatePath, path);
	path1 = strtok(operatePath, "/");
	p = search(dirEntry, path1, 0);
	while(true) {
		path1 = strtok(NULL, "/");
		if(NULL == path1) break;
		p = search(p, path1, 0);
	}
	if(NULL == p) return NULL;
	else return p;
}

//ɾ���ļ� 
int deleteFile(DirEntry* q, DirEntry* p) {
	DirEntry *index = (DirEntry *)malloc(sizeof(DirEntry));
	if(strcmp(p->firstChild->fileName, q->fileName) == 0) {
		//ɾ�����ļ�Ϊ��ǰĿ¼���ӽڵ� 
		if(NULL == q->nextSibling) p->firstChild = NULL;
		else p->firstChild = q->nextSibling;
		return 1;
	}
	else {
		index = p->firstChild;
		while(true) {
			//ɾ�����ļ�Ϊ��ǰĿ¼��һ�����ֵܽ�� 
			if(NULL == index->nextSibling) break;
			if(strcmp(index->nextSibling->fileName, q->fileName) == 0) {
				if(NULL == index->nextSibling->nextSibling) index->nextSibling = NULL; 
				else {
					index->nextSibling = index->nextSibling->nextSibling;
				}
				return 1;
			}
			index = index->nextSibling;
		}
	}
	return -1;
}

/*
 *�޸�λʾͼ��fat�� 
 */ 
changeBitMapAndFat(int address) {	
	
	int i = address;
	int j;
	int m, n;		
	if(i == -1) {
		
	} else {
		printf("\n");
		BitMap[i / bitMapSize][i % bitMapSize] = -1;
		i = Fat[i];
		while(true) {
			if(Fat[i] == -1) {
				BitMap[i / bitMapSize][i % bitMapSize] = -1;
				break;
			} else {
				BitMap[i / bitMapSize][i % bitMapSize] = -1;
			}
			i = Fat[i];
		}
	}
	
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			if(BitMap[m][n] == -1) {
				BitMap[m][n] = 0;
				Fat[m * bitMapSize + n] = -1;
			}
		}
	}
}

/*
 *ɾ���ļ� 
 */
commandDelete(char* secondStr) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	
	p = deleteFineNode();
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 3);
		//��һ������鵱ǰĿ¼�Ƿ���ڸ��ļ�
		if(NULL == q) printf("file is not exist");
		else {
			//�ڶ�����ɾ�����ļ�
			//��������������ļ����޸�λʾͼ��fat��
			if(	deleteFile(q, p) == 1) printf("delete successful");
			else printf("delete fail");
			
			changeBitMapAndFat(q->fileAddr);
		}
	}
}

/*
 *�ļ��Ķ�  
 */
commandRead(char* secondStr) {
	//��һ�������ҵ�ǰĿ¼�Ƿ���ڸ��ļ� 
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	p = deleteFineNode();
	
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 1);
		
		if(NULL == q) printf("file is not exist");
		else {
			if(q->type != 1) printf(" is not a file");
			else {
				if(q->state != -1) printf("please open the file at first");
				else {
					printf("read successful");
					printf("\n\nfilename:%s\n", q->fileName);
					if(NULL == q->content) printf("\n");
					else printf("content:%s\n", q->content);
				}
			}
		}
	}
}


/*
 *�ļ���  
 */
commandOpen(char* secondStr) {
	//��һ�������ҵ�ǰĿ¼�Ƿ���ڸ��ļ� 
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	p = deleteFineNode();
	
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 1);
		
		if(NULL == q) printf("file is not exist");
		else {
			if(q->type != 1) printf(" is not a file");
			else {
				if(q->state == -1) printf("file have already opened");
				else {
					q->state = -1;
					printf("open successful");
				}
			}
		}
	}
	
}

/*
 *�ļ��Ĺر� 
 */
commandClose(char* secondStr) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	p = deleteFineNode();
	
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 1);
		if(NULL == q) printf("file is not exist");
		else {
			if(q->type != 1) printf(" is not a file");
			else {
				if(q->state != -1) printf("file is closed");
				else {
					q->state = 1;
					printf("close successful");
				}
			}
		}
	}
}

/*
 *�ļ���д 
 */
commandWrite(char* secondStr) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	p = deleteFineNode();
	
	char* input;	
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 1);
		if(NULL == q) printf("file is not exist");
		else {
			if(q->type != 1) printf(" is not a file");
			else {
				if(q->state != -1) printf("please open the file at first");
				else {
					printf("content:");
					scanf("%s", q->content);
					printf("\nwrite successful");
				}
			}
		}
	}
	
}

//�鿴λʾͼ 
checkBitMap() {
	int m, n;
	//Fat�� 
	printf("\nFat:\n");
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			printf("%d	", Fat[m * bitMapSize + n]);
		}
		printf("\n");
	}
	//λʾͼ 
	printf("\nBitMap:\n");
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			printf("%d	", BitMap[m][n]);
		}
		printf("\n");
	}
}

/*
 *������� 
 */
analysis(char* command) {	
	char secondStr[20];
	
	char* currentPath;
	
	if(checkCommand(command) == -1) {						//��Ч���� 
		printf("invalid command");
	} else if(checkUserLogined() == -1) {					//�û�δ��¼ 
		printf("please login in first");
	} else {
		if(strcmp(command, allCommand[0]) == 0) printf("user have existed"); 
		else if(strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0) {
			//--help -h 	��ѯ���� 
			commandHelp();
		} else if(strcmp(command, allCommand[3]) == 0) {
			//--ls 			��ѯĿ¼�ļ��б� 			
			strcpy(currentPath, path);
			commandLs(getCurrentDir(currentPath));						
		} else if(strcmp(command, allCommand[4]) == 0) {
			//--create 		�ļ��Ĵ��� 
			scanf("%s", secondStr);
			commandCreate(secondStr);		
		} else if(strcmp(command, allCommand[5]) == 0) {
			//--open		�ļ��Ĵ� 
			scanf("%s", secondStr);
			commandOpen(secondStr);
		} else if(strcmp(command, allCommand[6]) == 0) {
			//--read		�ļ��Ķ� 
			scanf("%s", secondStr);
			commandRead(secondStr);
		} else if(strcmp(command, allCommand[7]) == 0) {
			//--write		�ļ���д 
			scanf("%s", secondStr);
			commandWrite(secondStr);
		} else if(strcmp(command, allCommand[8]) == 0) {
			//--close		�ļ��ر� 
			scanf("%s", secondStr);
			commandClose(secondStr);
		} else if(strcmp(command, allCommand[9]) == 0) {
			//--delete		ɾ���ļ� 
			scanf("%s", secondStr);
			commandDelete(secondStr);
		} else if(strcmp(command, allCommand[10]) == 0) {
			//--mkdir		����Ŀ¼ 
			scanf("%s", secondStr);
			commandMkdir(secondStr);
		} else if(strcmp(command, allCommand[11]) == 0) {	
			//--cd			�ı䵱ǰĿ¼ 
			scanf("%s", secondStr);
			commandCd(secondStr);
		} else if(strcmp(command, allCommand[12]) == 0) {
			//--dir			�г��ļ�Ŀ¼ 
			strcpy(currentPath, path);
			commandLs(getCurrentDir(currentPath));
		} else if(strcmp(command, allCommand[13]) == 0) {
			//--logout		�˳� 
			commandLogout();
		} else if(strcmp(command, allCommand[14]) == 0) {
			//--check		�鿴λʾͼ
			checkBitMap(); 
		}
	}
}


/*
 **��¼�� 
*/
logining() {
	char command[20];

	char username[10];
	char password[10];
	
	while (true) {
		printHead();
		scanf("%s", command);	
		if(checkCommand(command) == -1) {
			printf("invalid command");
		} else if(strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0) {
			commandHelp();
		} else if(strcmp(command, allCommand[0])  == 0) {
			if(checkUserLogined() == -1) {						//�û�δ��¼ 	
				scanf("%s", username);							//�û������û��� 
				printf("password:");
				scanf("%s", password);							//�û��������� 
				checkLogin(username, password);
			} else {											//�û��ѵ�½ 
				printf("�û�"); 
			}
		} else {
			analysis(command);									//�������� 
		}
	}
}

/*
 *��ʼ�� 
 */
initSystem() {
	int m, n;
	
	currentStatus = (CurrentStatus*)malloc(sizeof(CurrentStatus));
	
	//��ʼ���û� 
	user = (User *) malloc (sizeof(User));						
	
	//�ļ�Ŀ¼�ṹ����� 
	DirEntry* childChildDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	dirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* wenDirEntry = (DirEntry *)malloc(sizeof(DirEntry));	
	DirEntry* liuDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* fuDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* caiDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	
	//�û�cai 
	strcpy(caiDirEntry->fileName, "cai");
	caiDirEntry->type = 0;
	caiDirEntry->firstChild = NULL;
	caiDirEntry->nextSibling = NULL;
	
	//�û�fu 
	strcpy(fuDirEntry->fileName, "fu");
	fuDirEntry->type = 0;
	fuDirEntry->firstChild = NULL;
	fuDirEntry->nextSibling = caiDirEntry;
	
	//�û�liu 
	strcpy(liuDirEntry->fileName, "liu");
	liuDirEntry->type = 0;	
	liuDirEntry->firstChild = NULL;
	liuDirEntry->nextSibling = fuDirEntry;
	
	//�û�wen�µ��ļ� 
	strcpy(childChildDirEntry->fileName, "�ĸ�");
	childChildDirEntry->type = 1;	
	childChildDirEntry->firstChild = NULL;
	childChildDirEntry->nextSibling = NULL;
	
	//�û�wen 
	strcpy(wenDirEntry->fileName, "wen");
	wenDirEntry->type = 0;	
	wenDirEntry->firstChild = childChildDirEntry;
	wenDirEntry->nextSibling = liuDirEntry;
	
	//��Ŀ¼ 
	strcpy(dirEntry->fileName, "~");
	dirEntry->type = 0;		
	dirEntry->firstChild = wenDirEntry;
	dirEntry->nextSibling = NULL;
	
	//��ʼ��fat�� 
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			Fat[m * bitMapSize + n] = -1;
		}
	}
}

test() {
	char c;
	FILE* fp = NULL;
    fp = fopen("D:\\studio\\vsc\\haha.txt", "W"); 
    if(NULL==fp) {
    	
	} else {
		while(scanf("%c",&c)!=EOF) fprintf(fp,"%c",c); //�ӿ���̨�ж��벢���ı����
	    fclose(fp);
	    fp=NULL;
	}
}

main() {
	initSystem();
	welcomeFirst();
	logining();	
}



