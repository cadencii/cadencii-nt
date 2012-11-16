#include "../qt/ConcreteSettings.hpp"
#include "../qt/AppContainer.hpp"
#include "../command/AddEventCommand.hpp"
#include "Test.hpp"
#include "ConcreteTrackListViewStub.hpp"
#include "ConcretePropertyViewStub.hpp"
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

    ConcreteTrackListViewStub *stub = new ConcreteTrackListViewStub();
    AppContainer container;
    container.controller.setTrackListView(stub);
    container.concreteTrackListView = stub;
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
    int y = stub->height() - stub->height() / 2;
    QPoint pressPosition( x, y );
    QMouseEvent *event = new QMouseEvent( QMouseEvent::MouseButtonPress, pressPosition,
                                          Qt::LeftButton, Qt::LeftButton, Qt::NoModifier );
    stub->callOnMousePressSlot(event);
    delete event;
    QTestEventLoop::instance().enterLoop( 1 );

    QCOMPARE( stub->getTrackIndex(), 1 );
}

void Test::propertyViewFetchProperty() {
    Sequence sequence = Sequence("hoge", 1, 4, 4, 500000);
    Event note = Event(480, EventType::NOTE);
    note.lyricHandle.setLyricAt(0, Lyric("ra", "4 a"));
    note.setLength(480);
    note.note = 60;

    Event note1 = Event(2281, EventType::NOTE);
    Lyric l("i", "i");
    l.isProtected = true;
    note1.lyricHandle.setLyricAt(0, l);
    note1.setLength(481);
    note1.note = 61;
    note1.vibratoHandle = Handle(HandleType::VIBRATO);
    note1.vibratoHandle.iconId = "$04040003";
    note1.vibratoHandle.setLength(100);

    SettingsStub settings;
    Settings::instance(&settings);
    AppContainer container;
    ConcretePropertyViewStub stub;
    stub.setControllerAdapter(&container.controller);
    cadencii::ConcretePropertyValueProxy *proxy = stub.getProxy();

    {
        // fetch phrase property
        Event noteA = note;
        Event noteB = note1;
        Lyric l("wa", "w a");
        l.isProtected = true;
        noteA.lyricHandle.setLyricAt(0, l);
        noteB.lyricHandle.setLyricAt(0, l);

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(stub.getLyricPhraseProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.getLyricAt(0).phrase, string("wa"));

        stub.fetchProperty(stub.getLyricPhoneticSymbolProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.getLyricAt(0).getPhoneticSymbol(), string("w a"));

        stub.fetchProperty(stub.getLyricConsonandAdjustmentProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.getLyricAt(0).getConsonantAdjustment(), string("64,0"));

        stub.fetchProperty(stub.getLyricProtectProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.getLyricAt(0).isProtected, true);
    }

    {
        // fetch note property
        Event noteA = note;
        Event noteB = note1;

        noteA.note = 67;
        noteB.note = 67;
        noteA.setLength(2);
        noteB.setLength(2);

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);

        stub.fetchProperty(stub.getNoteNumberProperty(), &actual, &sequence);
        QCOMPARE(actual.note, 67);

        stub.fetchProperty(stub.getNoteLengthProperty(), &actual, &sequence);
        QCOMPARE(actual.getLength(), (tick_t)2);
    }

    {
        // fetch modified notelocation property
        Event noteA = note;
        Event noteB = note1;

        noteA.clock = 0;  // clock = 0, measure = 0, beat = 1, tick = 0
        noteB.clock = 1;  // clock = 1, measure = 0, beat = 1, tick = 1
                          // clock = -, measure = 0, beat = 1, tick = -

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        QCOMPARE(proxy->getNotelocationClock(), string(""));
        QCOMPARE(proxy->getNotelocationMeasure(), string("0"));
        QCOMPARE(proxy->getNotelocationBeat(), string("1"));
        QCOMPARE(proxy->getNotelocationTick(), string(""));

        Event actual(0, EventType::NOTE);
        actual.clock = 2402;  // clock = 2402, measure = 1, beat = 2, tick = 2
                              //                      -> 0,     -> 1,        2

        stub.fetchProperty(stub.getNotelocationMeasureProperty(), &actual, &sequence);
        QCOMPARE(actual.clock, (tick_t)482);
        stub.fetchProperty(stub.getNotelocationBeatProperty(), &actual, &sequence);
        QCOMPARE(actual.clock, (tick_t)2);
    }

    {
        // fetch not mofidfied notelocation property
        Event noteA = note;
        Event noteB = note1;

        noteA.clock = 13;
        noteB.clock = 13;

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);
        actual.clock = 2402;

        stub.fetchProperty(stub.getNotelocationClockProperty(), &actual, &sequence);
        QCOMPARE(actual.clock, (tick_t)13);
    }

    {
        // fetch vibrato property
        Event noteA = note;
        Event noteB = note1;

        Handle vibrato = noteB.vibratoHandle;
        noteA.vibratoHandle = vibrato;

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(stub.getVibratoTypeProperty(), &actual, &sequence);
        stub.fetchProperty(stub.getVibratoLengthProperty(), &actual, &sequence);

        QCOMPARE(actual.vibratoHandle.getHandleType(), HandleType::VIBRATO);
        QCOMPARE(actual.vibratoHandle.iconId, string("$04040003"));
        QCOMPARE(actual.vibratoHandle.getLength(), (tick_t)100);
    }
}
