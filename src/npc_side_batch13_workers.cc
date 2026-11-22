#include "entity_actor.hh"
#include "farm.hh"
#include "placeholder.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct WorkerTarget
{
    u8 kind : 4;
    u8 flags : 4;
    u8 index;
    u8 row;
    u8 padding;
    u32 extra;
};

struct WorkerTargetActor;
struct WorkerTargetActorVTable
{
    STRUCT_PAD(0x00, 0x50);
    bool (*needs_brushing)(WorkerTargetActor *);
    bool (*needs_milking)(WorkerTargetActor *);
    bool (*needs_shearing)(WorkerTargetActor *);
};

struct WorkerTargetActor
{
    STRUCT_PAD(0x00, 0x14);
    WorkerTargetActorVTable * vtable;
};

struct WorkerTargetMap;
struct WorkerTargetMapVTable
{
    STRUCT_PAD(0x00, 0x44);
    WorkerTargetActor * (*get_actor)(WorkerTargetMap *, u32);
};

struct WorkerTargetMap
{
    WorkerTargetMapVTable * vtable;
};

inline WorkerTargetActor * GetWorkerTargetActor(WorkerTargetMap * map, u32 index)
{
    register u32 actor_id asm("r1") = index;
    asm("" : "+l"(actor_id));
    return map->vtable->get_actor(map, actor_id);
}

struct WorkerTargetScratch
{
    Placeholder<RucksackItem, u8> item;
    Placeholder<Food> food;
    Placeholder<FieldPlot> plot;
    u8 products[2];
};

inline void * operator new(size_t, RucksackItem * item) { return item; }
inline void * operator new(size_t, Product * product) { return product; }
EC Food * GetWorkerTargetFood(Food * food, RucksackItem const * item) asm("GetFood__C12RucksackItem");
inline Food * ReadWorkerTargetFood(Food * output, RucksackItem const * input)
{
    register Food * food asm("r0") = output;
    register RucksackItem const * item asm("r1") = input;
    asm volatile("bl GetFood__C12RucksackItem" : "+l"(food), "+l"(item) : : "r2", "r3", "r12", "lr", "cc", "memory");
    return food;
}
EC void HarvestWorkerTargetItem(RucksackItem * item, FieldPlot * plot) asm("method_0800A4A4__9FieldPlot");
EC void InitializeWorkerFoodProduct(Product * product, u32 food) asm("__7ProductG4Food");
EC void InitializeWorkerArticleProduct(Product * product, Article article) asm("__7ProductG7Article");

inline RucksackItem * GetWorkerScratchItem(WorkerTargetScratch & scratch)
{
    register RucksackItem * item asm("r0") = reinterpret_cast<RucksackItem *>(&scratch.item);
    asm("" : "+l"(item));
    return item;
}

inline Food * GetWorkerScratchFood(WorkerTargetScratch & scratch)
{
    register Food * food asm("r0") = reinterpret_cast<Food *>(&scratch.food);
    asm("" : "+l"(food));
    return food;
}

EC u32 CanSpriteWorkerCompleteTarget(WorkerTarget * target, Farm * farm, WorkerTargetMap * map) NPC_SIDE_CODE(08033584);
EC u32 CanSpriteWorkerCompleteTarget(WorkerTarget * target, Farm * farm, WorkerTargetMap * map)
{
    WorkerTargetScratch scratch;
    switch (target->kind)
    {
    case 1:
    {
        FieldPlot plot = farm->field.PlotAt(target->index, target->row);
        u32 state = plot.unk_00_08;
        u32 result = 0;
        switch (plot.unk_00_00)
        {
        case 1:
            if (u32(state - 1) <= 3)
            {
                u32 crop = plot.unk_00_02;
                asm("" : "+r"(crop));
                result = (0U - (crop ^ 20)) >> 31;
            }
            break;
        }
        return result;
    }
    case 2:
    {
        FieldPlot const & source = farm->field.PlotAt(target->index, target->row);
        switch (source.unk_00_08)
        {
        case 5:
        {
            *reinterpret_cast<FieldPlot *>(&scratch.plot) = source;
            register FieldPlot * plot asm("r1") = reinterpret_cast<FieldPlot *>(&scratch.plot);
            asm("" : "+l"(plot));
            HarvestWorkerTargetItem(GetWorkerScratchItem(scratch), plot);
            if (!GetWorkerScratchItem(scratch)->IsEmpty())
            {
                switch (GetWorkerScratchItem(scratch)->GetKind())
                {
                case RucksackItem::KIND_FOOD:
                    goto food_product;
                case RucksackItem::KIND_ARTICLE:
                    goto article_product;
                default:
                    goto unavailable;
                }
            }
            break;
        }
        }
    }
    case 0:
    default:
    unavailable:
        return 0;
    food_product:
    {
        register Product * product asm("r4") = reinterpret_cast<Product *>(&scratch.products[0]);
        asm("" : "+l"(product));
        RucksackItem * item = reinterpret_cast<RucksackItem *>(&scratch.item);
        register u8 * food asm("r0") = reinterpret_cast<u8 *>(ReadWorkerTargetFood(GetWorkerScratchFood(scratch), item));
        asm("" : "+l"(food));
        register u32 low asm("r1");
        asm volatile("add %0, sp, %2\nldrb %1, [%0]" : "+l"(food), "=l"(low) : "I"(sizeof(scratch.item)), "m"(scratch.food));
        ++food;
        asm("" : "+l"(food));
        register u32 first_two asm("r0") = u32(*food) << 8;
        asm("" : "+l"(first_two));
        first_two |= low;
        asm("" : "+l"(first_two));
        register u8 * last asm("r1") = reinterpret_cast<u8 *>(&scratch.food) + 2;
        asm("" : "+l"(last));
        register u32 packed asm("r1") = u32(*last) << 16;
        asm("" : "+l"(packed));
        packed |= first_two;
        InitializeWorkerFoodProduct(product, packed);
        u32 price = product->GetPrice();
        register u32 result asm("r0") = (0U - price | price) >> 31;
        asm("" : "+l"(result) : "i"(0));
        return result;
    }
    article_product:
    {
        register Product * product asm("r4") = reinterpret_cast<Product *>(&scratch.products[1]);
        asm("" : "+l"(product));
        InitializeWorkerArticleProduct(product, GetWorkerScratchItem(scratch)->GetArticle());
        u32 price = product->GetPrice();
        register u32 result asm("r0") = (0U - price | price) >> 31;
        asm("" : "+l"(result) : "i"(1));
        return result;
    }
    case 3:
    {
        u32 result = 0;
        Coop * coop = &farm->coop;
        if (coop->GetStoredBushelCount() != 0 && !coop->HasBushelForEnt(target->index))
            result = 1;
        return result;
    }
    case 4:
    {
        u32 result = 0;
        Barn * barn = &farm->barn;
        if (barn->GetStoredBushelCount() != 0 && !barn->HasBushelForStall(target->index))
            result = 1;
        return result;
    }
    case 5:
    {
        u32 result = 0;
        Barn * barn = &farm->barn;
        if (barn->GetStoredBushelCount() != 0 && !barn->HasBushelForPregnancyStall(target->index))
            result = 1;
        return result;
    }
    case 6:
    {
        WorkerTargetActor * actor = GetWorkerTargetActor(map, target->index + 54);
        u32 result = 0;
        if (actor != nullptr && (actor->vtable->needs_brushing(actor) || actor->vtable->needs_milking(actor) || actor->vtable->needs_shearing(actor)))
            result = 1;
        return result;
    }
    case 7:
        GetWorkerTargetActor(map, target->index + 46);
        goto unavailable;
    case 8:
    {
        register u32 actor asm("r1") = reinterpret_cast<u32>(GetWorkerTargetActor(map, target->index + 76));
        asm("" : "+l"(actor));
        register u32 result asm("r0") = (0U - actor | actor) >> 31;
        asm("" : "+l"(result) : "i"(2));
        return result;
    }
    }
    return 0;
}
