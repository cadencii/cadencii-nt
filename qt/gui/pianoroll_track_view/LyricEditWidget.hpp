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

#include <QLineEdit>
#include <QKeyEvent>
#include <string>
#include <libvsq/libvsq.h>

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
         * @brief Symbol edit mode.
         */
        bool symbolEditMode;

    private:
        /**
         * @brief Note event corresponding lyric.
         */
        const vsq::Event *_event;

        std::string word;

        std::string phoneticSymbol;

    public:
        explicit LyricEditWidget(QWidget *parent = 0) :
            QLineEdit(parent), _event(0), symbolEditMode(false) {
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
            } else if (Qt::Key_Alt == key) {
                flipSymbolEditMode();
            } else if (Qt::Key_Escape == key) {
                emit onHide();
            } else {
                QLineEdit::keyPressEvent(event);
            }
        }

        /**
         * @brief Setup text from specified note event.
         */
        void setupText(const vsq::Event *event) {
            _event = event;
            if (!_event) {
                return;
            }

            word = "a";
            phoneticSymbol = "a";
            if (event->lyricHandle.size() > 0) {
                vsq::Lyric const& lyric = event->lyricHandle.get(0);
                word = lyric.phrase;
                phoneticSymbol = lyric.phoneticSymbol();
            }
            setText(QString::fromStdString(symbolEditMode ? phoneticSymbol : word));
            selectAll();
        }

        /**
         * @brief Get a instance of note event, associated to this widget.
         */
        const vsq::Event *event() {
            return _event;
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

    private:
        void flipSymbolEditMode() {
            static QPalette normalPalette;
            static QPalette symbolModePalette;
            static bool isInitialized = false;
            if (!isInitialized) {
                symbolModePalette.setColor(QPalette::Base, QColor::fromRgb(192, 192, 192));
                isInitialized = true;
            }

            symbolEditMode = !symbolEditMode;

            (!symbolEditMode) ? phoneticSymbol : word = text().toStdString();
            setText(QString::fromStdString(symbolEditMode ? phoneticSymbol : word));
            setPalette(symbolEditMode ? symbolModePalette : normalPalette);
            selectAll();
        }
    };
}

#endif
