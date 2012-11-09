/**
 * ConcretePropertyView.cpp
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
#include <QtIntPropertyManager>
#include <QtSpinBoxFactory>
#include <map>
#include <string>
#include <vector>
#include "ConcretePropertyView.hpp"
#include "PropertyTreeUpdateWorker.hpp"

namespace cadencii {

    ConcretePropertyView::ConcretePropertyView(QWidget *parent) :
        QtTreePropertyBrowser(parent), controllerAdapter(0) {
        setFocusPolicy(Qt::NoFocus);
        setResizeMode(QtTreePropertyBrowser::Interactive);
        setIndentation(10);
        initProperties();
        treeUpdateWorker = new PropertyTreeUpdateWorker(this);
        connect(treeUpdateWorker, SIGNAL(callUpdateTree()), this, SLOT(updateTree()));
        treeUpdateWorker->start();
        setNoFocus();
    }

    void ConcretePropertyView::setNoFocus() {
        const QObjectList children = this->children();
        for (QObjectList::const_iterator i = children.begin(); i != children.end(); ++i) {
            QObject *child = *i;
            if (child->isWidgetType()) {
                QWidget *widget = static_cast<QWidget *>(child);
                widget->setFocusPolicy(Qt::NoFocus);
            }
        }
    }

    ConcretePropertyView::~ConcretePropertyView() {
        delete treeUpdateWorker;
    }

    void ConcretePropertyView::statusChanged() {
        treeUpdateWorker->enqueueTreeUpdate();
    }

    void ConcretePropertyView::setControllerAdapter(ControllerAdapter *adapter) {
        controllerAdapter = adapter;
        treeUpdateWorker->setControllerAdapter(adapter);
    }

    void *ConcretePropertyView::getWidget() {
        return this;
    }

    void ConcretePropertyView::updateWidget() {
        repaint();
    }

    void ConcretePropertyView::initProperties() {
        // Lyric
        lyric = groupManager.addProperty(tr("Lyric"));
        lyricPhrase = stringPropertyManager.addProperty(tr("Phrase"));
        lyricPhoneticSymbol = stringPropertyManager.addProperty(tr("Phonetic Symbol"));
        lyricConsonantAdjustment = stringPropertyManager.addProperty(tr("Consonant Adjustment"));
        lyricProtect = enumPropertyManager.addProperty(tr("Protect"));
        lyric->addSubProperty(lyricPhrase);
        lyric->addSubProperty(lyricPhoneticSymbol);
        lyric->addSubProperty(lyricConsonantAdjustment);
        lyric->addSubProperty(lyricProtect);

        // Note
        note = groupManager.addProperty(tr("Note"));
        noteLength = stringPropertyManager.addProperty(tr("Length"));
        noteNumber = stringPropertyManager.addProperty(tr("Note#"));
        note->addSubProperty(noteLength);
        note->addSubProperty(noteNumber);

        // Note Location
        notelocation = groupManager.addProperty(tr("Note Location"));
        notelocationClock = stringPropertyManager.addProperty(tr("Clock"));
        notelocationMeasure = stringPropertyManager.addProperty(tr("Measure"));
        notelocationBeat = stringPropertyManager.addProperty(tr("Beat"));
        notelocationTick = stringPropertyManager.addProperty(tr("Tick"));
        notelocation->addSubProperty(notelocationClock);
        notelocation->addSubProperty(notelocationMeasure);
        notelocation->addSubProperty(notelocationBeat);
        notelocation->addSubProperty(notelocationTick);

        // Vibrato
        vibrato = groupManager.addProperty(tr("Vibrato"));
        vibratoType = enumPropertyManager.addProperty(tr("Vibrato"));
        vibratoLength = stringPropertyManager.addProperty(tr("Vibrato Length"));
        vibrato->addSubProperty(vibratoType);
        vibrato->addSubProperty(vibratoLength);

        // enum で列挙する文字列の一覧を作成
        QStringList types;
        types << "" << "Off" << "On";
        enumPropertyManager.setEnumNames(lyricProtect, types);
        setFactoryForManager(&enumPropertyManager, new QtEnumEditorFactory());

        setFactoryForManager(&stringPropertyManager, new QtLineEditFactory());

        QStringList vibratoTypes;
        std::vector<std::string> topTypes;
        topTypes.push_back("Normal");
        topTypes.push_back("Extreme");
        topTypes.push_back("Fast");
        topTypes.push_back("Slight");
        vibratoTypes << "-";
        for (std::vector<std::string>::iterator i = topTypes.begin(); i != topTypes.end(); ++i) {
            for (int j = 1; j <= 4; j++) {
                std::ostringstream name;
                name << "[" << (*i) << "] Type " << j;
                vibratoTypes << name.str().c_str();
            }
        }
        enumPropertyManager.setEnumNames(vibratoType, vibratoTypes);
    }

    void ConcretePropertyView::updateTree() {
        ConcretePropertyView *parent = this;
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const std::map<int, VSQ_NS::Event> *list = manager->getEventItemList();

        if (list->empty()) {
            parent->clear();
            return;
        }

        parent->addProperty(parent->lyric);
        parent->addProperty(parent->note);
        parent->addProperty(parent->notelocation);
        parent->addProperty(parent->vibrato);

        std::string lyricPhrase = "";
        std::string lyricPhoneticSymbol = "";
        std::string lyricConsonantAdjustment = "";
        int lyricProtect = 0;  // enum のインデックス 0 = 未選択, 1 = Off, 2 = On
        int noteLength = -1;
        int noteNumber = -1;
        VSQ_NS::tick_t notelocationClock = -1;
        int notelocationMeasure = -1;
        int notelocationBeat = 1;
        int notelocationTick = -1;
        int vibratoType = 0;
        int vibratoLength = -1;
        std::map<int, VSQ_NS::Event>::const_iterator i
                = list->begin();

        // 複数のイベントのプロパティを表示する場合、すべてのイベントのプロパティが同じもののみ、
        // 値を表示する。イベント同士で値が違うものは、空欄とする
        for (; i != list->end(); ++i) {
            VSQ_NS::Event item = i->second;
            VSQ_NS::Lyric lyric("a", "a");
            if (0 < item.lyricHandle.getLyricCount()) {
                lyric = item.lyricHandle.getLyricAt(0);
            }

            VSQ_NS::tick_t clock = item.clock;
            const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
            int premeasure = sequence->getPreMeasure();
            int measure = sequence->timesigList.getBarCountFromClock(clock) - premeasure + 1;
            int clock_bartop = sequence->timesigList.getClockFromBarCount(measure + premeasure - 1);
            VSQ_NS::Timesig timesig = sequence->timesigList.getTimesigAt(clock);
            int den = timesig.denominator;
            int dif = clock - clock_bartop;
            int step = 480 * 4 / den;
            int beat = dif / step + 1;
            int tick = dif - (beat - 1) * step;

            int vibType = 0;
            int vibLength = -1;
            if (item.vibratoHandle.getHandleType() == VSQ_NS::HandleType::VIBRATO) {
                if (item.vibratoHandle.iconId.length() == 9 && 0 < item.getLength()) {
                    std::string vibTypeString = item.vibratoHandle.iconId.substr(6);
                    vibType = StringUtil::parseInt<int>(vibTypeString, 16);
                    vibLength = item.vibratoHandle.getLength() * 100 / item.getLength();
                }
            }

            if (i == list->begin()) {
                lyricPhrase = lyric.phrase;
                lyricPhoneticSymbol = lyric.getPhoneticSymbol();
                lyricConsonantAdjustment = lyric.getConsonantAdjustment();
                lyricProtect = lyric.isProtected ? 2 : 1;
                noteLength = item.getLength();
                noteNumber = item.note;

                notelocationClock = clock;
                notelocationMeasure = measure;
                notelocationBeat = beat;
                notelocationTick = tick;

                vibratoType = vibType;
                vibratoLength = vibLength;
            } else {
                if (lyricPhrase != lyric.phrase) lyricPhrase = "";
                if (lyricPhoneticSymbol != lyric.getPhoneticSymbol()) lyricPhoneticSymbol = "";
                if (lyricConsonantAdjustment != lyric.getConsonantAdjustment()) {
                    lyricConsonantAdjustment = "";
                }
                if (lyricProtect != (lyric.isProtected ? 2 : 1)) lyricProtect = 0;

                if (noteLength != item.getLength()) noteLength = -1;
                if (noteNumber != item.note) noteNumber = -1;

                if (notelocationClock != clock) notelocationClock = -1;
                if (notelocationMeasure != measure) notelocationMeasure = -1;
                if (notelocationBeat != beat) notelocationBeat = -1;
                if (notelocationTick != tick) notelocationTick = -1;

                if (vibratoType != vibType) vibratoType = 0;
                if (vibratoLength != vibLength) vibratoLength = -1;
            }

            parent->stringPropertyManager.setValue(
                    parent->lyricPhrase, lyricPhrase.c_str());
            parent->stringPropertyManager.setValue(
                    parent->lyricPhoneticSymbol, lyricPhoneticSymbol.c_str());
            parent->stringPropertyManager.setValue(
                    parent->lyricConsonantAdjustment, lyricConsonantAdjustment.c_str());
            parent->enumPropertyManager.setValue(parent->lyricProtect, lyricProtect);

            parent->stringPropertyManager.setValue(
                    parent->noteLength,
                    0 <= noteLength ? StringUtil::toString(noteLength).c_str() : "");
            parent->stringPropertyManager.setValue(
                    parent->noteNumber,
                    0 <= noteNumber ? StringUtil::toString(noteNumber).c_str() : "");

            parent->stringPropertyManager.setValue(
                    parent->notelocationClock,
                    0 <= notelocationClock ? StringUtil::toString(notelocationClock).c_str() : "");
            parent->stringPropertyManager.setValue(
                    parent->notelocationMeasure,
                    0 <= notelocationMeasure
                        ? StringUtil::toString(notelocationMeasure).c_str()
                        : "");
            parent->stringPropertyManager.setValue(
                    parent->notelocationBeat,
                    0 <= notelocationBeat ? StringUtil::toString(notelocationBeat).c_str() : "");
            parent->stringPropertyManager.setValue(
                    parent->notelocationTick,
                    0 <= notelocationTick ? StringUtil::toString(notelocationTick).c_str() : "");

            parent->enumPropertyManager.setValue(parent->vibratoType, vibratoType);
            parent->stringPropertyManager.setValue(
                    parent->vibratoLength,
                    0 <= vibratoLength ? StringUtil::toString(vibratoLength).c_str() : "");
        }
    }
}
