#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#define false 0
#define true 1
#define commandNum 14


/*
 *用户 
 */
typedef struct u {
	char *userName;											//用户名称 
	int userType;											//用户类型 
}User;

/*
 *数据块在内存中的物理结构 
 */ 
typedef struct fb {
	unsigned FileBlockId;
	unsigned blockSize;
	char *FileBlockAddr;
	struct fb *next;
}FileBlock;

/*
 *文件的索引结构 
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
 *文件系统中文件元素的结构 
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
 *文件系统结构 
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
 *文件系统当前的运行状态 
 */
typedef struct {
	u currentUser;
	unsigned fileLevel;
	fse *currentParent;
	char *currentPath;
}CurrentStatus;

User *user = (User *) malloc (sizeof(User));				//初始化用户 

int isLogin = -1; 											//用户是否登录标志 
char path[20] = "~";										//用户访问的路径 

char allCommand[commandNum][10] = {"login", "help", "h", "ls", "create", "open", "read", "write", "close", "delete", "mkdir", "cd", "dir", "logout"};
char allCommandMean[commandNum][20] = {"用户登录", "查询帮助", "查询帮助", "查询文件目录", "创建文件", "打开文件", "文件的读", "文件的写", "关闭文件",
											 "删除文件", "创建目录", "改变当前目录", "查询文件目录", "退出登录"};
															//已设定好的命令 


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
    printf("                    *       欢迎进入%6s  的文件系统        *\n", user->userName);
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
}

/*
 *检查是否有用户在线 
 */
 int checkUserLogined() {
 	if(isLogin == 1) return 1;
 	else return -1;
 }

/*
 *打印命令行提示 
 */
printHead() {
	
	if(checkUserLogined() == -1) {
		printf("\n[null@localhost]%s$", path);					//用户未登录 
	} else {
		printf("\n[%s@localhost]%s$", user->userName, path);	//用户已登录 
	}
	
}

/*
 *检查命令是否有效 
 */
int checkCommand(char* command) {
	if(strcmp(command, allCommand[0]) == 0 || strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0 
				|| strcmp(command, allCommand[3]) == 0 || strcmp(command, allCommand[4]) == 0 || strcmp(command, allCommand[5]) == 0 
				|| strcmp(command, allCommand[6]) == 0 || strcmp(command, allCommand[7]) == 0 || strcmp(command, allCommand[8]) == 0 
				|| strcmp(command, allCommand[9]) == 0 || strcmp(command, allCommand[10]) == 0 || strcmp(command, allCommand[11]) == 0
				|| strcmp(command, allCommand[12]) == 0 || strcmp(command, allCommand[13]) == 0){
		return 1;												//返回1，命令是有效的 
	} else return -1;   										//返回-1，命令无效 
}



/*
 *检查登录 
*/
int checkLogin(char* username, char* password) {
	if(strcmp(username, "wen") == 0 && strcmp(password, "123") == 0) {
		user->userName = username;
		isLogin = 1;
		printf("login successful");								//登录成功 
		return 1;
	} else {
		printf("username or password is wrong");				//登录失败 
		return -1;
	}
}

/*
 *命令分析 
 */
analysis(char* command) {
	int i = 0; 
	if(checkCommand(command) == -1) {						//无效命令 
		printf("invalid command");
	} else if(checkUserLogined() == -1) {					//用户未登录 
		printf("please login in first");
	} else {
		if(strcmp(command, allCommand[0]) == 0) printf("user have existed"); 
		else if(strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0) {
			//--help -h 	查询帮助 
			for(i = 0; i < commandNum; i++) {
				printf("%s	", allCommand[i]);
				printf("%s\n", allCommandMean[i]);
			}
		} else if(strcmp(command, allCommand[3]) == 0) {
			//--ls 			查询目录文件列表 
		} else if(strcmp(command, allCommand[4]) == 0) {
			//--create 		文件的创建 
		} else if(strcmp(command, allCommand[5]) == 0) {
			//--open		文件的打开 
		} else if(strcmp(command, allCommand[6]) == 0) {
			//--read		文件的读 
		} else if(strcmp(command, allCommand[7]) == 0) {
			//--write		文件的写 
		} else if(strcmp(command, allCommand[8]) == 0) {
			//--close		文件关闭 
		} else if(strcmp(command, allCommand[9]) == 0) {
			//--delete		删除文件 
		} else if(strcmp(command, allCommand[10]) == 0) {
			//--mkdir		创建目录 
		} else if(strcmp(command, allCommand[11]) == 0) {
			//--cd			改变当前目录 
		} else if(strcmp(command, allCommand[12]) == 0) {
			//--dir			列出文件目录 
		} else if(strcmp(command, allCommand[13]) == 0) {
			//--logout		退出 
			free(user);
			user = (User *)malloc(sizeof(User));				//初始化用户 
			strcpy(path, "~");
			isLogin = -1;
		}
		
	}
}

/*
 **登录中 
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
			if(checkUserLogined() == -1) {						//用户未登录 
				printf("username:");
				scanf("%s", username);							//用户键入用户名 
				printf("password:");
				scanf("%s", password);							//用户键入密码 
				if(checkLogin(username, password) == 1) {		//登录成功 
					strcat(path, "/");
					strcat(path, user->userName);
					strcat(path, "/");
					welcomeUser();
				} else {										//登录失败 
					continue;
				}
			} else {											//用户已登陆 
				printf("user have existed"); 
			}
		} else {
			analysis(command);									//分析命令 
		}
	}
	
}

main() {
	
	welcomeFirst();
	logining();	
}



