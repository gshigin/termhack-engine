#include <gtest/gtest.h>

#include "detail/word_repository.hpp"

using termhack::detail::word_repository;

class WordRepositoryTestFixture : public ::testing::Test {
 protected:
  word_repository repo_;
  std::array<uint16_t, 20> default_offsets_{};

  void SetUp() override {
    for (uint16_t i = 0; i < 20; ++i) {
      default_offsets_[i] = static_cast<uint16_t>(100 + i * 5);  // 100, 105, ..., 195
    }

    repo_.init(6, 10, 3, default_offsets_);
  }
};

TEST_F(WordRepositoryTestFixture, InitializedCorrectly) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(repo_.word_length(), 6);
  EXPECT_EQ(repo_.word_count(), 10);
  EXPECT_EQ(repo_.answer_index(), 3);
  EXPECT_EQ(repo_.words_left(), 10);
}

TEST_F(WordRepositoryTestFixture, OffsetsAccessible) {
  // Arrange

  // Act
  const auto& offs = repo_.offsets();

  // Assert
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(offs[i], 100 + i * 5);
  }
}

TEST_F(WordRepositoryTestFixture, IsWordOffsetWorks) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(repo_.is_word_offset(115));
  EXPECT_FALSE(repo_.is_word_offset(999));
}

TEST_F(WordRepositoryTestFixture, IsAnswerOffsetWorks) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(repo_.is_answer_offset(115));
  EXPECT_FALSE(repo_.is_answer_offset(100));
}

TEST_F(WordRepositoryTestFixture, IsRemovedInitiallyFalse) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(repo_.is_removed(100));
  EXPECT_FALSE(repo_.is_removed(115));
  EXPECT_FALSE(repo_.is_removed(145));
}

TEST_F(WordRepositoryTestFixture, MarkRemovedSetsCorrectFlag) {
  // Arrange

  // Act
  repo_.mark_removed(115);

  // Assert
  EXPECT_TRUE(repo_.is_removed(115));
  EXPECT_FALSE(repo_.is_removed(100));
  EXPECT_EQ(repo_.words_left(), 9);
}

TEST_F(WordRepositoryTestFixture, MarkRemovedNonExistingOffsetDoesNothing) {
  // Arrange

  // Act
  repo_.mark_removed(999);  // not in list

  // Assert
  EXPECT_EQ(repo_.words_left(), 10);
}

TEST_F(WordRepositoryTestFixture, InitMarksExtraOffsetsAsRemoved) {
  // Arrange

  // Act

  // Assert
  for (size_t i = 10; i < 20; ++i) {
    EXPECT_TRUE(repo_.is_removed(default_offsets_[i]));
  }
}
