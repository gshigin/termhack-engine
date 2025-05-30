// Copyright 2024-2025 Gleb Shigin. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
// termhack
#include "interaction_logic.hpp"
#include "game_state.hpp"
#include "termhack/random.h"
#include "terminal_buffer.hpp"
#include "word_repository.hpp"
// stl
#include <algorithm>
#include <cassert>
#include <numeric>

namespace {
constexpr auto bracket_lookup(char c) noexcept -> char {
  constexpr std::array<std::pair<char, char>, 4> brackets{{{'{', '}'}, {'(', ')'}, {'<', '>'}, {'[', ']'}}};

  return std::find_if(begin(brackets), end(brackets), [&c](const auto& v) { return v.first == c; })->second;
}

constexpr auto is_open_bracket(char c) noexcept -> bool {
  return c == '(' || c == '[' || c == '<' || c == '{';
}

constexpr auto is_char(char c) noexcept -> bool {
  return c >= 'A' && c <= 'Z';
}

auto locate_word(std::size_t index, const termhack::detail::terminal_buffer& terminal) noexcept -> termhack::highlight {
  auto l = index;
  while (l > 0 && is_char(terminal.get(l - 1)))
    --l;

  auto r = index;
  while (r < terminal.size() && is_char(terminal.get(r)))
    ++r;

  return {l, r};
}

auto locate_bracket_match(std::size_t index, const termhack::detail::terminal_buffer& terminal) noexcept -> termhack::highlight {
  const size_t limit = ((index / 12) + 1) * 12;
  const char expected_close = bracket_lookup(terminal.get(index));

  for (size_t end_index = index; end_index < limit; ++end_index) {
    char c = terminal.get(end_index);
    if (is_char(c)) {
      break;
    }
    if (c == expected_close) {
      return {index, end_index + 1};
    }
  }
  return {index, index + 1};
}

auto find_dud_offset(termhack::detail::word_repository& words, termhack::util::xorshift32& rng) noexcept -> size_t {
  std::size_t dud_index = rng.next() % (words.words_left() - 1);
  for (auto offset : words.offsets()) {
    if (!words.is_removed(offset) && !words.is_answer_offset(offset)) {
      if (dud_index-- == 0)
        return offset;
    }
  }
  return words.offsets().front();  // fallback
}
}  // namespace

auto termhack::detail::interaction_logic::look_at(std::size_t index, const terminal_buffer& terminal, game_state& state) noexcept -> highlight {
  assert(index < terminal.size());

  char c = terminal.get(index);

  if (is_char(c)) {
    return locate_word(index, terminal);
  }

  if (is_open_bracket(c) && !state.is_bracket_used(index)) {
    return locate_bracket_match(index, terminal);
  }
  // case of one symbol
  return {index, index + 1};
}

auto termhack::detail::interaction_logic::click_at(std::size_t index,
                                                   terminal_buffer& terminal,
                                                   word_repository& words,
                                                   game_state& state,
                                                   uint32_t rng_state) noexcept -> click_status {
  assert(index < terminal.size());
  assert(!state.is_game_over());

  util::xorshift32 rng{rng_state};
  const auto& [b, e] = look_at(index, terminal, state);
  std::string_view substr = terminal.slice(b, e);
  const std::string_view term = terminal.view();

  if (substr.size() == 1) {
    return {click_result::error, {}};
  }

  // word
  if (words.is_word_offset(b)) {
    if (words.is_answer_offset(b)) {
      state.set_game_over();
      return {click_result::exact_match, {}};
    }

    state.consume_attempt();

    const size_t match =
        std::inner_product(substr.begin(), substr.end(), term.begin() + words.offsets()[words.answer_index()], 0, std::plus<>(), std::equal_to<>());

    terminal.replace_with_char(b, e);
    words.mark_removed(b);

    if (state.attempts_left() == 0) {
      state.set_game_over();
      return {click_result::lockout_in_progress, word_match{match, words.word_length()}};
    }

    return {click_result::entry_denied, word_match{match, words.word_length()}};
  }

  // brackets
  const std::size_t dist = std::distance(term.data(), substr.data());

  if (state.is_bracket_used(dist)) {
    return {click_result::error, {}};
  }

  state.mark_bracket_used(dist);

  if (words.words_left() == 1) {
    state.reset_attempts();
    return {click_result::allowance_replenished, {}};
  }

  const bool should_remove_dud = (state.attempts_left() == 4) || ((rng.next() & 1) == 1);

  if (should_remove_dud) {
    const size_t dud_offset = find_dud_offset(words, rng);
    terminal.replace_with_char(dud_offset, dud_offset + words.word_length());
    words.mark_removed(dud_offset);
    return {click_result::dud_removed, {}};
  }

  state.reset_attempts();
  return {click_result::allowance_replenished, {}};
}