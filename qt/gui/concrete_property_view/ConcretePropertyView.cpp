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
        proxy.setParent(this);
        initProperties();
        treeUpdateWorker = new PropertyTreeUpdateWorker(this);
        connect(treeUpdateWorker, SIGNAL(callUpdateTree()), this, SLOT(updateTree()));
        treeUpdateWorker->start();
        trackIndex = 0;

        setNoFocus();

        connect(this, SIGNAL(valueChanged(const QtProperty*)),
                this, SLOT(onValueChangedSlot(const QtProperty*)));
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
        const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *list = manager->getEventItemList();

        if (list->empty()) {
            parent->clear();
            return;
        }

        parent->addProperty(parent->lyric);
        parent->addProperty(parent->note);
        parent->addProperty(parent->notelocation);
        parent->addProperty(parent->vibrato);

        std::map<const VSQ_NS::Event *, VSQ_NS::Event>::const_iterator i
                = list->begin();

        // 複数のイベントのプロパティを表示する場合、すべてのイベントのプロパティが同じもののみ、
        // 値を表示する。イベント同士で値が違うものは、空欄とする
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        blockSignals(true);
        proxy.begin();
        for (; i != list->end(); ++i) {
            const VSQ_NS::Event *item = i->first;
            proxy.add(item, sequence);
        }
        proxy.commit();
        blockSignals(false);
    }

    void ConcretePropertyView::setTrackIndex(int trackIndex) {
        this->trackIndex = trackIndex;
    }

    void ConcretePropertyView::onValueChangedSlot(const QtProperty *property) {
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *itemList
                = manager->getEventItemList();
        std::map<const VSQ_NS::Event *, VSQ_NS::Event>::const_iterator i
                = itemList->begin();

        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        std::map<int, VSQ_NS::Event> editedItemList;
        for (; i != itemList->end(); ++i) {
            VSQ_NS::Event item = *(i->first);
            fetchProperty(property, &item, sequence);
            editedItemList.insert(std::make_pair(item.id, item));
        }

        EditEventCommand command(trackIndex, editedItemList);
        controllerAdapter->execute(&command);
    }

    void ConcretePropertyView::fetchProperty(
            const QtProperty *property,
            VSQ_NS::Event *event, const VSQ_NS::Sequence *sequence) {
        VSQ_NS::Lyric lyric = event->lyricHandle.getLyricAt(0);
        if (property == lyricPhrase) {
            lyric.phrase = proxy.getLyricPhrase();
            if (!lyric.isProtected) {
                const VSQ_NS::PhoneticSymbolDictionary::Element *element
                        = controllerAdapter->attachPhoneticSymbol(lyric.phrase);
                if (element) lyric.setPhoneticSymbol(element->symbol());
            }
            event->lyricHandle.setLyricAt(0, lyric);
        } else if (property == lyricPhoneticSymbol) {
            lyric.setPhoneticSymbol(proxy.getLyricPhoneticSymbol());
            event->lyricHandle.setLyricAt(0, lyric);
        } else if (property == lyricConsonantAdjustment) {
            std::string adjustment
                    = StringUtil::replace(proxy.getLyricConsonantAdjustment(), " ", ",");
            lyric.setConsonantAdjustment(adjustment);
            event->lyricHandle.setLyricAt(0, lyric);
        } else if (property == lyricProtect) {
            lyric.isProtected = proxy.getLyricProtect() == 2;
            event->lyricHandle.setLyricAt(0, lyric);
        } else if (property == noteLength) {
            event->setLength(StringUtil::parseInt<VSQ_NS::tick_t>(proxy.getNoteLength()));
        } else if (property == noteNumber) {
            event->note = StringUtil::parseInt<int>(proxy.getNoteNumber());
        } else if (property == vibratoType) {
            if (proxy.getVibratoType() == 0) {
                event->vibratoHandle = VSQ_NS::Handle(VSQ_NS::HandleType::UNKNOWN);
            } else {
                HandleStub handle = static_cast<HandleStub>(event->vibratoHandle);
                handle.setHandleType(VSQ_NS::HandleType::VIBRATO);
                handle.iconId = "$0404" + StringUtil::toString(proxy.getVibratoType() - 1, "%04x");
                event->vibratoHandle = handle;
            }
        } else if (property == vibratoLength) {
            event->vibratoHandle.setLength(StringUtil::parseInt<int>(proxy.getVibratoLength()));
        } else if (property == notelocationClock) {
            try {
                event->clock = StringUtil::parseInt<VSQ_NS::tick_t>(proxy.getNotelocationClock());
            } catch(StringUtil::IntegerParseException &) {}
        } else {
            int measure, beat, tick;
            proxy.getNotelocation(event->clock, &measure, &beat, &tick, sequence);
            if (property == notelocationMeasure) {
                measure
                    = StringUtil::parseInt<int>(proxy.getNotelocationMeasure());
            }
            if (property == notelocationBeat) {
                beat = StringUtil::parseInt<int>(proxy.getNotelocationBeat());
            }
            if (property == notelocationTick) {
                tick = StringUtil::parseInt<int>(proxy.getNotelocationTick());
            }
            int premeasure = sequence->getPreMeasure();
            VSQ_NS::tick_t clock
                    = sequence->timesigList.getClockFromBarCount(measure + premeasure - 1);
            VSQ_NS::Timesig timesig = sequence->timesigList.getTimesigAt(clock);
            int step = 480 * 4 / timesig.denominator;
            clock += (beat - 1) * step + tick;
            event->clock = clock;
        }
    }
}
