#include <gtest/gtest.h>
#include <cstddef>
#include <cstring>

extern "C" {
#include "../src/attr.h"
}

#define MOCK_EXT_X_STREAM             \
  "#EXT-X-STREAM-INF:"                \
  "BANDWIDTH=800000,"                 \
  "AVERAGE-BANDWIDTH=750000,"         \
  "CODECS=\"avc1.4d401f,mp4a.40.2\"," \
  "RESOLUTION=640x360,"               \
  "FRAME-RATE=30.000,"                \
  "AUDIO=\"audio\","                  \
  "SUBTITLES=\"subs\""

#define MOCK_EXT_X_STREAM_SHORT \
  "#EXT-X-STREAM-INF:BANDWIDTH=800000,AVERAGE-BANDWIDTH=750000"

// ----------- m3u8_attr_parse -----------

TEST(m3u8_attr_parse_test, given_valid_attributes_parses_successfully) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = strdup(MOCK_EXT_X_STREAM_SHORT);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "BANDWIDTH");
  EXPECT_STREQ(pivot->value, "800000");

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AVERAGE-BANDWIDTH");
  EXPECT_STREQ(pivot->value, "750000");
}

TEST(m3u8_attr_parse_test, given_null_pointer_returns_error) {
  m3u8_attr_t attrs;
  int         size = 0;
  char*       buffer = strdup(MOCK_EXT_X_STREAM);

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_attr_parse(buffer, nullptr), M3U8_ATTR_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_attr_parse(nullptr, &attrs), M3U8_ATTR_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_attr_parse(nullptr, nullptr), M3U8_ATTR_STATUS_INVALID_ARG);
}

TEST(m3u8_attr_parse_test, given_empty_string_returns_no_attributes) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = strdup("#EXT-X-STREAM-INF");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);
}

TEST(m3u8_attr_parse_test, given_null_buffer_returns_an_error) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = nullptr;

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_INVALID_ARG);
}

TEST(m3u8_attr_parse_test, given_duplicate_keys_stores_all) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char* buffer = strdup("#EXT-X-STREAM-INF:AUDIO=\"audio\",AUDIO=\"audio\"");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");
}

// ----------- m3u8_attr_from_key -----------

TEST(m3u8_attr_from_key_test, given_existing_key_returns_attribute) {
  m3u8_attr_t  attrs;
  m3u8_attr_t *attr_audio, *attr_bandwidth;
  char*        buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_EQ(m3u8_attr_from_key(&attrs, &attr_audio, strdup("AUDIO")),
            M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_STREQ(attr_audio->key, strdup("AUDIO"));
  EXPECT_STREQ(attr_audio->value, strdup("\"audio\""));

  EXPECT_EQ(m3u8_attr_from_key(&attrs, &attr_bandwidth, strdup("BANDWIDTH")),
            M3U8_ATTR_STATUS_NO_ERROR);

  EXPECT_STREQ(attr_bandwidth->key, strdup("BANDWIDTH"));
  EXPECT_STREQ(attr_bandwidth->value, strdup("800000"));
}

TEST(m3u8_attr_from_key_test, given_nonexistent_key_returns_not_found) {
  m3u8_attr_t  attrs;
  m3u8_attr_t *attr_audio, *attr_bandwidth;
  char*        buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_attr_from_key(&attrs, &attr_audio, strdup("AUDIO1")),
            M3U8_ATTR_STATUS_NOT_FOUND);
}

// ----------- m3u8_attr_count -----------

TEST(m3u8_attr_count_test, given_list_returns_correct_count) {
  m3u8_attr_t attrs;
  int         size = 0;
  char*       buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_attr_count(&attrs, &size), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(size, 7);
}

TEST(m3u8_attr_count_test, given_empty_list_returns_zero) {
  m3u8_attr_t attrs;
  int         size = -1;

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_count(&attrs, &size), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);
}

// ----------- m3u8_attr_destroy -----------

TEST(m3u8_attr_destroy_test, given_valid_list_frees_all_allocated_data) {
  m3u8_attr_t attrs;
  int         size = 0;
  char*       buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_attr_parse(buffer, &attrs), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_attr_count(&attrs, &size), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(size, 7);
  EXPECT_EQ(m3u8_attr_destroy(&attrs), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_attr_count(&attrs, &size), M3U8_ATTR_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);
}

TEST(m3u8_attr_destroy_test, given_null_pointer_returns_error) {
  m3u8_attr_t attrs;

  EXPECT_EQ(m3u8_attr_destroy(nullptr), M3U8_ATTR_STATUS_INVALID_ARG);
}

// ----------- main -----------

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
