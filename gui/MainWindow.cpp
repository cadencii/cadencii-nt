/**
 * MainWindow.cpp
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
#include "gui/MainWindow.h"
#include "ui_MainWindow.h"

namespace cadencii{

    MainWindow::MainWindow( QWidget *parent )
        : QMainWindow( parent ),
        ui( new Ui::MainWindow )
    {
        ui->setupUi( this );
    }

    MainWindow::~MainWindow(){
        delete ui;
    }

}
