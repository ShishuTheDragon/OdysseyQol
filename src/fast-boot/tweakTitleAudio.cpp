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

    // If the mode is set to `StopAtTitleScreen`, play the title screen music sooner. This makes it
    // sync up with `SkipTitleScreenAppearAnim`.
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

    // If the mode is set to `Resume1P`, don’t play the title screen music at all. Otherwise, it
    // will only play for a second before the stage fully loads.
    struct DontPlayTitleMusic : TrampolineHook<DontPlayTitleMusic> {
        static void Install() {
            InstallAtSymbol("_ZN2al8startBgmEPKNS_15IUseAudioKeeperEPKcii");
        }
        static void Callback(const al::IUseAudioKeeper* audioKeeper, const char* name, int a,
                             int b) {
            if (name != nullptr && strcmp(name, "Title") == 0)
                return;
            Orig(audioKeeper, name, a, b);
        }
    };
}  // namespace

void qol::impl::tweakTitleAudio(FastBootMode mode) {
    DontPlayTitleCall::Install();
    if (mode == FastBootMode::StopAtTitleScreen)
        PlayTitleMusicSooner::Install();
    else
        DontPlayTitleMusic::Install();
}
