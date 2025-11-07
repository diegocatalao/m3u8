#include <gtest/gtest.h>

extern "C" {
#include "../src/m3u8.h"
}

// ----------- m3u8_create -----------

TEST(m3u8_create_test, give_a_valid_pointer_create_a_m3u8) {
  m3u8_t* m3u8_ptr = nullptr;

  EXPECT_EQ(m3u8_create(&m3u8_ptr), M3U8_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_ptr->master, nullptr);
  EXPECT_EQ(m3u8_ptr->media, nullptr);
}

TEST(m3u8_create_test, give_a_null_pointer_returns_error) {
  EXPECT_EQ(m3u8_create(nullptr), M3U8_STATUS_INVALID_ARG);
}

TEST(m3u8_create_test, give_a_created_m3u8_returns_error) {
  m3u8_t* m3u8_ptr = nullptr;

  EXPECT_EQ(m3u8_create(&m3u8_ptr), M3U8_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_create(&m3u8_ptr), M3U8_STATUS_INVALID_ARG);
}

// ----------- m3u8_destroy -----------

TEST(m3u8_destroy_test, give_a_valid_pointer_destroy_a_m3u8) {
  m3u8_t* m3u8_ptr = nullptr;

  EXPECT_EQ(m3u8_create(&m3u8_ptr), M3U8_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_destroy(m3u8_ptr), M3U8_STATUS_NO_ERROR);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
