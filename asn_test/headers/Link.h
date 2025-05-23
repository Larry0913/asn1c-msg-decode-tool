/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MapLink"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_Link_H_
#define	_Link_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DescriptiveName.h"
#include "NodeReferenceID.h"
#include "LaneWidth.h"
#include "LaneList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SpeedLimitList;
struct PointList;
struct MovementList;

/* Link */
typedef struct Link {
	DescriptiveName_t	*name	/* OPTIONAL */;
	NodeReferenceID_t	 upstreamNodeId;
	struct SpeedLimitList	*speedLimits	/* OPTIONAL */;
	LaneWidth_t	 linkWidth;
	struct PointList	*points	/* OPTIONAL */;
	struct MovementList	*movements	/* OPTIONAL */;
	LaneList_t	 lanes;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Link_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Link;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SpeedLimitList.h"
#include "PointList.h"
#include "MovementList.h"

#endif	/* _Link_H_ */
#include <asn_internal.h>
