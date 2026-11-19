#include "prelude.h"

struct FrameScript
{
    enum State { Inactive, Running };
    State state;
    u16 frameRate;
    u16 framePosition;
    u8 const * commands;
    void const * commandData;
    u8 const * nextCommand;
    bool pending;
};

EC SECTION(".text.080ADD44") void ResetFrameScript(FrameScript & script)
{
    script.state = FrameScript::Inactive;
    script.frameRate = 0x100;
    script.framePosition = 0;
    script.commands = nullptr;
    script.commandData = nullptr;
    script.nextCommand = nullptr;
    script.pending = false;
}

EC SECTION(".text.080ADD5C") void StartFrameScript(FrameScript & script, u8 const * commands, void const * data)
{
    script.commands = commands;
    script.commandData = data;
    script.nextCommand = nullptr;
    script.pending = false;
    script.state = commands ? FrameScript::Running : FrameScript::Inactive;
    script.framePosition = 0;
}

EC SECTION(".text.080ADD74") void SetFrameScriptRate(FrameScript & script, u16 rate)
{
    script.frameRate = rate;
}
