/**
 * main.cpp
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
#include <QApplication>
#include <QTextCodec>
#include "qt/ConcreteSettings.hpp"
#include "qt/AppContainer.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    cadencii::ConcreteSettings settings;
    cadencii::Settings::instance(&settings);

    cadencii::AppContainer container;
    container.controller.showMainView();

    return a.exec();
}
