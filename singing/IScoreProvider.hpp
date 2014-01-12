#pragma once

namespace cadencii {
namespace singing {

class IScoreEvent;

class IScoreProvider
{
public:
    virtual bool next(std::shared_ptr<IScoreEvent> & out_event, cadencii::singing::tick_t & out_delta) = 0;

    virtual ~IScoreProvider()
    {}
};

}
}
