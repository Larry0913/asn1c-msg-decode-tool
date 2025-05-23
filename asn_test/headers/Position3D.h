/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DefPosition"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_Position3D_H_
#define	_Position3D_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Latitude.h"
#include "Longitude.h"
#include "Elevation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Position3D */
typedef struct Position3D {
	Latitude_t	 lat;
	Longitude_t	 Long;
	Elevation_t	*elevation	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Position3D_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Position3D;

#ifdef __cplusplus
}
#endif

#endif	/* _Position3D_H_ */
#include <asn_internal.h>
