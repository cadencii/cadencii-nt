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
#include <sstream>
#include "ConcretePropertyView.hpp"
#include "../../SignalEmitWorker.hpp"

namespace cadencii {

    ConcretePropertyView::ConcretePropertyView(QWidget *parent) :
        QtTreePropertyBrowser(parent), controllerAdapter(0) {
        setFocusPolicy(Qt::NoFocus);
        setResizeMode(QtTreePropertyBrowser::Interactive);
        setIndentation(10);
        proxy.setParent(this);
        initProperties();
        treeUpdateWorker = new SignalEmitWorker();
        connect(treeUpdateWorker, SIGNAL(signal()), this, SLOT(updateTree()));
        treeUpdateWorker->start();
        trackIndex = 0;

        setNoFocus(parent ? parent : this);

        connect(this, SIGNAL(valueChanged(const QtProperty*)),
                this, SLOT(onValueChangedSlot(const QtProperty*)));
    }

    void ConcretePropertyView::setNoFocus(QWidget *widget) {
        widget->setFocusPolicy(Qt::NoFocus);
        const QObjectList children = widget->children();
        for (QObjectList::const_iterator i = children.begin(); i != children.end(); ++i) {
            QObject *child = *i;
            if (child->isWidgetType()) {
                QWidget *childWidget = static_cast<QWidget *>(child);
                setNoFocus(childWidget);
            }
        }
    }

    ConcretePropertyView::~ConcretePropertyView() {
        delete treeUpdateWorker;
    }

    void ConcretePropertyView::statusChanged() {
        treeUpdateWorker->enqueue();
    }

    void ConcretePropertyView::setControllerAdapter(ControllerAdapter *adapter) {
        controllerAdapter = adapter;
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
        auto const* list = manager->getEventItemList();

        if (list->empty()) {
            parent->clear();
            return;
        }

        parent->addProperty(parent->lyric);
        parent->addProperty(parent->note);
        parent->addProperty(parent->notelocation);
        parent->addProperty(parent->vibrato);

        auto i = list->begin();

        // 複数のイベントのプロパティを表示する場合、すべてのイベントのプロパティが同じもののみ、
        // 値を表示する。イベント同士で値が違うものは、空欄とする
        const vsq::Sequence *sequence = controllerAdapter->getSequence();
        blockSignals(true);
        proxy.begin();
        for (; i != list->end(); ++i) {
            vsq::Event const* item = i->first;
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
        auto const* itemList = manager->getEventItemList();
        auto i = itemList->begin();

        vsq::Sequence const* sequence = controllerAdapter->getSequence();
        std::map<int, std::shared_ptr<vsq::Event>> editedItemList;
        for (; i != itemList->end(); ++i) {
            vsq::Event item = *(i->first);
            fetchProperty(property, &item, sequence);
            editedItemList[item.id] = std::make_shared<vsq::Event>(item);
        }

        EditEventCommand command(trackIndex, editedItemList);
        controllerAdapter->execute(&command);
    }

    void ConcretePropertyView::fetchProperty(
            const QtProperty *property,
            vsq::Event *event, const vsq::Sequence *sequence) {
        vsq::Lyric lyric = event->lyricHandle.get(0);
        if (property == lyricPhrase) {
            lyric.phrase = proxy.getLyricPhrase();
            if (!lyric.isProtected) {
                const vsq::PhoneticSymbolDictionary::Element *element
                        = controllerAdapter->attachPhoneticSymbol(lyric.phrase);
                if (element) lyric.phoneticSymbol(element->symbol());
            }
            event->lyricHandle.set(0, lyric);
        } else if (property == lyricPhoneticSymbol) {
            lyric.phoneticSymbol(proxy.getLyricPhoneticSymbol());
            event->lyricHandle.set(0, lyric);
        } else if (property == lyricConsonantAdjustment) {
            std::string adjustment
                    = vsq::StringUtil::replace(proxy.getLyricConsonantAdjustment(), " ", ",");
            lyric.consonantAdjustment(adjustment);
            event->lyricHandle.set(0, lyric);
        } else if (property == lyricProtect) {
            lyric.isProtected = proxy.getLyricProtect() == 2;
            event->lyricHandle.set(0, lyric);
        } else if (property == noteLength) {
            event->length(vsq::StringUtil::parseInt<vsq::tick_t>(proxy.getNoteLength()));
        } else if (property == noteNumber) {
            event->note = vsq::StringUtil::parseInt<int>(proxy.getNoteNumber());
        } else if (property == vibratoType) {
            if (proxy.getVibratoType() == 0) {
                event->vibratoHandle = vsq::Handle(vsq::HandleType::UNKNOWN);
            } else {
                HandleStub handle = static_cast<HandleStub>(event->vibratoHandle);
                handle.setHandleType(vsq::HandleType::VIBRATO);
                handle.iconId = "$0404" + vsq::StringUtil::toString(proxy.getVibratoType() - 1, "%04x");
                event->vibratoHandle = handle;
            }
        } else if (property == vibratoLength) {
            event->vibratoHandle.length(vsq::StringUtil::parseInt<int>(proxy.getVibratoLength()));
        } else if (property == notelocationClock) {
            try {
                event->tick = vsq::StringUtil::parseInt<vsq::tick_t>(proxy.getNotelocationClock());
            } catch(vsq::StringUtil::IntegerParseException &) {}
        } else {
            int measure, beat, tick;
            proxy.getNotelocation(event->tick, &measure, &beat, &tick, sequence);
            if (property == notelocationMeasure) {
                measure
                    = vsq::StringUtil::parseInt<int>(proxy.getNotelocationMeasure());
            }
            if (property == notelocationBeat) {
                beat = vsq::StringUtil::parseInt<int>(proxy.getNotelocationBeat());
            }
            if (property == notelocationTick) {
                tick = vsq::StringUtil::parseInt<int>(proxy.getNotelocationTick());
            }
            int premeasure = sequence->preMeasure();
            vsq::tick_t clock
                    = sequence->timesigList.tickFromBarCount(measure + premeasure - 1);
            vsq::Timesig timesig = sequence->timesigList.timesigAt(clock);
            int step = 480 * 4 / timesig.denominator;
            clock += (beat - 1) * step + tick;
            event->tick = clock;
        }
    }
}
