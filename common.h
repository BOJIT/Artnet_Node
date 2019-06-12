
#ifndef _defined_artnet_common_h
#define _defined_artnet_common_h

// ### LH - seem to have been lost
// is used in most examples
#define ARTNET_MAX_DMX   512 

/** 
 * The maximum ports per node built into the ArtNet protocol.
 * This is always 4. Don't change it unless you really know what your doing
 */
enum { ARTNET_MAX_PORTS = 4 };

/**
 * The length of the short name field. Always 18
 */
enum { ARTNET_SHORT_NAME_LENGTH = 18 };

/**
 * The length of the long name field. Always 64 
 */
enum { ARTNET_LONG_NAME_LENGTH	= 64 } ;

/**
 * The length of the report field. Always 64
 */
enum { ARTNET_REPORT_LENGTH = 64 };

/**
 * The length of the DMX field. Always 512
 */
enum { ARTNET_DMX_LENGTH  = 512 };

/*
 * Number of bytes in a RDM UID
 */
enum { ARTNET_RDM_UID_WIDTH = 6 };

/*
 * Length of the hardware address
 */
enum { ARTNET_MAC_SIZE = 6 };

#endif
