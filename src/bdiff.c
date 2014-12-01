#include "../include/bdiff.h"
#include "chunk.h"
#include "block.h"
#include "hunk.h"
#include <stdio.h>

hunk * const bdiff(data_fetcher const df, void * const a, void * const b) {
    chunks a_chunks = split_data(df, a);
    chunks b_chunks = split_data(df, b);
    blocks unique_a = unique_blocks(a_chunks, b_chunks);
    blocks unique_b = unique_blocks(b_chunks, a_chunks);
    chunk_free(a_chunks);
    chunk_free(b_chunks);
    hunk * const h = pair_blocks(unique_a, unique_b);
    block_free(unique_a);
    block_free(unique_b);
    return h;
}

typedef struct {
    char * data;
    unsigned length;
    unsigned pos;
} test_data;

unsigned test_reader(void * source, unsigned n_items, char * buffer) {
    test_data * td = source;
    unsigned stop = td->pos + n_items;
    if (stop > td->length) {
        stop = td->length;
    }
    stop -= td->pos;
    for (unsigned i=0; i < stop; i++) {
        buffer[i] = td->data[i + td->pos];
    }
    td->pos += stop;
    return stop;
}

int main() {
    test_data a = {.data = "foobarbaz", .length = 9};
    test_data b = {.data = "foobarbam", .length = 9};
    hunk * const h = bdiff(test_reader, (void *) &a, (void *) &b);
    printf("bs: %u be: %u\n", h->b.start, h->b.end);
    hunk_free(h);
}
