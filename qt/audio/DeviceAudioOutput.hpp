/**
 * DeviceAudioOutput.hpp
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
#ifndef __cadencii_qt_audio_DeviceAudioOutput_hpp__
#define __cadencii_qt_audio_DeviceAudioOutput_hpp__

#include "../../audio/AudioOutput.hpp"
#include <QAudioOutput>

namespace cadencii{
namespace audio{

    /**
     * @brief システムのデフォルトのサウンドデバイスに波形を出力する
     */
    class DeviceAudioOutput : public AudioOutput{
    protected:
        QAudioOutput *out;
        QIODevice *device;
        QAudioFormat format;
        const static int unitBufferLength = 12800;
        char buffer[unitBufferLength];

    public:
        explicit DeviceAudioOutput( int sampleRate ) :
            AudioOutput( sampleRate )
        {
            format.setChannels( 2 );
            format.setFrequency( sampleRate );
            format.setSampleSize( 16 );
            format.setCodec( "audio/pcm" );
            format.setSampleType( QAudioFormat::SignedInt );
            out = new QAudioOutput( format );
            device = 0;
            memset( buffer, 0, sizeof( char ) * unitBufferLength );
        }

        ~DeviceAudioOutput(){
            delete out;
        }

        void push( double *left, double *right, int length, int offset ){
            if( !device ){
                device = out->start();
            }
            int remain = length;
            int finished = 0;
            while( 0 < remain ){
                int amount = unitBufferLength / 4 <= remain ? unitBufferLength / 4 : remain;
                int index = 0;
                for( int i = 0; i < amount; i++ ){
                    int16_t leftData = (int16_t)(left[i + finished + offset] * 32767);
                    int16_t rightData = (int16_t)(right[i + finished + offset] * 32767);
                    buffer[index++] = ((uint8_t *)&leftData)[0];
                    buffer[index++] = ((uint8_t *)&leftData)[1];
                    buffer[index++] = ((uint8_t *)&rightData)[0];
                    buffer[index++] = ((uint8_t *)&rightData)[1];
                }
                int bytesToWrite = amount * 4;
                int bytesWritten = 0;
                while( bytesWritten < bytesToWrite ){
                    int actualWrittenBytes = device->write( (const char *)(buffer + bytesWritten), (qint64)(bytesToWrite - bytesWritten) );
                    bytesWritten += actualWrittenBytes;
                }
                remain -= amount;
                finished += amount;
            }
        }

        void flush(){ /* do nothing */ }
    };

}
}

#endif
