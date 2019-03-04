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

//用户 
typedef struct u {
	char *userName;											//用户名称 
	int userType;											//用户类型 
}User;

//当前状态 
typedef struct cs {
	User *currentUser;
	char *currentPath;
	char currentDir[60];
}CurrentStatus;


//文件目录树结构 
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
int Fat[bitMapSize*bitMapSize] = {0};						//fat表 	-1表示文件结束 
int	BitMap[bitMapSize][bitMapSize] = {0};					//位示图 	0表示空闲，1表示已用 



int isLogin = -1; 											//用户是否登录标志 
char path[60] = "~";										//用户访问的路径 
char dir[60];

HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);

char allCommand[commandNum][10] = {"login", "help", "h", "ls", "create", "open", "read", "write", "close", "delete", "mkdir", "cd", "dir", "logout", "check"};
char allCommandMean[commandNum][20] = {"用户登录", "查询帮助", "查询帮助", "查询文件目录", "创建文件", "打开文件", "文件的读", "文件的写", "关闭文件",
											 "删除文件", "创建目录", "改变当前目录", "查询文件目录", "退出登录", "查看位示图"};
															//已设定好的命令 

char allUsername[4][20] = {"wen", "liu", "fu", "cai"};
char allPassword[4][20] = {"123", "123", "123", "123"};


/*
 *改变当前字体颜色
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
    printf("                    *       欢迎进入%6s  的文件系统        *\n", user->userName);
    printf("                    *                                         *\n");
    printf("                    *******************************************\n");
}



/*
 *树的先序遍历 
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
	changeColor(WHITE);
	if(checkUserLogined() == -1) {
		printf("\n[root@localhost]%s: $", path);					//用户未登录 
	} else {
		printf("\n[%s@localhost]%s: $", user->userName, path);	//用户已登录 
	}
	changeColor(GREEN);	
}

/*
 *检查命令是否有效 
 */
int checkCommand(char* command) {
	if(strcmp(command, allCommand[0]) == 0 || strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0 
				|| strcmp(command, allCommand[3]) == 0 || strcmp(command, allCommand[4]) == 0 || strcmp(command, allCommand[5]) == 0 
				|| strcmp(command, allCommand[6]) == 0 || strcmp(command, allCommand[7]) == 0 || strcmp(command, allCommand[8]) == 0 
				|| strcmp(command, allCommand[9]) == 0 || strcmp(command, allCommand[10]) == 0 || strcmp(command, allCommand[11]) == 0
				|| strcmp(command, allCommand[12]) == 0 || strcmp(command, allCommand[13]) == 0 || strcmp(command, allCommand[14]) == 0){
		return 1;												//返回1，命令是有效的 
	} else return -1;   										//返回-1，命令无效 
}

/*
 *检查登录 
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
		strcpy(currentStatus->currentDir, username);			//当前目录 
		printf("login successful");								//登录成功 
		welcomeUser();
	} else {
		printf("username or password is wrong");				//登录失败 
	}
}

commandLogout() {
	free(user);
	user = (User *)malloc(sizeof(User));				//初始化用户 
	strcpy(path, "~");
	isLogin = -1;
}

/*
 *打印所有的命令 
 */ 
commandHelp() {
	int i = 0; 
	for(i = 0; i < commandNum; i++) {
		printf("%s	", allCommand[i]);
		printf("%s\n", allCommandMean[i]);
	}
}

/*
 *通过/划分命令，找出当前所在的目录 
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
 *在目录结构树中找到当前目录结点 
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
 *从指定目录中找出目标目录或文件 
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
 *列出当前目录的所有项 
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
	for(i = 0; i < bitMapSize; i++) {			//检查是否有足够的空闲盘块
		for(j = 0; j < bitMapSize; j++) {
			if(BitMap[i][j] == 0){
				BitMap[i][j] = -1;
				count++;						//找到一个空闲盘块，count++ 
				if(count == fileSize) break;	//找到足够的空间时，跳出循环 
			} 
		}
		if(count == fileSize) break;
	}
	 
	if(count < fileSize) {						//没有足够空间时，将上面标记的-1改回0 
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
	for(i = 0; i < bitMapSize; i++) {				//找出寻找空间时标记的-1 
		for(j = 0; j < bitMapSize; j++) {
			if(BitMap[i][j] == -1) {
				BitMap[i][j] = 1;
				count++;
				if(count == 1) {
					firstBit = i * bitMapSize + j;
					index = i * bitMapSize + j;
				}
				else {								//在fat表中，将盘块号链接下一个盘块 
					Fat[index] = i * bitMapSize + j;
					index = i * bitMapSize + j;
				}
				if(count == fileSize) {				//当找到最后一个标记的盘块时，在fat表中链接-1，表示文件的结束符 
					Fat[i * bitMapSize + j] = -1;
					break;
				}
			}
		}
	}
	return firstBit;
}

/*
 *在当前目录创建文件结点 
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
		
		//如果当前目录没有任何项，则直接孩子指向q 
		if(NULL == p->firstChild) {
			p->firstChild = q;
			
		} else {
			//如果当前目录的子节点的兄弟结点为空，则子节点的兄弟结点指向q 
			if(NULL == p->firstChild->nextSibling) {
				p->firstChild->nextSibling = q;
				
			} else {
				//否则找到子节点最后的兄弟结点，指向q 
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
 *判断输入的文件大小是否为数字 
 */ 
int createCheckFileSize(int fileSize) {
	
}

/*
 *判断文件是否存在重名 
 *当重名的分别是文件和目录时，可创建 
 */
int createCheckRepeatName(char* fileName, int type) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	
	p = search(dirEntry, currentStatus->currentDir, 0);
	
	if(NULL == p) printf("path is wrong");
	else if(p->type != 0) printf("%s not a directory", p->fileName);
	else {
		if(NULL == p->firstChild) return 1; 								//孩子结点为空，可创建 
		else if(strcmp(p->firstChild->fileName, fileName) == 0 && p->firstChild->type == type) return 0;	//与孩子结点重名，不可创建 
		else {
			if(NULL == p->firstChild->nextSibling) return 1;				//孩子结点的兄弟结点为空，可创建 
			else {
				q = p->firstChild;
				while(true) {
					if(NULL == q->nextSibling) {
						return 1;											//与所有兄弟结点均不重名，可创建 
					}
					
					if(strcmp(q->nextSibling->fileName, fileName) == 0 && q->nextSibling->type == type) {
						return 0;											//与某一兄弟结点重名，不可创建 
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
 *检查用户输入的文件名和文件大小 
 */
int createCheckInput(char* fileName, int fileSize) {
	if(NULL == fileName) {
		printf("fileName can't null'");
		return -1;
	} 
}

/*
 *创建文件 
 */
commandCreate(char * secondStr) {
	int fileSize;
	int tempCheckBitMap;
	int tempFirstBit;
	int tempCheckInput;
	int tempCheckRepeatName;
	printf("fileSize:");
	scanf("%d", &fileSize);
	
	//第一步：检查输入内容是否非法
//	tempCheckInput = createCheckInput(fileName, fileSize);
	//第二步：检查文件重名
	tempCheckRepeatName = createCheckRepeatName(secondStr, 1);
	if(tempCheckRepeatName == 1) {
		//第三步：检查位示图，查找空闲盘块
		tempCheckBitMap = createCheckBitMap(fileSize);
		if(tempCheckBitMap == -1) {
			printf("not enough space");
		} else {
			//第四步：将空闲盘块号链接到fat表
			tempFirstBit = createChangeFat(fileSize);
			//第五步：在目录树中创建文件结点 
			createCreateNode(secondStr, fileSize, tempFirstBit, 1);
		}
	}
	else printf("filename can't repeat");
}

/*
 *创建目录 
 */
commandMkdir(char * secondStr) {
	int tempCheckRepeatName;
	
	//第一步，检查输入	
	if(createCheckRepeatName(secondStr, 0) == 1) {			//第二步，检查重名
		createCreateNode(secondStr, -1, -1, 0);				//第三步，创建文件结点 
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
 *获取路径中最后一个目录 
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
 *截掉路径中最后一个目录 
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
 *查找用户输入的目录 
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
	
	//第一种：从根目录开始寻找
	if(strcmp(path2, "~") == 0) {			
		p = search(dirEntry, path2, 0);
		if(NULL == p) {
			printf("path is not exist");
			return -1;
		}
		else {
			while(true) {								//path2前面每一个目录 
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
			
			strcpy(currentStatus->currentDir, path1);	//修改当前目录 
			strcpy(path, inputPath);
			return 1;									//path1 最后一个目录 
		}
				
	} else if(strcmp(path2, "..") == 0) {
		//第三种：从当前目录向上寻找 
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
				strcpy(currentStatus->currentDir, path1);	//修改当前目录 
				strcpy(path, newPath);
				return 1;									//path1 最后一个目录 
			} else {
				strcpy(currentStatus->currentDir, path1);	//修改当前目录 
				strcpy(path, newPath);
				return 1;									//path1 最后一个目录 
			}
		}
		
	} else {		
		//第二种：从当前目录向下寻找
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
			strcpy(currentStatus->currentDir, path1);	//修改当前目录 
			strcat(path, "/");
			strcat(path, inputPath);
			return 1;									//path1 最后一个目录 
		}
	}
}

/*
 *修改当前目录 
 */
commandCd(char* secondStr) {
	//第一步：检查输入
	//第二步：检查目录是否存在
	if(cdFindDir(secondStr) == 1) {
		//第三步：检查用户是否有权限访问		
		//第四步：修改目录	
	} 
}

/*
 *获得当前目录的指针 
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

//删除文件 
int deleteFile(DirEntry* q, DirEntry* p) {
	DirEntry *index = (DirEntry *)malloc(sizeof(DirEntry));
	if(strcmp(p->firstChild->fileName, q->fileName) == 0) {
		//删除的文件为当前目录的子节点 
		if(NULL == q->nextSibling) p->firstChild = NULL;
		else p->firstChild = q->nextSibling;
		return 1;
	}
	else {
		index = p->firstChild;
		while(true) {
			//删除的文件为当前目录第一结点的兄弟结点 
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
 *修改位示图和fat表 
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
 *删除文件 
 */
commandDelete(char* secondStr) {
	DirEntry *p = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry *q = (DirEntry *)malloc(sizeof(DirEntry));
	
	p = deleteFineNode();
	if(NULL != p) {
		q = searchCurrentDir(p, secondStr, 3);
		//第一步：检查当前目录是否存在该文件
		if(NULL == q) printf("file is not exist");
		else {
			//第二步：删除该文件
			//第三步：如果问文件，修改位示图和fat表
			if(	deleteFile(q, p) == 1) printf("delete successful");
			else printf("delete fail");
			
			changeBitMapAndFat(q->fileAddr);
		}
	}
}

/*
 *文件的读  
 */
commandRead(char* secondStr) {
	//第一步：查找当前目录是否存在该文件 
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
 *文件打开  
 */
commandOpen(char* secondStr) {
	//第一步：查找当前目录是否存在该文件 
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
 *文件的关闭 
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
 *文件的写 
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

//查看位示图 
checkBitMap() {
	int m, n;
	//Fat表 
	printf("\nFat:\n");
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			printf("%d	", Fat[m * bitMapSize + n]);
		}
		printf("\n");
	}
	//位示图 
	printf("\nBitMap:\n");
	for(m = 0; m < bitMapSize; m++) {
		for(n = 0; n < bitMapSize; n++) {
			printf("%d	", BitMap[m][n]);
		}
		printf("\n");
	}
}

/*
 *命令分析 
 */
analysis(char* command) {	
	char secondStr[20];
	
	char* currentPath;
	
	if(checkCommand(command) == -1) {						//无效命令 
		printf("invalid command");
	} else if(checkUserLogined() == -1) {					//用户未登录 
		printf("please login in first");
	} else {
		if(strcmp(command, allCommand[0]) == 0) printf("user have existed"); 
		else if(strcmp(command, allCommand[1]) == 0 || strcmp(command, allCommand[2]) == 0) {
			//--help -h 	查询帮助 
			commandHelp();
		} else if(strcmp(command, allCommand[3]) == 0) {
			//--ls 			查询目录文件列表 			
			strcpy(currentPath, path);
			commandLs(getCurrentDir(currentPath));						
		} else if(strcmp(command, allCommand[4]) == 0) {
			//--create 		文件的创建 
			scanf("%s", secondStr);
			commandCreate(secondStr);		
		} else if(strcmp(command, allCommand[5]) == 0) {
			//--open		文件的打开 
			scanf("%s", secondStr);
			commandOpen(secondStr);
		} else if(strcmp(command, allCommand[6]) == 0) {
			//--read		文件的读 
			scanf("%s", secondStr);
			commandRead(secondStr);
		} else if(strcmp(command, allCommand[7]) == 0) {
			//--write		文件的写 
			scanf("%s", secondStr);
			commandWrite(secondStr);
		} else if(strcmp(command, allCommand[8]) == 0) {
			//--close		文件关闭 
			scanf("%s", secondStr);
			commandClose(secondStr);
		} else if(strcmp(command, allCommand[9]) == 0) {
			//--delete		删除文件 
			scanf("%s", secondStr);
			commandDelete(secondStr);
		} else if(strcmp(command, allCommand[10]) == 0) {
			//--mkdir		创建目录 
			scanf("%s", secondStr);
			commandMkdir(secondStr);
		} else if(strcmp(command, allCommand[11]) == 0) {	
			//--cd			改变当前目录 
			scanf("%s", secondStr);
			commandCd(secondStr);
		} else if(strcmp(command, allCommand[12]) == 0) {
			//--dir			列出文件目录 
			strcpy(currentPath, path);
			commandLs(getCurrentDir(currentPath));
		} else if(strcmp(command, allCommand[13]) == 0) {
			//--logout		退出 
			commandLogout();
		} else if(strcmp(command, allCommand[14]) == 0) {
			//--check		查看位示图
			checkBitMap(); 
		}
	}
}


/*
 **登录中 
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
			if(checkUserLogined() == -1) {						//用户未登录 	
				scanf("%s", username);							//用户键入用户名 
				printf("password:");
				scanf("%s", password);							//用户键入密码 
				checkLogin(username, password);
			} else {											//用户已登陆 
				printf("用户"); 
			}
		} else {
			analysis(command);									//分析命令 
		}
	}
}

/*
 *初始化 
 */
initSystem() {
	int m, n;
	
	currentStatus = (CurrentStatus*)malloc(sizeof(CurrentStatus));
	
	//初始化用户 
	user = (User *) malloc (sizeof(User));						
	
	//文件目录结构树结点 
	DirEntry* childChildDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	dirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* wenDirEntry = (DirEntry *)malloc(sizeof(DirEntry));	
	DirEntry* liuDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* fuDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	DirEntry* caiDirEntry = (DirEntry *)malloc(sizeof(DirEntry));
	
	//用户cai 
	strcpy(caiDirEntry->fileName, "cai");
	caiDirEntry->type = 0;
	caiDirEntry->firstChild = NULL;
	caiDirEntry->nextSibling = NULL;
	
	//用户fu 
	strcpy(fuDirEntry->fileName, "fu");
	fuDirEntry->type = 0;
	fuDirEntry->firstChild = NULL;
	fuDirEntry->nextSibling = caiDirEntry;
	
	//用户liu 
	strcpy(liuDirEntry->fileName, "liu");
	liuDirEntry->type = 0;	
	liuDirEntry->firstChild = NULL;
	liuDirEntry->nextSibling = fuDirEntry;
	
	//用户wen下的文件 
	strcpy(childChildDirEntry->fileName, "文哥");
	childChildDirEntry->type = 1;	
	childChildDirEntry->firstChild = NULL;
	childChildDirEntry->nextSibling = NULL;
	
	//用户wen 
	strcpy(wenDirEntry->fileName, "wen");
	wenDirEntry->type = 0;	
	wenDirEntry->firstChild = childChildDirEntry;
	wenDirEntry->nextSibling = liuDirEntry;
	
	//根目录 
	strcpy(dirEntry->fileName, "~");
	dirEntry->type = 0;		
	dirEntry->firstChild = wenDirEntry;
	dirEntry->nextSibling = NULL;
	
	//初始化fat表 
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
		while(scanf("%c",&c)!=EOF) fprintf(fp,"%c",c); //从控制台中读入并在文本输出
	    fclose(fp);
	    fp=NULL;
	}
}

main() {
	initSystem();
	welcomeFirst();
	logining();	
}



