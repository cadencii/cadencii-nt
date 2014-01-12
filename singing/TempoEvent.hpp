#pragma once

namespace cadencii {
namespace singing {

class IScoreEvent;

class TempoEvent : public IScoreEvent
{
public:
    explicit TempoEvent(double tempo)
        : tempo_(tempo)
    {}

    virtual ~TempoEvent()
    {}

    double tempo() const
    {
        return tempo_;
    }

private:
    double const tempo_;
};

}
}
