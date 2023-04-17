#include <gcf/internal/io.h>

gcf_result read_data(
    gcf_read_ctx * const restrict ctx,
    uint32_t const data_size,
    void * const out_data
) {
    const uint32_t bytes_read = ctx->read_clbk(out_data, data_size, ctx->user_data);

    if(bytes_read != data_size) {
        return GCF_RESULT_READ_ERROR;
    }

    return GCF_RESULT_SUCCESS;
}
