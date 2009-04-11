//only include in pm_bytestr_charset.c
#include "pmkernel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "pm_format.h"

#include "iconv/iconv-lt.h"
#include "pm_bytestr_def.h"
#include <libcharguess/charguess.h>
#include "pm_bytestr_charset.h"

#include "sys/stat.h"
#ifndef WIN32
	#include "errno.h"
#endif //end

#ifdef WIN32
	#pragma comment(lib,"libcharguess.lib")	
	#pragma comment(lib,"iconv.lib")
#endif


struct charset_mapping_table
{
    int	charset_index;
    const char *charset_name;
};

const char* supported_charsets[] =
{
	PM_CHARSET_US_ASCII				,
	PM_CHARSET_UTF8					,
	PM_CHARSET_UCS_2				,
	PM_CHARSET_UCS_2BE				,
	PM_CHARSET_UCS_2LE				,
	PM_CHARSET_UCS_4				,
	PM_CHARSET_UCS_4BE				,
	PM_CHARSET_UCS_4LE				,
	PM_CHARSET_UTF_16				,
	PM_CHARSET_UTF_16BE				,
	PM_CHARSET_UTF_16LE				,
	PM_CHARSET_UTF_32				,
	PM_CHARSET_UTF_32BE				,
	PM_CHARSET_UTF_32LE				,
	PM_CHARSET_UTF_7				,
	PM_CHARSET_UCS_2_INTERNAL		,
	PM_CHARSET_UCS_2_SWAPPED		,
	PM_CHARSET_UCS_4_INTERNAL		,
	PM_CHARSET_UCS_4_SWAPPED		,
	PM_CHARSET_C99					,
	PM_CHARSET_JAVA					,
	PM_CHARSET_ISO_8859_1			,
	PM_CHARSET_ISO_8859_2			,
	PM_CHARSET_ISO_8859_3			,
	PM_CHARSET_ISO_8859_4			,
	PM_CHARSET_ISO_8859_5			,
	PM_CHARSET_ISO_8859_6			,
	PM_CHARSET_ISO_8859_7			,
	PM_CHARSET_ISO_8859_8			,
	PM_CHARSET_ISO_8859_9			,
	PM_CHARSET_ISO_8859_10			,
	PM_CHARSET_ISO_8859_11			,
	PM_CHARSET_ISO_8859_13			,
	PM_CHARSET_ISO_8859_14			,
	PM_CHARSET_ISO_8859_15			,
	PM_CHARSET_ISO_8859_16			,
	PM_CHARSET_KOI8_R				,
	PM_CHARSET_KOI8_U				,
	PM_CHARSET_KOI8_RU				,
	PM_CHARSET_CP1250				,
	PM_CHARSET_CP1251				,
	PM_CHARSET_CP1252				,
	PM_CHARSET_CP1253				,
	PM_CHARSET_CP1254				,
	PM_CHARSET_CP1255				,
	PM_CHARSET_CP1256				,
	PM_CHARSET_CP1257				,
	PM_CHARSET_CP1258				,
	PM_CHARSET_CP850				,
	PM_CHARSET_CP862				,
	PM_CHARSET_CP866				,
	PM_CHARSET_MacRoman				,
	PM_CHARSET_MacCentralEurope		,
	PM_CHARSET_MacIceland			,
	PM_CHARSET_MacCroatian			,
	PM_CHARSET_MacRomania			,
	PM_CHARSET_MacCyrillic			,
	PM_CHARSET_MacUkraine			,
	PM_CHARSET_MacGreek				,
	PM_CHARSET_MacTurkish			,
	PM_CHARSET_MacHebrew			,
	PM_CHARSET_MacArabic			,
	PM_CHARSET_MacThai				,
	PM_CHARSET_HP_ROMAN8			,
	PM_CHARSET_NEXTSTEP				,
	PM_CHARSET_ARMSCII_8			,
	PM_CHARSET_GEORGIAN_ACADEMY		,
	PM_CHARSET_GEORGIAN_PS			,
	PM_CHARSET_KOI8_T				,
	PM_CHARSET_PT154				,
	PM_CHARSET_MULELAO_1			,
	PM_CHARSET_CP1133				,
	PM_CHARSET_TIS_620				,
	PM_CHARSET_CP874				,
	PM_CHARSET_TCVN					,
	PM_CHARSET_JIS_C6220_1969_RO	,
	PM_CHARSET_JIS_X0201			,
	PM_CHARSET_JIS_X0208			,
	PM_CHARSET_JIS_X0212			,
	PM_CHARSET_GB_1988_80			,
	PM_CHARSET_GB_2312_80			,
	PM_CHARSET_ISO_IR_165			,
	PM_CHARSET_KSC_5601				,
	PM_CHARSET_EUC_JP				,
	PM_CHARSET_SHIFT_JIS			,
	PM_CHARSET_CP932				,
	PM_CHARSET_ISO_2022_JP			,
	PM_CHARSET_ISO_2022_JP_1		,
	PM_CHARSET_ISO_2022_JP_2		,
	PM_CHARSET_GB2312				,
	PM_CHARSET_GBK					,
	PM_CHARSET_CP936				,
	PM_CHARSET_GB18030				,
	PM_CHARSET_ISO_2022_CN			,
	PM_CHARSET_ISO_2022_CN_EXT		,
	PM_CHARSET_HZ					,
	PM_CHARSET_EUC_TW				,
	PM_CHARSET_BIG5					,
	PM_CHARSET_CP950				,
	PM_CHARSET_BIG5_HKSCS_1999		,
	PM_CHARSET_BIG5_HKSCS_2001		,
	PM_CHARSET_BIG5_HKSCS			,
	PM_CHARSET_EUC_KR				,
	PM_CHARSET_CP949				,
	PM_CHARSET_JOHAB				,
	PM_CHARSET_ISO_2022_KR			,
	PM_CHARSET_EUC_JISX0213			,
	PM_CHARSET_SHIFT_JISX0213		,
	PM_CHARSET_ISO_2022_JP_3		,
	PM_CHARSET_BIG5_2003			,
	PM_CHARSET_TDS565				,
	PM_CHARSET_ATARIST				,
	PM_CHARSET_RISCOS_LATIN1		,
	PM_CHARSET_DEC_KANJI			,
	PM_CHARSET_DEC_HANYU			,
	PM_CHARSET_CP437				,
	PM_CHARSET_CP737				,
	PM_CHARSET_CP775				,
	PM_CHARSET_CP852				,
	PM_CHARSET_CP853				,
	PM_CHARSET_CP855				,
	PM_CHARSET_CP857				,
	PM_CHARSET_CP858				,
	PM_CHARSET_CP860				,
	PM_CHARSET_CP861				,
	PM_CHARSET_CP863				,
	PM_CHARSET_CP864				,
	PM_CHARSET_CP865				,
	PM_CHARSET_CP869				,
	PM_CHARSET_CP1125				,
	PM_CHARSET_CP856				,
	PM_CHARSET_CP922				,
	PM_CHARSET_CP943				,
	PM_CHARSET_CP1046				,
	PM_CHARSET_CP1124				,
	PM_CHARSET_CP1129				,
	PM_CHARSET_CP1161				,
	PM_CHARSET_CP1162				,
	PM_CHARSET_CP1163				,
};

enum {
	CHARSET_US_ASCII				,
	CHARSET_UTF_8				,
	CHARSET_UCS_2				,
	CHARSET_UCS_2BE				,
	CHARSET_UCS_2LE				,
	CHARSET_UCS_4				,
	CHARSET_UCS_4BE				,
	CHARSET_UCS_4LE				,
	CHARSET_UTF_16				,
	CHARSET_UTF_16BE				,
	CHARSET_UTF_16LE				,
	CHARSET_UTF_32				,
	CHARSET_UTF_32BE				,
	CHARSET_UTF_32LE				,
	CHARSET_UTF_7				,
	CHARSET_UCS_2_INTERNAL		,
	CHARSET_UCS_2_SWAPPED		,
	CHARSET_UCS_4_INTERNAL		,
	CHARSET_UCS_4_SWAPPED		,
	CHARSET_C99					,
	CHARSET_JAVA					,
	CHARSET_ISO_8859_1			,
	CHARSET_ISO_8859_2			,
	CHARSET_ISO_8859_3			,
	CHARSET_ISO_8859_4			,
	CHARSET_ISO_8859_5			,
	CHARSET_ISO_8859_6			,
	CHARSET_ISO_8859_7			,
	CHARSET_ISO_8859_8			,
	CHARSET_ISO_8859_9			,
	CHARSET_ISO_8859_10			,
	CHARSET_ISO_8859_11			,
	CHARSET_ISO_8859_13			,
	CHARSET_ISO_8859_14			,
	CHARSET_ISO_8859_15			,
	CHARSET_ISO_8859_16			,
	CHARSET_KOI8_R				,
	CHARSET_KOI8_U				,
	CHARSET_KOI8_RU				,
	CHARSET_CP1250				,
	CHARSET_CP1251				,
	CHARSET_CP1252				,
	CHARSET_CP1253				,
	CHARSET_CP1254				,
	CHARSET_CP1255				,
	CHARSET_CP1256				,
	CHARSET_CP1257				,
	CHARSET_CP1258				,
	CHARSET_CP850				,
	CHARSET_CP862				,
	CHARSET_CP866				,
	CHARSET_MacRoman			,
	CHARSET_MacCentralEurope		,
	CHARSET_MacIceland			,
	CHARSET_MacCroatian			,
	CHARSET_MacRomania			,
	CHARSET_MacCyrillic			,
	CHARSET_MacUkraine			,
	CHARSET_MacGreek				,
	CHARSET_MacTurkish			,
	CHARSET_MacHebrew			,
	CHARSET_MacArabic			,
	CHARSET_MacThai				,
	CHARSET_HP_ROMAN8			,
	CHARSET_NEXTSTEP				,
	CHARSET_ARMSCII_8			,
	CHARSET_GEORGIAN_ACADEMY		,
	CHARSET_GEORGIAN_PS			,
	CHARSET_KOI8_T				,
	CHARSET_PT154				,
	CHARSET_MULELAO_1			,
	CHARSET_CP1133				,
	CHARSET_TIS_620				,
	CHARSET_CP874				,
	CHARSET_TCVN					,
	CHARSET_JIS_C6220_1969_RO	,
	CHARSET_JIS_X0201			,
	CHARSET_JIS_X0208			,
	CHARSET_JIS_X0212			,
	CHARSET_GB_1988_80			,
	CHARSET_GB_2312_80			,
	CHARSET_ISO_IR_165			,
	CHARSET_KSC_5601				,
	CHARSET_EUC_JP				,
	CHARSET_SHIFT_JIS			,
	CHARSET_CP932				,
	CHARSET_ISO_2022_JP			,
	CHARSET_ISO_2022_JP_1		,
	CHARSET_ISO_2022_JP_2		,
	CHARSET_GB2312				,
	CHARSET_GBK					,
	CHARSET_CP936				,
	CHARSET_GB18030				,
	CHARSET_ISO_2022_CN			,
	CHARSET_ISO_2022_CN_EXT		,
	CHARSET_HZ					,
	CHARSET_EUC_TW				,
	CHARSET_BIG5					,
	CHARSET_CP950				,
	CHARSET_BIG5_HKSCS_1999		,
	CHARSET_BIG5_HKSCS_2001		,
	CHARSET_BIG5_HKSCS			,
	CHARSET_EUC_KR				,
	CHARSET_CP949				,
	CHARSET_JOHAB				,
	CHARSET_ISO_2022_KR			,
	CHARSET_EUC_JISX0213			,
	CHARSET_SHIFT_JISX0213		,
	CHARSET_ISO_2022_JP_3		,
	CHARSET_BIG5_2003			,
	CHARSET_TDS565				,
	CHARSET_ATARIST				,
	CHARSET_RISCOS_LATIN1		,
	CHARSET_DEC_KANJI			,
	CHARSET_DEC_HANYU			,
	CHARSET_CP437				,
	CHARSET_CP737				,
	CHARSET_CP775				,
	CHARSET_CP852				,
	CHARSET_CP853				,
	CHARSET_CP855				,
	CHARSET_CP857				,
	CHARSET_CP858				,
	CHARSET_CP860				,
	CHARSET_CP861				,
	CHARSET_CP863				,
	CHARSET_CP864				,
	CHARSET_CP865				,
	CHARSET_CP869				,
	CHARSET_CP1125				,
	CHARSET_CP856				,
	CHARSET_CP922				,
	CHARSET_CP943				,
	CHARSET_CP1046				,
	CHARSET_CP1124				,
	CHARSET_CP1129				,
	CHARSET_CP1161				,
	CHARSET_CP1162				,
	CHARSET_CP1163				,
};

const char* PM_GUESS_CHARSETS[] =
{
	PM_CHARSET_GB2312			,
	PM_CHARSET_UTF8				,
	PM_CHARSET_GBK				,
	PM_CHARSET_BIG5				,
	PM_CHARSET_EUC_TW			,
	PM_CHARSET_SHIFT_JIS		,
	PM_CHARSET_EUC_JP			,
	PM_CHARSET_EUC_KR			,
	PM_CHARSET_GB18030			
//	PM_CHARSET_ISO_8859_1		,
};

const int PM_GUESS_CHARSETS_NUM = sizeof(PM_GUESS_CHARSETS)/sizeof(const char*);

static struct charset_mapping_table bytestr_charset_table[]={
/*----------encodings_basic----------*/

{CHARSET_US_ASCII,  PM_CHARSET_US_ASCII},               /* IANA */
{CHARSET_US_ASCII,  "ASCII"},                  /* IANA, JDK 1.1 */
{CHARSET_US_ASCII,  "ISO646-US"},              /* IANA */
{CHARSET_US_ASCII,  "ISO_646.IRV:1991"},       /* IANA */
{CHARSET_US_ASCII,  "ISO-IR-6"},               /* IANA */
{CHARSET_US_ASCII,  "ANSI_X3.4-1968"},         /* IANA */
{CHARSET_US_ASCII,  "ANSI_X3.4-1986"},         /* IANA */
{CHARSET_US_ASCII,  "CP367"},                  /* IANA */
{CHARSET_US_ASCII,  "IBM367"},                 /* IANA */
{CHARSET_US_ASCII,  "US"},                     /* IANA */
{CHARSET_US_ASCII,  "csASCII"},                /* IANA */
/*"ISO646.1991-IRV",           X11R6.4 */

/* General multi-byte encodings */

{CHARSET_UTF_8,  PM_CHARSET_UTF8},                  /* IANA, RFC 2279 */
{CHARSET_UTF_8,  "UTF8"},                   /* JDK 1.1 */ /*Added*/
/*"CP65001",                   Windows */

{CHARSET_UCS_2,  PM_CHARSET_UCS_2},                  /* glibc */
{CHARSET_UCS_2,  "UCS2"},                   /* pushmail */ /*Added*/
{CHARSET_UCS_2,  "ISO-10646-UCS-2"},        /* IANA */
{CHARSET_UCS_2,  "csUnicode"},              /* IANA */

{CHARSET_UCS_2BE,  PM_CHARSET_UCS_2BE},                /* glibc */
{CHARSET_UCS_2BE,  "UNICODEBIG"},             /* glibc */
{CHARSET_UCS_2BE,  "UNICODE-1-1"},            /* IANA */
{CHARSET_UCS_2BE,  "csUnicode11"},            /* IANA */
/*"CP1201",                    Windows */

{CHARSET_UCS_2LE,  PM_CHARSET_UCS_2LE},                /* glibc */
{CHARSET_UCS_2LE,  "UNICODELITTLE"},          /* glibc */
/*"CP1200",                    Windows */

{CHARSET_UCS_4,  PM_CHARSET_UCS_4},                  /* glibc */
{CHARSET_UCS_4,  "ISO-10646-UCS-4"},        /* IANA */
{CHARSET_UCS_4,  "X-ISO-10646-UCS-4-2143"}, /* libcharguess */ /*Added*/
{CHARSET_UCS_4,  "X-ISO-10646-UCS-4-3412"}, /* libcharguess */ /*Added*/
{CHARSET_UCS_4,  "csUCS4"},                 /* IANA */

{CHARSET_UCS_4BE,  PM_CHARSET_UCS_4BE},                /* glibc */
/*"CP12001",                   Windows */

{CHARSET_UCS_4LE,  PM_CHARSET_UCS_4LE},                /* glibc */
/*"CP12000",                   Windows */

{CHARSET_UTF_16,  PM_CHARSET_UTF_16},                 /* IANA, RFC 2781 */

{CHARSET_UTF_16BE,  PM_CHARSET_UTF_16BE},               /* IANA, RFC 2781 */

{CHARSET_UTF_16LE,  PM_CHARSET_UTF_16LE},               /* IANA, RFC 2781 */

{CHARSET_UTF_32,  PM_CHARSET_UTF_32},                 /* IANA, Unicode 3.1 */

{CHARSET_UTF_32BE,  PM_CHARSET_UTF_32BE},               /* IANA, Unicode 3.1 */

{CHARSET_UTF_32LE,  PM_CHARSET_UTF_32LE},               /* IANA, Unicode 3.1 */

{CHARSET_UTF_7,  PM_CHARSET_UTF_7},                  /* IANA, RFC 2152 */
{CHARSET_UTF_7,  "UNICODE-1-1-UTF-7"},      /* IANA, RFC 1642 */
{CHARSET_UTF_7,  "csUnicode11UTF7"},        /* IANA */
/*"CP65000",                   Windows */

{CHARSET_UCS_2_INTERNAL,  "UCS-2-INTERNAL"},         /* libiconv */

{CHARSET_UCS_2_SWAPPED,  "UCS-2-SWAPPED"},          /* libiconv */

{CHARSET_UCS_4_INTERNAL,  "UCS-4-INTERNAL"},         /* libiconv */

{CHARSET_UCS_4_SWAPPED,  "UCS-4-SWAPPED"},          /* libiconv */

{CHARSET_C99,  PM_CHARSET_C99},

{CHARSET_JAVA,  PM_CHARSET_JAVA},

/* Standard 8-bit encodings */

{CHARSET_ISO_8859_1,  PM_CHARSET_ISO_8859_1},             /* IANA */
{CHARSET_ISO_8859_1,  "ISO_8859-1"},             /* IANA */
{CHARSET_ISO_8859_1,  "ISO_8859-1:1987"},        /* IANA */
{CHARSET_ISO_8859_1,  "ISO-IR-100"},             /* IANA */
{CHARSET_ISO_8859_1,  "CP819"},                  /* IANA */
{CHARSET_ISO_8859_1,  "IBM819"},                 /* IANA */
{CHARSET_ISO_8859_1,  "LATIN1"},                 /* IANA */
{CHARSET_ISO_8859_1,  "L1"},                     /* IANA */
{CHARSET_ISO_8859_1,  "csISOLatin1"},            /* IANA */
{CHARSET_ISO_8859_1,  "ISO8859-1"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_1,  "ISO8859_1"},              /* JDK 1.1 */ /*Added*/
/*"CP28591",                   Windows */

{CHARSET_ISO_8859_2,  PM_CHARSET_ISO_8859_2},             /* IANA */
{CHARSET_ISO_8859_2,  "ISO_8859-2"},             /* IANA */
{CHARSET_ISO_8859_2,  "ISO_8859-2:1987"},        /* IANA */
{CHARSET_ISO_8859_2,  "ISO-IR-101"},             /* IANA */
{CHARSET_ISO_8859_2,  "LATIN2"},                 /* IANA */
{CHARSET_ISO_8859_2,  "L2"},                     /* IANA */
{CHARSET_ISO_8859_2,  "csISOLatin2"},            /* IANA */
{CHARSET_ISO_8859_2,  "ISO8859-2"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_2,  "ISO8859_2"},              /* JDK 1.1 */ /*Added*/
/*"CP28592",                   Windows */

{CHARSET_ISO_8859_3,  PM_CHARSET_ISO_8859_3},             /* IANA */
{CHARSET_ISO_8859_3,  "ISO_8859-3"},             /* IANA */
{CHARSET_ISO_8859_3,  "ISO_8859-3:1988"},        /* IANA */
{CHARSET_ISO_8859_3,  "ISO-IR-109"},             /* IANA */
{CHARSET_ISO_8859_3,  "LATIN3"},                 /* IANA */
{CHARSET_ISO_8859_3,  "L3"},                     /* IANA */
{CHARSET_ISO_8859_3,  "csISOLatin3"},            /* IANA */
{CHARSET_ISO_8859_3,  "ISO8859-3"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_3,  "ISO8859_3"},              /* JDK 1.1 */ /*Added*/
/*"CP28593",                   Windows */

{CHARSET_ISO_8859_4,  PM_CHARSET_ISO_8859_4},             /* IANA */
{CHARSET_ISO_8859_4,  "ISO_8859-4"},             /* IANA */
{CHARSET_ISO_8859_4,  "ISO_8859-4:1988"},        /* IANA */
{CHARSET_ISO_8859_4,  "ISO-IR-110"},             /* IANA */
{CHARSET_ISO_8859_4,  "LATIN4"},                 /* IANA */
{CHARSET_ISO_8859_4,  "L4"},                     /* IANA */
{CHARSET_ISO_8859_4,  "csISOLatin4"},            /* IANA */
{CHARSET_ISO_8859_4,  "ISO8859-4"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_4,  "ISO8859_4"},              /* JDK 1.1 *//*Added*/
/*"CP28594",                   Windows */

{CHARSET_ISO_8859_5,  PM_CHARSET_ISO_8859_5},             /* IANA */
{CHARSET_ISO_8859_5,  "ISO_8859-5"},             /* IANA */
{CHARSET_ISO_8859_5,  "ISO_8859-5:1988"},        /* IANA */
{CHARSET_ISO_8859_5,  "ISO-IR-144"},             /* IANA */
{CHARSET_ISO_8859_5,  "CYRILLIC"},               /* IANA */
{CHARSET_ISO_8859_5,  "csISOLatinCyrillic"},     /* IANA */
{CHARSET_ISO_8859_5,  "ISO8859-5"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_5,  "ISO8859_5"},              /* JDK 1.1 *//*Added*/
/*"CP28595",                   Windows */

{CHARSET_ISO_8859_6,  PM_CHARSET_ISO_8859_6},             /* IANA */
{CHARSET_ISO_8859_6,  "ISO_8859-6"},             /* IANA */
{CHARSET_ISO_8859_6,  "ISO_8859-6:1987"},        /* IANA */
{CHARSET_ISO_8859_6,  "ISO-IR-127"},             /* IANA */
{CHARSET_ISO_8859_6,  "ECMA-114"},               /* IANA */
{CHARSET_ISO_8859_6,  "ASMO-708"},               /* IANA */
{CHARSET_ISO_8859_6,  "ARABIC"},                 /* IANA */
{CHARSET_ISO_8859_6,  "csISOLatinArabic"},       /* IANA */
{CHARSET_ISO_8859_6,  "ISO8859-6"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_6,  "ISO8859_6"},              /* JDK 1.1 *//*Added*/
/*"CP28596",                   Windows */

{CHARSET_ISO_8859_7,  PM_CHARSET_ISO_8859_7},             /* IANA, RFC 1947 */
{CHARSET_ISO_8859_7,  "ISO_8859-7"},             /* IANA */
{CHARSET_ISO_8859_7,  "ISO_8859-7:1987"},        /* IANA */
{CHARSET_ISO_8859_7,  "ISO_8859-7:2003"},
{CHARSET_ISO_8859_7,  "ISO-IR-126"},             /* IANA */
{CHARSET_ISO_8859_7,  "ECMA-118"},               /* IANA */
{CHARSET_ISO_8859_7,  "ELOT_928"},               /* IANA */
{CHARSET_ISO_8859_7,  "GREEK8"},                 /* IANA */
{CHARSET_ISO_8859_7,  "GREEK"},                  /* IANA */
{CHARSET_ISO_8859_7,  "csISOLatinGreek"},        /* IANA */
{CHARSET_ISO_8859_7,  "ISO8859-7"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_7,  "ISO8859_7"},              /* JDK 1.1 *//*Added*/
/*"CP28597",                   Windows */

{CHARSET_ISO_8859_8,  PM_CHARSET_ISO_8859_8},             /* IANA */
{CHARSET_ISO_8859_8,  "ISO_8859-8"},             /* IANA */
{CHARSET_ISO_8859_8,  "ISO_8859-8:1988"},        /* IANA */
{CHARSET_ISO_8859_8,  "ISO-IR-138"},             /* IANA */
{CHARSET_ISO_8859_8,  "HEBREW"},                 /* IANA */
{CHARSET_ISO_8859_8,  "csISOLatinHebrew"},       /* IANA */
{CHARSET_ISO_8859_8,  "ISO8859-8"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_8,  "ISO8859_8"},              /* JDK 1.1 *//*Added*/
/*"CP28598",                   Windows */
/*"CP38598",                   Windows */

{CHARSET_ISO_8859_9,  PM_CHARSET_ISO_8859_9},             /* IANA */
{CHARSET_ISO_8859_9,  "ISO_8859-9"},             /* IANA */
{CHARSET_ISO_8859_9,  "ISO_8859-9:1989"},        /* IANA */
{CHARSET_ISO_8859_9,  "ISO-IR-148"},             /* IANA */
{CHARSET_ISO_8859_9,  "LATIN5"},                 /* IANA */
{CHARSET_ISO_8859_9,  "L5"},                     /* IANA */
{CHARSET_ISO_8859_9,  "csISOLatin5"},            /* IANA */
{CHARSET_ISO_8859_9,  "ISO8859-9"},              /* X11R6.4, glibc, FreeBSD */
{CHARSET_ISO_8859_9,  "ISO8859_9"},              /* JDK 1.1 *//*Added*/
/*"CP28599",                   Windows */

{CHARSET_ISO_8859_10,  PM_CHARSET_ISO_8859_10},            /* IANA */
{CHARSET_ISO_8859_10,  "ISO_8859-10"},
{CHARSET_ISO_8859_10,  "ISO_8859-10:1992"},       /* IANA */
{CHARSET_ISO_8859_10,  "ISO-IR-157"},             /* IANA */
{CHARSET_ISO_8859_10,  "LATIN6"},                 /* IANA */
{CHARSET_ISO_8859_10,  "L6"},                     /* IANA */
{CHARSET_ISO_8859_10,  "csISOLatin6"},            /* IANA */
{CHARSET_ISO_8859_10,  "ISO8859-10"},             /* X11R6.4, glibc, FreeBSD */

{CHARSET_ISO_8859_11,  PM_CHARSET_ISO_8859_11},            /* glibc */
{CHARSET_ISO_8859_11,  "ISO_8859-11"},
{CHARSET_ISO_8859_11,  "ISO8859-11"},             /* X11R6.7, glibc */

{CHARSET_ISO_8859_13,  PM_CHARSET_ISO_8859_13},            /* IANA, glibc */
{CHARSET_ISO_8859_13,  "ISO_8859-13"},
{CHARSET_ISO_8859_13,  "ISO-IR-179"},             /* glibc */
{CHARSET_ISO_8859_13,  "LATIN7"},                 /* glibc */
{CHARSET_ISO_8859_13,  "L7"},                     /* glibc */
{CHARSET_ISO_8859_13,  "ISO8859-13"},             /* glibc, FreeBSD */

{CHARSET_ISO_8859_14,  PM_CHARSET_ISO_8859_14},            /* IANA, glibc */
{CHARSET_ISO_8859_14,  "ISO_8859-14"},            /* IANA */
{CHARSET_ISO_8859_14,  "ISO_8859-14:1998"},       /* IANA, glibc */
{CHARSET_ISO_8859_14,  "ISO-IR-199"},             /* IANA */
{CHARSET_ISO_8859_14,  "LATIN8"},                 /* IANA, glibc */
{CHARSET_ISO_8859_14,  "L8"},                     /* IANA, glibc */
{CHARSET_ISO_8859_14,  "ISO-CELTIC"},             /* IANA */
{CHARSET_ISO_8859_14,  "ISO8859-14"},             /* glibc, FreeBSD */

{CHARSET_ISO_8859_15,  PM_CHARSET_ISO_8859_15},            /* IANA, glibc */
{CHARSET_ISO_8859_15,  "ISO_8859-15"},            /* IANA */
{CHARSET_ISO_8859_15,  "ISO_8859-15:1998"},       /* glibc */
{CHARSET_ISO_8859_15,  "ISO-IR-203"},
{CHARSET_ISO_8859_15,  "LATIN-9"},                /* IANA */
{CHARSET_ISO_8859_15,  "ISO8859-15"},             /* glibc, FreeBSD */
/*"CP28605",                   Windows */

{CHARSET_ISO_8859_16,  PM_CHARSET_ISO_8859_16},            /* IANA */
{CHARSET_ISO_8859_16,  "ISO_8859-16"},            /* IANA */
{CHARSET_ISO_8859_16,  "ISO_8859-16:2001"},       /* IANA */
{CHARSET_ISO_8859_16,  "ISO-IR-226"},             /* IANA */
{CHARSET_ISO_8859_16,  "LATIN10"},                /* IANA */
{CHARSET_ISO_8859_16,  "L10"},                    /* IANA */
{CHARSET_ISO_8859_16,  "ISO8859-16"},             /* glibc, FreeBSD */

{CHARSET_KOI8_R,  PM_CHARSET_KOI8_R},                 /* IANA, RFC 1489, X11R6.4, JDK 1.1 */
{CHARSET_KOI8_R,  "csKOI8R"},                /* IANA */
/*"CP20866",                   Windows */

{CHARSET_KOI8_U,  PM_CHARSET_KOI8_U},                 /* IANA, RFC 2319 */

{CHARSET_KOI8_RU,  PM_CHARSET_KOI8_RU},

/* Windows 8-bit encodings */

{CHARSET_CP1250,  PM_CHARSET_CP1250},                 /* JDK 1.1 */
{CHARSET_CP1250,  "WINDOWS-1250"},           /* IANA */
{CHARSET_CP1250,  "MS-EE"},

{CHARSET_CP1251,  PM_CHARSET_CP1251},                 /* JDK 1.1 */
{CHARSET_CP1251,  "WINDOWS-1251"},           /* IANA */
{CHARSET_CP1251,  "MS-CYRL"},

{CHARSET_CP1252,  PM_CHARSET_CP1252},                 /* JDK 1.1 */
{CHARSET_CP1252,  "WINDOWS-1252"},           /* IANA */
{CHARSET_CP1252,  "MS-ANSI"},

{CHARSET_CP1253,  PM_CHARSET_CP1253},                 /* JDK 1.1 */
{CHARSET_CP1253,  "WINDOWS-1253"},           /* IANA */
{CHARSET_CP1253,  "MS-GREEK"},

{CHARSET_CP1254,  PM_CHARSET_CP1254},                 /* JDK 1.1 */
{CHARSET_CP1254,  "WINDOWS-1254"},           /* IANA */
{CHARSET_CP1254,  "MS-TURK"},

{CHARSET_CP1255,  PM_CHARSET_CP1255},                 /* JDK 1.1 */
{CHARSET_CP1255,  "WINDOWS-1255"},           /* IANA */
{CHARSET_CP1255,  "MS-HEBR"},

{CHARSET_CP1256,  PM_CHARSET_CP1256},                 /* JDK 1.1 */
{CHARSET_CP1256,  "WINDOWS-1256"},           /* IANA */
{CHARSET_CP1256,  "MS-ARAB"},

{CHARSET_CP1257,  PM_CHARSET_CP1257},                 /* JDK 1.1 */
{CHARSET_CP1257,  "WINDOWS-1257"},           /* IANA */
{CHARSET_CP1257,  "WINBALTRIM"},

{CHARSET_CP1258,  PM_CHARSET_CP1258},                 /* JDK 1.1 */
{CHARSET_CP1258,  "WINDOWS-1258"},           /* IANA */

/* DOS 8-bit encodings */

{CHARSET_CP850,  PM_CHARSET_CP850},                  /* IANA, JDK 1.1 */
{CHARSET_CP850,  "IBM850"},                 /* IANA */
{CHARSET_CP850,  "850"},                    /* IANA */
{CHARSET_CP850,  "csPC850Multilingual"},    /* IANA */

{CHARSET_CP862,  PM_CHARSET_CP862},                  /* IANA, JDK 1.1 */
{CHARSET_CP862,  "IBM862"},                 /* IANA */
{CHARSET_CP862,  "862"},                    /* IANA */
{CHARSET_CP862,  "csPC862LatinHebrew"},     /* IANA */

{CHARSET_CP866,  PM_CHARSET_CP866},                  /* IANA, JDK 1.1 */
{CHARSET_CP866,  "IBM866"},                 /* IANA */
{CHARSET_CP866,  "866"},                    /* IANA */
{CHARSET_CP866,  "csIBM866"},               /* IANA */

/* Macintosh 8-bit encodings */

{CHARSET_MacRoman,  PM_CHARSET_MacRoman},               /* JDK 1.1 */
  /* This is the best table for MACINTOSH. The ones */
  /* in glibc and FreeBSD-iconv are bad quality. */
{CHARSET_MacRoman,  "MACINTOSH"},              /* IANA */
{CHARSET_MacRoman,  "MAC"},                    /* IANA */
{CHARSET_MacRoman,  "csMacintosh"},            /* IANA */
/*"CP10000",                   Windows */

{CHARSET_MacCentralEurope,  PM_CHARSET_MacCentralEurope},       /* JDK 1.1 */
/*"CP10029",                   Windows */

{CHARSET_MacIceland,  PM_CHARSET_MacIceland},             /* JDK 1.1 */
/*"CP10079",                   Windows */

{CHARSET_MacCroatian,  PM_CHARSET_MacCroatian},            /* JDK 1.1 */
/*"CP10082",                   Windows */

{CHARSET_MacRomania,  PM_CHARSET_MacRomania},             /* JDK 1.1 */
/*"CP10010",                   Windows */

{CHARSET_MacCyrillic,  PM_CHARSET_MacCyrillic},            /* JDK 1.1 */
{CHARSET_MacCyrillic,  "x-mac-cyrillic"},          /* libcharguess */ /*Added*/
/*"CP10007",                   Windows */

{CHARSET_MacUkraine,  PM_CHARSET_MacUkraine},             /* JDK 1.1 */
/*"CP10017",                   Windows */

{CHARSET_MacGreek,  PM_CHARSET_MacGreek},               /* JDK 1.1 */
/*"CP10006",                   Windows */

{CHARSET_MacTurkish,  PM_CHARSET_MacTurkish},             /* JDK 1.1 */
/*"CP10081",                   Windows */

{CHARSET_MacHebrew,  PM_CHARSET_MacHebrew},              /* JDK 1.1 */
/*"CP10005",                   Windows */

{CHARSET_MacArabic,  PM_CHARSET_MacArabic},              /* JDK 1.1 */
/*"CP10004",                   Windows */

{CHARSET_MacThai,  PM_CHARSET_MacThai},                /* JDK 1.1 */
/*"CP10021",                   Windows */

/* Other platform specific 8-bit encodings */

{CHARSET_HP_ROMAN8,  PM_CHARSET_HP_ROMAN8},              /* IANA, X11R6.4 */
{CHARSET_HP_ROMAN8,  "ROMAN8"},                 /* IANA */
{CHARSET_HP_ROMAN8,  "R8"},                     /* IANA */
{CHARSET_HP_ROMAN8,  "csHPRoman8"},             /* IANA */

{CHARSET_NEXTSTEP,  PM_CHARSET_NEXTSTEP},

/* Regional 8-bit encodings used for a single language */

{CHARSET_ARMSCII_8,  PM_CHARSET_ARMSCII_8},

{CHARSET_GEORGIAN_ACADEMY,  PM_CHARSET_GEORGIAN_ACADEMY},

{CHARSET_GEORGIAN_PS,  PM_CHARSET_GEORGIAN_PS},

{CHARSET_KOI8_T,  PM_CHARSET_KOI8_T},

{CHARSET_PT154,  PM_CHARSET_PT154},                  /* IANA, glibc */
{CHARSET_PT154,  "PTCP154"},                /* IANA */
{CHARSET_PT154,  "CP154"},                  /* IANA */
{CHARSET_PT154,  "CYRILLIC-ASIAN"},         /* IANA */
{CHARSET_PT154,  "csPTCP154"},              /* IANA */

{CHARSET_MULELAO_1,  PM_CHARSET_MULELAO_1},

{CHARSET_CP1133,  PM_CHARSET_CP1133},
{CHARSET_CP1133,  "IBM-CP1133"},

{CHARSET_TIS_620,  PM_CHARSET_TIS_620},                /* IANA */
{CHARSET_TIS_620,  "TIS620"},                 /* glibc */
{CHARSET_TIS_620,  "TIS620-0"},               /* glibc */
{CHARSET_TIS_620,  "TIS620.2529-1"},          /* glibc */
{CHARSET_TIS_620,  "TIS620.2533-0"},          /* glibc */
{CHARSET_TIS_620,  "TIS620.2533-1"},
{CHARSET_TIS_620,  "ISO-IR-166"},             /* glibc */

{CHARSET_CP874,  "CP874"},                  /* JDK 1.1 */
{CHARSET_CP874,  "WINDOWS-874"},
{CHARSET_CP874,  "VISCII"},                 /* IANA, RFC 1456 */
{CHARSET_CP874,  "VISCII1.1-1"},
{CHARSET_CP874,  "csVISCII"},               /* IANA */

{CHARSET_TCVN,  PM_CHARSET_TCVN},
{CHARSET_TCVN,  "TCVN-5712"},
{CHARSET_TCVN,  "TCVN5712-1"},
{CHARSET_TCVN,  "TCVN5712-1:1993"},

/* CJK character sets (not documented) */

{CHARSET_JIS_C6220_1969_RO,  PM_CHARSET_JIS_C6220_1969_RO},      /* IANA */
{CHARSET_JIS_C6220_1969_RO,  "ISO646-JP"},              /* IANA */
{CHARSET_JIS_C6220_1969_RO,  "ISO-IR-14"},              /* IANA */
{CHARSET_JIS_C6220_1969_RO,  "JP"},                     /* IANA */
{CHARSET_JIS_C6220_1969_RO,  "csISO14JISC6220ro"},      /* IANA */

{CHARSET_JIS_X0201,  PM_CHARSET_JIS_X0201},              /* IANA */
{CHARSET_JIS_X0201,  "JISX0201-1976"},
{CHARSET_JIS_X0201,  "X0201"},                  /* IANA */
{CHARSET_JIS_X0201,  "csHalfWidthKatakana"},    /* IANA */
/*"JISX0201.1976-0",           X11R6.4 */
/*"JIS0201",                   JDK 1.1 */

{CHARSET_JIS_X0208,  PM_CHARSET_JIS_X0208},
{CHARSET_JIS_X0208,  "JIS_X0208-1983"},         /* IANA */
{CHARSET_JIS_X0208,  "JIS_X0208-1990"},
{CHARSET_JIS_X0208,  "JIS0208"},
{CHARSET_JIS_X0208,  "X0208"},                  /* IANA */
{CHARSET_JIS_X0208,  "ISO-IR-87"},              /* IANA */
{CHARSET_JIS_X0208,  "JIS_C6226-1983"},         /* IANA */
{CHARSET_JIS_X0208,  "csISO87JISX0208"},        /* IANA */
/*"JISX0208.1983-0",           X11R6.4 */
/*"JISX0208.1990-0",           X11R6.4 */
/*"JIS0208",                   JDK 1.1 */

{CHARSET_JIS_X0212,  PM_CHARSET_JIS_X0212},
{CHARSET_JIS_X0212,  "JIS_X0212.1990-0"},
{CHARSET_JIS_X0212,  "JIS_X0212-1990"},         /* IANA */
{CHARSET_JIS_X0212,  "X0212"},                  /* IANA */
{CHARSET_JIS_X0212,  "ISO-IR-159"},             /* IANA */
{CHARSET_JIS_X0212,  "csISO159JISX02121990"},   /* IANA */
/*"JISX0212.1990-0",           X11R6.4 */
/*"JIS0212",                   JDK 1.1 */

{CHARSET_GB_1988_80,  PM_CHARSET_GB_1988_80},             /* IANA */
{CHARSET_GB_1988_80,  "ISO646-CN"},              /* IANA */
{CHARSET_GB_1988_80,  "ISO-IR-57"},              /* IANA */
{CHARSET_GB_1988_80,  "CN"},                     /* IANA */
{CHARSET_GB_1988_80,  "csISO57GB1988"},          /* IANA */

{CHARSET_GB_2312_80,  PM_CHARSET_GB_2312_80},             /* IANA */
{CHARSET_GB_2312_80,  "ISO-IR-58"},              /* IANA */
{CHARSET_GB_2312_80,  "csISO58GB231280"},        /* IANA */
{CHARSET_GB_2312_80,  "CHINESE"},                /* IANA */
/*"GB2312.1980-0",             X11R6.4 */

{CHARSET_ISO_IR_165,  PM_CHARSET_ISO_IR_165},
{CHARSET_ISO_IR_165,  "CN-GB-ISOIR165"},         /* RFC 1922 */

{CHARSET_KSC_5601,  PM_CHARSET_KSC_5601},               /* IANA */
{CHARSET_KSC_5601,  "KS_C_5601-1987"},         /* IANA */
{CHARSET_KSC_5601,  "KS_C_5601-1989"},         /* IANA */
{CHARSET_KSC_5601,  "ISO-IR-149"},             /* IANA */
{CHARSET_KSC_5601,  "csKSC56011987"},          /* IANA */
{CHARSET_KSC_5601,  "KOREAN"},                 /* IANA */
/*"KSC5601.1987-0",            X11R6.4 */
/*"KSX1001:1992",              Ken Lunde */

/* CJK encodings */

{CHARSET_EUC_JP,  PM_CHARSET_EUC_JP},                 /* IANA */
{CHARSET_EUC_JP,  "EUCJP"},                  /* glibc */
{CHARSET_EUC_JP,  "Extended_UNIX_Code_Packed_Format_for_Japanese"}, /* IANA */
{CHARSET_EUC_JP,  "csEUCPkdFmtJapanese"},    /* IANA */
/*"EUC_JP",                    JDK 1.1 */
/*"CP51932",                   Windows */

{CHARSET_SHIFT_JIS,  PM_CHARSET_SHIFT_JIS},              /* IANA */
{CHARSET_SHIFT_JIS,  "SHIFT-JIS"},              /* glibc */
{CHARSET_SHIFT_JIS,  "SJIS"},                   /* JDK 1.1 */
{CHARSET_SHIFT_JIS,  "MS_KANJI"},               /* IANA */
{CHARSET_SHIFT_JIS,  "csShiftJIS"},             /* IANA */

{CHARSET_CP932,  PM_CHARSET_CP932},                  /* glibc */

{CHARSET_ISO_2022_JP,  PM_CHARSET_ISO_2022_JP},            /* IANA, RFC 1468 */
{CHARSET_ISO_2022_JP,  "csISO2022JP"},            /* IANA */
/*"ISO2022JP",                 JDK 1.1 */

{CHARSET_ISO_2022_JP_1,  PM_CHARSET_ISO_2022_JP_1},          /* RFC 2237 */

{CHARSET_ISO_2022_JP_2,  PM_CHARSET_ISO_2022_JP_2},          /* IANA, RFC 1554 */
{CHARSET_ISO_2022_JP_2,  "csISO2022JP2"},           /* IANA */

{CHARSET_GB2312,  PM_CHARSET_GB2312},                 /* IANA */
{CHARSET_GB2312,  "EUC-CN"},                 /* glibc */
{CHARSET_GB2312,  "EUCCN"},                  /* glibc */
{CHARSET_GB2312,  "GB-2312"},                /* pushmail */ /*Added*/
{CHARSET_GB2312,  "CN-GB"},                  /* RFC 1922 */
{CHARSET_GB2312,  "csGB2312"},               /* IANA */
/*"EUC_CN",                    JDK 1.1 */
/*"CP51936",                   Windows */

{CHARSET_GBK,  PM_CHARSET_GBK},                    /* IANA, JDK 1.1 */

{CHARSET_CP936,  PM_CHARSET_CP936},                  /* IANA */
{CHARSET_CP936,  "MS936"},                  /* IANA */
{CHARSET_CP936,  "WINDOWS-936"},            /* IANA */

{CHARSET_GB18030,  PM_CHARSET_GB18030},                /* IANA, glibc */
/*"CP54936",                   Windows */

{CHARSET_ISO_2022_CN,  PM_CHARSET_ISO_2022_CN},            /* IANA, RFC 1922 */
{CHARSET_ISO_2022_CN,  "csISO2022CN"},
/*"ISO2022CN",                 JDK 1.1 */

{CHARSET_ISO_2022_CN_EXT,  PM_CHARSET_ISO_2022_CN_EXT},        /* IANA, RFC 1922 */

{CHARSET_HZ,  PM_CHARSET_HZ},                     /* RFC 1843 */
{CHARSET_HZ,  "HZ-GB-2312"},             /* IANA, RFC 1842 */

{CHARSET_EUC_TW,  PM_CHARSET_EUC_TW},                 /* glibc */
{CHARSET_EUC_TW,  "x-euc-tw"},                /* libcharguess */ /*Added*/
{CHARSET_EUC_TW,  "EUCTW"},                  /* glibc */
{CHARSET_EUC_TW,  "csEUCTW"},
/*"EUC_TW",                    JDK 1.1 */
/*"CP51950",                   Windows */

{CHARSET_BIG5,  PM_CHARSET_BIG5},                   /* IANA, JDK 1.1 */
{CHARSET_BIG5,  "BIG-5"},                  /* glibc */
{CHARSET_BIG5,  "BIG-FIVE"},               /* glibc */
{CHARSET_BIG5,  "BIGFIVE"},                /* glibc */
{CHARSET_BIG5,  "CN-BIG5"},                /* RFC 1922 */
{CHARSET_BIG5,  "csBig5"},                 /* IANA */

{CHARSET_CP950,  PM_CHARSET_CP950},                  /* JDK 1.1 */

{CHARSET_BIG5_HKSCS_1999,  PM_CHARSET_BIG5_HKSCS_1999},

{CHARSET_BIG5_HKSCS_2001,  PM_CHARSET_BIG5_HKSCS_2001},

{CHARSET_BIG5_HKSCS,  PM_CHARSET_BIG5_HKSCS},             /* IANA */
{CHARSET_BIG5_HKSCS,  "BIG5HKSCS"},              /* glibc */
{CHARSET_BIG5_HKSCS,  "BIG5-HKSCS:2004"},

{CHARSET_EUC_KR,  PM_CHARSET_EUC_KR},                 /* IANA, RFC 1557 */
{CHARSET_EUC_KR,  "EUCKR"},                  /* glibc */
{CHARSET_EUC_KR,  "csEUCKR"},                /* IANA */
/*"EUC_KR",                    JDK 1.1 */
/*"CP51949",                   Windows */

{CHARSET_CP949,  PM_CHARSET_CP949},                  /* JDK 1.1 */
{CHARSET_CP949,  "UHC"},                    /* glibc */

{CHARSET_JOHAB,  PM_CHARSET_JOHAB},                  /* glibc */
{CHARSET_JOHAB,  "CP1361"},                 /* glibc */

{CHARSET_ISO_2022_KR,  PM_CHARSET_ISO_2022_KR},            /* IANA, RFC 1557 */
{CHARSET_ISO_2022_KR,  "csISO2022KR"},            /* IANA */
/*"ISO2022KR",                 JDK 1.1 */

/*----------encodings_extra----------*/

{CHARSET_EUC_JISX0213,  PM_CHARSET_EUC_JISX0213},

{CHARSET_SHIFT_JISX0213,  PM_CHARSET_SHIFT_JISX0213},

{CHARSET_ISO_2022_JP_3,  PM_CHARSET_ISO_2022_JP_3},

{CHARSET_BIG5_2003,  PM_CHARSET_BIG5_2003},

{CHARSET_TDS565,  PM_CHARSET_TDS565},
{CHARSET_TDS565,  "ISO-IR-230"},

{CHARSET_ATARIST,  PM_CHARSET_ATARIST},
{CHARSET_ATARIST,  "ATARI"},

{CHARSET_RISCOS_LATIN1,  PM_CHARSET_RISCOS_LATIN1},
            
/*----------encodings_osf1----------*/
/* Encodings used by system dependent locales on OSF/1 a.k.a. Tru64. */

{CHARSET_DEC_KANJI,  PM_CHARSET_DEC_KANJI},
  
{CHARSET_DEC_HANYU,  PM_CHARSET_DEC_HANYU},
          
/*----------encodings_dos----------*/
/* Encodings used by system dependent locales on MSDOS. */

{CHARSET_CP437,  PM_CHARSET_CP437},                  /* IANA, JDK 1.1 */
{CHARSET_CP437,  "IBM437"},                 /* IANA */
{CHARSET_CP437,  "437"},                    /* IANA */
{CHARSET_CP437,  "csPC8CodePage437"},       /* IANA */

{CHARSET_CP737,  PM_CHARSET_CP737},                  /* JDK 1.1 */

{CHARSET_CP775,  PM_CHARSET_CP775},                  /* IANA, JDK 1.1 */
{CHARSET_CP775,  "IBM775"},                 /* IANA */
{CHARSET_CP775,  "csPC775Baltic"},          /* IANA */

{CHARSET_CP852,  PM_CHARSET_CP852},                  /* IANA, JDK 1.1 */
{CHARSET_CP852,  "IBM852"},                 /* IANA */
{CHARSET_CP852,  "852"},                    /* IANA */
{CHARSET_CP852,  "csPCp852"},               /* IANA */

{CHARSET_CP853,  PM_CHARSET_CP853},

{CHARSET_CP855,  PM_CHARSET_CP855},                  /* IANA, JDK 1.1 */
{CHARSET_CP855,  "IBM855"},                 /* IANA */
{CHARSET_CP855,  "855"},                    /* IANA */
{CHARSET_CP855,  "csIBM855"},               /* IANA */

{CHARSET_CP857,  PM_CHARSET_CP857},                  /* IANA, JDK 1.1 */
{CHARSET_CP857,  "IBM857"},                 /* IANA */
{CHARSET_CP857,  "857"},                    /* IANA */
{CHARSET_CP857,  "csIBM857"},               /* IANA */

{CHARSET_CP858,  PM_CHARSET_CP858},                  /* JDK 1.1.7 */

{CHARSET_CP860,  PM_CHARSET_CP860},                  /* IANA, JDK 1.1 */
{CHARSET_CP860,  "IBM860"},                 /* IANA */
{CHARSET_CP860,  "860"},                    /* IANA */
{CHARSET_CP860,  "csIBM860"},               /* IANA */

{CHARSET_CP861,  PM_CHARSET_CP861},                  /* IANA, JDK 1.1 */
{CHARSET_CP861,  "IBM861"},                 /* IANA */
{CHARSET_CP861,  "861"},                    /* IANA */
{CHARSET_CP861,  "CP-IS"},                  /* IANA */
{CHARSET_CP861,  "csIBM861"},               /* IANA */

{CHARSET_CP863,  PM_CHARSET_CP863},                  /* IANA, JDK 1.1 */
{CHARSET_CP863,  "IBM863"},                 /* IANA */
{CHARSET_CP863,  "863"},                    /* IANA */
{CHARSET_CP863,  "csIBM863"},               /* IANA */

{CHARSET_CP864,  PM_CHARSET_CP864},                  /* IANA, JDK 1.1 */
{CHARSET_CP864,  "IBM864"},                 /* IANA */
{CHARSET_CP864,  "csIBM864"},               /* IANA */

{CHARSET_CP865,  PM_CHARSET_CP865},                  /* IANA, JDK 1.1 */
{CHARSET_CP865,  "IBM865"},                 /* IANA */
{CHARSET_CP865,  "865"},                    /* IANA */
{CHARSET_CP865,  "csIBM865"},               /* IANA */

{CHARSET_CP869,  PM_CHARSET_CP869},                  /* IANA, JDK 1.1 */
{CHARSET_CP869,  "IBM869"},                 /* IANA */
{CHARSET_CP869,  "869"},                    /* IANA */
{CHARSET_CP869,  "CP-GR"},                  /* IANA */
{CHARSET_CP869,  "csIBM869"},               /* IANA */

{CHARSET_CP1125,  PM_CHARSET_CP1125},                 /* ICU */

/*----------encodings_aix----------*/
/* Encodings used by system dependent locales on AIX. */

{CHARSET_CP856,  PM_CHARSET_CP856},

{CHARSET_CP922,  PM_CHARSET_CP922},

{CHARSET_CP943,  PM_CHARSET_CP943},

{CHARSET_CP1046,  PM_CHARSET_CP1046},

{CHARSET_CP1124,  PM_CHARSET_CP1124},

{CHARSET_CP1129,  PM_CHARSET_CP1129},

{CHARSET_CP1161,  PM_CHARSET_CP1161},
{CHARSET_CP1161,  "IBM1161"},                /* glibc */
{CHARSET_CP1161,  "IBM-1161"},               /* glibc */
{CHARSET_CP1161,  "csIBM1161"},              /* glibc */

{CHARSET_CP1162,  PM_CHARSET_CP1162},
{CHARSET_CP1162,  "IBM1162"},                /* glibc */
{CHARSET_CP1162,  "IBM-1162"},               /* glibc */
{CHARSET_CP1162,  "csIBM1162"},              /* glibc */

{CHARSET_CP1163,  PM_CHARSET_CP1163},
{CHARSET_CP1163,  "IBM1163"},                /* glibc */
{CHARSET_CP1163,  "IBM-1163"},               /* glibc */
{CHARSET_CP1163,  "csIBM1163"},              /* glibc */
};

//static int g_convert_failed_num = 0;
/*
static int save_failed_txt(const char* from, const char* to, char* inptr, int insize)
{
	if ((from == NULL) || (to == NULL))
	{
		return -1;
	}
	else
	{
		char filename[PM_MAX_PATH];
		char default_path[PM_MAX_PATH];
		FILE* fp ;
		PM_TIME_T curtime; 
		int pid;
		PM_TIME pt;
		struct stat st;

		const char* pszCefHome = pm_getenv( "CEF_HOME" );
		if( pszCefHome == PM_NULL )
			return -1;

		
#ifdef WIN32
		sprintf( default_path, "%s\\log", pszCefHome );
#else
		sprintf( default_path, "%s/log", pszCefHome );
#endif
		curtime = pm_time();
		pm_localtime( curtime,  &pt);

		sprintf(filename, "%s/%d", default_path, pt.tm_mday);
		if (stat(filename, &st) < 0)
		{
			if (pm_mkdir(filename, 0) != 0)
			{
				return -1;
			}
		}

		pid = pm_getpid();
		g_convert_failed_num++;

		sprintf(filename, "%s/%d/%d_%u_%d_%s_%s.iconv", 
			default_path, pt.tm_mday, pid, (int)curtime, g_convert_failed_num, from, to);

		fp = fopen(filename, "wb");
		if (fp)
		{
			fwrite(inptr, 1, insize, fp);
			fclose(fp);
		}
	}

	return 0;
}
*/

