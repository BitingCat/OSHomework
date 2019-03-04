#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#define false 0
#define true 1
#define commandNum 14


/*
 *�û� 
 */
typedef struct u {
	char *userName;											//�û����� 
	int userType;											//�û����� 
}User;

/*
 *���ݿ����ڴ��е�����ṹ 
 */ 
typedef struct fb {
	unsigned FileBlockId;
	unsigned blockSize;
	char *FileBlockAddr;
	struct fb *next;
}FileBlock;

/*
 *�ļ��������ṹ 
 */
typedef struct fie {
	unsigned index;
	char fileName[10];
	char parentName[10];
	unsigned fileBlockId;
	unsigned fileLevel;
	unsigned effect;
}FileIndexElement;

/*
 *�ļ�ϵͳ���ļ�Ԫ�صĽṹ 
 */
typedef struct fse {
	struct fse *parent;
	unsigned fileLevel;
	char FileName[10];
	unsigned fileBlockId;
	unsigned fileElemLength;
//	FileType type;
	u creator;
	char createTime[18];
	char lastModTime[18];
	char *fileData;
}FSElement;

/*
 *�ļ�ϵͳ�ṹ 
 */
typedef struct sb{
	char *fileStartAddr;
	unsigned maxBlockSize;
//	BitMap bm;
	unsigned blockCount;
	FileBlock *head;
//	FileIndex FI;
}FileBlockStruct;
 
/*
 *�ļ�ϵͳ��ǰ������״̬ 
 */
typedef struct {
	u currentUser;
	unsigned fileLevel;
	fse *currentParent;
	char *currentPath;
}CurrentStatus;

User *user = (User *) malloc (sizeof(User));				//��ʼ���û� 

int isLogin = -1; 											//�û��Ƿ��¼��־ 
char path[20] = "~";										//�û����ʵ�·�� 

char allCommand[commandNum][10] = {"login", "help", "h", "ls", "create", "open", "read", "write", "close", "delete", "mkdir", "cd", "dir", "logout"};
char allCommandMean[commandNum][20] = {"�û���¼", "��ѯ����", "��ѯ����", "��ѯ�ļ�Ŀ¼", "�����ļ�", "���ļ�", "�ļ��Ķ�", "�ļ���д", "�ر��ļ�",
											 "ɾ���ļ�", "����Ŀ¼", "�ı䵱ǰĿ¼", "��ѯ�ļ�Ŀ¼", "�˳���¼"};
															//���趨�õ����� 


welcomeFirst () {
	printf("\n");
	printf("                    *******************************************\n");
    printf("                    *                                         *\n");
    printf("                    *                 Welcome                 *\n");
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
}

welcomeUser () {
	printf("\n");
	printf("                    *******************************************\n");
    printf("                    *                                         *\n");
    printf("                    *       ��ӭ����%6s  ���ļ�ϵͳ        *\n", user->userName);
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
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
	
	if(checkUserLogined() == -1) {
		printf("\n[null@localhost]%s$", path);					//�û�δ��¼ 
	} else {
		printf("\n[%s@localhost]%s$", user->userName, path);	//�û��ѵ�¼ 
	}
	
}

/*
 *��������Ƿ���Ч 
 */
int checkCommand(char* command) {
	if(strcmp(command, allCommand[0]) == 0 || strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0 
				|| strcmp(command, allCommand[3]) == 0 || strcmp(command, allCommand[4]) == 0 || strcmp(command, allCommand[5]) == 0 
				|| strcmp(command, allCommand[6]) == 0 || strcmp(command, allCommand[7]) == 0 || strcmp(command, allCommand[8]) == 0 
				|| strcmp(command, allCommand[9]) == 0 || strcmp(command, allCommand[10]) == 0 || strcmp(command, allCommand[11]) == 0
				|| strcmp(command, allCommand[12]) == 0 || strcmp(command, allCommand[13]) == 0){
		return 1;												//����1����������Ч�� 
	} else return -1;   										//����-1��������Ч 
}



/*
 *����¼ 
*/
int checkLogin(char* username, char* password) {
	if(strcmp(username, "wen") == 0 && strcmp(password, "123") == 0) {
		user->userName = username;
		isLogin = 1;
		printf("login successful");								//��¼�ɹ� 
		return 1;
	} else {
		printf("username or password is wrong");				//��¼ʧ�� 
		return -1;
	}
}

/*
 *������� 
 */
analysis(char* command) {
	int i = 0; 
	if(checkCommand(command) == -1) {						//��Ч���� 
		printf("invalid command");
	} else if(checkUserLogined() == -1) {					//�û�δ��¼ 
		printf("please login in first");
	} else {
		if(strcmp(command, allCommand[0]) == 0) printf("user have existed"); 
		else if(strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0) {
			//--help -h 	��ѯ���� 
			for(i = 0; i < commandNum; i++) {
				printf("%s	", allCommand[i]);
				printf("%s\n", allCommandMean[i]);
			}
		} else if(strcmp(command, allCommand[3]) == 0) {
			//--ls 			��ѯĿ¼�ļ��б� 
		} else if(strcmp(command, allCommand[4]) == 0) {
			//--create 		�ļ��Ĵ��� 
		} else if(strcmp(command, allCommand[5]) == 0) {
			//--open		�ļ��Ĵ� 
		} else if(strcmp(command, allCommand[6]) == 0) {
			//--read		�ļ��Ķ� 
		} else if(strcmp(command, allCommand[7]) == 0) {
			//--write		�ļ���д 
		} else if(strcmp(command, allCommand[8]) == 0) {
			//--close		�ļ��ر� 
		} else if(strcmp(command, allCommand[9]) == 0) {
			//--delete		ɾ���ļ� 
		} else if(strcmp(command, allCommand[10]) == 0) {
			//--mkdir		����Ŀ¼ 
		} else if(strcmp(command, allCommand[11]) == 0) {
			//--cd			�ı䵱ǰĿ¼ 
		} else if(strcmp(command, allCommand[12]) == 0) {
			//--dir			�г��ļ�Ŀ¼ 
		} else if(strcmp(command, allCommand[13]) == 0) {
			//--logout		�˳� 
			free(user);
			user = (User *)malloc(sizeof(User));				//��ʼ���û� 
			strcpy(path, "~");
			isLogin = -1;
		}
		
	}
}

/*
 **��¼�� 
*/
logining() {
	char command[10];

	char username[10];
	char password[10];
	
	while (true) {
		printHead();
		scanf("%s", command);
		
		if(checkCommand(command) == -1) {
			printf("invalid command");
		} else if(strcmp(command, allCommand[0])  == 0) {
			if(checkUserLogined() == -1) {						//�û�δ��¼ 
				printf("username:");
				scanf("%s", username);							//�û������û��� 
				printf("password:");
				scanf("%s", password);							//�û��������� 
				if(checkLogin(username, password) == 1) {		//��¼�ɹ� 
					strcat(path, "/");
					strcat(path, user->userName);
					strcat(path, "/");
					welcomeUser();
				} else {										//��¼ʧ�� 
					continue;
				}
			} else {											//�û��ѵ�½ 
				printf("user have existed"); 
			}
		} else {
			analysis(command);									//�������� 
		}
	}
	
}

main() {
	
	welcomeFirst();
	logining();	
}



