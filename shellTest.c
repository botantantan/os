//#define MAX_BYTE 256

void cahnge_Current_Direct(char *path, char *curdir, char parentIndex);
void split_command(char *command, char commandList[16][16], int *num_command);
void copy_Str(char *str1, char *str2, int i);
void changeRel_ToAbs(char *dir, char *parentIndex, int *success);
void clear(char *buffer, int length);

int main() {
	char command[512];
	char prog[15];
	char commandList[16][16];
	char path[512];
	char temp[512];
	char curdir;
	char argc;
	char *argv[16];
	char readbuffer[512*16];
	char parentIdx;
	int result;
	int found;
	int num_command;
	int i,j;
	int addr;
	int sectors = 1;
 	
	curdir = 0xFF;
	while(1) {
		interrupt(0x21, 0x0, "$ ", 0, 0);

		for(i = 0; i < 16; i++) {
			clear(argv[j], 16);
		}
		clear(command,512);
		clear(commandList,16*16);
		clear(path,512);

		interrupt(0x21, 0x1, command, 1, 0);
		split_command(command, commandList, &num_command);
		
		if(commandList[0][0] == 'c' && commandList[0][1] == 'd') {
			copy_Str(commandList[1], path, 0);
			parentIdx = curdir;
			changeRel_ToAbs(path, &parentIdx, &found);
			if(found == 0) {
				cahnge_Current_Direct(path, &curdir, parentIdx);
				result = 1;
			}
			else {
				interrupt(0x21, 0x0, "Directory not found \n\r", 0, 0);
				result = 0;
			}
		}else if (commandList[0][0] == '.' && commandList[0][1] == '/') {
			copy_Str(commandList[0], path, 2);
			argc = num_command;
			for(i = 1; i < num_command; i++) {
				j = i - 1;
				argv[j] = commandList[i];
			}
			interrupt(0x21, 0x20, curdir, argc, argv);
			interrupt(0x21, (curdir << 8) | 0x06, path, &result, 0);
		}else if (commandList[0][0] == 'r' && commandList[0][1] == 'e' && commandList[0][2] == 's'){
			addr = (commandList[1][0] - '0') * 0x1000 + 0x2000;
			interrupt(0x21, 0x33, addr, &result, 0);
			result = !result;
		}else if (commandList[0][0] == 'k' && commandList[0][1] == 'i' && commandList[0][2] == 'l' && commandList[0][3] == 'l'){
			addr = (commandList[1][0] - '0') * 0x1000 + 0x2000;
			interrupt(0x21, 0x34, addr, &result, 0);
			result = !result;
		}else if (commandList[0][0] == 'p' && commandList[0][1] == 'a' && commandList[0][2] == 'u' && commandList[0][3] == 's' && commandList[0][4] == 'e'){
			addr = (commandList[1][0] - '0') * 0x1000 + 0x2000;
			interrupt(0x21, 0x32, addr, &result, 0);
			result = !result;
		}else if (commandList[num_command-1][0] == '&'){
			//interrupt(0x10, 0xE00+'0'+num_command, 0, 0, 0);
			for(i = 0; i < num_command-1; i++) {
				interrupt(0x21, 0x0, "[",0,0);
				interrupt(0x21, 0x0, commandList[i],0,0);
				interrupt(0x21, 0x0, "]",0,0);
				interrupt(0x21, 0x20, curdir, 0, 0);
				interrupt(0x21, (0xFF << 8) | 0x06, commandList[i], &result, 0);
				interrupt(0x21, (0xFF << 8) | 0x06, "shell\0", 0, 0);
			}
		}else{
			argc = num_command-1;
			for(i = 1; i < num_command; i++) {
				j = i - 1;
				argv[j] = commandList[i];
			}
			interrupt(0x21, 0x20, curdir, argc, argv);
			interrupt(0x21, (0xFF << 8) | 0x06, commandList[0], &result, 0);
		}
		
		if(!result) {
			interrupt(0x21, 0x0, "Command is invalid \n\r",0,0);
		}
	}
}

int strCmp(char *str1, char *str2, int size) {
	int eq = 1;
	int i = 0;

	while((eq == 1) && (i < size)) {
		if(str1[i] != str2[i]) {
			eq = 0;
		}
		i++;
	}
	
	return eq;
}

void copy_Str(char *str1, char *str2, int i) {
	int j = i;
	int k;
	while(str1[i] != '\0') {
		k = i - j;
		str2[k] = str1[i];
		i++;
	}
}

void split_command(char *command, char commandList[16][16], int *num_command) {
	int i;
	int len;
	int j;

	i = 0;
	len = 0;
	j = 0;
	
	while(command[len] != '\0') {
		if (command[len] == ' '){
			commandList[i][j] = '\0';
			j = 0;
			i++;
		}else{
			commandList[i][j] = command[len];
			j++;
		}
		len++;
	}

	*num_command = i+1;
}
		
void cahnge_Current_Direct(char *path, char *curdir, char parentIndex) {
	int i = 0;
	int isEqual;
	char dirs[512];
	
	interrupt(0x21, 0x02, dirs, 257, 0);
	
	while (i*16 < 512) {
		isEqual = strCmp(path,dirs+i*16+1, 15);
		if (isEqual && dirs[i*16] == parentIndex){
		   *curdir = i;
		   break;
		}else{
		   i++;
		}
	}

	if (path[0] == '.' && path[1] == '.' && parentIndex != 0xFF){
		*curdir = dirs[parentIndex*15];
	}
}

void changeRel_ToAbs(char *dir, char *parentIndex, int *success) {
   char dirs[512];
   char currpath[15];
   int i, j, k, isEqual, isLastPathDone, isDirnameDone;
   j = 0;
   isLastPathDone = 0;
   
   interrupt(0x21, 0x02, dirs, 257, 0);
   
   while (!isLastPathDone){
      
      clear(currpath,15);
      k = 0;
      isDirnameDone = 0;
      do{
         if (dir[j+k] == '/'){
            isDirnameDone = 1;
         }else if (dir[j+k] == '\0'){
            isDirnameDone = 1;
            isLastPathDone = 1;
         }else{  
            currpath[k] = dir[j+k];
         }
         k++;
      }while(k < 15 && !isDirnameDone);
      j += k;

      if (!isLastPathDone){
         
         i = 0;
         while (i*16 < 512){
            isEqual = strCmp(currpath,dirs+i*16+1, 15);
            if (isEqual && dirs[i*16] == *parentIndex){
               *parentIndex = i;
               break;
            }else{
               i++;
            }
         }
         
         if (i == 32){
            *success = -1;
            return;
         }   
      }
   }

   clear(dir,15);
   *success = 0;
   for(i = 0; i < 15; i++){
      dir[i] = currpath[i];
      if (currpath[i] == '\0') break;
   }
}

void clear(char *buffer, int length) {
   int i;
   for(i = 0; i < length; ++i) {
      buffer[i] = 0x00;
   }
}
