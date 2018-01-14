#pragma once

typedef enum
{
   LOG_ERROR,
   LOG_INFO,
   LOG_DEBUG
} LOG_SEVERITY;

void Log(LOG_SEVERITY Severity_E, const char* format, ...);

#define Log_Debug(msg, args...) Log(LOG_DEBUG, msg, ##args)
#define Log_Info(msg, args...)  Log(LOG_INFO, msg, ##args)
#define Log_Error(msg, args...) Log(LOG_ERROR, msg, ##args)

