#include <gtest/gtest.h>

#include "detail/game_state.hpp"

using termhack::detail::game_state;

class GameStateTestFixture : public testing::Test {
 protected:
  game_state state_;
};

TEST_F(GameStateTestFixture, BracketUsageInitialState) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(state_.is_bracket_used(10));
  EXPECT_FALSE(state_.is_bracket_used(407));
}

TEST_F(GameStateTestFixture, BracketUsageAfterMarking) {
  // Arrange

  // Act
  state_.mark_bracket_used(10);
  state_.mark_bracket_used(0);

  // Assert
  EXPECT_TRUE(state_.is_bracket_used(10));
  EXPECT_TRUE(state_.is_bracket_used(0));
}

TEST_F(GameStateTestFixture, AttemptsTest) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(state_.attempts_left(), 4);
}

TEST_F(GameStateTestFixture, AttemptsDecrementTest) {
  // Arrange

  // Act
  const auto result = state_.consume_attempt();

  // Assert
  EXPECT_TRUE(result);
  EXPECT_EQ(state_.attempts_left(), 3);
}

TEST_F(GameStateTestFixture, AttemptsDecrementAllTest) {
  // Arrange

  // Act
  const auto result1 = state_.consume_attempt();
  const auto result2 = state_.consume_attempt();
  const auto result3 = state_.consume_attempt();
  const auto result4 = state_.consume_attempt();

  // Assert
  EXPECT_TRUE(result1 & result2 & result3 & result4);
  EXPECT_EQ(state_.attempts_left(), 0);
}

TEST_F(GameStateTestFixture, AttemptsResetTest) {
  // Arrange

  // Act
  const auto result = state_.consume_attempt();
  state_.reset_attempts();

  // Assert
  EXPECT_TRUE(result);
  EXPECT_EQ(state_.attempts_left(), 4);
}

TEST_F(GameStateTestFixture, AttemptsConsumeAfterAllTest) {
  // Arrange

  // Act
  state_.consume_attempt();
  state_.consume_attempt();
  state_.consume_attempt();
  state_.consume_attempt();
  const auto result = state_.consume_attempt();

  // Assert
  EXPECT_FALSE(result);
  EXPECT_EQ(state_.attempts_left(), 0);
}

TEST_F(GameStateTestFixture, GameOverFlagTrue) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(state_.is_game_over());
}

TEST_F(GameStateTestFixture, GameOverFlagFalse) {
  // Arrange

  // Act
  state_.set_game_over();

  // Assert
  EXPECT_TRUE(state_.is_game_over());
}