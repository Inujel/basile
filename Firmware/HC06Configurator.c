#include "HC06Configurator.h"
#include "Time.h"
#include "CommChannel.h"

uint32_t HC06_GetBaudRateValue(HC06_BaudRate rate)
{
   switch (rate)
   {
   case HC06_BaudRate_1200: return 1200;
   case HC06_BaudRate_2400: return 2400;
   case HC06_BaudRate_4800: return 4800;
   case HC06_BaudRate_9600: return 9600;
   case HC06_BaudRate_19200: return 19200;
   case HC06_BaudRate_38400: return 38400;
   case HC06_BaudRate_57600: return 57600;
   case HC06_BaudRate_115200: return 115200;
   default: return 0;
   }
}

static bool GetLine(char* buffer, uint32_t size, uint32_t timeout)
{
   uint32_t read = 0;
   const uint32_t start = Time_Get();
   while (Time_ElapsedSince(start) < timeout && read < size)
   {
      char c;
      if (CommChannel_Read((void*)&c, 1))
      {
         if (c == '\n')
         {
            buffer[read] = 0;
            return true;
         }

         buffer[read++] = c;
      }
   }
   return false;
}

static bool Read(char* buffer, uint32_t size, uint32_t timeout)
{
   uint32_t read = 0;
   const uint32_t start = Time_Get();
   while (Time_ElapsedSince(start) < timeout && read < size)
   {
      read += CommChannel_Read(buffer + read, size - read);
   }
   return size == read;
}

static void Write(const char* buffer)
{
   const uint32_t size = strlen(buffer);
   uint32_t written = 0;

   while (written < size)
   {
      written += CommChannel_Write(buffer + written, size - written);
   }
}

bool HC06_CheckCommunication()
{
   char buffer[2];
   Write("AT");
   return Read(buffer, 2, 2000) && (memcmp(buffer, "OK", 2) == 0);
}

HC06_BaudRate HC06_GuessBaudRate()
{
   for (HC06_BaudRate rate = HC06_BaudRate_1200; rate <= HC06_BaudRate_115200; ++rate)
   {
      CommChannel_SetBaudRate(HC06_GetBaudRateValue(rate));
      if (HC06_CheckCommunication())
         return rate;
   }
   return HC06_BaudRate_Unknown;
}

bool HC06_SetBaudRate(HC06_BaudRate rate)
{
   char command[9], answer[9];

   memcpy(command, "AT+BAUD", 7);
   command[7] = '0' + (uint32_t)rate;
   command[8] = 0;

   Write(command);
   Read(answer, 9, 2000);
   if (memcmp(answer, "OK", 2) != 0)
      return false;

   CommChannel_SetBaudRate(HC06_GetBaudRateValue(rate));
   return true;
}

void HC06_SetName(const char* name)
{
   char answer[28];
   Write("AT+NAME");
   Write(name);
   Read(answer, 28, 2000);
}

void HC06_GetVersion(char* buffer)
{
   Write("AT+VERSION");
   Read(buffer, 32, 2000);
}
