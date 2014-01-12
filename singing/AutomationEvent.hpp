#pragma once

namespace cadencii {
namespace singing {

class IScoreEvent;

class AutomationEvent : public IScoreEvent
{
public:
    AutomationEvent(int type, double value)
        : type_(type)
        , value_(value)
    {}

    virtual ~AutomationEvent()
    {}

    int type() const
    {
        return type_;
    }

    double value() const
    {
        return value_;
    }

private:
    int const type_;
    double const value_;
};

}
}
