#include <gtest/gtest.h>
#include <cstddef>
#include <cstring>

extern "C" {
#include "../src/parser.h"
}

#define MOCK_EXT_X_STREAM             \
  "EXT-X-STREAM-INF:"                 \
  "BANDWIDTH=800000,"                 \
  "AVERAGE-BANDWIDTH=750000,"         \
  "CODECS=\"avc1.4d401f,mp4a.40.2\"," \
  "RESOLUTION=640x360,"               \
  "FRAME-RATE=30.000,"                \
  "AUDIO=\"audio\","                  \
  "SUBTITLES=\"subs\""

#define MOCK_EXT_X_STREAM_SHORT "EXT-X-STREAM-INF:BANDWIDTH=800000,AVERAGE-BANDWIDTH=750000"

// ----------- m3u8_parser_attr -----------

TEST(m3u8_parser_attr_test, given_valid_attributes_parses_successfully) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t* pivot = &attrs;
  char*               buffer = strdup(MOCK_EXT_X_STREAM_SHORT);

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_linked_next(pivot, m3u8_parser_attr_t, list);
  EXPECT_STREQ(pivot->key, "BANDWIDTH");
  EXPECT_STREQ(pivot->value, "800000");

  pivot = m3u8_linked_next(pivot, m3u8_parser_attr_t, list);
  EXPECT_STREQ(pivot->key, "AVERAGE-BANDWIDTH");
  EXPECT_STREQ(pivot->value, "750000");
}

TEST(m3u8_parser_attr_test, given_null_pointer_returns_error) {
  m3u8_parser_attr_t attrs;
  int                size = 0;
  char*              buffer = strdup(MOCK_EXT_X_STREAM);

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_parser_attr(buffer, nullptr), M3U8_PARSER_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_parser_attr(nullptr, &attrs), M3U8_PARSER_STATUS_INVALID_ARG);
  EXPECT_EQ(m3u8_parser_attr(nullptr, nullptr), M3U8_PARSER_STATUS_INVALID_ARG);
}

TEST(m3u8_parser_attr_test, given_empty_string_returns_no_attributes) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t* pivot = &attrs;
  char*               buffer = strdup("EXT-X-STREAM-INF");

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_linked_next(pivot, m3u8_parser_attr_t, list);
  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);
}

TEST(m3u8_parser_attr_test, given_null_buffer_returns_an_error) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t* pivot = &attrs;
  char*               buffer = nullptr;

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_INVALID_ARG);
}

TEST(m3u8_parser_attr_test, given_duplicate_keys_stores_all) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t* pivot = &attrs;
  char*               buffer = strdup("EXT-X-STREAM-INF:AUDIO=\"audio\",AUDIO=\"audio\"");

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_EQ(pivot->key, nullptr);
  EXPECT_EQ(pivot->value, nullptr);

  pivot = m3u8_linked_next(pivot, m3u8_parser_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");

  pivot = m3u8_linked_next(pivot, m3u8_parser_attr_t, list);
  EXPECT_STREQ(pivot->key, "AUDIO");
  EXPECT_STREQ(pivot->value, "\"audio\"");
}

// ----------- m3u8_parser_attr_from_key -----------

TEST(m3u8_parser_attr_from_key_test, given_existing_key_returns_attribute) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t *attr_audio, *attr_bandwidth;
  char*               buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_EQ(m3u8_parser_attr_from_key(&attrs, &attr_audio, strdup("AUDIO")),
            M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_STREQ(attr_audio->key, strdup("AUDIO"));
  EXPECT_STREQ(attr_audio->value, strdup("\"audio\""));

  EXPECT_EQ(m3u8_parser_attr_from_key(&attrs, &attr_bandwidth, strdup("BANDWIDTH")),
            M3U8_PARSER_STATUS_NO_ERROR);

  EXPECT_STREQ(attr_bandwidth->key, strdup("BANDWIDTH"));
  EXPECT_STREQ(attr_bandwidth->value, strdup("800000"));
}

TEST(m3u8_parser_attr_from_key_test, given_nonexistent_key_returns_not_found) {
  m3u8_parser_attr_t  attrs;
  m3u8_parser_attr_t *attr_audio, *attr_bandwidth;
  char*               buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_parser_attr_from_key(&attrs, &attr_audio, strdup("AUDIO1")),
            M3U8_PARSER_STATUS_NOT_FOUND);
}

// ----------- m3u8_parser_attr_count -----------

TEST(m3u8_parser_attr_count_test, given_list_returns_correct_count) {
  m3u8_parser_attr_t attrs;
  int                size = 0;
  char*              buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_parser_attr_count(&attrs, &size), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(size, 7);
}

TEST(m3u8_parser_attr_count_test, given_empty_list_returns_zero) {
  m3u8_parser_attr_t attrs;
  int                size = -1;

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr_count(&attrs, &size), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);
}

// ----------- m3u8_parser_attr_destroy -----------

TEST(m3u8_parser_attr_destroy_test, given_valid_list_frees_all_allocated_data) {
  m3u8_parser_attr_t attrs;
  int                size = 0;
  char*              buffer = strdup(MOCK_EXT_X_STREAM);

  memset(&attrs, 0, sizeof(m3u8_parser_attr_t));

  EXPECT_EQ(m3u8_parser_attr(buffer, &attrs), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_parser_attr_count(&attrs, &size), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(size, 7);
  EXPECT_EQ(m3u8_parser_attr_destroy(&attrs), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_parser_attr_count(&attrs, &size), M3U8_PARSER_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);
}

TEST(m3u8_parser_attr_destroy_test, given_null_pointer_returns_error) {
  m3u8_parser_attr_t attrs;

  EXPECT_EQ(m3u8_parser_attr_destroy(nullptr), M3U8_PARSER_STATUS_INVALID_ARG);
}

// ----------- m3u8_parser_tag_from_str -----------

TEST(m3u8_parser_tag_from_str_test, valid_extm3u_tag_returns_correct_enum) {
  char* line = strdup("#EXTM3U");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXTM3U);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, valid_ext_x_version_tag_returns_enum) {
  char* line = strdup("#EXT-X-VERSION:1");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXT_X_VERSION);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, valid_extinf_tag_returns_correct_enum) {
  char* line = strdup("#EXTINF:10.0");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXTINF);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, invalid_tag_no_ext_returns_unknown) {
  char* line = strdup("NOT_A_TAG");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXT_UNKNOWN);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, invalid_tag_hash_no_ext_returns_unknown) {
  char* line = strdup("#NOTEXT");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXT_UNKNOWN);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, unknown_ext_tag_returns_unknown) {
  char* line = strdup("#EXT-X-UNKNOWN-TAG:value");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXT_UNKNOWN);
  free(line);
}

TEST(m3u8_parser_tag_from_str_test, null_input_returns_unknown) {
  EXPECT_EQ(m3u8_parser_tag_from_str(nullptr), M3U8_PARSER_EXT_UNKNOWN);
}

TEST(m3u8_parser_tag_from_str_test, empty_string_returns_unknown) {
  char* line = strdup("");
  EXPECT_EQ(m3u8_parser_tag_from_str(line), M3U8_PARSER_EXT_UNKNOWN);
  free(line);
}

// ----------- m3u8_parser_playlist_type -----------

TEST(m3u8_parser_playlist_type_test, master_playlist_detects_stream_inf) {
  const char* manifest =
    "#EXTM3U\n"
    "#EXT-X-STREAM-INF:BANDWIDTH=150000\n"
    "media.m3u8\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MASTER_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, media_playlist_detects_extinf) {
  const char* manifest =
    "#EXTM3U\n"
    "#EXTINF:10.0,\n"
    "segment1.ts\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MEDIA_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, unknown_playlist_no_tags) {
  const char* manifest =
    "#EXTM3U\n"
    "#SOME_OTHER_TAG\n"
    "file.ts\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_UNKNOWN_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, empty_manifest_returns_unknown) {
  const char* manifest = "";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_UNKNOWN_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, null_manifest_returns_unknown) {
  EXPECT_EQ(m3u8_parser_playlist_type(nullptr), M3U8_PARSER_UNKNOWN_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, extm3u_only_returns_unknown) {
  const char* manifest = "#EXTM3U\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_UNKNOWN_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, manifest_with_empty_lines) {
  const char* manifest =
    "#EXTM3U\n"
    "\n"
    "#EXTINF:10.0,\n"
    "\n"
    "segment1.ts\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MEDIA_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, manifest_no_final_newline) {
  const char* manifest =
    "#EXTM3U\n"
    "#EXT-X-STREAM-INF:BANDWIDTH=150000";  // No final newline
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MASTER_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, master_tag_first_returns_master) {
  const char* manifest =
    "#EXTM3U\n"
    "#EXT-X-STREAM-INF:BANDWIDTH=150000\n"
    "#EXTINF:10.0,\n"  // Media tag after master
    "media.m3u8\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MASTER_PLAYLIST);
}

TEST(m3u8_parser_playlist_type_test, media_tag_first_returns_media) {
  const char* manifest =
    "#EXTM3U\n"
    "#EXTINF:10.0,\n"  // Media tag first
    "#EXT-X-STREAM-INF:BANDWIDTH=150000\n"
    "segment1.ts\n";
  EXPECT_EQ(m3u8_parser_playlist_type(manifest), M3U8_PARSER_MEDIA_PLAYLIST);
}

// ----------- m3u8_parser_from_str -----------

TEST(m3u8_parser_from_str, given_tag_with_value_parses_successfully) {
  m3u8_parser_t*       parser = NULL;
  const char*          line = "#EXT-X-VERSION:7";
  m3u8_parser_status_t status = m3u8_parser_from_str(line, &parser);

  ASSERT_EQ(status, M3U8_PARSER_STATUS_NO_ERROR);
  ASSERT_NE(parser, nullptr);
  EXPECT_EQ(parser->tag, M3U8_PARSER_EXT_X_VERSION);
  ASSERT_NE(parser->value, nullptr);
  EXPECT_STREQ(parser->value, "7");
  EXPECT_NE(parser->attrs, nullptr);
}

TEST(m3u8_parser_from_str, given_valueless_tag_parses_successfully) {
  m3u8_parser_t*       parser = NULL;
  const char*          line = "#EXT-X-ENDLIST";
  m3u8_parser_status_t status = m3u8_parser_from_str(line, &parser);

  ASSERT_EQ(status, M3U8_PARSER_STATUS_NO_ERROR);
  ASSERT_NE(parser, nullptr);
  EXPECT_EQ(parser->tag, M3U8_PARSER_EXT_X_ENDLIST);
  EXPECT_EQ(parser->value, nullptr);
  EXPECT_EQ(parser->attrs, nullptr);
}

TEST(m3u8_parser_from_str, given_tag_with_attributes_parses_successfully) {
  m3u8_parser_t*       parser = NULL;
  const char*          line = "#EXT-X-STREAM-INF:BANDWIDTH=1280000,CODECS=\"avc1.4d401f\"";
  m3u8_parser_status_t status = m3u8_parser_from_str(line, &parser);

  ASSERT_EQ(status, M3U8_PARSER_STATUS_NO_ERROR);
  ASSERT_NE(parser, nullptr);
  EXPECT_EQ(parser->tag, M3U8_PARSER_EXT_X_STREAM_INF);
  ASSERT_NE(parser->value, nullptr);
  ASSERT_NE(parser->attrs, nullptr);

  int attr_count = 0;
  m3u8_parser_attr_count(parser->attrs, &attr_count);
  EXPECT_EQ(attr_count, 2);
}

TEST(m3u8_parser_from_str, given_tag_with_value_and_no_attributes) {
  m3u8_parser_t*       parser = NULL;
  const char*          line = "#EXTINF:9.009,";
  m3u8_parser_status_t status = m3u8_parser_from_str(line, &parser);

  ASSERT_EQ(status, M3U8_PARSER_STATUS_NO_ERROR);
  ASSERT_NE(parser, nullptr);
  EXPECT_EQ(parser->tag, M3U8_PARSER_EXTINF);
  ASSERT_NE(parser->value, nullptr);
  EXPECT_STREQ(parser->value, "9.009,");
  ASSERT_NE(parser->attrs, nullptr);

  int attr_count = 0;
  m3u8_parser_attr_count(parser->attrs, &attr_count);
  EXPECT_EQ(attr_count, 0);
}

TEST(m3u8_parser_from_str, given_invalid_line_returns_failure) {
  m3u8_parser_t*       parser = NULL;
  const char*          line = "THIS IS NOT A VALID TAG";
  m3u8_parser_status_t status = m3u8_parser_from_str(line, &parser);

  EXPECT_EQ(status, M3U8_PARSER_STATUS_FAILURE);
  EXPECT_EQ(parser, nullptr);
}

TEST(m3u8_parser_from_str, given_null_line_returns_invalid_arg) {
  m3u8_parser_t*       parser = NULL;
  m3u8_parser_status_t status = m3u8_parser_from_str(NULL, &parser);

  EXPECT_EQ(status, M3U8_PARSER_STATUS_INVALID_ARG);
  EXPECT_EQ(parser, nullptr);
}

// ----------- main -----------

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
