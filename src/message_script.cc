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

EC SECTION(".text.080ADF40") int AdvanceMessageScriptClock(MessageScript & script)
{
    register MessageScript * state asm("r2") = &script;
    asm ("" : "+l" (state));
    register int frame asm("r0") = state->framePosition;
    asm ("" : "+l" (frame));
    register int previous asm("r5") = frame;
    asm ("" : "+l" (previous));
    frame = state->frameRate;
    asm ("" : "+l" (frame));
    register int position asm("r1") = previous + frame;
    asm ("" : "+l" (position));
    register int elapsedPosition asm("r4") = position;
    asm ("" : "+l" (elapsedPosition));
    frame = position & 0xff;
    register int remainder asm("r3") = 0;
    asm ("" : "+l" (frame), "+l" (remainder));
    if (frame != 0)
        remainder = position;
    state->framePosition = remainder;
    frame = elapsedPosition;
    asm ("" : "+l" (frame));
    register int nextFrame asm("r1") = frame >> 8;
    frame = previous;
    asm ("" : "+l" (frame), "+l" (nextFrame));
    frame = nextFrame - (frame >> 8);
    asm ("" : "+l" (frame));
    return frame;
}
