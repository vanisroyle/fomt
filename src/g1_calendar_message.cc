#include "g1_roster.hh"

struct MessageWindow;

struct G1CalendarGameState
{
    STRUCT_PAD(0x0000, 0x1CD4);
    G1NpcRoster roster;
};

struct CalendarTextOptions
{
    void const * vtable;
    u8 flags[4];
    char const * name;
};

struct G1CalendarMessageScene
{
    STRUCT_PAD(0x000, 0x008);
    G1CalendarGameState * game;
    u32 reserved_0C;
    MessageWindow * message;
    u32 audio[3];
    STRUCT_PAD(0x020, 0x216);
    i8 message_state;
    u8 reserved_217;
    u16 selected;
    u16 reserved_21A;
    CalendarTextOptions options;
};

EC void * HardwareContext_GetState(void *);
EC void * HardwareContext_GetOamBuffer(void *);
EC void * HardwareContext_GetDisplayRegisters(void *);
EC void * HardwareContext_GetFrameTasks(void *);
EC void * HardwareContext_GetVBlankTasks(void *);
EC u32 UpdateMessageWindow(MessageWindow *, void *, void *, void *, void *, void *, u32 *);
EC void FinishOpeningMessageWindow(MessageWindow *);
EC void HideMessageWindow(MessageWindow *);
EC void SetMessageWindowTextOptions(MessageWindow *, char const *, u16, void const *);
EC u32 GetRosterProposalCandidate(G1NpcRoster const *);
EC char const * GetRosterNpcName(G1NpcRoster const *, u32);
EC char const gUnk_0810497C[];
EC char const gUnk_081049C4[];
EC char const gUnk_081049F8[];
EC char const gUnk_08104A40[];
EC char const gUnk_08104A84[];
EC char const gUnk_08104AC4[];
EC char const gUnk_08104B08[];
EC char const gUnk_08104B4C[];
EC char const gUnk_08104B94[];
EC char const gUnk_08104BD4[];
EC char const gUnk_08104C18[];
EC char const gUnk_08104C54[];
EC char const gUnk_08104C98[];
EC char const gUnk_08104CDC[];
EC char const gUnk_08104D04[];
EC char const gUnk_08104D28[];
EC char const gUnk_08104D58[];
EC char const gUnk_08104D98[];
EC char const gUnk_08104DB8[];
EC char const gUnk_08104DDC[];

static inline u32 * GetCalendarMessageAudio(G1CalendarMessageScene * self)
{
    return self->audio;
}

EC SECTION(".text.g1_080A2940") void UpdateCalendarMessage(G1CalendarMessageScene * self)
{
    MessageWindow * message = self->message;
    void * state = HardwareContext_GetState(self);
    void * oam = HardwareContext_GetOamBuffer(self);
    void * registers = HardwareContext_GetDisplayRegisters(self);
    void * tasks = HardwareContext_GetFrameTasks(self);
    void * vblank = HardwareContext_GetVBlankTasks(self);
    switch (UpdateMessageWindow(message, state, oam, registers, tasks, vblank, GetCalendarMessageAudio(self)))
    {
    case 0:
        if (self->message_state == 0)
        {
            FinishOpeningMessageWindow(self->message);
            self->message_state = 3;
        }
        else
            self->message_state = 2;
        break;
    case 3:
        if (self->message_state == 5)
        {
            HideMessageWindow(self->message);
            self->message_state = 1;
        }
        else if (self->message_state == 3)
        {
            self->message_state = 5;
            switch (self->selected)
            {
            case 0: SetMessageWindowTextOptions(self->message, gUnk_0810497C, 0, &self->options); break;
            case 1: SetMessageWindowTextOptions(self->message, gUnk_081049C4, 0, &self->options); break;
            case 2: SetMessageWindowTextOptions(self->message, gUnk_081049F8, 0, &self->options); break;
            case 3: SetMessageWindowTextOptions(self->message, gUnk_08104A40, 0, &self->options); break;
            case 4: SetMessageWindowTextOptions(self->message, gUnk_08104A84, 0, &self->options); break;
            case 5: SetMessageWindowTextOptions(self->message, gUnk_08104AC4, 0, &self->options); break;
            case 6: SetMessageWindowTextOptions(self->message, gUnk_08104B08, 0, &self->options); break;
            case 7: SetMessageWindowTextOptions(self->message, gUnk_08104B4C, 0, &self->options); break;
            case 8: SetMessageWindowTextOptions(self->message, gUnk_08104B94, 0, &self->options); break;
            case 9: SetMessageWindowTextOptions(self->message, gUnk_08104BD4, 0, &self->options); break;
            case 10: SetMessageWindowTextOptions(self->message, gUnk_08104C18, 0, &self->options); break;
            case 11: SetMessageWindowTextOptions(self->message, gUnk_08104C54, 0, &self->options); break;
            case 12: SetMessageWindowTextOptions(self->message, gUnk_08104C98, 0, &self->options); break;
            case 13: SetMessageWindowTextOptions(self->message, gUnk_08104CDC, 0, &self->options); break;
            case 14: SetMessageWindowTextOptions(self->message, gUnk_08104D04, 0, &self->options); break;
            case 15: SetMessageWindowTextOptions(self->message, gUnk_08104D28, 0, &self->options); break;
            case 16: SetMessageWindowTextOptions(self->message, gUnk_08104D58, 0, &self->options); break;
            case 17:
                self->options.name = GetRosterNpcName(&self->game->roster, GetRosterProposalCandidate(&self->game->roster));
                SetMessageWindowTextOptions(self->message, gUnk_08104D98, 0, &self->options);
                break;
            case 18: SetMessageWindowTextOptions(self->message, gUnk_08104DB8, 0, &self->options); break;
            default: SetMessageWindowTextOptions(self->message, gUnk_08104DDC, 0, &self->options); break;
            }
        }
        break;
    case 1: case 2: case 4: case 5: case 6:
        break;
    }
}
