// Copyright 2025 Gleb Shigin. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#pragma once
// stl
#include <cstdint>
#include <memory>
#include <optional>

namespace termhack {

enum class lock_level : uint8_t { very_easy, easy, average, hard, very_hard };

enum class input_type : uint8_t {
  look,
  click,
  other // treated as nothing happened
};

struct input {
  input_type type;
  size_t idx;
};

enum class click_result {
  error,                 // non-interacting symbol clock
  dud_removed,           // brackets pair click, remove one wrong answer
  allowance_replenished, // brackets pair click, restore attempts
  entry_denied,          // word click, wrong answer
  lockout_in_progress,   // lock after all attemps are over
  exact_match            // answer is guessed
};

struct highlight {
  size_t begin;
  size_t end;
};

struct word_match {
  std::size_t of;
  std::size_t from;
};

struct event_result {
  std::optional<highlight> new_highlight;    // for look
  std::optional<click_result> click_outcome; // for click
  std::optional<word_match> match;           // for exact_match
};

struct state {
  uint8_t attempts_left;
  bool is_locked;
};

struct transition {
  state next_state;
  event_result event;
};

enum class fill_status { ok, error };

class engine {
public:
  engine(lock_level lock_level_setting, uint32_t player_science_skill,
         uint64_t seed) noexcept;
  engine() noexcept : engine(lock_level::average, 50, 0){};

  ~engine();

  engine(engine const &) = delete;
  auto operator=(engine const &) -> engine & = delete;

  engine(engine &&) noexcept = delete;
  auto operator=(engine &&) noexcept -> engine & = delete;

  static state initial_state() noexcept {
    return state{
        .attempts_left = 4,
        .is_locked = false,
    };
  }

  auto process_input(const state &current_state,
                     const input &in) noexcept -> transition;

  fill_status fill_terminal_buffer(char *buffer, size_t buffer_size,
                                   const state &current_state) const noexcept;

private:
  struct impl;
  std::unique_ptr<impl> pimpl_;
};
} // namespace termhack