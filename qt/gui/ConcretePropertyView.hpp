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

#include "../../gui/PropertyView.hpp"
#include "../../vsq/Sequence.hpp"
#include <QtTreePropertyBrowser>
#include <QtIntPropertyManager>
#include <QThread>
#include <QMutex>

namespace cadencii{

    class ConcretePropertyView : public QtTreePropertyBrowser, public PropertyView{
    private:
        /**
         * @brief プロパティツリーの更新を行うワーカースレッド
         */
        class PropertyTreeUpdateWorker : public QThread{
        private:
            static const int SLEEP_INTERVAL_MILLI_SECONDS = 10;
            ConcretePropertyView *parent;
            ControllerAdapter *controllerAdapter;
            QMutex mutex;
            bool updateRequested;

        public:
            PropertyTreeUpdateWorker( ConcretePropertyView *parent );

            void run();

            /**
             * @brief ツリーの更新を要求する
             */
            void enqueueTreeUpdate();

            void setControllerAdapter( ControllerAdapter * adapter );

        private:
            /**
             * @brief 音符・歌手変更イベントを表示するために、プロパティツリーを更新する
             */
            void updateTreeByEvent( const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *list );
        };

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

        PropertyTreeUpdateWorker treeUpdateWorker;

    public:
        ConcretePropertyView( QWidget *parent = 0 );

        void setControllerAdapter( ControllerAdapter *adapter );

        void *getWidget();

        void updateWidget();

        void statusChanged();

    private:
        /**
         * @brief QtProperty * 型のフィールドを初期化する
         */
        void initProperties();
    };

}

#endif
