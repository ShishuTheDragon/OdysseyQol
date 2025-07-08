#include "./impl.h"

void qol::enableFastBoot(FastBootMode mode) {
    qol::impl::endBootScreenEarly();
    qol::impl::skipTitleScreenAppearAnim();
    qol::impl::tweakTitleAudio(mode);
    qol::impl::automateTitleScreen(mode);
}
