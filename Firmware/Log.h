#pragma once

typedef enum
{
   LOG_ERROR,
   LOG_INFO,
   LOG_DEBUG
} LOG_SEVERITY;

void Log(LOG_SEVERITY Severity_E, const char* message);
void Log_Debug(const char* message);
void Log_Info(const char* message);
void Log_Error(const char* message);

