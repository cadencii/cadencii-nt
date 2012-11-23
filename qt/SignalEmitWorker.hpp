/**
 * SignalEmitWorker.hpp
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
#ifndef CADENCII_QT_SIGNALEMITWORKER_HPP_
#define CADENCII_QT_SIGNALEMITWORKER_HPP_

#include <QThread>
#include <QMutex>

namespace cadencii {

    class SignalEmitWorker : public QThread {
        Q_OBJECT

    private:
        int intervalMilliSeconds;
        QMutex mutex;
        bool updateRequested;
        bool stopRequested;

    signals:
        void signal();

    public:
        explicit SignalEmitWorker() {
            updateRequested = false;
            stopRequested = false;
            intervalMilliSeconds = 100;
        }

        ~SignalEmitWorker() {
            stopRequested = true;
            wait();
        }

        void run() {
            while (!stopRequested) {
                mutex.lock();
                if (updateRequested) {
                    emit signal();
                    updateRequested = false;
                }
                mutex.unlock();
                msleep(intervalMilliSeconds);
            }
        }

        /**
         * @brief Request signal emit.
         */
        void enqueue() {
            mutex.lock();
            updateRequested = true;
            mutex.unlock();
        }
    };
}

#endif
