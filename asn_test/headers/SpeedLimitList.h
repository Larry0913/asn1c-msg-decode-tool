/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MapSpeedLimit"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_SpeedLimitList_H_
#define	_SpeedLimitList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RegulatorySpeedLimit;

/* SpeedLimitList */
typedef struct SpeedLimitList {
	A_SEQUENCE_OF(struct RegulatorySpeedLimit) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SpeedLimitList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SpeedLimitList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RegulatorySpeedLimit.h"

#endif	/* _SpeedLimitList_H_ */
#include <asn_internal.h>
