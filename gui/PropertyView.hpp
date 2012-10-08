/**
 * PropertyView.hpp
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
#ifndef __cadencii_gui_PropertyView_hpp__
#define __cadencii_gui_PropertyView_hpp__

#include "../ControllerAdapter.hpp"
#include "../ItemSelectionStatusListener.hpp"
#include "../vsq/Sequence.hpp"

namespace cadencii{

    /**
     * @brief 編集可能なアイテムのプロパティーを編集するビュー
     */
    class PropertyView : public ItemSelectionStatusListener{
    public:
        virtual void setControllerAdapter( ControllerAdapter *adapter ) = 0;

        virtual void updateWidget() = 0;

        virtual void *getWidget() = 0;

        virtual void setSequence( VSQ_NS::Sequence *sequence ) = 0;
    };

}

#endif
