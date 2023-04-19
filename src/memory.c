/*
Copyright © 2021-2023 Alfredo Mungo <alfredo.mungo@protonmail.ch>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <gcf/gcf.h>

void * memory_alloc(gcf_read_ctx * const ctx, size_t const size) {
    return ctx->alloc_clbk(size, ctx->user_data);
}

void memory_free(gcf_read_ctx * const ctx, void * const address) {
    ctx->free_clbk(address, ctx->user_data);
}

size_t align_size(size_t const orig_size, size_t const alignment) {
    size_t const mask = alignment - 1;

    return (orig_size + mask) & ~mask;
}
