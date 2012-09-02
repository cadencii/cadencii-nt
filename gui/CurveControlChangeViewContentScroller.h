/**
 * CurveControlChangeViewContentScroller.h
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __CurveControlChangeViewContentScroller_h__
#define __CurveControlChangeViewContentScroller_h__

#include <QScrollArea>

namespace cadencii{

    class CurveControlChangeView;

    class CurveControlChangeViewContentScroller : public QScrollArea{
        Q_OBJECT

    private:
        CurveControlChangeView *controlChangeView;

    public:
        explicit CurveControlChangeViewContentScroller( QWidget *parent = 0 );

        /**
         * @brief このインスタンスを持っているコントロールチェンジビューワを設定する
         * @param controlChangeView コントロールチェンジビューワ
         */
        void setControlChangeView( CurveControlChangeView *controlChangeView );

        void scrollContentsBy( int dx, int dy );
    };

}
#endif
