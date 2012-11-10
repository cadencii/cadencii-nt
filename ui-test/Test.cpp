#include "../qt/ConcreteSettings.hpp"
#include "../qt/AppContainer.hpp"
#include "../command/AddEventCommand.hpp"
#include "Test.hpp"
#include "CurveControlChangeViewStub.hpp"
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
    container.controller.showMainView();

    container.controller.openVSQFile( "./fixture/invalid.vsq" );
}

void Test::removeSelectedEvents(){
    SettingsStub settings;
    Settings::instance( &settings );

    AppContainer container;
    container.controller.showMainView();
    ItemSelectionManager *manager = container.controller.getItemSelectionManager();

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
        container.controller.execute( &command );

        // デフォルトで追加されている歌手変更イベントと、追加した音符イベントで
        // 合計 3 つのイベントがあるはず
        QCOMPARE( 3, container.controller.getSequence()->track[0].events()->size() );
    }

    // 今しがた追加した音符を選択状態にする
    {
        const Event::List *events = container.controller.getSequence()->track[0].events();
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
    container.controller.removeSelectedItems();

    // 音符が 1 個に減っていること
    QCOMPARE( 1, container.controller.getSequence()->track[0].events()->size() );

    // 選択状態を管理するマネージャに、音符がもはや選択されていないこと
    QCOMPARE( (size_t)0, manager->getEventItemList()->size() );
}

void Test::avoidCrashByZeroLengthEvent(){
    SettingsStub settings;
    Settings::instance( &settings );

    AppContainer container;

    // 画面を表示
    container.controller.showMainView();

    // 長さが 0 のイベントを追加。
    // ビブラートイベントの長さを、イベント本体の長さに対する % で表示する処理があり、
    // ここでのクラッシュがないかどうかを特にテストしたい。
    Event note( 480, EventType::NOTE );
    note.setLength( 0 );
    note.vibratoHandle = Handle( HandleType::VIBRATO );
    vector<Event> eventList;
    eventList.push_back( note );
    AddEventCommand command( 0, eventList );
    container.controller.execute( &command );

    // 追加したイベントを取得
    const Track *track = &container.controller.getSequence()->track[0];
    const Event *targetEvent = 0;
    for( int i = 0; i < track->events()->size(); i++ ){
        const Event *item = track->events()->get( i );
        if( item->clock == 480 ){
            targetEvent = item;
            break;
        }
    }

    // 長さが 0 のイベントを選択状態とする
    ItemSelectionManager *manager = container.controller.getItemSelectionManager();
    manager->add( targetEvent );
}

void Test::exportAsVSQFileWithException(){
    SettingsStub settings;
    Settings::instance( &settings );

    AppContainer container;
    container.controller.exportAsVSQFile( "" );
}

void Test::exportAsMusicXmlWithException(){
    SettingsStub settings;
    Settings::instance( &settings );

    AppContainer container;
    container.controller.exportAsMusicXml( "" );
}

void Test::changeTrackIndex(){
    SettingsStub settings;
    Settings::instance( &settings );

    CurveControlChangeViewStub *stub = new CurveControlChangeViewStub();
    AppContainer container;
    container.controller.setControlChangeView( stub );
    container.curveControlChangeView = stub;
    container.controller.showMainView();

    // トラックが 2 つある VSQ ファイルを読み込ませる。
    // 読み込みに時間かかるかもしれないので、適当に待つ
    container.controller.openVSQFile( "./fixture/two-tracks.vsq" );
    int waitCount = 0;
    const int MAX_WAIT_COUNT = 20;
    while( container.controller.getSequence()->track.size() != 2 ){
        QTestEventLoop::instance().enterLoop( 1 );
        waitCount++;
        QVERIFY( waitCount <= MAX_WAIT_COUNT );
    }
    QCOMPARE( container.controller.getSequence()->track.size(), (size_t)2 );
    QCOMPARE( stub->getTrackIndex(), 0 );

    // 2番目のトラックが表示されている領域をクリックしたことにする
    int x = stub->getTrackTabWidthForTest() * 3 / 2;
    int y = stub->height() - stub->getLaneHeight() / 2;
    QPoint pressPosition( x, y );
    QMouseEvent *event = new QMouseEvent( QMouseEvent::MouseButtonPress, pressPosition,
                                          Qt::LeftButton, Qt::LeftButton, Qt::NoModifier );
    stub->callOnMainContentMousePressSlot(event);
    delete event;
    QTestEventLoop::instance().enterLoop( 1 );

    QCOMPARE( stub->getTrackIndex(), 1 );
}
