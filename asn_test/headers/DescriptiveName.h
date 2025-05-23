/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MapNode"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#ifndef	_DescriptiveName_H_
#define	_DescriptiveName_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DescriptiveName */
typedef IA5String_t	 DescriptiveName_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DescriptiveName;
asn_struct_free_f DescriptiveName_free;
asn_struct_print_f DescriptiveName_print;
asn_constr_check_f DescriptiveName_constraint;
ber_type_decoder_f DescriptiveName_decode_ber;
der_type_encoder_f DescriptiveName_encode_der;
xer_type_decoder_f DescriptiveName_decode_xer;
xer_type_encoder_f DescriptiveName_encode_xer;
per_type_decoder_f DescriptiveName_decode_uper;
per_type_encoder_f DescriptiveName_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DescriptiveName_H_ */
#include <asn_internal.h>
