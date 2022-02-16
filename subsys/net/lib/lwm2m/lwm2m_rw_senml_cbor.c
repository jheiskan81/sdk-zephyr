/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_MODULE_NAME net_lwm2m_senml_cbor
#define LOG_LEVEL CONFIG_LWM2M_LOG_LEVEL

#include <logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#include <sys/util.h>

#include <cbor_common.h>
#include <cbor_decode.h>
#include <cbor_encode.h>

#include "lwm2m_object.h"
#include "lwm2m_rw_senml_cbor.h"
#include "lwm2m_rw_plain_text.h"
#include "lwm2m_engine.h"
#include "lwm2m_senml_cbor.h"
#include "lwm2m_util.h"

struct cbor_out_formatter_data {
	/* Data */
	struct lwm2m_senml input;

	/* Storage for basenames and names */
	struct {
		char bnames[CONFIG_LWM2M_RW_SENML_CBOR_RECORDS][sizeof("/65535/65535/")];
		char names[CONFIG_LWM2M_RW_SENML_CBOR_RECORDS][sizeof("65535/65535")];
		size_t bname_sz; /* Basename buff size */
		size_t name_sz; /* Name buff size */
	};
};

struct cbor_in_fmt_data {
	/* Decoded data */
	struct lwm2m_senml output;
	struct record *current;
};

/* Get the current record */
#define GET_CBOR_FD_REC(fd) &((fd)->input.recs[(fd)->input.rec_cnt])
/* Consume the current record */
#define CONSUME_CBOR_FD_REC(fd) &((fd)->input.recs[(fd)->input.rec_cnt++])
/* Get CBOR output formatter data */
#define LWM2M_OFD_CBOR(octx) ((struct cbor_out_formatter_data *)engine_get_out_user_data(octx))

static int put_end(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	size_t len;

	bool ret =
		cbor_encode_lwm2m_senml(CPKT_BUF_W_REGION(out->out_cpkt),
					(struct lwm2m_senml *)&(LWM2M_OFD_CBOR(out)->input), &len);

	if (ret) {
		out->out_cpkt->offset += len;
	}

	return ret ? len : -EINVAL;
}

static int put_name_r(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	/* Get a handle to a structure which is used for tracking the encoding process */
	struct cbor_out_formatter_data *fd = LWM2M_OFD_CBOR(out);
	int len;

	/* Write resource name */
	len = snprintk(fd->names[fd->input.rec_cnt], sizeof("65535"), "%" PRIu16 "", path->res_id);

	if ((len < sizeof("0") - 1) || (len > sizeof("65535") - 1)) {
		__ASSERT_NO_MSG(false);
		return -EINVAL;
	}

	/* Tell CBOR encoder where to find the name */
	struct record *record = GET_CBOR_FD_REC(fd);

	record->n.hndl.value = fd->names[fd->input.rec_cnt];
	record->n.hndl.len = len;
	record->n_prsnt = 1;

	return 0;
}

static int put_begin_oi(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	struct cbor_out_formatter_data *fd = LWM2M_OFD_CBOR(out);

	if (fd->input.rec_cnt >= CONFIG_LWM2M_RW_SENML_CBOR_RECORDS) {
		__ASSERT_PRINT("CONFIG_LWM2M_RW_SENML_CBOR_RECORDS too small");
		return -ENOMEM;
	}

	/* Basename is the path a requester requests */
	int len = lwm2m_path_to_string(fd->bnames[fd->input.rec_cnt], fd->bname_sz, path,
				    LWM2M_PATH_LEVEL_OBJECT_INST);

	if ((len < sizeof("/0/0/") - 1) || (len > sizeof("/65535/65535/") - 1)) {
		__ASSERT_NO_MSG(false);
		return -EINVAL;
	}

	/* Get the first record which will hold the basename */
	struct record *record = GET_CBOR_FD_REC(fd);

	record->bn.hndl.value = fd->bnames[fd->input.rec_cnt];
	record->bn.hndl.len = len;
	record->bn_prsnt = 1;

	/* Does not advance the record count, name next */
	return 0;
}

static int put_begin_r(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	struct cbor_out_formatter_data *fd = LWM2M_OFD_CBOR(out);

	if (fd->input.rec_cnt >= CONFIG_LWM2M_RW_SENML_CBOR_RECORDS) {
		__ASSERT_PRINT("CONFIG_LWM2M_RW_SENML_CBOR_RECORDS too small");
		return -ENOMEM;
	}

	/* Does not advance the record count, waiting for data */
	return put_name_r(out, path);
}

static int put_begin_ri(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	struct cbor_out_formatter_data *fd = LWM2M_OFD_CBOR(out);
	int res_id_len = strlen(fd->names[fd->input.rec_cnt]);
	struct record *record = GET_CBOR_FD_REC(fd);

	/* Append the resource instance name */
	int len = snprintk(&fd->names[fd->input.rec_cnt][res_id_len], sizeof("/65535"),
			   "/%" PRIu16 "", path->res_inst_id);

	if ((len < sizeof("/0") - 1) || (len > sizeof("/65535") - 1)) {
		__ASSERT_NO_MSG(false);
		return -EINVAL;
	}

	/* Length includes res inst ID */
	record->n.hndl.len += len;

	/* Does not advance the record count, waiting for data */
	return 0;
}

static int put_name_nth_ri(struct lwm2m_output_context *out, struct lwm2m_obj_path *path)
{
	struct cbor_out_formatter_data *fd = LWM2M_OFD_CBOR(out);

	if (fd->input.rec_cnt >= CONFIG_LWM2M_RW_SENML_CBOR_RECORDS) {
		__ASSERT_PRINT("CONFIG_LWM2M_RW_SENML_CBOR_RECORDS too small");
		return -ENOMEM;
	}

	/* With the default instance as resource name
	 * and resource instance name are already in place
	 */
	if (path->res_inst_id > 0) {
		int ret = put_name_r(out, path);

		if (ret < 0) {
			return ret;
		}

		ret = put_begin_ri(out, path);
		if (ret < 0) {
			return ret;
		}
	}

	/* Does not advance the record count, waiting for data */
	return 0;
}

static int put_value(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int32_t value)
{
	int ret = put_name_nth_ri(out, path);

	if (ret < 0) {
		return ret;
	}

	struct record *record = CONSUME_CBOR_FD_REC(LWM2M_OFD_CBOR(out));

	/* Write the value */
	record->u.choice = LSB_U_V;
	record->u.v = value;
	record->u_prsnt = 1;

	return 0;
}

static int put_s8(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int8_t value)
{
	return put_value(out, path, value);
}

static int put_s16(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int16_t value)
{
	return put_value(out, path, value);
}

static int put_s32(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int32_t value)
{
	return put_value(out, path, value);
}

static int put_s64(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int64_t value)
{
	/* TODO: remove this check once the CBOR library supports 64-bit values */
	if (value != (value & 0xFFFFFFFF)) {
		LOG_WRN("64-bit values are not supported");
		return -EINVAL;
	}

	return put_value(out, path, (uint32_t)value);
}

static int put_time(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, int64_t value)
{
	int ret = put_name_nth_ri(out, path);

	if (ret < 0) {
		return ret;
	}

	struct record *record = CONSUME_CBOR_FD_REC(LWM2M_OFD_CBOR(out));

	/* Write the value */
	record->u.choice = LSB_U_V;
	record->u.v = value;
	record->u_prsnt = 1;

	return 0;
}

static int put_string(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, char *buf,
		      size_t buflen)
{
	int ret = put_name_nth_ri(out, path);

	if (ret < 0) {
		return ret;
	}

	struct record *record = CONSUME_CBOR_FD_REC(LWM2M_OFD_CBOR(out));

	/* Write the value */
	record->u.choice = LSB_U_VS;
	record->u.vs.value = buf;
	record->u.vs.len = buflen;
	record->u_prsnt = 1;

	return 0;
}

static int put_bool(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, bool value)
{
	int ret = put_name_nth_ri(out, path);

	if (ret < 0) {
		return ret;
	}

	struct record *record = CONSUME_CBOR_FD_REC(LWM2M_OFD_CBOR(out));

	/* Write the value */
	record->u.choice = LSB_U_VB;
	record->u.vb = value;
	record->u_prsnt = 1;

	return 0;
}

static int put_opaque(struct lwm2m_output_context *out, struct lwm2m_obj_path *path, char *buf,
		      size_t buflen)
{
	int ret = put_name_nth_ri(out, path);

	if (ret < 0) {
		return ret;
	}

	struct record *record = CONSUME_CBOR_FD_REC(LWM2M_OFD_CBOR(out));

	/* Write the value */
	record->u.choice = LSB_U_VD;
	record->u.vd.value = buf;
	record->u.vd.len = buflen;
	record->u_prsnt = 1;

	return 0;
}

static int put_objlnk(struct lwm2m_output_context *out, struct lwm2m_obj_path *path,
		      struct lwm2m_objlnk *value)
{
	int32_t value_s32 = (value->obj_id << 16) | value->obj_inst;

	return put_s32(out, path, value_s32);
}

static int get_opaque(struct lwm2m_input_context *in,
			 uint8_t *value, size_t buflen,
			 struct lwm2m_opaque_context *opaque,
			 bool *last_block)
{
	struct cbor_in_fmt_data *fd;

	/* Get the CBOR header only on first read. */
	if (opaque->remaining == 0) {

		fd = engine_get_in_user_data(in);
		if (!fd || !fd->current) {
			return -EINVAL;
		}

		/* TODO: get the opaque data and it's length -
		 * now plain zero
		 */

		opaque->len = fd->current->u.vd.len;
		opaque->remaining = fd->current->u.vd.len;

		fd->current = NULL;
		goto NOT_SUPPORTED;
	}

	return lwm2m_engine_get_opaque_more(in, value, buflen,
					    opaque, last_block);
NOT_SUPPORTED:
	return -ENOTSUP;
}

static int get_s32(struct lwm2m_input_context *in, int32_t *value)
{
	struct cbor_in_fmt_data *fd;

	fd = engine_get_in_user_data(in);
	if (!fd || !fd->current) {
		return -EINVAL;
	}

	*value = fd->current->u.v;
	fd->current = NULL;

	return 0;
}

static int get_s64(struct lwm2m_input_context *in, int64_t *value)
{
	struct cbor_in_fmt_data *fd;

	fd = engine_get_in_user_data(in);
	if (!fd || !fd->current) {
		return -EINVAL;
	}

	*value = fd->current->u.v;
	fd->current = NULL;

	return 0;
}


static int get_string(struct lwm2m_input_context *in, uint8_t *buf, size_t buflen)
{
	struct cbor_in_fmt_data *fd;
	int len;

	fd = engine_get_in_user_data(in);
	if (!fd || !fd->current) {
		return -EINVAL;
	}

	len = MIN(buflen-1, fd->current->u.vs.len);

	memcpy(buf, fd->current->u.vs.value, len);
	buf[len] = '\0';

	fd->current = NULL;

	return 0;
}

static int get_objlnk(struct lwm2m_input_context *in,
			 struct lwm2m_objlnk *value)
{
	char objlnk[sizeof("65535:65535")] = {0};
	unsigned long id;
	int ret;


	ret = get_string(in, objlnk, sizeof(objlnk));
	if (ret < 0) {
		return ret;
	}

	value->obj_id = LWM2M_OBJLNK_MAX_ID;
	value->obj_inst = LWM2M_OBJLNK_MAX_ID;

	char *end;
	char *idp = objlnk;

	for (int idx = 0; idx < 2; idx++) {

		errno = 0;
		id = strtoul(idp, &end, 10);

		idp = end;

		if ((id == 0 && errno == ERANGE) || id > 65535) {
			LOG_WRN("decoded id %lu out of range[0..65535]", id);
			return -EBADMSG;
		}

		switch (idx) {
		case 0:
			value->obj_id = id;
			continue;
		case 1:
			value->obj_inst = id;
			continue;
		}
	}

	if (value->obj_inst != LWM2M_OBJLNK_MAX_ID && (value->obj_id == LWM2M_OBJLNK_MAX_ID)) {
		LOG_WRN("decoded obj inst id without obj id");
		return -EBADMSG;
	}

	return ret;
}

static int get_bool(struct lwm2m_input_context *in, bool *value)
{
	struct cbor_in_fmt_data *fd;

	fd = engine_get_in_user_data(in);
	if (!fd || !fd->current) {
		return -EINVAL;
	}

	*value = fd->current->u.vb;
	fd->current = NULL;

	return 0;
}

static int do_write_op_item(struct lwm2m_message *msg, struct record *rec)
{
	struct lwm2m_engine_obj_inst *obj_inst = NULL;
	struct lwm2m_engine_obj_field *obj_field;
	struct lwm2m_engine_res *res = NULL;
	struct lwm2m_engine_res_inst *res_inst = NULL;
	int ret;
	uint8_t created = 0U;
	struct cbor_in_fmt_data *fd;

	fd = engine_get_in_user_data(&msg->in);
	if (!fd) {
		return -EINVAL;
	}

	ret = lwm2m_get_or_create_engine_obj(msg, &obj_inst, &created);
	if (ret < 0) {
		return ret;
	}

	ret = lwm2m_engine_validate_write_access(msg, obj_inst, &obj_field);
	if (ret < 0) {
		return ret;
	}

	ret = lwm2m_engine_get_create_res_inst(&msg->path, &res, &res_inst);
	if (ret < 0) {
		return -ENOENT;
	}

	fd->current = rec;

	return lwm2m_write_handler(obj_inst, res, res_inst, obj_field, msg);
}

const struct lwm2m_writer senml_cbor_writer = {
	.put_end = put_end,
	.put_begin_oi = put_begin_oi,
	.put_begin_r = put_begin_r,
	.put_begin_ri = put_begin_ri,
	.put_s8 = put_s8,
	.put_s16 = put_s16,
	.put_s32 = put_s32,
	.put_s64 = put_s64,
	.put_time = put_time,
	.put_string = put_string,
	.put_float = NULL,
	.put_bool = put_bool,
	.put_opaque = put_opaque,
	.put_objlnk = put_objlnk,
};

const struct lwm2m_reader senml_cbor_reader = {
	.get_s32 = get_s32,
	.get_s64 = get_s64,
	.get_time = get_s64,
	.get_string = get_string,
	.get_float = NULL,
	.get_bool = get_bool,
	.get_opaque = get_opaque,
	.get_objlnk = get_objlnk,
};

int do_read_op_senml_cbor(struct lwm2m_message *msg)
{
	int ret;
	struct cbor_out_formatter_data *fd = malloc(sizeof(*fd));

	if (!fd) {
		return -ENOBUFS;
	}

	(void)memset(fd, 0, sizeof(*fd));
	engine_set_out_user_data(&msg->out, fd);
	fd->bname_sz = sizeof("/65535/65535/");
	fd->name_sz = sizeof("65535/65535");

	ret = lwm2m_perform_read_op(msg, LWM2M_FORMAT_APP_SENML_CBOR);

	free(fd);
	fd = NULL;

	engine_clear_out_user_data(&msg->out);

	return ret;
}

int do_write_op_senml_cbor(struct lwm2m_message *msg)
{
	bool decoded;
	int ret = 0;
	uint32_t decoded_sz;
	struct cbor_in_fmt_data *fd;

	/* With block-wise transfer consecutive blocks will not carry the content header -
	 * go directly to the message processing
	 */
	if (msg->in.block_ctx != NULL && msg->in.block_ctx->ctx.current > 0) {
		msg->path.res_id = msg->in.block_ctx->res_id;
		msg->path.level = msg->in.block_ctx->level;

		if (msg->path.level == LWM2M_PATH_LEVEL_RESOURCE_INST) {
			msg->path.res_inst_id = msg->in.block_ctx->res_inst_id;
		}

		return do_write_op_item(msg, NULL);
	}

	fd = malloc(sizeof(*fd));
	if (!fd) {
		LOG_WRN("unable to decode, out of memory");
		return -ENOBUFS;
	}

	(void)memset(fd, 0, sizeof(*fd));
	engine_set_in_user_data(&msg->in, fd);

	decoded = cbor_decode_lwm2m_senml(ICTX_BUF_R_PTR(&msg->in), ICTX_BUF_R_LEFT_SZ(&msg->in),
					   &fd->output, &decoded_sz);

	if (decoded) {
		msg->in.offset += decoded_sz;

		for (int idx = 0; idx < fd->output.rec_cnt; idx++) {

			ret = do_write_op_item(msg, &fd->output.recs[idx]);

			/* Write isn't supposed to fail */
			if (ret < 0) {
				break;
			}
		}
	} else {
		ret = -EBADMSG;
	}

	free(fd);
	fd = NULL;

	return ret < 0 ?  ret : decoded_sz;
}
