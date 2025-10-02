#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "../src/ext.h"
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

#define MOCK_EXT_X_STREAM_ATTS_STR    \
  "BANDWIDTH=800000,"                 \
  "AVERAGE-BANDWIDTH=750000,"         \
  "CODECS=\"avc1.4d401f,mp4a.40.2\"," \
  "RESOLUTION=640x360,"               \
  "FRAME-RATE=30.000,"                \
  "AUDIO=\"audio\","                  \
  "SUBTITLES=\"subs\""

#define MOCK_EXT_X_STREAM_LONG \
  "#EXT-THIS-IS-A-VERY-LONG-TAG-NAME-EXCEEDING-32-CHARS:FOO=BAR"

#define MOCK_EXT_X_STREAM_SHORT \
  "#EXT-X-STREAM-INF:BANDWIDTH=800000,AVERAGE-BANDWIDTH=750000"

// ---------------- m3u8_ext_lookup_tag ----------------

TEST(m3u8_ext_lookup_tag_test, parses_valid_tag_and_value) {
  m3u8_ext_e ext;
  char*      value = NULL;
  char*      line = strdup(MOCK_EXT_X_STREAM);

  char* expected_value_str = strdup(MOCK_EXT_X_STREAM_ATTS_STR);

  EXPECT_EQ(m3u8_ext_lookup_tag(line, &ext, &value), M3U8_EXT_STATUS_NO_ERROR);
  EXPECT_EQ(ext, M3U8_EXT_STREAM_INF);
  EXPECT_STREQ(value, expected_value_str);

  free(line);
  free(value);
  free(expected_value_str);
}

TEST(m3u8_ext_lookup_tag_test, returns_error_on_null_line) {
  m3u8_ext_e ext = M3U8_EXT_UNKNOWN;
  char*      value = NULL;

  EXPECT_EQ(m3u8_ext_lookup_tag(NULL, &ext, &value),
            M3U8_EXT_STATUS_INVALID_ARG);
  EXPECT_EQ(ext, M3U8_EXT_UNKNOWN);
}

TEST(m3u8_ext_lookup_tag_test, returns_error_on_null_ext_ptr) {
  m3u8_ext_e* ext = NULL;
  char*       value = NULL;
  char*       line = strdup(MOCK_EXT_X_STREAM);

  EXPECT_EQ(m3u8_ext_lookup_tag(line, ext, &value),
            M3U8_EXT_STATUS_INVALID_ARG);

  free(line);
}

TEST(m3u8_ext_lookup_tag_test, returns_error_on_null_value_ptr) {
  m3u8_ext_e ext = M3U8_EXT_UNKNOWN;
  char*      line = strdup(MOCK_EXT_X_STREAM);

  EXPECT_EQ(m3u8_ext_lookup_tag(line, &ext, NULL), M3U8_EXT_STATUS_INVALID_ARG);
  EXPECT_EQ(ext, M3U8_EXT_UNKNOWN);

  free(line);
}

TEST(m3u8_ext_lookup_tag_test, returns_error_on_invalid_tag_format) {
  m3u8_ext_e ext = M3U8_EXT_UNKNOWN;
  char*      value = NULL;
  char*      line = strdup("#INVALID-X-TAG");

  EXPECT_EQ(m3u8_ext_lookup_tag(line, &ext, &value),
            M3U8_EXT_STATUS_INVALID_TAGS);
  EXPECT_EQ(ext, M3U8_EXT_UNKNOWN);

  free(line);
  free(value);
}

TEST(m3u8_ext_lookup_tag_test, returns_error_on_tag_too_long) {
  m3u8_ext_e ext = M3U8_EXT_UNKNOWN;
  char*      value = NULL;
  char*      line = strdup(MOCK_EXT_X_STREAM_LONG);

  EXPECT_EQ(m3u8_ext_lookup_tag(line, &ext, &value),
            M3U8_EXT_STATUS_INVALID_TAGS);
  EXPECT_EQ(ext, M3U8_EXT_UNKNOWN);

  free(line);
  free(value);
}

TEST(m3u8_ext_lookup_tag_test, handles_tag_with_max_length) {
  m3u8_ext_e ext = M3U8_EXT_UNKNOWN;
  char*      value = NULL;
  char*      line = strdup("#EXT-THIS-IS-A-31-CHARACTER-TAG:FOO=BAR");
  char*      expected_value = strdup("FOO=BAR");

  EXPECT_EQ(m3u8_ext_lookup_tag(line, &ext, &value), M3U8_EXT_STATUS_NO_ERROR);
  EXPECT_EQ(ext, M3U8_EXT_UNKNOWN);
  EXPECT_STREQ(value, expected_value);

  free(line);
  free(value);
  free(expected_value);
}

// ---------------- m3u8_ext_lookup_attr ----------------

TEST(m3u8_ext_lookup_attr_test, given_valid_attributes_parses_successfully) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = strdup(MOCK_EXT_X_STREAM_SHORT);

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "BANDWIDTH");
  EXPECT_STREQ(pivot->value, "800000");

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AVERAGE-BANDWIDTH");
  EXPECT_STREQ(pivot->value, "750000");
}

TEST(m3u8_ext_lookup_attr_test, given_null_pointer_returns_error) {
  m3u8_attr_t attrs;
  int         size = 0;
  char*       buffer = strdup(MOCK_EXT_X_STREAM);

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, NULL), M3U8_EXT_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_ext_lookup_attr(NULL, &attrs), M3U8_EXT_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_ext_lookup_attr(NULL, NULL), M3U8_EXT_STATUS_INVALID_ARG);
}

TEST(m3u8_ext_lookup_attr_test, given_empty_string_returns_no_attributes) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = strdup("#EXT-X-STREAM-INF");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);
}

TEST(m3u8_ext_lookup_attr_test, given_null_buffer_returns_an_error) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char*        buffer = NULL;

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_INVALID_ARG);
}

TEST(m3u8_ext_lookup_attr_test, given_duplicate_keys_stores_all) {
  m3u8_attr_t  attrs;
  m3u8_attr_t* pivot = &attrs;
  char* buffer = strdup("#EXT-X-STREAM-INF:AUDIO=\"audio\",AUDIO=\"audio\"");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");

  pivot = m3u8_list_next(pivot, m3u8_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");
}

// ---------------- m3u8_ext_destroy_attr ----------------

TEST(m3u8_ext_destroy_attr_test, destroys_empty_list) {
  m3u8_attr_t attrs;
  char* buffer = strdup("#EXT-X-STREAM-INF:BANDWIDTH=800000,AUDIO=\"audio\"");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_ext_destroy_attr(&attrs), M3U8_EXT_STATUS_NO_ERROR);

  EXPECT_EQ(attrs.key, nullptr);
  EXPECT_EQ(attrs.value, nullptr);

  bool empty = false;
  EXPECT_EQ(m3u8_list_is_empty(&attrs.list, &empty), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_TRUE(empty);
}

TEST(m3u8_ext_destroy_attr_test, returns_error_on_null_argument) {
  EXPECT_EQ(m3u8_ext_destroy_attr(NULL), M3U8_EXT_STATUS_INVALID_ARG);
}

TEST(m3u8_ext_destroy_attr_test, double_destroy_does_not_crash) {
  m3u8_attr_t attrs;
  char* buffer = strdup("#EXT-X-STREAM-INF:BANDWIDTH=800000,AUDIO=\"audio\"");

  memset(&attrs, 0, sizeof(m3u8_attr_t));

  EXPECT_EQ(m3u8_ext_lookup_attr(buffer, &attrs), M3U8_EXT_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_ext_destroy_attr(&attrs), M3U8_EXT_STATUS_NO_ERROR);

  EXPECT_EQ(attrs.key, nullptr);
  EXPECT_EQ(attrs.value, nullptr);

  bool empty = false;
  EXPECT_EQ(m3u8_list_is_empty(&attrs.list, &empty), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_TRUE(empty);

  EXPECT_EQ(m3u8_ext_destroy_attr(&attrs), M3U8_EXT_STATUS_INVALID_ARG);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
