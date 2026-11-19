#include "prelude.h"

struct MessageScript
{
    enum State { Inactive, Running };
    State state;
    u16 frameRate;
    u16 framePosition;
    char const * text;
    void const * substitutions;
    char const * cursor;
    bool pending;
};

EC SECTION(".text.080ADD44") void ResetMessageScript(MessageScript & script)
{
    script.state = MessageScript::Inactive;
    script.frameRate = 0x100;
    script.framePosition = 0;
    script.text = nullptr;
    script.substitutions = nullptr;
    script.cursor = nullptr;
    script.pending = false;
}

EC SECTION(".text.080ADD5C") void StartMessageScript(MessageScript & script, char const * text, void const * substitutions)
{
    script.text = text;
    script.substitutions = substitutions;
    script.cursor = nullptr;
    script.pending = false;
    script.state = text ? MessageScript::Running : MessageScript::Inactive;
    script.framePosition = 0;
}

EC SECTION(".text.080ADD74") void SetMessageScriptRate(MessageScript & script, u16 rate)
{
    script.frameRate = rate;
}
