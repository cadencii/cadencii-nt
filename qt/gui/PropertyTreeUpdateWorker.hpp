/**
 * PropertyTreeUpdateWorker.hpp
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
#ifndef __cadencii_PropertyTreeUpdateWorker_hpp__
#define __cadencii_PropertyTreeUpdateWorker_hpp__

#include <QThread>
#include <QMutex>
#include "ConcretePropertyView.hpp"

namespace cadencii {

    /**
     * @brief プロパティツリーの更新を行うワーカースレッド
     */
    class PropertyTreeUpdateWorker : public QThread {
        Q_OBJECT

    private:
        static const int SLEEP_INTERVAL_MILLI_SECONDS = 100;
        ConcretePropertyView *parent;
        ControllerAdapter *controllerAdapter;
        QMutex mutex;
        bool updateRequested;
        bool stopRequested;

    signals:
        void callUpdateTree();

    public:
        explicit PropertyTreeUpdateWorker(ConcretePropertyView *parent);

        ~PropertyTreeUpdateWorker();

        void run();

        /**
         * @brief ツリーの更新を要求する
         */
        void enqueueTreeUpdate();

        void setControllerAdapter(ControllerAdapter * adapter);
    };
}

#endif
