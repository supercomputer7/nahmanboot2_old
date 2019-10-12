#include "stdstring.h"

bool strncmp(const char *str1, const char *str2,uint16_t len)
{
    int i=0;
    while(i<len)
    {
        if(str1[i] == '\0' && str2[i] == '\0')
            break;
        if(str1[i] != str2[i])
            return false;
        i++;
    }
    return true;
}
void strcopy(char *source, char *dest,uint32_t len)
{
    for(uint32_t i=0; i<len; ++i)
        dest[i] = source[i];
}
uint32_t strlen(const char *str)
{
    char *p = (char*)str;
    uint32_t count = 0;
    while(*p != '\0')
    {
        count++;
        p++;
    }
    return count;
}