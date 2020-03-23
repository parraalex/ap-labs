        #include <stdio.h>
        #include <fcntl.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <string.h>
        #include <stdlib.h>


        #define REPORT_FILE "packages_report.txt"
        #define PACKAGES_ARRAY_SIZE 800 //this number was assigned by viewing how many installed operations were on the txt, but if needed it can change.

        void analizeLog(char *logFile, char *report);

        //struct for storing data about the package
        struct Package
        {
            char *name, *installDate, *updateDate, *removeDate;
            int numUpdates, removed;
        };

        //function that simplifies writing function.
        void writeToFile(int fd, char *txt) {
            
            char *txtSpace = malloc(sizeof(char)*100);
            sprintf(txtSpace, "%s\n",txt);
            int n = strlen(txtSpace);
            write(fd, txtSpace, n);
        }

        //function that reads a line and returns a char** with important data: date, useless source, action and name.
        char ** analizeLine(int fileDescriptor){
            char **sections;//array containing date, useless source, action performed and name of package.
            int currentItem = 0;
            int pointer = 0;
            char tmpBuffer[1];

            //assign memory for the final array of elements, in this case 4 as outer and 200 inner as a secure number
            sections = malloc(sizeof(char*)*200);
            for(int i = 0; i < 200; i++){
                sections[i] = malloc(sizeof(int*)*200);
            }

            while (currentItem < 5) {
                if (read(fileDescriptor, tmpBuffer, 1) == 1){
                        if(tmpBuffer[0] == ' ' && currentItem < 4){//if it is a space
                            if (currentItem == 0 && pointer < 15) {//if the space is the one in the middle of the date section
                                sections[currentItem][pointer] = tmpBuffer[0];
                                pointer++;
                            }
                            else{
                                pointer = 0;
                                currentItem++;
                            }
                        } 
                        else if(tmpBuffer[0] == '\n'){
                            currentItem = 5;
                        }
                        else {
                            sections[currentItem][pointer] = tmpBuffer[0];
                            pointer++;
                        }
                }
                else {
                    sections[0][0] = ' ';
                    return sections;
                }
            }
            return sections;
        }


        //searchs if a name is installed in the database already
        int searchInstalled(int numInstalled, char *name, struct Package *pkg){
            if (numInstalled == 0) {
                return -1;
            }
            else {
                for (int i = 0; i < numInstalled; i++){
                    if(strcmp(name, pkg[i].name) == 0){
                    return i;
                    }
                }
            }

            return -1;
        }

        char * formatDate(char *date){
            char* substr = malloc(16);
            strncpy(substr, date+1, 16);
            return substr;
        }

        void analizeLog(char *logFile, char *report) {
            printf("Generating Report from: [%s] log file\n", logFile);
            int fd;
            int numLines = 0;
            struct Package pkg[PACKAGES_ARRAY_SIZE];
            char **lineRead;
            int numInstalled, numRemoved, numUpgraded, numCurrent, numArray = 0;
        
            if ((fd = open(logFile, O_RDONLY)) < 0 ){
                printf("file opening error: not found");
                return;
            }

            lineRead = analizeLine(fd);
            while (lineRead[0][0] != ' '){//while the function returns a valid line
                if (strcmp(lineRead[2],"installed") == 0){
                    int n = searchInstalled(numArray, lineRead[3], pkg);
                    if(n != -1){
                        //reinstall after being deleted in same location
                        pkg[n].installDate = formatDate(lineRead[0]);
                        pkg[n].updateDate = "-";
                        pkg[n].removeDate = "-";
                        pkg[n].numUpdates = 0;
                        pkg[n].removed = 0;
                        numCurrent++;
                        numRemoved--;
                    }
                    else {
                        //fresh install
                        pkg[numArray].name = lineRead[3];
                        pkg[numArray].installDate = formatDate(lineRead[0]);
                        pkg[numArray].updateDate = "-";
                        pkg[numArray].removeDate = "-";
                        pkg[numArray].numUpdates = 0;
                        pkg[numArray].removed = 0;
                        numInstalled++;
                        numCurrent++;
                        numArray++;
                    }
                }
                else if (strcmp(lineRead[2],"upgraded") == 0){
                    //updates the counter and date
                    int n = searchInstalled(numArray, lineRead[3], pkg);
                    pkg[n].updateDate = formatDate(lineRead[0]);
                    if(pkg[n].numUpdates<1){
                        numUpgraded++;
                    }
                    pkg[n].numUpdates++;
                    
                }
                else if (strcmp(lineRead[2],"removed") == 0) {
                    //adds removal date and adds counters
                    int n = searchInstalled(numArray, lineRead[3], pkg);
                    pkg[n].removeDate = formatDate(lineRead[0]);
                    numRemoved++;
                    numCurrent--;
                    if(pkg[n].numUpdates < 0){
                        numUpgraded--;
                    }
                    pkg[n].removed = 1;
                }
                
                numLines++;
                lineRead = analizeLine(fd);               
            }
            close(fd);


            //-----------------PHASE 2: OPENING NEW REPORT AND WRITING-----------------------
            
            //we create and open the file
            int reportFileDescriptor;
            int code = 0;
            //create a temporal buffer for storing formated text that will be printed
            char *tmpBuffer = malloc(sizeof(char)*100);

            if ((reportFileDescriptor = open(REPORT_FILE, O_WRONLY | O_CREAT, 0640)) < 0 ){
                printf("file opening error: couldnt create");
                return;
            }
            writeToFile(reportFileDescriptor, "Pacman Packages Report");
            writeToFile(reportFileDescriptor, "----------------------");

            sprintf(tmpBuffer, "- Installed packages : %d", numInstalled);
            writeToFile(reportFileDescriptor, tmpBuffer);

            sprintf(tmpBuffer, "- Removed packages : %d", numRemoved);
            writeToFile(reportFileDescriptor, tmpBuffer);

            sprintf(tmpBuffer, "- Upgraded packages : %d", numUpgraded);
            writeToFile(reportFileDescriptor, tmpBuffer);

            sprintf(tmpBuffer, "- Current installed : %d", numCurrent);
            writeToFile(reportFileDescriptor, tmpBuffer);

            writeToFile(reportFileDescriptor, " ");
            writeToFile(reportFileDescriptor, "List of packages");
            writeToFile(reportFileDescriptor, "----------------");
            
            for(int i = 0; i < numArray; i++) {
                sprintf(tmpBuffer, "- Package Name        : %s", pkg[i].name);
                writeToFile(reportFileDescriptor, tmpBuffer);

                sprintf(tmpBuffer, "  - Install date      : %s", pkg[i].installDate);
                writeToFile(reportFileDescriptor, tmpBuffer);

                sprintf(tmpBuffer, "  - Last update date  : %s", pkg[i].updateDate);
                writeToFile(reportFileDescriptor, tmpBuffer);

                sprintf(tmpBuffer, "  - How many updates  : %d", pkg[i].numUpdates);
                writeToFile(reportFileDescriptor, tmpBuffer);

                sprintf(tmpBuffer, "  - Removal date      : %s", pkg[i].removeDate);
                writeToFile(reportFileDescriptor, tmpBuffer);
            }
            close(reportFileDescriptor);

            printf("Report is generated at: [%s]", report);

        }

        //main function that runs the analyzer, nothing changed.
        int main(int argc, char **argv) {

            if (argc < 2) {
            printf("Usage:./pacman-analizer.o pacman.log\n");
            return 1;
            }
            analizeLog(argv[1], REPORT_FILE);

            return 0;
        }
