/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LWM2M_RW_SENML_CBOR_H_
#define LWM2M_RW_SENML_CBOR_H_

#include "lwm2m_object.h"

extern const struct lwm2m_writer senml_cbor_writer;
extern const struct lwm2m_reader senml_cbor_reader;

int do_read_op_senml_cbor(struct lwm2m_message *msg);
int do_composite_read_op_senml_cbor(struct lwm2m_message *msg);
int do_write_op_senml_cbor(struct lwm2m_message *msg);

#endif /* LWM2M_RW_SENML_CBOR_H_ */
