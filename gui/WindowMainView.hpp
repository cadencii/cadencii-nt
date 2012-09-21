/**
 * WindowMainView.hpp
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
#ifndef __WindowMainView_hpp__
#define __WindowMainView_hpp__

#include <QMainWindow>
#include "MainView.hpp"

namespace Ui{
    class WindowMainView;
}

namespace cadencii{
    class WindowMainView : public QMainWindow, public MainView{
        Q_OBJECT
    private:
        Ui::WindowMainView *ui;
        ControllerAdapter *controllerAdapter;

    public:
        explicit WindowMainView( QWidget *parent = 0 );
        ~WindowMainView();

        void setTrackView( TrackView *trackView );

        void setControlChangeView( ControlChangeView *controlChangeView );

        void setBarCountView( BarCountView *barCountView );

        void setTempoView( TempoView *tempoView );

        void setTimesigView( TimesigView *timesigView );

        void setControllerAdapter( ControllerAdapter *controllerAdapter );

    private:
        /**
         * @brief ピアノロールウィジェットを設定する
         */
        void setupPianorollWidget();

    private slots:
        void on_action_open_vsq_vocaloid_midi_triggered();
    };

}
#endif // __WindowMainView_h__
