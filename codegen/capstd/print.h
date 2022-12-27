#include <stdio.h>
#include <stdarg.h>

char *convert(unsigned int num, int base)
{
    static char rep[] = "0123456789ABCDEF";
    static char buf[50];
    char *ptr;

    ptr = &buf[49];
    *ptr = '\0';

    do
    {
        *--ptr = rep[num%base];
        num /= base;
    } while (num != 0);

    return (ptr);
}

void print(char* format, ...)
{
    char *traverse, *s;
    unsigned int i;

    va_list arg; va_start(arg, format);

    for (traverse = format; *traverse != '\0'; traverse++) {
        while (*traverse != '%') {putchar(*traverse); traverse++;}

        traverse++;

        switch(*traverse)
        {
            case 'd': i = va_arg(arg, int); putchar(i); break;
            case 'c': i = va_arg(arg, int);
                if (i < 0) {i = -i; putchar('-');}
                puts(convert(i, 10));
                break;
            case 'o': i = va_arg(arg, unsigned int); puts(convert(i, 8)); break;
            case 's': s = va_arg(arg, char*); puts(s); break;
            case 'x': i = va_arg(arg, unsigned int); puts(convert(i, 16)); break;
        }
    }

    va_end(arg);
}