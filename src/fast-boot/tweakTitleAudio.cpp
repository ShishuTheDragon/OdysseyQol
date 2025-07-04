// Since we’re skipping animations, there’s no longer a big pause before the main menu appears. The
// hooks in this file tweak the audio to fit the new boot sequence. See the individual hooks’
// comments for more info.

#include "../extra-headers/HakoniwaStateBootLoadData.h"
#include "./impl.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Nerve/NerveUtil.h"
#include "Sequence/HakoniwaSequence.h"
#include "exl/hook/trampoline.hpp"
using ::exl::hook::impl::TrampolineHook;

namespace {
    // Don’t play the clip where Mario announces the name of the game. Without the pause before the
    // title screen, this audio clip feels more out of place.
    struct DontPlayTitleCall : TrampolineHook<DontPlayTitleCall> {
        static void Install() {
            InstallAtSymbol("_ZN2al7startSeEPKNS_15IUseAudioKeeperERKN4sead14SafeStringBaseIcEE");
        }
        static void Callback(const al::IUseAudioKeeper* audioKeeper,
                             const sead::SafeString& seName) {
            if (seName == "TitleCall")
                return;
            Orig(audioKeeper, seName);
        }
    };

    // Play the title screen music sooner to make it sync up with `SkipTitleScreenAppearAnim`.
    struct PlayTitleMusicSooner : TrampolineHook<PlayTitleMusicSooner> {
        static void Install() {
            InstallAtSymbol("_ZN25HakoniwaStateBootLoadData7exeMenuEv");
        }
        static void Callback(const HakoniwaStateBootLoadData* self) {
            if (al::isStep(self, 3)) {  // 195 in vanilla
                // This nerve is active in two situations: on the title screen and when loading a
                // different save file. We don’t want to play music in the latter case.
                if (!self->mIsLoading)
                    al::startBgm(self->mSequence, "Title", -1, 0);
            }
            Orig(self);
        }
    };
}  // namespace

void qol::impl::tweakTitleAudio() {
    DontPlayTitleCall::Install();
    PlayTitleMusicSooner::Install();
}
