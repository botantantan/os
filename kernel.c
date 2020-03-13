#define Sector_Size 512

void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
void writeSector(char *buffer, int sector);
void findFile(char * parent, char * current, char * filename, int * idx, int * result)
void findFile(char * parent, char * current, char * filename, int * idx, int * result)
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

void readFile(char *buffer, char *path, int *result, char parentIndex) {
    int i = 0;
    char current;
    char dir[SECTOR_SIZE];
    findFile(&parentIndex, &current, path, &i, result);
    readSector(dir, SECTORS_SECTOR);

    if (*result == SUCCESS) {
        char processing = TRUE;
        char * sectors = dir + (current * ENTRY_LENGTH);
        for (i = 0; (i < MAX_SECTORS) && (processing == TRUE); i++){
            if (sectors[i] == 0) processing = FALSE;
            else readSector(buffer + i * SECTOR_SIZE, sectors[i]);
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
void findFile(char * parent, char * current, char * filename, int * idx, int * result) {
    char name[MAX_FILENAME+3];
    char dir[SECTOR_SIZE];
    char file; char found = 0;
    int cnt = 0;
    int i = 0;
    int j;

    /*parent = directory root*/
    if (filename[*idx] == '/')
        *idx++;
    for (i = 0; filename[*idx+i] != '/' && filename[*idx+i] != '\0'; i++)
        name[i] = filename[*idx + i];
    
    /*file akan bernilai 1 jika diakhiri null terminator */
    if (filename[*idx+i] == '\0')
        file = 1;
    else file = 0;
    name[i] = '\0';
    
    /*membaca respective sector*/
    j = i;
    if (!file){
        readSector(dir, DIRS_SECTOR);
        cnt = MAX_DIRS;
    }else{
        readSector(dir, FILES_SECTOR);
        cnt = MAX_FILES; 
    }

    /*mencari directory atau file yang ada pada sektor*/
    i = 0;
    while ((i < cnt) && !found) {
        if ((dir[i * ENTRY_LENGTH] == *parent) && (cmpArray(name, dir + (i * ENTRY_LENGTH) + 1, MAX_FILENAME)))
            found = 1;
        else i++;
    }
    
    if (found){
        *current = i;
        /*file berhasil ditemukan!*/
        if (file)
            *result = SUCCESS;
        else{
            /*melakukan rekursif untuk mencari path dari directorynya*/
            *parent = *current;
            *idx = *idx + j + 1;
            findFile(parent, current, filename, idx, result);
        }
    }else 
        *result = NOT_FOUND;
}
void showOSLogo(){
	int pos = 0x8000;
	printToDisplay("             _____                                                      ", pos, 0xD); pos += 0xA0;
	printToDisplay("            /     \\                                                     ", pos, 0xD); pos += 0xA0;
	printToDisplay("            vvvvvvv  /|__/|                                             ", pos, 0xD); pos += 0xA0;
	printToDisplay("               I   /O,O   |                                             ", pos, 0xD); pos += 0xA0;
	printToDisplay("               I /_____   |      /|/|                                   ", pos, 0xD); pos += 0xA0;
	printToDisplay("              J|/^ ^ ^ \\  |    /00  |    _//|                           ", pos, 0xD); pos += 0xA0;
	printToDisplay("               |^ ^ ^ ^ |W|   |/^^\\ |   /oo |                           ", pos, 0xD); pos += 0xA0;
	printToDisplay("                \\m___m__|_|    \\m_m_|   \\mm_|                           ", pos, 0xD); pos += 0xA0;
	printToDisplay("                                                                        ", pos, 0xD); pos += 0xA0;
	printToDisplay("                Irfan        Asif        Jofi                           ", pos, 0x4); pos += 0xA0;
	printToDisplay("                13517078     13517099    13517135                       ", pos, 0x4); pos += 0xA0;
	printToDisplay("                                                                        ", pos, 0xD); pos += 0xA0;
	printToDisplay("            _        _             ___        ___                       ", pos, 0x2); pos += 0xA0;
	printToDisplay("           | |      | |           |  _|      |_  |                      ", pos, 0x2); pos += 0xA0;
	printToDisplay("           | |_ ___ | |_ ___  _ __| | ___  ___ | |                      ", pos, 0x2); pos += 0xA0;
	printToDisplay("           | __/ _ \\| __/ _ \\| '__| |/ _ \\/ __|| |                      ", pos, 0x2); pos += 0xA0;
	printToDisplay("           | || (_) | || (_) | |  | | (_) \\__ \\| |                      ", pos, 0x2); pos += 0xA0;
	printToDisplay("            \\__\\___/ \\__\\___/|_|  | |\\___/|___/| |                      ", pos, 0x2); pos += 0xA0;
	printToDisplay("                                  |___|      |___|                      ", pos, 0x2); pos += 0xA0;					
}

void printToDisplay(char teks[100], int posisi, int warna){
	int indeks = 0;
	int lokasi = 0xB000;
	while(teks[indeks] != '\0'){
		putInMemory(lokasi, posisi, teks[i]);
		posisi++;
		putInMemory(lokasi, posisi, warna);
		posisi++;
		indeks++;
	}
}
