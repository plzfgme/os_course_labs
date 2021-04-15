#ifndef __SHELL_H__
#define __SHELL_H__

typedef int (*cmdFunc)(int argc, char* argv[]);
typedef void (*helpFunc)(void);

void startShell(void);
void registerCmd(char* cmdName, cmdFunc cmd, helpFunc help);
void getCmd(void);
void initShell(void);

#endif