#ifndef ENTITY_SOUND_CHANNEL_HH
#define ENTITY_SOUND_CHANNEL_HH

#include "unknown_types.hh"

struct EntitySoundChannels
{
    void ** begin;
    void ** end;
};

struct EntitySoundGameVTable
{
    STRUCT_PAD(0x00, 0x14C);
    EntitySoundChannels const * (*get_channels)(GameObject * game);
};

EC bool func_08008CD0(void * channel);
EC void func_08008B6C(void * channel, u16 sound);

inline void * FindEntitySoundChannel(GameObject * game)
{
    EntitySoundGameVTable const * table = *reinterpret_cast<EntitySoundGameVTable const **>(game);
    EntitySoundChannels const * channels = table->get_channels(game);
    void ** it = channels->begin;
    void ** end = channels->end;
    for (; it != end; ++it)
    {
        if (!func_08008CD0(*it))
            return *it;
    }
    return end[-1];
}

#endif
