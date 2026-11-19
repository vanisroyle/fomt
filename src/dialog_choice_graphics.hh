#ifndef DIALOG_CHOICE_GRAPHICS_HH
#define DIALOG_CHOICE_GRAPHICS_HH

#include "prelude.h"

struct DialogChoiceGraphics;

struct DialogChoiceGraphicsOwner
{
    DialogChoiceGraphics * graphics;
};

EC DialogChoiceGraphicsOwner & InitializeDialogChoiceGraphics(DialogChoiceGraphicsOwner & owner);
EC void DestroyDialogChoiceGraphics(DialogChoiceGraphicsOwner & owner, int destructorFlags);

#endif // DIALOG_CHOICE_GRAPHICS_HH
