
#include <gtest/gtest.h>
#include <stdbool.h>
extern "C" {
#include "../src/list.h"
#include "../src/m3u8.h"
}

// ----------- m3u8_list_init -----------
TEST(m3u8_list_test, given_a_list_set_next_prev_as_head) {
  m3u8_list_node_t* list = nullptr;

  EXPECT_EQ(m3u8_list_init(&list), M3U8_LIST_STATUS_NO_ERROR);
  ASSERT_NE(list, nullptr);

  EXPECT_EQ(list->next, list);
  EXPECT_EQ(list->prev, list);

  free(list);
}

TEST(m3u8_list_test, null_pointer_returns_invalid_args) {
  EXPECT_EQ(m3u8_list_init(nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_add_head -----------
TEST(m3u8_list_add_head_test, given_valid_head_and_node_adds_node_after_head) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));

  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_head(head, node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head->next, node);
  EXPECT_EQ(node->prev, head);
  EXPECT_EQ(node->next, head);
  EXPECT_EQ(head->prev, node);

  free(node);
  free(head);
}

TEST(m3u8_list_add_head_test, null_head_returns_invalid_args) {
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_add_head(nullptr, node), M3U8_LIST_STATUS_INVALID_ARGS);
  free(node);
}

TEST(m3u8_list_add_head_test, null_node_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_head(head, nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
  free(head);
}

// ----------- m3u8_list_add_tail -----------
TEST(m3u8_list_add_tail_test, given_valid_new_and_head_adds_node_before_head) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_tail(node, head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head->prev, node);
  EXPECT_EQ(node->next, head);
  EXPECT_EQ(node->prev, head);
  EXPECT_EQ(head->next, node);

  free(node);
  free(head);
}

TEST(m3u8_list_add_tail_test, null_new_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_tail(nullptr, head), M3U8_LIST_STATUS_INVALID_ARGS);
  free(head);
}

TEST(m3u8_list_add_tail_test, null_head_returns_invalid_args) {
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_add_tail(node, nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
  free(node);
}

// ----------- m3u8_list_remove -----------
TEST(m3u8_list_remove_test, given_valid_node_removes_node_from_list) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_head(head, node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(m3u8_list_remove(node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head->next, head);
  EXPECT_EQ(head->prev, head);
  EXPECT_EQ(node->next, nullptr);
  EXPECT_EQ(node->prev, nullptr);

  free(node);
  free(head);
}

TEST(m3u8_list_remove_test, null_node_returns_invalid_args) {
  EXPECT_EQ(m3u8_list_remove(nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_empty -----------
TEST(m3u8_list_empty_test, given_empty_list_returns_true) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  bool is_empty = false;
  EXPECT_EQ(m3u8_list_empty(head, &is_empty), M3U8_STATUS_NO_ERROR);
  EXPECT_TRUE(is_empty);

  free(head);
}

TEST(m3u8_list_empty_test, given_non_empty_list_returns_false) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_head(head, node), M3U8_LIST_STATUS_NO_ERROR);

  bool is_empty = true;
  EXPECT_EQ(m3u8_list_empty(head, &is_empty), M3U8_STATUS_NO_ERROR);
  EXPECT_FALSE(is_empty);

  free(node);
  free(head);
}

TEST(m3u8_list_empty_test, null_head_returns_invalid_args) {
  bool is_empty = false;
  EXPECT_EQ(m3u8_list_empty(nullptr, &is_empty), M3U8_LIST_STATUS_INVALID_ARGS);
}

TEST(m3u8_list_empty_test, null_output_pointer_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_empty(head, nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
  free(head);
}

// ----------- m3u8_list_count -----------
TEST(m3u8_list_count_test, given_empty_list_returns_zero) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  int count = -1;
  EXPECT_EQ(m3u8_list_count(head, &count), M3U8_STATUS_NO_ERROR);
  EXPECT_EQ(count, 0);

  free(head);
}

TEST(m3u8_list_count_test, given_non_empty_list_returns_count) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  m3u8_list_node_t* node1 = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  m3u8_list_node_t* node2 = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));

  EXPECT_EQ(m3u8_list_add_head(head, node1), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_add_head(head, node2), M3U8_LIST_STATUS_NO_ERROR);

  int count = -1;
  EXPECT_EQ(m3u8_list_count(head, &count), M3U8_STATUS_NO_ERROR);
  EXPECT_EQ(count, 2);

  free(node1);
  free(node2);
  free(head);
}

TEST(m3u8_list_count_test, null_head_returns_invalid_args) {
  int count = -1;
  EXPECT_EQ(m3u8_list_count(nullptr, &count), M3U8_LIST_STATUS_INVALID_ARGS);
}

TEST(m3u8_list_count_test, null_output_pointer_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_count(head, nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
  free(head);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
