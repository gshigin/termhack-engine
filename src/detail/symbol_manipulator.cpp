// Copyright 2024-2025 Gleb Shigin. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
// termhack
#include "symbol_manipulator.hpp"
#include "dictionary.hpp"
#include "termhack/random.h"
#include "terminal_buffer.hpp"
// stl
#include <algorithm>
#include <cassert>

namespace termhack::detail::symbol_manipulator {
auto generate_words(std::size_t length, std::size_t count, uint32_t rng_state) noexcept -> std::array<std::string, 20> {
  assert(4 <= length && length <= 12);
  assert(5 <= count && count <= 20);

  constexpr size_t word_array_size = dictionary::dictionary_size;

  util::xorshift32 rng{rng_state};

  const std::array<std::string, word_array_size>& word_array = [length]() -> const std::array<std::string, word_array_size>& {
    if (length == 4) {
      return dictionary::word_array_4;
    }
    if (length == 5) {
      return dictionary::word_array_5;
    }
    if (length == 6) {
      return dictionary::word_array_6;
    }
    if (length == 7) {
      return dictionary::word_array_7;
    }
    if (length == 8) {
      return dictionary::word_array_8;
    }
    if (length == 9) {
      return dictionary::word_array_9;
    }
    if (length == 10) {
      return dictionary::word_array_10;
    }
    if (length == 11) {
      return dictionary::word_array_11;
    }
    return dictionary::word_array_12;
  }();

  std::array<std::string, 20> words;
  std::size_t i = 0;
  do {
    std::string next_word = word_array[rng.next() % word_array_size];
    if (std::find(words.begin(), words.end(), next_word) == words.end()) {
      words[i++] = next_word;
    }
  } while (i < count);

  return words;
}
void generate_term_chars(terminal_buffer& terminal, uint32_t rng_state) noexcept {
  static constexpr std::array<char, 24> placeholders = {'.', ',', '!', '?', '/', '*', '+', '\'', ':', ';', '-', '_',
                                                        '%', '$', '|', '@', '{', '}', '[', ']',  '(', ')', '<', '>'};

  util::xorshift32 rng{rng_state};

  for (size_t i = 0; i < terminal.size(); ++i) {
    terminal.set(i, placeholders[rng.next() % placeholders.size()]);
  }
}
auto place_words(terminal_buffer& terminal, const std::array<std::string, 20>& words, std::size_t length, std::size_t count, uint32_t rng_state) noexcept
    -> std::array<uint16_t, 20> {
  std::array<uint16_t, 20> offsets{};

  util::xorshift32 rng{rng_state};

  const std::size_t space_per_word = terminal.size() / count;
  const std::size_t possible_start = space_per_word - length;
  for (std::size_t id = 0; id < count; ++id) {
    offsets[id] = id * space_per_word + rng.next() % possible_start;
    terminal.replace_with_string(offsets[id], words[id]);
  }

  return offsets;
}
}  // namespace termhack::detail::symbol_manipulator