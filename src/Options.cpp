#include "Options.h"


Options* Options::instance = NULL;

Options* Options::GetInstance(void)
{
    if (!instance)
        instance = new Options();
    return instance;
}

/* difficulty gestion */
Options::Options()
{
    diff = NORMAL;
    level = 1;
}

string Options::to_string(void)
{
    string l("level " + patch::to_string(level) + " : ");
    switch (diff)
    {
        case EASY:
            return l + STR_EASY;
        case NORMAL:
            return l + STR_NORMAL;
        case HARD:
            return l + STR_HARD;
    }
    return "lolnope";
}

int Options::GetLevel(void)
{
    return level;
}

void Options::SetLevel(int l)
{
    if (l > 0 && l <= MAX_LEVELS)
        level = l;
}

void Options::UpLevel(void)
{
    if (level < MAX_LEVELS)
        level++;
    else
        level = 1;
}

/* return difficulty */
t_difficulty Options::GetDifficulty(void)
{
    return diff;
}

/* set difficulty */
void Options::SetDifficulty(t_difficulty d)
{
    diff = d;
}