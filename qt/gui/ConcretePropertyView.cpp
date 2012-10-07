/**
 * ConcretePropertyView.cpp
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
#include "ConcretePropertyView.hpp"
#include <QtIntPropertyManager>
#include <QtSpinBoxFactory>

namespace cadencii{

    ConcretePropertyView::ConcretePropertyView( QWidget *parent ) :
        QtTreePropertyBrowser( parent ), controllerAdapter( 0 )
    {
        setFocusPolicy( Qt::NoFocus );
        setResizeMode( QtTreePropertyBrowser::Interactive );
        setIndentation( 10 );
        initProperties();
    }

    void ConcretePropertyView::statusChanged(){
        ItemSelectionManager *manager = controllerAdapter->getItemSelectionManager();
        const std::vector<const VSQ_NS::Event *> *list = manager->getEventItemList();

        if( list->size() == 0 ){
            clear();
            return;
        }

        updateTreeByEvent();
    }

    void ConcretePropertyView::setControllerAdapter( ControllerAdapter *adapter ){
        this->controllerAdapter = adapter;
    }

    void *ConcretePropertyView::getWidget(){
        return this;
    }

    void ConcretePropertyView::updateWidget(){
        repaint();
    }

    void ConcretePropertyView::updateTreeByEvent(){
        addProperty( lyric );
        addProperty( note );
        addProperty( notelocation );
        addProperty( vibrato );
    }

    void ConcretePropertyView::initProperties(){
        // Lyric
        lyric = groupManager.addProperty( tr( "Lyric" ) );
        lyricPhrase = stringPropertyManager.addProperty( tr( "Phrase" ) );
        lyricPhoneticSymbol = stringPropertyManager.addProperty( tr( "Phonetic Symbol" ) );
        lyricConsonantAdjustment = stringPropertyManager.addProperty( tr( "Consonant Adjustment" ) );
        lyricProtect = enumPropertyManager.addProperty( tr( "Protect" ) );
        lyric->addSubProperty( lyricPhrase );
        lyric->addSubProperty( lyricPhoneticSymbol );
        lyric->addSubProperty( lyricConsonantAdjustment );
        lyric->addSubProperty( lyricProtect );

        // Note
        note = groupManager.addProperty( tr( "Note" ) );
        noteLength = intPropertyManager.addProperty( tr( "Length" ) );
        noteNumber = intPropertyManager.addProperty( tr( "Note#" ) );
        intPropertyManager.setRange( noteNumber, 0, 127 );
        note->addSubProperty( noteLength );
        note->addSubProperty( noteNumber );

        // Note Location
        notelocation = groupManager.addProperty( tr( "Note Location" ) );
        notelocationClock = intPropertyManager.addProperty( tr( "Clock" ) );
        notelocationMeasure = intPropertyManager.addProperty( tr( "Measure" ) );
        notelocationBeat = intPropertyManager.addProperty( tr( "Beat" ) );
        notelocationTick = intPropertyManager.addProperty( tr( "Tick" ) );
        notelocation->addSubProperty( notelocationClock );
        notelocation->addSubProperty( notelocationMeasure );
        notelocation->addSubProperty( notelocationBeat );
        notelocation->addSubProperty( notelocationTick );

        // Vibrato
        vibrato = groupManager.addProperty( tr( "Vibrato" ) );
        vibratoType = enumPropertyManager.addProperty( tr( "Vibrato" ) );
        vibratoLength = intPropertyManager.addProperty( tr( "Vibrato Length" ) );
        intPropertyManager.setRange( vibratoLength, 0, 100 );
        vibrato->addSubProperty( vibratoType );
        vibrato->addSubProperty( vibratoLength );

        QStringList types;
        types << "Off" << "On";
        enumPropertyManager.setEnumNames( lyricProtect, types );
        setFactoryForManager( &enumPropertyManager, new QtEnumEditorFactory() );

        setFactoryForManager( &stringPropertyManager, new QtLineEditFactory() );

        setFactoryForManager( &intPropertyManager, new QtSpinBoxFactory() );

        QStringList vibratoTypes;
        std::vector<std::string> topTypes;
        topTypes.push_back( "Normal" );
        topTypes.push_back( "Extreme" );
        topTypes.push_back( "Fast" );
        topTypes.push_back( "Slight" );
        vibratoTypes << "-";
        for( std::vector<std::string>::iterator i = topTypes.begin(); i != topTypes.end(); ++i ){
            for( int j = 1; j <= 4; j++ ){
                std::ostringstream name;
                name << "[" << (*i) << "] Type " << j;
                vibratoTypes << name.str().c_str();
            }
        }
        enumPropertyManager.setEnumNames( vibratoType, vibratoTypes );
    }

}
