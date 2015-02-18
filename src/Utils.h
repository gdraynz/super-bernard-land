#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <sstream>

// Window consts
#define WINDOWX 640
#define WINDOWY 480
#define MAXVISIONX 100
#define MAXVISIONY 30
#define TILESIZE 20

#define MAINFONT "./fonts/smb3.ttf"
#define DIEFONT "./fonts/test.ttf"

// Event consts
#define QUITAPP -1
#define CONTINUE 1
#define OPENMENU 2
#define STARTGAME 3
#define RESTARTGAME 4

// Colors
#define MONSTERCOLOR { 200, 200, 200, 0 }

#define FLOORCOLOR { 0, 114, 70, 0 }
#define PLANTCOLOR { 0, 51, 0, 0 }

#define RED { 255, 0, 0, 0 }
#define RED2 { 200, 0, 0, 0 }
#define RED3 { 150, 0, 0, 0 }
#define WHITE { 255, 255, 255, 0 }
#define YELLOW { 255, 255, 0, 0 }
#define BLACK { 0, 0, 0, 0 }
#define GREEN { 0, 255, 0, 0 }
#define BLUE { 0, 0, 255, 0 }
#define COULEURCHELOU { 0, 255, 255, 0 }

#define MENUBLUE { 115, 174, 252, 0 }
#define SHADOWBLACK { 0, 0, 0, 0 }
#define SHADOWBLUE { 32, 0, 161, 0 }

// Patch for std::to_string -> patch::to_string
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

#endif