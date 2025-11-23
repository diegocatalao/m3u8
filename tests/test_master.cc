#include <gtest/gtest.h>
#include <string.h>

extern "C" {
#include "../src/linked.h"
#include "../src/master.h"
}

static m3u8_master_t* __m3u8_create_master_mock() {
  m3u8_master_t* master = (m3u8_master_t*)calloc(1, sizeof(m3u8_master_t));
  size_t         size = 0; /** size of sturcts just to reduce the line length */

  master->is_signed = true;
  master->version = 7;
  master->is_independent_segments = false;

  /** allocate the #EXT-X-START */
  master->ext_x_start = (m3u8_ext_x_start_t*)calloc(1, sizeof(m3u8_ext_x_start_t));
  master->ext_x_start->precise = true;
  master->ext_x_start->time_offset = 10;

  /** allocate the #EXT-X-SESSION-DATA */
  size = sizeof(m3u8_ext_x_session_data_t);
  master->ext_x_session_data = (m3u8_ext_x_session_data_t*)calloc(1, size);
  master->ext_x_session_data->data_id = strdup("session-1");
  master->ext_x_session_data->value = strdup("english value");
  master->ext_x_session_data->uri = strdup("https://example.com/session-1");
  master->ext_x_session_data->language = strdup("en");
  EXPECT_EQ(m3u8_linked_init(&master->ext_x_session_data->list), M3U8_LINKED_STATUS_NO_ERROR);

  /** allocate the #EXT-X-SESSION-KEY */
  size = sizeof(m3u8_ext_x_session_key_t);
  master->ext_x_session_key = (m3u8_ext_x_session_key_t*)calloc(1, size);
  master->ext_x_session_key->method = strdup("AES-128");
  master->ext_x_session_key->uri = strdup("https://example.com/key");
  strcpy((char*)master->ext_x_session_key->iv, "1234567890123456");
  master->ext_x_session_key->key_format = strdup("identity");
  master->ext_x_session_key->key_format_versions = strdup("1/2");
  EXPECT_EQ(m3u8_linked_init(&master->ext_x_session_key->list), M3U8_LINKED_STATUS_NO_ERROR);

  /** allocate the #EXT-X-MEDIA */
  size = sizeof(m3u8_ext_x_media_t);
  master->ext_x_media = (m3u8_ext_x_media_t*)calloc(1, size);
  master->ext_x_media->group_id = strdup("audio");
  master->ext_x_media->name = strdup("English");
  master->ext_x_media->type = M3U8_EXT_X_MEDIA_TYPE_AUDIO;
  master->ext_x_media->is_default = true;
  master->ext_x_media->is_autoselect = true;
  master->ext_x_media->is_forced = false;
  master->ext_x_media->language = strdup("en");
  master->ext_x_media->assoc_language = strdup("en");
  master->ext_x_media->instream_id = strdup("C1");
  master->ext_x_media->characteristics = strdup("public.accessibility.transcribes-spoken-dialog");
  master->ext_x_media->channels = strdup("1/2");
  master->ext_x_media->uri = strdup("https://example.com/audio.m3u8");
  EXPECT_EQ(m3u8_linked_init(&master->ext_x_media->list), M3U8_LINKED_STATUS_NO_ERROR);

  /** allocate the #EXT-X-SESSION-KEY */
  size = sizeof(m3u8_ext_x_stream_inf_t);
  master->ext_x_stream_inf = (m3u8_ext_x_stream_inf_t*)calloc(1, size);
  master->ext_x_stream_inf->bandwidth = 1234567890;
  master->ext_x_stream_inf->average_bandwidth = 12345;
  master->ext_x_stream_inf->codecs = strdup("avc1.4d002a,mp4a.40.2");
  master->ext_x_stream_inf->resolution = strdup("640x360");
  master->ext_x_stream_inf->frame_rate = 30.000;
  master->ext_x_stream_inf->hdcp_level = strdup("TYPE-0");
  master->ext_x_stream_inf->audio = strdup("opus");
  master->ext_x_stream_inf->video = strdup("h264");
  master->ext_x_stream_inf->subtitles = strdup("subtitles");
  master->ext_x_stream_inf->closed_captions = strdup("closed-captions");
  master->ext_x_stream_inf->uri = strdup("https://example.com/video");
  EXPECT_EQ(m3u8_linked_init(&master->ext_x_stream_inf->list), M3U8_LINKED_STATUS_NO_ERROR);

  /** allocate the #EXT-X-SESSION-KEY */
  size = sizeof(m3u8_ext_x_i_frame_stream_inf_t);
  master->ext_x_i_frame_stream_inf = (m3u8_ext_x_i_frame_stream_inf_t*)calloc(1, size);
  master->ext_x_i_frame_stream_inf->bandwidth = 999999;
  master->ext_x_i_frame_stream_inf->average_bandwidth = 444444;
  master->ext_x_i_frame_stream_inf->codecs = strdup("avc1.4d002a,mp4a.40.2");
  master->ext_x_i_frame_stream_inf->resolution = strdup("640x360");
  master->ext_x_i_frame_stream_inf->hdcp_level = strdup("TYPE-0");
  master->ext_x_i_frame_stream_inf->video = strdup("h264");
  master->ext_x_i_frame_stream_inf->uri = strdup("https://example.com/video");
  EXPECT_EQ(m3u8_linked_init(&master->ext_x_i_frame_stream_inf->list), M3U8_LINKED_STATUS_NO_ERROR);

  return master;
}

// ----------- m3u8_master_destroy -----------

TEST(m3u8_master_destroy_test, given_a_valid_master_ptr_destroy_it) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_an_invalid_pointer_returns_error) {
  EXPECT_EQ(m3u8_master_destroy(nullptr), M3U8_MASTER_STATUS_INVALID_ARG);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_start) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  /** remove the ext_x_start struct  */
  free(master->ext_x_start);
  master->ext_x_start = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_session_data) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_session_data_t* entry = NULL;
  m3u8_ext_x_session_data_t* head = master->ext_x_session_data;

  /** remove the ext_x_session_data struct  */
  m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_data_t, list) {
    free(master->ext_x_session_data->data_id);
    free(master->ext_x_session_data->value);
    free(master->ext_x_session_data->uri);
    free(master->ext_x_session_data->language);
    free(entry);
  }

  free(master->ext_x_session_data);
  master->ext_x_session_data = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_session_key) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_session_key_t* entry = NULL;
  m3u8_ext_x_session_key_t* head = master->ext_x_session_key;

  /** remove the ext_x_session_key struct  */
  m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_session_key_t, list) {
    free(master->ext_x_session_key->method);
    free(master->ext_x_session_key->uri);
    free(master->ext_x_session_key->key_format);
    free(master->ext_x_session_key->key_format_versions);
    free(entry);
  }

  free(master->ext_x_session_key);
  master->ext_x_session_key = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_media) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_media_t* entry = NULL;
  m3u8_ext_x_media_t* head = master->ext_x_media;

  /** remove the ext_x_media struct  */
  m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_media_t, list) {
    free(master->ext_x_media->group_id);
    free(master->ext_x_media->name);
    free(master->ext_x_media->language);
    free(master->ext_x_media->assoc_language);
    free(master->ext_x_media->instream_id);
    free(master->ext_x_media->characteristics);
    free(master->ext_x_media->channels);
    free(master->ext_x_media->uri);
    free(entry);
  }

  free(master->ext_x_media);
  master->ext_x_media = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_stream_inf) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_stream_inf_t* entry = NULL;
  m3u8_ext_x_stream_inf_t* head = master->ext_x_stream_inf;

  /** remove the ext_x_stream_inf struct  */
  m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_stream_inf_t, list) {
    free(master->ext_x_stream_inf->codecs);
    free(master->ext_x_stream_inf->resolution);
    free(master->ext_x_stream_inf->hdcp_level);
    free(master->ext_x_stream_inf->audio);
    free(master->ext_x_stream_inf->video);
    free(master->ext_x_stream_inf->subtitles);
    free(master->ext_x_stream_inf->closed_captions);
    free(master->ext_x_stream_inf->uri);
    free(entry);
  }

  free(master->ext_x_stream_inf);
  master->ext_x_stream_inf = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_without_ext_x_i_frame_stream_inf) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_i_frame_stream_inf_t* entry = NULL;
  m3u8_ext_x_i_frame_stream_inf_t* head = master->ext_x_i_frame_stream_inf;

  /** remove the ext_x_i_frame_stream_inf struct  */
  m3u8_linked_foreach(entry, &head->list, m3u8_ext_x_i_frame_stream_inf_t, list) {
    free(master->ext_x_i_frame_stream_inf->codecs);
    free(master->ext_x_i_frame_stream_inf->resolution);
    free(master->ext_x_i_frame_stream_inf->hdcp_level);
    free(master->ext_x_i_frame_stream_inf->video);
    free(master->ext_x_i_frame_stream_inf->uri);
    free(entry);
  }

  free(master->ext_x_i_frame_stream_inf);
  master->ext_x_i_frame_stream_inf = nullptr;

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_with_a_ext_x_session_data_list) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_session_data_t* entry = NULL;
  entry = (m3u8_ext_x_session_data_t*)calloc(1, sizeof(m3u8_ext_x_session_data_t));

  entry->data_id = strdup("session-2");
  entry->value = strdup("spanish value");
  entry->uri = strdup("https://example.com/session-2");
  entry->language = strdup("es");

  m3u8_linked_ina(&master->ext_x_session_data->list, &entry->list);

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_with_a_ext_x_session_key_list) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_session_key_t* entry = NULL;
  entry = (m3u8_ext_x_session_key_t*)calloc(1, sizeof(m3u8_ext_x_session_key_t));

  entry->method = strdup("AES-256");
  entry->uri = strdup("https://example.com/key-2");
  strcpy((char*)entry->iv, "1234567890123456");
  entry->key_format = strdup("identity");
  entry->key_format_versions = strdup("1/2");

  m3u8_linked_ina(&master->ext_x_session_key->list, &entry->list);

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_with_a_ext_x_media_list) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_media_t* entry = NULL;
  entry = (m3u8_ext_x_media_t*)calloc(1, sizeof(m3u8_ext_x_media_t));

  entry->group_id = strdup("audio-2");
  entry->name = strdup("Spanish");
  entry->type = M3U8_EXT_X_MEDIA_TYPE_AUDIO;
  entry->is_default = false;
  entry->is_autoselect = false;
  entry->is_forced = true;
  entry->language = strdup("es");
  entry->assoc_language = strdup("es");
  entry->instream_id = strdup("CC1");
  entry->characteristics = strdup("public.accessibility.describes-video");
  entry->channels = strdup("2");
  entry->uri = strdup("https://example.com/audio-2.m3u8");

  m3u8_linked_ina(&master->ext_x_media->list, &entry->list);

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_with_a_ext_x_stream_inf_list) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_stream_inf_t* entry = NULL;
  entry = (m3u8_ext_x_stream_inf_t*)calloc(1, sizeof(m3u8_ext_x_stream_inf_t));

  entry->bandwidth = 1234567890;
  entry->average_bandwidth = 12345;
  entry->codecs = strdup("avc1.4d002a,mp4a.40.2");
  entry->resolution = strdup("640x360");
  entry->frame_rate = 30.000;
  entry->hdcp_level = strdup("TYPE-0");
  entry->audio = strdup("opus");
  entry->video = strdup("h264");
  entry->subtitles = strdup("subtitles");
  entry->closed_captions = strdup("closed-captions");
  entry->uri = strdup("https://example.com/video-2");

  m3u8_linked_ina(&master->ext_x_stream_inf->list, &entry->list);

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

TEST(m3u8_master_destroy_test, given_a_valid_m3u8_with_a_ext_x_i_frame_stream_inf_list) {
  m3u8_master_t* master = __m3u8_create_master_mock();

  m3u8_ext_x_i_frame_stream_inf_t* entry = NULL;
  entry = (m3u8_ext_x_i_frame_stream_inf_t*)calloc(1, sizeof(m3u8_ext_x_i_frame_stream_inf_t));

  entry->bandwidth = 999999;
  entry->average_bandwidth = 444444;
  entry->codecs = strdup("avc1.4d002a,mp4a.40.2");
  entry->resolution = strdup("640x360");
  entry->hdcp_level = strdup("TYPE-0");
  entry->video = strdup("h264");
  entry->uri = strdup("https://example.com/video-2");

  m3u8_linked_ina(&master->ext_x_i_frame_stream_inf->list, &entry->list);

  ASSERT_NE(nullptr, master);
  EXPECT_EQ(m3u8_master_destroy(master), M3U8_MASTER_STATUS_NO_ERROR);
}

// ----------- main -----------

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
