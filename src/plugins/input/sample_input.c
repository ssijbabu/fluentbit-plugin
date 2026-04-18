/* Sample Input Plugin for Fluent Bit
 * This plugin demonstrates how to create a basic input plugin
 */

#include <fluent-bit/flb_input.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLUGIN_NAME "sample_input"
#define DEFAULT_INTERVAL_SEC 5

/* Plugin context */
struct plugin_context {
    int counter;
    int interval_sec;
};

/* Initialize plugin */
static int init(struct flb_input_instance *in,
                struct flb_config *config, void *data)
{
    struct plugin_context *ctx;
    const char *interval;

    /* Allocate space for the context */
    ctx = malloc(sizeof(struct plugin_context));
    if (!ctx) {
        flb_errno();
        return -1;
    }

    /* Get configuration parameters */
    interval = flb_input_get_property("interval_sec", in);
    if (interval) {
        ctx->interval_sec = atoi(interval);
    } else {
        ctx->interval_sec = DEFAULT_INTERVAL_SEC;
    }

    ctx->counter = 0;

    /* Set the context */
    flb_input_set_context(in, ctx);

    /* Schedule the callback */
    return flb_input_set_collector_time(in, cb_collect, 0,
                                       ctx->interval_sec * 1000, config);
}

/* Callback to collect data */
static int cb_collect(struct flb_input_instance *in,
                      struct flb_config *config, void *data)
{
    struct plugin_context *ctx = data;
    struct flb_time tms;
    msgpack_packer mp_pck;
    msgpack_sbuffer mp_sbuf;
    char msg[256];

    (void) in;
    (void) config;

    /* Create message */
    snprintf(msg, sizeof(msg), "Sample data from input plugin - counter: %d",
             ctx->counter++);

    /* Init buffer */
    msgpack_sbuffer_init(&mp_sbuf);
    msgpack_packer_init(&mp_pck, &mp_sbuf, msgpack_sbuffer_write);

    /* Get timestamp */
    flb_time_get(&tms);

    /* Pack message */
    msgpack_pack_array(&mp_pck, 2);
    flb_time_append(&tms, &mp_pck);

    msgpack_pack_map(&mp_pck, 1);
    msgpack_pack_bin(&mp_pck, 3);
    msgpack_pack_bin_body(&mp_pck, "msg", 3);
    msgpack_pack_bin(&mp_pck, strlen(msg));
    msgpack_pack_bin_body(&mp_pck, msg, strlen(msg));

    /* Send data */
    flb_input_push(in, NULL, 0, mp_sbuf.data, mp_sbuf.size);

    /* Cleanup */
    msgpack_sbuffer_destroy(&mp_sbuf);

    return 0;
}

/* Cleanup */
static int cb_exit(void *data)
{
    struct plugin_context *ctx = data;
    if (ctx) {
        free(ctx);
    }
    return 0;
}

/* Plugin definition */
struct flb_input_plugin input_plugin = {
    .name         = PLUGIN_NAME,
    .description  = "Sample input plugin",
    .cb_init      = init,
    .cb_pre_run   = NULL,
    .cb_collect   = cb_collect,
    .cb_resume    = NULL,
    .cb_pause     = NULL,
    .cb_exit      = cb_exit,
    .flags        = 0
};
