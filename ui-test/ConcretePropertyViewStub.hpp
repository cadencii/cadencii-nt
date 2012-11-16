#include "../qt/gui/concrete_property_view/ConcretePropertyView.hpp"

class ConcretePropertyViewStub : public cadencii::ConcretePropertyView {
public:
    cadencii::ConcretePropertyValueProxy *getProxy() { return &proxy; }

    void fetchProperty(
            const QtProperty *property,
            VSQ_NS::Event *event, const VSQ_NS::Sequence *sequence) {
        cadencii::ConcretePropertyView::fetchProperty(property, event, sequence);
    }

    const QtProperty *getLyricPhraseProperty() { return lyricPhrase; }
    const QtProperty *getLyricPhoneticSymbolProperty() { return lyricPhoneticSymbol; }
    const QtProperty *getLyricConsonandAdjustmentProperty() { return lyricConsonantAdjustment; }
    const QtProperty *getLyricProtectProperty() { return lyricProtect; }

    const QtProperty *getNoteNumberProperty() { return noteNumber; }
    const QtProperty *getNoteLengthProperty() { return noteLength; }

    const QtProperty *getNotelocationClockProperty() { return notelocationClock; }
    const QtProperty *getNotelocationMeasureProperty() { return notelocationMeasure; }
    const QtProperty *getNotelocationBeatProperty() { return notelocationBeat; }
    const QtProperty *getNotelocationTickProperty() { return notelocationTick; }

    const QtProperty *getVibratoTypeProperty() { return vibratoType; }
    const QtProperty *getVibratoLengthProperty() { return vibratoLength; }
};
