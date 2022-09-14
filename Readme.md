# Linux based File Explorer
A file explorer for linux systems that runs on terminal
## Prerequisites

**1. Platform:** Linux 

**2. Software Requirement:**
* G++ compiler
   * **To install G++ :** ```sudo apt-get install g++```

## Steps to run the project
- Open your terminal with present working directory as the project folder. Then run the **make** command to run the makefile.
    ```make```
- Then finally run the project as shown below, 
    ```./main```
## Assumptions
* Default mode is Normal mode and press **:** to switch to Command mode.
* Press **ESC** key to go back to normal mode from command mode.
* Press **q** key from normal mode to exit from program.
* In Normal Mode, assumed the max window size as 10.
* In Command mode, to edit the command use backspace only.
* For files/directories which contain space in name, backslash (\\) is used as delimeter. 
   * For Example :- If file name is "os assignment" (without double quote) then user should enter "os\ assignment" (without double quote).
* Preferable to open terminal in full size. if one record spans over more than one line then navigation won't work correctly.
* if **Enter** is pressed on some file and if there is no default application found then there will be no movement.
