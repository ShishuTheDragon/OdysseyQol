// The hooks in this file automate the pressing of the “Resume” button on the title screen,
// allowing you to get into the game faster.
//
// The first hook overrides `MenuSelectParts::isDecideContinue` to return true, which effectively
// tells the game that the player pressed “Resume.” However, because the “Resume” button on the
// title screen and the “Continue” button on the pause screen are the same button, we need to use
// the second hook to discern the call context.

#include "./impl.h"
#include "exl/hook/trampoline.hpp"
#include "qol/qol.h"
using ::exl::hook::impl::TrampolineHook;

namespace {
    bool gIsOnTitleScreen = false;

    struct AutoSelectResume : TrampolineHook<AutoSelectResume> {
        static void Install() {
            InstallAtSymbol("_ZNK15MenuSelectParts16isDecideContinueEv");
        }
        static bool Callback(class MenuSelectParts* self) {
            if (gIsOnTitleScreen)
                return true;
            return Orig(self);
        }
    };

    struct TrackIsOnTitleScreen : TrampolineHook<TrackIsOnTitleScreen> {
        static void Install() {
            InstallAtSymbol("_ZN14TitleMenuScene7exeMenuEv");
        }
        static void Callback(class TitleMenuScene* self) {
            gIsOnTitleScreen = true;
            Orig(self);
            gIsOnTitleScreen = false;
        }
    };
}  // namespace

void qol::impl::automateTitleScreen(FastBootMode mode) {
    if (mode == FastBootMode::Resume1P) {
        AutoSelectResume::Install();
        TrackIsOnTitleScreen::Install();
    }
}
