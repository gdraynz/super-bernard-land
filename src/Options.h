#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string>
#include "Utils.h"

using namespace std;

#define STR_EASY "easy"
#define STR_NORMAL "normal"
#define STR_HARD "hard"
#define MAX_LEVELS 3

enum t_difficulty { EASY, NORMAL, HARD };

class Options
{
    public:
        static Options* GetInstance(void);

        t_difficulty GetDifficulty(void);
        void SetDifficulty(t_difficulty d);
        int GetLevel(void);
        void SetLevel(int l);
        void UpLevel(void);
        string to_string(void);

    private:
        static Options* instance;

        t_difficulty diff;
        int level;

        Options();
};

#define OPTIONS Options::GetInstance()

#endif