/*Thomas O'Leary
Jarrod LeComte
*/

void printString(char*);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
void makeInterrupt21();
void readFile(char*, char*, int);
void executeProgram(char*);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);

int searchDirectory(char*, char*);


void main() {

	int startVidMem = 0xb800;
	int vidMemOffset = 0x0;
	int white = 0x7;
	char* letters = "Hello World\0";
	char* line[80];
	char buffer[512];
	while(*letters != 0x0) {

		putInMemory(startVidMem, vidMemOffset, *letters);
		vidMemOffset++;
		putInMemory(startVidMem, vidMemOffset, white);
		vidMemOffset++;
		letters++;
	}
	/*
	printString("Enter a line: ");
	readString(line);
	printString(line);
	*/
	/*
	readSector(buffer,30);
	printString(buffer);
	
	makeInterrupt21();
	interrupt(0x21, 1, line, 0, 0);
	interrupt(0x21, 0, line, 0, 0);
	*/
/*
	char buffer[13312];
	int sectorsRead;
	makeInterrupt21();
	interrupt(0x21, 3, "messag", buffer, &sectorsRead);
	if (sectorsRead > 0){
		interrupt(0x21, 0, buffer, 0, 0);
	}
	else{
		interrupt(0x21, 0, "messag not found\r\n", 0, 0);
	}


	makeInterrupt21();
	interrupt(0x21, 4, "tstpr1", 0, 0);
*/
	makeInterrupt21();
	interrupt(0x21, 4, "shell", 0, 0);
	while(1);
}

void printString(char* chars){

	int i = 0;
	
	while(chars[i] != '\0'){
		int ch = chars[i];
			interrupt(0x10, 0xE*256+ch, 0, 0, 0);
		i++;	
	}
	
}

void readString(char* line){
	int enter = 0xd;
	int backsp = 0x8;
	int i = 0;
	while(1){
		int ascii = interrupt(0x16, 0, 0, 0, 0);
		if (ascii == enter){
			line[i] = 0xa;
			i++;
			line[i] = 0x0;
			break;
		}
		else if (ascii == backsp){
			if (i > 1){
				line[i] = 0x0;
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
				i++;
				interrupt(0x10, 0xe*256+0x0, 0, 0, 0);
				i--;
				interrupt(0x10, 0xe*256+0x8, 0, 0, 0);
			}

		}
		else{
			line[i] = ascii;
			interrupt(0x10, 0xe*256+ascii, 0, 0, 0);
			i++;

		}
		
   }


}

void readSector(char* buffer, int sector){

	int ah = 2; //Tells BIOS to read
	int al = 1; // Number of sectors to read
	int bx = buffer; // Address where the data should be stored to
	int ch = 0; // Track number
	int cl = sector + 1; //Relative sector number (sector+1)
	int dh = 0; // Head number
	int dl = 0x80; //Device number (0x80 for hard disk)

	int ax = ah*256+al;
	int cx = ch*256+cl;
	int dx = dh*256+dl;

	interrupt(0x13, ax, bx, cx, dx);


}

void handleInterrupt21(int AX, int BX, int CX, int DX){

	if (AX == 0){
		printString(BX);
	}
	else if (AX == 1){
		readString(BX);
	}
	else if (AX == 2){
		readSector(BX, 30);
	}
	else if (AX > 2){
		printString("Error\0");
	}
	else if (AX == 6){
		writeSector(BX, CX);
	}
	else if (AX == 7){
		deleteFile(BX);
	}
}

void readFile(char* filename, char* buffer, int directory){
	int entryOffset;
	char directoryBuffer[512];
	int bufferPointer = 0;
	int sectorPointer = 0;
	readSector(&directoryBuffer, directory);
	entryOffset = searchDirectory(directoryBuffer, filename);

	if (entryOffset < 0){
		char errorMessage[15];
		errorMessage[0] = 'F';
		errorMessage[1] = 'i';
		errorMessage[2] = 'l';
		errorMessage[3] = 'e';
		errorMessage[4] = ' ';
		errorMessage[5] = 'n';
		errorMessage[6] = 'o';
		errorMessage[7] = 't';
		errorMessage[8] = ' ';
		errorMessage[9] = 'f';
		errorMessage[10] = 'o';
		errorMessage[11] = 'u';
		errorMessage[12] = 'n';
		errorMessage[13] = 'd';
		errorMessage[14] = '\0';
		printString(errorMessage);

		*buffer = '\0'
		return;
	}

	sectorPointer = entryOffset + 6;

	while (directoryBuffer[sectorPointer] != 0){
		readSector(buffer + bufferPointer, directoryBuffer[sectorPointer]);
		bufferPointer += 512;
		sectorPointer++;
	}
}

void executeProgram(char* name){
	char programBuffer[13312];
	int i;
	int segment;

	readFile(name, programBuffer, 2);

	segment = 0x2000;

	for (i = 0; i <= 13312; i++){
		putInMemory(segment, i, programBuffer[i]);
	}
	launchProgram(segment);
	
	interrupt(0x21, 4, bx, 0, 0);
}

void terminate(){
	char shellname[6];
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
	while(1);

}

void writeSector(char* chars, int sector){
	int ax = 6;
	int bx = chars;
	int cx = sector;

	interrupt(0x13, ax, bx, cx, dx);

}

void deleteFile(char* filename){
	int entryOffset;
	int i;
	char dir[512];
	char map[512];
	readSector(dir, 30);
	readSector(map, 1);

	entryOffset = searchDirectory(dir, filename);

	directoryBuffer[entryOffset] = 0;
	for (i = 6; i < 32; i++){
		map[dir[entryOffset + i]] = 0;
		if (dir[entryOffset + i] == 0){
			break;
		}

	}
	writeSector(dir, 2);
	writeSector(map, 1);
}

void writeFile(char* buffer, char* filename, int numberOfSectors){
	char dir[512];
	char map[512];
	
	readSector(dir, 2);
	

}


