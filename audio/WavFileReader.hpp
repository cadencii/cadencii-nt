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

#include "Sender.hpp"
#include "../vsq/BitConverter.hpp"
#include <string>
#include <string.h>
#include <stdint.h>
#include <fstream>

namespace cadencii{
namespace audio{

    /**
     * @brief WAV ファイルから、波形を読み込むクラス
     */
    class WavFileReader : public Sender{
    private:
        /**
         * @brief 内部で使用するバッファの長さ
         */
        static const int unitBufferLength = 6400;
        /**
         * @brief 内部バッファ(左チャンネル)
         */
        double bufferLeft[unitBufferLength];
        /**
         * @brief 内部バッファ(右チャンネル)
         */
        double bufferRight[unitBufferLength];
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
         * @brief ファイルストリーム
         */
        std::basic_ifstream<char> stream;
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
        explicit WavFileReader( const std::string &filePath ) :
            Sender( 44100 )
        {
            memset( bufferLeft, 0, sizeof( double ) * unitBufferLength );
            memset( bufferRight, 0, sizeof( double ) * unitBufferLength );
            bufferFilledLength = 0;
            bufferIndex = 0;

            channels = 2;
            sampleRate = 44100;
            bytesPerSample = 2;
            isReady = false;
            totalSamples = 0;
            open( filePath );
        }

        ~WavFileReader(){
            close();
        }

        void pull( double *left, double *right, int length ){
            int remain = length;
            int finished = 0;

            // 残っているバッファーを使い切る
            while( 0 < remain ){
                int amount = bufferFilledLength <= bufferIndex + length ? bufferFilledLength - bufferIndex : length;
                if( 0 < amount ){
                    for( int i = 0; i < amount; i++ ){
                        left[i + finished] = bufferLeft[bufferIndex + i];
                        right[i + finished] = bufferRight[bufferIndex + i];
                    }
                    finished += amount;
                    bufferIndex += amount;
                    remain -= amount;
                    if( bufferIndex == bufferFilledLength && bufferFilledLength < unitBufferLength ){
                        break;
                    }
                }else{
                    // バッファーが枯渇しているので、読み込む
                    fillin();
                    if( bufferFilledLength == 0 ){
                        // 読み込もうとしたが、結果ひとつも読み込めなかった場合
                        break;
                    }
                }
            }

            // ファイルからの追加読み込み(fillin)もできなかった場合は、残りを 0 で埋める
            for( int i = finished; i < length; i++ ){
                left[i] = 0.0;
                right[i] = 0.0;
            }
        }

        /**
         * @brief ファイルを閉じる
         */
        void close(){
            if( isReady ){
                stream.close();
                isReady = false;
            }
        }

        /**
         * @brief サンプリングレートを取得する
         * @return サンプリングレート(Hz)
         */
        int getSampleRate(){
            return sampleRate;
        }

        /**
         * @brief 記録されているサンプル数を取得する
         * @return サンプル数
         */
        uint32_t getTotalSamples(){
            return totalSamples;
        }

    private:
        /**
         * @brief ファイルを開き、WAV のフォーマット情報を読み込む
         * @param filePath 開くファイルのパス
         * @return フォーマット情報の読み込みに成功した場合に true を返す
         */
        bool open( const std::string &filePath ){
            stream.open( filePath.c_str(), std::ios::binary );

            // RIFF
            char buf[4];
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 || buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F' ){
                stream.close();
                return false;
            }

            // ファイルサイズ - 8最後に記入
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 ){
                stream.close();
                return false;
            }

            // WAVE
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 || buf[0] != 'W' || buf[1] != 'A' || buf[2] != 'V' || buf[3] != 'E' ){
                stream.close();
                return false;
            }

            // fmt
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 || buf[0] != 'f' || buf[1] != 'm' || buf[2] != 't' || buf[3] != ' ' ){
                stream.close();
                return false;
            }

            // fmt チャンクのサイズ
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 ){
                stream.close();
                return false;
            }
            uint64_t chunksize = (uint64_t)VSQ_NS::BitConverter::makeUInt32LE( buf );
            uint64_t fmt_chunk_end_location = (uint64_t)stream.tellg() + chunksize;

            // format ID
            stream.read( buf, sizeof( char ) * 2 );
            if( stream.gcount() != 2 ){
                stream.close();
                return false;
            }

            // チャンネル数
            stream.read( buf, sizeof( char ) * 2 );
            if( stream.gcount() != 2 ){
                stream.close();
                return false;
            }
            channels = buf[1] << 8 | buf[0];

            // サンプリングレート
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 ){
                stream.close();
                return false;
            }
            sampleRate = (int)VSQ_NS::BitConverter::makeUInt32LE( buf );

            // データ速度
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 ){
                stream.close();
                return false;
            }

            // ブロックサイズ
            stream.read( buf, sizeof( char ) * 2 );
            if( stream.gcount() != 2 ){
                stream.close();
                return false;
            }

            // サンプルあたりのビット数
            stream.read( buf, sizeof( char ) * 2 );
            if( stream.gcount() != 2 ){
                stream.close();
                return false;
            }
            int bit_per_sample = buf[1] << 8 | buf[0];
            bytesPerSample = bit_per_sample / 8;

            // 拡張部分
            stream.seekg( fmt_chunk_end_location, std::ios::beg );

            // data
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 || buf[0] != 'd' || buf[1] != 'a' || buf[2] != 't' || buf[3] != 'a' ){
                stream.close();
                return false;
            }

            // size of data chunk
            stream.read( buf, sizeof( char ) * 4 );
            if( stream.gcount() != 4 ){
                stream.close();
                return false;
            }
            uint32_t size = (uint32_t)VSQ_NS::BitConverter::makeUInt32LE( buf );
            totalSamples = size / (channels * bytesPerSample);

            isReady = true;
            return true;
        }

        /**
         * @brief ストリームから内部バッファに読み込む
         */
        inline void fillin(){
            if( bytesPerSample == 2 ){
                fillin16();
            }else if( bytesPerSample == 1 ){
                fillin8();
            }
        }

        /**
         * @brief 16 bit per sample と見なして、ストリームから波形を読み込み、内部バッファに格納する
         */
        inline void fillin16(){
            const int bufferLength = unitBufferLength * bytesPerSample * channels;
            char temporaryBuffer[bufferLength];
            stream.read( temporaryBuffer, bufferLength );
            int actualBufferLegnth = stream.gcount() / bytesPerSample / channels;
            const double coeff = 1.0 / 32768.0;
            char *temporaryBufferIndex = temporaryBuffer;
            if( channels == 2 ){
                for( int i = 0; i < actualBufferLegnth; i++ ){
                    bufferLeft[i] = VSQ_NS::BitConverter::makeInt16LE( temporaryBufferIndex ) * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i] = VSQ_NS::BitConverter::makeInt16LE( temporaryBufferIndex ) * coeff;
                    temporaryBufferIndex += bytesPerSample;
                }
            }else{
                for( int i = 0; i < actualBufferLegnth; i++ ){
                    double value = VSQ_NS::BitConverter::makeInt16LE( temporaryBufferIndex ) * coeff;
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
        inline void fillin8(){
            const int bufferLength = unitBufferLength * bytesPerSample * channels;
            char temporaryBuffer[bufferLength];
            stream.read( temporaryBuffer, bufferLength );
            int actualBufferLegnth = stream.gcount() / bytesPerSample / channels;
            const double coeff = 1.0 / 128.0;
            int temporaryBufferIndex = 0;
            if( channels == 2 ){
                for( int i = 0; i < actualBufferLegnth; i++ ){
                    bufferLeft[i] = ((int)(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127) * coeff;
                    temporaryBufferIndex += bytesPerSample;
                    bufferRight[i] = ((int)(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127) * coeff;
                    temporaryBufferIndex += bytesPerSample;
                }
            }else{
                for( int i = 0; i < actualBufferLegnth; i++ ){
                    double value = ((int)(0xFF & temporaryBuffer[temporaryBufferIndex]) - 127) * coeff;
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
