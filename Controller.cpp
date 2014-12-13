/**
 * Controller.cpp
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
#include <QMessageBox>
#include <vector>
#include <map>
#include <string>
#include "Controller.hpp"
#include <libvsq/libvsq.h>
#include "Settings.hpp"
#include "command/DeleteEventCommand.hpp"
#include "sequence/io/XVSQFileReader.hpp"
#include "qt/sequence/io/ConcreteSAXAdapter.hpp"
#include "sequence/io/XVSQFileWriter.hpp"

namespace cadencii {

    Controller::Controller()
        : trackView(0), mainView(0), controlChangeView(0), barCountView(0),
          tempoView(0), timesigView(0), propertyView(0), singerListView(0),
          trackListView(0), songPosition(0), pixelPerTick(0.2), saveFilePath("") {
        model.reset(vsq::Sequence("Miku", 1, 4, 4, 500000));
        toolKind = ToolKind::POINTER;
        trackIndex = 0;
    }

    void Controller::setTrackView(TrackView *trackView)throw() {
        this->trackView = trackView;
        if (this->trackView) {
            this->trackView->setControllerAdapter(this);
        }

        if (mainView) {
            mainView->setTrackView(this->trackView);
        }
    }

    void Controller::setControlChangeView(ControlChangeView *controlChangeView)throw() {
        this->controlChangeView = controlChangeView;
        if (this->controlChangeView) {
            this->controlChangeView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setControlChangeView(this->controlChangeView);
        }
    }

    void Controller::setMainView(MainView *mainView)throw() {
        this->mainView = mainView;
        if (this->mainView) {
            this->mainView->setControllerAdapter(this);

            if (this->trackView) {
                this->mainView->setTrackView(this->trackView);
            }
            if (this->controlChangeView) {
                this->mainView->setControlChangeView(this->controlChangeView);
            }
            if (this->barCountView) {
                this->mainView->setBarCountView(this->barCountView);
            }
            if (this->tempoView) {
                this->mainView->setTempoView(this->tempoView);
            }
            if (this->timesigView) {
                this->mainView->setTimesigView(this->timesigView);
            }
        }
    }

    void Controller::setBarCountView(BarCountView *barCountView)throw() {
        this->barCountView = barCountView;
        if (this->barCountView) {
            this->barCountView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setBarCountView(this->barCountView);
        }
    }

    void Controller::setTempoView(TempoView *tempoView)throw() {
        this->tempoView = tempoView;
        if (this->tempoView) {
            this->tempoView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setTempoView(this->tempoView);
        }
    }

    void Controller::setTimesigView(TimesigView *timesigView)throw() {
        this->timesigView = timesigView;
        if (this->timesigView) {
            this->timesigView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setTimesigView(this->timesigView);
        }
    }

    void Controller::setPropertyView(PropertyView *propertyView)throw() {
        this->propertyView = propertyView;
        if (this->propertyView) {
            this->propertyView->setControllerAdapter(this);
            itemSelectionManager.addStatusListener(this->propertyView);
        }
        if (mainView) {
            mainView->setPropertyView(this->propertyView);
        }
    }

    void Controller::setSingerListView(SingerListView *singerListView) {
        this->singerListView = singerListView;
        if (this->singerListView) {
            this->singerListView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setSingerListView(this->singerListView);
        }
    }

    void Controller::setTrackListView(TrackListView *trackListView) {
        this->trackListView = trackListView;
        if (this->trackListView) {
            this->trackListView->setControllerAdapter(this);
        }
        if (mainView) {
            mainView->setTrackListView(this->trackListView);
        }
    }

    void Controller::openVSQFile(const std::string &filePath)throw() {
        vsq::VSQFileReader reader;
        vsq::FileInputStream stream(filePath);
        vsq::Sequence sequence;
        try {
            reader.read(sequence, stream, "Shift_JIS");
            stream.close();
            setupSequence(sequence);
        } catch(std::exception &) {
            if (!Settings::instance()->isUnderUnitTest()) {
                QMessageBox::information(
                    0, QObject::tr("Error"),
                    QObject::tr("Invalid VSQ/VOCALOID MIDI file"), Qt::NoButton);
            }
        }
    }

    void Controller::openXVSQFile(const std::string &filePath)throw() {
        XVSQFileReader reader;
        vsq::Sequence sequence;
        try {
            ConcreteSAXAdapter adapter(filePath);
            reader.read(&sequence, &adapter);
            setupSequence(sequence);
        } catch(std::exception &) {
            if (!Settings::instance()->isUnderUnitTest()) {
                QMessageBox::information(
                    0, QObject::tr("Error"),
                    QObject::tr("Invalid XVSQ file"), Qt::NoButton);
            }
        }
    }

    void Controller::drawOffsetChanged(void *sender, vsq::tick_t offset)throw() {
        if (sender == static_cast<void *>(trackView)) {
            if (controlChangeView) controlChangeView->setDrawOffset(offset);
            if (barCountView) barCountView->setDrawOffset(offset);
            if (tempoView) tempoView->setDrawOffset(offset);
            if (timesigView) timesigView->setDrawOffset(offset);
            if (singerListView) singerListView->setDrawOffset(offset);
        } else if (sender == static_cast<void *>(controlChangeView)) {
            if (trackView) trackView->setDrawOffset(offset);
            if (barCountView) barCountView->setDrawOffset(offset);
            if (tempoView) tempoView->setDrawOffset(offset);
            if (timesigView) timesigView->setDrawOffset(offset);
            if (singerListView) singerListView->setDrawOffset(offset);
        } else if (sender == static_cast<void *>(barCountView)) {
            if (controlChangeView) controlChangeView->setDrawOffset(offset);
            if (trackView) trackView->setDrawOffset(offset);
            if (tempoView) tempoView->setDrawOffset(offset);
            if (timesigView) timesigView->setDrawOffset(offset);
            if (singerListView) singerListView->setDrawOffset(offset);
        } else if (sender == static_cast<void *>(tempoView)) {
            if (controlChangeView) controlChangeView->setDrawOffset(offset);
            if (trackView) trackView->setDrawOffset(offset);
            if (barCountView) barCountView->setDrawOffset(offset);
            if (timesigView) timesigView->setDrawOffset(offset);
            if (singerListView) singerListView->setDrawOffset(offset);
        } else if (sender == static_cast<void *>(timesigView)) {
            if (controlChangeView) controlChangeView->setDrawOffset(offset);
            if (trackView) trackView->setDrawOffset(offset);
            if (barCountView) barCountView->setDrawOffset(offset);
            if (tempoView) tempoView->setDrawOffset(offset);
            if (singerListView) singerListView->setDrawOffset(offset);
        } else if (sender == static_cast<void *>(singerListView)) {
            if (controlChangeView) controlChangeView->setDrawOffset(offset);
            if (trackView) trackView->setDrawOffset(offset);
            if (barCountView) barCountView->setDrawOffset(offset);
            if (timesigView) timesigView->setDrawOffset(offset);
            if (tempoView) tempoView->setDrawOffset(offset);
        }
    }

    vsq::tick_t Controller::getSongPosition()throw() {
        return songPosition;
    }

    int Controller::getXFromTick(vsq::tick_t tick)throw() {
        return static_cast<int>(tick * pixelPerTick) + 5;
    }

    double Controller::getTickFromX(int x)throw() {
        return (x - 5) / pixelPerTick;
    }

    void Controller::setTrackIndex(void *, int index) throw() {
        trackIndex = index;
        // TODO(kbinani): senderの値によって、どのコンポーネントに
        //                setTrackIndexを呼ぶか振り分ける処理が必要
        trackView->setTrackIndex(index);
        controlChangeView->setTrackIndex(index);
        trackListView->setTrackIndex(index);
        singerListView->setTrackIndex(index);
        propertyView->setTrackIndex(index);
        itemSelectionManager.clear();
        if (mainView) {
            mainView->notifyCommandHistoryChanged();
        }
    }

    void Controller::setupSequence(const vsq::Sequence &sequence) {
        model.reset(sequence);
        setTrackIndex(this, 0);
        controlChangeView->setControlChangeName("pit");
    }

    void Controller::moveSongPositionStepped(bool isBackward)throw() {
        QuantizeMode::QuantizeModeEnum mode = Settings::instance()->getQuantizeMode();
        vsq::tick_t unit = QuantizeMode::getQuantizeUnitTick(mode);
        vsq::tick_t newSongPosition
                = getQuantizedTick(songPosition + (isBackward ? -unit : unit), mode);
        int minX = getXFromTick(0);
        int x = getXFromTick(newSongPosition);
        if (x < minX) {
            newSongPosition = getTickFromX(minX);
        }
        int preferredComponentWidth = getPreferredComponentWidth();
        if (preferredComponentWidth < x) {
            newSongPosition = getTickFromX(preferredComponentWidth);
        }
        if (newSongPosition != songPosition) {
            setSongPosition(newSongPosition);
        }
    }

    vsq::tick_t Controller::getQuantizedTick(
            vsq::tick_t before,
            QuantizeMode::QuantizeModeEnum mode) {
        vsq::tick_t unit = QuantizeMode::getQuantizeUnitTick(mode);
        vsq::tick_t odd = before % unit;
        vsq::tick_t result = before - odd;
        if (odd > unit / 2) {
            result += unit;
        }
        return result;
    }

    void Controller::updateAllWidget() {
        if (mainView) mainView->updateWidget();
        if (barCountView) barCountView->updateWidget();
        if (tempoView) tempoView->updateWidget();
        if (timesigView) timesigView->updateWidget();
        if (trackView) trackView->updateWidget();
        if (controlChangeView) controlChangeView->updateWidget();
        if (propertyView) propertyView->updateWidget();
        if (singerListView) singerListView->updateWidget();
        if (trackListView) trackListView->updateWidget();
    }

    int Controller::getPreferredComponentWidth() throw() {
        vsq::tick_t totalClocks = model.getSequence()->totalTicks();
        int result = getXFromTick(totalClocks);
        if (trackView) {
            result += trackView->getTrackViewWidth();
        }
        return result;
    }

    void Controller::setSongPosition(vsq::tick_t songPosition) {
        this->songPosition = songPosition;
        if (Settings::instance()->isAutoScroll()) {
            trackView->ensureNoteVisible(songPosition, 0);
        }
        updateAllWidget();
    }

    void Controller::exportAsMusicXml(const std::string &filePath)throw() {
        vsq::MusicXmlWriter writer;
        try {
            vsq::StreamWriter stream(filePath);
            writer.write(*model.getSequence(), stream, "cadencii");
            stream.close();
        } catch(vsq::StreamWriter::IOException const&) {
            if (!Settings::instance()->isUnderUnitTest()) {
                QMessageBox::information(
                    0, QObject::tr("Error"),
                    QObject::tr("Can't write to file: ") + filePath.c_str(),
                    Qt::NoButton);
            }
        }
    }

    void Controller::exportAsVSQFile(const std::string &filePath)throw() {
        vsq::VSQFileWriter writer;
        try {
            vsq::FileOutputStream stream(filePath);
            writer.write(*model.getSequence(), stream, 500, "Shift_JIS", false);
            stream.close();
        } catch(vsq::FileOutputStream::IOException const&) {
            if (!Settings::instance()->isUnderUnitTest()) {
                QMessageBox::information(
                    0, QObject::tr("Error"),
                    QObject::tr("Can't write to file: ") + filePath.c_str(),
                    Qt::NoButton);
            }
        }
    }

    void Controller::setToolKind(ToolKind::ToolKindEnum kind)throw() {
        toolKind = kind;
        if (mainView) mainView->setToolKind(toolKind);
    }

    ToolKind::ToolKindEnum Controller::getToolKind()throw() {
        return toolKind;
    }

    ItemSelectionManager *Controller::getItemSelectionManager()throw() {
        return &itemSelectionManager;
    }

    const vsq::Sequence *Controller::getSequence()throw() {
        return model.getSequence();
    }

    void Controller::redo() {
        model.redo();
        itemSelectionManager.updateSelectedContents(trackIndex, model.getSequence());
        if (mainView) mainView->notifyCommandHistoryChanged();
        if (propertyView) propertyView->statusChanged();
    }

    void Controller::undo() {
        model.undo();
        itemSelectionManager.updateSelectedContents(trackIndex, model.getSequence());
        if (mainView) mainView->notifyCommandHistoryChanged();
        if (propertyView) propertyView->statusChanged();
    }

    bool Controller::canRedo() {
        return model.canRedo();
    }

    bool Controller::canUndo() {
        return model.canUndo();
    }

    void Controller::execute(AbstractCommand *command) {
        model.execute(command);

        // update value of selected items.
        itemSelectionManager.updateSelectedContents(trackIndex, model.getSequence());

        if (mainView) mainView->notifyCommandHistoryChanged();
        if (propertyView) propertyView->statusChanged();
    }

    void Controller::showMainView() {
        if (mainView) mainView->showWidget();
    }

    void Controller::removeEvent(int trackIndex, const vsq::Event *item) {
        if (item) {
            // マウスの位置にイベントがあった場合.
            std::vector<int> idList;
            auto const* selectedItemList = itemSelectionManager.getEventItemList();
            auto index = selectedItemList->find(item);
            if (index == selectedItemList->end()) {
                // マウスの位置のイベントが、選択されたイベントに含まれていなかった場合,
                // マウス位置のイベントのみ削除する.
                idList.push_back(item->id);
            } else {
                // マウスの位置のイベントが、選択されたイベントに含まれていた場合、
                // 選択されたイベントを全て削除する.
                auto i = selectedItemList->begin();
                for (; i != selectedItemList->end(); ++i) {
                    idList.push_back(i->first->id);
                }
            }

            itemSelectionManager.clear();

            DeleteEventCommand *command = new DeleteEventCommand(trackIndex, idList);
            execute(command);
        } else {
            itemSelectionManager.clear();
        }
    }

    void Controller::removeSelectedItems() {
        // TODO(kbinani): 音符・歌手イベント以外の選択ができるようになったら対応する.
        auto const* itemList = itemSelectionManager.getEventItemList();
        auto i = itemList->begin();
        std::vector<int> idList;
        for (; i != itemList->end(); ++i) {
            idList.push_back(i->first->id);
        }

        itemSelectionManager.clear();
        DeleteEventCommand *command = new DeleteEventCommand(trackIndex, idList);
        execute(command);
    }

    void Controller::setApplicationShortcutEnabled(bool enabled) {
        if (mainView) mainView->setApplicationShortcutEnabled(enabled);
    }

    const vsq::PhoneticSymbolDictionary::Element *Controller::attachPhoneticSymbol(
            const std::string &word) {
        return vsq::PhoneticSymbolDictionary::vocaloidJpDictionary()->attach(word);
    }

    std::string Controller::getSaveFilePath()const {
        return saveFilePath;
    }

    void Controller::saveAsXVSQFile(std::string const& filePath) {
        saveFilePath = filePath;
        XVSQFileWriter writer;
        vsq::FileOutputStream stream(filePath);
        writer.write(model.getSequence(), &stream);
    }
}
