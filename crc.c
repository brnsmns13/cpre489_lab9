#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "introduceerror.h"

uint8_t crc_gen(uint8_t message[], int nBytes);
uint8_t crc_check(uint8_t message[], uint8_t crc_val);
void crcInit(void);

#define WIDTH  (8 * sizeof(uint8_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0x8811
uint8_t  crcTable[256];

int main(int argc, char **argv)
{
	uint8_t crc_val = 0;
    char data[1024];

    memset(data, 0, sizeof(char) * 1024);

    printf("Enter Data: ");
    fgets(data, 1024, stdin);
    crcInit();
    crc_val = crc_gen(data, strlen(data));
    printf("CRC: 0x%x\n", crc_val);
    data[strlen(data)] = crc_val;
    printf("Introducing error...\n");
    //IntroduceError(data, 0);
    crc_check(data, crc_val);
    return 0;
}

uint8_t crc_gen(uint8_t message[], int nBytes)
{
    uint8_t data;
    uint8_t rem = 0;

    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (rem >> (WIDTH - 8));
        rem = crcTable[data] ^ (rem << 8);
    }

    return rem;
}

uint8_t crc_check(uint8_t message[], uint8_t crc_val)
{

    uint8_t data;
    uint8_t rem = 0;
    int nBytes = strlen(message) - 1;

    for (int byte = 0; byte < nBytes; ++byte)
    {
        data = message[byte] ^ (rem >> (WIDTH - 8));
        rem = crcTable[data] ^ (rem << 8);
    }

    if(crc_val == rem)
    {
    	printf("No Error: 0x%x\n", rem);
    }
    else
    {
    	printf("Error detected: 0x%x != 0x%x\n", crc_val, rem);
    }

    return rem;
}


void crcInit(void)
{
    uint8_t  rem;
    uint8_t  bit;
    int dividend;

    for (dividend = 0; dividend < 256; ++dividend)
    {
        rem = dividend << (WIDTH - 8);
        for (bit = 8; bit > 0; --bit)
        {		
            if (rem & TOPBIT)
            {
                rem = (rem << 1) ^ POLYNOMIAL;
            }
            else
            {
                rem = (rem << 1);
            }
        }
        crcTable[dividend] = rem;
    }
}