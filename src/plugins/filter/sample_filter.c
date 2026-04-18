/* Sample Filter Plugin for Fluent Bit
 * This plugin demonstrates how to create a basic filter plugin
 */

#include <fluent-bit/flb_filter.h>
#include <fluent-bit/flb_str.h>
#include <fluent-bit/flb_pack.h>
#include <msgpack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLUGIN_NAME "sample_filter"

/* Plugin context */
struct plugin_context {
    int records_modified;
    char *add_field_name;
    char *add_field_value;
};

/* Initialize plugin */
static int init(struct flb_filter_instance *f_ins,
                struct flb_config *config,
                void *data)
{
    struct plugin_context *ctx;
    const char *field_name;
    const char *field_value;

    (void) config;
    (void) data;

    /* Allocate space for the context */
    ctx = malloc(sizeof(struct plugin_context));
    if (!ctx) {
        return -1;
    }

    /* Get configuration parameters */
    field_name = flb_filter_get_property("add_field_name", f_ins);
    field_value = flb_filter_get_property("add_field_value", f_ins);

    ctx->add_field_name = field_name ? strdup(field_name) : strdup("added_field");
    ctx->add_field_value = field_value ? strdup(field_value) : strdup("added_value");
    ctx->records_modified = 0;

    /* Set the context */
    flb_filter_set_context(f_ins, ctx);

    return 0;
}

/* Filter callback */
static int cb_filter(const void *data, size_t bytes,
                    const char *tag, int tag_len,
                    void **out_data, size_t *out_size,
                    struct flb_filter_instance *f_ins,
                    struct flb_input_instance *i_ins,
                    void *filter_context,
                    struct flb_config *config)
{
    struct plugin_context *ctx = filter_context;
    msgpack_unpacker unp;
    msgpack_object root;
    msgpack_packer mp_pck;
    msgpack_sbuffer mp_sbuf;
    int ret;
    int record_count = 0;

    (void) f_ins;
    (void) i_ins;
    (void) config;

    /* Initialize output buffer */
    msgpack_sbuffer_init(&mp_sbuf);
    msgpack_packer_init(&mp_pck, &mp_sbuf, msgpack_sbuffer_write);

    /* Initialize unpacker */
    msgpack_unpacker_init(&unp, 8192);

    /* Unpack and process data */
    ret = msgpack_unpacker_reserve_buffer(&unp, bytes);
    if (ret != MSGPACK_UNPACK_SUCCESS) {
        msgpack_sbuffer_destroy(&mp_sbuf);
        return FLB_FILTER_NOOP;
    }

    memcpy(msgpack_unpacker_buffer(&unp), data, bytes);
    msgpack_unpacker_buffer_consumed(&unp, bytes);

    while (msgpack_unpacker_next(&unp, &root) == MSGPACK_UNPACK_SUCCESS) {
        if (root.type != MSGPACK_OBJECT_ARRAY || root.via.array.size != 2) {
            continue;
        }

        /* Pack timestamp */
        msgpack_pack_array(&mp_pck, 2);
        msgpack_object_pack(&mp_pck, root.via.array.ptr[0]);

        /* Pack modified record */
        msgpack_object record = root.via.array.ptr[1];

        if (record.type == MSGPACK_OBJECT_MAP) {
            /* Pack map with extra field */
            msgpack_pack_map(&mp_pck, record.via.map.size + 1);

            /* Pack existing fields */
            int i;
            for (i = 0; i < record.via.map.size; i++) {
                msgpack_object_pack(&mp_pck, record.via.map.ptr[i].key);
                msgpack_object_pack(&mp_pck, record.via.map.ptr[i].val);
            }

            /* Pack added field */
            msgpack_pack_bin(&mp_pck, strlen(ctx->add_field_name));
            msgpack_pack_bin_body(&mp_pck, ctx->add_field_name,
                                 strlen(ctx->add_field_name));
            msgpack_pack_bin(&mp_pck, strlen(ctx->add_field_value));
            msgpack_pack_bin_body(&mp_pck, ctx->add_field_value,
                                 strlen(ctx->add_field_value));
        } else {
            msgpack_object_pack(&mp_pck, record);
        }

        ctx->records_modified++;
        record_count++;
    }

    msgpack_unpacker_destroy(&unp);

    if (record_count == 0) {
        msgpack_sbuffer_destroy(&mp_sbuf);
        return FLB_FILTER_NOOP;
    }

    /* Set output */
    *out_data = mp_sbuf.data;
    *out_size = mp_sbuf.size;

    return FLB_FILTER_OK;
}

/* Cleanup */
static int cb_exit(void *data)
{
    struct plugin_context *ctx = data;

    if (ctx) {
        fprintf(stderr, "[%s] Total records modified: %d\n",
                PLUGIN_NAME, ctx->records_modified);
        if (ctx->add_field_name) free(ctx->add_field_name);
        if (ctx->add_field_value) free(ctx->add_field_value);
        free(ctx);
    }

    return 0;
}

/* Plugin definition */
struct flb_filter_plugin filter_plugin = {
    .name           = PLUGIN_NAME,
    .description    = "Sample filter plugin",
    .cb_init        = init,
    .cb_filter      = cb_filter,
    .cb_exit        = cb_exit
};
