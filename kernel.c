void showLogo(){
	int pos = 0x8000;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                               ,--.   ,--.                                           ", pos, 0xA); pos += 0xA0;
	printLogo("                               \\  /-~-\\  /                                              ", pos, 0xA); pos += 0xA0;
	printLogo("                                )' a a `(                                              ", pos, 0xA); pos += 0xA0;
	printLogo("                               (  ,---.  )                                           ", pos, 0xA); pos += 0xA0;
	printLogo("                                `(_o_o_)'                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                           ", pos, 0xA); pos += 0xA0;
	printLogo("                                  pigOS                                        ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;
	printLogo("                                                                         ", pos, 0xA); pos += 0xA0;				
}

void printLogo(char teks[100], int posisi, int warna){
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
