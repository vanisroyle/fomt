#include "message_script.hh"

#include <string.h>

struct MessageScriptOutput;
struct MessageScriptInput;
struct MessageScriptSubstitutions;

struct MessageScriptOutputMethods
{
    STRUCT_PAD(0, 0xc);
    u8 (* addGlyph)(MessageScriptOutput *, u32);
    void (* carriageReturn)(MessageScriptOutput *);
    void (* lineFeed)(MessageScriptOutput *);
    void (* clear)(MessageScriptOutput *);
    u8 (* isFull)(MessageScriptOutput *);
};

struct MessageScriptInputMethods
{
    STRUCT_PAD(0, 0xc);
    u8 (* skipDelay)(MessageScriptInput *);
    u8 (* shouldPause)(MessageScriptInput *);
    u8 (* shouldResume)(MessageScriptInput *);
};

struct MessageScriptSubstitutionMethods
{
    STRUCT_PAD(0, 0xc);
    void (* replace)(MessageScriptReplacement *, MessageScriptSubstitutions const *, u32);
};

struct MessageScriptOutput { MessageScriptOutputMethods const * methods; };
struct MessageScriptInput { MessageScriptInputMethods const * methods; };
struct MessageScriptSubstitutions { MessageScriptSubstitutionMethods const * methods; };

struct MessageScriptTick
{
    MessageScriptReplacement replacement;
    volatile u32 skipDelay;
};

struct MessageScript
{
    enum State { Inactive, Running, Waiting };
    State state;
    u16 frameRate;
    u16 framePosition;
    char const * text;
    MessageScriptSubstitutions const * substitutions;
    char const * cursor;
    char replacementText[32];
};

EC SECTION(".text.080ADD44") void ResetMessageScript(MessageScript & script)
{
    script.state = MessageScript::Inactive;
    script.frameRate = 0x100;
    script.framePosition = 0;
    script.text = nullptr;
    script.substitutions = nullptr;
    script.cursor = nullptr;
    script.replacementText[0] = 0;
}

EC SECTION(".text.080ADD5C") void StartMessageScript(MessageScript & script, char const * text, void const * substitutions)
{
    script.text = text;
    script.substitutions = static_cast<MessageScriptSubstitutions const *>(substitutions);
    script.cursor = nullptr;
    script.replacementText[0] = 0;
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

EC SECTION(".text.080ADD78") MessageScript::State UpdateMessageScript(MessageScript & script, MessageScriptOutput * outputArg, MessageScriptInput * inputArg)
{
    register MessageScriptOutput * output asm("r8") = outputArg;
    register MessageScriptInput * input asm("sl") = inputArg;
    register char const * cursor asm("r5");
    register u32 glyph asm("sb");
    asm ("" : "+h" (output), "+h" (input));
    MessageScriptTick tick;
    tick.skipDelay = 0;
    register int frames asm("r6") = AdvanceMessageScriptClock(script);
    asm ("" : "+l" (frames));
    asm volatile ("" : : "X" (&&pause));
    if (script.state == MessageScript::Waiting)
    {
        register MessageScriptInput * gate asm("r1") = input;
        asm ("" : "+l" (gate));
        if (!gate->methods->shouldResume(input))
            frames = 0;
        else
            script.state = MessageScript::Running;
        goto chooseCursor;
    }
    goto checkSkipDelay;

pause:
    script.state = MessageScript::Waiting;
    asm volatile ("" : : : "memory");
    goto finish;

checkSkipDelay:
    {
        u32 skip = 0;
        register MessageScriptInput * gate asm("r1") = input;
        asm ("" : "+l" (gate));
        u32 requested = gate->methods->skipDelay(input);
        if (script.frameRate == 0)
            requested |= 1;
        if (requested)
            skip = 1;
        tick.skipDelay = skip;
    }

chooseCursor:
    cursor = script.cursor;
    if (!cursor)
        cursor = script.text;
    asm ("" : "+l" (cursor));
    glyph = 0;
    asm ("" : "+h" (glyph));
    goto nextByte;

decodeByte:
    {
        unsigned byte = static_cast<u8>(*cursor++);
        void (* outputCommand)(MessageScriptOutput *);
        switch (byte)
        {
        default:
        {
            if (byte <= 0x1f)
                goto nextByte;
            {
                register u32 combined asm("r1") = glyph;
                asm ("" : "+l" (combined));
                glyph = combined | byte;
            }
            asm ("" : "+h" (glyph));
            register MessageScriptOutput * writer asm("r1") = output;
            asm ("" : "+l" (writer));
            if (writer->methods->addGlyph(output, glyph))
            {
                glyph = 0;
                asm ("" : "+h" (glyph));
                goto countFrame;
            }
            register MessageScriptSubstitutions const * dictionary asm("r2") = script.substitutions;
            asm ("" : "+l" (dictionary));
            if (dictionary && !script.cursor)
            {
                register MessageScriptSubstitutionMethods const * methods asm("r1") = dictionary->methods;
                asm ("" : "+l" (methods));
                register MessageScriptReplacement * replacement asm("r0") = &tick.replacement;
                asm ("" : "+l" (replacement));
                methods->replace(replacement, dictionary, glyph);
                if (tick.replacement.found)
                {
                    register u32 emptyGlyph asm("r1") = 0;
                    asm ("" : "+l" (emptyGlyph));
                    glyph = emptyGlyph;
                    asm ("" : "+h" (glyph));
                    strcpy(script.replacementText, tick.replacement.text);
                    script.text = cursor;
                    script.cursor = script.replacementText;
                    cursor = script.cursor;
                    goto nextByte;
                }
                register u32 partialGlyph asm("r0") = glyph;
                asm ("" : "+l" (partialGlyph));
                glyph = partialGlyph << 8;
                asm ("" : "+h" (glyph));
                goto nextByte;
            }
            register u32 partialGlyph asm("r1") = glyph;
            asm ("" : "+l" (partialGlyph));
            glyph = partialGlyph << 8;
            asm ("" : "+h" (glyph));
            goto nextByte;
        }
        case 0:
            if (script.cursor)
            {
                script.cursor = nullptr;
                cursor = script.text;
                goto nextByte;
            }
            cursor = nullptr;
            goto finish;
        case '\r':
        {
            register MessageScriptOutput * writer asm("r1") = output;
            asm ("" : "+l" (writer));
            outputCommand = writer->methods->carriageReturn;
            goto runCommand;
        }
        case '\n':
        {
            register MessageScriptOutput * writer asm("r1") = output;
            asm ("" : "+l" (writer));
            outputCommand = writer->methods->lineFeed;
            goto runCommand;
        }
        case '\f':
        {
            register MessageScriptOutput * writer asm("r1") = output;
            asm ("" : "+l" (writer));
            outputCommand = writer->methods->clear;
            goto runCommand;
        }
        runCommand:
            outputCommand(output);
        countFrame:
            if (frames)
                --frames;
            goto nextByte;
        case 5:
        {
            register MessageScriptInput * gate asm("r1") = input;
            asm ("" : "+l" (gate));
            if (!gate->methods->shouldPause(input))
                goto nextByte;
            goto pause;
        }
        }
    }
nextByte:
    if (cursor && (tick.skipDelay || frames))
    {
        register MessageScriptOutput * writer asm("r1") = output;
        asm ("" : "+l" (writer));
        if (!writer->methods->isFull(output))
            goto decodeByte;
    }
finish:
    if (script.cursor)
        script.cursor = cursor;
    else
    {
        script.text = cursor;
        asm ("" : "+l" (cursor));
        if (!cursor)
            script.state = static_cast<MessageScript::State>(reinterpret_cast<u32>(cursor));
    }
    return script.state;
}
