// Copyright 2024-2025 Gleb Shigin. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#pragma once
// termhack
#include <termhack/types.h>
// stl
#include <cstdint>

// fwd
namespace termhack::detail {
class game_state;
class terminal_buffer;
class word_repository;
}  // namespace termhack::detail

namespace termhack::detail::interaction_logic {

auto look_at(std::size_t index, const terminal_buffer& terminal, game_state& state) noexcept -> highlight;

auto click_at(std::size_t index, terminal_buffer& terminal, word_repository& words, game_state& state, uint32_t rng_state) noexcept -> click_status;

}  // namespace termhack::detail::interaction_logic