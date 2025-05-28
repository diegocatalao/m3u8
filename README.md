# M3U8
---
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
$ make
```

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

    m3u8_media_segment_t* segment;
    m3u8_playlist_t* playlist = m3u8_parse_from_string(playlist_string);

    if (playlist) {
        printf("M3U8 Version: %d\n", playlist->version);
        printf("Target Duration: %d\n", playlist->target_duration);

        for (size_t i = 0; i < playlist->segments->count; ++i) {
            segment = (m3u8_media_segment_t*)playlist->segments->items[i];
            printf("URL: %s, Duration: %.3f\n", segment->uri, segment->duration);
        }

        m3u8_free_playlist(playlist);
    } else {
        fprintf(stderr, "Failed to parse M3U8 playlist.\n");
        return -1;
    }

    return 0;
}
```

## Licensing

**M3U8** adheres to both the current M4U8 RFC standards (e.g., RFC 8216) and the 
**Apple M3U8 License** and specification. This ensures broad compatibility and 
compliance with both open standards and Apple's proprietary extensions to the 
HLS standard. Refer to the IETF RFC 8216 and the [Apple HLS Authoring 
Specifications][ietf_rfc_8216] for Apple Devices for full terms.

## Contributing

Contributions are welcome. Please report bugs, suggest features, or submit 
pull requests.


[ietf_rfc_8216][https://developer.apple.com/documentation/http-live-streaming/hls-authoring-specification-for-apple-devices]
