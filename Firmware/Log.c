#include "Log.h"
#include "CommChannel.h"
#include <stdio.h>
#include <stdarg.h>
#include "Time.h"

int _write(int fd, char *pBuffer, int size)
{
   return CommChannel_Write(pBuffer, size);
}

int _read(int fd, char *pBuffer, int size)
{
   return CommChannel_Read(pBuffer, size);
}

void Log(LOG_SEVERITY Severity_E, const char* format, ...)
{
   const uint32_t time = Time_Get();
   printf("[%4lu.%03lu DBG] ", time / 1000, time % 1000);
   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);
   printf("\n");
}
