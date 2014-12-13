/**
 * WavFileReader.hpp
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
#ifndef __cadencii_audio_WavFileReader_hpp__
#define __cadencii_audio_WavFileReader_hpp__

#include <stdint.h>
#include <cstring>
#include <cstdio>
#include <string>
#include "AudioSender.hpp"
#include <libvsq/libvsq.h>

namespace cadencii {
namespace audio {

    /**
     * @brief WAV ファイルから、波形を読み込むクラス
     */
    class WavFileReader : public AudioSender {
    private:
        /**
         * @brief 内部で使用するバッファの長さ
         */
        static const int kUnitBufferLength = 6400;
        /**
         * @brief 内部バッファ(左チャンネル)
         */
        double bufferLeft[kUnitBufferLength];
        /**
         * @brief 内部バッファ(右チャンネル)
         */
        double bufferRight[kUnitBufferLength];
        /**
         * @brief bufferLeft, bufferRight に入っているバッファの長さ
         * unitBufferLength より小さい場合、EOF に達したためそれ以上の読み込みができないことを意味する
         */
        int bufferFilledLength;
        /**
         * @brief bufferLeft, bufferRight を次回呼ぶ際は、bufferIndex 番目からを使う
         */
        int bufferIndex;
        /**
         * @brief A file stream.
         */
        FILE *stream;
        /**
         * @brief チャンネル数
         */
        int channels;
        /**
         * @brief サンプリングレート(s^-1)
         */
        int sampleRate;
        /**
         * @brief 1 チャンネル、1 サンプルあたりのバイト数
         */
        int bytesPerSample;
        /**
         * @brief ファイルストリームが正しく open できているかどうか
         */
        bool isReady;
        /**
         * @brief 記録されているサンプル数
         */
        uint32_t totalSamples;

    public:
        /**
         * @brief WAV ファイルのファイルパスを指定し、初期化する
         * @param filePath ファイルパス
         * @todo Sender( 44100 ) となっている箇所、これでよいか検討する
         */
        explicit WavFileReader(const std::string &filePath) :
            AudioSender(44100) {
            memset(bufferLeft, 0, sizeof(double) * kUnitBufferLength);
            memset(bufferRight, 0, sizeof(double) * kUnitBufferLength);
            bufferFilledLength = 0;
            bufferIndex = 0;

            channels = 2;
            sampleRate = 44100;
            bytesPerSample = 2;
            isReady = false;
            totalSamples = 0;
            open(filePath);
        }

        ~WavFileReader() {
            close();
        }

        void pull(double *left, double *right, int length) {
            int remain = length;
            int finished = 0;

            // 残っているバッファーを使い切る
            while (0 < remain) {
                int amount = bufferFilledLength - bufferIndex <= remain
                        ? bufferFilledLength - bufferIndex
                        : remain;
                if (0 < amount) {
                    for (int i = 0; i < amount; i++) {
                        left[i + finished] = bufferLeft[bufferIndex + i];
                        right[i + finished] = bufferRight[bufferIndex + i];
                    }
                    finished += amount;
                    bufferIndex += amount;
                    remain -= amount;
                    if (bufferIndex == bufferFilledLength &&
                            bufferFilledLength < kUnitBufferLength) {
                        break;
                    }
                } else {
                    // バッファーが枯渇しているので、読み込む
                    fillin();
                    if (bufferFilledLength == 0) {
                        // 読み込もうとしたが、結果ひとつも読み込めなかった場合
                        break;
                    }
                }
            }

            // ファイルからの追加読み込み(fillin)もできなかった場合は、
            // 残りを 0 で埋める
            for (int i = finished; i < length; i++) {
                left[i] = 0.0;
                right[i] = 0.0;
            }
        }

        /**
         * @brief ファイルを閉じる
         */
        void close() {
            if (isReady) {
                fclose(stream);
                isReady = false;
            }
        }

        /**
         * @brief サンプリングレートを取得する
         * @return サンプリングレート(Hz)
         */
        int getSampleRate() {
            return sampleRate;
        }

        /**
         * @brief 記録されているサンプル数を取得する
         * @return サンプル数
         */
        uint32_t getTotalSamples() {
            return totalSamples;
        }

    private:
        /**
         * @brief ファイルを開き、WAV のフォーマット情報を読み込む
         * @param filePath 開くファイルのパス
         * @return フォーマット情報の読み込みに成功した場合に true を返す
         */
        bool open(const std::string &filePath) {
            stream = fopen(filePath.c_str(), "rb");
            if (!stream) return false;

            // RIFF
            char buf[4];
            size_t readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4 || buf[0] != 'R' ||
                    buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F') {
                fclose(stream);
                return false;
            }

            // ファイルサイズ - 8最後に記入
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4) {
                fclose(stream);
                return false;
            }

            // WAVE
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4 || buf[0] != 'W' ||
                    buf[1] != 'A' || buf[2] != 'V' || buf[3] != 'E') {
                fclose(stream);
                return false;
            }

            // fmt
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4 || buf[0] != 'f' ||
                    buf[1] != 'm' || buf[2] != 't' || buf[3] != ' ') {
                fclose(stream);
                return false;
            }

            // fmt チャンクのサイズ
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4) {
                fclose(stream);
                return false;
            }
            uint64_t chunksize
                    = (uint64_t)vsq::BitConverter::makeUInt32LE(buf);
            uint64_t fmt_chunk_end_location
                    = (uint64_t)ftell(stream) + chunksize;

            // format ID
            readCount = fread(buf, sizeof(char), 2, stream);
            if (readCount != 2) {
                fclose(stream);
                return false;
            }

            // チャンネル数
            readCount = fread(buf, sizeof(char), 2, stream);
            if (readCount != 2) {
                fclose(stream);
                return false;
            }
            channels = buf[1] << 8 | buf[0];

            // サンプリングレート
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4) {
                fclose(stream);
                return false;
            }
            sampleRate = static_cast<int>(
                        vsq::BitConverter::makeUInt32LE(buf));

            // データ速度
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4) {
                fclose(stream);
                return false;
            }

            // ブロックサイズ
            readCount = fread(buf, sizeof(char), 2, stream);
            if (readCount != 2) {
                fclose(stream);
                return false;
            }

            // サンプルあたりのビット数
            readCount = fread(buf, sizeof(char), 2, stream);
            if (readCount != 2) {
                fclose(stream);
                return false;
            }
            int bit_per_sample = buf[1] << 8 | buf[0];
            bytesPerSample = bit_per_sample / 8;

            // 拡張部分
            fseek(stream, fmt_chunk_end_location, SEEK_SET);

            // data
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4 || buf[0] != 'd' ||
                    buf[1] != 'a' || buf[2] != 't' || buf[3] != 'a') {
                fclose(stream);
                return false;
            }

            // size of data chunk
            readCount = fread(buf, sizeof(char), 4, stream);
            if (readCount != 4) {
                fclose(stream);
                return false;
            }
            uint32_t size = static_cast<uint32_t>(
                        vsq::BitConverter::makeUInt32LE(buf));
            totalSamples = size / (channels * bytesPerSample);

            isReady = true;
            return true;
        }

        /**
         * @brief ストリームから内部バッファに読み込む
         */
        inline void fillin() {
            if (bytesPerSample == 2) {
                fillin16();
            } else if (bytesPerSample == 1) {
                fillin8();
            }
        }

        /**
         * @brief 16 bit per sample と見なして、ストリームから波形を読み込み、内部バッファに格納する
         */
        inline void fillin16() {
            const int kBufferLength
                    = kUnitBufferLength * bytesPerSample * channels;
            char temporaryBuffer[kBufferLength];
            size_t readCount = fread(temporaryBuffer, sizeof(char), kBufferLength, stream);
            int actualBufferLegnth
                    = readCount / bytesPerSample / channels;
            const double coeff = 1.0 / 32768.0;
            char *temporaryBufferIndex = temporaryBuffer;
            if (channels == 2) {
                for (int i = 0; i < actualBufferLegnth; i++) {
                    bufferLeft[i]
                        = vsq::BitConverter::makeInt16LE(temporaryBufferIndex)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i]
                        = vsq::BitConverter::makeInt16LE(temporaryBufferIndex)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                }
            } else {
                for (int i = 0; i < actualBufferLegnth; i++) {
                    double value
                        = vsq::BitConverter::makeInt16LE(temporaryBufferIndex)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i] = value;
                    bufferLeft[i] = value;
                }
            }
            bufferFilledLength = actualBufferLegnth;
            bufferIndex = 0;
        }

        /**
         * @brief 8 bit per sample と見なして、ストリームから波形を読み込み、内部バッファに格納する
         */
        inline void fillin8() {
            const int kBufferLength
                = kUnitBufferLength * bytesPerSample * channels;
            char temporaryBuffer[kBufferLength];
            size_t readCount = fread(temporaryBuffer, sizeof(char), kBufferLength, stream);
            int actualBufferLegnth
                = readCount / bytesPerSample / channels;
            const double coeff = 1.0 / 128.0;
            int temporaryBufferIndex = 0;
            if (channels == 2) {
                for (int i = 0; i < actualBufferLegnth; i++) {
                    bufferLeft[i]
                        = (static_cast<int>(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i]
                        = (static_cast<int>(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                }
            } else {
                for (int i = 0; i < actualBufferLegnth; i++) {
                    double value
                        = (static_cast<int>(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127)
                            * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i] = value;
                    bufferLeft[i] = value;
                }
            }
            bufferFilledLength = actualBufferLegnth;
            bufferIndex = 0;
        }
    };
}
}

#endif
