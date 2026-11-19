# D3 NPC side entities

The two original D3 units are `code_entities_080320DC.s` and
`code_entities_08034CEC.s`. Matching replacements are in `src/npc_side_entities.cc`
and `src/npc_side_batch2.cc` through `src/npc_side_batch7.cc`.
Names describe observed behavior; numbered variants are not verified character IDs.
The carpenter is identified by `gCarpenterScheduleInfo` and its vtable chain.

## Entity families

- **Scheduled NPCs** bind an NPC record, world state, schedule and interaction ID.
  Their refresh/factory hooks select walking/idle animations before making a sprite.
  Some selectors use marriage-candidate IDs, map 29, relationship flags, the active
  roster NPC's age and a time-dependent schedule query (`08034C64`).
- **Moving NPCs** keep a saved location, movement box, animation and countdown.
  Movement choices change facing, duration and velocity; variants provide speed,
  animation and reset hooks. Facing-to-velocity conversion is shared.
- **Reaction NPCs** include arrival-triggered and timed reactions. They choose a
  reaction/animation, create a reaction renderer and advance an optional timer.
- **Launched sprites** store launch state, height and rise speed, plus independent
  planar movement. Actor update and sprite-animation update are separate hooks.
- **Side effects** own transition and particle renderers. Begin/poll/clear helpers
  manage transitions; the renderer's particle pointer is tested at `080390D0`.
- **Sprite workers** bind a `HarvestSprite`, shared work-target slots and a work
  context. They choose work using friendship and task experience, or install a
  random pause/facing. Targets include farm tiles, coop, barn and actor indices.
  `UpdateSpriteWorkerState` (`08033B94`) is the large retained state dispatcher.

## Vtables, lifetime and fields

The common lifecycle is **init → create sprite → refresh/update → destroy**.
Initializers run base construction, install a family vtable and set local state.
Factories allocate a renderer; refresh hooks synchronize state after changes;
update hooks tick movement, schedules, timers or animation. Destruction releases
owned sprites and conditionally deletes the entity according to destructor flags.
`fomt.lds` interleaves exact-address C++/asm sections; vtables are in `asm/vtables.s`.

- `AEntity`: game/services `+00`, map `+04`, Q16 X/Y `+08/+0C`, owned sprite
  `+10`, vtable `+14` (the old compiler's layout, not a modern ABI assumption).
- `AActorEntity`: Q16 velocity `+18/+1C`, facing `+20`, mode `+21`, animation
  `+22`, animation/timer state `+24/+26`, movement flags near `+2E`.
- Scheduled NPC layouts extend the actor with NPC/world references; the world
  pointer used by the animation selectors is `+34`, and schedules begin at `+38`.
- Worker: index `+30`, record `+34`, targets `+38`, context `+3C`, state/pause/
  activity flags `+40/+41/+42`. Target slots have a low-nibble kind and byte indices.
- Effect renderer: transition pointer `+0C`, particle pointer `+10`; it is not an
  `AEntity`. Sprite animators commonly live at renderer `+30` and optional `+70`.
- These are family-specific inferred layouts, not interchangeable universal structs.

## Still in assembly

There are **15** marked routines in `080320DC` and **30** in `08034CEC`, with
**no remaining raw byte spans**. Both former spans are single routines, now fully
disassembled: `08033B94` and `DrawSpriteWorkerEffectLayer` (`08034AE8`).
Batch 8 audited all 55 original entries: each has its own prologue/return, matches
its ROM address, and has no incoming cross-function non-call branch; none needed
folding. Bounds, task maps, family schedules, animation, arrival/proximity logic,
effect targeting and activity destruction now match in C++.
Remaining unnamed symbols have the prefix `func_`; the complete address inventory:

```text
080320DC unit:
0803242C 0803260C 08032690 08032BB4 080330F4 08033584
08033928 08033B24 08033B94 08034180 08034260 0803436C 08034940 08034A14
08034AE8
08034CEC unit:
08035380 08035978 08037118
0803726C 08037494 08037568 0803763C 08037714 080377E8 08037A5C 08037AD0
08037D74 08037F7C 08038110 08038398 080385B0 08038820 08038928 08038A70
08038EE0 08039204 08039310 0803955C 08039708 080398A4
08039A60 08039F90 0803A144 0803A180 0803A394
```

These include schedule traversal, worker dispatch/path/task selection, movement
choosers, reaction/effect rendering and several constructors/destructors. They stay
in asm where attempted C++ differs in instructions/register allocation. Acceptance
is `make -j$(nproc) compare` → `fomt.gba: OK`; no runtime behavior is inferred from
that byte-for-byte check alone.
