#include "../qt/gui/CurveControlChangeView.hpp"

namespace cadencii{

    class CurveControlChangeViewStub : public CurveControlChangeView{
    public:
        explicit CurveControlChangeViewStub( QWidget *parent = 0 ) :
            CurveControlChangeView( parent )
        {
        }

        ~CurveControlChangeViewStub(){
        }

        int getTrackIndex(){
            return trackIndex;
        }

        void callOnMainContentMousePressSlot(QMouseEvent *event) {
            onMainContentMousePressSlot(event);
        }

        int getTrackTabWidthForTest(){
            return getTrackTabWidth();
        }

        int getLaneHeight(){
            return LANE_HEIGHT;
        }
    };

}
