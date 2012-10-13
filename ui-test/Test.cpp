#include "../qt/ConcreteSettings.hpp"
#include "../qt/AppContainer.hpp"
#include "Test.hpp"
#include <QTextCodec>
#include <QTestEventLoop>
#include <QTest>
#include <QThread>

Test::Test(){
    QTextCodec *codec = QTextCodec::codecForName( "UTF-8" );
    QTextCodec::setCodecForCStrings( codec );
    QTextCodec::setCodecForTr( codec );
}

void Test::openVsqFileWithException(){
    SettingsStub settings;
    cadencii::Settings::instance( &settings );

    cadencii::AppContainer container;
    container.c.showMainView();

    container.c.openVSQFile( "./fixture/invalid.vsq" );
}
