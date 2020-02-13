void printString(char *string);
void readString(char *string);
void readSector(char *buffer, int sector);
int mod(int angka1,int angka2);
int div(int angak1, int angka2);
int main () {
    showOSLogo();

    char* hu;
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
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                               ,--.   ,--.                                           ", pos, 0xA); pos += 0xA0;
	printToDisplay("                               \\  /-~-\\  /                                              ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                )' a a `(                                              ", pos, 0xA); pos += 0xA0;
	printToDisplay("                               (  ,---.  )                                           ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                `(_o_o_)'                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                           ", pos, 0xA); pos += 0xA0;
	printToDisplay("                              King of Kings                                        ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;
	printToDisplay("                                                                         ", pos, 0xA); pos += 0xA0;				
}

void printToDisplay(char teks[100], int posisi, int warna){
	int indeks = 0;
	int lokasi = 0xB000;
	while(teks[indeks] != '\0'){
		putInMemory(lokasi, posisi, teks[indeks]);
		posisi++;
		putInMemory(lokasi, posisi, warna);
		posisi++;
		indeks++;
	}
}