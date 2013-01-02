/**
 * Controller.hpp
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
#ifndef __Controller_hpp__
#define __Controller_hpp__

#include <string>
#include "gui/MainView.hpp"
#include "gui/TrackView.hpp"
#include "gui/ControlChangeView.hpp"
#include "gui/PropertyView.hpp"
#include "gui/SingerListView.hpp"
#include "gui/TrackListView.hpp"
#include "ControllerAdapter.hpp"
#include <libvsq/libvsq.h>
#include "enum/QuantizeMode.hpp"
#include "sequence/SequenceModel.hpp"

namespace cadencii {

    class Controller : public ControllerAdapter {
    private:
        TrackView *trackView;
        MainView *mainView;
        ControlChangeView *controlChangeView;
        BarCountView *barCountView;
        TempoView *tempoView;
        TimesigView *timesigView;
        PropertyView *propertyView;
        SingerListView *singerListView;
        TrackListView *trackListView;
        SequenceModel model;
        vsq::tick_t songPosition;
        int trackIndex;
        std::string saveFilePath;

        /**
         * @brief 1 tick 時刻を何ピクセルで描画するか.
         */
        double pixelPerTick;

        /**
         * @brief 選択されているツールの種類.
         */
        ToolKind::ToolKindEnum toolKind;

        /**
         * @brief アイテムの選択状態を管理するマネージャオブジェクト.
         */
        ItemSelectionManager itemSelectionManager;

    public:
        explicit Controller();

        void openVSQFile(std::string const& filePath) throw() override;

        void openXVSQFile(std::string const& filePath) throw() override;

        void drawOffsetChanged(void *sender, vsq::tick_t offset) throw() override;

        /**
         * @brief 表示するトラックのインデックスを設定する
         * @param トラックのインデックス
         */
        void setTrackIndex(void *sender, int index) throw() override;

        vsq::tick_t getSongPosition() throw() override;

        int getXFromTick(vsq::tick_t tick) throw() override;

        double getTickFromX(int x) throw() override;

        void moveSongPositionStepped(bool isBackward) throw() override;

        int getPreferredComponentWidth() throw() override;

        void exportAsMusicXml(std::string const& filePath) throw() override;

        void exportAsVSQFile(std::string const& filePath) throw() override;

        void setToolKind(ToolKind::ToolKindEnum kind) throw() override;

        ToolKind::ToolKindEnum getToolKind() throw() override;

        ItemSelectionManager* getItemSelectionManager() throw() override;

        vsq::Sequence const* getSequence() throw() override;

        void undo() override;

        void redo() override;

        bool canUndo() override;

        bool canRedo() override;

        void execute(AbstractCommand *command) override;

        void removeEvent(int trackIndex, vsq::Event const* item) override;

        void removeSelectedItems() override;

        void setApplicationShortcutEnabled(bool enabled) override;

        void notifyItemSelectionChange() throw();

        std::string getSaveFilePath()const;

        void saveAsXVSQFile(const string &filePath);

        void notifyItemSelectionChange()throw();

        vsq::PhoneticSymbolDictionary::Element const* attachPhoneticSymbol(std::string const& word) override;

        /**
         * @brief トラックのビューを設定する
         * @param[in] trackView ビュー
         */
        void setTrackView(TrackView *trackView)throw();

        /**
         * @brief コントロールチェンジのビューを設定する
         * @param [in] controlChangeView ビュー
         */
        void setControlChangeView(ControlChangeView *controlChangeView)throw();

        /**
         * @brief 小節数のビューを設定する
         * @param [in] barCountView ビュー
         */
        void setBarCountView(BarCountView *barCountView)throw();

        /**
         * @brief テンポ変更イベントのビューを設定する
         * @param [in] tempoView ビュー
         */
        void setTempoView(TempoView *tempoView)throw();

        /**
         * @brief 拍子変更イベントのビューを設定する
         * @param [in] timesigView ビュー
         */
        void setTimesigView(TimesigView *timesigView)throw();

        /**
         * @brief メインのビューを設定する
         * @param[in] mainView ビュー
         */
        void setMainView(MainView *mainView)throw();

        /**
         * @brief プロパティーのビューを設定する
         * @param[in] propertyView ビュー
         */
        void setPropertyView(PropertyView *propertyView)throw();

        /**
         * @brief Set a view of singer list.
         * @param singerListView [in] A view instance.
         */
        void setSingerListView(SingerListView *singerListView);

        /**
         * @brief Set a view of track list.
         * @param trackListView [in] A view instance.
         */
        void setTrackListView(TrackListView *trackListView);

        /**
         * @brief メインのビューを表示する
         */
        void showMainView();

    protected:
        /**
         * @brief tick 単位の時刻を、指定されたクオンタイズモードに応じて丸める
         * @todo 三連符かどうかのフラグを渡せるようにする
         * @todo テスト
         */
        vsq::tick_t getQuantizedTick(vsq::tick_t before, QuantizeMode::QuantizeModeEnum mode);

    private:
        /**
         * @brief sequence のインスタンスを置き換えたのち、各コンポーネントに sequence のインスタンスが置き換わったことを通知する
         * @param sequence 置き換わるインスタンス
         */
        void setupSequence(const vsq::Sequence &sequence);

        /**
         * @brief すべてのウィジェットを再描画する
         */
        void updateAllWidget();

        /**
         * @brief ソングポジションを設定する
         * @param songPosition ソングポジション
         */
        void setSongPosition(vsq::tick_t songPosition);
    };
}

#endif
