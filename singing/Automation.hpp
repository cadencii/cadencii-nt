#pragma once

namespace cadencii {
namespace singing {

enum class Automation : int
{
    PITCHBEND = 1,
    DYNAMICS,
    MAX_PREDEFINED = DYNAMICS + 1,
};

}
}
