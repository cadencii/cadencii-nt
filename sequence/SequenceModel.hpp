/**
 * SequenceModel.hpp
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
#ifndef __cadencii_SequenceModel_hpp__
#define __cadencii_SequenceModel_hpp__

#include <vector>
#include <libvsq/libvsq.h>
#include "../command/AbstractCommand.hpp"

namespace cadencii {

    /**
     * @brief Sequence の操作を仲介するモデル
     */
    class SequenceModel
    {
    protected:
        vsq::Sequence sequence;
        std::vector<AbstractCommand *> commandHistory;
        int currentHistoryIndex;

    public:
        SequenceModel()
        {
            currentHistoryIndex = -1;
        }

        ~SequenceModel()
        {
            clear();
        }

        /**
         * @brief シーケンスのインスタンスへのポインターを取得する
         */
        vsq::Sequence const* getSequence() const
        {
            return &sequence;
        }

        /**
         * @brief シーケンスを操作するコマンドを実行し、操作履歴を登録する
         * @param command 実行するコマンド。command はこのメソッド内では delete されない
         */
        void execute(AbstractCommand *command)
        {
            AbstractCommand *inverseCommand = command->execute(&sequence);
            sortAllItems(&sequence);
            if (currentHistoryIndex == commandHistory.size() - 1) {
                // 新しいコマンドバッファを追加する場合.
                commandHistory.push_back(inverseCommand);
                currentHistoryIndex = commandHistory.size() - 1;
            } else {
                // 既にあるコマンドバッファを上書きする場合.
                {
                    const AbstractCommand *oldCommand = commandHistory[currentHistoryIndex + 1];
                    commandHistory[currentHistoryIndex + 1] = inverseCommand;
                    delete oldCommand;
                }
                for (int i = commandHistory.size() - 1; i >= currentHistoryIndex + 2; i--) {
                    const AbstractCommand *oldCommand = commandHistory[i];
                    std::vector<AbstractCommand *>::iterator iterator = commandHistory.begin();
                    std::advance(iterator, i);
                    commandHistory.erase(iterator);
                    delete oldCommand;
                }
                currentHistoryIndex++;
            }
        }

        /**
         * @brief undo できるかどうかを取得する
         */
        bool canUndo()
        {
            return (!commandHistory.empty()) &&
                    0 <= currentHistoryIndex &&
                    currentHistoryIndex < commandHistory.size();
        }

        /**
         * @brief redo できるかどうかを取得する
         */
        bool canRedo()
        {
            return (!commandHistory.empty()) &&
                    0 <= currentHistoryIndex + 1 &&
                    currentHistoryIndex + 1 < commandHistory.size();
        }

        /**
         * @brief 編集操作を一つ元に戻す
         */
        void undo()
        {
            AbstractCommand *undoCommand = commandHistory[currentHistoryIndex];
            AbstractCommand *reverseCommand = undoCommand->execute(&sequence);

            commandHistory[currentHistoryIndex] = reverseCommand;
            delete undoCommand;
            currentHistoryIndex--;
        }

        /**
         * @brief 編集操作を一つやり直す
         */
        void redo()
        {
            AbstractCommand *redoCommand = commandHistory[currentHistoryIndex + 1];
            AbstractCommand *reverseCommand = redoCommand->execute(&sequence);

            commandHistory[currentHistoryIndex + 1] = reverseCommand;
            delete redoCommand;
            currentHistoryIndex++;
        }

        /**
         * @brief 操作履歴を初期化し、シーケンスを設定する
         */
        void reset(vsq::Sequence const& sequence)
        {
            clear();
            this->sequence = sequence;
        }

    private:
        /**
         * @brief 編集操作履歴を消去する
         */
        void clear() {
            currentHistoryIndex = -1;
            std::vector<AbstractCommand *>::iterator i = commandHistory.begin();
            for (; i != commandHistory.end(); ++i) {
                delete (*i);
            }
            commandHistory.clear();
        }

        /**
         * @brief シーケンス中の、時系列順に並べ替えが必要なすべての項目の並べ替えを行う
         * @param sequence 編集対象のシーケンス
         */
        void sortAllItems(vsq::Sequence *sequence) {
            // 各トラックのカーブと、シーケンスのtimesigList はそれぞれ自動でソートされるので
            // 気にしなくてよい.
            for (vsq::Track &track : sequence->tracks()) {
                track.events().sort();
            }
            sequence->tempoList.updateTempoInfo();
            sequence->updateTotalTicks();
        }
    };
}

#endif
