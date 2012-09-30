/**
 * FilterDriver.hpp
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
#ifndef __cadencii_audio_AudioFilter_hpp__
#define __cadencii_audio_AudioFilter_hpp__

#include "AudioFilter.hpp"
#include "Sender.hpp"
#include <vector>

namespace cadencii{
namespace audio{

    /**
     * @brief AudioFilter を Sender として動作させるためのクラス
     */
    class FilterDriver : public Sender{
    private:
        class MemoryReceiver : public Receiver{
        private:
            std::vector<double> bufferLeft;
            std::vector<double> bufferRight;

        public:
            explicit MemoryReceiver( int sampleRate ) :
                Receiver( sampleRate )
            {
            }

            void push( double *left, double *right, int length, int offset ){
                for( int i = 0; i < length; i++ ){
                    bufferLeft.push_back( left[i + offset] );
                    bufferRight.push_back( right[i + offset] );
                }
            }

            void flush(){
            }

            int getBuffer( double *left, double *right, int length ){
                int result = std::min( length, (int)std::min( bufferLeft.size(), bufferRight.size() ) );
                for( int i = 0; i < result; i++ ){
                    left[i] = bufferLeft[i];
                    right[i] = bufferRight[i];
                }
                if( 0 < result ){
                    std::vector<double>::iterator leftIterator = bufferLeft.begin();
                    std::advance( leftIterator, result );
                    bufferLeft.erase( bufferLeft.begin(), leftIterator );

                    std::vector<double>::iterator rightIterator = bufferRight.begin();
                    std::advance( rightIterator, result );
                    bufferRight.erase( bufferRight.begin(), rightIterator );
                }
                return result;
            }
        };

        AudioFilter *filter;
        MemoryReceiver receiver;
        Sender *sender;
        double *bufferLeft;
        double *bufferRight;
        int unitBufferLength;
        double *temporaryBufferLeft;
        double *temporaryBufferRight;

    public:
        explicit FilterDriver( int sampleRate, AudioFilter *filter ) :
            Sender( sampleRate ), receiver( sampleRate )
        {
            this->filter = filter;
            this->filter->setReceiver( &receiver );
            unitBufferLength = 4096;
            bufferLeft = new double[unitBufferLength];
            bufferRight = new double[unitBufferLength];
            temporaryBufferLeft = new double[unitBufferLength];
            temporaryBufferRight = new double[unitBufferLength];
            sender = 0;
        }

        ~FilterDriver(){
            delete [] bufferLeft;
            delete [] bufferRight;
            delete [] temporaryBufferLeft;
            delete [] temporaryBufferRight;
        }

        void setSender( Sender *sender ){
            this->sender = sender;
        }

        void pull( double *left, double *right, int length ){
            int remain = length;
            int finished = 0;
            while( 0 < remain ){
                int amount = unitBufferLength <= remain ? unitBufferLength : remain;
                sender->pull( bufferLeft, bufferRight, amount );
                filter->push( bufferLeft, bufferRight, amount, 0 );

                int actualReceivedBufferLength;
                while( 0 < (actualReceivedBufferLength = receiver.getBuffer( temporaryBufferLeft, temporaryBufferRight, amount )) ){
                    for( int i = 0; i < actualReceivedBufferLength; i++ ){
                        left[i + finished] = temporaryBufferLeft[i];
                        right[i + finished] = temporaryBufferRight[i];
                    }
                    remain -= actualReceivedBufferLength;
                    finished += actualReceivedBufferLength;
                    amount -= actualReceivedBufferLength;
                }
            }
        }

    };

}
}

#endif
