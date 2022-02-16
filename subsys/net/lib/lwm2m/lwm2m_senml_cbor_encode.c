/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/*
 * Generated using cddl_gen version 0.3.99
 * https://github.com/NordicSemiconductor/cddl-gen
 * Generated with a default_max_qty of 3
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <sys/util.h>

#include "cbor_encode.h"
#include "lwm2m_senml_cbor.h"

static bool encode_repeated_record_bn(cbor_state_t *state, const struct record_bn *input)
{
	cbor_print("%s\n", __func__);

	int32_t tmp = -2;

	bool tmp_result = ((((intx32_encode(state, (int32_t *)&tmp))) &&
			    (tstrx_encode(state, (&(*input).hndl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_repeated_record_bt(cbor_state_t *state, const struct record_bt *input)
{
	cbor_print("%s\n", __func__);

	int32_t tmp = -3;

	bool tmp_result = ((((intx32_encode(state, (int32_t *)&tmp))) &&
			    (intx32_encode(state, (&(*input).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_repeated_record_n(cbor_state_t *state, const struct record_n *input)
{
	cbor_print("%s\n", __func__);

	uint32_t tmp = 0;

	bool tmp_result = ((((uintx32_encode(state, (uint32_t *)&tmp))) &&
			    (tstrx_encode(state, (&(*input).hndl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_repeated_record_t(cbor_state_t *state, const struct record_t *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		((((uintx32_put(state, (6)))) && (intx32_encode(state, (&(*input).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_repeated_record_union(cbor_state_t *state, const struct record_union_ *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		(((((*input).choice == LSB_U_V) ?
			   (((uintx32_put(state, (2)))) && (intx32_encode(state, (&(*input).v)))) :
			   (((*input).choice == LSB_U_VS) ?
				    (((uintx32_put(state, (3)))) &&
				     (tstrx_encode(state, (&(*input).vs)))) :
				    (((*input).choice == LSB_U_VB) ?
					     (((uintx32_put(state, (4)))) &&
					      (boolx_encode(state, (&(*input).vb)))) :
					     (((*input).choice == LSB_U_VD) ?
						      (((uintx32_put(state, (8)))) &&
						       (bstrx_encode(state, (&(*input).vd)))) :
						      false))))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_non_b_label(cbor_state_t *state, const struct non_b_label_ *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		(((((*input).choice == _non_b_label_tstr) ?
			   ((tstrx_encode(state, (&(*input)._non_b_label_tstr)))) :
			   (((*input).choice == _non_b_label_uint) ?
				    ((uintx32_encode(state, (&(*input)._non_b_label_uint)))) :
				    false))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_b_label(cbor_state_t *state, const struct b_label_ *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = (((((*input).choice == _b_label_tstr) ?
				     ((tstrx_encode(state, (&(*input)._b_label_tstr)))) :
				     (((*input).choice == _b_label_nint) ?
					      (((*input)._b_label_nint <= -1) &&
					       (intx32_encode(state, (&(*input)._b_label_nint)))) :
					      false))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_label(cbor_state_t *state, const struct label_ *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		(((((*input).choice == _label__non_b_label) ?
			   ((encode_non_b_label(state, (&(*input)._label__non_b_label)))) :
			   (((*input).choice == _label__b_label) ?
				    ((encode_b_label(state, (&(*input)._label__b_label)))) :
				    false))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_value(cbor_state_t *state, const struct value_ *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = (((
		((*input).choice == _value_tstr) ?
			((tstrx_encode(state, (&(*input)._value_tstr)))) :
			(((*input).choice == _value_bstr) ?
				 ((bstrx_encode(state, (&(*input)._value_bstr)))) :
				 (((*input).choice == _value__numeric) ?
					  ((intx32_encode(state, (&(*input)._value__numeric)))) :
					  (((*input).choice == _value_bool) ?
						((boolx_encode(state, (&(*input)._value_bool)))) :
						false))))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_key_value_pair_label(cbor_state_t *state,
					const struct key_value_pair_label *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = ((((encode_label(state, (&(*input).key)))) &&
			    (encode_value(state, (&(*input).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_repeated_record__key_value_pair(cbor_state_t *state,
						   const struct record__key_value_pair *input)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = (((encode_key_value_pair_label(state, (&(*input).lbl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_record(cbor_state_t *state, const struct record *input)
{
	cbor_print("%s\n", __func__);
	bool int_res;
	size_t max_records = ARRAY_SIZE(input->kvp);

	bool tmp_result = ((
		(map_start_encode(state, 8) &&
		 (int_res =
			  (present_encode(&((*input).bn_prsnt), (void *)encode_repeated_record_bn,
					  state, (&(*input).bn)) &&
			   present_encode(&((*input).bt_prsnt), (void *)encode_repeated_record_bt,
					  state, (&(*input).bt)) &&
			   present_encode(&((*input).n_prsnt), (void *)encode_repeated_record_n,
					  state, (&(*input).n)) &&
			   present_encode(&((*input).t_prsnt), (void *)encode_repeated_record_t,
					  state, (&(*input).t)) &&
			   present_encode(&((*input).u_prsnt), (void *)encode_repeated_record_union,
					  state, (&(*input).u)) &&
			   multi_encode(0, max_records, &(*input).kvp_cnt,
					(void *)encode_repeated_record__key_value_pair, state,
					(&(*input).kvp), sizeof(struct record__key_value_pair))),
		  ((map_end_encode(state, 8)) && int_res)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool encode_lwm2m_senml(cbor_state_t *state, const struct lwm2m_senml *input)
{
	cbor_print("%s\n", __func__);
	bool int_res;
	size_t max_records = ARRAY_SIZE(input->recs);

	bool tmp_result = ((
		(list_start_encode(state, max_records) &&
		 (int_res = (multi_encode(1, max_records, &(*input).rec_cnt, (void *)encode_record,
					  state, (&(*input).recs), sizeof(struct record))),
		  ((list_end_encode(state, max_records)) && int_res)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

bool cbor_encode_lwm2m_senml(uint8_t *payload, uint32_t payload_len,
			     const struct lwm2m_senml *input, uint32_t *payload_len_out)
{
	cbor_state_t states[7];

	new_state(states, sizeof(states) / sizeof(cbor_state_t), payload, payload_len, 1);

	bool ret = encode_lwm2m_senml(states, input);

	if (ret && (payload_len_out != NULL)) {
		*payload_len_out = MIN(payload_len, (size_t)states[0].payload - (size_t)payload);
	}

	return ret;
}
