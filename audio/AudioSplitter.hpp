/**
 * AudioSplitter.hpp
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
#ifndef __AudioSplitter_hpp__
#define __AudioSplitter_hpp__

#include "Receiver.hpp"
#include <vector>

namespace cadencii{
namespace audio{

    /**
     * @brief AudioSplitter は、オーディオ波形を複数の受け取り先に分割するクラスです
     */
    class AudioSplitter : public Receiver{
    private:
        std::vector<Receiver *> receiverList;
        static const int unitBufferLength = 4096;
        double bufferLeft[unitBufferLength];
        double bufferRight[unitBufferLength];

    public:
        explicit AudioSplitter( int sampleRate ) :
            Receiver( sampleRate )
        {
        }

        /**
         * @brief オーディオ波形の受け取り先を追加する
         * @param[in] オーディオ波形の受け取り先
         */
        void addReceiver( Receiver *receiver ){
            receiverList.push_back( receiver );
        }

        void push( double *left, double *right, int length, int offset ){
            int remain = length;
            int finished = 0;
            while( 0 < remain ){
                int amount = unitBufferLength <= remain ? unitBufferLength : remain;
                memcpy( bufferLeft, left + sizeof( double ) * (offset + finished), sizeof( double ) * amount );
                memcpy( bufferRight, left + sizeof( double ) * (offset + finished), sizeof( double ) * amount );
                std::vector<Receiver *>::iterator i;
                for( i = receiverList.begin(); i != receiverList.end(); ++i ){
                    (*i)->push( bufferLeft, bufferRight, amount, 0 );
                }
                finished += amount;
                remain -= amount;
            }
        }

        void flush(){
            std::vector<Receiver *>::iterator i;
            for( i = receiverList.begin(); i != receiverList.end(); ++i ){
                (*i)->flush();
            }
        }
    };

}
}

#endif
