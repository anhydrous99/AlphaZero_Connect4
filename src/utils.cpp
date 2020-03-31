//
// Created by aherrera on 3/29/20.
//

#include "utils.h"

#include <utility>
#include <gsl/gsl_randist.h>

bool operator<(const State &a, const State &b) {
    return std::lexicographical_compare(
            a.data(), a.data() + a.size(),
            b.data(), b.data() + b.size()
    );
}

TensorPair::TensorPair(const torch::Tensor &t1, const torch::Tensor &t2) : tensor1(t1), tensor2(t1) {}

MoveResult::MoveResult(State st, bool wn) : state(std::move(st)), done(wn) {}

LeafResult::LeafResult(float val, State cur_state, Player pl, std::vector<State> st, std::vector<int64_t> acts)
        : value(val), current_state(std::move(cur_state)), current_player(pl), states(std::move(st)),
          actions(std::move(acts)) {}

VSAT::VSAT(float val, std::vector<State> sts, std::vector<int64_t> acts)
        : value(val), states(std::move(sts)), actions(std::move(acts)) {}

SAT::SAT(State st, std::vector<State> sts, std::vector<int64_t> acts)
        : state(std::move(st)), states(std::move(sts)), actions(std::move(acts)) {}

Vector7f generate_dirichlet(const Vector7d &alpha) {
    Vector7d output(Vector7d::Zero());
    double *output_ptr = output.data();
    const double *alpha_ptr = alpha.data();

    const gsl_rng_type *T;
    gsl_rng *r;

    gsl_rng_env_setup();

    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    gsl_ran_dirichlet(r, 7, alpha_ptr, output_ptr);

    gsl_rng_free(r);
    Vector7f output_float = output.cast<float>();
    return output_float;
}

torch::Tensor get_state_tensor(const State &state) {
    torch::Tensor output_tensor = torch::zeros({2, 6, 7}, torch::dtype(torch::kFloat32));
    auto accessor = output_tensor.accessor<float, 3>();
    for (auto col = 0; col < state.cols(); ++col) {
        for (auto row = 0; row < state.rows(); ++row) {
            Player p = state(row, col);
            if (p == Player::Player1)
                accessor[0][row][col] = 1.0f;
            else if (p == Player::Player2)
                accessor[1][row][col] = 1.0f;
        }
    }
    return output_tensor;
}

torch::Tensor get_states_tensors(const std::vector<State> &states) {
    std::vector<torch::Tensor> tensor_list;
    for (const auto& state : states)
        tensor_list.push_back(get_state_tensor(state));
    return torch::stack(tensor_list);
}
