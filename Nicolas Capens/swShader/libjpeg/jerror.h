#ifdef __cplusplus
extern "C" {
#endif



/*
 * jerror.h
 *
 * Copyright (C) 1994-1995, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file defines the error and message codes for the JPEG library.
 * Edit this file to add new codes, or to translate the message strings to
 * some other language.
 * A set of error-reporting macros are defined too.  Some applications using
 * the JPEG library may wish to include this file to get the error codes
 * and/or the macros.
 */





/*
 * To define the enum list of message codes, include this file without
 * defining macro JMESSAGE.  To create a message string table, include it
 * again with a suitable JMESSAGE definition (see jerror.c for an example).
 */
#ifndef JMESSAGE
#ifndef JERROR_H
#include <setjmp.h>
jmp_buf *jerror_env( void );
/* First time through, define the enum list */
#define JMAKE_ENUM_LIST
#else
/* Repeated inclusions of this file are no-ops unless JMESSAGE is defined */
#define JMESSAGE(code,string)
#endif /* JERROR_H */
#endif /* JMESSAGE */


#ifdef JMAKE_ENUM_LIST

typedef enum {

#define JMESSAGE(code,string)	code ,

#endif /* JMAKE_ENUM_LIST */



JMESSAGE(JMSG_NOMESSAGE, "%d") /* Must be first entry! */

/* For maintenance convenience, list is alphabetical by message code name */

JMESSAGE(JERR_ARITH_NOTIMPL, "")
JMESSAGE(JERR_BAD_ALIGN_TYPE, "")
JMESSAGE(JERR_BAD_ALLOC_CHUNK, "")
JMESSAGE(JERR_BAD_BUFFER_MODE, "")
JMESSAGE(JERR_BAD_COMPONENT_ID, "")
JMESSAGE(JERR_BAD_DCTSIZE, "")
JMESSAGE(JERR_BAD_IN_COLORSPACE, "")
JMESSAGE(JERR_BAD_J_COLORSPACE, "")
JMESSAGE(JERR_BAD_LENGTH, "")
JMESSAGE(JERR_BAD_LIB_VERSION, "")
JMESSAGE(JERR_BAD_MCU_SIZE, "")
JMESSAGE(JERR_BAD_POOL_ID, "")
JMESSAGE(JERR_BAD_PRECISION, "")
JMESSAGE(JERR_BAD_PROGRESSION, "")
JMESSAGE(JERR_BAD_PROG_SCRIPT, "")
JMESSAGE(JERR_BAD_SAMPLING, "")
JMESSAGE(JERR_BAD_SCAN_SCRIPT, "")
JMESSAGE(JERR_BAD_STATE, "")
JMESSAGE(JERR_BAD_STRUCT_SIZE, "")
JMESSAGE(JERR_BAD_VIRTUAL_ACCESS, "")
JMESSAGE(JERR_BUFFER_SIZE, "")
JMESSAGE(JERR_CANT_SUSPEND, "")
JMESSAGE(JERR_CCIR601_NOTIMPL, "")
JMESSAGE(JERR_COMPONENT_COUNT, "")
JMESSAGE(JERR_CONVERSION_NOTIMPL, "")
JMESSAGE(JERR_DAC_INDEX, "")
JMESSAGE(JERR_DAC_VALUE, "")
JMESSAGE(JERR_DHT_COUNTS, "")
JMESSAGE(JERR_DHT_INDEX, "")
JMESSAGE(JERR_DQT_INDEX, "")
JMESSAGE(JERR_EMPTY_IMAGE, "")
JMESSAGE(JERR_EMS_READ, "")
JMESSAGE(JERR_EMS_WRITE, "")
JMESSAGE(JERR_EOI_EXPECTED, "")
JMESSAGE(JERR_FILE_READ, "")
JMESSAGE(JERR_FILE_WRITE, "")
JMESSAGE(JERR_FRACT_SAMPLE_NOTIMPL, "")
JMESSAGE(JERR_HUFF_CLEN_OVERFLOW, "")
JMESSAGE(JERR_HUFF_MISSING_CODE, "")
JMESSAGE(JERR_IMAGE_TOO_BIG, "%u")
JMESSAGE(JERR_INPUT_EMPTY, "")
JMESSAGE(JERR_INPUT_EOF, "")
JMESSAGE(JERR_MISMATCHED_QUANT_TABLE, "")
JMESSAGE(JERR_MISSING_DATA, "")
JMESSAGE(JERR_MODE_CHANGE, "")
JMESSAGE(JERR_NOTIMPL, "")
JMESSAGE(JERR_NOT_COMPILED, "")
JMESSAGE(JERR_NO_BACKING_STORE, "")
JMESSAGE(JERR_NO_HUFF_TABLE, "")
JMESSAGE(JERR_NO_IMAGE, "")
JMESSAGE(JERR_NO_QUANT_TABLE, "")
JMESSAGE(JERR_NO_SOI, "")
JMESSAGE(JERR_OUT_OF_MEMORY, "")
JMESSAGE(JERR_QUANT_COMPONENTS, "")
JMESSAGE(JERR_QUANT_FEW_COLORS, "")
JMESSAGE(JERR_QUANT_MANY_COLORS, "")
JMESSAGE(JERR_SOF_DUPLICATE, "")
JMESSAGE(JERR_SOF_NO_SOS, "")
JMESSAGE(JERR_SOF_UNSUPPORTED, "")
JMESSAGE(JERR_SOI_DUPLICATE, "")
JMESSAGE(JERR_SOS_NO_SOF, "")
JMESSAGE(JERR_TFILE_CREATE, "")
JMESSAGE(JERR_TFILE_READ, "")
JMESSAGE(JERR_TFILE_SEEK, "")
JMESSAGE(JERR_TFILE_WRITE, "")
JMESSAGE(JERR_TOO_LITTLE_DATA, "")
JMESSAGE(JERR_UNKNOWN_MARKER, "")
JMESSAGE(JERR_VIRTUAL_BUG, "")
JMESSAGE(JERR_WIDTH_OVERFLOW, "")
JMESSAGE(JERR_XMS_READ, "")
JMESSAGE(JERR_XMS_WRITE, "")
JMESSAGE(JMSG_COPYRIGHT, "")
JMESSAGE(JMSG_VERSION, "")
JMESSAGE(JTRC_16BIT_TABLES, "")
JMESSAGE(JTRC_ADOBE, "")
JMESSAGE(JTRC_APP0, "")
JMESSAGE(JTRC_APP14, "")
JMESSAGE(JTRC_DAC, "")
JMESSAGE(JTRC_DHT, "")
JMESSAGE(JTRC_DQT, "")
JMESSAGE(JTRC_DRI, "")
JMESSAGE(JTRC_EMS_CLOSE, "")
JMESSAGE(JTRC_EMS_OPEN, "")
JMESSAGE(JTRC_EOI, "")
JMESSAGE(JTRC_HUFFBITS, "")
JMESSAGE(JTRC_JFIF, "")
JMESSAGE(JTRC_JFIF_BADTHUMBNAILSIZE, "")
JMESSAGE(JTRC_JFIF_MINOR, "")
JMESSAGE(JTRC_JFIF_THUMBNAIL, "")
JMESSAGE(JTRC_MISC_MARKER, "")
JMESSAGE(JTRC_PARMLESS_MARKER, "")
JMESSAGE(JTRC_QUANTVALS, "")
JMESSAGE(JTRC_QUANT_3_NCOLORS, "")
JMESSAGE(JTRC_QUANT_NCOLORS, "")
JMESSAGE(JTRC_QUANT_SELECTED, "")
JMESSAGE(JTRC_RECOVERY_ACTION, "")
JMESSAGE(JTRC_RST, "")
JMESSAGE(JTRC_SMOOTH_NOTIMPL, "")
JMESSAGE(JTRC_SOF, "")
JMESSAGE(JTRC_SOF_COMPONENT, "")
JMESSAGE(JTRC_SOI, "")
JMESSAGE(JTRC_SOS, "")
JMESSAGE(JTRC_SOS_COMPONENT, "")
JMESSAGE(JTRC_SOS_PARAMS, "")
JMESSAGE(JTRC_TFILE_CLOSE, "")
JMESSAGE(JTRC_TFILE_OPEN, "")
JMESSAGE(JTRC_UNKNOWN_IDS, "")
JMESSAGE(JTRC_XMS_CLOSE, "")
JMESSAGE(JTRC_XMS_OPEN, "")
JMESSAGE(JWRN_ADOBE_XFORM, "")
JMESSAGE(JWRN_BOGUS_PROGRESSION, "")
JMESSAGE(JWRN_EXTRANEOUS_DATA, "")
JMESSAGE(JWRN_HIT_MARKER, "")
JMESSAGE(JWRN_HUFF_BAD_CODE, "")
JMESSAGE(JWRN_JFIF_MAJOR, "")
JMESSAGE(JWRN_JPEG_EOF, "")
JMESSAGE(JWRN_MUST_RESYNC, "")
JMESSAGE(JWRN_NOT_SEQUENTIAL, "")
JMESSAGE(JWRN_TOO_MUCH_DATA, "")

#ifdef JMAKE_ENUM_LIST

  JMSG_LASTMSGCODE
} J_MESSAGE_CODE;

#undef JMAKE_ENUM_LIST
#endif /* JMAKE_ENUM_LIST */

/* Zap JMESSAGE macro so that future re-inclusions do nothing by default */
#undef JMESSAGE


#ifndef JERROR_H
#define JERROR_H

/* Macros to simplify using the error and trace message stuff */
/* The first parameter is either type of cinfo pointer */

/* Fatal errors (print message and exit) */
#define ERREXIT(cinfo,code)    ((cinfo)->err->msg_code = (code),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT1(cinfo,code,p1)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT2(cinfo,code,p1,p2)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (cinfo)->err->msg_parm.i[1] = (p2),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT3(cinfo,code,p1,p2,p3)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (cinfo)->err->msg_parm.i[1] = (p2),    (cinfo)->err->msg_parm.i[2] = (p3),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXIT4(cinfo,code,p1,p2,p3,p4)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (cinfo)->err->msg_parm.i[1] = (p2),    (cinfo)->err->msg_parm.i[2] = (p3),    (cinfo)->err->msg_parm.i[3] = (p4),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))
#define ERREXITS(cinfo,code,str)    ((cinfo)->err->msg_code = (code),    strncpy((cinfo)->err->msg_parm.s, (str), JMSG_STR_PARM_MAX),    (*(cinfo)->err->error_exit) ((j_common_ptr) (cinfo)))

#define MAKESTMT(stuff)		do { stuff } while (0)

/* Nonfatal errors (we can keep going, but the data is probably corrupt) */
#define WARNMS(cinfo,code)    ((cinfo)->err->msg_code = (code),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))
#define WARNMS1(cinfo,code,p1)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))
#define WARNMS2(cinfo,code,p1,p2)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (cinfo)->err->msg_parm.i[1] = (p2),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), -1))

/* Informational/debugging messages */
#define TRACEMS(cinfo,lvl,code)    ((cinfo)->err->msg_code = (code),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS1(cinfo,lvl,code,p1)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS2(cinfo,lvl,code,p1,p2)    ((cinfo)->err->msg_code = (code),    (cinfo)->err->msg_parm.i[0] = (p1),    (cinfo)->err->msg_parm.i[1] = (p2),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))
#define TRACEMS3(cinfo,lvl,code,p1,p2,p3)    MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; 	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); 	   (cinfo)->err->msg_code = (code); 	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMS4(cinfo,lvl,code,p1,p2,p3,p4)    MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; 	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); _mp[3] = (p4); 	   (cinfo)->err->msg_code = (code); 	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMS8(cinfo,lvl,code,p1,p2,p3,p4,p5,p6,p7,p8)    MAKESTMT(int * _mp = (cinfo)->err->msg_parm.i; 	   _mp[0] = (p1); _mp[1] = (p2); _mp[2] = (p3); _mp[3] = (p4); 	   _mp[4] = (p5); _mp[5] = (p6); _mp[6] = (p7); _mp[7] = (p8); 	   (cinfo)->err->msg_code = (code); 	   (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)); )
#define TRACEMSS(cinfo,lvl,code,str)    ((cinfo)->err->msg_code = (code),    strncpy((cinfo)->err->msg_parm.s, (str), JMSG_STR_PARM_MAX),    (*(cinfo)->err->emit_message) ((j_common_ptr) (cinfo), (lvl)))

#endif /* JERROR_H */


#ifdef __cplusplus
}
#endif
