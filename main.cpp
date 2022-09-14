#include<unistd.h>

#include<bits/stdc++.h>

#include<stdlib.h>

#include "allFunctions.h"

// Clear's the terminal screen
#define clrscr() printf("\033[H\033[J")
using namespace std;

int main() {
        clrscr();
        char path[1024];
        size_t size = 1024;

        getcwd(path, size);

        // set the root path for program
        setRootPath(path);

        // displays ls output
        getSetCurrentDir(path);
        clrscr();
        getSetCurrentDir(path);

        // initialise terminal settings to Normal Mode
        initKeyboard();

        char ch = '$';

        while (ch != 'q') {
                if (kbHit()) {
                        ch = readCh(); // take a character as user input and respond instantly

                        if (ch == 65) {
                                // scroll up on pressing up arrow	
                                scrollUp();

                        } else if (ch == 'k') {
                                // scroll up on pressing up arrow	
                                K();

                        } else if (ch == 66) {
                                // scroll down on down arrow
                                scrollDown();

                        } else if (ch == 'l') {
                                // scroll down on down arrow
                                L();

                        } else if (ch == 127) {
                                // go up one level on backspace
                                goUp();

                        } else if (ch == 68) {
                                // go back on left arrow
                                goBack();

                        } else if (ch == 67) {
                                // go forward on right arrow
                                goForward();

                        } else if (ch == 'h' or ch == 'H') {
                                // go to root path of project
                                goHome();

                        } else if (ch == 10) {
                                // opens a directory and file on pressing enter key
                                openFile();

                        } else if (ch == ':') {
                                // switch to command mode on pressing ':' 
                                toggleMode();
                        }

                }
        }

        closeKeyboard(); // restore terminal settings
        clrscr();
       //cout << "Khatam...Taataa Good Bye...Gaya\n";
        exit(0);
}
