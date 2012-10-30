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
    protected:
        /**
         * @brief A list of note or singer event, before editing.
         */
        std::map<int, VSQ_NS::Event> originalEventItemList;

    private:
        /**
         * @brief 選択されたアイテムのうち、音符・歌手変更イベント。イベントの id をキーとした、編集中アイテムの値を保持する
         */
        std::map<int, VSQ_NS::Event> eventItemList;

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
            return eventItemList.find( event->id ) != eventItemList.end();
        }

        /**
         * @brief 選択状態の音符・歌手変更イベントのマップを取得する。
         * マップのキーは、イベントの id、値は編集操作中の値を格納する一時的インスタンス
         */
        const std::map<int, VSQ_NS::Event> *getEventItemList()const{
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
            std::map<int, VSQ_NS::Event>::iterator i
                    = eventItemList.begin();
            for( ; i != eventItemList.end(); ++i ){
                int id = i->first;
                VSQ_NS::Event editingItem = i->second;
                VSQ_NS::Event originalItem = originalEventItemList.at( id );
                editingItem.clock = originalItem.clock + deltaClocks;
                editingItem.note = originalItem.note + deltaNoteNumbers;
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
            return EditEventCommand( trackIndex, eventItemList );
        }

        /**
         * @brief アイテムの選択状態を、指定された manager のインスタンスが表すものと同等にする
         */
        void revertSelectionStatusTo( const ItemSelectionManager &manager ){
            silentClear();
            const std::map<int, VSQ_NS::Event> *eventItemList
                    = manager.getEventItemList();
            std::map<int, VSQ_NS::Event>::const_iterator i
                    = eventItemList->begin();
            for( ; i != eventItemList->end(); ++i ){
                silentAdd( &i->second );
            }
            notifyStatusChange();
        }

        /**
         * @brief Update contents of selected items.
         * @param trackIndex Index of track.
         * @param sequence An instance of Sequence.
         */
        void updateSelectedContents( int trackIndex, const VSQ_NS::Sequence *sequence ){
            const VSQ_NS::Track *track = &sequence->track[trackIndex];
            const VSQ_NS::Event::List *list = track->getConstEvents();
            std::map<int, VSQ_NS::Event>::iterator i
                    = eventItemList.begin();
            bool modified = false;
            for( ; i != eventItemList.end(); ++i ){
                int id = i->first;
                const VSQ_NS::Event *item = list->findFromId( id );
                if( item ){
                    originalEventItemList[id] = *item;
                }else{
                    modified = true;
                    silentRemove( item );
                }
            }
            if( modified ) notifyStatusChange();
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
            eventItemList.clear();
            originalEventItemList.clear();
        }

        /**
         * @brief notifyStatusChange メソッドを呼ぶことなく、アイテムを追加する
         * @param item 追加するアイテム
         * @return 追加されたかどうか。既に選択状態になっていた場合は false が返る
         */
        inline bool silentAdd( const VSQ_NS::Event *event ){
            if( !isContains( event ) ){
                eventItemList.insert( std::make_pair( event->id, *event ) );
                originalEventItemList.insert( std::make_pair( event->id, *event ) );
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
            return 0 < eventItemList.erase( event->id ) + originalEventItemList.erase( event->id );
        }
    };

}

#endif
