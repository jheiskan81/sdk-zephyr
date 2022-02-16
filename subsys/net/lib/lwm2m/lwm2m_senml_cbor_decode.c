/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
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

#include "cbor_decode.h"
#include "lwm2m_senml_cbor.h"

static bool decode_repeated_record_bn(cbor_state_t *state, struct record_bn *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		((((intx32_expect(state, (-2)))) && (tstrx_decode(state, (&(*result).hndl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_repeated_record_bt(cbor_state_t *state, struct record_bt *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		((((intx32_expect(state, (-3)))) && (intx32_decode(state, (&(*result).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_repeated_record_n(cbor_state_t *state, struct record_n *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		((((uintx32_expect(state, (0)))) && (tstrx_decode(state, (&(*result).hndl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_repeated_record_t(cbor_state_t *state, struct record_t *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result =
		((((uintx32_expect(state, (6)))) && (intx32_decode(state, (&(*result).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_repeated_record_union(cbor_state_t *state, struct record_union_ *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = (((((uintx32_decode(state, (uint32_t *)&(*result).choice))) &&
			     ((((*result).choice == LSB_U_V) &&
			       (((1) && (intx32_decode(state, (&(*result).v)))))) ||
			      (((*result).choice == LSB_U_VS) &&
			       (((1) && (tstrx_decode(state, (&(*result).vs)))))) ||
			      (((*result).choice == LSB_U_VB) &&
			       (((1) && (boolx_decode(state, (&(*result).vb)))))) ||
			      (((*result).choice == LSB_U_VD) &&
			       (((1) && (bstrx_decode(state, (&(*result).vd))))))))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_non_b_label(cbor_state_t *state, struct non_b_label_ *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;

	bool tmp_result =
		(((union_start_code(state) &&
		   (int_res = ((((tstrx_decode(state, (&(*result)._non_b_label_tstr)))) &&
				(((*result).choice = _non_b_label_tstr) || 1)) ||
			       (((uintx32_decode(state, (&(*result)._non_b_label_uint)))) &&
				(((*result).choice = _non_b_label_uint) || 1))),
		    union_end_code(state), int_res))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_b_label(cbor_state_t *state, struct b_label_ *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;

	bool tmp_result = (((union_start_code(state) &&
			     (int_res = ((((tstrx_decode(state, (&(*result)._b_label_tstr)))) &&
					  (((*result).choice = _b_label_tstr) || 1)) ||
					 (((intx32_decode(state, (&(*result)._b_label_nint))) &&
					   ((*result)._b_label_nint <= -1)) &&
					  (((*result).choice = _b_label_nint) || 1))),
			      union_end_code(state), int_res))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_label(cbor_state_t *state, struct label_ *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;

	bool tmp_result =
		(((union_start_code(state) &&
		   (int_res = ((((decode_non_b_label(state, (&(*result)._label__non_b_label)))) &&
				(((*result).choice = _label__non_b_label) || 1)) ||
			       (union_elem_code(state) &&
				(((decode_b_label(state, (&(*result)._label__b_label)))) &&
				 (((*result).choice = _label__b_label) || 1)))),
		    union_end_code(state), int_res))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_value(cbor_state_t *state, struct value_ *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;

	bool tmp_result = (((union_start_code(state) &&
			     (int_res = ((((tstrx_decode(state, (&(*result)._value_tstr)))) &&
					  (((*result).choice = _value_tstr) || 1)) ||
					 (((bstrx_decode(state, (&(*result)._value_bstr)))) &&
					  (((*result).choice = _value_bstr) || 1)) ||
					 (((intx32_decode(state, (&(*result)._value__numeric)))) &&
					  (((*result).choice = _value__numeric) || 1)) ||
					 (((boolx_decode(state, (&(*result)._value_bool)))) &&
					  (((*result).choice = _value_bool) || 1))),
			      union_end_code(state), int_res))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_key_value_pair_label(cbor_state_t *state, struct key_value_pair_label *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = ((((decode_label(state, (&(*result).key)))) &&
			    (decode_value(state, (&(*result).val)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_repeated_record__key_value_pair(cbor_state_t *state,
						   struct record__key_value_pair *result)
{
	cbor_print("%s\n", __func__);

	bool tmp_result = (((decode_key_value_pair_label(state, (&(*result).lbl)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_record(cbor_state_t *state, struct record *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;
	size_t max_records = ARRAY_SIZE(result->kvp);

	bool tmp_result = (((
		map_start_decode(state) &&
		(int_res =
			 (present_decode(&((*result).bn_prsnt), (void *)decode_repeated_record_bn,
					 state, (&(*result).bn)) &&
			  present_decode(&((*result).bt_prsnt), (void *)decode_repeated_record_bt,
					 state, (&(*result).bt)) &&
			  present_decode(&((*result).n_prsnt), (void *)decode_repeated_record_n,
					 state, (&(*result).n)) &&
			  present_decode(&((*result).t_prsnt), (void *)decode_repeated_record_t,
					 state, (&(*result).t)) &&
			  present_decode(&((*result).u_prsnt), (void *)decode_repeated_record_union,
					 state, (&(*result).u)) &&
			  multi_decode(0, max_records, &(*result).kvp_cnt,
				       (void *)decode_repeated_record__key_value_pair, state,
				       (&(*result).kvp), sizeof(struct record__key_value_pair))),
		 ((map_end_decode(state)) && int_res)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

static bool decode_lwm2m_senml(cbor_state_t *state, struct lwm2m_senml *result)
{
	cbor_print("%s\n", __func__);
	bool int_res;

	size_t max_records = ARRAY_SIZE(result->recs);

	bool tmp_result = ((
		(list_start_decode(state) &&
		 (int_res = (multi_decode(1, max_records, &(*result).rec_cnt, (void *)decode_record,
					  state, (&(*result).recs), sizeof(struct record))),
		  ((list_end_decode(state)) && int_res)))));

	if (!tmp_result)
		cbor_trace();

	return tmp_result;
}

bool cbor_decode_lwm2m_senml(const uint8_t *payload, uint32_t payload_len,
			     struct lwm2m_senml *result, uint32_t *payload_len_out)
{
	cbor_state_t states[7];

	new_state(states, sizeof(states) / sizeof(cbor_state_t), payload, payload_len, 1);

	bool ret = decode_lwm2m_senml(states, result);

	if (ret && (payload_len_out != NULL)) {
		*payload_len_out = MIN(payload_len, (size_t)states[0].payload - (size_t)payload);
	}

	return ret;
}
