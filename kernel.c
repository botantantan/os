void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
// void readFile(char *buffer, char *filename, int *success);
// void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void writeFile(char *buffer, char *filename, int *sectors);
void executeProgram(char *filename, int segment, int *success);

int mod(int angka1,int angka2);
int div(int angak1, int angka2);
int main () {
  char* hu;
  putInMemory(0xB000, 0x8000, 'H');
  putInMemory(0xB000, 0x8001, 0xD);
  putInMemory(0xB000, 0x8002, 'a');
  putInMemory(0xB000, 0x8003, 0xD);
  putInMemory(0xB000, 0x8004, 'i');
  putInMemory(0xB000, 0x8005, 0xD);
  printString("1 2 3 4 5");
  readString(hu);
  printString(hu);
  while (1);
}

void handleInterrupt21 (int AX, int BX, int CX, int DX) {}
void printString(char* string){
    int i;
    i = 0;
    while(string[i]!='\0'){
        interrupt(0x10,0xE00+string[i],0,0,0);
        i++;
    }
}
void readString(char* string){
    char in = interrupt(0x16,0,0,0,0);
    int kursor = 0;
    while(in != '\r'){
        if(in == '\b'){
            if(kursor>0){
                interrupt(0x10,0xE00 + '\b',0,0,0);
                interrupt(0x10,0xE00 + '\0',0,0,0);
                interrupt(0x10,0xE00 + '\b',0,0,0);
                kursor-=1;
            }
        }else{
            string[kursor] = in;
            interrupt(0x10,0xE00 + string[kursor],0,0,0);
            kursor+=1;
        }
        in = interrupt(0x16,0,0,0,0);
    }
    string[kursor] = '\0';
    interrupt(0x10,0xE00+'\n',0,0,0);
    interrupt(0x10,0xe00+'\r',0,0,0);

}

void readSector(char *buffer, int sector){
    interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void readFile(char *buffer, char *filename, int *success){
    int dir[512];
    int found=0;
    int i = 0;
    *success=1;

    readSector(dir,2);
    while (!found && i<16)
    {
        int j = 0;
        while (j< 12 && filename[j]==dir[i*32+j])
        {
            j++;
        }
        if (dir[i*32+j]=='\0' && i<16)found=1;
        i++;
    }
    if (i==16)
    {
        *success=0;
    }
    if (*success)
    {
        int k = 12;
        while(k < 32)
        {
            buffer[k]=dir[i*32+k];
            k++;
        }
        
    }
    
}


void executeProgram(char *filename, int segment, int *success){
    char buff[20];
    int succes;
    int i=0;

    *success=0;
    readFile(buff, filename, &succes);
    if (succes)
    {
        while (i<20)
        {
            putInMemory(segment,i,buff[i]);
        }
        launchProgram(segment);
        *success=1;
    }
    
}

void writeFile(char *buffer, char *filename, int *sectors){
    char buffMap[512], buffDir[512];
    buffDir[0] = 0;
    int i = 0, flag = 1;

    while (flag){
        readSector(buffDir[i], 2);
        if (strlen(buffDir[i] == 0)){
            
        }
    }
    


}

int mod(int angka1, int angka2){
    while(angka1>angka2){
        angka1 -=angka2;
    }
    return angka1;
}

int div(int a, int b){
    return a/b;
}

