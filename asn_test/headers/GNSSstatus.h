/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "VehSafetyExt"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_GNSSstatus_H_
#define	_GNSSstatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GNSSstatus {
	GNSSstatus_unavailable	= 0,
	GNSSstatus_isHealthy	= 1,
	GNSSstatus_isMonitored	= 2,
	GNSSstatus_baseStationType	= 3,
	GNSSstatus_aPDOPofUnder5	= 4,
	GNSSstatus_inViewOfUnder5	= 5,
	GNSSstatus_localCorrectionsPresent	= 6,
	GNSSstatus_networkCorrectionsPresent	= 7
} e_GNSSstatus;

/* GNSSstatus */
typedef BIT_STRING_t	 GNSSstatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GNSSstatus;
asn_struct_free_f GNSSstatus_free;
asn_struct_print_f GNSSstatus_print;
asn_constr_check_f GNSSstatus_constraint;
ber_type_decoder_f GNSSstatus_decode_ber;
der_type_encoder_f GNSSstatus_encode_der;
xer_type_decoder_f GNSSstatus_decode_xer;
xer_type_encoder_f GNSSstatus_encode_xer;
per_type_decoder_f GNSSstatus_decode_uper;
per_type_encoder_f GNSSstatus_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _GNSSstatus_H_ */
#include <asn_internal.h>
