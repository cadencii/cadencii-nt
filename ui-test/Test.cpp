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
using namespace vsq;

Test::Test(){
    QTextCodec *codec = QTextCodec::codecForName( "UTF-8" );
    QTextCodec::setCodecForLocale( codec );
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
        noteA.length( 480 );
        Event noteB( 1920, EventType::NOTE );
        noteB.length( 480 );
        vector<Event> eventList;
        eventList.push_back( noteA );
        eventList.push_back( noteB );
        AddEventCommand command( 0, eventList );
        container.controller.execute( &command );

        // デフォルトで追加されている歌手変更イベントと、追加した音符イベントで
        // 合計 3 つのイベントがあるはず
        QCOMPARE(3, container.controller.getSequence()->track(0)->events()->size());
    }

    // 今しがた追加した音符を選択状態にする
    {
        Event::List const& events = container.controller.getSequence()->track(0).events();
        for( int i = 0; i < events.size(); i++ ){
            Event const* item = events.get( i );
            if( 480 == item->tick ||
                1920 == item->tick )
            {
                manager->add( item );
            }
        }

        QCOMPARE( (size_t)2, manager->getEventItemList()->size() );
    }

    // テスト対象の removeSelectedItems を実行
    container.controller.removeSelectedItems();

    // 音符が 1 個に減っていること
    QCOMPARE(1, container.controller.getSequence()->track(0).events().size());

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
    // ここでのクラッシュがないかどうかを特にテストしたい.
    Event note( 480, EventType::NOTE );
    note.length( 0 );
    note.vibratoHandle = Handle( HandleType::VIBRATO );
    vector<Event> eventList;
    eventList.push_back( note );
    AddEventCommand command( 0, eventList );
    container.controller.execute( &command );

    // 追加したイベントを取得.
    Track const& track = container.controller.getSequence()->track(0);
    const Event *targetEvent = 0;
    for( int i = 0; i < track.events().size(); i++ ){
        const Event *item = track.events().get( i );
        if( item->tick == 480 ){
            targetEvent = item;
            break;
        }
    }

    // 長さが 0 のイベントを選択状態とする.
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
    // 読み込みに時間かかるかもしれないので、適当に待つ.
    container.controller.openVSQFile( "./fixture/two-tracks.vsq" );
    int waitCount = 0;
    const int MAX_WAIT_COUNT = 20;

    while (container.controller.getSequence()->tracks().size() != 2) {
        QTestEventLoop::instance().enterLoop( 1 );
        waitCount++;
        QVERIFY( waitCount <= MAX_WAIT_COUNT );
    }
    QCOMPARE(container.controller.getSequence()->tracks().size(), (size_t)2);
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
    note.lyricHandle.set(0, Lyric("ra", "4 a"));
    note.length(480);
    note.note = 60;

    Event note1 = Event(2281, EventType::NOTE);
    Lyric l("i", "i");
    l.isProtected = true;
    note1.lyricHandle.set(0, l);
    note1.length(481);
    note1.note = 61;
    note1.vibratoHandle = Handle(HandleType::VIBRATO);
    note1.vibratoHandle.iconId = "$04040003";
    note1.vibratoHandle.length(100);

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
        noteA.lyricHandle.set(0, l);
        noteB.lyricHandle.set(0, l);

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);
        stub.fetchProperty(stub.getLyricPhraseProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.get(0).phrase, string("wa"));

        stub.fetchProperty(stub.getLyricPhoneticSymbolProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.get(0).phoneticSymbol(), string("w a"));

        stub.fetchProperty(stub.getLyricConsonandAdjustmentProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.get(0).consonantAdjustment(), string("64,0"));

        stub.fetchProperty(stub.getLyricProtectProperty(), &actual, &sequence);
        QCOMPARE(actual.lyricHandle.get(0).isProtected, true);
    }

    {
        // fetch note property
        Event noteA = note;
        Event noteB = note1;

        noteA.note = 67;
        noteB.note = 67;
        noteA.length(2);
        noteB.length(2);

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);

        stub.fetchProperty(stub.getNoteNumberProperty(), &actual, &sequence);
        QCOMPARE(actual.note, 67);

        stub.fetchProperty(stub.getNoteLengthProperty(), &actual, &sequence);
        QCOMPARE(actual.length(), (tick_t)2);
    }

    {
        // fetch modified notelocation property
        Event noteA = note;
        Event noteB = note1;

        noteA.tick = 0;  // clock = 0, measure = 0, beat = 1, tick = 0
        noteB.tick = 1;  // clock = 1, measure = 0, beat = 1, tick = 1
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
        actual.tick = 2402;  // clock = 2402, measure = 1, beat = 2, tick = 2
                              //                      -> 0,     -> 1,        2

        stub.fetchProperty(stub.getNotelocationMeasureProperty(), &actual, &sequence);
        QCOMPARE(actual.tick, (tick_t)482);
        stub.fetchProperty(stub.getNotelocationBeatProperty(), &actual, &sequence);
        QCOMPARE(actual.tick, (tick_t)2);
    }

    {
        // fetch not mofidfied notelocation property
        Event noteA = note;
        Event noteB = note1;

        noteA.tick = 13;
        noteB.tick = 13;

        proxy->begin();
        proxy->add(&noteA, &sequence);
        proxy->add(&noteB, &sequence);
        proxy->commit();

        Event actual(0, EventType::NOTE);
        actual.tick = 2402;

        stub.fetchProperty(stub.getNotelocationClockProperty(), &actual, &sequence);
        QCOMPARE(actual.tick, (tick_t)13);
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

        QCOMPARE(actual.vibratoHandle.type(), HandleType::VIBRATO);
        QCOMPARE(actual.vibratoHandle.iconId, string("$04040003"));
        QCOMPARE(actual.vibratoHandle.length(), (tick_t)100);
    }
}

void Test::openXvsq() {
    SettingsStub settings;
    cadencii::Settings::instance( &settings );

    cadencii::AppContainer container;
    container.controller.showMainView();

    container.controller.openVSQFile( "./fixture/valid.xvsq" );
}

void Test::openXvsqWithException() {
    SettingsStub settings;
    cadencii::Settings::instance( &settings );

    cadencii::AppContainer container;
    container.controller.showMainView();

    container.controller.openVSQFile( "./fixture/invalid.xvsq" );
}
