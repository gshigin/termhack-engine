#include <gtest/gtest.h>
#include <algorithm>
#include <string_view>

#include "detail/symbol_manipulator.hpp"
#include "detail/terminal_buffer.hpp"
#include "termhack/random.h"

using termhack::detail::terminal_buffer;
using termhack::util::xorshift32;

class SymbolManipulatorTestFixture : public testing::Test {
 protected:
  terminal_buffer buffer_;
  xorshift32 rng_{0xAABBCCDD};
};

TEST_F(SymbolManipulatorTestFixture, CorrectLengthAndCount) {
  // Arrange
  const std::size_t word_length = 9;
  const std::size_t word_count = 17;

  // Act
  auto words = termhack::detail::symbol_manipulator::generate_words(word_length, word_count, rng_);

  // Assert
  for (int i = 0; i < word_count; ++i) {
    EXPECT_EQ(words[i].size(), word_length);
  }
  for (int i = word_count; i < 20; ++i) {
    EXPECT_TRUE(words[i].empty());
  }
}

TEST_F(SymbolManipulatorTestFixture, GenerateChars) {
  // Arrange
  static constexpr std::array<char, 24> placeholders = {'.', ',', '!', '?', '/', '*', '+', '\'', ':', ';', '-', '_',
                                                        '%', '$', '|', '@', '{', '}', '[', ']',  '(', ')', '<', '>'};

  // Act
  termhack::detail::symbol_manipulator::generate_term_chars(buffer_, rng_);

  // Assert
  for (int i = 0; i < buffer_.size(); ++i) {
    EXPECT_TRUE(std::find(placeholders.begin(), placeholders.end(), buffer_.get(i)) != placeholders.end());
  }
}

TEST_F(SymbolManipulatorTestFixture, PlaceWords) {
  // Arrange
  const std::size_t word_length = 9;
  const std::size_t word_count = 17;

  auto words = termhack::detail::symbol_manipulator::generate_words(word_length, word_count, rng_);
  termhack::detail::symbol_manipulator::generate_term_chars(buffer_, rng_);

  // Act
  termhack::detail::symbol_manipulator::place_words(buffer_, words, word_length, word_count, rng_);

  auto buffer_view = buffer_.view();

  // Assert
  for (int i = 0; i < word_length; ++i) {
    auto word = words[i];
    EXPECT_TRUE(buffer_view.find(word) != std::string_view::npos);
  }
}