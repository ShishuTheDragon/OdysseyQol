# OdysseyQol

Small quality of life improvements for SMO modders and players.

These tweaks are designed to be easily imported into other mods, but they will also be available in a standalone mod.

### Fast Boot

This tweak gets you to the title screen sooner by skipping unnecessary pauses and animations. This saves about 5–10 seconds each time you launch the game.

To enable it from your own mod, add a call to `qol::enableFastBoot` into your `userMain` method. For example:
```cpp
extern "C" void userMain() {
    exl::hook::Initialize();
    mallow::init::installHooks();

    // ADD THIS:
    qol::enableFastBoot();

    // (all your other custom hooks and logic)
}
```

This tweak also provides an option for skipping the title screen as well, i.e. automatically selecting “Resume.” For example:
```cpp
    qol::enableFastBoot(qol::FastBootMode::Resume1P);
```
