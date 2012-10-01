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

    class WavFileReader : public Sender{
    private:
        static const int unitBufferLength = 6400;
        double bufferLeft[unitBufferLength];
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

        std::basic_ifstream<char> m_stream;//TODO: rename
        int m_channel;//TODO: rename
        int m_sample_per_sec;//TODO: rename
        int m_byte_per_sample;//TODO: rename
        uint64_t m_total_samples;//TODO: rename
        uint64_t m_header_offset;//TODO: rename
        bool m_opened;

    public:
        explicit WavFileReader( const std::string &filePath ) :
            Sender( 44100 )
        {
            memset( bufferLeft, 0, sizeof( double ) * unitBufferLength );
            memset( bufferRight, 0, sizeof( double ) * unitBufferLength );
            bufferFilledLength = 0;
            bufferIndex = 0;

            m_channel = 2;
            m_sample_per_sec = 44100;
            m_byte_per_sample = 2;
            m_total_samples = 0;
            m_header_offset = 0;
            m_opened = false;
            open( filePath );
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

        void close(){
            if( m_opened ){
                m_stream.close();
                m_opened = false;
            }
        }

    private:
        bool open( const std::string &filePath ){
            m_stream.open( filePath.c_str(), std::ios::binary );

            // RIFF
            char buf[4];
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 || buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F' ){
                m_stream.close();
                return false;
            }

            // ファイルサイズ - 8最後に記入
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 ){
                m_stream.close();
                return false;
            }

            // WAVE
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 || buf[0] != 'W' || buf[1] != 'A' || buf[2] != 'V' || buf[3] != 'E' ){
                m_stream.close();
                return false;
            }

            // fmt
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 || buf[0] != 'f' || buf[1] != 'm' || buf[2] != 't' || buf[3] != ' ' ){
                m_stream.close();
                return false;
            }

            // fmt チャンクのサイズ
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 ){
                m_stream.close();
                return false;
            }
            uint64_t chunksize = (uint64_t)VSQ_NS::BitConverter::makeUInt32LE( buf );
            uint64_t fmt_chunk_end_location = (uint64_t)m_stream.tellg() + chunksize;

            // format ID
            m_stream.read( buf, sizeof( char ) * 2 );
            if( m_stream.gcount() != 2 ){
                m_stream.close();
                return false;
            }

            // チャンネル数
            m_stream.read( buf, sizeof( char ) * 2 );
            if( m_stream.gcount() != 2 ){
                m_stream.close();
                return false;
            }
            m_channel = buf[1] << 8 | buf[0];

            // サンプリングレート
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 ){
                m_stream.close();
                return false;
            }
            m_sample_per_sec = (int)VSQ_NS::BitConverter::makeUInt32LE( buf );

            // データ速度
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 ){
                m_stream.close();
                return false;
            }

            // ブロックサイズ
            m_stream.read( buf, sizeof( char ) * 2 );
            if( m_stream.gcount() != 2 ){
                m_stream.close();
                return false;
            }

            // サンプルあたりのビット数
            m_stream.read( buf, sizeof( char ) * 2 );
            if( m_stream.gcount() != 2 ){
                m_stream.close();
                return false;
            }
            int bit_per_sample = buf[1] << 8 | buf[0];
            m_byte_per_sample = bit_per_sample / 8;

            // 拡張部分
            m_stream.seekg( fmt_chunk_end_location, std::ios::beg );

            // data
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 || buf[0] != 'd' || buf[1] != 'a' || buf[2] != 't' || buf[3] != 'a' ){
                m_stream.close();
                return false;
            }

            // size of data chunk
            m_stream.read( buf, sizeof( char ) * 4 );
            if( m_stream.gcount() != 4 ){
                m_stream.close();
                return false;
            }
            int size = (int)VSQ_NS::BitConverter::makeUInt32LE( buf );
            m_total_samples = size / (m_channel * m_byte_per_sample);

            m_opened = true;
            m_header_offset = (uint64_t)m_stream.tellg();
            return true;
        }

        /**
         * @brief ストリームから bufferLeft, bufferRight に読み込む
         */
        void fillin(){
            if( m_byte_per_sample == 2 ){
                if( m_channel == 2 ){
                    fillinStereo16();
                }
            }
        }

        /**
         * @brief ステレオ、16 bit per sample と見なして、ストリームから読み込む
         */
        void fillinStereo16(){
            const int bufferLength = unitBufferLength * (16 / 8) * 2;
            char buffer[bufferLength];
            m_stream.read( buffer, bufferLength );
            int actualBufferLegnth = m_stream.gcount() / (16 / 8) / 2;
            const double coeff = 1.0 / 32768.0;
            for( int i = 0; i < actualBufferLegnth; i++ ){
                bufferLeft[i] = VSQ_NS::BitConverter::makeInt16LE( buffer + sizeof( char ) * (i * 4) ) * coeff;
                bufferRight[i] = VSQ_NS::BitConverter::makeInt16LE( buffer + sizeof( char ) * (i * 4 + 2) ) * coeff;
            }
            bufferFilledLength = actualBufferLegnth;
            bufferIndex = 0;
        }
    };

}
}

#endif
