// Copyright 2024-2025 Gleb Shigin. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#pragma once
// stl
#include <array>
#include <string>

// fwd
namespace termhack::detail {
class terminal_buffer;
}

namespace termhack::detail::symbol_manipulator {
auto generate_words(std::size_t length, std::size_t count, uint32_t rng_state) noexcept -> std::array<std::string, 20>;

void generate_term_chars(terminal_buffer& terminal, uint32_t rng_state) noexcept;

auto place_words(terminal_buffer& terminal, const std::array<std::string, 20>& words, std::size_t length, std::size_t count, uint32_t rng_state) noexcept
    -> std::array<uint16_t, 20>;

}  // namespace termhack::detail::symbol_manipulator
