#include "prelude.h"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct ActivitySpawnSprite;
EC void InitializeActivityRenderSprite(ActivitySpawnSprite *, void *, u32, u32);
EC i8 SelectActivitySpriteAnimationPair(ActivitySpawnSprite *, u32 *, i32);
EC u32 RollEntityMovementRandom(u32);

EC void const * const vtable_unk_080E759C[];
EC void const * const vtable_unk_080E75B4[];
EC void const * const vtable_unk_080E75CC[];
EC void const * const vtable_unk_080E75E4[];
EC void const * const vtable_unk_080E75FC[];
EC void const * const vtable_unk_080E7614[];
EC void const * const vtable_unk_080E762C[];
EC void const * const vtable_unk_080E7644[];
EC void const * const vtable_unk_080E765C[];
EC void const * const vtable_unk_080E7674[];
EC void const * const vtable_unk_080E768C[];
EC void const * const vtable_unk_080E76A4[];

struct ActivitySpawnSprite
{
    u8 renderer[0x4C];
    void const * methods;
    u32 state;
    i16 x, y;
    i32 fixed_x, fixed_y;
    u16 motion_timer;
    u8 motion_phase;
    u8 reserved;

    void Initialize(void * services, u32 animation, u32 mode)
    {
        register void * resources asm("r1") = services;
        asm("" : "+l"(resources));
        InitializeActivityRenderSprite(this, resources, animation, mode);
    }

    void ResetMovement()
    {
        motion_timer = 0;
        motion_phase = 0;
        asm("" : : : "memory");
        fixed_x = u32(i32(x)) << 16;
        fixed_y = u32(i32(y)) << 16;
    }
};

struct ActivityTimedSprite : ActivitySpawnSprite
{
    u32 reserved;
    u32 timer;
    ActivityTimedSprite(void * services, i32 spawn_x, i32 spawn_y)
    {
        Initialize(services, 0x202, 1);
        methods = vtable_unk_080E7644;
        state = 0;
        x = spawn_x;
        y = spawn_y;
        ResetMovement();
        timer = 0;
    }
};

struct ActivityRandomizedSprite : ActivitySpawnSprite
{
    STRUCT_PAD(0x64, 0x78);
    i8 variation;
    u8 previous_variation;
    STRUCT_PAD(0x7A, 0x84);
    u8 timer;
    STRUCT_PAD(0x85, 0x88);
    ActivityRandomizedSprite(void * services)
    {
        Initialize(services, 0x2A4, 1);
        methods = vtable_unk_080E762C;
        state = 0;
        variation = SelectActivitySpriteAnimationPair(this, &state, -1);
        previous_variation = 0xFF;
        ResetMovement();
        timer = 0xF8;
    }
};

struct ActivityPairedSprite : ActivitySpawnSprite
{
    ActivityPairedSprite(void * services)
    {
        Initialize(services, 0x15, 0);
        methods = vtable_unk_080E759C;
        state = 0;
        SelectActivitySpriteAnimationPair(this, &state, -1);
        ResetMovement();
    }
};

struct ActivitySpawnCollection
{
    u32 count;
    ActivitySpawnSprite * sprites[3];
};

struct ActivitySpawnController
{
    STRUCT_PAD(0x00, 0x08);
    ActivitySpawnCollection collection;
    void Append(ActivitySpawnSprite * sprite)
    {
        register ActivitySpawnController * owner asm("r6") = this;
        asm("" : "+l"(owner));
        register ActivitySpawnCollection * storage asm("r1") = &owner->collection;
        asm("" : "+l"(storage));
        u32 count = owner->collection.count;
        if (count <= 2)
        {
            ActivitySpawnSprite ** slot = &storage->sprites[count];
            if (slot != nullptr)
                *slot = sprite;
            ++owner->collection.count;
        }
    }
};

struct PACKED ActivitySpawnChoice
{
    u8 weights[4];
    u8 count;
};
EC ActivitySpawnChoice const gUnk_080F18D2[];
EC ActivitySpawnChoice const gUnk_080F1904[];

EC void CreateRandomActivitySprites(ActivitySpawnController *, void *, i32, u32) NPC_EXTRA_CODE(0803A394);
EC void CreateRandomActivitySprites(ActivitySpawnController * input_self, void * input_services, i32 input_season, u32 input_alternate)
{
    register ActivitySpawnController * self asm("r6") = input_self;
    register void * services asm("r8") = input_services;
    register i32 season asm("r5") = input_season;
    asm("" : "+l"(self), "+h"(services), "+l"(season));
    register u32 alternate_byte asm("r3") = u8(input_alternate);
    asm("" : "+l"(alternate_byte));
    register u32 alternate asm("r9") = alternate_byte;
    asm("" : "+h"(alternate) : "l"(alternate_byte));
    ActivitySpawnChoice const * choices = gUnk_080F1904;
    if (alternate_byte == 0)
        choices = gUnk_080F18D2;
    asm("" : "+r"(choices));
    register i32 four asm("r0") = 4;
    asm("" : "+l"(four));
    register i32 choice_count asm("r10") = four;
    asm("" : "+h"(choice_count));
    register u32 check_alternate asm("r1") = alternate;
    asm("" : "+l"(check_alternate));
    if (check_alternate == 0)
    {
        register i32 ten asm("r2") = 10;
        asm("" : "+l"(ten));
        choice_count = ten;
    }
    asm("" : "+h"(choice_count));
    register u32 random asm("r4") = u16(RollEntityMovementRandom(99));
    asm("" : "+l"(random));
    i32 variant;
    i32 count;
    register u32 column asm("r12");
    {
        register i32 zero_variant asm("r3") = 0;
        asm("" : "+l"(zero_variant));
        variant = zero_variant;
        register i32 zero_count asm("r0") = 0;
        asm("" : "+l"(zero_count));
        count = zero_count;
        column = zero_count;
    }
    asm("" : "+m"(variant), "+m"(count));
    switch (season)
    {
    case 0: { register u32 value asm("r1") = 0; asm("" : "+l"(value)); column = value; break; }
    case 1: { register u32 value asm("r2") = 1; asm("" : "+l"(value)); column = value; break; }
    case 2: { register u32 value asm("r3") = 2; asm("" : "+l"(value)); column = value; break; }
    case 3: { register u32 value asm("r5") = 3; asm("" : "+l"(value)); column = value; break; }
    }
    asm("" : "+r"(column));
    register i32 choice asm("r3") = 0;
    asm("" : "+l"(choice));
    if (choice >= choice_count)
        goto selected;
    if (i16(random) < choices->weights[column])
    {
        variant = choice;
        register i32 first_count asm("r5") = choices->count;
        asm("" : "+l"(first_count));
        count = first_count;
        goto selected;
    }
next_choice:
    {
        register u32 remaining asm("r0") = random - choices[choice].weights[column];
        asm("" : "+l"(remaining));
        random = u16(remaining);
    }
    ++choice;
    if (choice >= choice_count)
        goto selected;
    {
        register i32 comparison asm("r0") = i16(random);
        asm("" : "+l"(comparison));
        register u32 byte_offset asm("r1") = choice * sizeof(ActivitySpawnChoice);
        asm("" : "+l"(byte_offset));
        register ActivitySpawnChoice const * row asm("r2") = reinterpret_cast<ActivitySpawnChoice const *>(byte_offset + reinterpret_cast<u32>(choices));
        asm("" : "+l"(row));
        if (comparison >= row->weights[column])
            goto next_choice;
        variant = choice;
        register i32 row_count asm("r2") = row->count;
        asm("" : "+l"(row_count));
        count = row_count;
    }
selected:
    if (variant != 0 && alternate != 0)
        variant += 9;
    register i32 spawn_count asm("r2") = count;
    asm("" : "+l"(spawn_count));
    if (spawn_count > 1)
    {
        register i32 maximum asm("r0") = spawn_count;
        asm("" : "+l"(maximum));
        maximum -= 1;
        count = RollEntityMovementRandom(maximum) + 1;
    }
    register i32 initial_count asm("r3") = count;
    asm("" : "+l"(initial_count));
    if (initial_count <= 0)
        return;
    do
    {
        register i32 x asm("r5") = RollEntityMovementRandom(392);
        asm("" : "+l"(x));
        x += 184;
        asm("" : "+l"(x));
        i32 y;
        if (x <= 239)
        {
            register i32 random_y asm("r0") = RollEntityMovementRandom(56);
            register i32 base_y asm("r1") = 496;
            asm("" : "+l"(base_y) : "l"(random_y));
            y = random_y + base_y;
        }
        else
        {
            register i32 random_y asm("r0") = RollEntityMovementRandom(48);
            register i32 base_y asm("r2") = 552;
            asm("" : "+l"(base_y) : "l"(random_y));
            y = random_y + base_y;
        }
        asm("" : "+r"(y));
        ActivitySpawnSprite * walking;
        void const * walking_methods;
        switch (variant - 1)
        {
        case 0:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x9B6, 1);
            walking_methods = vtable_unk_080E76A4;
finish_walking:
            walking->methods = walking_methods;
            walking->state = 0;
            walking->x = x;
            walking->y = y;
            walking->ResetMovement();
            self->Append(walking);
            break;
        case 1:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x10, 1);
            walking_methods = vtable_unk_080E768C;
            goto finish_walking;
        case 2:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x3DB, 1);
            walking_methods = vtable_unk_080E7674;
            goto finish_walking;
        case 3:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x806, 1);
            walking_methods = vtable_unk_080E765C;
            goto finish_walking;
        case 4: self->Append(new ActivityTimedSprite(services, x, y)); break;
        case 5: self->Append(new ActivityRandomizedSprite(services)); break;
        case 6:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x87B, 1);
            walking_methods = vtable_unk_080E7614;
            goto finish_walking;
        case 7:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x65C, 1);
            walking_methods = vtable_unk_080E75FC;
            goto finish_walking;
        case 8:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x8F9, 1);
            walking_methods = vtable_unk_080E75E4;
            goto finish_walking;
        case 9:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x9A9, 1);
            walking_methods = vtable_unk_080E75CC;
            goto finish_walking;
        case 10:
            walking = new ActivitySpawnSprite;
            walking->Initialize(services, 0x8FD, 1);
            walking_methods = vtable_unk_080E75B4;
            goto finish_walking;
        case 11: self->Append(new ActivityPairedSprite(services)); break;
        }
        register i32 remaining asm("r5") = count;
        asm("" : "+l"(remaining));
        --remaining;
        count = remaining;
        if (remaining <= 0)
            break;
    } while (true);
}
