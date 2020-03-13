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