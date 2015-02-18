#include "Logger.h"


Logger* Logger::instance = NULL;

/* Get the instance */
Logger* Logger::GetInstance(void)
{
    if (!instance)
        instance = new Logger();
    return instance;
}

 /* Constructor */
Logger::Logger()
{
    logfile.open(LOGFILE, ios::out | ios::app);
}

/* Write the log message in the given file */
void Logger::Write(string message, const string file, const int line)
{
    char t[MAX_LOG_LENGTH];
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime(t, MAX_LOG_LENGTH, "%d/%m/%Y %H:%M:%S", timeinfo);
    cout << "[" << t << "]\t[" << file << ":" << line << "]\t" << message << endl;
    logfile << "[" << t << "]\t[" << file << ":" << line << "]\t" << message << endl;
}

void Logger::Close(void)
{
    logfile.close();
}