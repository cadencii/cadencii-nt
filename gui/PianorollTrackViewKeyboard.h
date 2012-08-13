/**
 * PianorollTrackViewKeyboard.h
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __PianorollTrackViewKeyboard_h__
#define __PianorollTrackViewKeyboard_h__

#include <QWidget>

class PianorollTrackView;

class PianorollTrackViewKeyboard : public QWidget
{
    friend class PianorollTrackView;

    Q_OBJECT

private:
    PianorollTrackView *pianoroll;

    /**
     * @brief スクロールが現在どの位置にあるか。親からの notifyVerticalScroll の呼び出しで更新される
     */
    int top;

    /**
     * @brief 鍵盤の音の名前(C4など)
     */
    QString *keyNames;

    /**
     * @brief ノートの描画高さ
     */
    int trackHeight;

public:
    explicit PianorollTrackViewKeyboard( QWidget *parent = 0 );

    ~PianorollTrackViewKeyboard();

    void paintEvent( QPaintEvent *e );

    /**
     * @brief このインスタンスを持っているピアノロールを設定する
     * @param pianoroll ピアノロール
     */
    void setPianoroll( PianorollTrackView *pianoroll );

    void setTrackHeight( int trackHeight );

private:
    /**
     * @brief 鍵盤を描画する
     */
    void paintKeyboard( QPainter *g );

    /**
     * @brief スクロール領域が縦方向にスクロールしたことを Pianoroll -> PianorollKeyboard に通知する
     */
    void notifyVerticalScroll( int y );
};

#endif
