/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DefPosition"
 * 	found in "all.asn"
 * 	`asn1c -gen-PER`
 */

#include "Position3D.h"

static asn_TYPE_member_t asn_MBR_Position3D_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct Position3D, lat),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Latitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"lat"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Position3D, Long),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Longitude,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"long"
		},
	{ ATF_POINTER, 1, offsetof(struct Position3D, elevation),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Elevation,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"elevation"
		},
};
static const int asn_MAP_Position3D_oms_1[] = { 2 };
static const ber_tlv_tag_t asn_DEF_Position3D_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_Position3D_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* lat */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* long */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* elevation */
};
static asn_SEQUENCE_specifics_t asn_SPC_Position3D_specs_1 = {
	sizeof(struct Position3D),
	offsetof(struct Position3D, _asn_ctx),
	asn_MAP_Position3D_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_Position3D_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_Position3D = {
	"Position3D",
	"Position3D",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_Position3D_tags_1,
	sizeof(asn_DEF_Position3D_tags_1)
		/sizeof(asn_DEF_Position3D_tags_1[0]), /* 1 */
	asn_DEF_Position3D_tags_1,	/* Same as above */
	sizeof(asn_DEF_Position3D_tags_1)
		/sizeof(asn_DEF_Position3D_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_Position3D_1,
	3,	/* Elements count */
	&asn_SPC_Position3D_specs_1	/* Additional specs */
};

