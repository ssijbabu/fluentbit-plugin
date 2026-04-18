/* Sample Output Plugin for Fluent Bit
 * This plugin demonstrates how to create a basic output plugin
 */

#include <fluent-bit/flb_output.h>
#include <fluent-bit/flb_utils.h>
#include <fluent-bit/flb_pack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLUGIN_NAME "sample_output"

/* Plugin context */
struct plugin_context {
    int records_sent;
};

/* Initialize plugin */
static int init(struct flb_output_instance *out,
                struct flb_config *config,
                void *data)
{
    struct plugin_context *ctx;

    (void) config;
    (void) data;

    /* Allocate space for the context */
    ctx = malloc(sizeof(struct plugin_context));
    if (!ctx) {
        return -1;
    }

    ctx->records_sent = 0;

    /* Set the context */
    flb_output_set_context(out, ctx);

    return 0;
}

/* Flush callback */
static int cb_flush(struct flb_event_chunk *event_chunk,
                   struct flb_output_flush_context *fctx,
                   struct flb_config *config,
                   void *data)
{
    struct plugin_context *ctx = data;
    int ret;

    (void) config;

    /* Process the event chunk */
    ret = flb_event_chunk_process_chunked(event_chunk,
                                         (void *)ctx);

    if (ret >= 0) {
        ctx->records_sent++;
        return FLB_OK;
    }

    return FLB_ERROR;
}

/* Cleanup */
static int cb_exit(void *data)
{
    struct plugin_context *ctx = data;

    if (ctx) {
        fprintf(stderr, "[%s] Total records sent: %d\n",
                PLUGIN_NAME, ctx->records_sent);
        free(ctx);
    }

    return 0;
}

/* Plugin definition */
struct flb_output_plugin output_plugin = {
    .name           = PLUGIN_NAME,
    .description    = "Sample output plugin",
    .cb_init        = init,
    .cb_pre_run     = NULL,
    .cb_flush       = cb_flush,
    .cb_exit        = cb_exit,
    .config_map     = NULL,
    .workers        = 1,
    .flags          = FLB_OUTPUT_NET
};
