#include "Library/Nerve/IUseNerve.h"
#include "Library/Thread/AsyncFunctorThread.h"

class HakoniwaSequence;
namespace al {
    class AsyncFunctorThread;
}

class HakoniwaStateBootLoadData : public al::IUseNerve {
public:
    char unk1[16];
    HakoniwaSequence* mSequence;
    char unk2[225];
    bool mIsLoading;
    al::AsyncFunctorThread* mLoadingThread;
};

static_assert(offsetof(HakoniwaStateBootLoadData, mSequence) == 24);
static_assert(offsetof(HakoniwaStateBootLoadData, mIsLoading) == 257);
static_assert(offsetof(HakoniwaStateBootLoadData, mLoadingThread) == 264);
