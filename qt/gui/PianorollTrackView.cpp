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
#include "vsq/Event.hpp"

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
    }

    PianorollTrackView::~PianorollTrackView(){
        delete [] keyNames;
    }

    void PianorollTrackView::setSequence( VSQ_NS::Sequence *sequence ){
        this->sequence = sequence;
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
        if( sequence == NULL ){
            return;
        }
        VSQ_NS::Event::List *list = sequence->track[trackIndex].getEvents();
        int count = list->size();
        int height = trackHeight - 1;

        QColor fillColor = QColor( 181, 220, 86 );
        QColor borderColor = QColor( 125, 123, 124 );

        int visibleMinX = visibleArea.left();
        int visibleMaxX = visibleArea.right();
        int visibleMinY = visibleArea.top();
        int visibleMaxY = visibleArea.bottom();

        for( int i = 0; i < count; i++ ){
            VSQ_NS::Event item = list->get( i );
            if( item.type != VSQ_NS::EventType::NOTE ) continue;
            VSQ_NS::tick_t tick = item.clock;
            int x = controllerAdapter->getXFromTick( tick );
            int width = controllerAdapter->getXFromTick( tick + item.getLength() ) - x;

            if( visibleMinX <= x + width && x <= visibleMaxX ){
                int y = getYFromNoteNumber( item.note, trackHeight ) + 1;
                if( visibleMinY <= y + height && y <= visibleMaxY ){
                    g->fillRect( x, y, width, height, fillColor );
                    g->setPen( borderColor );
                    g->drawRect( x, y, width, height );

                    VSQ_NS::Lyric lyric = item.lyricHandle.getLyricAt( 0 );
                    g->setPen( QColor( 0, 0, 0 ) );
                    g->drawText( x + 1, y + trackHeight - 2,
                                 QString::fromUtf8( (lyric.phrase + " [" + lyric.getPhoneticSymbol() + "]").c_str() ) );
                }
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

}
