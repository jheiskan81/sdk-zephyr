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

#ifndef LWM2M_SENML_CBOR_H__
#define LWM2M_SENML_CBOR_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <cbor_common.h>
#include <cbor_decode.h>
#include <cbor_encode.h>

struct record_bn {
	cbor_string_type_t hndl;
};

struct record_bt {
	int32_t val;
};

struct record_n {
	cbor_string_type_t hndl;
};

struct record_t {
	int32_t val;
};

struct record_union_ {
	union {
		struct {
			int32_t v; /** Value */
		};
		struct {
			cbor_string_type_t vs; /** String value */
		};
		struct {
			bool vb; /** Boolean value */
		};
		struct {
			cbor_string_type_t vd; /** Data value */
		};
	};
	enum { LSB_U_V = 2,
	       LSB_U_VS = 3,
	       LSB_U_VB = 4,
	       LSB_U_VD = 8,
	} choice;
};

struct non_b_label_ {
	union {
		cbor_string_type_t _non_b_label_tstr;
		uint32_t _non_b_label_uint;
	};
	enum { _non_b_label_tstr,
	       _non_b_label_uint,
	} choice;
};

struct b_label_ {
	union {
		cbor_string_type_t _b_label_tstr;
		int32_t _b_label_nint;
	};
	enum { _b_label_tstr,
	       _b_label_nint,
	} choice;
};

struct label_ {
	union {
		struct non_b_label_ _label__non_b_label;
		struct b_label_ _label__b_label;
	};
	enum { _label__non_b_label,
	       _label__b_label,
	} choice;
};

struct value_ {
	union {
		cbor_string_type_t _value_tstr;
		cbor_string_type_t _value_bstr;
		int32_t _value__numeric;
		bool _value_bool;
	};
	enum { _value_tstr,
	       _value_bstr,
	       _value__numeric,
	       _value_bool,
	} choice;
};

struct key_value_pair_label {
	struct label_ key;
	struct value_ val;
};

struct record__key_value_pair {
	struct key_value_pair_label lbl;
};

struct record {
	struct record_bn bn;
	uint32_t bn_prsnt;
	struct record_bt bt;
	uint32_t bt_prsnt;
	struct record_n n;
	uint32_t n_prsnt;
	struct record_t t;
	uint32_t t_prsnt;
	struct record_union_ u;
	uint32_t u_prsnt;
	struct record__key_value_pair kvp[CONFIG_LWM2M_RW_SENML_CBOR_RECORDS];
	uint32_t kvp_cnt;
};

struct lwm2m_senml {
	struct record recs[CONFIG_LWM2M_RW_SENML_CBOR_RECORDS];
	uint32_t rec_cnt;
};

bool cbor_encode_lwm2m_senml(uint8_t *payload, uint32_t payload_len,
			     const struct lwm2m_senml *input, uint32_t *payload_len_out);

bool cbor_decode_lwm2m_senml(const uint8_t *payload, uint32_t payload_len,
			     struct lwm2m_senml *result, uint32_t *payload_len_out);

#endif /* LWM2M_SENML_CBOR_H__ */
