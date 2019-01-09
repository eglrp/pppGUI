/*------------------------------------------------------------------------------
* rtklib.h : rtklib constants, types and function prototypes
*
*          Copyright (C) 2007-2016 by T.TAKASU, All rights reserved.
*
* options : -DENAGLO   enable GLONASS
*           -DENAGAL   enable Galileo
*           -DENAQZS   enable QZSS
*           -DENACMP   enable BeiDou
*           -DNFREQ=n  set number of obs codes/frequencies
*           -DNEXOBS=n set number of extended obs codes
*           -DMAXOBS=n set max number of obs data in an epoch
*           -DEXTLEX   enable QZSS LEX extension
*
* version : $Revision: 1.1 $ $Date: 2008/07/17 21:48:06 $
* history : 2007/01/13 1.0  rtklib ver.1.0.0
*           2007/03/20 1.1  rtklib ver.1.1.0
*           2008/07/15 1.2  rtklib ver.2.1.0
*           2008/10/19 1.3  rtklib ver.2.1.1
*           2009/01/31 1.4  rtklib ver.2.2.0
*           2009/04/30 1.5  rtklib ver.2.2.1
*           2009/07/30 1.6  rtklib ver.2.2.2
*           2009/12/25 1.7  rtklib ver.2.3.0
*           2010/07/29 1.8  rtklib ver.2.4.0
*           2011/05/27 1.9  rtklib ver.2.4.1
*           2013/03/28 1.10 rtklib ver.2.4.2
*           2016/01/26 1.11 rtklib ver.2.4.3
*-----------------------------------------------------------------------------*/
#ifndef RTKLIB_H
#define RTKLIB_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment( lib,"winmm.lib" )
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <pthread.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

	/* constants -----------------------------------------------------------------*/

#define VER_RTKLIB  "2.4.3"             /* library version */

#define PATCH_LEVEL "b8"                /* patch level */

#define COPYRIGHT_RTKLIB   "Copyright (C) 2007-2016 by T.Takasu\nAll rights reserved."

	//append fuq-2017-2-18
	//#define DLL
#define MAXOUTFILE    50	  //maximum of output files
	extern int comb_j;

#define OFILE_PPPPOS  0      
#define OFILE_SPPPOS  1     
#define OFILE_SATPOS  2      
#define OFILE_SSR_SATPOS  3      
#define OFILE_PRECISE_SATPOS    4    

#define OFILE_DEBUG  5      
#define OFILE_AMBIGUITY    6  
#define OFILE_RESC  7     
#define OFILE_RESP    8  
#define OFILE_PDOP    9  

#define OFILE_GFSLIP    10  
#define OFILE_MWSLIP    11   
#define OFILE_PPP    12  

#define OutputFileName "guet20181223"
#define RECEIVE_RAW_LOG "raw2081220.log"
#define RECEIVE_SP3_CLK_LOG "sp32.log"
#define ENTRIP_DATA1 "user:paswd@ntrip.gnsslab.cn:2101/IGS01"
#define ENTRIP_DATA2 "user:paswd@ntrip.gnsslab.cn:2101/IGS01"
#define ENTRIP_DATA3 "user:paswd@ntrip.gnsslab.cn:2101/IGS01"
#define RAW_DATA_CLIENT "192.168.1.5:6003"


#define rover_data "20181213.log"
   // #define rover_data "com5"
	//#define rover_data "192.168.1.2:4"
#define SP3_FILE "igs19774.sp3"
#define CLK_FILE "igs19774.clk"
#define BLQ_FILE "ocnload.blq"
#define ERP_FILE "igs19774.erp"
#define P1C1DCB "P1C11709.DCB"
#define P1P2DCB "P1P21709.DCB"
#define P2C2DCB "P2C21709.DCB"


#define NSYS_USED     1       //number of used satellite systems, GPS/GLO/BDS/GAL/QZS
	//append end
#define PI          3.1415926535897932  /* pi */
#define D2R         (PI/180.0)          /* deg to rad */
#define R2D         (180.0/PI)          /* rad to deg */
#define CLIGHT      299792458.0         /* speed of light (m/s) */
#define SC2RAD      3.1415926535898     /* semi-circle to radian (IS-GPS) */
#define AU          149597870691.0      /* 1 AU (m) */
#define AS2R        (D2R/3600.0)        /* arc sec to radian */

#define OMGE        7.2921151467E-5     /* earth angular velocity (IS-GPS) (rad/s) */

#define RE_WGS84    6378137.0           /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84    (1.0/298.257223563) /* earth flattening (WGS84) */

#define HION        350000.0            /* ionosphere height (m) */

#define MAXFREQ     7                   /* max NFREQ */


#define GNSISB_OFF    0       /* GNSS ISB: off */
#define GNSISB_CT     1       /* GNSS ISB: time constant */
#define GNSISB_PWC    2       /* GNSS ISB: piece-wise (every 30 min) constant */
#define GNSISB_RW     3       /* GNSS ISB: random walk process */
#define GNSISB_WN     4       /* GNSS ISB: white noise process */

#define FREQ1       1.57542E9           /* L1/E1  frequency (Hz) */
#define FREQ2       1.22760E9           /* L2     frequency (Hz) */
#define FREQ5       1.17645E9           /* L5/E5a frequency (Hz) */
#define FREQ6       1.27875E9           /* E6/LEX frequency (Hz) */
#define FREQ7       1.20714E9           /* E5b    frequency (Hz) */
#define FREQ8       1.191795E9          /* E5a+b  frequency (Hz) */
#define FREQ1_GLO   1.60200E9           /* GLONASS G1 base frequency (Hz) */
#define DFRQ1_GLO   0.56250E6           /* GLONASS G1 bias frequency (Hz/n) */
#define FREQ2_GLO   1.24600E9           /* GLONASS G2 base frequency (Hz) */
#define DFRQ2_GLO   0.43750E6           /* GLONASS G2 bias frequency (Hz/n) */
#define FREQ3_GLO   1.202025E9          /* GLONASS G3 frequency (Hz) */
#define FREQ1_CMP   1.561098E9          /* BeiDou B1 frequency (Hz) */
#define FREQ2_CMP   1.20714E9           /* BeiDou B2 frequency (Hz) */
#define FREQ3_CMP   1.26852E9           /* BeiDou B3 frequency (Hz) */

#define EFACT_GPS   1.0                 /* error factor: GPS */
#define EFACT_GLO   1.5                 /* error factor: GLONASS */
#define EFACT_GAL   1.0                 /* error factor: Galileo */
#define EFACT_QZS   1.0                 /* error factor: QZSS */
#define EFACT_CMP   1.0                 /* error factor: BeiDou */
#define EFACT_SBS   3.0                 /* error factor: SBAS */

#define SYS_NONE    0x00                /* navigation system: none */
#define SYS_GPS     0x01                /* navigation system: GPS */
#define SYS_SBS     0x02                /* navigation system: SBAS */
#define SYS_GLO     0x04                /* navigation system: GLONASS */
#define SYS_GAL     0x08                /* navigation system: Galileo */
#define SYS_QZS     0x10                /* navigation system: QZSS */
#define SYS_CMP     0x20                /* navigation system: BeiDou */
#define SYS_LEO     0x40                /* navigation system: LEO */
#define SYS_ALL     0xFF                /* navigation system: all */

#define TSYS_GPS    0                   /* time system: GPS time */
#define TSYS_UTC    1                   /* time system: UTC */
#define TSYS_GLO    2                   /* time system: GLONASS time */
#define TSYS_GAL    3                   /* time system: Galileo time */
#define TSYS_QZS    4                   /* time system: QZSS time */
#define TSYS_CMP    5                   /* time system: BeiDou time */

#ifndef NFREQ
#define NFREQ       3                   /* number of carrier frequencies */
#endif
#define NFREQGLO    2                   /* number of carrier frequencies of GLONASS */

#ifndef NEXOBS
#define NEXOBS      0                  /* number of extended obs codes */
#endif
#define ENAGPS      1                   /*2017-1-15 fuq */
#ifdef ENAGPS
#define MINPRNGPS   1                   /* min satellite PRN number of GPS */
#define MAXPRNGPS   32                  /* max satellite PRN number of GPS */
#define NSATGPS     (MAXPRNGPS-MINPRNGPS+1) /* number of GPS satellites */
#define NSYSGPS     1
#else
#define MINPRNGPS   0                   /* min satellite PRN number of GPS */
#define MAXPRNGPS   0                  /* max satellite PRN number of GPS */
#define NSATGPS     0                  /* number of GPS satellites */
#define NSYSGPS     0
#endif

//#define ENAGLO      1                   /*2017-1-15 fuq */
#ifdef ENAGLO        
#define MINPRNGLO   1                   /* min satellite slot number of GLONASS */
#define MAXPRNGLO   27                  /* max satellite slot number of GLONASS */
#define NSATGLO     (MAXPRNGLO-MINPRNGLO+1) /* number of GLONASS satellites */
#define NSYSGLO     1
#else
#define MINPRNGLO   0
#define MAXPRNGLO   0
#define NSATGLO     0
#define NSYSGLO     0
#endif
//#define ENAGAL     2
#ifdef ENAGAL
#define MINPRNGAL   1                   /* min satellite PRN number of Galileo */
#define MAXPRNGAL   30                  /* max satellite PRN number of Galileo */
#define NSATGAL    (MAXPRNGAL-MINPRNGAL+1) /* number of Galileo satellites */
#define NSYSGAL     1
#else
#define MINPRNGAL   0
#define MAXPRNGAL   0
#define NSATGAL     0
#define NSYSGAL     0
#endif
#ifdef ENAQZS
#define MINPRNQZS   193                 /* min satellite PRN number of QZSS */
#define MAXPRNQZS   199                 /* max satellite PRN number of QZSS */
#define MINPRNQZS_S 183                 /* min satellite PRN number of QZSS SAIF */
#define MAXPRNQZS_S 189                 /* max satellite PRN number of QZSS SAIF */
#define NSATQZS     (MAXPRNQZS-MINPRNQZS+1) /* number of QZSS satellites */
#define NSYSQZS     1
#else
#define MINPRNQZS   0
#define MAXPRNQZS   0
#define MINPRNQZS_S 0
#define MAXPRNQZS_S 0
#define NSATQZS     0
#define NSYSQZS     0
#endif
//#define ENACMP      35
#ifdef ENACMP
#define MINPRNCMP   1                   /* min satellite sat number of BeiDou */
#define MAXPRNCMP   35                  /* max satellite sat number of BeiDou */
#define NSATCMP     (MAXPRNCMP-MINPRNCMP+1) /* number of BeiDou satellites */
#define NSYSCMP     1
#else
#define MINPRNCMP   0
#define MAXPRNCMP   0
#define NSATCMP     0
#define NSYSCMP     0
#endif
#ifdef ENALEO
#define MINPRNLEO   1                   /* min satellite sat number of LEO */
#define MAXPRNLEO   10                  /* max satellite sat number of LEO */
#define NSATLEO     (MAXPRNLEO-MINPRNLEO+1) /* number of LEO satellites */
#define NSYSLEO     1
#else
#define MINPRNLEO   0
#define MAXPRNLEO   0
#define NSATLEO     0
#define NSYSLEO     0
#endif

#ifdef ENSBS
#define MINPRNSBS   120                 /* min satellite PRN number of SBAS */
#define MAXPRNSBS   142                 /* max satellite PRN number of SBAS */
#define NSATSBS     (MAXPRNSBS-MINPRNSBS+1) /* number of SBAS satellites */
#else
#define MINPRNSBS  0                /* min satellite PRN number of SBAS */
#define MAXPRNSBS  0                 /* max satellite PRN number of SBAS */
#define NSATSBS    0 /* number of SBAS satellites */
#endif

#define MAXSAT      (NSATGPS+NSATGLO+NSATGAL+NSATQZS+NSATCMP+NSATSBS+NSATLEO)
#define NSYS        (NSYSGPS+NSYSGLO+NSYSGAL+NSYSQZS+NSYSCMP+NSYSLEO) /* number of systems */                                    /* max satellite number (1 to MAXSAT) */
	//#define MAXSTA      255

#ifndef MAXOBS
#define MAXOBS      64                  /* max number of obs in an epoch */
#endif
#define MAXRCV      64                  /* max receiver number (1 to MAXRCV) */
#define MAXOBSTYPE  64                  /* max number of obs type in RINEX */
#define DTTOL       0.005               /* tolerance of time difference (s) */
#define MAXDTOE     7200.0              /* max time difference to GPS Toe (s) */
#define MAXDTOE_QZS 7200.0              /* max time difference to QZSS Toe (s) */
#define MAXDTOE_GAL 10800.0             /* max time difference to Galileo Toe (s) */
#define MAXDTOE_CMP 21600.0             /* max time difference to BeiDou Toe (s) */
#define MAXDTOE_GLO 1800.0              /* max time difference to GLONASS Toe (s) */
#define MAXDTOE_SBS 360.0               /* max time difference to SBAS Toe (s) */
#define MAXDTOE_S   86400.0             /* max time difference to ephem toe (s) for other */
#define MAXGDOP     300.0               /* max GDOP */

#define INT_SWAP_TRAC 86400.0           /* swap interval of trace file (s) */
#define INT_SWAP_STAT 86400.0           /* swap interval of solution status file (s) */

#define MAXEXFILE   1024                /* max number of expanded files */
#define MAXSBSAGEF  30.0                /* max age of SBAS fast correction (s) */
#define MAXSBSAGEL  1800.0              /* max age of SBAS long term corr (s) */
#define MAXSBSURA   8                   /* max URA of SBAS satellite */
#define MAXBAND     10                  /* max SBAS band of IGP */
#define MAXNIGP     201                 /* max number of IGP in SBAS band */
#define MAXNGEO     4                   /* max number of GEO satellites */
#define MAXCOMMENT  10                  /* max number of RINEX comments */
#define MAXSTRPATH  1024                /* max length of stream path */
#define MAXSTRMSG   1024                /* max length of stream message */
#define MAXSTRRTK   8                   /* max number of stream in RTK server */
#define MAXSBSMSG   32                  /* max number of SBAS msg in RTK server */
#define MAXSOLMSG   8191                /* max length of solution message */
#define MAXRAWLEN   4096                /* max length of receiver raw message */
#define MAXERRMSG   4096                /* max length of error/warning message */
#define MAXANT      64                  /* max length of station name/antenna type */
#define MAXSOLBUF   256                 /* max number of solution buffer */
#define MAXOBSBUF   128                 /* max number of observation data buffer */
#define MAXNRPOS    16                  /* max number of reference positions */
#define MAXLEAPS    64                  /* max number of leap seconds table */

#define RNX2VER     2.10                /* RINEX ver.2 default output version */
#define RNX3VER     3.00                /* RINEX ver.3 default output version */

#define OBSTYPE_PR  0x01                /* observation type: pseudorange */
#define OBSTYPE_CP  0x02                /* observation type: carrier-phase */
#define OBSTYPE_DOP 0x04                /* observation type: doppler-freq */
#define OBSTYPE_SNR 0x08                /* observation type: SNR */
#define OBSTYPE_ALL 0xFF                /* observation type: all */

#define FREQTYPE_L1 0x01                /* frequency type: L1/E1 */
#define FREQTYPE_L2 0x02                /* frequency type: L2/B1 */
#define FREQTYPE_L5 0x04                /* frequency type: L5/E5a/L3 */
#define FREQTYPE_L6 0x08                /* frequency type: E6/LEX/B3 */
#define FREQTYPE_L7 0x10                /* frequency type: E5b/B2 */
#define FREQTYPE_L8 0x20                /* frequency type: E5(a+b) */
#define FREQTYPE_ALL 0xFF               /* frequency type: all */

#define CODE_NONE   0                   /* obs code: none or unknown */
#define CODE_L1C    1                   /* obs code: L1C/A,G1C/A,E1C (GPS,GLO,GAL,QZS,SBS) */
#define CODE_L1P    2                   /* obs code: L1P,G1P    (GPS,GLO) */
#define CODE_L1W    3                   /* obs code: L1 Z-track (GPS) */
#define CODE_L1Y    4                   /* obs code: L1Y        (GPS) */
#define CODE_L1M    5                   /* obs code: L1M        (GPS) */
#define CODE_L1N    6                   /* obs code: L1codeless (GPS) */
#define CODE_L1S    7                   /* obs code: L1C(D)     (GPS,QZS) */
#define CODE_L1L    8                   /* obs code: L1C(P)     (GPS,QZS) */
#define CODE_L1E    9                   /* obs code: L1-SAIF    (QZS) */
#define CODE_L1A    10                  /* obs code: E1A        (GAL) */
#define CODE_L1B    11                  /* obs code: E1B        (GAL) */
#define CODE_L1X    12                  /* obs code: E1B+C,L1C(D+P) (GAL,QZS) */
#define CODE_L1Z    13                  /* obs code: E1A+B+C,L1SAIF (GAL,QZS) */
#define CODE_L2C    14                  /* obs code: L2C/A,G1C/A (GPS,GLO) */
#define CODE_L2D    15                  /* obs code: L2 L1C/A-(P2-P1) (GPS) */
#define CODE_L2S    16                  /* obs code: L2C(M)     (GPS,QZS) */
#define CODE_L2L    17                  /* obs code: L2C(L)     (GPS,QZS) */
#define CODE_L2X    18                  /* obs code: L2C(M+L),B1I+Q (GPS,QZS,CMP) */
#define CODE_L2P    19                  /* obs code: L2P,G2P    (GPS,GLO) */
#define CODE_L2W    20                  /* obs code: L2 Z-track (GPS) */
#define CODE_L2Y    21                  /* obs code: L2Y        (GPS) */
#define CODE_L2M    22                  /* obs code: L2M        (GPS) */
#define CODE_L2N    23                  /* obs code: L2codeless (GPS) */
#define CODE_L5I    24                  /* obs code: L5/E5aI    (GPS,GAL,QZS,SBS) */
#define CODE_L5Q    25                  /* obs code: L5/E5aQ    (GPS,GAL,QZS,SBS) */
#define CODE_L5X    26                  /* obs code: L5/E5aI+Q  (GPS,GAL,QZS,SBS) */
#define CODE_L7I    27                  /* obs code: E5bI,B2I   (GAL,CMP) */
#define CODE_L7Q    28                  /* obs code: E5bQ,B2Q   (GAL,CMP) */
#define CODE_L7X    29                  /* obs code: E5bI+Q,B2I+Q (GAL,CMP) */
#define CODE_L6A    30                  /* obs code: E6A        (GAL) */
#define CODE_L6B    31                  /* obs code: E6B        (GAL) */
#define CODE_L6C    32                  /* obs code: E6C        (GAL) */
#define CODE_L6X    33                  /* obs code: E6B+C,LEXS+L,B3I+Q (GAL,QZS,CMP) */
#define CODE_L6Z    34                  /* obs code: E6A+B+C    (GAL) */
#define CODE_L6S    35                  /* obs code: LEXS       (QZS) */
#define CODE_L6L    36                  /* obs code: LEXL       (QZS) */
#define CODE_L8I    37                  /* obs code: E5(a+b)I   (GAL) */
#define CODE_L8Q    38                  /* obs code: E5(a+b)Q   (GAL) */
#define CODE_L8X    39                  /* obs code: E5(a+b)I+Q (GAL) */
#define CODE_L2I    40                  /* obs code: B1I        (CMP) */
#define CODE_L2Q    41                  /* obs code: B1Q        (CMP) */
#define CODE_L6I    42                  /* obs code: B3I        (CMP) */
#define CODE_L6Q    43                  /* obs code: B3Q        (CMP) */
#define CODE_L3I    44                  /* obs code: G3I        (GLO) */
#define CODE_L3Q    45                  /* obs code: G3Q        (GLO) */
#define CODE_L3X    46                  /* obs code: G3I+Q      (GLO) */
#define CODE_L1I    47                  /* obs code: B1I        (BDS) */
#define CODE_L1Q    48                  /* obs code: B1Q        (BDS) */
#define MAXCODE     48                  /* max number of obs code */

#define PMODE_SINGLE 0                  /* positioning mode: single */
#define PMODE_DGPS   1                  /* positioning mode: DGPS/DGNSS */
#define PMODE_KINEMA 2                  /* positioning mode: kinematic */
#define PMODE_STATIC 3                  /* positioning mode: static */
#define PMODE_MOVEB  4                  /* positioning mode: moving-base */
#define PMODE_FIXED  5                  /* positioning mode: fixed */
#define PMODE_PPP_KINEMA 6              /* positioning mode: PPP-kinemaric */
#define PMODE_PPP_STATIC 7              /* positioning mode: PPP-static */
#define PMODE_PPP_FIXED 8               /* positioning mode: PPP-fixed */

#define SOLF_LLH    0                   /* solution format: lat/lon/height */
#define SOLF_XYZ    1                   /* solution format: x/y/z-ecef */
#define SOLF_ENU    2                   /* solution format: e/n/u-baseline */
#define SOLF_NMEA   3                   /* solution format: NMEA-183 */
#define SOLF_STAT   4                   /* solution format: solution status */
#define SOLF_GSIF   5                   /* solution format: GSI-F1/F2/F3 */

#define SOLQ_NONE   0                   /* solution status: no solution */
#define SOLQ_FIX    1                   /* solution status: fix */
#define SOLQ_FLOAT  2                   /* solution status: float */
#define SOLQ_SBAS   3                   /* solution status: SBAS */
#define SOLQ_DGPS   4                   /* solution status: DGPS/DGNSS */
#define SOLQ_SINGLE 5                   /* solution status: single */
#define SOLQ_PPP    6                   /* solution status: PPP */
#define SOLQ_DR     7                   /* solution status: dead reconing */
#define MAXSOLQ     7                   /* max number of solution status */

#define TIMES_GPST  0                   /* time system: gps time */
#define TIMES_UTC   1                   /* time system: utc */
#define TIMES_JST   2                   /* time system: jst */

#define IONOOPT_OFF 0                   /* ionosphere option: correction off */
#define IONOOPT_BRDC 1                  /* ionosphere option: broadcast model */
#define IONOOPT_SBAS 2                  /* ionosphere option: SBAS model */
#define IONOOPT_IFLC 3                  /* ionosphere option: L1/L2 or L1/L5 iono-free LC */
#define IONOOPT_EST 4                   /* ionosphere option: estimation */
#define IONOOPT_TEC 5                   /* ionosphere option: IONEX TEC model */
#define IONOOPT_QZS 6                   /* ionosphere option: QZSS broadcast model */
#define IONOOPT_LEX 7                   /* ionosphere option: QZSS LEX ionospehre */
#define IONOOPT_STEC 8                  /* ionosphere option: SLANT TEC model */

#define TROPOPT_OFF 0                   /* troposphere option: correction off */
#define TROPOPT_SAAS 1                  /* troposphere option: Saastamoinen model */
#define TROPOPT_SBAS 2                  /* troposphere option: SBAS model */
#define TROPOPT_EST 3                   /* troposphere option: ZTD estimation */
#define TROPOPT_ESTG 4                  /* troposphere option: ZTD+grad estimation */
#define TROPOPT_ZTD 5                   /* troposphere option: ZTD correction */

#define EPHOPT_BRDC 0                   /* ephemeris option: broadcast ephemeris */
#define EPHOPT_PREC 1                   /* ephemeris option: precise ephemeris */
#define EPHOPT_SBAS 2                   /* ephemeris option: broadcast + SBAS */
#define EPHOPT_SSRAPC 3                 /* ephemeris option: broadcast + SSR_APC */
#define EPHOPT_SSRCOM 4                 /* ephemeris option: broadcast + SSR_COM */
#define EPHOPT_LEX  5                   /* ephemeris option: QZSS LEX ephemeris */

#define ARMODE_OFF  0                   /* AR mode: off */
#define ARMODE_CONT 1                   /* AR mode: continuous */
#define ARMODE_INST 2                   /* AR mode: instantaneous */
#define ARMODE_FIXHOLD 3                /* AR mode: fix and hold */
#define ARMODE_WLNL 4                   /* AR mode: wide lane/narrow lane */
#define ARMODE_TCAR 5                   /* AR mode: triple carrier ar */

#define SBSOPT_LCORR 1                  /* SBAS option: long term correction */
#define SBSOPT_FCORR 2                  /* SBAS option: fast correction */
#define SBSOPT_ICORR 4                  /* SBAS option: ionosphere correction */
#define SBSOPT_RANGE 8                  /* SBAS option: ranging */

#define STR_NONE     0                  /* stream type: none */
#define STR_SERIAL   1                  /* stream type: serial */
#define STR_FILE     2                  /* stream type: file */
#define STR_TCPSVR   3                  /* stream type: TCP server */
#define STR_TCPCLI   4                  /* stream type: TCP client */
#define STR_UDP      5                  /* stream type: UDP stream */
#define STR_NTRIPSVR 6                  /* stream type: NTRIP server */
#define STR_NTRIPCLI 7                  /* stream type: NTRIP client */
#define STR_FTP      8                  /* stream type: ftp */
#define STR_HTTP     9                  /* stream type: http */

#define STRFMT_RTCM2 0                  /* stream format: RTCM 2 */
#define STRFMT_RTCM3 1                  /* stream format: RTCM 3 */
#define STRFMT_OEM4  2                  /* stream format: NovAtel OEMV/4 */
#define STRFMT_OEM3  3                  /* stream format: NovAtel OEM3 */
#define STRFMT_UBX   4                  /* stream format: u-blox LEA-*T */
#define STRFMT_SS2   5                  /* stream format: NovAtel Superstar II */
#define STRFMT_CRES  6                  /* stream format: Hemisphere */
#define STRFMT_STQ   7                  /* stream format: SkyTraq S1315F */
#define STRFMT_GW10  8                  /* stream format: Furuno GW10 */
#define STRFMT_JAVAD 9                  /* stream format: JAVAD GRIL/GREIS */
#define STRFMT_NVS   10                 /* stream format: NVS NVC08C */
#define STRFMT_BINEX 11                 /* stream format: BINEX */
#define STRFMT_RT17  12                 /* stream format: Trimble RT17 */
#define STRFMT_SEPT  13                 /* stream format: Septentrio */
#define STRFMT_LEXR  14                 /* stream format: Furuno LPY-10000 */
#define STRFMT_RINEX 15                 /* stream format: RINEX */
#define STRFMT_SP3   16                 /* stream format: SP3 */
#define STRFMT_RNXCLK 17                /* stream format: RINEX CLK */
#define STRFMT_SBAS  18                 /* stream format: SBAS messages */
#define STRFMT_NMEA  19                 /* stream format: NMEA 0183 */
#ifndef EXTLEX
#define MAXRCVFMT    13                 /* max number of receiver format */
#else
#define MAXRCVFMT    14
#endif

#define STR_MODE_R  0x1                 /* stream mode: read */
#define STR_MODE_W  0x2                 /* stream mode: write */
#define STR_MODE_RW 0x3                 /* stream mode: read/write */

#define GEOID_EMBEDDED    0             /* geoid model: embedded geoid */
#define GEOID_EGM96_M150  1             /* geoid model: EGM96 15x15" */
#define GEOID_EGM2008_M25 2             /* geoid model: EGM2008 2.5x2.5" */
#define GEOID_EGM2008_M10 3             /* geoid model: EGM2008 1.0x1.0" */
#define GEOID_GSI2000_M15 4             /* geoid model: GSI geoid 2000 1.0x1.5" */
#define GEOID_RAF09       5             /* geoid model: IGN RAF09 for France 1.5"x2" */

#define COMMENTH    "%"                 /* comment line indicator for solution */
#define MSG_DISCONN "$_DISCONNECT\r\n"  /* disconnect message */

#define DLOPT_FORCE   0x01              /* download option: force download existing */
#define DLOPT_KEEPCMP 0x02              /* download option: keep compressed file */
#define DLOPT_HOLDERR 0x04              /* download option: hold on error file */
#define DLOPT_HOLDLST 0x08              /* download option: hold on listing file */

#define P2_5        0.03125             /* 2^-5 */
#define P2_6        0.015625            /* 2^-6 */
#define P2_11       4.882812500000000E-04 /* 2^-11 */
#define P2_15       3.051757812500000E-05 /* 2^-15 */
#define P2_17       7.629394531250000E-06 /* 2^-17 */
#define P2_19       1.907348632812500E-06 /* 2^-19 */
#define P2_20       9.536743164062500E-07 /* 2^-20 */
#define P2_21       4.768371582031250E-07 /* 2^-21 */
#define P2_23       1.192092895507810E-07 /* 2^-23 */
#define P2_24       5.960464477539063E-08 /* 2^-24 */
#define P2_27       7.450580596923828E-09 /* 2^-27 */
#define P2_29       1.862645149230957E-09 /* 2^-29 */
#define P2_30       9.313225746154785E-10 /* 2^-30 */
#define P2_31       4.656612873077393E-10 /* 2^-31 */
#define P2_32       2.328306436538696E-10 /* 2^-32 */
#define P2_33       1.164153218269348E-10 /* 2^-33 */
#define P2_35       2.910383045673370E-11 /* 2^-35 */
#define P2_38       3.637978807091710E-12 /* 2^-38 */
#define P2_39       1.818989403545856E-12 /* 2^-39 */
#define P2_40       9.094947017729280E-13 /* 2^-40 */
#define P2_43       1.136868377216160E-13 /* 2^-43 */
#define P2_48       3.552713678800501E-15 /* 2^-48 */
#define P2_50       8.881784197001252E-16 /* 2^-50 */
#define P2_55       2.775557561562891E-17 /* 2^-55 */

#ifdef WIN32
#define thread_t    HANDLE
#define lock_t      CRITICAL_SECTION
#define initlock(f) InitializeCriticalSection(f)
#define lock(f)     EnterCriticalSection(f)
#define unlock(f)   LeaveCriticalSection(f)
#define FILEPATHSEP '\\'
#else
#define thread_t    pthread_t
#define lock_t      pthread_mutex_t
#define initlock(f) pthread_mutex_init(f,NULL)
#define lock(f)     pthread_mutex_lock(f)
#define unlock(f)   pthread_mutex_unlock(f)
#define FILEPATHSEP '/'
#endif

	/* type definitions ----------------------------------------------------------*/
	//my defined structs by fzhou @ GFZ
	typedef struct {   //一天中的秒数
		long   sn;
		double tos;
	} sod_t;
	typedef struct {   //约化儒略日
		long  day;
		sod_t ds;
	} mjd_t;


	typedef struct {        /* time struct */
		time_t time;        /* time (s) expressed by standard time_t */
		double sec;         /* fraction of second under 1 s */
	} gtime_t;

	typedef struct {        /* observation data record */
		gtime_t time;       /* receiver sampling time (GPST) */
		unsigned char sat, rcv; /* satellite/receiver number */
		unsigned char SNR[NFREQ + NEXOBS]; /* signal strength (0.25 dBHz) */
		unsigned char LLI[NFREQ + NEXOBS]; /* loss of lock indicator */
		unsigned char code[NFREQ + NEXOBS]; /* code indicator (CODE_???) */
		double L[NFREQ + NEXOBS]; /* observation data carrier-phase (cycle) */
		double P[NFREQ + NEXOBS]; /* observation data pseudorange (m) */
		float  D[NFREQ + NEXOBS]; /* observation data doppler frequency (Hz) */
	} obsd_t;

	typedef struct {        /* observation data */
		int n, nmax;         /* number of obervation data/allocated */
		obsd_t *data;       /* observation data records */
	} obs_t;

	typedef struct {        /* earth rotation parameter data type */
		double mjd;         /* mjd (days) */
		double xp, yp;       /* pole offset (rad) */
		double xpr, ypr;     /* pole offset rate (rad/day) */
		double ut1_utc;     /* ut1-utc (s) */
		double lod;         /* length of day (s/day) */
	} erpd_t;

	typedef struct {        /* earth rotation parameter type */
		int n, nmax;         /* number and max number of data */
		erpd_t *data;       /* earth rotation parameter data */
	} erp_t;

	typedef struct {        /* antenna parameter type */
		int sat;            /* satellite number (0:receiver) */
		char type[MAXANT];  /* antenna type */
		char code[MAXANT];  /* serial number or satellite code */
		gtime_t ts, te;      /* valid time start and end */
		double off[5 * NFREQ][3]; /* phase center offset e/n/u or x/y/z (m) */
		double var[5 * NFREQ][80 * 20]; /* phase center variation (m) */
										/* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */
		double dazi;             //Increment of the azimuth：0 to 360 with increment 'DAZI'(in degrees).
		double zen1, zen2, dzen;   //Receiver antenna:Definition of the grid in zenith angle.
								   //Satellite antenna:Definition of the grid in nadir angle.
	} pcv_t;

	typedef struct {        /* antenna parameters type */
		int n, nmax;         /* number of data/allocated */
		pcv_t *pcv;         /* antenna parameters data */
	} pcvs_t;

	typedef struct {        /* almanac type */
		int sat;            /* satellite number */
		int svh;            /* sv health (0:ok) */
		int svconf;         /* as and sv config */
		int week;           /* GPS/QZS: gps week, GAL: galileo week */
		gtime_t toa;        /* Toa */
							/* SV orbit parameters */
		double A, e, i0, OMG0, omg, M0, OMGd;
		double toas;        /* Toa (s) in week */
		double f0, f1;       /* SV clock parameters (af0,af1) */
	} alm_t;

	typedef struct {        /* GPS/QZS/GAL broadcast ephemeris type */
		int sat;            /* satellite number */
		int iode, iodc;      /* IODE,IODC */
		int sva;            /* SV accuracy (URA index) */
		int svh;            /* SV health (0:ok) */
		int week;           /* GPS/QZS: gps week, GAL: galileo week */
		int code;           /* GPS/QZS: code on L2, GAL/CMP: data sources */
		int flag;           /* GPS/QZS: L2 P data flag, CMP: nav type */
		gtime_t toe, toc, ttr; /* Toe,Toc,T_trans */
							   /* SV orbit parameters */
		double A, e, i0, OMG0, omg, M0, deln, OMGd, idot;
		double crc, crs, cuc, cus, cic, cis;
		double toes;        /* Toe (s) in week */
		double fit;         /* fit interval (h) */
		double f0, f1, f2;    /* SV clock parameters (af0,af1,af2) */
		double tgd[4];      /* group delay parameters */
							/* GPS/QZS:tgd[0]=TGD */
							/* GAL    :tgd[0]=BGD E5a/E1,tgd[1]=BGD E5b/E1 */
							/* CMP    :tgd[0]=BGD1,tgd[1]=BGD2 */
		double Adot, ndot;   /* Adot,ndot for CNAV */
	} eph_t;

	typedef struct {        /* GLONASS broadcast ephemeris type */
		int sat;            /* satellite number */
		int iode;           /* IODE (0-6 bit of tb field) */
		int frq;            /* satellite frequency number */
		int svh, sva, age;    /* satellite health, accuracy, age of operation */
		gtime_t toe;        /* epoch of epherides (gpst) */
		gtime_t tof;        /* message frame time (gpst) */
		double pos[3];      /* satellite position (ecef) (m) */
		double vel[3];      /* satellite velocity (ecef) (m/s) */
		double acc[3];      /* satellite acceleration (ecef) (m/s^2) */
		double taun, gamn;   /* SV clock bias (s)/relative freq bias */
		double dtaun;       /* delay between L1 and L2 (s) */
	} geph_t;

	typedef struct {        /* precise ephemeris type */
		gtime_t time;       /* time (GPST) */
		int index;          /* ephemeris index for multiple files */
		double pos[MAXSAT][4]; /* satellite position/clock (ecef) (m|s) */
		float  std[MAXSAT][4]; /* satellite position/clock std (m|s) */
		double vel[MAXSAT][4]; /* satellite velocity/clk-rate (m/s|s/s) */
		float  vst[MAXSAT][4]; /* satellite velocity/clk-rate std (m/s|s/s) */
		float  cov[MAXSAT][3]; /* satellite position covariance (m^2) */
		float  vco[MAXSAT][3]; /* satellite velocity covariance (m^2) */
	} peph_t;

	typedef struct {        /* precise clock type */
		gtime_t time;       /* time (GPST) */
		int index;          /* clock index for multiple files */
		double clk[MAXSAT][1]; /* satellite clock (s) */
		float  std[MAXSAT][1]; /* satellite clock std (s) */
	} pclk_t;

	typedef struct {        /* SBAS ephemeris type */
		int sat;            /* satellite number */
		gtime_t t0;         /* reference epoch time (GPST) */
		gtime_t tof;        /* time of message frame (GPST) */
		int sva;            /* SV accuracy (URA index) */
		int svh;            /* SV health (0:ok) */
		double pos[3];      /* satellite position (m) (ecef) */
		double vel[3];      /* satellite velocity (m/s) (ecef) */
		double acc[3];      /* satellite acceleration (m/s^2) (ecef) */
		double af0, af1;     /* satellite clock-offset/drift (s,s/s) */
	} seph_t;

	typedef struct {        /* satellite fcb data type */
		gtime_t ts, te;      /* start/end time (GPST) */
		double bias[MAXSAT][3]; /* fcb value   (cyc) */
		double std[MAXSAT][3]; /* fcb std-dev (cyc) */
	} fcbd_t;


	typedef struct {        /* SSR correction type */
		gtime_t t0[6];      /* epoch time (GPST) {eph,clk,hrclk,ura,bias,pbias} */
		double udi[6];      /* SSR update interval (s) */
		int iod[6];         /* iod ssr {eph,clk,hrclk,ura,bias,pbias} */
		int iode;           /* issue of data */
		int iodcrc;         /* issue of data crc for beidou/sbas */
		int ura;            /* URA indicator */
		int refd;           /* sat ref datum (0:ITRF,1:regional) */
		double deph[3];    /* delta orbit {radial,along,cross} (m) */
		double ddeph[3];    /* dot delta orbit {radial,along,cross} (m/s) */
		double dclk[3];    /* delta clock {c0,c1,c2} (m,m/s,m/s^2) */
		double hrclk;       /* high-rate clock corection (m) */
		float  cbias[MAXCODE]; /* code biases (m) */
		double pbias[MAXCODE]; /* phase biases (m) */
		float  stdpb[MAXCODE]; /* std-dev of phase biases (m) */
		double yaw_ang, yaw_rate; /* yaw angle and yaw rate (deg,deg/s) */
		unsigned char update; /* update flag (0:no update,1:update) */
	} ssr_t;


	typedef struct {        /* stec data type */
		gtime_t time;       /* time (GPST) */
		unsigned char sat;  /* satellite number */
		double ion;         /* slant ionos delay (m) */
		float std;          /* std-dev (m) */
		float azel[2];      /* azimuth/elevation (rad) */
		unsigned char flag; /* fix flag */
	} stec_t;

	typedef struct {        /* trop data type */
		gtime_t time;       /* time (GPST) */
		double trp[3];      /* zenith tropos delay/gradient (m) */
		float std[3];       /* std-dev (m) */
	} trop_t;


	typedef struct {        /* navigation data type */
		int n, nmax;         /* number of broadcast ephemeris */
		int ng, ngmax;       /* number of glonass ephemeris */
		int ns, nsmax;       /* number of sbas ephemeris */
		int ne, nemax;       /* number of precise ephemeris */
		int nc, ncmax;       /* number of precise clock */
		int na, namax;       /* number of almanac data */
		int nt, ntmax;       /* number of tec grid data */
		int nf, nfmax;       /* number of satellite fcb data */
		eph_t *eph;         /* GPS/QZS/GAL ephemeris */
		geph_t *geph;       /* GLONASS ephemeris */
		seph_t *seph;       /* SBAS ephemeris */
		peph_t *peph;       /* precise ephemeris */
		pclk_t *pclk;       /* precise clock */
		alm_t *alm;         /* almanac data */
							// tec_t *tec;         /* tec grid data */
		fcbd_t *fcb;        /* satellite fcb data */
		erp_t  erp;         /* earth rotation parameters */
		double utc_gps[4];  /* GPS delta-UTC parameters {A0,A1,T,W} */
		double utc_glo[4];  /* GLONASS UTC GPS time parameters */
		double utc_gal[4];  /* Galileo UTC GPS time parameters */
		double utc_qzs[4];  /* QZS UTC GPS time parameters */
		double utc_cmp[4];  /* BeiDou UTC parameters */
		double utc_sbs[4];  /* SBAS UTC parameters */
		double ion_gps[8];  /* GPS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
		double ion_gal[4];  /* Galileo iono model parameters {ai0,ai1,ai2,0} */
		double ion_qzs[8];  /* QZSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
		double ion_cmp[8];  /* BeiDou iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
		int leaps;          /* leap seconds (s) */
		double lam[MAXSAT][NFREQ]; /* carrier wave lengths (m) */
		double cbias[MAXSAT][3]; /* satellite dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
		double rbias[MAXRCV][2][3]; /* receiver dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
		double wlbias[MAXSAT];   /* wide-lane bias (cycle) */
		double glo_cpbias[4];    /* glonass code-phase bias {1C,1P,2C,2P} (m) */
		char glo_fcn[MAXPRNGLO + 1]; /* glonass frequency channel number + 8 */
		pcv_t pcvs[MAXSAT]; /* satellite antenna pcv */
		ssr_t ssr[MAXSAT];  /* SSR corrections */
	} nav_t;

	typedef struct {        /* station parameter type */
		char name[MAXANT]; /* marker name */
		char marker[MAXANT]; /* marker number */
		char antdes[MAXANT]; /* antenna descriptor */
		char antsno[MAXANT]; /* antenna serial number */
		char rectype[MAXANT]; /* receiver type descriptor */
		char recver[MAXANT]; /* receiver firmware version */
		char recsno[MAXANT]; /* receiver serial number */
		int antsetup;       /* antenna setup id */
		int itrf;           /* ITRF realization year */
		int deltype;        /* antenna delta type (0:enu,1:xyz) */
		double pos[3];      /* station position (ecef) (m) */
		double del[3];      /* antenna position delta (e/n/u or x/y/z) (m) */
		double hgt;         /* antenna height (m) */
	} sta_t;

	typedef struct {        /* solution type */
		gtime_t time, ssr_time;       /* time (GPST) */
		double rr[6];       /* position/velocity (m|m/s) */
							/* {x,y,z,vx,vy,vz} or {e,n,u,ve,vn,vu} */
		float  qr[6];       /* position variance/covariance (m^2) */
							/* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
							/* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
		double dtr[6];      /* receiver clock bias to time systems (s) */
		unsigned char type; /* type (0:xyz-ecef,1:enu-baseline) */
		unsigned char stat; /* solution status (SOLQ_???) */
		unsigned char ns;   /* number of valid satellites */
		unsigned char nsats[NSYS_USED];   /* number of valid satellites */
		float age;          /* age of differential (s) */
		float ratio;        /* AR ratio factor for valiation */
		float thres;        /* AR ratio threshold for valiation */
        double dxyz[3];
		double dop[4];
	} sol_t;

	typedef struct {        /* solution buffer type */
		int n, nmax;         /* number of solution/max number of buffer */
		int cyclic;         /* cyclic buffer flag */
		int start, end;      /* start/end index */
		gtime_t time;       /* current solution time */
		sol_t *data;        /* solution data */
		double rb[3];       /* reference position {x,y,z} (ecef) (m) */
		unsigned char buff[MAXSOLMSG + 1]; /* message buffer */
		int nb;             /* number of byte in message buffer */
	} solbuf_t;

	typedef struct {        /* solution status type */
		gtime_t time;       /* time (GPST) */
		unsigned char sat;  /* satellite number */
		unsigned char frq;  /* frequency (1:L1,2:L2,...) */
		float az, el;        /* azimuth/elevation angle (rad) */
		float resp;         /* pseudorange residual (m) */
		float resc;         /* carrier-phase residual (m) */
		unsigned char flag; /* flags: (vsat<<5)+(slip<<3)+fix */
		unsigned char snr;  /* signal strength (0.25 dBHz) */
		unsigned short lock;  /* lock counter */
		unsigned short outc;  /* outage counter */
		unsigned short slipc; /* slip counter */
		unsigned short rejc;  /* reject counter */
	} solstat_t;

	typedef struct {        /* solution status buffer type */
		int n, nmax;         /* number of solution/max number of buffer */
		solstat_t *data;    /* solution status data */
	} solstatbuf_t;

	typedef struct {        /* RTCM control struct type */
		int staid;          /* station id */
		int stah;           /* station health */
		int seqno;          /* sequence number for rtcm 2 or iods msm */
		int outtype;        /* output message type */
		gtime_t time;       /* message time */
		gtime_t time_s;     /* message start time */
		obs_t obs;          /* observation data (uncorrected) */
		nav_t nav;          /* satellite ephemerides */
		sta_t sta;          /* station parameters */
		ssr_t ssr[MAXSAT];  /* output of ssr corrections */
		char msg[128];      /* special message */
		char msgtype[256];  /* last message type */
		char msmtype[6][128]; /* msm signal types */
		int obsflag;        /* obs data complete flag (1:ok,0:not complete) */
		int ephsat;         /* update satellite of ephemeris */
		double cp[MAXSAT][NFREQ + NEXOBS]; /* carrier-phase measurement */
		unsigned char lock[MAXSAT][NFREQ + NEXOBS]; /* lock time */
		unsigned char loss[MAXSAT][NFREQ + NEXOBS]; /* loss of lock count */
		gtime_t lltime[MAXSAT][NFREQ + NEXOBS]; /* last lock time */
		int nbyte;          /* number of bytes in message buffer */
		int nbit;           /* number of bits in word buffer */
		int len;            /* message length (bytes) */
		unsigned char buff[1200]; /* message buffer */
		unsigned int word;  /* word buffer for rtcm 2 */
		unsigned int nmsg2[100]; /* message count of RTCM 2 (1-99:1-99,0:other) */
		unsigned int nmsg3[400]; /* message count of RTCM 3 (1-299:1001-1299,300-399:2000-2099,0:ohter) */
		char opt[256];      /* RTCM dependent options */
		int week;
	} rtcm_t;

	typedef struct {        /* rinex control struct type */
		gtime_t time;       /* message time */
		double ver;         /* rinex version */
		char   type;        /* rinex file type ('O','N',...) */
		int    sys;         /* navigation system */
		int    tsys;        /* time system */
		char   tobs[6][MAXOBSTYPE][4]; /* rinex obs types */
		obs_t  obs;         /* observation data */
		nav_t  nav;         /* navigation data */
		sta_t  sta;         /* station info */
		int    ephsat;      /* ephemeris satellite number */
		char   opt[256];    /* rinex dependent options */
	} rnxctr_t;

	typedef struct {        /* download url type */
		char type[32];      /* data type */
		char path[1024];    /* url path */
		char dir[1024];    /* local directory */
		double tint;        /* time interval (s) */
	} url_t;

	typedef struct {        /* option type */
		char *name;         /* option name */
		int format;         /* option format (0:int,1:double,2:string,3:enum) */
		void *var;          /* pointer to option variable */
		char *comment;      /* option comment/enum labels/unit */
	} opt_t;

	typedef struct {        /* extended receiver error model */
		int ena[4];         /* model enabled */
		double cerr[4][NFREQ * 2]; /* code errors (m) */
		double perr[4][NFREQ * 2]; /* carrier-phase errors (m) */
		double gpsglob[NFREQ]; /* gps-glonass h/w bias (m) */
		double gloicb[NFREQ]; /* glonass interchannel bias (m/fn) */
	} exterr_t;

	typedef struct {        /* SNR mask type */
		int ena[2];         /* enable flag {rover,base} */
		double mask[NFREQ][9]; /* mask (dBHz) at 5,10,...85 deg */
	} snrmask_t;

	typedef struct {        /* processing options type */
		int mode;           /* positioning mode (PMODE_???) */
		int soltype;        /* solution type (0:forward,1:backward,2:combined) */
		int nf;             /* number of frequencies (1:L1,2:L1+L2,3:L1+L2+L5) */
		int navsys;         /* navigation system */
		int gnsisb;         /* stochastic modeling for ISBs in multi-GNSS processing */
		double elmin;       /* elevation mask angle (rad) */
		snrmask_t snrmask;  /* SNR mask */
		int sateph;         /* satellite ephemeris/clock (EPHOPT_???) */
		int modear;         /* AR mode (0:off,1:continuous,2:instantaneous,3:fix and hold,4:ppp-ar) */
		int glomodear;      /* GLONASS AR mode (0:off,1:on,2:auto cal,3:ext cal) */
		int bdsmodear;      /* BeiDou AR mode (0:off,1:on) */
		int maxout;         /* obs outage count to reset bias */
		int minlock;        /* min lock count to fix ambiguity */
		int minfix;         /* min fix count to hold ambiguity */
		int armaxiter;      /* max iteration to resolve ambiguity */
		int ionoopt;        /* ionosphere option (IONOOPT_???) */
		int tropopt;        /* troposphere option (TROPOPT_???) */
		int dynamics;       /* dynamics model (0:none,1:velociy,2:accel) */
		int tidecorr;       /* earth tide correction (0:off,1:solid,2:solid+otl+pole) */
		int niter;          /* number of filter iteration */
		int codesmooth;     /* code smoothing window size (0:none) */
		int intpref;        /* interpolate reference obs (for post mission) */
		int sbascorr;       /* SBAS correction options */
		int sbassatsel;     /* SBAS satellite selection (0:all) */
		int rovpos;         /* rover position for fixed mode */
		int refpos;         /* base position for relative mode */
							/* (0:pos in prcopt,  1:average of single pos, */
							/*  2:read from file, 3:rinex header, 4:rtcm pos) */
		double eratio[NFREQ]; /* code/phase error ratio */
		double err[5];      /* measurement error factor */
							/* [0]:reserved */
							/* [1-3]:error factor a/b/c of phase (m) */
							/* [4]:doppler frequency (hz) */
		double std[3];      /* initial-state std [0]bias,[1]iono [2]trop */
		double prn[6];      /* process-noise std [0]bias,[1]iono [2]trop [3]acch [4]accv [5] pos */
		double sclkstab;    /* satellite clock stability (sec/sec) */
		double thresar[8];  /* AR validation threshold */
		double elmaskar;    /* elevation mask of AR for rising satellite (deg) */
		double elmaskhold;  /* elevation mask to hold ambiguity (deg) */
		double thresslip;   /* slip threshold of geometry-free phase (m) */
		double maxtdiff;    /* max difference of time (sec) */
		double maxinno;     /* reject threshold of innovation (m) */
		double maxgdop;     /* reject threshold of gdop */
		double baseline[2]; /* baseline length constraint {const,sigma} (m) */
		double ru[3];       /* rover position for fixed mode {x,y,z} (ecef) (m) */
		double rb[3];       /* base position for relative mode {x,y,z} (ecef) (m) */
		char anttype[2][MAXANT]; /* antenna types {rover,base} */
		double antdel[2][3]; /* antenna delta {{rov_e,rov_n,rov_u},{ref_e,ref_n,ref_u}} */
		pcv_t pcvr[2];      /* receiver antenna parameters {rov,base} */
		unsigned char exsats[MAXSAT]; /* excluded satellites (1:excluded,2:included) */
		char rnxopt[2][256]; /* rinex options {rover,base} */
		int  posopt[6];     /* positioning options */
		int  syncsol;       /* solution sync mode (0:off,1:on) */
		double odisp[2][6 * 11]; /* ocean tide loading parameters {rov,base} */
		exterr_t exterr;    /* extended receiver error model */
		int freqopt;        /* disable L2-AR */
		char pppopt[256];   /* ppp option */
	} prcopt_t;
    extern prcopt_t prcopt1;
	typedef struct {        /* solution options type */
		int posf;           /* solution format (SOLF_???) */
		int times;          /* time system (TIMES_???) */
		int timef;          /* time format (0:sssss.s,1:yyyy/mm/dd hh:mm:ss.s) */
		int timeu;          /* time digits under decimal point */
		int degf;           /* latitude/longitude format (0:ddd.ddd,1:ddd mm ss) */
		int outhead;        /* output header (0:no,1:yes) */
		int outopt;         /* output processing options (0:no,1:yes) */
		int datum;          /* datum (0:WGS84,1:Tokyo) */
		int height;         /* height (0:ellipsoidal,1:geodetic) */
		int geoid;          /* geoid model (0:EGM96,1:JGD2000) */
		int solstatic;      /* solution of static mode (0:all,1:single) */
		int sstat;          /* solution statistics level (0:off,1:states,2:residuals) */
		int trace;          /* debug trace level (0:off,1-5:debug) */
		double nmeaintv[2]; /* nmea output interval (s) (<0:no,0:all) */
							/* nmeaintv[0]:gprmc,gpgga,nmeaintv[1]:gpgsv */
		char sep[64];       /* field separator */
		char prog[64];      /* program name */
		int fpout[50];  //for solution files output
	} solopt_t;

	typedef struct {        /* file options type */
		char satantp[MAXSTRPATH]; /* satellite antenna parameters file */
		char rcvantp[MAXSTRPATH]; /* receiver antenna parameters file */
		char stapos[MAXSTRPATH]; /* station positions file */
		char geoid[MAXSTRPATH]; /* external geoid data file */
		char iono[MAXSTRPATH]; /* ionosphere data file */
		char dcb[MAXSTRPATH]; /* dcb data file */
		char eop[MAXSTRPATH]; /* eop data file */
		char blq[MAXSTRPATH]; /* ocean tide loading blq file */
		char tempdir[MAXSTRPATH]; /* ftp/http temporaly directory */
		char geexe[MAXSTRPATH]; /* google earth exec file */
		char solstat[MAXSTRPATH]; /* solution statistics file */
		char trace[MAXSTRPATH]; /* debug trace file */

	} filopt_t;

	typedef struct {        /* RINEX options type */
		gtime_t ts, te;      /* time start/end */
		double tint;        /* time interval (s) */
		double tunit;       /* time unit for multiple-session (s) */
		double rnxver;      /* RINEX version */
		int navsys;         /* navigation system */
		int obstype;        /* observation type */
		int freqtype;       /* frequency type */
		char mask[6][64];   /* code mask {GPS,GLO,GAL,QZS,SBS,CMP} */
		char staid[32];    /* station id for rinex file name */
		char prog[32];    /* program */
		char runby[32];    /* run-by */
		char marker[64];    /* marker name */
		char markerno[32];  /* marker number */
		char markertype[32]; /* marker type (ver.3) */
		char name[2][32];   /* observer/agency */
		char rec[3][32];   /* receiver #/type/vers */
		char ant[3][32];   /* antenna #/type */
		double apppos[3];   /* approx position x/y/z */
		double antdel[3];   /* antenna delta h/e/n */
		char comment[MAXCOMMENT][64]; /* comments */
		char rcvopt[256];   /* receiver dependent options */
		unsigned char exsats[MAXSAT]; /* excluded satellites */
		int scanobs;        /* scan obs types */
		int outiono;        /* output iono correction */
		int outtime;        /* output time system correction */
		int outleaps;       /* output leap seconds */
		int autopos;        /* auto approx position */
		gtime_t tstart;     /* first obs time */
		gtime_t tend;       /* last obs time */
		gtime_t trtcm;      /* approx log start time for rtcm */
		char tobs[6][MAXOBSTYPE][4]; /* obs types {GPS,GLO,GAL,QZS,SBS,CMP} */
		int nobs[6];        /* number of obs types {GPS,GLO,GAL,QZS,SBS,CMP} */
	} rnxopt_t;

	typedef struct {        /* satellite status type */
		unsigned char sys;  /* navigation system */
		unsigned char vs;   /* valid satellite flag single */
		double azel[2];     /* azimuth/elevation angles {az,el} (rad) */
		double resp[NFREQ]; /* residuals of pseudorange (m) */
		double resc[NFREQ]; /* residuals of carrier-phase (m) */
		unsigned char vsat[NFREQ]; /* valid satellite flag */
		unsigned char snr[NFREQ]; /* signal strength (0.25 dBHz) */
		unsigned char fix[NFREQ]; /* ambiguity fix flag (1:fix,2:float,3:hold) */
		unsigned char slip[NFREQ]; /* cycle-slip flag */
		unsigned int lock[NFREQ]; /* lock counter of phase */
		unsigned int outc[NFREQ]; /* obs outage counter of phase */
		unsigned int slipc[NFREQ]; /* cycle-slip counter */
		unsigned int rejc[NFREQ]; /* reject counter */
		double  gf;         /* geometry-free phase L1-L2 (m) */
		double  gf2;        /* geometry-free phase L1-L5 (m) */
		double  mw;         /* MW-LC (m) */
		double  phw;        /* phase windup (cycle) */
		gtime_t pt[2][NFREQ]; /* previous carrier-phase time */
		double  ph[2][NFREQ]; /* previous carrier-phase observable (cycle) */
							  //new---------------------------
		double satpos[6];   /* sat. position (m) and velocity (m/s) */
		double satclk;      /* sat. clock offset (m) */
		double dsag;
		double dtrp, wmap, shd;
		double dtid;
		double dant[NFREQ];
		int flag;
		double P[NFREQ], L[NFREQ];

        double obs0[MAXSAT][4];
		int clkJump;
		int clkJump2;
        double Mp1[MAXSAT][1];
        double Mp2[MAXSAT][1];
        double TEC1[MAXSAT][1];
        double TEC2[MAXSAT][1];
	} ssat_t;

	typedef struct {        /* ambiguity control type */
		gtime_t epoch[4];   /* last epoch */
		int n[4];           /* number of epochs */
		double LC[4];      /* linear combination average */
		double LCv[4];      /* linear combination variance */
		int fixcnt;         /* fix count */
		char flags[MAXSAT]; /* fix flags */
	} ambc_t;

	typedef struct {        /* RTK control/result type */
		sol_t  sol;         /* RTK solution */
		double rb[6];       /* base position/velocity (ecef) (m|m/s) */
		int nx, na;          /* number of float states/fixed states */
		double tt;          /* time difference between current and previous (s) */
		double *x, *P;      /* float states and their covariance */
		double *xa, *Pa;     /* fixed states and their covariance */
		int nfix;           /* number of continuous fixes of ambiguity */
		ambc_t ambc[MAXSAT]; /* ambibuity control */
		ssat_t ssat[MAXSAT]; /* satellite status */
		int neb;            /* bytes in error message buffer */
		char errbuf[MAXERRMSG]; /* error message buffer */
		prcopt_t opt;       /* processing options */
		double tow0;
	} rtk_t;

	typedef struct {        /* receiver raw data control type */
		gtime_t time;       /* message time */
		gtime_t tobs;       /* observation data time */
		obs_t obs;          /* observation data */
		obs_t obuf;         /* observation data buffer */
		nav_t nav;          /* satellite ephemerides */
		sta_t sta;          /* station parameters */
		int ephsat;         /* sat number of update ephemeris (0:no satellite) */
		char msgtype[256];  /* last message type */
		unsigned char subfrm[MAXSAT][380];  /* subframe buffer */
		double lockt[MAXSAT][NFREQ + NEXOBS]; /* lock time (s) */
		double icpp[MAXSAT], off[MAXSAT], icpc; /* carrier params for ss2 */
		double prCA[MAXSAT], dpCA[MAXSAT]; /* L1/CA pseudrange/doppler for javad */
		unsigned char halfc[MAXSAT][NFREQ + NEXOBS]; /* half-cycle add flag */
		char freqn[MAXOBS]; /* frequency number for javad */
		int nbyte;          /* number of bytes in message buffer */
		int len;            /* message length (bytes) */
		int iod;            /* issue of data */
		int tod;            /* time of day (ms) */
		int tbase;          /* time base (0:gpst,1:utc(usno),2:glonass,3:utc(su) */
		int flag;           /* general purpose flag */
		int outtype;        /* output message type */
		unsigned char buff[MAXRAWLEN]; /* message buffer */
		char opt[256];      /* receiver dependent options */
		double receive_time;/* RT17: Reiceve time of week for week rollover detection */
		unsigned int plen;  /* RT17: Total size of packet to be read */
		unsigned int pbyte; /* RT17: How many packet bytes have been read so far */
		unsigned int page;  /* RT17: Last page number */
		unsigned int reply; /* RT17: Current reply number */
		int week;           /* RT17: week number */
		unsigned char pbuff[255 + 4 + 2]; /* RT17: Packet buffer */
	} raw_t;

	typedef struct {        /* stream type */
		int type;           /* type (STR_???) */
		int mode;           /* mode (STR_MODE_?) */
		int state;          /* state (-1:error,0:close,1:open) */
		unsigned int inb, inr;   /* input bytes/rate */
		unsigned int outb, outr; /* output bytes/rate */
		unsigned int tick, tact; /* tick/active tick */
		unsigned int inbt, outbt; /* input/output bytes at tick */
		lock_t lock;        /* lock flag */
		void *port;         /* type dependent port control struct */
		char path[MAXSTRPATH]; /* stream path */
		char msg[MAXSTRMSG];  /* stream message */
	} stream_t;


	typedef struct {        /* stream server type */
		int state;          /* server state (0:stop,1:running) */
		int cycle;          /* server cycle (ms) */
		int buffsize;       /* input/monitor buffer size (bytes) */
		int nmeacycle;      /* NMEA request cycle (ms) (0:no) */
		int nstr;           /* number of streams (1 input + (nstr-1) outputs */
		int npb;            /* data length in peek buffer (bytes) */
		double nmeapos[3];  /* NMEA request position (ecef) (m) */
		unsigned char *buff; /* input buffers */
		unsigned char *pbuf; /* peek buffer */
		unsigned int tick;  /* start tick */
		stream_t stream[16]; /* input/output streams */
		thread_t thread;    /* server thread */
		lock_t lock;        /* lock flag */
	} strsvr_t;

	typedef struct {
		int  prn;
		int  sys;
		char id[4];
		char type[21];
	} satFlag_t;

	typedef struct {        /* RTK server type */
		int state;          /* server state (0:stop,1:running) */
		int cycle;          /* processing cycle (ms) */
		int nmeacycle;      /* NMEA request cycle (ms) (0:no req) */
		int nmeareq;        /* NMEA request (0:no,1:nmeapos,2:single sol) */
		double nmeapos[3];  /* NMEA request position (ecef) (m) */
		int buffsize;       /* input buffer size (bytes) */
		int format[3];      /* input format {rov,base,corr} */
		solopt_t solopt[2]; /* output solution options {sol1,sol2} */
		int navsel;         /* ephemeris select (0:all,1:rover,2:base,3:corr) */
		int nsbs;           /* number of sbas message */
		int nsol;           /* number of solution buffer */
		rtk_t rtk;          /* RTK control/result struct */
		int nb[4];         /* bytes in input buffers {rov,base} */
		int nsb[2];         /* bytes in soulution buffers */
		int npb[4];         /* bytes in input peek buffers */
		unsigned char *buff[4]; /* input buffers {rov,base,corr} */
		unsigned char *sbuf[2]; /* output buffers {sol1,sol2} */
		unsigned char *pbuf[4]; /* peek buffers {rov,base,corr} */
		sol_t solbuf[MAXSOLBUF]; /* solution buffer */
		unsigned int nmsg[4][10]; /* input message counts */
		raw_t  raw[4];     /* receiver raw control {rov,base,corr} */
		rtcm_t rtcm[4];     /* RTCM control {rov,base,corr} */
		gtime_t ftime[3];   /* download time {rov,base,corr} */
		char files[3][MAXSTRPATH]; /* download paths {rov,base,corr} */
		obs_t obs[4][MAXOBSBUF]; /* observation data {rov,base,corr} */
		nav_t nav;          /* navigation data */
		stream_t stream[8]; /* streams {rov,base,corr,sol1,sol2,logr,logb,logc} */
		stream_t *moni;     /* monitor stream */
		unsigned int tick;  /* start tick */
		thread_t thread;    /* server thread */
		int cputime;        /* CPU time (ms) for a processing cycle */
		int prcout;         /* missing observation data count */
		lock_t lock;        /* lock flag */
		satFlag_t sFlag[MAXSAT];
	} rtksvr_t;
    extern rtksvr_t svr1;
	typedef struct {        /* file options type */
		char *outf[50];
	} fileout_t;

//    fileout_t file_out;
//    FILE* outFp[50];   //输出文件指针
	typedef void fatalfunc_t(const char *); /* fatal callback function type */
   extern int startppp(int *strs,char streamPaths[8][1024],char inputFilePaths[2][1024],char outResultPath[1024]);
											/* global variables ----------------------------------------------------------*/
	extern const double chisqr[];           /* chi-sqr(n) table (alpha=0.001) */
	extern const double lam_carr[];         /* carrier wave length (m) {L1,L2,...} */
    extern const prcopt_t prcopt_default;   /* default positioning options */
	extern const solopt_t solopt_default;   /* default solution output options */
	extern const char *formatstrs[];        /* stream format strings */
	extern opt_t sysopts[];                 /* system options table */

											/* satellites, systems, codes functions --------------------------------------*/
	extern int  satno(int sys, int prn);
	extern int  satsys(int sat, int *prn);
	extern int  satid2no(const char *id);
	extern void satno2id(int sat, char *id);
	extern unsigned char obs2code(const char *obs, int *freq);
	extern char *code2obs(unsigned char code, int *freq);
	extern int  satexclude(int sat, int svh, const prcopt_t *opt);
	extern int  testsnr(int base, int freq, double el, double snr,
		const snrmask_t *mask);
	extern void setcodepri(int sys, int freq, const char *pri);
	extern int  getcodepri(int sys, unsigned char code, const char *opt);

	/* matrix and vector functions -----------------------------------------------*/
	extern double *mat(int n, int m);
	extern int    *imat(int n, int m);
	extern double *zeros(int n, int m);
	extern double *eye(int n);
	extern double dot(const double *a, const double *b, int n);
	extern double norm(const double *a, int n);
	extern void cross3(const double *a, const double *b, double *c);
	extern int  normv3(const double *a, double *b);
	extern void matcpy(double *A, const double *B, int n, int m);
	extern void matmul(const char *tr, int n, int k, int m, double alpha,
		const double *A, const double *B, double beta, double *C);
	extern int  matinv(double *A, int n);
	extern int  solve(const char *tr, const double *A, const double *Y, int n,
		int m, double *X);
	extern int  lsq(const double *A, const double *y, int n, int m, double *x,
		double *Q);
    //extern int filter(double *x, double *P, const double *H, const double *v, double *R, int n, int m, int *nn, double *std);
    extern int filter(double *x, double *P, const double *H, const double *v, double *R, int n, int m);
	extern int  smoother(const double *xf, const double *Qf, const double *xb,
		const double *Qb, int n, double *xs, double *Qs);
	extern void matprint(const double *A, int n, int m, int p, int q);
	extern void matfprint(const double *A, int n, int m, int p, int q, FILE *fp);

	extern void add_fatal(fatalfunc_t *func);

	/* time and string functions -------------------------------------------------*/
	extern double  str2num(const char *s, int i, int n);
	extern int     str2time(const char *s, int i, int n, gtime_t *t);
	extern void    time2str(gtime_t t, char *str, int n);
	extern gtime_t epoch2time(const double *ep);
	extern void    time2epoch(gtime_t t, double *ep);
	extern gtime_t gpst2time(int week, double sec);
	extern double  time2gpst(gtime_t t, int *week);
	extern gtime_t gst2time(int week, double sec);
	extern double  time2gst(gtime_t t, int *week);
	extern gtime_t bdt2time(int week, double sec);
	extern double  time2bdt(gtime_t t, int *week);
	extern char    *time_str(gtime_t t, int n);

	extern gtime_t timeadd(gtime_t t, double sec);
	extern double  timediff(gtime_t t1, gtime_t t2);
	extern gtime_t gpst2utc(gtime_t t);
	extern gtime_t utc2gpst(gtime_t t);
	extern gtime_t gpst2bdt(gtime_t t);
	extern gtime_t bdt2gpst(gtime_t t);
	extern gtime_t timeget(void);
	extern void    timeset(gtime_t t);
	extern double  time2doy(gtime_t t);
	extern double  utc2gmst(gtime_t t, double ut1_utc);
	extern int read_leaps(const char *file);

	extern int adjgpsweek(int week);
	extern unsigned int tickget(void);
	extern void sleepms(int ms);

	extern int reppath(const char *path, char *rpath, gtime_t time, const char *rov,
		const char *base);
	extern int reppaths(const char *path, char *rpaths[], int nmax, gtime_t ts,
		gtime_t te, const char *rov, const char *base);

	/* coordinates transformation ------------------------------------------------*/
	extern void ecef2pos(const double *r, double *pos);
	extern void pos2ecef(const double *pos, double *r);
	extern void ecef2enu(const double *pos, const double *r, double *e);
	extern void enu2ecef(const double *pos, const double *e, double *r);
	extern void covenu(const double *pos, const double *P, double *Q);
	extern void covecef(const double *pos, const double *Q, double *P);
	extern void xyz2enu(const double *pos, double *E);
	extern void eci2ecef(gtime_t tutc, const double *erpv, double *U, double *gmst);
	extern void deg2dms(double deg, double *dms);
	extern double dms2deg(const double *dms);

	/* input and output functions ------------------------------------------------*/
	extern void readpos(const char *file, const char *rcv, double *pos);
	extern int  sortobs(obs_t *obs);
	extern void uniqnav(nav_t *nav);
	extern int  screent(gtime_t time, gtime_t ts, gtime_t te, double tint);
	extern int  readnav(const char *file, nav_t *nav);
	extern int  savenav(const char *file, const nav_t *nav);
	extern void freeobs(obs_t *obs);
	extern void freenav(nav_t *nav, int opt);
	extern int  readblq(const char *file, const char *sta, double *odisp);
	extern int  readerp(const char *file, erp_t *erp);
	extern int  geterp(const erp_t *erp, gtime_t time, double *val);

	/* debug trace functions -----------------------------------------------------*/
	extern void traceopen(const char *file);
	extern void traceclose(void);
	extern void tracelevel(int level);
	extern void trace(int level, const char *format, ...);
	extern void tracet(int level, const char *format, ...);
	extern void tracemat(int level, const double *A, int n, int m, int p, int q);
	extern void traceobs(int level, const obsd_t *obs, int n);
	extern void tracenav(int level, const nav_t *nav);
	extern void tracegnav(int level, const nav_t *nav);
	extern void tracehnav(int level, const nav_t *nav);
	extern void tracepeph(int level, const nav_t *nav);
	extern void tracepclk(int level, const nav_t *nav);
	extern void traceb(int level, const unsigned char *p, int n);

	/* platform dependent functions ----------------------------------------------*/
	extern int execcmd(const char *cmd);
	extern int expath(const char *path, char *paths[], int nmax);
	extern void createdir(const char *path);

	/* positioning models --------------------------------------------------------*/
	extern double satwavelen(int sat, int frq, const nav_t *nav);
	extern double satazel(const double *pos, const double *e, double *azel);
	extern double geodist(const double *rs, const double *rr, double *e);
	extern void dops(int ns, const double *azel, double elmin, double *dop);
	extern void csmooth(obs_t *obs, int ns);

	/* atmosphere models ---------------------------------------------------------*/
	extern double ionmodel(gtime_t t, const double *ion, const double *pos,
		const double *azel);
	extern double ionmapf(const double *pos, const double *azel);
	extern double ionppp(const double *pos, const double *azel, double re,
		double hion, double *pppos);
	extern double tropmodel(gtime_t time, const double *pos, const double *azel,
		double humi, double *zwd, int atmodel);
	extern double tropmapf(gtime_t time, const double pos[], const double azel[],
		double *mapfw);
	extern int ionocorr(gtime_t time, const nav_t *nav, int sat, const double *pos,
		const double *azel, int ionoopt, double *ion, double *var);
	/* antenna models ------------------------------------------------------------*/
	extern int  readpcv(const char *file, pcvs_t *pcvs);
	extern pcv_t *searchpcv(int sat, const char *type, gtime_t time,
		const pcvs_t *pcvs);
	//extern void antmodel(const pcv_t *pcv, const double *del, const double *azel,
	//                     int opt, double *dant);

	/* earth tide models ---------------------------------------------------------*/
	extern void sunmoonpos(gtime_t tutc, const double *erpv, double *rsun,
		double *rmoon, double *gmst);
	extern void tidedisp(gtime_t tutc, const double *rr, int opt, const erp_t *erp,
		const double *odisp, double *dr);

	/* geiod models --------------------------------------------------------------*/
	extern int opengeoid(int model, const char *file);
	extern void closegeoid(void);
	extern double geoidh(const double *pos);



	/* rinex functions -----------------------------------------------------------*/

	extern int readrnxt(const char *file, int rcv, gtime_t ts, gtime_t te,
		double tint, const char *opt, obs_t *obs, nav_t *nav,
		sta_t *sta);
	extern int readrnxc(const char *file, nav_t *nav);


	/* ephemeris and clock functions ---------------------------------------------*/
	extern double eph2clk(gtime_t time, const eph_t  *eph);
	extern double geph2clk(gtime_t time, const geph_t *geph);
	extern double seph2clk(gtime_t time, const seph_t *seph);
	extern void eph2pos(gtime_t time, const eph_t  *eph, double *rs, double *dts,
		double *var);
	extern void geph2pos(gtime_t time, const geph_t *geph, double *rs, double *dts,
		double *var);
	extern void seph2pos(gtime_t time, const seph_t *seph, double *rs, double *dts,
		double *var);
	extern int  peph2pos(gtime_t time, int sat, const nav_t *nav, int opt,
		double *rs, double *dts, double *var);
	extern void satantoff(gtime_t time, const double *rs, int sat, const nav_t *nav,
		double *dant);
	extern int  satpos(gtime_t time, gtime_t teph, int sat, int ephopt,
		const nav_t *nav, double *rs, double *dts, double *var,
		int *svh);
	extern void satposs(gtime_t time, const obsd_t *obs, int n, const nav_t *nav,
		int sateph, double *rs, double *dts, double *var, int *svh);
	extern void readsp3(const char *file, nav_t *nav, int opt);
	extern int  readsap(const char *file, gtime_t time, nav_t *nav);
	extern int  readdcb(const char *file, nav_t *nav, const sta_t *sta);
	extern int  readfcb(const char *file, nav_t *nav);
	extern void alm2pos(gtime_t time, const alm_t *alm, double *rs, double *dts);


	/* receiver raw data functions -----------------------------------------------*/
	extern unsigned int getbitu(const unsigned char *buff, int pos, int len);
	extern int          getbits(const unsigned char *buff, int pos, int len);
	extern void setbitu(unsigned char *buff, int pos, int len, unsigned int data);
	extern void setbits(unsigned char *buff, int pos, int len, int data);
	extern unsigned int crc32(const unsigned char *buff, int len);
	extern unsigned int crc24q(const unsigned char *buff, int len);
	extern unsigned short crc16(const unsigned char *buff, int len);



	/* rtcm functions ------------------------------------------------------------*/
	extern int init_rtcm(rtcm_t *rtcm);
	extern void free_rtcm(rtcm_t *rtcm);

	extern int input_rtcm3(rtcm_t *rtcm, unsigned char data);
	extern int input_rtcm3f(rtcm_t *rtcm, FILE *fp);


	/* solution functions --------------------------------------------------------*/
	extern void initsolbuf(solbuf_t *solbuf, int cyclic, int nmax);
	extern void freesolbuf(solbuf_t *solbuf);
	extern void freesolstatbuf(solstatbuf_t *solstatbuf);
	extern sol_t *getsol(solbuf_t *solbuf, int index);
	extern int addsol(solbuf_t *solbuf, const sol_t *sol);
	extern int readsol(char *files[], int nfile, solbuf_t *sol);
	extern int readsolt(char *files[], int nfile, gtime_t ts, gtime_t te,
		double tint, int qflag, solbuf_t *sol);
	extern int readsolstat(char *files[], int nfile, solstatbuf_t *statbuf);
	extern int readsolstatt(char *files[], int nfile, gtime_t ts, gtime_t te,
		double tint, solstatbuf_t *statbuf);
	extern int inputsol(unsigned char data, gtime_t ts, gtime_t te, double tint,
		int qflag, const solopt_t *opt, solbuf_t *solbuf);

	extern int outprcopts(unsigned char *buff, const prcopt_t *opt);
	extern int outsolheads(unsigned char *buff, const solopt_t *opt);
	extern int outsols(unsigned char *buff, const sol_t *sol, const double *rb,
		const solopt_t *opt);
	extern int outsolexs(unsigned char *buff, const sol_t *sol, const ssat_t *ssat,
		const solopt_t *opt);
	extern void outprcopt(FILE *fp, const prcopt_t *opt);
	extern void outsolhead(FILE *fp, const solopt_t *opt);
	extern void outsol(FILE *fp, const sol_t *sol, const double *rb,
		const solopt_t *opt);
	extern void outsolex(FILE *fp, const sol_t *sol, const ssat_t *ssat,
		const solopt_t *opt);
	extern int outnmea_rmc(unsigned char *buff, const sol_t *sol);
	extern int outnmea_gga(unsigned char *buff, const sol_t *sol);
	extern int outnmea_gsa(unsigned char *buff, const sol_t *sol,
		const ssat_t *ssat);
	extern int outnmea_gsv(unsigned char *buff, const sol_t *sol,
		const ssat_t *ssat);

	/* google earth kml converter ------------------------------------------------*/
	extern int convkml(const char *infile, const char *outfile, gtime_t ts,
		gtime_t te, double tint, int qflg, double *offset,
		int tcolor, int pcolor, int outalt, int outtime);


	/* options functions ---------------------------------------------------------*/
	extern opt_t *searchopt(const char *name, const opt_t *opts);
	extern int str2opt(opt_t *opt, const char *str);
	extern int opt2str(const opt_t *opt, char *str);
	extern int opt2buf(const opt_t *opt, char *buff);
	extern int loadopts(const char *file, opt_t *opts);
	extern int saveopts(const char *file, const char *mode, const char *comment,
		const opt_t *opts);
	extern void resetsysopts(void);
    //extern void getsysopts(prcopt_t *popt, solopt_t *sopt, filopt_t *fopt);
    extern void getsysopts(solopt_t *sopt, filopt_t *fopt);
	extern void setsysopts(const prcopt_t *popt, const solopt_t *sopt,
		const filopt_t *fopt);

	/* stream data input and output functions ------------------------------------*/
	extern void strinitcom(void);
	extern void strinit(stream_t *stream);
	extern void strlock(stream_t *stream);
	extern void strunlock(stream_t *stream);
	extern int  stropen(stream_t *stream, int type, int mode, const char *path);
	extern void strclose(stream_t *stream);
	extern int  strread(stream_t *stream, unsigned char *buff, int n);
	extern int  strwrite(stream_t *stream, unsigned char *buff, int n);
	extern int  strstat(stream_t *stream, char *msg);
	extern void strsum(stream_t *stream, int *inb, int *inr, int *outb, int *outr);
	extern void strsetopt(const int *opt);
	extern gtime_t strgettime(stream_t *stream);
	extern void strsendnmea(stream_t *stream, const double *pos);
	extern void strsendcmd(stream_t *stream, const char *cmd);
	extern void strsettimeout(stream_t *stream, int toinact, int tirecon);
	extern void strsetdir(const char *dir);
	extern void strsetproxy(const char *addr);

	/* integer ambiguity resolution ----------------------------------------------*/
	extern int lambda(int n, int m, const double *a, const double *Q, double *F,
		double *s);
	extern int lambda_reduction(int n, const double *Q, double *Z);
	extern int lambda_search(int n, int m, const double *a, const double *Q,
		double *F, double *s);

	/* standard positioning ------------------------------------------------------*/
	extern int pntpos(const obsd_t *obs, int n, const nav_t *nav,
		const prcopt_t *opt, sol_t *sol, double *azel,
		ssat_t *ssat, char *msg);

	/* precise positioning -------------------------------------------------------*/
	extern void rtkinit(rtk_t *rtk, const prcopt_t *opt);
	extern void rtkfree(rtk_t *rtk);
	extern int  rtkpos(rtk_t *rtk, obsd_t *obs, int nobs, const nav_t *nav);
	extern int  rtkopenstat(const char *file, int level);
	extern void rtkclosestat(void);
	extern int  rtkoutstat(rtk_t *rtk, char *buff);

	/* precise point positioning -------------------------------------------------*/
	extern void pppos(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav);
	extern int pppnx(const prcopt_t *opt);
	extern int pppoutstat(rtk_t *rtk, char *buff);

	extern int ppp_ar(rtk_t *rtk, const obsd_t *obs, int n, int *exc,
		const nav_t *nav, const double *azel, double *x, double *P);


	/* rtk server functions ------------------------------------------------------*/
	extern int  rtksvrinit(rtksvr_t *svr);

    extern void rtksvrstop(rtksvr_t *svr);
	extern int  rtksvropenstr(rtksvr_t *svr, int index, int str, const char *path,
		const solopt_t *solopt);
	extern void rtksvrlock(rtksvr_t *svr);
	extern void rtksvrunlock(rtksvr_t *svr);
    extern int  rtksvrostat (rtksvr_t *svr, int type, gtime_t *time, int *sat,
                             double *az, double *el, int **snr, int *vsat);

	/* downloader functions ------------------------------------------------------*/

	extern void readotl(prcopt_t *popt, const char *file, const sta_t *sta);
	extern void setpcv(rtksvr_t *svr, gtime_t time, prcopt_t *popt, nav_t *nav, const pcvs_t *pcvs,
		const pcvs_t *pcvr, const sta_t *sta);

	/* application defined functions ---------------------------------------------*/
	extern int showmsg(char *format, ...);
	extern void settspan(gtime_t ts, gtime_t te);
	extern void settime(gtime_t time);

	/* qzss lex functions --------------------------------------------------------*/
	extern int myRound(const double dNum);
	extern void writesol(rtksvr_t *svr, int index);
	extern void writesolhead(stream_t *stream, const solopt_t *solopt);
	extern void saveoutbuf(rtksvr_t *svr, unsigned char *buff, int n, int index);
	extern void readotl(prcopt_t *popt, const char *file, const sta_t *sta);
	extern void decodefile(rtksvr_t *svr, int index);
	extern void corr_phase_bias(obsd_t *obs, int n, const nav_t *nav);
	extern int decoderaw(rtksvr_t *svr, int index);
	extern void antmodel(int sat, const pcv_t *pcv, const double *del, const double *azel,
		int opt, double *dant);
	extern void antmodel_s(int sat, const pcv_t *pcv, double nadir, double *dant);
	extern void select_combination(const int l, const int p, const int n, const int m, int *sn);
	extern int lsqPlus(const double *A, const double *y, const int nx, const int nv, double *x, double *Q);
	extern void getGPT(const double *pos, double dmjd, double *pres, double *temp, double *undu);
	extern int spp(const obsd_t *obs, int n, const nav_t *nav, const prcopt_t *opt,
		sol_t *sol, double *azel, ssat_t *ssat, char *msg);


	extern int findGross(int ppp, int bMulGnss, double *v, const int nv, const int nbad,
		double *std_ex, double *ave_ex, int *ibadsn, const double ratio,
		const double minv, const double stdmin);
	extern int uncompress(const char *file, char *uncfile);
	//
	//#define MW_ARI_NULLREAD 0x10    /* Null read (to start debugger) */
	//#define MW_ARI_ABORT    0x04    /* ARI handler says: abort program! */
	//#define MW_ARI_RETRY    0x02    /* ARI handler says: retry action! */
	//#define MW_ARI_IGNORE   0x01    /* ARI handler says: ignore error! */
	//
	//#define MW_VAL_NEW      0xFE    /* value in newly allocated memory */
	//#define MW_VAL_DEL      0xFD    /* value in newly deleted memory */
	//#define MW_VAL_NML      0xFC    /* value in no-mans-land */
	//#define MW_VAL_GRB      0xFB    /* value in grabbed memory */
	//
	//#define MW_TEST_ALL     0xFFFF  /* perform all tests */
	//#define MW_TEST_CHAIN   0x0001  /* walk the heap chain */
	//#define MW_TEST_ALLOC   0x0002  /* test allocations & NML guards */
	//#define MW_TEST_NML     0x0004  /* test all-NML areas for modifications */
	//
	//#define MW_NML_NONE     0       /* no NML */
	//#define MW_NML_FREE     1       /* turn FREE'd memory into NML */
	//#define MW_NML_ALL      2       /* all unused memory is NML */
	//#define MW_NML_DEFAULT  0       /* the default NML setting */
	//
	//#define MW_STAT_GLOBAL  0       /* only global statistics collected */
	//#define MW_STAT_MODULE  1       /* collect statistics on a module basis */
	//#define MW_STAT_LINE    2       /* collect statistics on a line basis */
	//#define MW_STAT_DEFAULT 0       /* the default statistics setting */
	//
	///*
	//** MemWatch internal constants
	//**  You may change these and recompile MemWatch to change the limits
	//**  of some parameters. Respect the recommended minimums!
	//*/
	//#define MW_TRACE_BUFFER 2048    /* (min 160) size of TRACE()'s output buffer */
	//#define MW_FREE_LIST    64      /* (min 4) number of free()'s to track */
	//
	///*
	//** Exported variables
	//**  In case you have to remove the 'const' keyword because your compiler
	//**  doesn't support it, be aware that changing the values may cause
	//**  unpredictable behaviour.
	//**  - mwCounter contains the current action count. You can use this to
	//**      place breakpoints using a debugger, if you want.
	//*/
	//#ifndef __MEMWATCH_C
	//extern const unsigned long mwCounter;
	//#endif
	//
	///*
	//** System functions
	//**  Normally, it is not nessecary to call any of these. MEMWATCH will
	//**  automatically initialize itself on the first MEMWATCH function call,
	//**  and set up a call to mwAbort() using atexit(). Some C++ implementations
	//**  run the atexit() chain before the program has terminated, so you
	//**  may have to use mwInit() or the MemWatch C++ class to get good
	//**  behaviour.
	//**  - mwInit() can be called to disable the atexit() usage. If mwInit()
	//**      is called directly, you must call mwTerm() to end MemWatch, or
	//**      mwAbort().
	//**  - mwTerm() is usually not nessecary to call; but if called, it will
	//**      call mwAbort() if it finds that it is cancelling the 'topmost'
	//**      mwInit() call.
	//**  - mwAbort() cleans up after MEMWATCH, reports unfreed buffers, etc.
	//*/
	//void  mwInit(void);
	//void  mwTerm(void);
	//void  mwAbort(void);
	//
	///*
	//** Setup functions
	//**  These functions control the operation of MEMWATCH's protective features.
	//**  - mwFlushNow() causes MEMWATCH to flush it's buffers.
	//**  - mwDoFlush() controls whether MEMWATCH flushes the disk buffers after
	//**      writes. The default is smart flushing: MEMWATCH will not flush buffers
	//**      explicitly until memory errors are detected. Then, all writes are
	//**      flushed until program end or mwDoFlush(0) is called.
	//**  - mwLimit() sets the allocation limit, an arbitrary limit on how much
	//**      memory your program may allocate in bytes. Used to stress-test app.
	//**      Also, in virtual-memory or multitasking environs, puts a limit on
	//**      how much MW_NML_ALL can eat up.
	//**  - mwGrab() grabs up X kilobytes of memory. Allocates actual memory,
	//**      can be used to stress test app & OS both.
	//**  - mwDrop() drops X kilobytes of grabbed memory.
	//**  - mwNoMansLand() sets the behaviour of the NML logic. See the
	//**      MW_NML_xxx for more information. The default is MW_NML_DEFAULT.
	//**  - mwStatistics() sets the behaviour of the statistics collector. See
	//**      the MW_STAT_xxx defines for more information. Default MW_STAT_DEFAULT.
	//**  - mwFreeBufferInfo() enables or disables the tagging of free'd buffers
	//**      with freeing information. This information is written in text form,
	//**      using sprintf(), so it's pretty slow. Disabled by default.
	//**  - mwAutoCheck() performs a CHECK() operation whenever a MemWatch function
	//**      is used. Slows down performance, of course.
	//**  - mwCalcCheck() calculates checksums for all data buffers. Slow!
	//**  - mwDumpCheck() logs buffers where stored & calc'd checksums differ. Slow!!
	//**  - mwMark() sets a generic marker. Returns the pointer given.
	//**  - mwUnmark() removes a generic marker. If, at the end of execution, some
	//**      markers are still in existence, these will be reported as leakage.
	//**      returns the pointer given.
	//*/
	//void        mwFlushNow(void);
	//void        mwDoFlush(int onoff);
	//void        mwLimit(long bytes);
	//unsigned    mwGrab(unsigned kilobytes);
	//unsigned    mwDrop(unsigned kilobytes);
	//void        mwNoMansLand(int mw_nml_level);
	//void        mwStatistics(int level);
	//void        mwFreeBufferInfo(int onoff);
	//void        mwAutoCheck(int onoff);
	//void        mwCalcCheck(void);
	//void        mwDumpCheck(void);
	//void *      mwMark(void *p, const char *description, const char *file, unsigned line);
	//void *      mwUnmark(void *p, const char *file, unsigned line);
	//
	///*
	//** Testing/verification/tracing
	//**  All of these macros except VERIFY() evaluates to a null statement
	//**  if MEMWATCH is not defined during compilation.
	//**  - mwIsReadAddr() checks a memory area for read privilige.
	//**  - mwIsSafeAddr() checks a memory area for both read & write privilige.
	//**      This function and mwIsReadAddr() is highly system-specific and
	//**      may not be implemented. If this is the case, they will default
	//**      to returning nonzero for any non-NULL pointer.
	//**  - CHECK() does a complete memory integrity test. Slow!
	//**  - CHECK_THIS() checks only selected components.
	//**  - CHECK_BUFFER() checks the indicated buffer for errors.
	//**  - mwASSERT() or ASSERT() If the expression evaluates to nonzero, execution continues.
	//**      Otherwise, the ARI handler is called, if present. If not present,
	//**      the default ARI action is taken (set with mwSetAriAction()).
	//**      ASSERT() can be disabled by defining MW_NOASSERT.
	//**  - mwVERIFY() or VERIFY() works just like ASSERT(), but when compiling without
	//**      MEMWATCH the macro evaluates to the expression.
	//**      VERIFY() can be disabled by defining MW_NOVERIFY.
	//**  - mwTRACE() or TRACE() writes some text and data to the log. Use like printf().
	//**      TRACE() can be disabled by defining MW_NOTRACE.
	//*/
	//int   mwIsReadAddr(const void *p, unsigned len);
	//int   mwIsSafeAddr(void *p, unsigned len);
	//int   mwTest(const char *file, int line, int mw_test_flags);
	//int   mwTestBuffer(const char *file, int line, void *p);
	//int   mwAssert(int, const char*, const char*, int);
	//int   mwVerify(int, const char*, const char*, int);
	//
	///*
	//** User I/O functions
	//**  - mwTrace() works like printf(), but dumps output either to the
	//**      function specified with mwSetOutFunc(), or the log file.
	//**  - mwPuts() works like puts(), dumps output like mwTrace().
	//**  - mwSetOutFunc() allows you to give the adress of a function
	//**      where all user output will go. (exeption: see mwSetAriFunc)
	//**      Specifying NULL will direct output to the log file.
	//**  - mwSetAriFunc() gives MEMWATCH the adress of a function to call
	//**      when an 'Abort, Retry, Ignore' question is called for. The
	//**      actual error message is NOT printed when you've set this adress,
	//**      but instead it is passed as an argument. If you call with NULL
	//**      for an argument, the ARI handler is disabled again. When the
	//**      handler is disabled, MEMWATCH will automatically take the
	//**      action specified by mwSetAriAction().
	//**  - mwSetAriAction() sets the default ARI return value MEMWATCH should
	//**      use if no ARI handler is specified. Defaults to MW_ARI_ABORT.
	//**  - mwAriHandler() is an ANSI ARI handler you can use if you like. It
	//**      dumps output to stderr, and expects input from stdin.
	//**  - mwBreakOut() is called in certain cases when MEMWATCH feels it would
	//**      be nice to break into a debugger. If you feel like MEMWATCH, place
	//**      an execution breakpoint on this function.
	//*/
	//void  mwTrace(const char* format_string, ...);
	//void  mwPuts(const char* text);
	//void  mwSetOutFunc(void(*func)(int));
	//void  mwSetAriFunc(int(*func)(const char*));
	//void  mwSetAriAction(int mw_ari_value);
	//int   mwAriHandler(const char* cause);
	//void  mwBreakOut(const char* cause);
	//
	///*
	//** Allocation/deallocation functions
	//**  These functions are the ones actually to perform allocations
	//**  when running MEMWATCH, for both C and C++ calls.
	//**  - mwMalloc() debugging allocator
	//**  - mwMalloc_() always resolves to a clean call of malloc()
	//**  - mwRealloc() debugging re-allocator
	//**  - mwRealloc_() always resolves to a clean call of realloc()
	//**  - mwCalloc() debugging allocator, fills with zeros
	//**  - mwCalloc_() always resolves to a clean call of calloc()
	//**  - mwFree() debugging free. Can only free memory which has
	//**      been allocated by MEMWATCH.
	//**  - mwFree_() resolves to a) normal free() or b) debugging free.
	//**      Can free memory allocated by MEMWATCH and malloc() both.
	//**      Does not generate any runtime errors.
	//*/
	//void* mwMalloc(size_t, const char*, int);
	//void* mwMalloc_(size_t);
	//void* mwRealloc(void *, size_t, const char*, int);
	//void* mwRealloc_(void *, size_t);
	//void* mwCalloc(size_t, size_t, const char*, int);
	//void* mwCalloc_(size_t, size_t);
	//void  mwFree(void*, const char*, int);
	//void  mwFree_(void*);
	//char* mwStrdup(const char *, const char*, int);
	//
	///*
	//** Enable/disable precompiler block
	//**  This block of defines and if(n)defs make sure that references
	//**  to MEMWATCH is completely removed from the code if the MEMWATCH
	//**  manifest constant is not defined.
	//*/
	//#ifndef __MEMWATCH_C
	//#ifdef MEMWATCH
	//
	//#define mwASSERT(exp)   while(mwAssert((int)(exp),#exp,__FILE__,__LINE__))
	//#ifndef MW_NOASSERT
	//#ifndef ASSERT
	//#define ASSERT          mwASSERT
	//#endif /* !ASSERT */
	//#endif /* !MW_NOASSERT */
	//#define mwVERIFY(exp)   while(mwVerify((int)(exp),#exp,__FILE__,__LINE__))
	//#ifndef MW_NOVERIFY
	//#ifndef VERIFY
	//#define VERIFY          mwVERIFY
	//#endif /* !VERIFY */
	//#endif /* !MW_NOVERIFY */
	//#define mwTRACE         mwTrace
	//#ifndef MW_NOTRACE
	//#ifndef TRACE
	//#define TRACE           mwTRACE
	//#endif /* !TRACE */
	//#endif /* !MW_NOTRACE */
	//
	///* some compilers use a define and not a function */
	///* for strdup(). */
	//#ifdef strdup
	//#undef strdup
	//#endif
	//
	//#define malloc(n)       mwMalloc(n,__FILE__,__LINE__)
	//#define strdup(p)       mwStrdup(p,__FILE__,__LINE__)
	//#define realloc(p,n)    mwRealloc(p,n,__FILE__,__LINE__)
	//#define calloc(n,m)     mwCalloc(n,m,__FILE__,__LINE__)
	//#define free(p)         mwFree(p,__FILE__,__LINE__)
	//#define CHECK()         mwTest(__FILE__,__LINE__,MW_TEST_ALL)
	//#define CHECK_THIS(n)   mwTest(__FILE__,__LINE__,n)
	//#define CHECK_BUFFER(b) mwTestBuffer(__FILE__,__LINE__,b)
	//#define MARK(p)         mwMark(p,#p,__FILE__,__LINE__)
	//#define UNMARK(p)       mwUnmark(p,__FILE__,__LINE__)
	//
	//#else /* MEMWATCH */
	//
	//#define mwASSERT(exp)
	//#ifndef MW_NOASSERT
	//#ifndef ASSERT
	//#define ASSERT          mwASSERT
	//#endif /* !ASSERT */
	//#endif /* !MW_NOASSERT */
	//
	//#define mwVERIFY(exp)    exp
	//#ifndef MW_NOVERIFY
	//#ifndef VERIFY
	//#define VERIFY          mwVERIFY
	//#endif /* !VERIFY */
	//#endif /* !MW_NOVERIFY */
	//
	///*lint -esym(773,mwTRACE) */
	//#define mwTRACE         /*lint -save -e506 */ 1?(void)0:mwDummyTraceFunction /*lint -restore */
	//#ifndef MW_NOTRACE
	//#ifndef TRACE
	///*lint -esym(773,TRACE) */
	//#define TRACE           mwTRACE
	//#endif /* !TRACE */
	//#endif /* !MW_NOTRACE */
	//
	//extern void mwDummyTraceFunction(const char *, ...);
	///*lint -save -e652 */
	//#define mwDoFlush(n)
	//#define mwPuts(s)
	//#define mwInit()
	//#define mwGrab(n)
	//#define mwDrop(n)
	//#define mwLimit(n)
	//#define mwTest(f,l)
	//#define mwSetOutFunc(f)
	//#define mwSetAriFunc(f)
	//#define mwDefaultAri()
	//#define mwNomansland()
	//#define mwStatistics(f)
	//#define mwMark(p,t,f,n)     (p)
	//#define mwUnmark(p,f,n)     (p)
	//#define mwMalloc(n,f,l)     malloc(n)
	//#define mwStrdup(p,f,l)     strdup(p)
	//#define mwRealloc(p,n,f,l)  realloc(p,n)
	//#define mwCalloc(n,m,f,l)   calloc(n,m)
	//#define mwFree(p)           free(p)
	//#define mwMalloc_(n)        malloc(n)
	//#define mwRealloc_(p,n)     realloc(p,n)
	//#define mwCalloc_(n,m)      calloc(n,m)
	//#define mwFree_(p)          free(p)
	//#define mwAssert(e,es,f,l)
	//#define mwVerify(e,es,f,l)  (e)
	//#define mwTrace             mwDummyTrace
	//#define mwTestBuffer(f,l,b) (0)
	//#define CHECK()
	//#define CHECK_THIS(n)
	//#define CHECK_BUFFER(b)
	//#define MARK(p)             (p)
	//#define UNMARK(p)           (p)
	///*lint -restore */
	//
	//#endif /* MEMWATCH */
	//#endif /* !__MEMWATCH_C */
	//
	//#ifdef __cplusplus
	//}
	//#endif
	//
	//#if 0 /* 980317: disabled C++ */
	//
	///*
	//** C++ support section
	//**  Implements the C++ support. Please note that in order to avoid
	//**  messing up library classes, C++ support is disabled by default.
	//**  You must NOT enable it until AFTER the inclusion of all header
	//**  files belonging to code that are not compiled with MEMWATCH, and
	//**  possibly for some that are! The reason for this is that a C++
	//**  class may implement it's own new() function, and the preprocessor
	//**  would substitute this crucial declaration for MEMWATCH new().
	//**  You can forcibly deny C++ support by defining MEMWATCH_NOCPP.
	//**  To enble C++ support, you must be compiling C++, MEMWATCH must
	//**  be defined, MEMWATCH_NOCPP must not be defined, and finally,
	//**  you must define 'new' to be 'mwNew', and 'delete' to be 'mwDelete'.
	//**  Unlike C, C++ code can begin executing *way* before main(), for
	//**  example if a global variable is created. For this reason, you can
	//**  declare a global variable of the class 'MemWatch'. If this is
	//**  is the first variable created, it will then check ALL C++ allocations
	//**  and deallocations. Unfortunately, this evaluation order is not
	//**  guaranteed by C++, though the compilers I've tried evaluates them
	//**  in the order encountered.
	//*/
	//#ifdef __cplusplus
	//#ifndef __MEMWATCH_C
	//#ifdef MEMWATCH
	//#ifndef MEMWATCH_NOCPP
	//extern int mwNCur;
	//extern const char *mwNFile;
	//extern int mwNLine;
	//class MemWatch {
	//public:
	//	MemWatch();
	//	~MemWatch();
	//};
	//void * operator new(size_t);
	//void * operator new(size_t, const char *, int);
	//void * operator new[](size_t, const char *, int);	// hjc 07/16/02
	//void operator delete(void *);
	//#define mwNew new(__FILE__,__LINE__)
	//#define mwDelete (mwNCur=1,mwNFile=__FILE__,mwNLine=__LINE__),delete
	//#endif /* MEMWATCH_NOCPP */
	//#endif /* MEMWATCH */
	//#endif /* !__MEMWATCH_C */
	//#endif /* __cplusplus */
	//
	//#endif /* 980317: disabled C++ */


#ifdef __cplusplus
}
#endif
#endif /* RTKLIB_H */
