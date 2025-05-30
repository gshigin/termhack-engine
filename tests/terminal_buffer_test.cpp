#include <gtest/gtest.h>

#include "detail/terminal_buffer.hpp"

using termhack::detail::terminal_buffer;

class TerminalBufferTestFixture : public testing::Test {
 protected:
  terminal_buffer buffer_;
};

TEST_F(TerminalBufferTestFixture, SizeConstant) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(buffer_.size(), 408);
}

TEST_F(TerminalBufferTestFixture, InitiallyZeroedContent) {
  // Arrange

  // Act

  // Assert
  for (size_t i = 0; i < buffer_.size(); ++i) {
    EXPECT_EQ(buffer_.get(i), '\0');
  }
}

TEST_F(TerminalBufferTestFixture, SetAndGetCharacters) {
  // Arrange

  // Act
  buffer_.set(0, 'A');
  buffer_.set(407, 'Z');

  // Assert
  EXPECT_EQ(buffer_.get(0), 'A');
  EXPECT_EQ(buffer_.get(407), 'Z');
}

TEST_F(TerminalBufferTestFixture, SliceReturnsCorrectRange) {
  // Arrange

  // Act
  buffer_.set(100, 'x');
  buffer_.set(101, 'y');
  buffer_.set(102, 'z');

  auto view = buffer_.slice(100, 103);

  // Assert
  EXPECT_EQ(view.size(), 3);
  EXPECT_EQ(view, "xyz");
}

TEST_F(TerminalBufferTestFixture, SliceAtEndBoundary) {
  // Arrange

  // Act
  buffer_.set(405, 'a');
  buffer_.set(406, 'b');
  buffer_.set(407, 'c');

  auto view = buffer_.slice(405, 408);

  // Assert
  EXPECT_EQ(view, "abc");
}

TEST_F(TerminalBufferTestFixture, ViewReturnsFullContent) {
  // Arrange

  // Act
  buffer_.set(0, '1');
  buffer_.set(1, '2');
  buffer_.set(2, '3');

  auto view = buffer_.view();

  // Assert
  EXPECT_EQ(view.substr(0, 3), "123");
  EXPECT_EQ(view.size(), buffer_.size());
}

TEST_F(TerminalBufferTestFixture, ReplaceWithCharInRange) {
  // Arrange

  // Act
  buffer_.replace_with_char(100, 103, '*');

  // Assert
  EXPECT_EQ(buffer_.get(100), '*');
  EXPECT_EQ(buffer_.get(101), '*');
  EXPECT_EQ(buffer_.get(102), '*');
}

TEST_F(TerminalBufferTestFixture, ReplaceWithStringWithinBounds) {
  // Arrange

  // Act
  buffer_.replace_with_string(50, "hello");

  // Assert
  EXPECT_EQ(buffer_.get(50), 'h');
  EXPECT_EQ(buffer_.get(51), 'e');
  EXPECT_EQ(buffer_.get(52), 'l');
  EXPECT_EQ(buffer_.get(53), 'l');
  EXPECT_EQ(buffer_.get(54), 'o');
}

TEST_F(TerminalBufferTestFixture, ReplaceWithStringAtEdge) {
  // Arrange

  // Act
  buffer_.replace_with_string(405, "xyz");

  // Assert
  EXPECT_EQ(buffer_.get(405), 'x');
  EXPECT_EQ(buffer_.get(406), 'y');
  EXPECT_EQ(buffer_.get(407), 'z');
}

TEST_F(TerminalBufferTestFixture, RawAccessReflectsSetChanges) {
  // Arrange

  // Act
  buffer_.set(123, 'K');

  const auto& raw = buffer_.raw();

  // Assert
  EXPECT_EQ(raw[123], 'K');
}
