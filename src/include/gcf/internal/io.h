#ifndef GCF_IO_H
#define GCF_IO_H

#include <gcf/gcf.h>

gcf_result read_data(
    gcf_read_ctx * const GCF_RESTRICT ctx,
    uint32_t const data_size,
    void * const out_data
);

#endif /* !GCF_IO_H */
