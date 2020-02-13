void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
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