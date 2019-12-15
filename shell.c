/*Tom OLeary
Jarrod LeComte
shell.c
*/
int stringMatch(char*, char*);

int main(){
	char prompt[4];
	char buffer[512];
	char badCommand[13];
	char commandType[5];
	char commandExecute[5];
	char commandDir[4];
	char commandDel[4];
	char commandCopy[5];
	char commandCreate[7];

	prompt[0] = 'A';
	prompt[1] = ':';
	prompt[2] = '>';
	prompt[3] = '\0';

	commandType[0] = 't';
	commandType[1] = 'y';
	commandType[2] = 'p';
	commandType[3] = 'e';
	commandType[4] = '\0';

	commandExecute[0] = 'e';
	commandExecute[1] = 'x';
	commandExecute[2] = 'e';
	commandExecute[3] = 'c';
	commandExecute[4] = '\0';

	commandDir[0] = 'd';
	commandDir[1] = 'i';
	commandDir[2] = 'r';
	commandDir[3] = '\0';

	commandDel[0] = 'd';
	commandDel[1] = 'e';
	commandDel[2] = 'l';
	commandDel[3] = '\0';

	commandCopy[0] = 'c';
	commandCopy[1] = 'o';
	commandCopy[2] = 'p';
	commandCopy[3] = 'y';
	commandCopy[4] = '\0';

	commandCreate[0] = 'c';
	commandCreate[1] = 'r';
	commandCreate[2] = 'e';
	commandCreate[3] = 'a';
	commandCreate[4] = 't';
	commandCreate[5] = 'e';
	commandCreate[6] = '\0';

	while (1){
		syscall(1, prompt, 0, 0);
		syscall(0, buffer, 30, 0);
	
		if (stringMatch(buffer, prompt) != 1){
			badCommand[0] = 'B';
			badCommand[1] = 'a';
			badCommand[2] = 'd';
			badCommand[3] = ' ';
			badCommand[4] = 'C';
			badCommand[5] = 'o';
			badCommand[6] = 'm';
			badCommand[7] = 'm';
			badCommand[8] = 'a';
			badCommand[9] = 'n';
			badCommand[10] = 'd';
			badCommand[11] = '!';
			badCommand[12] = '\0';
			syscall(1, badCommand, 0, 0);
		}
		if (stringMatch(buffer, commandType) == 1){
			int pointer = 0;
			char fileSize[MAX_FILE_SIZE];
			buffer[pointer] = buffer;
			syscall(0, buffer[pointer], fileSize, 0);
			syscall(1, buffer[pointer], 0, 0);
			buffer++
			pointer++;
		}
		if (stringMatch(buffer, commandExecute) == 1){
			int pointer = 0;
			buffer[pointer] = buffer;
			syscall(4, buffer[pointer], 0x2000, 0);
			buffer++;
			pointer++;
		}
		if (stringMatch(buffer, commandDir) == 1){
			int entry;
			int flag;
			char directoryBuffer[SECTOR_SIZE];
			interrupt(0x21, 2, directoryBuffer, 2, 0);
			for (entry = 0; entry < 16; entry++) {
				char filename[7];
				int pointer;
				int i;
				pointer = entry * 32;
				if (directoryBuffer[pointer] == 0x00){
					continue;
				}
				flag = 0;
				for (i = 0; i < 6; i++){
					if (directoryBuffer[pointer + i] == 0){
						flag = 1;
					}
					if (!flag) {
						filename[i] = directoryBuffer[pointer + i];
					}
					else{
						filename[i] = '\0';
						break;
					}
				}
			syscall(0, filename, 0, 0);
			}
		}
	}
}

int stringMatch(char* a, char* b){
	while (1){
		if (a != b){
			return 0;
		}
	}
}
