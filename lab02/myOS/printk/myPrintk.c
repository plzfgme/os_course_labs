/*
 * 实现myPrint功能 先学习C语言的可变参数内容
 * 需要调用到格式化输出的function（vsprintf）
 */ 
#define va_list char*

extern void append2screen(char *str,int color);
extern void uart_put_chars(char *str);

extern int vsprintf(char *buf, const char *fmt, va_list args);

#define va_start(args, a) (args = (((va_list) (&(a))) + (sizeof (a))))

#define va_end(args) (args = (va_list)0)

char kBuf[400];
int myPrintk(int color,const char *format, ...){
	va_list args;
	int printed;

	va_start(args, format);
	printed = vsprintf(kBuf, format, args);
	va_end(args);
    uart_put_chars(kBuf);
    // append2screen(kBuf, color);
}

char uBuf[400];
int myPrintf(int color,const char *format, ...){
	va_list args;
	int printed;

	va_start(args, format);
	printed = vsprintf(uBuf, format, args);
	va_end(args);

    append2screen(uBuf, color);
}