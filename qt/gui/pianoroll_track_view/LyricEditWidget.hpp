/**
 * LyricEditWidget.hpp
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
#ifndef CADENCII_QT_GUI_PIANOROLL_TRACK_VIEW_LYRICEDITWIDGET_HPP_
#define CADENCII_QT_GUI_PIANOROLL_TRACK_VIEW_LYRICEDITWIDGET_HPP_

#include "../../../vsq/Event.hpp"
#include <QLineEdit>
#include <QKeyEvent>

namespace cadencii {

    /**
     * @brief A widget for lyric editing.
     */
    class LyricEditWidget : public QLineEdit {
        Q_OBJECT

    public:
        /**
         * @brief Position of 'lyricEdit' component, based on scene geometry.
         */
        QPoint scenePosition;

        /**
         * @brief Note event corresponding lyric.
         */
        const VSQ_NS::Event *event;

    public:
        explicit LyricEditWidget(QWidget *parent = 0) :
            QLineEdit(parent), event(0)
        {
        }

        void keyPressEvent(QKeyEvent *event) {
            int key = event->key();
            if (Qt::Key_Tab == key) {
                emit onCommit();
                emit onMove(false);
            } else if (Qt::Key_Backtab == key) {
                emit onCommit();
                emit onMove(true);
            } else if (Qt::Key_Return == key) {
                emit onCommit();
                emit onHide();
            } else {
                QLineEdit::keyPressEvent(event);
            }
        }

    signals:
        /**
         * @brief Emitted when lyric to be edited.
         */
        void onCommit();

        /**
         * @brief Emitted when text box to be moved.
         */
        void onMove(bool isBackward);

        /**
         * @brief Emitted when text box to be hidden.
         */
        void onHide();
    };

}

#endif
