/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "SPATIntersectionState"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_PhaseState_H_
#define	_PhaseState_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LightState.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TimeChangeDetails;

/* PhaseState */
typedef struct PhaseState {
	LightState_t	 light;
	struct TimeChangeDetails	*timing	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PhaseState_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PhaseState;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "TimeChangeDetails.h"

#endif	/* _PhaseState_H_ */
#include <asn_internal.h>
