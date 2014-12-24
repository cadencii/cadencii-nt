/**
 * PianorollTrackView.cpp
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
#include <QScrollBar>
#include <QMouseEvent>
#include <QLineEdit>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <libvsq/libvsq.h>
#include "../../command/EditEventCommand.hpp"
#include "../../command/AddEventCommand.hpp"
#include "../../enum/QuantizeMode.hpp"
#include "../../Settings.hpp"
#include "ui_EditorWidgetBase.h"
#include "PianorollTrackView.hpp"

namespace cadencii {

    PianorollTrackView::PianorollTrackView(QWidget *parent) :
        EditorWidgetBase(parent) {
        mutex = 0;
        trackHeight = DEFAULT_TRACK_HEIGHT;
        trackIndex = 0;
        ui->mainContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->mainContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        lyricEdit = new LyricEditWidget(ui->mainContent->viewport());
        lyricEdit->setVisible(false);

        // キーボードのキーの音名を作成
        keyNames = new QString[NOTE_MAX - NOTE_MIN + 1];
        std::vector<std::string> const names = {
            std::string("C"),
            std::string("C#"),
            std::string("D"),
            std::string("Eb"),
            std::string("E"),
            std::string("F"),
            std::string("F#"),
            std::string("G"),
            std::string("G#"),
            std::string("A"),
            std::string("Bb"),
            std::string("B")
        };
        for (int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++) {
            int modura = getNoteModuration(noteNumber);
            int order = getNoteOctave(noteNumber);
            std::string name = names[modura];
            std::ostringstream oss;
            oss << name << order;
            keyNames[noteNumber - NOTE_MIN] = QString(oss.str().c_str());
        }
        connect(ui->mainContent, SIGNAL(onMousePress(QMouseEvent *)),
                this, SLOT(onMousePressSlot(QMouseEvent *)));
        connect(ui->mainContent, SIGNAL(onMouseMove(QMouseEvent *)),
                this, SLOT(onMouseMoveSlot(QMouseEvent *)));
        connect(ui->mainContent, SIGNAL(onMouseRelease(QMouseEvent *)),
                this, SLOT(onMouseReleaseSlot(QMouseEvent *)));
        connect(ui->mainContent, SIGNAL(onMouseDoubleClick(QMouseEvent *)),
                this, SLOT(onMouseDoubleClickSlot(QMouseEvent *)));

        connect(ui->mainContent->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                this, SLOT(onContentScroll(int)));
        connect(ui->mainContent->verticalScrollBar(), SIGNAL(valueChanged(int)),
                this, SLOT(onContentScroll(int)));

        connect(lyricEdit, SIGNAL(onCommit()), this, SLOT(onLyricEditCommitSlot()));
        connect(lyricEdit, SIGNAL(onHide()), this, SLOT(onLyricEditHideSlot()));
        connect(lyricEdit, SIGNAL(onMove(bool)),
                this, SLOT(onLyricEditMoveSlot(bool)));
    }

    PianorollTrackView::~PianorollTrackView() {
        delete [] keyNames;
        delete lyricEdit;
    }

    void *PianorollTrackView::getWidget() {
        return static_cast<void *>(this);
    }

    void PianorollTrackView::setDrawOffset(vsq::tick_t drawOffset) {
        setDrawOffsetInternal(drawOffset);
    }

    void PianorollTrackView::setControllerAdapter(ControllerAdapter *controllerAdapter) {
        this->controllerAdapter = controllerAdapter;
    }

    void *PianorollTrackView::getScrollEventSender() {
        return static_cast<TrackView *>(this);
    }

    QSize PianorollTrackView::getPreferredMainContentSceneSize() {
        QScrollBar *scrollBar = ui->mainContent->verticalScrollBar();
        int width = controllerAdapter->getPreferredComponentWidth() - scrollBar->width();
        int height = trackHeight * (NOTE_MAX - NOTE_MIN + 1);
        return QSize(width, height);
    }

    void PianorollTrackView::ensureNoteVisible(
            vsq::tick_t tick, vsq::tick_t length, int noteNumber) {
        int left = controllerAdapter->getXFromTick(tick);
        int right = controllerAdapter->getXFromTick(tick + length);

        QRect visibleArea = ui->mainContent->getVisibleArea();
        QScrollBar *horizontalScrollBar = ui->mainContent->horizontalScrollBar();
        QScrollBar *verticalScrollBar = ui->mainContent->verticalScrollBar();
        int dx = 0;
        int newValue = verticalScrollBar->value();
        if (visibleArea.right() < right) {
            dx = ui->mainContent->width() - (right - left);
        } else if (left < visibleArea.left()) {
            dx = -ui->mainContent->width() + (right - left);
        }
        if (0 <= noteNumber) {
            int top = getYFromNoteNumber(noteNumber, trackHeight);
            int bottom = top + trackHeight;
            if (top < visibleArea.top() || visibleArea.bottom() < bottom) {
                newValue = (bottom + top) / 2 - visibleArea.height() / 2;
            }

            if (verticalScrollBar->value() != newValue) {
                if (newValue < verticalScrollBar->minimum()) {
                    verticalScrollBar->setValue(verticalScrollBar->minimum());
                } else if (verticalScrollBar->maximum() < newValue) {
                    verticalScrollBar->setValue(verticalScrollBar->maximum());
                } else {
                    verticalScrollBar->setValue(newValue);
                }
            }
        }
        if (dx) {
            int value = horizontalScrollBar->value() + dx;
            if (value < horizontalScrollBar->minimum()) {
                horizontalScrollBar->setValue(horizontalScrollBar->minimum());
            } else if (horizontalScrollBar->maximum() < value) {
                horizontalScrollBar->setValue(horizontalScrollBar->maximum());
            } else {
                horizontalScrollBar->setValue(value);
            }
        }
    }

    void PianorollTrackView::setMutex(QMutex *mutex) {
        this->mutex = mutex;
    }

    void PianorollTrackView::paintMainContent(QPainter *painter, const QRect &rect) {
        paintBackground(painter, rect);
        ui->mainContent->paintMeasureLines(painter, rect);
        if (mutex) {
            mutex->lock();
            paintItems(painter, rect);
            mutex->unlock();
        } else {
            paintItems(painter, rect);
        }
        ui->mainContent->paintSongPosition(painter, rect);

        // 矩形選択の範囲を描画する
        if (mouseStatus.mode == MouseStatus::LEFTBUTTON_SELECT_ITEM) {
            QRect selectRect = mouseStatus.rect();
            static QColor const fillColor(0, 0, 0, 100);
            static QColor const borderColor(0, 0, 0, 200);
            static QColor const shadowColor(0, 0, 0, 40);
            painter->fillRect(selectRect, fillColor);
            painter->setPen(borderColor);
            painter->drawRect(selectRect);

            // 選択範囲の周囲に影を描く.
            painter->setPen(shadowColor);
            // 上側
            painter->drawLine(selectRect.left(), selectRect.top() - 1,
                              selectRect.right() + 1, selectRect.top() - 1);
            // 下側
            painter->drawLine(selectRect.left(), selectRect.bottom() + 2,
                              selectRect.right() + 1, selectRect.bottom() + 2);
            // 左側
            painter->drawLine(selectRect.left() - 1, selectRect.top(),
                              selectRect.left() - 1, selectRect.bottom() + 1);
            // 右側
            painter->drawLine(selectRect.right() + 2, selectRect.top(),
                              selectRect.right() + 2, selectRect.bottom() + 1);
        }
    }

    void PianorollTrackView::paintSubContent(QPainter *painter, const QRect &rect) {
        // カーソル位置でのノート番号を取得する.
        QPoint cursor = QCursor::pos();
        QPoint pianoroll = mapToGlobal(QPoint(0, 0));
        int top = ui->mainContent->getVisibleArea().top();
        int noteAtCursor = getNoteNumberFromY(cursor.y() - pianoroll.y() + top, trackHeight);

        painter->fillRect(0, 0, rect.width(), rect.height(),
                     QColor(240, 240, 240));
        QColor keyNameColor = QColor(72, 77, 98);
        QColor blackKeyColor = QColor(125, 123, 124);
        for (int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++) {
            int y = getYFromNoteNumber(noteNumber, trackHeight) - top;
            int modura = getNoteModuration(noteNumber);

            // C4 などの表示を描画.
            if (modura == 0 || noteAtCursor == noteNumber) {
                painter->setPen(keyNameColor);
                painter->drawText(0, y, rect.width() - 2, trackHeight,
                                   Qt::AlignRight | Qt::AlignVCenter,
                                   keyNames[noteNumber - NOTE_MIN]);
            }

            // 鍵盤ごとの横線.
            painter->setPen(QColor(212, 212, 212));
            painter->drawLine(0, y, rect.width(), y);

            // 黒鍵を描く.
            if (modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10) {
                painter->fillRect(0, y, 34, trackHeight + 1, blackKeyColor);
            }
        }

        // 鍵盤とピアノロール本体との境界線
        painter->setPen(QColor(212, 212, 212));
        painter->drawLine(rect.width() - 1, 0, rect.width() - 1, rect.height());
    }

    void PianorollTrackView::paintBackground(QPainter *g, QRect visibleArea) {
        // 背景
        int height = getYFromNoteNumber(NOTE_MIN - 1, trackHeight) - visibleArea.y();
        g->fillRect(visibleArea.x(), visibleArea.y(),
                     visibleArea.width(), height,
                     QColor(240, 240, 240));

        // 黒鍵
        for (int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++) {
            int y = getYFromNoteNumber(noteNumber, trackHeight);
            int modura = getNoteModuration(noteNumber);

            if (visibleArea.bottom() < y) {
                continue;
            }

            // 黒鍵.
            if (modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10) {
                g->fillRect(visibleArea.x(), y,
                             visibleArea.width(), trackHeight + 1,
                             QColor(212, 212, 212));
            }

            // 白鍵が隣り合う部分に境界線を書く.
            if (modura == 11 || modura == 4) {
                g->setPen(QColor(210, 203, 173));
                g->drawLine(visibleArea.left(), y,
                            visibleArea.right(), y);
            }

            if (y < visibleArea.y()) {
                break;
            }
        }
    }

    void PianorollTrackView::paintItems(QPainter *g, QRect visibleArea) {
        const vsq::Sequence *sequence = controllerAdapter->getSequence();
        if (!sequence) {
            return;
        }
        vsq::Event::List const& list = sequence->track(trackIndex).events();
        int count = list.size();

        static QColor fillColor = QColor(181, 220, 86);
        static QColor hilightFillColor = QColor(100, 149, 237);
        static QColor borderColor = QColor(125, 123, 124);

        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        auto const* eventItemList = manager->getEventItemList();

        for (int i = 0; i < count; i++) {
            vsq::Event const* item = list.get(i);
            if (item->type() != vsq::EventType::NOTE) {
                continue;
            }
            vsq::Event const* actualDrawItem = item;
            QColor color;
            if (manager->isContains(item)) {
                color = hilightFillColor;
                actualDrawItem = eventItemList->at(item).get();
            } else {
                color = fillColor;
            }
            QRect itemRect = getNoteItemRect(actualDrawItem);
            if (itemRect.right() < visibleArea.left()) continue;
            if (visibleArea.right() < itemRect.left()) break;

            if (visibleArea.intersects(itemRect)) {
                paintItem(g, actualDrawItem, itemRect, color, borderColor);
            }
        }

        if (mouseStatus.mode == MouseStatus::LEFTBUTTON_ADD_ITEM) {
            paintItem(g, &mouseStatus.addingNoteItem,
                      getNoteItemRect(&mouseStatus.addingNoteItem), fillColor, borderColor);
        }
    }

    void PianorollTrackView::paintItem(
            QPainter *g, const vsq::Event *item, const QRect &itemRect,
            const QColor &color, const QColor &borderColor) {
        g->fillRect(itemRect, color);
        g->setPen(borderColor);
        g->drawRect(itemRect);

        vsq::Lyric lyric = item->lyricHandle.get(0);
        g->setPen(QColor(0, 0, 0));
        static std::string bra(" [");
        static std::string ket("]");
        std::string text = lyric.phrase + bra + lyric.phoneticSymbol() + ket;
        g->drawText(itemRect.left() + 1, itemRect.bottom() - 1,
                    QString::fromUtf8(text.c_str()));
    }

    int PianorollTrackView::getYFromNoteNumber(int noteNumber, int trackHeight) {
        return (NOTE_MAX - noteNumber) * trackHeight;
    }

    int PianorollTrackView::getNoteNumberFromY(int y, int trackHeight) {
        return NOTE_MAX - static_cast<int>(::floor(static_cast<double>(y / trackHeight)));
    }

    int PianorollTrackView::getNoteModuration(int noteNumber) {
        return ((noteNumber % 12) + 12) % 12;
    }

    int PianorollTrackView::getNoteOctave(int noteNumber) {
        int modura = getNoteModuration(noteNumber);
        return (noteNumber - modura) / 12 - 2;
    }

    void PianorollTrackView::setTrackIndex(int index) {
        trackIndex = index;
        updateWidget();
    }

    void PianorollTrackView::updateWidget() {
        repaint();
    }

    int PianorollTrackView::getTrackViewWidth() {
        return ui->mainContent->width();
    }

    /**
     * @todo Shift を押しながら音符イベントにマウスがおろされたとき、直前に選択した音符イベントがある場合、
     * この２つの音符イベントの間にある音符イベントをすべて選択状態とする動作を実装する
     * @todo 既に選択されたアイテムをさらに MousePress した場合の処理を要検討。現状だと、Ctrl を押さないままアイテムを移動しようと
     * すると、選択状態だった他のアイテムが選択から外れてしまう。
     */
    void PianorollTrackView::handleMouseLeftButtonPressByPointer(QMouseEvent *event) {
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const vsq::Event *noteEventOnMouse = findNoteEventAt(event->pos());
        if (noteEventOnMouse) {
            initMouseStatus(MouseStatus::LEFTBUTTON_MOVE_ITEM, event, noteEventOnMouse);
            manager->add(noteEventOnMouse);
        } else {
            if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
                manager->clear();
            }
            initMouseStatus(MouseStatus::LEFTBUTTON_SELECT_ITEM, event, noteEventOnMouse);
            hideLyricEdit();
        }
        updateWidget();
    }

    void PianorollTrackView::handleMouseLeftButtonPressByEraser(QMouseEvent *event) {
        const vsq::Event *noteEventOnMouse = findNoteEventAt(event->pos());
        controllerAdapter->removeEvent(trackIndex, noteEventOnMouse);
        if (!noteEventOnMouse) {
            initMouseStatus(MouseStatus::LEFTBUTTON_SELECT_ITEM, event, noteEventOnMouse);
        }
        updateWidget();
    }

    void PianorollTrackView::handleMouseLeftButtonPressByPencil(QMouseEvent *event) {
        const vsq::Event *noteEventOnMouse = findNoteEventAt(event->pos());
        if (noteEventOnMouse) {
            initMouseStatus(MouseStatus::LEFTBUTTON_MOVE_ITEM, event, noteEventOnMouse);
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
                manager->clear();
            }
            manager->add(noteEventOnMouse);
            updateWidget();
        } else {
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            bool repaint = false;
            if (!manager->getEventItemList()->empty()) {
                manager->clear();
                repaint = true;
            }

            initMouseStatus(MouseStatus::LEFTBUTTON_ADD_ITEM, event, noteEventOnMouse);
            QPoint mousePosition = mapToScene(event->pos());
            int note = getNoteNumberFromY(mousePosition.y(), trackHeight);
            vsq::tick_t clock = controllerAdapter->getTickFromX(mousePosition.x());
            clock = quantize(clock);
            mouseStatus.addingNoteItem = vsq::Event(clock, vsq::EventType::NOTE);
            mouseStatus.addingNoteItem.note = note;

            hideLyricEdit();

            if (repaint) updateWidget();
        }
    }

    void PianorollTrackView::handleMouseMiddleButtonPress(QMouseEvent *event) {
        initMouseStatus(MouseStatus::MIDDLEBUTTON_SCROLL, event, 0);
    }

    const vsq::Event *PianorollTrackView::findNoteEventAt(const QPoint &mousePosition) {
        vsq::Sequence const* sequence = controllerAdapter->getSequence();
        vsq::Event::List const& list = sequence->track(trackIndex).events();
        int count = list.size();
        QPoint sceneMousePos = mapToScene(mousePosition);

        for (int i = 0; i < count; i++) {
            vsq::Event const* item = list.get(i);
            if (item->type() != vsq::EventType::NOTE) {
                continue;
            }
            QRect itemRect = getNoteItemRect(item);
            if (itemRect.contains(sceneMousePos)) {
                return item;
            }
        }

        return 0;
    }

    QRect PianorollTrackView::getNoteItemRect(const vsq::Event *item) {
        vsq::tick_t tick = item->tick;
        int x = controllerAdapter->getXFromTick(tick);
        int width = controllerAdapter->getXFromTick(tick + item->length()) - x;
        int y = getYFromNoteNumber(item->note, trackHeight);
        return QRect(x, y, width, trackHeight);
    }

    QPoint PianorollTrackView::mapToScene(const QPoint &mousePos) {
        return ui->mainContent->mapToScene(mousePos).toPoint();
    }

    void PianorollTrackView::onMousePressSlot(QMouseEvent *event) {
        ToolKind::ToolKindEnum tool = controllerAdapter->getToolKind();
        Qt::MouseButton button = event->button();

        if (button == Qt::LeftButton) {
            if (lyricEdit->isVisible()) {
                hideLyricEdit();
            }

            if (tool == ToolKind::POINTER) {
                handleMouseLeftButtonPressByPointer(event);
            } else if (tool == ToolKind::ERASER) {
                handleMouseLeftButtonPressByEraser(event);
            } else if (tool == ToolKind::PENCIL || tool == ToolKind::LINE) {
                // treat PENCIL and LINE as if equal
                handleMouseLeftButtonPressByPencil(event);
            }
        } else if (button == Qt::MidButton) {
            handleMouseMiddleButtonPress(event);
        }
    }

    void PianorollTrackView::onMouseMoveSlot(QMouseEvent *event) {
        if (mouseStatus.mode == MouseStatus::LEFTBUTTON_SELECT_ITEM) {
            mouseStatus.endPosition = mapToScene(event->pos());
            updateSelectedItem();
            updateWidget();
        } else if (mouseStatus.mode == MouseStatus::MIDDLEBUTTON_SCROLL) {
            QPoint globalMousePos = ui->mainContent->mapToGlobal(event->pos());
            int deltaX = globalMousePos.x() - mouseStatus.globalStartPosition.x();
            int deltaY = globalMousePos.y() - mouseStatus.globalStartPosition.y();

            ui->mainContent->horizontalScrollBar()
                          ->setValue(mouseStatus.horizontalScrollStartValue - deltaX);
            ui->mainContent->verticalScrollBar()
                          ->setValue(mouseStatus.verticalScrollStartValue - deltaY);
            updateWidget();
        } else if (mouseStatus.mode == MouseStatus::LEFTBUTTON_MOVE_ITEM) {
            QPoint currentMousePos = mapToScene(event->pos());

            // マウスの移動量から、クロック・ノートの移動量を算出
            vsq::tick_t deltaClocks = controllerAdapter->getTickFromX(currentMousePos.x())
                    - controllerAdapter->getTickFromX(mouseStatus.startPosition.x());
            if (mouseStatus.noteOnMouse) {
                vsq::tick_t editedNoteClock
                        = quantize(mouseStatus.noteOnMouse->tick + deltaClocks);
                deltaClocks = editedNoteClock - mouseStatus.noteOnMouse->tick;
            }
            int deltaNoteNumbers = getNoteNumberFromY(currentMousePos.y(), trackHeight)
                    - getNoteNumberFromY(mouseStatus.startPosition.y(), trackHeight);

            // 選択されたアイテムすべてについて、移動を適用する
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            manager->moveItems(deltaClocks, deltaNoteNumbers);
            updateWidget();
        } else if (mouseStatus.mode == MouseStatus::LEFTBUTTON_ADD_ITEM) {
            QPoint currentMousePos = mapToScene(event->pos());
            vsq::tick_t endClock = controllerAdapter->getTickFromX(currentMousePos.x());
            vsq::tick_t length = 0;
            if (mouseStatus.addingNoteItem.tick < endClock) {
                length = endClock - mouseStatus.addingNoteItem.tick;
                length = quantize(length);
            }
            mouseStatus.addingNoteItem.length(length);
            updateWidget();
        }
        mouseStatus.isMouseMoved = true;
    }

    void PianorollTrackView::onMouseReleaseSlot(QMouseEvent *event) {
        mouseStatus.endPosition = mapToScene(event->pos());
        if (mouseStatus.mode == MouseStatus::LEFTBUTTON_MOVE_ITEM) {
            if (mouseStatus.isMouseMoved) {
                ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
                EditEventCommand command = manager->getEditEventCommand(trackIndex);
                controllerAdapter->execute(&command);
            } else {
                ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
                const vsq::Event *noteEventOnMouse = findNoteEventAt(event->pos());
                if (noteEventOnMouse) {
                    if ((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
                        manager->clear();
                    }
                    if (mouseStatus.itemSelectionStatusAtFirst.isContains(noteEventOnMouse)) {
                        manager->remove(noteEventOnMouse);
                    } else {
                        manager->add(noteEventOnMouse);
                    }
                }
            }
        } else if (mouseStatus.mode == MouseStatus::LEFTBUTTON_ADD_ITEM) {
            if (0 < mouseStatus.addingNoteItem.length()) {
                std::vector<vsq::Event> eventList;
                eventList.push_back(mouseStatus.addingNoteItem);
                AddEventCommand command(trackIndex, eventList);
                controllerAdapter->execute(&command);
            }
        }
        mouseStatus.mode = MouseStatus::NONE;
        updateWidget();
    }

    void PianorollTrackView::onMouseDoubleClickSlot(QMouseEvent *event) {
        const vsq::Event *noteOnMouse = findNoteEventAt(event->pos());
        if (noteOnMouse) {
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            if (!manager->isContains(noteOnMouse) || manager->getEventItemList()->size() != 1) {
                manager->clear();
                manager->add(noteOnMouse);
            }
            showLyricEdit(noteOnMouse);
        }
    }

    /**
     * @todo パフォーマンス悪いので改善する。
     * 例えば、以下の改善策がある。
     * (1) 矩形内にアイテムが一つもなかった場合は、特に何もしない
     */
    void PianorollTrackView::updateSelectedItem() {
        const vsq::Sequence *sequence = controllerAdapter->getSequence();
        if (!sequence) {
            return;
        }

        // 選択状態を最初の状態に戻す.
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        manager->revertSelectionStatusTo(mouseStatus.itemSelectionStatusAtFirst);

        // 矩形に入っているアイテムを、選択状態とする.
        // ただし、矩形選択直前に選択状態となっているものは選択状態を解除する.
        QRect rect = mouseStatus.rect();
        vsq::Event::List const& list = sequence->track(trackIndex).events();
        int count = list.size();
        std::set<const vsq::Event *> add;
        std::set<const vsq::Event *> remove;
        for (int i = 0; i < count; i++) {
            const vsq::Event *item = list.get(i);
            if (item->type() != vsq::EventType::NOTE) {
                continue;
            }
            QRect itemRect = getNoteItemRect(item);
            if (itemRect.right() < rect.left()) {
                continue;
            }
            if (rect.right() < itemRect.left()) {
                break;
            }

            if (rect.intersects(itemRect)) {
                if (mouseStatus.itemSelectionStatusAtFirst.isContains(item)) {
                    remove.insert(item);
                } else {
                    add.insert(item);
                }
            }
        }
        manager->add(add);
        manager->remove(remove);
    }

    vsq::tick_t PianorollTrackView::quantize(vsq::tick_t tick) {
        QuantizeMode::QuantizeModeEnum mode = Settings::instance()->getQuantizeMode();
        bool triplet = false;  // TODO(kbinani): consider triplet mode.

        vsq::tick_t unitTick = QuantizeMode::getQuantizeUnitTick(mode);
        if (triplet && 1 < unitTick) {
            unitTick = unitTick * 2 / 3;
        }

        vsq::tick_t odd = tick % unitTick;
        vsq::tick_t newTick = tick - odd;
        if (odd > unitTick / 2) {
            newTick += unitTick;
        }
        return newTick;
    }

    void PianorollTrackView::initMouseStatus(
            MouseStatus::MouseStatusEnum status, const QMouseEvent *event,
            const vsq::Event *noteOnMouse) {
        mouseStatus.mode = status;
        mouseStatus.startPosition = mapToScene(event->pos());
        mouseStatus.endPosition = mouseStatus.startPosition;
        mouseStatus.horizontalScrollStartValue = ui->mainContent->horizontalScrollBar()->value();
        mouseStatus.verticalScrollStartValue = ui->mainContent->verticalScrollBar()->value();
        mouseStatus.globalStartPosition = ui->mainContent->mapToGlobal(event->pos());
        mouseStatus.isMouseMoved = false;
        mouseStatus.itemSelectionStatusAtFirst = *controllerAdapter->getItemSelectionManager();
        mouseStatus.noteOnMouse = noteOnMouse;
    }

    void PianorollTrackView::onContentScroll(int value) {
        updateLyricEditComponentPosition();
    }

    void PianorollTrackView::updateLyricEditComponentPosition() {
        int x = ui->mainContent->horizontalScrollBar()->value();
        int y = ui->mainContent->verticalScrollBar()->value();
        lyricEdit->move(lyricEdit->scenePosition.x() - x, lyricEdit->scenePosition.y() - y);
        updateWidget();
    }

    QPoint PianorollTrackView::getLyricEditPosition(const vsq::Event *noteEvent) {
        QRect noteRect = getNoteItemRect(noteEvent);
        int y = noteRect.y() - (lyricEdit->height() - noteRect.height()) / 2;
        return QPoint(noteRect.x(), y);
    }

    void PianorollTrackView::onLyricEditCommitSlot() {
        const vsq::Event *event = lyricEdit->event();
        if (!event) return;

        vsq::Lyric originalLyric = event->lyricHandle.size() == 0
                ? vsq::Lyric("a", "a")
                : event->lyricHandle.get(0);

        // TODO(kbinani): separate into phrases

        std::string word;
        std::string symbol;
        if (lyricEdit->symbolEditMode) {
            word = originalLyric.phrase;
            symbol = lyricEdit->text().toStdString();
        } else {
            word = lyricEdit->text().toStdString();

            if (originalLyric.isProtected) {
                symbol = originalLyric.phoneticSymbol();
            } else {
                symbol = "a";
                const vsq::PhoneticSymbolDictionary::Element *element
                        = controllerAdapter->attachPhoneticSymbol(word);
                if (element) symbol = element->symbol();
            }
        }

        vsq::Lyric lyric(word, symbol);
        lyric.isProtected = lyricEdit->symbolEditMode
                ? true
                : originalLyric.isProtected;
        vsq::Event edited = *lyricEdit->event();
        if (edited.lyricHandle.size() == 0) {
            edited.lyricHandle.add(lyric);
        } else {
            edited.lyricHandle.set(0, lyric);
        }

        if (edited.lyricHandle.size() != event->lyricHandle.size() ||
                !lyric.equals(originalLyric)) {
            EditEventCommand command(trackIndex, lyricEdit->event()->id, edited);
            controllerAdapter->execute(&command);
            updateWidget();
        }
    }

    void PianorollTrackView::onLyricEditHideSlot() {
        hideLyricEdit();
    }

    void PianorollTrackView::onLyricEditMoveSlot(bool isBackward) {
        if (!lyricEdit->event()) return;

        int id = lyricEdit->event()->id;
        vsq::Track const& track = controllerAdapter->getSequence()->track(trackIndex);
        vsq::Event::List const& events = track.events();
        int index = events.findIndexFromId(id);

        // find forward/backward note item.
        const vsq::Event *moveTo = 0;
        int step = isBackward ? -1 : 1;
        int i = index + step;
        while (0 <= i && i < events.size()) {
            const vsq::Event *item = events.get(i);
            if (vsq::EventType::NOTE == item->type()) {
                moveTo = item;
                break;
            }
            i += step;
        }

        if (moveTo) {
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            manager->clear();
            manager->add(moveTo);
            showLyricEdit(moveTo);
        } else {
            hideLyricEdit();
        }
    }

    void PianorollTrackView::showLyricEdit(vsq::Event const* note) {
        controllerAdapter->setApplicationShortcutEnabled(false);
        lyricEdit->setEnabled(true);
        lyricEdit->setupText(note);
        lyricEdit->setVisible(true);
        lyricEdit->setFocus();
        lyricEdit->scenePosition = getLyricEditPosition(note);
        updateLyricEditComponentPosition();
    }

    void PianorollTrackView::hideLyricEdit() {
        lyricEdit->setVisible(false);
        lyricEdit->setupText(0);
        lyricEdit->setEnabled(false);
        controllerAdapter->setApplicationShortcutEnabled(true);
    }

    PianorollTrackView::MouseStatus::MouseStatus()
        : addingNoteItem(vsq::Event::eos())
    {
        clear();
    }

    QRect PianorollTrackView::MouseStatus::rect()const {
        int x = std::min(startPosition.x(), endPosition.x());
        int y = std::min(startPosition.y(), endPosition.y());
        int width = std::abs(startPosition.x() - endPosition.x());
        int height = std::abs(startPosition.y() - endPosition.y());
        return QRect(x, y, width, height);
    }

    void PianorollTrackView::MouseStatus::clear() {
        mode = NONE;
        startPosition = QPoint();
        endPosition = QPoint();
        horizontalScrollStartValue = 0;
        verticalScrollStartValue = 0;
        globalStartPosition = QPoint();
        isMouseMoved = false;
        itemSelectionStatusAtFirst = ItemSelectionManager();
        noteOnMouse = 0;
    }
}
