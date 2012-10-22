/**
 * ItemSelectionManager.hpp
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
#ifndef __cadencii_ItemSelectionManager_hpp__
#define __cadencii_ItemSelectionManager_hpp__

#include "vsq/Event.hpp"
#include "ItemSelectionStatusListener.hpp"
#include <set>
#include <map>

namespace cadencii{

    /**
     * @brief 編集対象アイテムの選択状態を管理するクラス
     */
    class ItemSelectionManager{
    private:
        /**
         * @brief 選択されたアイテムへのポインタ
         */
        std::set<void *> itemList;

        /**
         * @brief 選択されたアイテムのうち、音符・歌手変更イベントへのポインタ。これをキーとした、編集中アイテムの値を保持する
         */
        std::map<const VSQ_NS::Event *, VSQ_NS::Event> eventItemList;

        /**
         * @brief アイテムの選択状態を監視するリスナーのリスト
         */
        std::vector<ItemSelectionStatusListener *> listenerList;

    public:
        /**
         * @brief アイテムの選択状態を解除する
         */
        void clear(){
            itemList.clear();
            eventItemList.clear();
            notifyStatusChange();
        }

        /**
         * @brief アイテムを選択状態にする
         */
        void add( const VSQ_NS::Event *event ){
            if( !isContains( event ) ){
                itemList.insert( (void *)event );
                eventItemList.insert( std::make_pair( event, *event ) );
                notifyStatusChange();
            }
        }

        /**
         * @brief アイテムを選択状態から解除する
         * @param event 選択状態を解除するアイテム
         */
        void remove( const VSQ_NS::Event *event ){
            std::set<void *>::iterator index = itemList.find( (void *)event );
            if( index != itemList.end() ){
                itemList.erase( index );
                eventItemList.erase( event );
                notifyStatusChange();
            }
        }

        /**
         * @brief アイテムが選択状態担っているかどうかを取得する
         */
        bool isContains( const VSQ_NS::Event *event )const{
            return itemList.find( (void *)event ) != itemList.end();
        }

        /**
         * @brief 選択状態の音符・歌手変更イベントのマップを取得する。
         * マップのキーは、選択状態の編集前のインスタンスを指すポインタ、値は編集操作中の値を格納する一時的インスタンス
         */
        const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *getEventItemList()const{
            return &eventItemList;
        }

        /**
         * @brief 選択状態を監視するリスナーを登録する
         */
        void addStatusListener( ItemSelectionStatusListener *listener ){
            listenerList.push_back( listener );
        }

        /**
         * @brief 選択状態のアイテムを、指定した時間、指定したノート番号分だけ移動する
         * @param deltaClocks 移動する時間
         * @param deltaNoteNumbers 移動するノート番号
         */
        void moveItems( VSQ_NS::tick_t deltaClocks, int deltaNoteNumbers ){
            std::map<const VSQ_NS::Event *, VSQ_NS::Event>::iterator i
                    = eventItemList.begin();
            for( ; i != eventItemList.end(); ++i ){
                const VSQ_NS::Event *originalItem = i->first;
                VSQ_NS::Event editingItem = i->second;
                editingItem.clock = originalItem->clock + deltaClocks;
                editingItem.note = originalItem->note + deltaNoteNumbers;
                i->second = editingItem;
            }
        }

    private:
        /**
         * @brief すべてのリスナーに、選択状態が変化したことを通知する
         */
        void notifyStatusChange(){
            std::vector<ItemSelectionStatusListener *>::iterator i = listenerList.begin();
            for( ; i != listenerList.end(); ++i ){
                (*i)->statusChanged();
            }
        }
    };

}

#endif
