/**
 * CurveControlChangeViewSelector.h
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
#ifndef __CurveControlChangeViewSelector_h__
#define __CurveControlChangeViewSelector_h__

#include <QWidget>

namespace cadencii{

    class CurveControlChangeView;

    class CurveControlChangeViewSelector : public QWidget{
        friend class CurveControlChangeView;

        Q_OBJECT

    private:
        CurveControlChangeView *controlChangeView;

        /**
         * @brief スクロールが現在どの位置にあるか。親からの notifyVerticalScroll の呼び出しで更新される
         */
        int top;

    public:
        explicit CurveControlChangeViewSelector( QWidget *parent = 0 );

        ~CurveControlChangeViewSelector();

        void paintEvent( QPaintEvent *e );

        /**
         * @brief このインスタンスを持っているコントロールチェンジビューワを設定する
         * @param controlChangeView コントロールチェンジビューワ
         */
        void setControlChangeView( CurveControlChangeView *controlChangeView );
    };

}
#endif
