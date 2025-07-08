#pragma once

#include "../../qol.h"

namespace qol::impl {
    void endBootScreenEarly();
    void skipTitleScreenAppearAnim();
    void tweakTitleAudio(FastBootMode mode);
    void automateTitleScreen(FastBootMode mode);
}  // namespace qol::impl
