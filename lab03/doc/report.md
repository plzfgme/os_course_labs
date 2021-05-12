# 实验三：shell&Interrupt&timer

PB18061414 郭宇韬

> 基于助教发布的 lab2 答案，但在 vga、vsprintf 等部分有修改。

----

## 软件框图及概述

![1](figs/1.png)

myOS 包含了中断、时钟等内容，并与底层硬件交互，为上层应用程序提供了一系列库函数（如打印等）。

用户程序由 main 开始，随后进入 shell 环境。

----

## 主流程及其实现

![2](figs/2.png)

OS首先通过 multiboot 协议启动，为运行 c 语言编译的程序建立了一个堆栈，并初始化了 BSS 段，然后建立了中断向量表，之后转到 osStart，osStart 进行 i8253 等设备的初始化，并开启中断，打印一些提示信息后进入 myMain，myMain 中运行了 startShell 进入 shell 环境，与用户交互，执行结束后（实际上之后会进入一个死循环，不会运行到 shut_down）操作系统运行完毕。

----

## 主要功能模块及其实现

### 中断相关

中断向量表的初始化放置在 *myOS/i386/irq.S* 中，包含 IDT 的分配, 以及在 *start32.S* 中被调用的负责初始化 IDT 的 `set_interrupt`：

![3](figs/3.png)

*myOS/i386/irqs.c* 中有中断处理函数 `ignoreIntBody`，该函数在屏幕左下角显示 `unknown interrupt`，文件中还包括开启和关闭中断的函数 `enable_interrupt` 和 `disable_interrupt`，它们直接用嵌入式汇编 `sti` 和 `cli` 来实现。

另外一个中断处理函数 `tick` 请见下文。

### i8253、i8259A 硬件初始化

*myOS/dev* 中的 *i8253.c* 和 *i8259A.c* 分别有硬件初始化函数 `init8253` 和 `init8259A`，基本上按照 PPT 上的内容使用 `outb` 操作端口进行初始化（其中 i8253 的频率选择了 100Hz），比较简单，就不给出流程图了。

### 时钟相关

*myOS/kernel/tick.c* 包含中断处理函数 `tick`，其每秒（即100次调用，用一个变量 `count` 来计算调用次数）调用一次 `updateWallClock` 来更新墙钟。
、
*myOS/kernel/wallClock.c* 包含了墙钟相关的所有函数，包括设置、读取、更新墙钟，以及用于注册在墙钟更新时执行的函数的函数 `setWallClockHook`，以及一个负责在右下角显示时间的一个钩子函数 `showWallClock`。

该文件内定义了一些全局变量： `hh`、`mm`、`ss` 用于记录当前时间，`funcs` 存储了要被调用的钩子函数的列表。

设置和读取时间的函数直接对对应全局变量进行读取和写入，注册钩子函数的函数也只是将钩子函数放到 `funcs` 而已，比较简单，下面不包含它们的流程图，其他函数的流程图如下：

![4](figs/4.png)

### shell 相关

*userApp/shell.c* 包含了 shell 有关的函数。

`startShell` 用于运行 shell，它调用一个函数 `getCmd` 来获取命令，并在一个存放命令的数组 `cmds` 中搜寻对应命令（另外有一个 `cmdName` 存放命令名用于搜寻）并执行，如此循环往复。

![5](figs/5.png)

`getCmd` 获取命令和参数, 定义了几个全局变量：`cmdBuf` 用于存放读取的命令，argv 用于存放各参数的指针，argc 用于存放参数个数，流程图如下：

![6](figs/6.png)

`registerCmd` 用于注册新的命令，需要三个参数：名称、函数和帮助函数。它将这些参数分别存放在 cmdNames、cmds和helps全局数组中供查找。

最后 `initShell` 用于初始化各种全局变量，并预先用 `register` 添加了 `cmd` 和 `help` 命令。`cmd` 用于打印所有的命令名，`help` 用于调用对应命令的 help 函数，比较简单，直接靠查询全局变量就可以得到。

----

## 源代码说明

### 目录组织

```
.
├── Makefile
├── multibootheader
│   └── multibootHeader.S
├── myOS
│   ├── dev
│   │   ├── i8253.c
│   │   ├── i8259A.c
│   │   ├── Makefile
│   │   ├── uart.c
│   │   └── vga.c
│   ├── i386
│   │   ├── io.c
│   │   ├── irq.S
│   │   ├── irqs.c
│   │   └── Makefile
│   ├── include
│   │   ├── i8253.h
│   │   ├── i8259A.h
│   │   ├── io.h
│   │   ├── irqs.h
│   │   ├── myPrintk.h
│   │   ├── shell.h
│   │   ├── string.h
│   │   ├── tick.h
│   │   ├── type.h
│   │   ├── uart.h
│   │   ├── vga.h
│   │   ├── vsprintf.h
│   │   └── wallClock.h
│   ├── kernel
│   │   ├── Makefile
│   │   ├── tick.c
│   │   └── wallClock.c
│   ├── lib
│   │   ├── Makefile
│   │   └── string.c
│   ├── Makefile
│   ├── myOS.ld
│   ├── osStart.c
│   ├── printk
│   │   ├── Makefile
│   │   ├── myPrintk.c
│   │   └── vsprintf.c
│   └── start32.S
├── source2run.sh
└── userApp
    ├── main.c
    ├── Makefile
    └── shell.c
```

基本上与 PPT 相同，大体上分为 *multibootheader*、*myOS* 和 *userApp* 三个目录，分别负责 multiboot 协议头、操作系统及相关函数和用户应用程序。

**myOS**
*dev*：与硬件相关的函数（文件名表示对应设备）；
*i386*：表示是 i386 特定的，包含中断向量的设置以及各种汇编语言的 c 函数包装；
*printk*：内是打印相关函数；
*kernel*：包含了时间中断处理函数和墙钟；
*lib*：内是各种头文件；
*start32.S* 和 *osStart.c*：OS 的入口。

**userApp**
*main.c" 用户主程序；
"shell.c" shell 相关函数。

### Makefile 组织

主要内容均在根目录的 Makefile 中，其他目录下的 Makefile 主要是为文件位置定义变量。

----

## 代码布局空间说明

使用了自动链接，没有提供 ld 文件。

分为multiboot_header 段、text 段、data 段和 bss 段，multiboot_header 段放置 multiboot 协议头，text 段放置代码，data 段放置已初始化全局或静态局部变量，bss 段放置未初始化全局或静态局部变量。

----

## 编译过程说明

首先把各个 .c 和 .S 文件均编译为 .o 文件，然后链接各个 .o 文件生成 myOS.elf 文件放到 output 目录中。

## 运行和运行结果说明  

在 main.c 中向 shell 注册了一个 `echo` 命令（打印所有的参数）用于测试注册功能。

![7](figs/7.png)

可见时钟和 shell 均实现了预期的功能。
