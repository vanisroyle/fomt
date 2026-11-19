#ifndef MESSAGE_SCRIPT_HH
#define MESSAGE_SCRIPT_HH

#include "prelude.h"

struct MessageScriptReplacement
{
    u8 found;
    STRUCT_PAD(1, 4);
    char text[32];
};

struct GameScriptTextVariables;

EC void SetGameScriptTextVariable(GameScriptTextVariables & variables, unsigned index, char const * text);

#endif // MESSAGE_SCRIPT_HH
