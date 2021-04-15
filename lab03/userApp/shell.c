#include "shell.h"
#include "uart.h"
#include "myPrintk.h"
#include "type.h"
#include "vga.h"
#include "string.h"

char cmdBuf[100] = {};
char *argv[100] = {};
int argc = 0;
void getCmd(void) {
    bool inWord = FALSE;
    int wordCount = 0;
    char *p = cmdBuf;
    char ch = uart_get_char();
    argc = 0;
    while (ch != '\r') {
        if (ch == ' ') {
            if (inWord) {
                inWord = FALSE;
                *p = '\0';
            }  
        } else {
            if (!inWord) {
                inWord = TRUE;
                ++wordCount;
                if (wordCount >= 2) {
                    argv[argc] = p;
                    ++argc;
                }
            }
            *p = ch;
        }
        append_char_to_screen(ch, 7);
        uart_put_char(ch);
        ++p;
        ch = uart_get_char();
    }
    uart_put_chars("\r\n");
    append_char_to_screen('\n', 7);
    *p = '\0';
}

char *cmdNames[1000] = {};
cmdFunc cmds[1000] = {};
helpFunc helps[1000] = {};
int cmdCount = 0;
void registerCmd(char* cmdName, cmdFunc cmd, helpFunc help) {
    cmdNames[cmdCount] = cmdName;
    cmds[cmdCount] = cmdFunc;
    helps[cmdCount] = help;
    ++cmdCount;
}

int returnVal = 0;
void startShell(void) {
    clear_screen();
    while (TRUE)
    {
        myPrintf(7, "$ ");
        getCmd();
        for (int i = 0; i < cmdCount; ++i) {
            if (strcmp(cmdNames[i], cmdBuf) == 0) {
                returnVal = cmds[i](argc, argv);
            }
        }
    }
}

void initShell(void) {
    argc = 0;
    cmdCount = 0;
    returnVal = 0;
}
