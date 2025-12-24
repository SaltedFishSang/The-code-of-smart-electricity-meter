#include "BSP.h"
const char AsciiTab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
/*将16进制数值转换成字符串函数*/
void hexstring_from_data(u8 *data, u8 len, char *output)
{
    const unsigned char *buf = (const unsigned char *)data;
    size_t i, j;
    for (i = j = 0; i < len; ++i)
    {
        char c;
        c = (buf[i] >> 4) & 0xf;
        c = (c > 9) ? c + 'a' - 10 : c + '0';
        output[j++] = c;
        c = (buf[i] & 0xf);
        c = (c > 9) ? c + 'a' - 10 : c + '0';
        output[j++] = c;
    }
}
u16 Hex2Ascii(u8 *p, char *t, u16 len)
{
    u16 i;
    for (i = 0; i < len; i++)
    {
        *(t + i * 2) = AsciiTab[(*(p + i) >> 4) & 0x0F];
        *(t + i * 2 + 1) = AsciiTab[*(p + i) & 0x0F];
    }
    return (len << 1);
}
u16 Ascii2Hex(u8 *p, char *t, u16 len)
{
    u8 i, Ldat, Hdat;
    u16 k;

    for (k = 0; k < len; k++)
    {
        for (i = 0; i < 16; i++)
            if (*(t + (k << 1)) == AsciiTab[i])
                Hdat = (i & 0x0F) << 4;
        for (i = 0; i < 16; i++)
            if (*(t + (k << 1) + 1) == AsciiTab[i])
                Ldat = i & 0x0F;
        *(p + k) = Hdat | Ldat;
    }
    return (len >> 1);
}
u8 Ascii2Dec(char *p)
{
    u8 i;
    for (i = 0; i < 10; i++)
    {
        if (*p == AsciiTab[i])
            break;
    }
    return i;
}
