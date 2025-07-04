// In vanilla, the boot screen (the screen that tells you what controllers you can use) appears for
// a minimum of 9 seconds, even if the game loads quicker than that. This hooks gets you past the
// boot screen faster by getting rid of this minimum duration.

#include "../extra-headers/HakoniwaStateBootLoadData.h"
#include "./impl.h"
#include "Library/Nerve/NerveKeeper.h"
#include "exl/hook/trampoline.hpp"
using ::exl::hook::impl::TrampolineHook;

namespace {
    struct EndBootScreenEarly : TrampolineHook<EndBootScreenEarly> {
        static void Install() {
            InstallAtSymbol("_ZN25HakoniwaStateBootLoadData13exeBootLayoutEv");
        }
        static void Callback(const HakoniwaStateBootLoadData* self) {
            // If the game has loaded, then pretend the player has already been staring at the boot
            // screen for the requisite duration.
            if (self->mLoadingThread->isDone())
                self->getNerveKeeper()->mStep = 541;
            Orig(self);
        }
    };
}  // namespace

void qol::impl::endBootScreenEarly() {
    EndBootScreenEarly::Install();
}
