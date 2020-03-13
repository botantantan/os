void readFile(char *buffer, char *path, int *result, char parentIndex) {
    int i = 0;
    char current;
    char dir[512];
    findFile(&parentIndex, &current, path, &i, result);
    readSector(dir, 0x103);

    if (*result == 0) {
        char processing = TRUE;
        char * sectors = dir + (current * 16);
        for (i = 0; (i < 16) && (processing == TRUE); i++){
            if (sectors[i] == 0) processing = FALSE;
            else readSector(buffer + i * 512, sectors[i]);
        }
    }
}
void findFile(char * parent, char * current, char * filename, int * idx, int * result) {
    char name[15+3];
    char dir[512];
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
        readSector(dir, 0x101);
        cnt = 32;
    }else{
        readSector(dir, 0x102);
        cnt = 32; 
    }

    /*mencari directory atau file yang ada pada sektor*/
    i = 0;
    while ((i < cnt) && !found) {
        if ((dir[i * 16] == *parent) && (cmpArray(name, dir + (i * 16) + 1, 15)))
            found = 1;
        else i++;
    }
    
    if (found){
        *current = i;
        /*file berhasil ditemukan!*/
        if (file)
            *result = 0;
        else{
            /*melakukan rekursif untuk mencari path dari directorynya*/
            *parent = *current;
            *idx = *idx + j + 1;
            findFile(parent, current, filename, idx, result);
        }
    }else 
        *result = -1;
}
void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    int dirIndex;
    int i, j, sectorCount;
    char found;
	
    char map[512];
    char files[512];
    char sector[512];
    char sectorBuffer[512];

    readSector(map, 0x100);
    readSector(files, 0x102);
    readSector(sector, 0x103);
	
	i = 0; sectorCount = 0;
	while ((i < 256) && (sectorCount < *sectors)) {
        if (map[i] == "\0")
            sectorCount++;
		i++;
    }
	
    if (sectorCount >= *sectors){
        dirIndex = 0;
	found = FALSE;
        while ((dirIndex < 32) && (found == FALSE)) {
            if (files[dirIndex * 16 + 1] == '\0')
				found = TRUE;
			else
				dirIndex++;
        }
        if (dirIndex < 32) {
            char parent = parentIndex;
            char current;
            int result;
            j = 0;
            findFile(&parent, &current, path, &j, &result);
            if (result != 0) {
                char filename[15 + 1];
                char offset = j;
                char file;
                for(; (path[j] != '\0') && (path[j] != '/'); j++)
                    filename[j - offset] = path[j];
                filename[j - offset] = '\0';
                file = path[j] == '\0';
                if (file) {
                    /*menetapkan parent index*/
                    files[dirIndex * 16] = parent;
                    /*menetapkan nama file / filename*/
                    for (j = 0; filename[j] != '\0'; j++)
                        files[dirIndex * 16 + 1 + j] = filename[j];
                    writeSector(files, 0x102);
                    i = 0; sectorCount = 0;
                    while (i < 256 && sectorCount < *sectors) {
                        if (map[i] == '\0') {
                            map[i] = 0xFF;
                            sector[dirIndex * 16 + sectorCount] = i;
                            clear(sectorBuffer, 512);
                            for (j = 0; j < 512; j++)
                                sectorBuffer[j] = buffer[sectorCount * 512 + j];
                            writeSector(sectorBuffer, i);
                            sectorCount++;
                        }
						i++;
                    }
                    writeSector(map, 0x100);
                    writeSector(sector, 0x103);
                } else
                    *sectors = -1;
            } else
                *sectors = -2;
        } else
            *sectors = -3;
    } else
        *sectors = 0;

}
char cmpArray(char * arr1, char * arr2, int length) {
    int idx = 0;
    char same = 1;
    while (idx < length && same){
        if (arr1[idx] == arr2[idx]) same = 1;
        else same = 0;
        if (same && arr1[idx] == '\0')
            idx = length;
        idx++;
    }
    return same;
}