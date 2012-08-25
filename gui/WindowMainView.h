/**
 * WindowMainView.h
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __WindowMainView_h__
#define __WindowMainView_h__

#include <QMainWindow>

namespace Ui {
    class WindowMainView;
}

namespace cadencii{

    class WindowMainView : public QMainWindow{
        Q_OBJECT

    public:
        explicit WindowMainView( QWidget *parent = 0 );
        ~WindowMainView();

    private:
        Ui::WindowMainView *ui;
    };

}
#endif // __WindowMainView_h__
