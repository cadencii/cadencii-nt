/**
 * ControllerAdapter.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __ControllerAdapter_hpp__
#define __ControllerAdapter_hpp__

namespace cadencii{

    class ControllerAdapter{
    public:
        /**
         * @brief VSQ ファイルを開く
         * @param[in] 開くVSQのパス
         */
        virtual void openVSQFile( const string &filePath )throw() = 0;
    };

}

#endif
