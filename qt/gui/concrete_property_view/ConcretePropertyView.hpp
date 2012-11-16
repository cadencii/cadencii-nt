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

        class HandleStub : public VSQ_NS::Handle {
        public:
            explicit HandleStub(const VSQ_NS::Handle handle) :
                VSQ_NS::Handle(handle) {
            }

            void setHandleType(VSQ_NS::HandleType::HandleTypeEnum type) {
                _type = type;
            }
        };

    protected:
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

        ConcretePropertyValueProxy proxy;

    private:
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

        PropertyTreeUpdateWorker *treeUpdateWorker;
        int trackIndex;

    public:
        explicit ConcretePropertyView(QWidget *parent = 0);

        ~ConcretePropertyView();

        void setControllerAdapter(ControllerAdapter *adapter);

        void *getWidget();

        void updateWidget();

        void statusChanged();

        void setTrackIndex(int trackIndex);

    public slots:
        /**
         * @brief 音符・歌手変更イベントを表示するために、プロパティツリーを更新する
         */
        void updateTree();

        /**
         * @brief Receive valueChanged signal from QtTreePropertyBrowser.
         */
        void onValueChangedSlot(const QtProperty *property);

    protected:
        /**
         * @brief Get property values from property view.
         */
        void fetchProperty(
                const QtProperty *property,
                VSQ_NS::Event *event, const VSQ_NS::Sequence *sequence);

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
