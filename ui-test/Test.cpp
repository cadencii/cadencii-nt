#include "../qt/ConcreteSettings.hpp"
#include "../qt/AppContainer.hpp"
#include "../command/AddEventCommand.hpp"
#include "Test.hpp"
#include <QTextCodec>
#include <QTestEventLoop>
#include <QTest>
#include <QThread>

using namespace std;
using namespace cadencii;
using namespace VSQ_NS;

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

void Test::removeSelectedEvents(){
    SettingsStub settings;
    Settings::instance( &settings );

    AppContainer container;
    container.c.showMainView();
    ItemSelectionManager *manager = container.c.getItemSelectionManager();

    // シーケンスに音符を 2 つ追加する
    {
        Event noteA( 480, EventType::NOTE );
        noteA.setLength( 480 );
        Event noteB( 1920, EventType::NOTE );
        noteB.setLength( 480 );
        vector<Event> eventList;
        eventList.push_back( noteA );
        eventList.push_back( noteB );
        AddEventCommand command( 0, eventList );
        container.c.execute( &command );

        // デフォルトで追加されている歌手変更イベントと、追加した音符イベントで
        // 合計 3 つのイベントがあるはず
        QCOMPARE( 3, container.c.getSequence()->track[0].getConstEvents()->size() );
    }

    // 今しがた追加した音符を選択状態にする
    {
        const Event::List *events = container.c.getSequence()->track[0].getConstEvents();
        for( int i = 0; i < events->size(); i++ ){
            const Event *item = events->get( i );
            if( 480 == item->clock ||
                1920 == item->clock )
            {
                manager->add( item );
            }
        }

        QCOMPARE( (size_t)2, manager->getEventItemList()->size() );
    }

    // テスト対象の removeSelectedItems を実行
    container.c.removeSelectedItems();

    // 音符が 1 個に減っていること
    QCOMPARE( 1, container.c.getSequence()->track[0].getConstEvents()->size() );

    // 選択状態を管理するマネージャに、音符がもはや選択されていないこと
    QCOMPARE( (size_t)0, manager->getEventItemList()->size() );
}
