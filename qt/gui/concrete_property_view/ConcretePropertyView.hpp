/**
 * ConcretePropertyView.hpp
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
#ifndef __cadencii_qt_gui_ConcretePropertyView_hpp__
#define __cadencii_qt_gui_ConcretePropertyView_hpp__

#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QThread>
#include <QMutex>
#include "../../gui/PropertyView.hpp"
#include "../../gui/PropertyValueProxy.hpp"
#include "../../vsq/Sequence.hpp"
#include "ConcretePropertyValueProxy.hpp"

namespace cadencii {

    class PropertyTreeUpdateWorker;

    class ConcretePropertyView : public QtTreePropertyBrowser, public PropertyView {
        Q_OBJECT

        friend class PropertyTreeUpdateWorker;
        friend class ConcretePropertyValueProxy;

    private:
        ConcretePropertyValueProxy proxy;
        ControllerAdapter *controllerAdapter;

        QtGroupPropertyManager groupManager;

        /**
         * @brief 汎用文字列プロパティマネージャ
         */
        QtStringPropertyManager stringPropertyManager;
        /**
         * @brief 列挙子のプロパティ用マネージャ
         */
        QtEnumPropertyManager enumPropertyManager;

        // Lyric
        QtProperty *lyric;
        QtProperty *lyricPhrase;
        QtProperty *lyricPhoneticSymbol;
        QtProperty *lyricConsonantAdjustment;
        QtProperty *lyricProtect;

        // Note
        QtProperty *note;
        QtProperty *noteLength;
        QtProperty *noteNumber;

        // Note Location
        QtProperty *notelocation;
        QtProperty *notelocationClock;
        QtProperty *notelocationMeasure;
        QtProperty *notelocationBeat;
        QtProperty *notelocationTick;

        // Vibrato
        QtProperty *vibrato;
        QtProperty *vibratoType;
        QtProperty *vibratoLength;

        PropertyTreeUpdateWorker *treeUpdateWorker;

    public:
        explicit ConcretePropertyView(QWidget *parent = 0);

        ~ConcretePropertyView();

        void setControllerAdapter(ControllerAdapter *adapter);

        void *getWidget();

        void updateWidget();

        void statusChanged();

    public slots:
        /**
         * @brief 音符・歌手変更イベントを表示するために、プロパティツリーを更新する
         */
        void updateTree();

    private:
        /**
         * @brief QtProperty * 型のフィールドを初期化する
         */
        void initProperties();

        /**
         * @brief Forbid focus on this widget.
         */
        void setNoFocus();
    };
}

#endif
