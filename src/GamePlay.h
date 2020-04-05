//
// Created by Armando Herrera on 4/4/2020.
//

#ifndef ALPHAZERO_CONNECT4_GAMEPLAY_H
#define ALPHAZERO_CONNECT4_GAMEPLAY_H

#include "MCTS.h"
#include "Model.h"
#include "Defines.h"
#include "Utilities.h"
#include "FixedQueue.h"

GameResult
play_game(const std::vector<MCTS> &mcts_store, FixedQueue<BufferEntry, REPLAY_BUFFER> &replay_buffer, Model &net1,
          Model &net2, int64_t steps_before_tau_0, int64_t mcts_searches, int64_t mcts_batch_size);

#endif //ALPHAZERO_CONNECT4_GAMEPLAY_H
