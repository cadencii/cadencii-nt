/**
 * CurveControlChangeView.h
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
#ifndef __CurveControlChangeView_h__
#define __CurveControlChangeView_h__

#include <map>
#include <QScrollArea>
#include <QMutex>
#include "vsq/MeasureLineIterator.hpp"
#include "vsq/TimesigList.hpp"
#include "vsq/Track.hpp"
#include "gui/CurveControlChangeViewContent.h"
#include "gui/CurveControlChangeViewContentScroller.h"
#include "gui/CurveControlChangeViewSelector.h"
#include "ControlChangeView.hpp"

namespace Ui{
    class CurveControlChangeView;
}

namespace cadencii{
    class CurveControlChangeView : public QWidget, public ControlChangeView{
        friend class CurveControlChangeViewContentScroller;
        friend class CurveControlChangeViewContent;

        Q_OBJECT

    public:
        Ui::CurveControlChangeView *ui;

        explicit CurveControlChangeView( QWidget *parent = 0 );

        ~CurveControlChangeView();

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        /**
         * @brief オーバーライドする。ピアノロール本体と、鍵盤部分を repaint する処理を追加している。
         */
        void repaint();

        /**
         * @brief ミューテックスを設定する
         * @param mutex ミューテックス
         */
        void setMutex( QMutex *mutex );

        /**
         * @brief ソングポジションの移動に伴って自動スクロールするかどうかを取得する
         * @return 自動スクロールする場合は true を返す
         */
        bool isAutoScroll();

    private:
        /**
         * @brief スクロール領域が横方向にスクロールしたことを CurveControlChangeViewContentScroller -> CurveControlChangeView に通知する
         */
        void notifyHorizontalScroll();
    };

}
#endif // __CurveControlChangeView_h__
