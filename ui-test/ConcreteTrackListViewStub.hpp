#include "../qt/gui/ConcreteTrackListView.hpp"

using namespace cadencii;

class ConcreteTrackListViewStub : public ConcreteTrackListView {
public:
    void callOnMousePressSlot(QMouseEvent *event) {
        onMousePressSlot(event);
    }

    int getTrackIndex() {
        return trackIndex;
    }

    int getTrackTabWidthForTest() {
        return getTrackTabWidth();
    }
};
