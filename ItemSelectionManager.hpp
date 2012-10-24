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
#include "command/EditEventCommand.hpp"
#include <set>
#include <map>

namespace cadencii{

    /**
     * @brief 編集対象アイテムの選択状態を管理するクラス
     * @todo 選択状態となっているアイテムがどのトラックに存在するかどうかを、このクラスが保持するべきなのではないか？
     *       そして、後から別トラックのアイテムが追加されようとした際は、自動で選択状態をクリアし、
     *       新たなアイテムだけ選択状態となるようにするべき。
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
            silentClear();
            notifyStatusChange();
        }

        /**
         * @brief アイテムを選択状態にする
         */
        void add( const VSQ_NS::Event *event ){
            if( silentAdd( event ) ){
                notifyStatusChange();
            }
        }

        /**
         * @brief アイテムを選択状態にする
         */
        void add( const std::set<const VSQ_NS::Event *> eventList ){
            std::set<const VSQ_NS::Event *>::const_iterator i
                    = eventList.begin();
            bool added = false;
            for( ; i != eventList.end(); ++i ){
                added |= silentAdd( *i );
            }
            if( added ) notifyStatusChange();
        }

        /**
         * @brief アイテムを選択状態から解除する
         * @param event 選択状態を解除するアイテム
         */
        void remove( const VSQ_NS::Event *event ){
            if( silentRemove( event ) ){
                notifyStatusChange();
            }
        }

        /**
         * @brief アイテムを選択状態から解除する
         * @param event 選択状態を解除するアイテムのリスト
         */
        void remove( const std::set<const VSQ_NS::Event *> eventList ){
            std::set<const VSQ_NS::Event *>::const_iterator i
                    = eventList.begin();
            bool removed = false;
            for( ; i != eventList.end(); ++i ){
                removed |= silentRemove( *i );
            }
            if( removed ) notifyStatusChange();
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

        /**
         * @brief 選択されたアイテムが編集された状態を、SequenceModel に反映するためのコマンドを取得する
         * @param trackIndex 選択されたアイテムが存在するトラックの番号
         * @return コマンド
         * @todo trackIndex 引数を削除できるよう努める
         */
        EditEventCommand getEditEventCommand( int trackIndex ){
            std::map<int, VSQ_NS::Event> itemList;
            std::map<const VSQ_NS::Event *, VSQ_NS::Event>::const_iterator i
                    = eventItemList.begin();
            for( ; i != eventItemList.end(); ++i ){
                const VSQ_NS::Event *selectedItem = i->first;
                VSQ_NS::Event editedItem = i->second;
                int eventId = selectedItem->id;
                editedItem.id = eventId;
                itemList.insert( std::make_pair( eventId, editedItem ) );
            }
            return EditEventCommand( trackIndex, itemList );
        }

        /**
         * @brief アイテムの選択状態を、指定された manager のインスタンスが表すものと同等にする
         */
        void revertSelectionStatusTo( const ItemSelectionManager &manager ){
            silentClear();
            const std::map<const VSQ_NS::Event *, VSQ_NS::Event> *eventItemList
                    = manager.getEventItemList();
            std::map<const VSQ_NS::Event *, VSQ_NS::Event>::const_iterator i
                    = eventItemList->begin();
            for( ; i != eventItemList->end(); ++i ){
                silentAdd( i->first );
            }
            notifyStatusChange();
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

        /**
         * @brief notifyStatusChange メソッドを呼ぶことなく、ステータスをクリアする
         */
        inline void silentClear(){
            itemList.clear();
            eventItemList.clear();
        }

        /**
         * @brief notifyStatusChange メソッドを呼ぶことなく、アイテムを追加する
         * @param item 追加するアイテム
         * @return 追加されたかどうか。既に選択状態になっていた場合は false が返る
         */
        inline bool silentAdd( const VSQ_NS::Event *event ){
            if( !isContains( event ) ){
                itemList.insert( (void *)event );
                eventItemList.insert( std::make_pair( event, *event ) );
                return true;
            }else{
                return false;
            }
        }

        /**
         * @brief notifyStatusChange メソッドを呼ぶことなく、アイテムを削除する
         * @param item 削除するアイテム
         * @return 削除されたかどうか。既に選択状態でなかった場合は false が返る
         */
        inline bool silentRemove( const VSQ_NS::Event *event ){
            std::set<void *>::iterator index = itemList.find( (void *)event );
            if( index != itemList.end() ){
                itemList.erase( index );
                eventItemList.erase( event );
                return true;
            }else{
                return false;
            }
        }
    };

}

#endif
