// The vanilla title screen has a few seconds of animation before it becomes interactable. The
// hooks in this file bypass those animations using three tricks, which are described below.
//
// Trick 1: Manipulate the frame counter in `TitleMenuScene::exeAppear`. Frames 1, 2, and 150 do
// some setup that can’t be skipped, but the other frames are pointless.
//
// Trick 2: Skip to the end of the menu buttons’ appear animation. This is a prerequisite for
// switching to the interactable state.
//
// Trick 3: Once all the setup is done, override `al::isActionEnd` to return true. This is also a
// prerequisite for switching to the interactable state.

#include "./impl.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Scene/TitleMenuScene.h"
#include "exl/hook/trampoline.hpp"
using ::exl::hook::impl::TrampolineHook;

static_assert(sizeof(al::SimpleLayoutAppearWaitEnd) != 0);  // suppress warning

namespace {
    bool gForceIsActionEnd = false;

    struct SkipTitleScreenAppearAnim : TrampolineHook<SkipTitleScreenAppearAnim> {
        static void Install() {
            InstallAtSymbol("_ZN14TitleMenuScene9exeAppearEv");
        }
        static void Callback(const TitleMenuScene* self) {
            auto nerveKeeper = self->getNerveKeeper();
            if (nerveKeeper->mStep == 3) {
                nerveKeeper->mStep = 150;
            }
            if (nerveKeeper->mStep == 151) {
                al::setActionFrame(self->mLayoutMenu, 240.0, "Boot");
                gForceIsActionEnd = true;
            }
            Orig(self);
            gForceIsActionEnd = false;
        }
    };

    struct IsActorActionEnd : TrampolineHook<IsActorActionEnd> {
        static void Install() {
            InstallAtSymbol("_ZN2al11isActionEndEPKNS_9LiveActorE");
        }
        static bool Callback(const al::LiveActor* actor) {
            if (gForceIsActionEnd)
                return true;
            return Orig(actor);
        }
    };

    struct IsLayoutActionEnd : TrampolineHook<IsLayoutActionEnd> {
        static void Install() {
            InstallAtSymbol("_ZN2al11isActionEndEPKNS_16IUseLayoutActionEPKc");
        }
        static bool Callback(const al::IUseLayoutAction* layout, const char* paneName) {
            if (gForceIsActionEnd)
                return true;
            return Orig(layout, paneName);
        }
    };
}  // namespace

void qol::impl::skipTitleScreenAppearAnim() {
    SkipTitleScreenAppearAnim::Install();
    IsActorActionEnd::Install();
    IsLayoutActionEnd::Install();
}
