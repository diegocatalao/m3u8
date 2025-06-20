#include "mock_list.hh"

#include <dlfcn.h>
#include <stdio.h>

int (*impl_m3u8_list_init)(m3u8_list_node_t*);

int (*impl_m3u8_list_ina)(m3u8_list_node_t*, m3u8_list_node_t*);

int (*impl_m3u8_list_inb)(m3u8_list_node_t*, m3u8_list_node_t*);

int (*impl_m3u8_list_remove)(m3u8_list_node_t*);

int (*impl_m3u8_list_is_empty)(const m3u8_list_node_t*, bool*);

int (*impl_m3u8_list_count)(const m3u8_list_node_t*, int*);

static void __attribute__((constructor)) mock_list_impls(void) {
  std::unordered_map<std::string, void**> methods;

  methods["m3u8_list_init"] = (void**)(&impl_m3u8_list_init);
  methods["m3u8_list_ina"] = (void**)(&impl_m3u8_list_ina);
  methods["m3u8_list_inb"] = (void**)(&impl_m3u8_list_inb);
  methods["m3u8_list_remove"] = (void**)(&impl_m3u8_list_remove);
  methods["m3u8_list_is_empty"] = (void**)(&impl_m3u8_list_is_empty);
  methods["m3u8_list_count"] = (void**)(&impl_m3u8_list_count);

  for (const auto& [name, method_ptr] : methods) {
    *method_ptr = dlsym(RTLD_NEXT, name.c_str());
  }
}

std::function<int(m3u8_list_node_t*)> m3u8_list_init_mock;

std::function<int(m3u8_list_node_t*)> m3u8_list_remove_mock;

std::function<int(m3u8_list_node_t*, m3u8_list_node_t*)> m3u8_list_ina_mock;

std::function<int(m3u8_list_node_t*, m3u8_list_node_t*)> m3u8_list_inb_mock;

std::function<int(const m3u8_list_node_t*, bool*)> m3u8_list_is_empty_mock;

std::function<int(const m3u8_list_node_t*, int*)> m3u8_list_count_mock;

extern "C" int m3u8_list_init(m3u8_list_node_t* head) {
  return m3u8_list_init_mock ? m3u8_list_init_mock(head)
                             : impl_m3u8_list_init(head);
}

extern "C" int m3u8_list_ina(m3u8_list_node_t* head, m3u8_list_node_t* node) {
  return m3u8_list_ina_mock ? m3u8_list_ina_mock(head, node)
                            : impl_m3u8_list_ina(head, node);
}

extern "C" int m3u8_list_inb(m3u8_list_node_t* head, m3u8_list_node_t* node) {
  return m3u8_list_inb_mock ? m3u8_list_inb_mock(head, node)
                            : impl_m3u8_list_inb(head, node);
}

extern "C" int m3u8_list_remove(m3u8_list_node_t* node) {
  return m3u8_list_remove_mock ? m3u8_list_remove_mock(node)
                               : impl_m3u8_list_remove(node);
}

extern "C" int m3u8_list_is_empty(const m3u8_list_node_t* head,
                                  bool*                   is_empty) {
  return m3u8_list_is_empty_mock ? m3u8_list_is_empty_mock(head, is_empty)
                                 : impl_m3u8_list_is_empty(head, is_empty);
}

extern "C" int m3u8_list_count(const m3u8_list_node_t* head, int* size) {
  return m3u8_list_count_mock ? m3u8_list_count_mock(head, size)
                              : impl_m3u8_list_count(head, size);
}
