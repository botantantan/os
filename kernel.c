#define Sector_Size 512

void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void readFile(char *buffer, char *filename, int *success);
// void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void writeFile(char *buffer, char *filename, int *sectors);
void executeProgram(char *filename, int segment, int *success);

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

void readFile(char *buffer, char *filename, int *success){
    char dir[512];
    int found=0;
    int i = 0;
    int length;
    *success=1;

    readSector(dir,2);
    length=0;
    while (filename[length]!=0x0)
    {
        length++;
    }
    
    while (found==0 && i<16)
    {
        int j = 0;
        while (j< length && filename[j]==dir[i*32+j])
        {
            j++;
            buffer[j]=filename[j];
        }
        if (j==length)found=1;
        i++;
    }
    if (i>=16)
    {
        *success=0;
    }
    if (*success)
    {
        int j=12;
        int k=0;
        while (dir[i*32+j]!=0x0 && j<32)
        {
            readSector(buffer[k*512],dir[i*32+j]);
            k++;
            j++;
        }
    }
    
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
    char secbuff[512];
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int h = 0;
    int cekSector = 0;
    int tulisSector = 0;
    int found = 0;

    readSector(dirfiles,0x02);
    readSector(mapfiles,0x01);
    while(found == 1&& i <16){
        if(dirfiles[32*i]=='\0'){
            found = 1;
        }
        i+=1;
    }
    if(found == 0){
        printString("Tidak ada ruang");
        return;
    }
    while(h < 512 && cekSector < *sectors){
        if(mapfiles[h] == 0x00){
            cekSector+=1;
        }
        h+=1;

    }
    if(cekSector < *sectors){
        return;
    }else{
        int sek = 0;
        clear(32*i + dirfiles,32);
        while(k < 12 && filename[k] == '\0'){
            dirfiles[32*i+k] = filename[k];
            k+=1;
        }
        while(l < 256 && tulisSector<*sectors){
            if(mapfiles[l]=='\0'){
                dirfiles[i*32 + 32 + tulisSector] = l;
                clear(secbuff,512);
                while(sek < 512){
                    secbuff[sek] = buffer[i*512+sek];
                    sek+=1;
                }
                writeSector(secbuff,l);
                tulisSector+=1;
                l+=1;
            }
            l+=1;
        }
        writeSector(dirfiles,2);
        writeSector(mapfiles,1);

    }

    
}

