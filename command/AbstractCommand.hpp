/**
 * AbstractCommand.hpp
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
#ifndef __cadencii_command_AbstractCommand_hpp__
#define __cadencii_command_AbstractCommand_hpp__

#include "../vsq/Sequence.hpp"

namespace cadencii {

    /**
     * @brief シーケンスの編集操作を表現するクラス
     */
    class AbstractCommand {
    public:
        /**
         * @brief 編集操作を実行する
         * @return 操作を元に戻すコマンドを返す。戻り値のインスタンスは、SequenceModel が delete するので気にしなくてよい
         */
        virtual AbstractCommand *execute(VSQ_NS::Sequence *sequence) = 0;

        virtual ~AbstractCommand() = 0;
    };
}

#endif
