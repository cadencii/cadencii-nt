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

namespace cadencii {

    class PropertyTreeUpdateWorker;

    class ConcretePropertyView : public QtTreePropertyBrowser, public PropertyView {
        Q_OBJECT

        friend class PropertyTreeUpdateWorker;

        class Proxy : public PropertyValueProxy {
        private:
            ConcretePropertyView *parent;

        public:
            Proxy() :
                PropertyValueProxy(), parent(0) {
            }

            void setParent(ConcretePropertyView *parent) {
                this->parent = parent;
            }

            void setLyricPhrase(const std::string &lyricPhrase) {
                parent->stringPropertyManager.setValue(
                    parent->lyricPhrase,
                    lyricPhrase.c_str()
                );
            }

            void setLyricPhoneticSymbol(const std::string &lyricPhoneticSymbol) {
                parent->stringPropertyManager.setValue(
                    parent->lyricPhoneticSymbol,
                    lyricPhoneticSymbol.c_str()
                );
            }

            void setLyricConsonantAdjustment(const std::string &lyricConsonantAdjustment) {
                parent->stringPropertyManager.setValue(
                    parent->lyricConsonantAdjustment,
                    lyricConsonantAdjustment.c_str()
                );
            }

            void setLyricProtect(int lyricProtect) {
                parent->enumPropertyManager.setValue(parent->lyricProtect, lyricProtect);
            }

            void setNoteLength(const std::string &noteLength) {
                parent->stringPropertyManager.setValue(
                    parent->noteLength,
                    noteLength.c_str()
                );
            }

            void setNoteNumber(const std::string &noteNumber) {
                parent->stringPropertyManager.setValue(
                    parent->noteNumber,
                    noteNumber.c_str()
                );
            }

            void setNotelocationClock(const std::string &notelocationClock) {
                parent->stringPropertyManager.setValue(
                    parent->notelocationClock,
                    notelocationClock.c_str()
                );
            }

            void setNotelocationMeasure(const std::string &notelocationMeasure) {
                parent->stringPropertyManager.setValue(
                    parent->notelocationMeasure,
                    notelocationMeasure.c_str()
                );
            }

            void setNotelocationBeat(const std::string &notelocationBeat) {
                parent->stringPropertyManager.setValue(
                    parent->notelocationBeat,
                    notelocationBeat.c_str()
                );
            }

            void setNotelocationTick(const std::string &notelocationTick) {
                parent->stringPropertyManager.setValue(
                    parent->notelocationTick,
                    notelocationTick.c_str()
                );
            }

            void setVibratoType(int vibratoType) {
                parent->enumPropertyManager.setValue(parent->vibratoType, vibratoType);
            }

            void setVibratoLength(const std::string &vibratoLength) {
                parent->stringPropertyManager.setValue(
                    parent->vibratoLength,
                    vibratoLength.c_str()
                );
            }
        };

    private:
        Proxy proxy;
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
