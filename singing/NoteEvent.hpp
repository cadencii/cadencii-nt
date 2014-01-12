#pragma once

namespace cadencii {
namespace singing {

class IScoreEvent;

class NoteEvent : public IScoreEvent
{
public:
    NoteEvent(std::string const& lyric, int note_number, cadencii::singing::tick_t duration)
        : lyric_(lyric)
        , note_number_(note_number)
        , duration_(duration)
    {}

    virtual ~NoteEvent()
    {}

    std::string lyric() const
    {
        return lyric_;
    }

    int noteNumber() const
    {
        return note_number_;
    }

    cadencii::singing::tick_t duration() const
    {
        return duration_;
    }

private:
    std::string const lyric_;
    int const note_number_;
    cadencii::singing::tick_t const duration_;
};

}
}
