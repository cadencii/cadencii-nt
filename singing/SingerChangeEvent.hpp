#pragma

namespace cadencii {
namespace singing {

class IScoreEvent;

class SingerChangeEvent : public IScoreEvent
{
public:
    explicit SingerChangeEvent(size_t singer_index)
        : index_(singer_index)
    {}

    virtual ~SingerChangeEvent()
    {}

    size_t index() const
    {
        return index_;
    }

private:
    size_t const index_;
};

}
}
