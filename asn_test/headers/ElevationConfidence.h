/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DefPosition"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_ElevationConfidence_H_
#define	_ElevationConfidence_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ElevationConfidence {
	ElevationConfidence_unavailable	= 0,
	ElevationConfidence_elev_500_00	= 1,
	ElevationConfidence_elev_200_00	= 2,
	ElevationConfidence_elev_100_00	= 3,
	ElevationConfidence_elev_050_00	= 4,
	ElevationConfidence_elev_020_00	= 5,
	ElevationConfidence_elev_010_00	= 6,
	ElevationConfidence_elev_005_00	= 7,
	ElevationConfidence_elev_002_00	= 8,
	ElevationConfidence_elev_001_00	= 9,
	ElevationConfidence_elev_000_50	= 10,
	ElevationConfidence_elev_000_20	= 11,
	ElevationConfidence_elev_000_10	= 12,
	ElevationConfidence_elev_000_05	= 13,
	ElevationConfidence_elev_000_02	= 14,
	ElevationConfidence_elev_000_01	= 15
} e_ElevationConfidence;

/* ElevationConfidence */
typedef long	 ElevationConfidence_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ElevationConfidence;
asn_struct_free_f ElevationConfidence_free;
asn_struct_print_f ElevationConfidence_print;
asn_constr_check_f ElevationConfidence_constraint;
ber_type_decoder_f ElevationConfidence_decode_ber;
der_type_encoder_f ElevationConfidence_encode_der;
xer_type_decoder_f ElevationConfidence_decode_xer;
xer_type_encoder_f ElevationConfidence_encode_xer;
per_type_decoder_f ElevationConfidence_decode_uper;
per_type_encoder_f ElevationConfidence_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ElevationConfidence_H_ */
#include <asn_internal.h>
