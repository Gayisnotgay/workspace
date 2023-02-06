#include "AutoInitRuntimeObject.h"

AutoInitRuntimeObject::AutoInitRuntimeObject(const InitFunc& init)
{
    if (init) {
        init();
    }
}
