#pragma once

namespace qol {
    enum class FastBootMode {
        // Get to the title screen asap.
        StopAtTitleScreen,
        // Get to the title screen asap then automatically click “Resume.”
        Resume1P,
    };

    // Installs hooks that speed up the boot sequence and (optionally) skip the title screen.
    void enableFastBoot(FastBootMode mode = FastBootMode::StopAtTitleScreen);
}  // namespace qol
