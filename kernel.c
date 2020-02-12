#define Sector_Size 512

void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
int mod(int angka1,int angka2);
int div(int angak1, int angka2);
void clear(char* buffer,int length);
void writeFile(char *buffer, char *filename, int *sectors);
int main () {
  char* hu;
  int i=0;
  putInMemory(0xB000, 0x8000, 'H');
  putInMemory(0xB000, 0x8001, 0xD);
  putInMemory(0xB000, 0x8002, 'a');
  putInMemory(0xB000, 0x8003, 0xD);
  putInMemory(0xB000, 0x8004, 'i');
  putInMemory(0xB000, 0x8005, 0xD);
  printString("1 2 3 4 5");
  readSector(hu,0x01);
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
    printString(buffer);
}

void writeSector(char *buffer, int sector){
    interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

int mod(int angka1, int angka2){
    while(angka1>angka2){
        angka1 -=angka2;
    }
    return angka1;
}

int div(int angka1, int angka2){
    int hasil = 0;
    while(hasil*angka2 >= angka1){
        hasil+=1;
    }
    return hasil-1;
}
void clear(char *buffer, int length){
    int i = 0;
    while(i < length){
        buffer[i] = 0x00;
        i++;
    }
}
void writeFile(char *buffer, char *filename, int *sectors){
    char dirfiles[512];
    char mapfiles[512];
    int i = 0;
    int found = 0;

    readSector(dirfiles,0x02);
    readSector(mapfiles,0x01);
    while(i <16 && found == 0){
        int j = 0;
        while(j<12 && dirfiles[i*j]==0){
            j++;
        }
        if (j==12){
            found = 1;
        }
    }
    
}

