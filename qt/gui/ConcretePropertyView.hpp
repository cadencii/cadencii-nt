/**
 * ConcretePropertyView.hpp
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
#ifndef __cadencii_qt_gui_ConcretePropertyView_hpp__
#define __cadencii_qt_gui_ConcretePropertyView_hpp__

#include "../../gui/PropertyView.hpp"
#include <QtTreePropertyBrowser>
#include <QFocusEvent>

namespace cadencii{

    class ConcretePropertyView : public QtTreePropertyBrowser, public PropertyView{
    private:
        ControllerAdapter *controllerAdapter;

    public:
        ConcretePropertyView( QWidget *parent = 0 ) :
            QtTreePropertyBrowser( parent )
        {
            setFocusPolicy( Qt::NoFocus );
        }

        void updateTree(){
            //TODO:
        }

        void setControllerAdapter( ControllerAdapter *adapter ){
            this->controllerAdapter = adapter;
        }

        void *getWidget(){
            return this;
        }

        void updateWidget(){
            repaint();
        }
    };

}

#endif
