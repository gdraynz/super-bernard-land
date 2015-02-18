#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

#define LOGFILE "logfile.log"
#define MAX_LOG_LENGTH 1024

using namespace std;

class Logger
{
    public:
        static Logger* GetInstance(void);

        void Write(string message, string file, int line);
        void Close(void);

    private:
        static Logger* instance;

        ofstream logfile;

        Logger();
};

/*
** #include "Logger.h"
** logging("truc à log");
*/
#define logging(message) Logger::GetInstance()->Write(message, __FILE__, __LINE__)

#endif