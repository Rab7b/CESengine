#pragma once
#include "Registry.h"

namespace ces {
    class System
    {
    protected:
        ces::Registry& reg;

    public:
        System(ces::Registry& r) : reg(r) {}

        inline virtual ~System() = default;

        inline virtual void runIt(ces::Entity& en) = 0;
    };
};