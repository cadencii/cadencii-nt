/**
 * SampleRateLinearConverter.hpp
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
#ifndef __cadencii_audio_SampleRateConverter_hpp__
#define __cadencii_audio_SampleRateConverter_hpp__

#include <stdint.h>
#include <cmath>
#include "AudioFilter.hpp"

namespace cadencii {
namespace audio {

    /**
     * @brief サンプリングレートを線形補間により変換するフィルター
     */
    class SampleRateLinearConverter : public AudioFilter {
    protected:
        std::shared_ptr<AudioReceiver> receiver;
        /**
         * @brief 変換前のサンプリングレート
         */
        int bRate;
        /**
         * @brief 変換後のサンプリングレート
         */
        int aRate;
        /**
         * @brief 変換後の、push 済みのサンプル数
         */
        uint64_t aFinished;
        /**
         * @brief 受け取ったサンプル数
         */
        uint64_t bFinished;
        /**
         * @brief 内部バッファーの長さ
         */
        int unitBufferLength;
        /**
         * @brief 内部バッファー
         */
        double *bufferLeft;
        /**
         * @brief 内部バッファー
         */
        double *bufferRight;
        /**
         * @brief push により与えられた波形バッファのうち、末尾の値(左チャンネル)
         */
        double lastLeft;
        /**
         * @brief push により与えられた波形バッファのうち、末尾の値(右チャンネル)
         */
        double lastRight;

    public:
        /**
         * @brief 初期化
         * @param[in] convertFrom 変換前のサンプリングレート(sec^-1)
         * @param[in] convertTo 変換後のサンプリングレート(sec^-1)
         */
        explicit SampleRateLinearConverter(int convertFrom, int convertTo)
            : AudioFilter(convertTo)
            , receiver(0)
        {
            bRate = convertFrom;
            aRate = convertTo;
            bFinished = 0;
            aFinished = 0;
            unitBufferLength = 4096;
            if (aRate == bRate) {
                bufferLeft = 0;
                bufferRight = 0;
            } else {
                bufferLeft = new double[unitBufferLength];
                bufferRight = new double[unitBufferLength];
            }
            lastLeft = 0;
            lastRight = 0;
        }

        ~SampleRateLinearConverter() {
            if (bufferLeft) {
                delete [] bufferLeft;
                bufferLeft = 0;
            }
            if (bufferRight) {
                delete [] bufferRight;
                bufferRight = 0;
            }
        }

        void setReceiver(std::shared_ptr<AudioReceiver> const& receiver) override {
            this->receiver = receiver;
        }

        void flush() {/* do nothing */}

        void push(double *left, double *right, int length, int offset) {
            if (!receiver) return;
            if (aRate == bRate) {
                receiver->push(left, right, length, offset);
                return;
            }

            // index for internal buffer
            int bufferIndex = 0;

            // 何サンプル目まで変換するか
            uint64_t aEndSample = static_cast<uint64_t>(std::floor(
                (bFinished + length - 1) / static_cast<double>(bRate) * aRate));
            int finished = 0;
            for (uint64_t aSample = aFinished;
                 aSample <= aEndSample; ++aSample) {
                uint64_t bSample
                    = static_cast<uint64_t>(std::floor(
                        aSample / static_cast<double>(aRate) * bRate));
                double deltaSecond
                    = aSample / static_cast<double>(aRate)
                        - bSample / static_cast<double>(bRate);
                bufferLeft[bufferIndex]
                    = getInterpolatedValue(left, offset, lastLeft,
                                           bSample, deltaSecond);
                bufferRight[bufferIndex]
                    = getInterpolatedValue(right, offset, lastRight,
                                           bSample, deltaSecond);
                finished++;
                bufferIndex++;

                // flush buffers
                if (unitBufferLength == bufferIndex) {
                    bufferIndex = 0;
                    receiver->push(bufferLeft, bufferRight,
                                   unitBufferLength, 0);
                }
            }

            // flush buffers
            if (0 < bufferIndex) {
                receiver->push(bufferLeft, bufferRight, bufferIndex, 0);
            }

            lastLeft = left[offset + length - 1];
            lastRight = right[offset + length - 1];
            bFinished += length;
            aFinished += finished;
        }

    private:
        /**
         * @brief 補間によって、引数 outgoingBufferIndex サンプル目の値を取得する
         * @param incomingBuffer push メソッドによって渡されたバッファ
         * @param incomingBufferOffset push メソッドに渡されたバッファのオフセット
         * @param incomingBufferLastValue 直前の push メソッドで渡された、バッファーの末尾位置の値
         * @param outgoingBufferIndex 取得するサンプル位置
         * @param deltaSecond 補間の起点となる時刻から、outgoingBufferIndex サンプル位置での時刻の時間差
         * @return 補間後の値
         */
        inline double getInterpolatedValue(
                double *incomingBuffer,
                int incomingBufferOffset, double incomingBufferLastValue,
                uint64_t outgoingBufferIndex, double deltaSecond) {
            // interpolate using `i` and `i + 1` th value of incoming buffer
            int i = outgoingBufferIndex + incomingBufferOffset - bFinished;

            // value of `i + 1` th incoming buffer
            double y_ip1 = incomingBuffer[i + 1];

            // value of `i` th incoming buffer.
            // select from last-value if `i` is out of the range
            // of incoming buffer
            double y_i;
            if (outgoingBufferIndex < bFinished) {
                y_i = incomingBufferLastValue;
            } else {
                y_i = incomingBuffer[i];
            }

            // slope
            double a = (y_ip1 - y_i) * bRate;
            return y_i + a * deltaSecond;
        }
    };
}
}

#endif
