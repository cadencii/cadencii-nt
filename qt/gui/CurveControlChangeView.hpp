/**
 * CurveControlChangeView.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2.0 as published by the Free Software Foundation.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __CurveControlChangeView_hpp__
#define __CurveControlChangeView_hpp__

#include <QGraphicsScene>
#include <string>
#include <vector>
#include "EditorWidgetBase.hpp"
#include "../../gui/ControlChangeView.hpp"

namespace cadencii {

    class CurveControlChangeView : public EditorWidgetBase, public ControlChangeView {
        Q_OBJECT

    protected:
        int trackIndex;

    private:
        /**
         * @brief 手前に表示している BPList
         */
        const vsq::BPList *front;
        /**
         * @brief コンポーネントの上端と、グラフの最大値の位置の距離
         */
        static const int MARGIN_TOP = 8;
        /**
         * @brief コンポーネントの下端と、グラフの最小の位置の距離
         */
        static const int MARGIN_BOTTOM = 8;
        /**
         * @brief Height control curve name box.
         */
        static const int CURVE_NAME_HEIGHT = 18;
        /**
         * @brief Mergin between control curve name box and component border.
         */
        static const int CURVE_NAME_MARGIN = 5;
        /**
         * @brief Vertical margin between control curve name boxes.
         */
        static const int CURVE_NAME_SPACE = 4;

        /**
         * @brief デフォルトで描画するシーケンス
         */
        vsq::Sequence defaultSequence;
        /**
         * @brief 表示中のコントロールチェンジの名前
         */
        std::string controlChangeName;
        /**
         * @brief コンポーネント間の区切り線の色
         */
        QColor borderColor;

    public:
        explicit CurveControlChangeView(QWidget *parent = 0);

        ~CurveControlChangeView();

        void *getWidget() override;

        void setDrawOffset(vsq::tick_t drawOffset) override;

        void setControllerAdapter(ControllerAdapter *controllerAdapter) override;

        void paintMainContent(QPainter *painter, const QRect &rect) override;

        void paintSubContent(QPainter *painter, const QRect &rect) override;

        void *getScrollEventSender() override;

        void setTrackIndex(int index) override;

        void setControlChangeName(const std::string &name) override;

        QSize getPreferredSubContentSceneSize() override;

    protected:
        void drawMeasureLine(
                QPainter *painter, const QRect &rect, int x,
                const vsq::MeasureLine &measureLine) override;

    protected slots:
        /**
         * @brief Receive mouse press signal from sub content.
         */
        void onSubContentMousePressSlot(QMouseEvent *event);

    private:
        void paintBPList(QPainter *painter, const vsq::BPList *list, const QRect &rect);

        /**
         * @brief コントロールチェンジの値から、描画するY座標を取得する
         * @param max コントロールチェンジの最大値
         * @param min コントロールチェンジの最小値
         * @param value コントロールチェンジの値
         */
        int getYFromValue(int max, int min, int value);

        /**
         * @brief コンポーネントのY座標から、コントロールチェンジの値を取得する
         * @param max コントロールチェンジの最大値
         * @param min コントロールチェンジの最小値
         * @param y コンポーネントのY座標
         */
        int getValueFromY(int max, int min, int y);

        void updateWidget() override;

        /**
         * @brief Get draw rectangle.
         * @param index An index, couted from above.
         */
        inline QRectF getCurveNameRect(int index);

        /**
         * @brief Get a list of curve name for current track.
         */
        inline const std::vector<std::string> *getCurrentCurveNameList();
    };
}

#endif
