#include <gtest/gtest.h>
#include <stdbool.h>
#include <stdlib.h>

extern "C" {
#include "../src/list.h"
}

// ----------- m3u8_list_init -----------
TEST(m3u8_list_test, given_a_list_set_next_prev_as_head) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);
}

TEST(m3u8_list_test, null_pointer_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  EXPECT_EQ(m3u8_list_init(nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_insert_after -----------

TEST(m3u8_list_insert_after, insert_node) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  m3u8_list_node_t node;
  memset(&node, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_after(&head, &node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node);
  EXPECT_EQ(head.prev, &node);
  EXPECT_EQ(node.next, &head);
  EXPECT_EQ(node.prev, &head);
}

TEST(m3u8_list_insert_after, inserts_multiple_nodes) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  m3u8_list_node_t node1;
  memset(&node1, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_after(&head, &node1), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node1);
  EXPECT_EQ(head.prev, &node1);
  EXPECT_EQ(node1.next, &head);
  EXPECT_EQ(node1.prev, &head);

  m3u8_list_node_t node2;
  memset(&node2, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_after(&head, &node2), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node2);
  EXPECT_EQ(head.prev, &node1);
  EXPECT_EQ(node2.next, &node1);
  EXPECT_EQ(node2.prev, &head);
}

TEST(m3u8_list_insert_after, null_node_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = nullptr;

  // when head and node are null
  EXPECT_EQ(m3u8_list_insert_after(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  // when head is null
  node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_after(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  // when node is null
  free(node);
  node = nullptr;
  head = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_after(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  free(head);
}

// ----------- m3u8_list_insert_before -----------

TEST(m3u8_list_insert_before, insert_node) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  m3u8_list_node_t node;
  memset(&node, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node);
  EXPECT_EQ(head.prev, &node);
  EXPECT_EQ(node.next, &head);
  EXPECT_EQ(node.prev, &head);
}

TEST(m3u8_list_insert_before, inserts_multiple_nodes) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  m3u8_list_node_t node1;
  memset(&node1, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node1), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node1);
  EXPECT_EQ(head.prev, &node1);
  EXPECT_EQ(node1.next, &head);
  EXPECT_EQ(node1.prev, &head);

  m3u8_list_node_t node2;
  memset(&node2, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node2), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node1);
  EXPECT_EQ(head.prev, &node2);
  EXPECT_EQ(node2.next, &head);
  EXPECT_EQ(node2.prev, &node1);
}

TEST(m3u8_list_insert_before, null_node_returns_invalid_args) {
  m3u8_list_node_t* head = nullptr;
  m3u8_list_node_t* node = nullptr;

  // when head and node are null
  EXPECT_EQ(m3u8_list_insert_before(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  // when head is null
  node = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  // when node is null
  free(node);
  node = nullptr;
  head = (m3u8_list_node_t*)malloc(sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(head, node), M3U8_LIST_STATUS_INVALID_ARGS);

  free(head);
}

// ----------- m3u8_list_remove -----------
TEST(m3u8_list_remove_test, given_valid_node_removes_node_from_list) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  m3u8_list_node_t node1;
  memset(&node1, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node1), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node1);
  EXPECT_EQ(head.prev, &node1);
  EXPECT_EQ(node1.next, &head);
  EXPECT_EQ(node1.prev, &head);

  m3u8_list_node_t node2;
  memset(&node2, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node2), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node1);
  EXPECT_EQ(head.prev, &node2);
  EXPECT_EQ(node2.next, &head);
  EXPECT_EQ(node2.prev, &node1);

  // remove node 1
  EXPECT_EQ(m3u8_list_remove(&node1), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node2);
  EXPECT_EQ(head.prev, &node2);
  EXPECT_EQ(node2.next, &head);
  EXPECT_EQ(node2.prev, &head);

  // check if node 2 is linked
  EXPECT_EQ(node1.next, nullptr);
  EXPECT_EQ(node1.prev, nullptr);
}

TEST(m3u8_list_remove_test, null_node_returns_invalid_args) {
  m3u8_list_node_t* node = nullptr;
  EXPECT_EQ(m3u8_list_remove(node), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_empty -----------
TEST(m3u8_list_empty_test, given_empty_list_returns_true) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  bool empty = false;

  EXPECT_EQ(m3u8_list_is_empty(&head, &empty), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_TRUE(empty);
}

TEST(m3u8_list_empty_test, given_non_empty_list_returns_false) {
  m3u8_list_node_t head;
  m3u8_list_node_t node;

  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  bool empty = false;

  EXPECT_EQ(m3u8_list_is_empty(&head, &empty), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_TRUE(empty);

  EXPECT_EQ(m3u8_list_insert_after(&head, &node), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(m3u8_list_is_empty(&head, &empty), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_FALSE(empty);
}

TEST(m3u8_list_empty_test, null_head_returns_invalid_args) {
  bool              empty = false;
  m3u8_list_node_t* head = nullptr;

  EXPECT_EQ(m3u8_list_is_empty(head, &empty), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_count -----------
TEST(m3u8_list_count_test, given_empty_list_returns_zero) {
  int              size = 0;
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  EXPECT_EQ(m3u8_list_count(&head, &size), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);
}

TEST(m3u8_list_count_test, given_non_empty_list_returns_count) {
  int              size = 0;
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_init(&head), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &head);
  EXPECT_EQ(head.prev, &head);

  EXPECT_EQ(m3u8_list_count(&head, &size), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(size, 0);

  m3u8_list_node_t node;
  memset(&node, 0, sizeof(m3u8_list_node_t));
  EXPECT_EQ(m3u8_list_insert_before(&head, &node), M3U8_LIST_STATUS_NO_ERROR);

  EXPECT_EQ(head.next, &node);
  EXPECT_EQ(head.prev, &node);
  EXPECT_EQ(node.next, &head);
  EXPECT_EQ(node.prev, &head);

  EXPECT_EQ(m3u8_list_count(&head, &size), M3U8_LIST_STATUS_NO_ERROR);
  EXPECT_EQ(size, 1);
}

TEST(m3u8_list_count_test, null_head_returns_invalid_args) {
  int              size = 0;
  m3u8_list_node_t head;

  EXPECT_EQ(m3u8_list_count(&head, &size), M3U8_LIST_STATUS_INVALID_ARGS);
  EXPECT_EQ(size, 0);
}

TEST(m3u8_list_count_test, null_output_pointer_returns_invalid_args) {
  m3u8_list_node_t head;
  EXPECT_EQ(m3u8_list_count(&head, nullptr), M3U8_LIST_STATUS_INVALID_ARGS);
}

// ----------- m3u8_list_container_of -----------

typedef struct {
  int    int_value;
  double double_value;
  char*  string_value;
  // the embedded list struct pointer
  m3u8_list_node_t list;
} generic_struct_t;

TEST(m3u8_list_container_of, return_currect_container_pointer) {
  generic_struct_t generic = {1, 0.1, strdup("generic"), nullptr};
  EXPECT_EQ(m3u8_list_init(&generic.list), M3U8_LIST_STATUS_NO_ERROR);

  generic_struct_t* generic_ptr;
  m3u8_list_node_t* head = &generic.list;

  generic_ptr = m3u8_list_container_of(head, generic_struct_t, list);

  EXPECT_EQ(generic_ptr, &generic);
  EXPECT_EQ(generic_ptr->int_value, 1);
  EXPECT_EQ(generic_ptr->double_value, 0.1);
  EXPECT_STREQ(generic_ptr->string_value, "generic");
}

TEST(m3u8_list_foreach, iterates_over_list_from_container) {
  generic_struct_t generics[] = {
    {0, 0.0, NULL},  // SENTINEL
    {1, 1.0, strdup("linux")},
    {2, 2.0, strdup("is")},
    {3, 3.0, strdup("the")},
    {4, 4.0, strdup("best")},
    {5, 5.0, strdup("SO")},
  };

  EXPECT_EQ(m3u8_list_init(&generics[0].list), M3U8_LIST_STATUS_NO_ERROR);

  for (int i = 1; i < sizeof(generics) / sizeof(generics[0]); i++) {
    int status = m3u8_list_insert_before(&generics[0].list, &generics[i].list);
    EXPECT_EQ(status, M3U8_LIST_STATUS_NO_ERROR);
  }

  generic_struct_t* generic;
  int               index = 1;

  m3u8_list_foreach(generic, &generics[0].list, generic_struct_t, list) {
    EXPECT_EQ(generic->int_value, generics[index].int_value);
    EXPECT_EQ(generic->double_value, generics[index].double_value);
    EXPECT_EQ(generic->string_value, generics[index].string_value);
    index++;
  }
}

TEST(m3u8_list_next, given_an_list_goto_next) {
  generic_struct_t generics[] = {
    {0, 0.0, nullptr},  // SENTINEL
    {1, 1.0, strdup("linux")}, {2, 2.0, strdup("is")}, {3, 3.0, strdup("the")},
    {4, 4.0, strdup("best")},  {5, 5.0, strdup("SO")},
  };

  EXPECT_EQ(m3u8_list_init(&generics[0].list), M3U8_LIST_STATUS_NO_ERROR);

  for (int i = 1; i < sizeof(generics) / sizeof(generics[0]); i++) {
    int status = m3u8_list_insert_before(&generics[0].list, &generics[i].list);
    EXPECT_EQ(status, M3U8_LIST_STATUS_NO_ERROR);
  }

  generic_struct_t* pivot = &generics[0];

  EXPECT_EQ(pivot->int_value, 0);
  EXPECT_EQ(pivot->double_value, 0.0);
  EXPECT_EQ(pivot->string_value, nullptr);

  pivot = m3u8_list_next(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 1);
  EXPECT_EQ(pivot->double_value, 1.0);
  EXPECT_STREQ(pivot->string_value, "linux");

  pivot = m3u8_list_next(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 2);
  EXPECT_EQ(pivot->double_value, 2.0);
  EXPECT_STREQ(pivot->string_value, "is");

  pivot = m3u8_list_next(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 3);
  EXPECT_EQ(pivot->double_value, 3.0);
  EXPECT_STREQ(pivot->string_value, "the");

  pivot = m3u8_list_next(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 4);
  EXPECT_EQ(pivot->double_value, 4.0);
  EXPECT_STREQ(pivot->string_value, "best");

  pivot = m3u8_list_next(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 5);
  EXPECT_EQ(pivot->double_value, 5.0);
  EXPECT_STREQ(pivot->string_value, "SO");

  for (int i = 1; i < sizeof(generics) / sizeof(generics[0]); i++) {
    free(generics[i].string_value);
  }
}

TEST(m3u8_list_prev, given_a_list_goto_previous) {
  generic_struct_t generics[] = {
    {0, 0.0, nullptr},  // SENTINEL
    {1, 1.0, strdup("linux")}, {2, 2.0, strdup("is")}, {3, 3.0, strdup("the")},
    {4, 4.0, strdup("best")},  {5, 5.0, strdup("SO")},
  };

  EXPECT_EQ(m3u8_list_init(&generics[0].list), M3U8_LIST_STATUS_NO_ERROR);

  for (int i = 1; i < sizeof(generics) / sizeof(generics[0]); i++) {
    int status = m3u8_list_insert_before(&generics[0].list, &generics[i].list);
    EXPECT_EQ(status, M3U8_LIST_STATUS_NO_ERROR);
  }

  generic_struct_t* pivot = &generics[5];
  EXPECT_EQ(pivot->int_value, 5);
  EXPECT_DOUBLE_EQ(pivot->double_value, 5.0);
  EXPECT_STREQ(pivot->string_value, "SO");

  pivot = m3u8_list_prev(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 4);
  EXPECT_DOUBLE_EQ(pivot->double_value, 4.0);
  EXPECT_STREQ(pivot->string_value, "best");

  pivot = m3u8_list_prev(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 3);
  EXPECT_DOUBLE_EQ(pivot->double_value, 3.0);
  EXPECT_STREQ(pivot->string_value, "the");

  pivot = m3u8_list_prev(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 2);
  EXPECT_DOUBLE_EQ(pivot->double_value, 2.0);
  EXPECT_STREQ(pivot->string_value, "is");

  pivot = m3u8_list_prev(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 1);
  EXPECT_DOUBLE_EQ(pivot->double_value, 1.0);
  EXPECT_STREQ(pivot->string_value, "linux");

  pivot = m3u8_list_prev(pivot, generic_struct_t, list);
  EXPECT_EQ(pivot->int_value, 0);
  EXPECT_DOUBLE_EQ(pivot->double_value, 0.0);
  EXPECT_EQ(pivot->string_value, nullptr);

  for (int i = 1; i < sizeof(generics) / sizeof(generics[0]); i++) {
    free(generics[i].string_value);
  }
}

// ----------- main -----------
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
