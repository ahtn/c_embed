// Copyright 2018 jem@seethis.link
// Licensed under the MIT license (http://opensource.org/licenses/MIT)

#include "ring_buf.h"

#include <stdio.h>

// Ring buffer with int elements
typedef uint8_t my_int_type;
DEFINE_RING_BUF_VARIANT(2047, uint16_t, my_int_type, ring_buf2048);

// Ring buffer with struct elements
typedef struct point_type {
    uint8_t x;
    uint8_t y;
    uint8_t z;
} point_type;

DEFINE_RING_BUF_VARIANT(7, uint8_t, point_type, point_buf);

int main(int argc, char *argv[]) {
    {
        ring_buf2048_type int_buf;
        ring_buf2048_clear(&int_buf);

        if (ring_buf2048_has_data(&int_buf)) {
            printf("has data\n");
        } else {
            printf("is empty\n");
        }

        // Fill the buffer with numbers
        uint8_t count = 0;
        while(ring_buf2048_space(&int_buf) != 0) {
            ring_buf2048_put(&int_buf, ('0' + count % 10));
            count++;
        }

        // Skip lots of number
        if (ring_buf2048_len(&int_buf) > 2040) {
            ring_buf2048_skip(&int_buf, 2040);
        }

        // Add some letters
        my_int_type test_fill[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
        uint8_t test_fill_len = sizeof(test_fill) / sizeof(test_fill[0]);
        if (test_fill_len < ring_buf2048_space(&int_buf) ) {
            ring_buf2048_fill(&int_buf, test_fill, test_fill_len);
        }

        const uint8_t copy_buf_len = 7;
        my_int_type copy_buf[copy_buf_len];
        if (ring_buf2048_len(&int_buf) > copy_buf_len) {
            ring_buf2048_take(&int_buf, copy_buf, copy_buf_len);
            for (int i = 0; i < copy_buf_len; ++i) {
                printf("copy_buf[%d]: '%c'\n", i, (uint8_t)copy_buf[i]);
            }
        }

        // Print what's left
        while(ring_buf2048_has_data(&int_buf)) {
            printf("got: '%c'\n", (uint8_t)ring_buf2048_get(&int_buf));
        }
    }

    // Test the point buffer
    {
        point_buf_type point_buf;
        point_buf_clear(&point_buf);

        uint8_t counter;
        while(point_buf_space(&point_buf) != 0) {
            const point_type test_point = { counter++, counter++, counter++ };
            point_buf_put(&point_buf, test_point);
        }

        // Print what's in the ring buffer
        while(point_buf_has_data(&point_buf)) {
            const point_type point = point_buf_get(&point_buf);
            printf("got: Point{%d, %d, %d}\n", point.x, point.y, point.z);
        }
    }

    return 0;
}
