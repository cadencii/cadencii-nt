/**
 * DeleteEventCommand.hpp
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
#ifndef __cadencii_DeleteEventCommand_hpp__
#define __cadencii_DeleteEventCommand_hpp__

#include "AbstractCommand.hpp"
#include "AddEventCommand.hpp"
#include <vector>

namespace cadencii{

    /**
     * @brief イベントの削除
     */
    class DeleteEventCommand : public AbstractCommand{
    private:
        /**
         * @brief 削除するイベントのあるトラック番号
         */
        int track;

        /**
         * @brief 削除するイベントの ID のリスト
         */
        std::vector<int> idList;

    public:
        /**
         * @brief コマンドを初期化する
         * @param track 削除するイベントのあるトラック番号
         * @param idList 削除するイベントの ID のリスト
         */
        explicit DeleteEventCommand( int track, const std::vector<int> &idList );

        AbstractCommand *execute( VSQ_NS::Sequence *sequence );
    };

}


#endif
