#include "Log.h"
#include "CommChannel.h"

int _write(int fd, char *pBuffer, int size)
{
   return CommChannel_Write(pBuffer, size);
}

int _read(int fd, char *pBuffer, int size)
{
   return CommChannel_Read(pBuffer, size);
}
