#ifndef __M3U8_TESTS_LIST_MOCK_HH__
#define __M3U8_TESTS_LIST_MOCK_HH__

#include <functional>

extern "C" {
#include "../../src/list.h"
}

extern std::function<int(m3u8_list_node_t*)> m3u8_list_init_mock;

extern std::function<int(m3u8_list_node_t*, m3u8_list_node_t*)>
  m3u8_list_ina_mock;

extern std::function<int(m3u8_list_node_t*, m3u8_list_node_t*)>
  m3u8_list_inb_mock;

extern std::function<int(m3u8_list_node_t*)> m3u8_list_remove_mock;

extern std::function<int(const m3u8_list_node_t*, bool*)>
  m3u8_list_is_empty_mock;

extern std::function<int(const m3u8_list_node_t*, int*)> m3u8_list_count_mock;

extern int (*impl_m3u8_list_init)(m3u8_list_node_t*);
extern int (*impl_m3u8_list_ina)(m3u8_list_node_t*, m3u8_list_node_t*);
extern int (*impl_m3u8_list_inb)(m3u8_list_node_t*, m3u8_list_node_t*);
extern int (*impl_m3u8_list_remove)(m3u8_list_node_t*);
extern int (*impl_m3u8_list_is_empty)(const m3u8_list_node_t*, bool*);
extern int (*impl_m3u8_list_count)(const m3u8_list_node_t*, int*);

#endif  // __M3U8_TESTS_LIST_MOCK_HH__
