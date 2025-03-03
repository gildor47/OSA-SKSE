#include "Papyrus\Papyrus.h"

#include "Game/LocaleManager.h"
#include "Game/Locker.h"
#include "Papyrus\Script.h"
#include "Util.h"

#define BIND(a_method, ...) a_vm->RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)
#include "Papyrus/PapyrusActor.h"
#include "Papyrus/PapyrusCamera.h"
#include "Papyrus/PapyrusControl.h"
#include "Papyrus/PapyrusDatabase.h"
#include "Papyrus/PapyrusFace.h"
#include "Papyrus/PapyrusMetadata.h"
#include "Papyrus/PapyrusObject.h"
#include "Papyrus/PapyrusScenes.h"
#include "Papyrus/PapyrusUtil.h"
#undef BIND

namespace Papyrus {
    void Bind() {
        auto papyrus = SKSE::GetPapyrusInterface();
        if (papyrus) {
            papyrus->Register(PapyrusActor::Bind);
            papyrus->Register(PapyrusCamera::Bind);
            papyrus->Register(PapyrusControl::Bind);
            papyrus->Register(PapyrusDatabase::Bind);
            papyrus->Register(PapyrusFace::Bind);
            papyrus->Register(PapyrusMetadata::Bind);
            papyrus->Register(PapyrusObject::Bind);
            papyrus->Register(PapyrusScenes::Bind);
            papyrus->Register(PapyrusUtil::Bind);
        }
    }

    void Build() {
        PapyrusDatabase::BuildDB();
    }
}  // namespace Papyrus