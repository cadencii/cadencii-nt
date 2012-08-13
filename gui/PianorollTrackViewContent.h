/**
 * PianorollTrackViewContent.h
 * Copyright © 2012 kbinani
 *
 * This file is part of `MIDIInput UG Job Plugin'.
 *
 * `MIDIInput UG Job Plugin' is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * `MIDIInput UG Job Plugin' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __PianorollTrackViewContent_h__
#define __PianorollTrackViewContent_h__

#include <map>
#include <QWidget>
#include <QMutex>
#include "vsq/TimesigList.hpp"
#include "vsq/MeasureLineIterator.hpp"
#include "vsq/Track.hpp"

class PianorollTrackView;

class PianorollTrackViewContent : public QWidget
{
    Q_OBJECT

public:
    /**
     * ノート描画高さのデフォルト値(ピクセル単位)
     */
    static const int DEFAULT_TRACK_HEIGHT = 14;

    static const int NOTE_MIN = 0;

    static const int NOTE_MAX = 127;

private:
    PianorollTrackView *pianoroll;

    /**
     * @brief 描画されるアイテムの一覧
     */
    VSQ_NS::Track *track;

    /**
     * 拍ごとの線を描画するための、拍子変更情報
     */
    VSQ_NS::TimesigList *timesigList;

    /**
     * @brief デフォルトの拍子変更情報
     */
    VSQ_NS::TimesigList defaultTimesigList;

    /**
     * @brief 拍ごとの線を描画するために、timesigList から作成した反復子
     */
    VSQ_NS::MeasureLineIterator *measureLineIterator;

    /**
     * @brief ノートの描画高さ
     */
    int trackHeight;

    /**
     * 1 tick 時刻を何ピクセルで描画するか
     */
    double pixelPerTick;

    /**
     * @brief ソングポジション
     */
    cadencii::vsq::tick_t songPosition;

    /**
     * @brief 描画アイテムのリストをロックするための Mutex
     */
    QMutex *mutex;

    /**
     * @brief 曲頭から Musical Part までのオフセット
     */
    VSQ_NS::tick_t musicalPartOffset;

public:
    /**
     * @brief ノート番号から、音名を表す番号を取得する。Cであれば0, C#であれば1など
     * @param noteNumber ノート番号
     * @return 音名を表す番号
     */
    static int getNoteModuration( int noteNumber );

    /**
     * @brief ノート番号から、その音高が何オクターブめかを取得する。
     * @param noteNumber ノート番号
     * @return 何オクターブめかを表す番号
     */
    static int getNoteOctave( int noteNumber );

    /**
     * @brief ノート番号から、描画時の y 座標を取得する
     * @param noteNumber ノート番号
     * @param trackHeight ノートの描画高さ
     */
    static int getYFromNoteNumber( int noteNumber, int trackHeight );

    /**
     * @brief y 座標からノート番号を取得する
     * @param y 座標
     * @return ノート番号
     */
    static int getNoteNumberFromY( int y, int trackHeight );

    explicit PianorollTrackViewContent( QWidget *parent = 0 );

    ~PianorollTrackViewContent();

    /**
     * @brief 描画対象のトラックを設定する
     * @param items 描画対象のトラック
     */
    void setTrack( VSQ_NS::Track *track );

    /**
     * @brief テンポ変更リストを設定する
     * @param timesigList テンポ変更リスト
     */
    void setTimesigList( VSQ_NS::TimesigList *timesigList );

    /**
     * オーバーライドする。ピアノロールの描画処理が追加される
     */
    void paintEvent( QPaintEvent *e );

    /**
     * オーバーライドする。再描画処理が追加される
     */
    void mouseMoveEvent( QMouseEvent *e );

    /**
     * @brief ノートの描画高さを設定する
     * @param trackHeight ノートの描画高さ
     */
    void setTrackHeight( int trackHeight );

    /**
     * @brief ノートの描画高さを取得する
     * @return ノートの描画高さ
     */
    int getTrackHeight();

    /**
     * @brief このインスタンスを持っているピアノロールを設定する
     * @param pianoroll ピアノロール
     */
    void setPianoroll( PianorollTrackView *pianoroll );

    /**
     * スクロールされた結果、可視状態となっている領域を取得する
     */
    QRect getVisibleArea();

    /**
     * @brief ソングポジションを設定する
     */
    void setSongPosition( cadencii::vsq::tick_t songPosition );

    /**
     * @brief ソングポジションを取得する
     */
    cadencii::vsq::tick_t getSongPosition();

    /**
     * @brief ミューテックスを設定する
     * @param mutex ミューテックス
     */
    void setMutex( QMutex *mutex );

    /**
     * @brief tick 単位の時刻から、描画時の x 座標を取得する
     * @param tick 時刻
     * @return x 座標
     */
    int getXFromTick( cadencii::vsq::tick_t tick );

    /**
     * @brief 曲頭から Musical Part までのオフセットを設定する
     * @param musicalPartOffset オフセット
     */
    void setMusicalPartOffset( VSQ_NS::tick_t musicalPartOffset );

private:
    /**
     * ピアノロールのバックグラウンドを描画する
     */
    void paintBackground( QPainter *g, QRect visibleArea );

    /**
     * アイテムを描画する
     */
    void paintItems( QPainter *g, QRect visibleArea );

    /**
     * 1拍ごとの線を描画する
     */
    void paintMeasureLines( QPainter *g, QRect visibleArea );

    /**
     * @brief ソングポジションを描画する
     */
    void paintSongPosition( QPainter *g, QRect visibleArea );

    /**
     * @brief x 座標から、tick 単位の時刻を取得する
     */
    inline double getTickFromX( int x );

    /**
     * @brief 最小高さを取得する
     */
    inline int getMinimumHeight();

    QRect getPaintArea();
};

#endif
