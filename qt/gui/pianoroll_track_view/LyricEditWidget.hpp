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

namespace cadencii {

    /**
     * @brief A widget for lyric editing.
     */
    class LyricEditWidget : public QLineEdit {
    public:
        /**
         * @brief Position of 'lyricEdit' component, based on scene geometry.
         */
        QPoint scenePosition;

    public:
        explicit LyricEditWidget(QWidget *parent = 0) :
            QLineEdit(parent)
        {
        }
    };

}

#endif
