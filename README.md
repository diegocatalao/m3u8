# M3U8

M3U8 is a C library for parsing, generating, and managing M3U8 playlists.
Designed for performance and strict adherence to both the current M3U8 RFC 
standards and the Apple HLS specification.

## Features

- *Compliant Generation*: Creates M3U8 playlists that adhere to both the latest 
M3U8 RFCs and Apple's HLS specification.
- *Clear Data Structures*: Provides intuitive C structures for representing M3U8 
tags and attributes.
- *Dual Standard Adherence*: Developed to comply with both the current M3U8 RFC 
(e.g., RFC 8216) and the Apple M3U8 specification and associated licensing 
terms.

## Installation

1. clone this repository:
```bash
$ git clone https://github.com/diegocatalao/m3u8.git
$ cd m3u8
```

2. create and navigate to the build directory, then run make:
```bash
$ mkdir -p build
$ cmake .. && make
$ sudo make install
```

3. (optional) build and run all unittests
```bash
$ mkdir -p build
$ cmake -DCOMPILE_TESTS=ON .. && make
$ sudo make install
$ ctest --extra-verbose --output-on-failure
```

## Documentation

You can generate the doxygen documentation following this steps:

1. install the Doxygen from [official website][doxygen_documentation];

2. generate the documentation from source:

```bash
$ doxygen Doxyfile
```

## Releases

Stay up-to-date with the latest improvements and features! We publish new 
versions of M3U8 regularly.

For a detailed history of changes, new features, and bug fixes for each 
version, please refer to our [CHANGELOG.md][changelog] file.
You can also find all released versions and their associated assets on 
our [GitHub Releases page][github_releases].

## Basic usage example

```C
#include <m3u8.h>
#include <stdio.h>

int main() {
  const char* playlist_string =
    "#EXTM3U\n"
    "#EXT-X-VERSION:3\n"
    "#EXT-X-TARGETDURATION:10\n"
    "#EXTINF:9.009,\n"
    "http://example.com/movie/segment0.ts\n"
    "#EXTINF:9.009,\n"
    "http://example.com/movie/segment1.ts\n";

  m3u8_t *playlist_ptr = NULL;
  
  if (m3u8_create_playlist(&playlist_ptr) != M3U8_STATUS_NO_ERROR) {
    fprintf(stderr, "Unable to allocated playlist.\n");
    return -1;
  }

  if (m3u8_parse_from_str(playlist_string) != M3U8_STATUS_NO_ERROR) {
    m3u8_free_playlist(playlist_ptr);
    fprintf(stderr, "Unable to parse playlist.\n");
    return -1;
  }

  printf("M3U8 Version: %d\n", playlist->version);
  printf("Target Duration: %d\n", playlist->target_duration);

  for (size_t i = 0; i < playlist->segments->count; ++i) {
    segment = (m3u8_media_segment_t*)playlist->segments->items[i];
    printf("URL: %s, Duration: %.3f\n", segment->uri, segment->duration);
  }

  m3u8_free_playlist(playlist);

  return 0;
}
```

## Licensing

**M3U8** adheres to both the current M3U8 RFC standards (e.g., RFC 8216) and the 
**Apple M3U8 License** and specification. This ensures broad compatibility and 
compliance with both open standards and Apple's proprietary extensions to the 
HLS standard. Refer to the [IETF RFC 8216][ietf_rfc_8216] and the 
[Apple HLS Authoring Specifications][apple_hls_spec] for Apple Devices for 
full terms.

## Contributing

We warmly welcome contributions to this library. Your involvement is invaluable 
in enhancing this library and ensuring its continued evolving M3U8 standards. 
To maintain a high quality and consistent development flow, please review the 
guidelines below before initiating any contributions.

[doxygen_documentation]: https://www.doxygen.nl/download.html#google_vignette "Dogygen documentation"
[changelog]: CHANGELOG.md "M3U8.c Changelog"
[github_releases]: https://github.com/seuusuario/m3u8.c/releases "M3U8.c GitHub Releases"
[apple_hls_spec]: https://developer.apple.com/documentation/http_live_streaming/hls_authoring_specification_for_apple_devices "Apple HLS Authoring Specs"
[ietf_rfc_8216]: https://datatracker.ietf.org/doc/html/rfc8216 "IETF RFC 8216 (M3U8)"
