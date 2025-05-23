/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "VehSafetyExt"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_VehicleSafetyExtensions_H_
#define	_VehicleSafetyExtensions_H_


#include <asn_application.h>

/* Including external dependencies */
#include "VehicleEventFlags.h"
#include "ExteriorLights.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PathHistory;
struct PathPrediction;

/* VehicleSafetyExtensions */
typedef struct VehicleSafetyExtensions {
	VehicleEventFlags_t	*events	/* OPTIONAL */;
	struct PathHistory	*pathHistory	/* OPTIONAL */;
	struct PathPrediction	*pathPrediction	/* OPTIONAL */;
	ExteriorLights_t	*lights	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VehicleSafetyExtensions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VehicleSafetyExtensions;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PathHistory.h"
#include "PathPrediction.h"

#endif	/* _VehicleSafetyExtensions_H_ */
#include <asn_internal.h>
