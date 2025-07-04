#include "./impl.h"

void qol::enableFastBoot() {
    qol::impl::endBootScreenEarly();
    qol::impl::skipTitleScreenAppearAnim();
    qol::impl::tweakTitleAudio();
}
