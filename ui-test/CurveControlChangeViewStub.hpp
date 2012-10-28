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

        void callOnMousePressSlot( QMouseEvent *event ){
            onMousePressSlot( event );
        }

        int getTrackTabWidthForTest(){
            return getTrackTabWidth();
        }

        int getLaneHeight(){
            return LANE_HEIGHT;
        }
    };

}
