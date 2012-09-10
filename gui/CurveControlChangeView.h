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

#include "EditorWidgetBase.h"
#include "../ControlChangeView.hpp"
#include <QGraphicsScene>

namespace cadencii{

    class CurveControlChangeView : public EditorWidgetBase, public ControlChangeView{
    private:
        VSQ_NS::Sequence *sequence;
        int trackIndex;
        /**
         * @brief 手前に表示している BPList
         */
        VSQ_NS::BPList *front;
        /**
         * @brief コンポーネントの上端と、グラフの最大値の位置の距離
         */
        static const int MARGIN_TOP = 10;
        /**
         * @brief コンポーネントの下端と、グラフの最小の位置の距離
         */
        static const int MARGIN_BOTTOM = 10;
        /**
         * @brief デフォルトで描画するシーケンス
         */
        VSQ_NS::Sequence defaultSequence;
        /**
         * @brief 表示中のコントロールチェンジの名前
         */
        std::string controlChangeName;

    public:
        CurveControlChangeView( QWidget *parent = 0 );

        void setTimesigList( VSQ_NS::TimesigList *timesigList );

        void setSequence( VSQ_NS::Sequence *sequence );

        void *getWidget();

        void setDrawOffset( VSQ_NS::tick_t drawOffset );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

        void paintMainContent( QPainter *painter, const QRect &rect );

        void *getScrollEventSender();

        QSizeF getPreferedSceneSize();

        void setTrackIndex( int index );

        void setControlChangeName( const std::string &name );

    private:
        void paintBPList( QPainter *painter, VSQ_NS::BPList *list, const QRect &rect );
    };

}

#endif
