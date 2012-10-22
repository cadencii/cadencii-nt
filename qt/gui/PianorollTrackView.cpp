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
#include "PianorollTrackView.hpp"
#include "ui_EditorWidgetBase.h"
#include <QScrollBar>
#include <QMouseEvent>
#include "../../vsq/Event.hpp"
#include "../../command/DeleteEventCommand.hpp"

namespace cadencii{

    PianorollTrackView::PianorollTrackView( QWidget *parent ) :
        EditorWidgetBase( parent )
    {
        mutex = 0;
        trackHeight = DEFAULT_TRACK_HEIGHT;
        trackIndex = 0;
        ui->scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
        ui->scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );

        // キーボードのキーの音名を作成
        keyNames = new QString[NOTE_MAX - NOTE_MIN + 1];
        char *names[] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B" };
        for( int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++ ){
            int modura = getNoteModuration( noteNumber );
            int order = getNoteOctave( noteNumber );
            char *name = names[modura];
            ostringstream oss;
            oss << name << order;
            keyNames[noteNumber - NOTE_MIN] = QString( oss.str().c_str() );
        }
        connect( ui->scrollArea, SIGNAL(onMousePress(QMouseEvent*)), this, SLOT(onMousePressSlot(QMouseEvent*)) );
        connect( ui->scrollArea, SIGNAL(onMouseMove(QMouseEvent*)), this, SLOT(onMouseMoveSlot(QMouseEvent*)) );
        connect( ui->scrollArea, SIGNAL(onMouseRelease(QMouseEvent*)), this, SLOT(onMouseReleaseSlot(QMouseEvent*)) );
    }

    PianorollTrackView::~PianorollTrackView(){
        delete [] keyNames;
    }

    void *PianorollTrackView::getWidget(){
        return (void *)this;
    }

    void PianorollTrackView::setDrawOffset( VSQ_NS::tick_t drawOffset ){
        setDrawOffsetInternal( drawOffset );
    }

    void PianorollTrackView::setControllerAdapter( ControllerAdapter *controllerAdapter ){
        this->controllerAdapter = controllerAdapter;
    }

    void *PianorollTrackView::getScrollEventSender(){
        return static_cast<TrackView *>( this );
    }

    QSize PianorollTrackView::getPreferedSceneSize(){
        QScrollBar *scrollBar = ui->scrollArea->verticalScrollBar();
        int width = controllerAdapter->getPreferedComponentWidth() - scrollBar->width();
        int height = trackHeight * (NOTE_MAX - NOTE_MIN + 1);
        return QSize( width, height );
    }

    void PianorollTrackView::ensureNoteVisible( VSQ_NS::tick_t tick, VSQ_NS::tick_t length, int noteNumber ){
        int left = controllerAdapter->getXFromTick( tick );
        int right = controllerAdapter->getXFromTick( tick + length );

        QRect visibleArea = ui->scrollArea->getVisibleArea();
        QScrollBar *horizontalScrollBar = ui->scrollArea->horizontalScrollBar();
        QScrollBar *verticalScrollBar = ui->scrollArea->verticalScrollBar();
        int dx = 0;
        int newValue = verticalScrollBar->value();
        if( visibleArea.right() < right ){
            dx = ui->scrollArea->width() - (right - left);
        }else if( left < visibleArea.left() ){
            dx = -ui->scrollArea->width() + (right - left);
        }
        if( 0 <= noteNumber ){
            int top = getYFromNoteNumber( noteNumber, trackHeight );
            int bottom = top + trackHeight;
            if( top < visibleArea.top() || visibleArea.bottom() < bottom ){
                newValue = (bottom + top) / 2 - visibleArea.height() / 2;
            }

            if( verticalScrollBar->value() != newValue ){
                if( newValue < verticalScrollBar->minimum() ){
                    verticalScrollBar->setValue( verticalScrollBar->minimum() );
                }else if( verticalScrollBar->maximum() < newValue ){
                    verticalScrollBar->setValue( verticalScrollBar->maximum() );
                }else{
                    verticalScrollBar->setValue( newValue );
                }
            }
        }
        if( dx ){
            int value = horizontalScrollBar->value() + dx;
            if( value < horizontalScrollBar->minimum() ){
                horizontalScrollBar->setValue( horizontalScrollBar->minimum() );
            }else if( horizontalScrollBar->maximum() < value ){
                horizontalScrollBar->setValue( horizontalScrollBar->maximum() );
            }else{
                horizontalScrollBar->setValue( value );
            }
        }
    }

    void PianorollTrackView::setMutex( QMutex *mutex ){
        this->mutex = mutex;
    }

    void PianorollTrackView::paintMainContent( QPainter *painter, const QRect &rect ){
        paintBackground( painter, rect );
        ui->scrollArea->paintMeasureLines( painter, rect );
        if( mutex ){
            mutex->lock();
            paintItems( painter, rect );
            mutex->unlock();
        }else{
            paintItems( painter, rect );
        }
        ui->scrollArea->paintSongPosition( painter, rect );

        // 矩形選択の範囲を描画する
        if( mouseStatus.mode == MouseStatus::LEFTBUTTON_SELECT_ITEM ){
            QRect selectRect = mouseStatus.rect();
            static QColor fillColor( 0, 0, 0, 100 );
            static QColor borderColor( 0, 0, 0, 200 );
            static QColor shadowColor( 0, 0, 0, 40 );
            painter->fillRect( selectRect, fillColor );
            painter->setPen( borderColor );
            painter->drawRect( selectRect );

            // 選択範囲の周囲に影を描く
            painter->setPen( shadowColor );
            // 上側
            painter->drawLine( selectRect.left(), selectRect.top() - 1, selectRect.right() + 1, selectRect.top() - 1 );
            // 下側
            painter->drawLine( selectRect.left(), selectRect.bottom() + 2, selectRect.right() + 1, selectRect.bottom() + 2 );
            // 左側
            painter->drawLine( selectRect.left() - 1, selectRect.top(), selectRect.left() - 1, selectRect.bottom() + 1 );
            // 右側
            painter->drawLine( selectRect.right() + 2, selectRect.top(), selectRect.right() + 2, selectRect.bottom() + 1 );
        }
    }

    void PianorollTrackView::paintSubContent( QPainter *painter, const QRect &rect ){
        // カーソル位置でのノート番号を取得する
        QPoint cursor = QCursor::pos();
        QPoint pianoroll = mapToGlobal( QPoint( 0, 0 ) );
        int top = ui->scrollArea->getVisibleArea().top();
        int noteAtCursor = getNoteNumberFromY( cursor.y() - pianoroll.y() + top, trackHeight );

        painter->fillRect( 0, 0, rect.width(), rect.height(),
                     QColor( 240, 240, 240 ) );
        QColor keyNameColor = QColor( 72, 77, 98 );
        QColor blackKeyColor = QColor( 125, 123, 124 );
        for( int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++ ){
            int y = getYFromNoteNumber( noteNumber, trackHeight ) - top;
            int modura = getNoteModuration( noteNumber );

            // C4 などの表示を描画
            if( modura == 0 || noteAtCursor == noteNumber ){
                painter->setPen( keyNameColor );
                painter->drawText( 0, y, rect.width() - 2, trackHeight,
                                   Qt::AlignRight | Qt::AlignVCenter,
                                   keyNames[noteNumber - NOTE_MIN] );
            }

            // 鍵盤ごとの横線
            painter->setPen( QColor( 212, 212, 212 ) );
            painter->drawLine( 0, y, rect.width(), y );

            // 黒鍵を描く
            if( modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10 ){
                painter->fillRect( 0, y, 34, trackHeight + 1, blackKeyColor );
            }
        }

        // 鍵盤とピアノロール本体との境界線
        painter->setPen( QColor( 212, 212, 212 ) );
        painter->drawLine( rect.width() - 1, 0, rect.width() - 1, rect.height() );
    }

    void PianorollTrackView::paintBackground( QPainter *g, QRect visibleArea ){
        // 背景
        int height = getYFromNoteNumber( NOTE_MIN - 1, trackHeight ) - visibleArea.y();
        g->fillRect( visibleArea.x(), visibleArea.y(),
                     visibleArea.width(), height,
                     QColor( 240, 240, 240 ) );

        // 黒鍵
        for( int noteNumber = NOTE_MIN; noteNumber <= NOTE_MAX; noteNumber++ ){
            int y = getYFromNoteNumber( noteNumber, trackHeight );
            int modura = getNoteModuration( noteNumber );

            if( visibleArea.bottom() < y ){
                continue;
            }

            // 黒鍵
            if( modura == 1 || modura == 3 || modura == 6 || modura == 8 || modura == 10 ){
                g->fillRect( visibleArea.x(), y,
                             visibleArea.width(), trackHeight + 1,
                             QColor( 212, 212, 212 ) );
            }

            // 白鍵が隣り合う部分に境界線を書く
            if( modura == 11 || modura == 4 ){
                g->setPen( QColor( 210, 203, 173 ) );
                g->drawLine( visibleArea.left(), y,
                             visibleArea.right(), y );
            }

            if( y < visibleArea.y() ){
                break;
            }
        }
    }

    void PianorollTrackView::paintItems( QPainter *g, QRect visibleArea ){
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        if( !sequence ){
            return;
        }
        const VSQ_NS::Event::List *list = sequence->track[trackIndex].getConstEvents();
        int count = list->size();

        static QColor fillColor = QColor( 181, 220, 86 );
        static QColor hilightFillColor = QColor( 100, 149, 237 );
        static QColor borderColor = QColor( 125, 123, 124 );

        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *eventItemList = manager->getEventItemList();

        for( int i = 0; i < count; i++ ){
            const VSQ_NS::Event *item = list->get( i );
            if( item->type != VSQ_NS::EventType::NOTE ) continue;
            const VSQ_NS::Event *actualDrawItem = item;
            QColor color;
            if( manager->isContains( item ) ){
                color = hilightFillColor;
                actualDrawItem = &eventItemList->at( item );
            }else{
                color = fillColor;
            }
            QRect itemRect = getNoteItemRect( actualDrawItem );

            if( visibleArea.intersects( itemRect ) ){
                g->fillRect( itemRect, color );
                g->setPen( borderColor );
                g->drawRect( itemRect );

                VSQ_NS::Lyric lyric = actualDrawItem->lyricHandle.getLyricAt( 0 );
                g->setPen( QColor( 0, 0, 0 ) );
                g->drawText( itemRect.left() + 1, itemRect.bottom() - 1,
                             QString::fromUtf8( (lyric.phrase + " [" + lyric.getPhoneticSymbol() + "]").c_str() ) );
            }
        }
    }

    int PianorollTrackView::getYFromNoteNumber( int noteNumber, int trackHeight ){
        return (NOTE_MAX - noteNumber) * trackHeight;
    }

    int PianorollTrackView::getNoteNumberFromY( int y, int trackHeight ){
        return NOTE_MAX - (int)::floor( (double)(y / trackHeight) );
    }

    int PianorollTrackView::getNoteModuration( int noteNumber ){
        return ((noteNumber % 12) + 12) % 12;
    }

    int PianorollTrackView::getNoteOctave( int noteNumber ){
        int modura = getNoteModuration( noteNumber );
        return (noteNumber - modura) / 12 - 2;
    }

    void PianorollTrackView::setTrackIndex( int index ){
        trackIndex = index;
        updateWidget();
    }

    void PianorollTrackView::updateWidget(){
        repaint();
    }

    int PianorollTrackView::getTrackViewWidth(){
        return ui->scrollArea->width();
    }

    /**
     * @todo Shift を押しながら音符イベントにマウスがおろされたとき、直前に選択した音符イベントがある場合、
     * この２つの音符イベントの間にある音符イベントをすべて選択状態とする動作を実装する
     * @todo 既に選択されたアイテムをさらに MousePress した場合の処理を要検討。現状だと、Ctrl を押さないままアイテムを移動しようと
     * すると、選択状態だった他のアイテムが選択から外れてしまう。
     */
    void PianorollTrackView::handleMouseLeftButtonPressByPointer( QMouseEvent *event ){
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const VSQ_NS::Event *noteEventOnMouse = findNoteEventAt( event->pos() );
        if( noteEventOnMouse ){
            if( (event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier ){
                manager->clear();
            }
            manager->add( noteEventOnMouse );
            initMouseStatus( MouseStatus::LEFTBUTTON_MOVE_ITEM, event );
        }else{
            initMouseStatus( MouseStatus::LEFTBUTTON_SELECT_ITEM, event );
            manager->clear();
        }
        updateWidget();
    }

    void PianorollTrackView::handleMouseLeftButtonPressByEraser( QMouseEvent *event ){
        const VSQ_NS::Event *noteEventOnMouse = findNoteEventAt( event->pos() );
        controllerAdapter->removeEvent( trackIndex, noteEventOnMouse );
        if( !noteEventOnMouse ){
            initMouseStatus( MouseStatus::LEFTBUTTON_SELECT_ITEM, event );
        }
        updateWidget();
    }

    void PianorollTrackView::handleMouseMiddleButtonPress( QMouseEvent *event ){
        initMouseStatus( MouseStatus::MIDDLEBUTTON_SCROLL, event );
    }

    const VSQ_NS::Event *PianorollTrackView::findNoteEventAt( const QPoint &mousePosition ){
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        const VSQ_NS::Event::List *list = sequence->track[trackIndex].getConstEvents();
        int count = list->size();
        QPoint sceneMousePos = mapToScene( mousePosition );

        for( int i = 0; i < count; i++ ){
            const VSQ_NS::Event *item = list->get( i );
            if( item->type != VSQ_NS::EventType::NOTE ) continue;
            QRect itemRect = getNoteItemRect( item );
            if( itemRect.contains( sceneMousePos ) ){
                return item;
            }
        }

        return 0;
    }

    QRect PianorollTrackView::getNoteItemRect( const VSQ_NS::Event *item ){
        VSQ_NS::tick_t tick = item->clock;
        int x = controllerAdapter->getXFromTick( tick );
        int width = controllerAdapter->getXFromTick( tick + item->getLength() ) - x;
        int y = getYFromNoteNumber( item->note, trackHeight );
        return QRect( x, y, width, trackHeight );
    }

    QPoint PianorollTrackView::mapToScene( const QPoint &mousePos ){
        return ui->scrollArea->mapToScene( mousePos ).toPoint();
    }

    void PianorollTrackView::onMousePressSlot( QMouseEvent *event ){
        ToolKind::ToolKindEnum tool = controllerAdapter->getToolKind();
        Qt::MouseButton button = event->button();

        if( button == Qt::LeftButton ){
            if( tool == ToolKind::POINTER ){
                handleMouseLeftButtonPressByPointer( event );
            }else if( tool == ToolKind::ERASER ){
                handleMouseLeftButtonPressByEraser( event );
            }
        }else if( button == Qt::MidButton ){
            handleMouseMiddleButtonPress( event );
        }
    }

    void PianorollTrackView::onMouseMoveSlot( QMouseEvent *event ){
        if( mouseStatus.mode == MouseStatus::LEFTBUTTON_SELECT_ITEM ){
            mouseStatus.endPosition = mapToScene( event->pos() );
            updateSelectedItem();
            updateWidget();
        }else if( mouseStatus.mode == MouseStatus::MIDDLEBUTTON_SCROLL ){
            QPoint globalMousePos = ui->scrollArea->mapToGlobal( event->pos() );
            int deltaX = globalMousePos.x() - mouseStatus.globalStartPosition.x();
            int deltaY = globalMousePos.y() - mouseStatus.globalStartPosition.y();

            ui->scrollArea->horizontalScrollBar()->setValue( mouseStatus.horizontalScrollStartValue - deltaX );
            ui->scrollArea->verticalScrollBar()->setValue( mouseStatus.verticalScrollStartValue - deltaY );
            updateWidget();
        }else if( mouseStatus.mode == MouseStatus::LEFTBUTTON_MOVE_ITEM ){
            QPoint currentMousePos = mapToScene( event->pos() );

            // マウスの移動量から、クロック・ノートの移動量を算出
            VSQ_NS::tick_t deltaClocks = controllerAdapter->getTickFromX( currentMousePos.x() )
                    - controllerAdapter->getTickFromX( mouseStatus.startPosition.x() );
            int deltaNoteNumbers = getNoteNumberFromY( currentMousePos.y(), trackHeight )
                    - getNoteNumberFromY( mouseStatus.startPosition.y(), trackHeight );

            // 選択されたアイテムすべてについて、移動を適用する
            ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
            manager->moveItems( deltaClocks, deltaNoteNumbers );
            updateWidget();
        }
        mouseStatus.isMouseMoved = true;
    }

    void PianorollTrackView::onMouseReleaseSlot( QMouseEvent *event ){
        mouseStatus.endPosition = mapToScene( event->pos() );
        //TODO: mouseStatus.mode == MouseStatus.LEFTBUTTON_MOVE_ITEMS の場合の処理
        mouseStatus.mode = MouseStatus::NONE;
        updateWidget();
    }

    void PianorollTrackView::updateSelectedItem(){
        const VSQ_NS::Sequence *sequence = controllerAdapter->getSequence();
        if( !sequence ){
            return;
        }

        const VSQ_NS::Event::List *list = sequence->track[trackIndex].getConstEvents();
        int count = list->size();

        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        QRect rect = mouseStatus.rect();

        for( int i = 0; i < count; i++ ){
            const VSQ_NS::Event *item = list->get( i );
            if( item->type != VSQ_NS::EventType::NOTE ) continue;
            QRect itemRect = getNoteItemRect( item );

            if( rect.intersects( itemRect ) ){
                manager->add( item );
            }else{
                manager->remove( item );
            }
        }
    }

    void PianorollTrackView::initMouseStatus( MouseStatus::MouseStatusEnum status, const QMouseEvent *event ){
        mouseStatus.mode = status;
        mouseStatus.startPosition = mapToScene( event->pos() );
        mouseStatus.endPosition = mouseStatus.startPosition;
        mouseStatus.horizontalScrollStartValue = ui->scrollArea->horizontalScrollBar()->value();
        mouseStatus.verticalScrollStartValue = ui->scrollArea->verticalScrollBar()->value();
        mouseStatus.globalStartPosition = ui->scrollArea->mapToGlobal( event->pos() );
        mouseStatus.isMouseMoved = false;
    }

    PianorollTrackView::MouseStatus::MouseStatus(){
        clear();
    }

    QRect PianorollTrackView::MouseStatus::rect()const{
        int x = std::min( startPosition.x(), endPosition.x() );
        int y = std::min( startPosition.y(), endPosition.y() );
        int width = std::abs( startPosition.x() - endPosition.x() );
        int height = std::abs( startPosition.y() - endPosition.y() );
        return QRect( x, y, width, height );
    }

    void PianorollTrackView::MouseStatus::clear(){
        mode = NONE;
        startPosition = QPoint();
        endPosition = QPoint();
        horizontalScrollStartValue = 0;
        verticalScrollStartValue = 0;
        globalStartPosition = QPoint();
        isMouseMoved = false;
    }

}
