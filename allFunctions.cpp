// this file contains all the definitions of functions declared in allFunctions.h
#include<bits/stdc++.h>
#include<unistd.h>
#include<stdlib.h>
#include<termios.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<dirent.h>
#include "allFunctions.h"

// clears the screen
#define clr() printf("\033[H\033[J")

// ASCII value of ESC
#define esc 27

using namespace std;

// global variables to be used throughout the program
static struct termios initSettings, newSettings;
static int peek_char = -1;
char const * rootPath;
size_t dirSize = 1024;
char currentDir[1024];

// checks if it's file
bool isFile;

// max items to display in normal mode
#define MAX 10

// struct used by ioctl
struct winsize w;

// dirent is a struct present in dirent.h
vector < dirent * > files;

vector < string > inputVector, searchStack, backStack, forwardStack;

int firstIndex = 0, lastIndex = firstIndex + MAX, cursor = 1, modLine, outputLine, statusLine, inputLine;
int strt=0, end;
FILE * out;

// move cursor to x and y coordinate
void moveCursor(int x, int y) {
        cout << "\033[" << x << ";" << y << "H";
        fflush(stdout);
}

// set the root path on program start
void setRootPath(char
        const * path) {
        rootPath = path;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, & w);
        modLine = w.ws_row - 5;
        outputLine = modLine + 1;
        statusLine = outputLine + 1;
        inputLine = statusLine + 1;
}

// display search results for directory
void printResults() {

        for (auto x: searchStack) {
                // directory to be searched is present
                if (x == inputVector[1]) {
                        printf("\nTrue");
                        return;
                }

        }

        printf("\nFalse");
}

// print the files vector (i.e Similar to ls -l)
void printFiles() {
        clr();
        struct stat fileInfo;
        register uid_t uid;
        register gid_t gid;
        register struct group * g;
        register struct passwd * pw;
        // printf("at: %s\n",currentDir);
        for (int fileIt = firstIndex; fileIt < lastIndex && fileIt < files.size(); fileIt++) {

                // gives all information of file
                lstat(files[fileIt] -> d_name, & fileInfo);

                // permission of a file (r-w-x of user,group and other)
                printf((S_ISDIR(fileInfo.st_mode)) ? "d" : "-");
                printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
                printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
                printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
                printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
                printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
                printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
                printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");
                printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
                printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");

                double size = fileInfo.st_size / 1024.0;
                printf("\t%10.4f KB", size); // size of a file    

                uid = geteuid();
                pw = getpwuid(uid);
                if (pw) {
                        printf("\t%s", pw -> pw_name);
                        printf("\t%s", pw -> pw_name);

                }

                string dt = ctime( & fileInfo.st_mtime);

                printf("\t");

                // displays last modified date of a file
                for (unsigned int i = 0; i < dt.size() - 1; i++)
                        printf("%c", dt[i]);

                // display's file or directory name     
                if ((S_ISDIR(fileInfo.st_mode)))
                        cout << "\t" << files[fileIt] -> d_name;

                else
                        cout << "\t" << files[fileIt] -> d_name;

                cout << "\n";
        }

        moveCursor(modLine, 0);
        cout << "Mode: Normal Mode. Press : to switch to Command Mode";
        return;
}

// set the files vector

void getSetCurrentDir(char const * dir) {
        DIR * dp;
        struct dirent * entry;

        if ((dp = opendir(dir)) == NULL) {
                fprintf(stderr, "Can't open the Directory!\n");
                return;
        }

        chdir(dir); // change working directory to dir
        getcwd(currentDir, dirSize);
        files.clear();
        while ((entry = readdir(dp)) != NULL) {
                files.push_back(entry);
        }
        closedir(dp);
        firstIndex = 0;
        cursor = lastIndex = min(MAX, int(files.size()));
        printFiles();
        moveCursor(cursor, 0);
        return;
}

void initKeyboard() {
        tcgetattr(0, & initSettings);
        newSettings = initSettings;
        newSettings.c_lflag &= ~ICANON;
        newSettings.c_lflag &= ~ECHO;
        newSettings.c_cc[VMIN] = 1;
        newSettings.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, & newSettings);
        return;
}

// For Canonical mode
void closeKeyboard() {
        tcsetattr(0, TCSANOW, & initSettings);
}

int kbHit() {
        char ch;
        int nread;
        if (peek_char != -1) return 1;
        newSettings.c_cc[VMIN] = 0;
        tcsetattr(0, TCSANOW, & newSettings);
        nread = read(0, & ch, 1);
        newSettings.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, & newSettings);
        if (nread == 1) {
                peek_char = ch;
                return 1;
        }
        return 0;
}

int readCh() {
        char ch;
        if (peek_char != -1) {
                ch = peek_char;
                peek_char = -1;
                return ch;
        }
        read(0, & ch, 1);
        return ch;
}

// down by MAX
void L()
{

 if(lastIndex == files.size()) return;
 	
 if(firstIndex + MAX < files.size())
 {
 	firstIndex += MAX;
 	//cursor = firstIndex;
 	if(lastIndex + MAX <= files.size())
 	{
 		lastIndex = firstIndex + MAX;
 	}
 	
 	else
 	    	lastIndex = files.size();
 	    	
 	printFiles();
 	moveCursor(cursor, 0);
 }
 
}

// up by MAX
void K()
{

 if(firstIndex == 0) return;
 	
 if(lastIndex - MAX >= 0)
 {
 	if(firstIndex - MAX > 0)
 	{
 		firstIndex -= MAX;
 	}
 	else
 	    	firstIndex = 0;
 	    	
 	lastIndex = firstIndex + MAX;
 	//cursor = firstIndex;	
 	printFiles();
 	moveCursor(cursor, 0);
 }
 
}
void scrollUp() {

        // checks if it's possible to Scroll Up
        if (cursor > 1) {
                cursor--;
                moveCursor(cursor, 0);
                return;
        }

        if (firstIndex != 0) {
                firstIndex--;
                lastIndex--;
                printFiles();
                moveCursor(cursor, 0);
        }

}
void scrollDown() {

        if (cursor < files.size() && cursor < MAX) {
                cursor++;
                moveCursor(cursor, 0);
                return;
        }

        if (lastIndex != files.size()) {
                firstIndex++;
                lastIndex++;
                printFiles();
                moveCursor(cursor, 0);
        }

}
void goUp() {

        if (strcmp(currentDir, rootPath) != 0) {
                backStack.push_back(string(currentDir));
                getSetCurrentDir("../");
        }

}
void goHome() {
        // checks if currentDir is not rootDir
        if (strcmp(currentDir, rootPath) != 0) {
                backStack.push_back(string(currentDir));
                getSetCurrentDir(rootPath);
        }

}
void goBack() {
        // checks if we can go back
        if (backStack.size() != 0) {
                string toVisit = backStack.back();
                backStack.pop_back();
                forwardStack.push_back(string(currentDir));
                getSetCurrentDir(toVisit.c_str());
        }

}
void goForward() {

        // checks if we can go forward
        if (forwardStack.size() != 0) {
                string toVisit = forwardStack.back();
                forwardStack.pop_back();
                backStack.push_back(string(currentDir));
                getSetCurrentDir(toVisit.c_str());
        }

}
void openFile() {
        struct stat statbuf;
        char * fileName = files[cursor + firstIndex - 1] -> d_name; // get the entry d_name in files which is a dirent struct
        lstat(fileName, & statbuf); // returns info about fileName

        if (S_ISDIR(statbuf.st_mode)) // checks if it's a directory
        {
                // if user pressed enter on '..' and the currentDir does not have a parent directory 
                if (strcmp(fileName, "..") == 0 && strcmp(currentDir, rootPath) == 0) return;

                // if user pressed enter on . then do nothing
                if (strcmp(fileName, ".") == 0) return;

                // push current dir in backStack
                backStack.push_back(string(currentDir));

                getSetCurrentDir((string(currentDir) + '/' + string(fileName)).c_str());
        } else {
                // if it's not a directory the open the file using the default software available on the system
                pid_t pid = fork();

                if (pid == 0) {
                        execl("/usr/bin/xdg-open", "xdg-open", fileName, NULL);
                        exit(1);
                }
        }
        return;
}

// clears CMD of command mode 
void clrCMD() {
        moveCursor(modLine, 0);

        int i = modLine, j = 0;
        while (i < w.ws_row) {

                while (j < w.ws_col) {
                        cout << " ";
                        j++;
                }
                i++;
        }

        moveCursor(modLine, 0);

}

// splits the input of command mode by space
void fillInputVector(string input) {
        inputVector.clear();

        string temp;
        bool backFound = false;
        for (char c: input) {
                if (backFound) {
                        backFound = false;
                        continue;
                } else if (c == ' ') {
                        inputVector.push_back(temp);
                        temp = "";
                } else if (c == 92) {
                        temp += " ";
                        backFound = true;
                } else {
                        temp.push_back(c);

                }

        }

        if (temp.size())
                inputVector.push_back(temp);

}

void copyFile(string fileName, string dest) {
        char block[1024];
        int in , out, nread; in = open(fileName.c_str(), O_RDONLY);
        out = open((dest + '/' + fileName).c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        while ((nread = read( in , block, sizeof(block))) > 0) {
                write(out, block, nread);
        }
}

int deleteFile(string filePath) {
        int x = remove(filePath.c_str());
        return x;
}

void copyDir(string dir, string dest) {
        DIR * dp;
        struct dirent * entry;
        struct stat statbuf;

        if ((dp = opendir(dir.c_str())) == NULL) {
                printf("Can't open the directory: %s", dir.c_str());
                return;
        }
        chdir(dir.c_str()); //  i m in a
        while ((entry = readdir(dp)) != NULL) {

                lstat(entry -> d_name, & statbuf);

                // checks if it's a directory
                if (S_ISDIR(statbuf.st_mode)) {

                        if (strcmp(".", entry -> d_name) != 0 and strcmp("..", entry -> d_name) != 0) {
                                mkdir((dest + '/' + entry -> d_name).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                                copyDir(entry -> d_name, dest + '/' + entry -> d_name);
                        }

                }

                // else it's a file 
                else {
                        copyFile(entry -> d_name, dest);
                }
        }
        chdir("..");
        closedir(dp);
        return;
}

void delDir(string dir) {
        DIR * dp;
        struct dirent * entry;
        struct stat statbuf;

        if ((dp = opendir(dir.c_str())) == NULL) {
                fprintf(stderr, "Can't open the directory: %s\n", dir.c_str());
                return;
        }
        chdir(dir.c_str()); //  i m in a
        while ((entry = readdir(dp)) != NULL) {
                lstat(entry -> d_name, & statbuf);
                if (S_ISDIR(statbuf.st_mode)) {
                        if (strcmp(".", entry -> d_name) != 0 and strcmp("..", entry -> d_name) != 0) {
                                delDir(entry -> d_name);
                                rmdir(entry -> d_name);
                        }

                } else {
                        unlink(entry -> d_name);
                }
        }
        chdir("..");
        closedir(dp);
}

void searchFiles(string fileName, string dir) {
        DIR * dp;
        struct dirent * entry;
        struct stat statbuf;

        //char *fileN = fileName;
        if ((dp = opendir(dir.c_str())) == NULL) {
                fprintf(stderr, "Can't open the directory: %s\n", dir.c_str());
                return;
        }
        chdir(dir.c_str()); //  i m in a
        while ((entry = readdir(dp)) != NULL) {
                lstat(entry -> d_name, & statbuf);
                if (S_ISDIR(statbuf.st_mode)) {
                        //cout << entry->d_name <<endl;
                        if (strcmp(".", entry -> d_name) != 0 and strcmp("..", entry -> d_name) != 0) {

                                // push if it's a valid directory				
                                searchStack.push_back(string(entry -> d_name));
                                // cout << string(entry->d_name) << endl;
                                searchFiles(fileName, entry -> d_name);

                        }

                        /*
                        if(strcmp(".",entry->d_name)==0 or strcmp("..",entry->d_name)==0){
                        	continue;
                        }
                        searchFiles(fileName,entry->d_name);
                        */

                } else {
                        if (strcmp(fileName.c_str(), entry -> d_name) == 0) {

                                string a = string(entry -> d_name);
                                if (!isFile and a.size() > 0)
                                        printf("\nTrue");
                                isFile = true;
                                char buf[1000];
                                getcwd(buf, 1000);
                                string toPut = string(buf);
                                int offset = string(rootPath).size();

                                // push it's path if it is a file 
                                searchStack.push_back(toPut.substr(offset) + '/' + string(entry -> d_name));
                        }
                }
        }

        chdir("..");
        closedir(dp);
}

void toggleMode() {
        //closeKeyboard();
        clrCMD();
        cout << "Mode: Command Mode. Press ESC to switch to Normal Mode\n";
        char ch;

        string input = ""; // user input in command mode
        while (1) {
                cout << "\n \n";
                moveCursor(statusLine, 0);
                printf(": ");
                input = "";
                bool first = false;
                ch = '$';
                while ((ch = cin.get()) != esc and ch != 10) {
                        if (first and input.size() == 0) printf(": ");

                        cout << ch;

                        // detects backslash
                        if (ch == 127) {
                                if (input.length() <= 1) {
                                        input = "";
                                        printf("\b \b"); // erases a char
                                        moveCursor(statusLine, 0); // moves cursor back to start
                                } else {
                                        input.pop_back();
                                        printf("\b \b"); // erases a char
                                }
                        } else
                                input += ch;

                        first = true;

                }

                // exit command mode
                if (ch == esc) {
                        getSetCurrentDir(currentDir);
                        moveCursor(cursor, 0);
                        closeKeyboard();
                        break;
                } else {

                        //cout << input <<endl;
                        fillInputVector(input);

                        // search the file
                        if (inputVector[0] == "search") {
                                string fileName = inputVector[1];
                                searchStack.clear();
                                isFile = false;
                                searchFiles(fileName, currentDir);

                                if (!isFile)
                                        printResults();

                                cout << "\n\n"; // add a line space between output and : of next command
                                
                                //printf("\33[2K");
                                /*moveCursor(cursor, 0);
                                moveCursor(cursor, 0);*/

                        }

                        // move directory recursively
                        else if (inputVector[0] == "move_dir") {
                                string src, dest;
                                int size = inputVector.size();
                                if (inputVector[size - 1][0] == '~') {
                                        dest = rootPath + inputVector[size - 1].substr(1);
                                } else if (inputVector[size - 1][0] == '/') {
                                        dest = currentDir + inputVector[size - 1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }
                                for (int i = 1; i < size - 1; i++) {
                                        src = inputVector[i];
                                        mkdir((dest + '/' + src).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                                        copyDir(currentDir + '/' + src, dest + '/' + src);
                                        delDir(src);
                                        rmdir(src.c_str());
                                }
                                clrCMD();
                                moveCursor(statusLine, 0);
                                cout << "\n\n\n\n\n";
                                cout << "Moved Succesfully!";
                        }

                        // delete directory recursively
                        else if (inputVector[0] == "delete_dir") {
                                string dest;
                                if (inputVector[1][0] == '~') {
                                        dest = rootPath + inputVector[1].substr(1);
                                } else if (inputVector[1][0] == '/') {
                                        dest = currentDir + inputVector[1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!\n";
                                        continue;
                                }
                                delDir(dest);
                                rmdir(dest.c_str());
                                clrCMD();
                                moveCursor(statusLine, 0);
                                cout << "\n\n\n\n\n";
                                cout << "\nDeleted Succesfully!";
                                cout << "\n\n\n";
                        }

                        //create file or directory
                        else if (inputVector[0] == "create_file"
                                or inputVector[0] == "create_dir") {
                                string loc;
                                int size = inputVector.size();
                                if (inputVector[size - 1][0] == '~') {
                                        loc = rootPath + inputVector[size - 1].substr(1);
                                } else if (inputVector[size - 1][0] == '/') {
                                        loc = currentDir + inputVector[size - 1];
                                } else if (inputVector[size - 1][0] == '.') {
                                        loc = currentDir;
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }
                                for (int i = 1; i < size - 1; i++) {
                                        string fileName = inputVector[i];
                                        if (inputVector[0] == "create_file") {
                                                open((loc + '/' + fileName).c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                                        } else {
                                                mkdir((loc + '/' + fileName).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                                        }
                                }
                                clrCMD();
                                //printf("\33[2k");
                                moveCursor(statusLine, 0);
                               // printf("\33[2k");
                                cout << "Created Succesfully!";

                        }

                        // rename file
                        else if (inputVector[0] == "rename") {
                                string a = inputVector[1];
                                string b = inputVector[2];
                                rename(a.c_str(), b.c_str());
                                clrCMD();
                                moveCursor(statusLine, 0);
                                cout << "Renamed Succesfully!";
                        }

                        // move a file 
                        else if (inputVector[0] == "move") {
                                string fileName, dest;
                                int size = inputVector.size();
                                if (inputVector[size - 1][0] == '~') {
                                        dest = rootPath + inputVector[size - 1].substr(1);
                                } else if (inputVector[size - 1][0] == '/') {
                                        dest = currentDir + inputVector[size - 1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }
                                for (int i = 1; i < size - 1; i++) {
                                        fileName = inputVector[i];
                                        copyFile(fileName, dest);
                                        deleteFile(currentDir + '/' + fileName);
                                }
                                
                                clrCMD();
                                moveCursor(statusLine, 0);
                                if(deleteFile(currentDir + '/' + fileName)){
                                // cout << "Deleted Succesfully!";
                                } else {
                                        cout << "Deletion Failed!";
                                }
                                printFiles();
                                cout << "Moved Succesfully!";
                        }

                        // goto a specific directory
                        else if (inputVector[0] == "goto") {
                                // checks if target location is home directory of application
                                if (inputVector[1][0] == '/') {
                                        goHome();
                                } else {
                                        if (inputVector[1][0] == '~') {
                                                getSetCurrentDir((rootPath + inputVector[1].substr(1)).c_str());
                                        }
                                }
                                moveCursor(cursor, 0);
                        }

                        // delete a file
                        else if (inputVector[0] == "delete_file") {
                                string filePath;
                                if (inputVector[1][0] == '~') {
                                        filePath = rootPath + inputVector[1].substr(1);
                                } else if (inputVector[1][0] == '/') {
                                        filePath = currentDir + inputVector[1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }
                                clrCMD();
                                moveCursor(statusLine, 0);
                                if (!deleteFile(rootPath + '/' + filePath)) {
                                       // cout << "Deleted Succesfully!";
                                } else {
                                        cout << "Deletion Failed!";
                                }

                        }

                        // copy directory recursively
                        else if (inputVector[0] == "copy_dir") {
                                string dirName, dest;
                                int size = inputVector.size(), i = 1;
                                if (inputVector[size - 1][0] == '~') {
                                        dest = rootPath + inputVector[size - 1].substr(1);
                                } else if (inputVector[size - 1][0] == '/') {
                                        dest = currentDir + inputVector[size - 1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }

                                while (i < size - 1) {
                                        dirName = inputVector[i];
                                        mkdir((dest + '/' + dirName).c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
                                        copyDir(currentDir + '/' + dirName, dest + '/' + dirName);
                                        i++;
                                }
                                clrCMD();
                                moveCursor(statusLine, 0);
                               // cout << "Copied Succesfully!";
                        }

                        // to copy file
                        else if (inputVector[0] == "copy") {
                                string fileName, dest;
                                int size = inputVector.size(), i = 1;
                                if (inputVector[size - 1][0] == '~') {
                                        dest = rootPath + inputVector[size - 1].substr(1);
                                } else if (inputVector[size - 1][0] == '/') {
                                        dest = currentDir + inputVector[size - 1];
                                } else {
                                        clrCMD();
                                        moveCursor(statusLine, 0);
                                        cout << "Invalid Path!";
                                        continue;
                                }

                                while (i < size - 1) {
                                        fileName = inputVector[i];
                                        copyFile(fileName, dest);
                                        i++;
                                }

                                clrCMD();
                                moveCursor(statusLine, 0);
                               // cout << "Copied Succesfully!";
                        }

                        // to detect invalid command
                        else {
                                clrCMD();
                                cout << "\n";
                                moveCursor(statusLine, 0);
                                cout << "Status: Invalid Command!"; // show error on invalid command
                        }

                        cout << "\n\n\n";
                }
        }

        // set terminal settings back to normal mode
        initKeyboard();
        return;
}
