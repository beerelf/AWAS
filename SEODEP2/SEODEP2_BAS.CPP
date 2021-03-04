// *************************************************************
//   Created with BCX -- The BASIC To C Translator (ver 5.05)
//  BCX (c) 1999, 2000, 2001, 2002, 2003, 2004 by Kevin Diggins
// *************************************************************
//     Translated for compiling using the Lcc-Win32 Compiler
// *************************************************************
#include <windows.h>    // Win32 Header File 
#include <windowsx.h>   // Win32 Header File 
#include <commctrl.h>   // Win32 Header File 
#include <mmsystem.h>   // Win32 Header File 
#include <shellapi.h>   // Win32 Header File 
#include <shlobj.h>     // Win32 Header File 
#include <richedit.h>   // Win32 Header File 
#include <wchar.h>      // Win32 Header File 
#include <objbase.h>    // Win32 Header File 
#include <ocidl.h>      // Win32 Header File 
#include <winuser.h>    // Win32 Header File 
#include <olectl.h>     // Win32 Header File 
#include <oaidl.h>      // Win32 Header File 
#include <ole2.h>       // Win32 Header File 
#include <oleauto.h>    // Win32 Header File 
#include <conio.h>
#include <direct.h>
#include <ctype.h>
#include <io.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include <stdarg.h>
#include <process.h>


// ***************************************************
// Compiler Defines
// ***************************************************

// C++
#if defined( __cplusplus )
#endif

// LCCWIN32 defs
#if defined( __LCC__ )
  #define USE_LCCWIN32C
#endif

// MingW32 defs
#if defined( __MINGW32__ )
  #define USE_MINGW32C
#endif

// PellesC defs
#if defined( __POCC__ )
  #ifndef _WINDOWS_H
    #include <windows.h>
  #endif
  #define USE_PELLESC
  #if !defined( __POCC__OLDNAMES )
    #define O_RDONLY         _O_RDONLY
    #define O_WRONLY         _O_WRONLY
    #define O_RDWR           _O_RDWR
    #define O_APPEND         _O_APPEND
    #define O_CREAT          _O_CREAT
    #define O_TRUNC          _O_TRUNC
    #define O_EXCL           _O_EXCL
    #define O_TEXT           _O_TEXT
    #define O_BINARY         _O_BINARY
    #define O_NOINHERIT      _O_NOINHERIT
    #define O_TEMPORARY      _O_TEMPORARY
    #define O_SEQUENTIAL     _O_SEQUENTIAL
    #define O_RANDOM         _O_RANDOM
    #define SH_DENYRW        _SH_DENYRW
    #define SH_DENYWR        _SH_DENYWR
    #define SH_DENYRD        _SH_DENYRD
    #define SH_DENYNO        _SH_DENYNO
    #define S_IREAD          _S_IREAD
    #define S_IWRITE         _S_IWRITE
    #define LK_UNLCK         _LK_UNLCK
    #define LK_LOCK          _LK_LOCK
    #define LK_NBLCK         _LK_NBLCK
    #define LK_RLCK          _LK_RLCK
    #define LK_NBRLCK        _LK_NBRLCK
    #define P_WAIT           _P_WAIT
    #define P_NOWAIT         _P_NOWAIT
    #define P_OVERLAY        _P_OVERLAY
    #define P_NOWAITO        _P_NOWAITO
    #define P_DETACH         _P_DETACH
    #define  access           _access
    #define  chdir            _chdir
    #define  chmod            _chmod
    #define  chsize           _chsize
    #define  close            _close
    #define  creat            _creat
    #define  cwait            _cwait
    #define  dup              _dup
    #define  dup2             _dup2
    #define  eof              _eof
    #define  fdopen           _fdopen
    #define  filelength       _filelength
    #define  fileno           _fileno
    #define  fstat            _fstat
    #define  ftime            _ftime
    #define  getch            _getch
    #define  getche           _getche
    #define  getcwd           _getcwd
    #define  getpid           _getpid
    #define  inp              _inp
    #define  inp              _inp
    #define  inpd             _inpd
    #define  inpw             _inpw
    #define  inpw             _inpw
    #define  isascii          _isascii
    #define  isatty           _isatty
    #define  itoa             _itoa
    #define  kbhit            _kbhit
    #define  locking          _locking
    #define  lrotl            _lrotl
    #define  lrotr            _lrotr
    #define  lseek            _lseek
    #define  ltoa             _ltoa
    #define  memicmp          _memicmp
    #define  mkdir            _mkdir
    #define  open             _open
    #define  out              _out
    #define  outp             _outp
    #define  outpd            _outpd
    #define  outpw            _outpw
    #define  outw             _outw
    #define  putch            _putch
    #define  putenv           _putenv
    #define  read             _read
    #define  rmdir            _rmdir
    #define  rotl             _rotl
    #define  rotr             _rotr
    #define  setmode          _setmode
    #define  sopen            _sopen
    #define  spawnl           _spawnl
    #define  spawnle          _spawnle
    #define  spawnlp          _spawnlp
    #define  spawnlpe         _spawnlpe
    #define  spawnv           _spawnv
    #define  spawnve          _spawnve
    #define  spawnvp          _spawnvp
    #define  spawnvpe         _spawnvpe
    #define  stat             _stat
    #define  strdup           _strdup
    #define  stricmp          _stricmp
    #define  strlwr           _strlwr
    #define  strnicmp         _strnicmp
    #define  strnset          _strnset
    #define  strrev           _strrev
    #define  strupr           _strupr
    #define  tell             _tell
    #define  ultoa            _ultoa
    #define  ungetch          _ungetch
    #define  unlink           _unlink
    #define  utime            _utime
    #define  wascii           _wascii
    #define  wcsdup           _wcsdup
    #define  wcsicmp          _wcsicmp
    #define  wcslwr           _wcslwr
    #define  wcsnicmp         _wcsnicmp
    #define  wcsnset          _wcsnset
    #define  wcsrev           _wcsrev
    #define  wcsupr           _wcsupr
    #define  write            _write
  #endif
  #define _asm    __asm
  #define strdate _strdate
  // ===== PellesC Libraries ==========
  #pragma comment(lib,"kernel32.lib")
  #pragma comment(lib,"user32.lib")
  #pragma comment(lib,"gdi32.lib")
  #pragma comment(lib,"comctl32.lib")
  #pragma comment(lib,"advapi32.lib")
  #pragma comment(lib,"winspool.lib")
  #pragma comment(lib,"shell32.lib")
  #pragma comment(lib,"ole32.lib")
  #pragma comment(lib,"oleaut32.lib")
  #pragma comment(lib,"uuid.lib")
  #pragma comment(lib,"odbc32.lib")
  #pragma comment(lib,"odbccp32.lib")
  #pragma comment(lib,"winmm.lib")
  #pragma comment(lib,"comdlg32.lib")
  // ==================================
#endif

// Open Watcom defs
#if defined( __WATCOMC__ )
  #define USE_WATCOMC
  #define strdate _strdate
  #define _fcloseall fcloseall
#endif

// Borland C++ 5.5.1 defs - bcc32.exe
#if defined( __BCPLUSPLUS__ )
  #define strdate _strdate
  #define USE_BCPLUSPLUS
  // ===== Borland Libraries ==========
  #pragma comment(lib,"import32.lib")
  #pragma comment(lib,"cw32.lib")
  // ==================================
#endif

// Borland C
#if defined( __BORLANDC__ )
  #define strdate _strdate
  #define USE_BORLANDC
  // ===== Borland Libraries ==========
  #pragma comment(lib,"import32.lib")
  #pragma comment(lib,"cw32.lib")
  // ==================================
#endif

// Microsoft C
#if defined( __MSC_VER )
  #define USE_MSC
#endif

#if !defined( __LCC__ )
#else
#endif
// *************************************************
//                System Variables
// *************************************************

char    Date [2048];
COORD   cursor;
HANDLE  hConsole;
int     color_fg = 7;
int     color_bg = 0;
int     ScanError;
char    BCX_STR [1024*1024];
char    InputBuffer[1048576];
jmp_buf GosubStack[32];
int     GosubNdx;

// *************************************************
//            User Global Variables
// *************************************************

static HDC     BcxPtr_hDC;
static long    BcxPtr_FontMetrix;
static long    BcxPtr_LineCtr;
static long    BcxPtr_PrinterOn;
static HFONT   BcxPtr_hFont;
static HFONT   BcxPtr_hFontOld;
static DOCINFO BcxPtr_di;
static LOGFONT BcxPtr_Lf;
static TEXTMETRIC BcxPtr_tm;
static char    BcxPtr_Text[2048];
static char    BcxPtr_Buffer[2048];
static FILE*  FP3;



// *************************************************
//               Standard Macros
// *************************************************

#define Cint(A)(A>=0)?(int)(A+0.5):(int)(A-0.5)
#define VAL(a)(double)atof(a)
#define FINT(a)floor(a)


// *************************************************
//               Standard Prototypes
// *************************************************

int     str_cmp(char*, char*);
void    cls(void);
void    locate (int,int,int=1,int=12);
char*   BCX_TmpStr(size_t);
char*   mid (char*, int, int=-1);
char*   left (char*,int);
char*   extract (char*,char*);
char*   str (double);
char*   RemoveStr (char*,char*);
char*   timef (int i=0);
char*   join (int, ... );
char*   chr(int,int=0,int=0,int=0,int=0,int=0,int=0,int=0,int=0,int=0);
int     instr(char*,char*,int=0,int=0);
char    *_stristr_(char*,char*);
double  Abs (double);
int     scan (char *input, char *format, ... );
int     Split (char [][2048], char*, char*, int=0);
int  PrinterOpen  (void);
void PrinterWrite (char*);
void EjectPage    (void);
void PrinterClose (void);

// *************************************************
//                  Main Program
// *************************************************

int SEODEP2_BAS(int argc, char *argv[])
{
  hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
  10;
  20;
  30;
  40;
  50WIDTH"LPT1:",150;
  60lprintchr(27);"@";
  70cls;
  80PRINT"                       ANALYTICAL STREAM DEPLETION MODEL";
  90PRINT"                         OFFICE OF THE STATE ENGINEER";
  100PRINT"                     COLORADO DIVISION OF WATER RESOURCES";
  110PRINT"                             Dewayne R. Schroeder";
  120PRINT"                                 May 24, 1984";
  // additional modifications to output to create output 
  // file by Jeff Foster, HRS, 7-89 
  printf("\n");
  printf("\n");
  printf("%s\n","This version modified by jsf to produce output file");
  printf("%s\n","which contains q/Q and v/V values for infinite and ");
  printf("%s\n","alluvial aquifer conditions (options 1 and 2).");
  printf("%s\n","NOTE that q/Q values are only meaningful for specific ");
  printf("%s\n","pumping schedules; constant pumping or constant pumping");
  printf("%s\n","followed by shutoff.  For variable pumping schedules,");
  printf("%s\n","q/Q is poorly defined.");
  for(x=1; x<=2; x+=1)
    {
      printf("\n");
    }
  140INPUT" Do you want double precision? (Y/N)                                             if computation involves a large number of image wells use double precision      ",NUT;
  if(str_cmp(NUT,"n")==0 || str_cmp(NUT,"N")==0)
    {
      J=1000;
      THENGOTO170;
    }
  else
    {
      J=790;
    }
  160DEFDBLA,C-H,K-M,O-W,Y-Z;
  170PI=3.141592653589793;
  E=2.718281828459045;
  180I=200;
  190DIML(18),LL(18),TU(4),DELT(I),q(I),ZZAT(I),QQ(J),VV(J),qd(J),vd(J),VPTP(I),AVP(I),LTP(365),QTP(365),H(202);
  200;
  210cls;
  220lineinput"ENTER 1st HEADER (not to exceed 70 characters)                                            "head1;head1;
  if(strlen(head1)>70)
    {
      printf("%s\n","HEADING TO LONG");
      THENGOTO220;
    }
  240lineinput"ENTER 2nd HEADER (not to exceed 70 characters)                                            "HEAD2;HEAD2;
  if(strlen(HEAD2)>70)
    {
      printf("%s\n","HEADING TO LONG");
      THENGOTO240;
    }
  260XMO=VAL(mid(strdate(Date),1,2));
  270FORx=1TOXMO;
  280READXMON;
  290NEXTx;
  sprintf(BCX_STR,"%s%s%s%s%s",XMON," ",mid( strdate(Date) , 4 , 2 ) ,", ",mid( strdate(Date) , 7 ) );
  strcpy(300DTE,BCX_STR);
  310DATA"JAN.","FEB.","MARCH","APRIL","MAY","JUNE","JULY","AUG.","SEPT.","OCT.","NOV.","DEC.";
  320;
  330cls;
  340PRINT"SELECT BOUNDARY CONDITIONS (ENTER ITEM NUMBER)";
  350PRINT"    1) INFINITE AQUIFER";
  360PRINT"    2) ALLUVIAL AQUIFER";
  370PRINT"    3) NO FLOW BOUNDARY PERPENDICULAR TO STREAM";
  380PRINT"    4) USE EFFECTIVE STREAM DEPLETION FACTOR TO APPROXIMATE EFFECT OF BOUNDARIES";
  390INPUTBI;
  if(BI==4)
    {
      strcpy((char*)ZZSEG,"N");
      THENGOTO440;
    }
  if(BI>4)
    {
      THENGOTO330;
    }
  420INPUT"DO YOU WANT TO COMPUTE DEPLETION FOR A SEGMENT OF STREAM (Y OR N)";ZZSEG;
  if(str_cmp(ZZSEG,"N")==0 || str_cmp(ZZSEG,"n")==0)
    {
      strcpy((char*)ZZSEG,"N");
    }
  else
    {
      strcpy((char*)ZZSEG,"Y");
    }
  440;
  450FORx=1TO18;
  strcpy(460L(x),(char*)" ");
  strcpy(470LL(x),(char*)" ");
  480PRINT;
  490NEXTx;
  if(str_cmp(ZZSEG,"N")==0)
    {
      THENGOTO540;
    }
  strcpy(510L(1),(char*)"      <--(-Z) : (+Z)-->");
  strcpy(520L(2),(char*)"    <---Z1--->:<---Z2--->");
  strcpy(530L(3),(char*)"    <------SEGMENT------>");
  strcpy(540L(4),(char*)"=============================");
  strcpy(550L(5),(char*)"  STREAM");
  strcpy(560L(12),(char*)"              O WELL");
  if(BI==4)
    {
      THENGOTO890;
    }
  if(BI==2)
    {
      THENGOTO760;
    }
  590FORx=6TO11;
  strcpy(600L(x),(char*)"              :");
  610NEXTx;
  strcpy(620L(5),(char*)"  STREAM      :");
  strcpy(630L(8),(char*)"              X");
  if(BI==1)
    {
      THENGOTO890;
    }
  strcpy(650L(10),(char*)"              :<-----B------>");
  660FORx=1TO14;
  strcpy(670LL(x),(char*)"://///");
  680NEXTx;
  strcpy(690LL(3),(char*)"://N//");
  strcpy(700LL(4),(char*)"://O//");
  strcpy(710LL(6),(char*)"://F//");
  strcpy(720LL(7),(char*)"://L//");
  strcpy(730LL(8),(char*)"://O//");
  strcpy(740LL(9),(char*)"://W//");
  750GOTO890;
  760FORx=6TO11;
  strcpy(770L(x),(char*)"              :       :");
  780NEXTx;
  strcpy(790L(5),(char*)"  STREAM      :       :");
  strcpy(800L(8),(char*)"              X       W");
  strcpy(810L(13),(char*)"                      :");
  strcpy(820L(14),(char*)"                      :");
  830FORx=16TO18;
  strcpy(840L(x),(char*)"/////////////////////////////");
  850NEXTx;
  strcpy(860L(15),(char*)"-----------------------------");
  strcpy(870L(17),(char*)"///////////NO FLOW///////////");
  strcpy(880L(12),(char*)"              O WELL  :");
  890FORx=1TO18;
  900PRINTL(x);TAB(30);LL(x);
  910NEXTx;
  920PRINT;
  930PRINT;
  940;
  if(BI==4)
    {
      printf("ENTER STREAM DEPLETION FACTOR (days) ");
      SDF=0;
      gets(InputBuffer);
      ScanError = scan(InputBuffer,"%d",&SDF);

      *InputBuffer=0;
      THENGOTO1070;
    }
  if(BI==2)
    {
      printf("W(FEET)");
      W=0;
      gets(InputBuffer);
      ScanError = scan(InputBuffer,"%d",&W);

      *InputBuffer=0;
    }
  if(BI==3)
    {
      printf("B(FEET)");
      B=0;
      gets(InputBuffer);
      ScanError = scan(InputBuffer,"%d",&B);

      *InputBuffer=0;
    }
  980INPUT"TRANSMISSIVITY (GPD/FT)";TR;
  990INPUT"SPECIFIC YIELD";S;
  1000INPUT"X(FEET)";DXX;
  if(BI==2&&(W-DXX)<0)
    {
      printf("%s\n","X CANNOT EXCEED W");
      THENGOTO960;
    }
  if(str_cmp(ZZSEG,"N")==0)
    {
      THENGOTO1070;
    }
  1030INPUT"ENTER LEFT LIMIT (Z1, SEE DIAGRAM) OF STREAM SEGMENT. ENTER -99999 FOR NEGATIVE INFINITY (FEET)";Z1;
  1040INPUT"ENTER RIGHT LIMIT (Z2, SEE DIAGRAM) OF STREAM SEGMENT. ENTER 99999 FOR INFINITY (FEET)";Z2;
  if(BI==3&&B<Z2)
    {
      printf("%s\n","Z2 CANNOT EXCEED B ");
      THENGOTO1040;
    }
  if((Z2-Z1)<0)
    {
      printf("%s\n","Z1 CANNOT EXCEED Z2");
      THENGOTO1030;
    }
  1070;
  1080;
  1090FORx=1TO10;
  1100PRINT;
  1110NEXTx;
  if(str_cmp(ZZSEG,"N")==0)
    {
      THENGOTO1300;
    }
  1130PRINT"Z1=";Z1;"FEET,  Z2=";Z2;"FEET";
  if(Z1<0)
    {
      THENGOTO1170;
    }
  1150PRINTTAB(15);":<--Z1-->";TAB(30);LL(1);
  1160GOTO1180;
  1170PRINT"<------Z1---->:";TAB(30);LL(1);
  if(Z2<0)
    {
      THENGOTO1240;
    }
  if(Z2==B)
    {
      THENGOTO1220;
    }
  1200PRINTTAB(15);":<----Z2---->";TAB(30);LL(2);
  1210GOTO1250;
  1220PRINTTAB(15);":<-----Z2----->";LL(2);
  1230GOTO1250;
  1240PRINT"        <-Z2->:";TAB(30);LL(2);
  1250PRINTTAB(30);LL(3);
  1260FORx=4TO7;
  1270PRINTL(x);TAB(30);LL(x);
  1280NEXTx;
  1290GOTO1330;
  1300FORx=1TO7;
  1310PRINTL(x);TAB(30);LL(x);
  1320NEXTx;
  if(BI==4)
    {
      THENGOTO1470;
    }
  if(BI==2)
    {
      THENGOTO1370;
    }
  1350PRINTTAB(11);DXX;"'";TAB(30);LL(8);
  1360GOTO1380;
  1370PRINTTAB(11);DXX;"'";TAB(21);W;"'";TAB(30);LL(8);
  1380PRINTL(9);TAB(30);LL(9);
  if(BI==3)
    {
      THENGOTO1420;
    }
  1400PRINTL(10);TAB(30);LL(10);
  1410GOTO1430;
  1420PRINTTAB(15);":<-";B;"'";TAB(28);"->";LL(10);
  1430FORx=11TO18;
  1440PRINTL(x);TAB(30);LL(x);
  1450NEXTx;
  1460GOTO1520;
  1470FORx=8TO12;
  1480PRINTL(x);
  1490NEXTx;
  1500PRINT"SDF=";SDF;"DAYS";
  1510GOTO1530;
  1520PRINT"T=";TR;"GPD/FT,  S=";S;
  1530PRINT"NOT DRAWN TO SCALE";
  1540;
  1550;
  1560INPUT"DO YOU WISH TO MAKE ANY CHANGES (Y OR N )";NUT;
  if(str_cmp(NUT,"N")==0 || str_cmp(NUT,"n")==0)
    {
      THENGOTO1830;
    }
  1580PRINT"ENTER ITEM NUMBER TO BE CHANGED";
  1590PRINT"  1) BOUNDRIES";
  1600PRINT"  2) TRANSMISSIVITY";
  1610PRINT"  3) SPECIFIC YIELD";
  1620PRINT"  4) X-DISTANCE FROM STREAM";
  1630PRINT"  5) W-DISTANCE BETWEEN STREAM AND EDGE OF ALLUVIUM";
  1640PRINT"  6) B-DISTANCE BETWEEN WELL AND NO FLOW BOUNDARY";
  1650PRINT"  7) Z1-LEFT COORDINATE OF STREAM SEGMENT";
  1660PRINT"  8) Z2-RIGHT COORDINATE OF STREAM SEGMENT";
  1670INPUTCHA;
  1680ONCHAGOTO340,1690,1710,1730,1750,1770,1790,1810;
  1690INPUT"T(GPD/FT)";TR;
  1700GOTO1070;
  1710INPUT"SPECIFIC YIELD";S;
  1720GOTO1070;
  1730INPUT"DISTANCE FROM STREAM (FEET)";DXX;
  1740GOTO1070;
  1750INPUT"DISTANCE BETWEEN STREAM AND EDGE OF ALLUVIUM (FEET)";W;
  1760GOTO1070;
  1770INPUT"DISTANCE BETWEEN WELL AND NO FLOW BOUNDARY (FEET)";B;
  1780GOTO1070;
  1790INPUT"LEFT COORDINATE OF STREAM SEGMENT (FEET)";Z1;
  1800GOTO1070;
  1810INPUT"RIGHT COORDINATE OF STREAM SEGMENT (FEET)";Z2;
  1820GOTO1070;
  1830;
  1840;
  1850cls;
  1860PRINT"A PUMPING PERIOD IS A PERIOD OF TIME DURING WHICH THE PUMPING RATE IS CONSTANT";
  1870TT=0;
  1880PRINT;
  1890INPUT"DO YOU WISH TO ENTER A CYCLIC (SEASONAL) PUMPING SCHEDULE (Y OR N)";CSI;
  if(str_cmp(CSI,"N")==0 || str_cmp(CSI,"n")==0)
    {
      THENGOTO1950;
    }
  1910INPUT"NUMBER OF CYCLES OR SEASONS TO BE SIMULATED";NC;
  1920INPUT"NUMBER OF PUMPING PERIODS PER CYCLE";NPPC;
  1930np=NC*NPPC;
  1940GOTO1970;
  1950PRINT"ENTER NUMBER OF PUMPING PERIODS";
  1960INPUTnp;
  1970PRINT"ENTER NUMBER FOR TIME UNITS TO BE USED";
  1980PRINT"   (1)DAYS";
  1990PRINT"   (2)WEEKS - A WEEK WILL = 365/52 DAYS";
  2000PRINT"   (3)MONTHS - A MONTH WILL = 365/12 DAYS";
  2010PRINT"   (4)YEARS - A YEAR WILL = 365 DAYS";
  2020INPUTTUI;
  2030RESTORE2050;
  for(x=1; x<=TUI; x+=1)
    {
      READTU(x),fact;
    }
  2050DATADAYS,1,WEEKS,7.019231,MONTHS,30.41667,YEARS,365;
  if(str_cmp(CSI,"Y")==0 || str_cmp(CSI,"y")==0)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 6940;
      THENGOTO2160;
    }
  2070FORx=1TOnp;
  2080PRINT"FOR PUMPING PERIOD #";x;"ENTER LENGTH OF PUMPING PERIOD IN ";TU(TUI);"  (LENGTH MUST BE WHOLE NUMBER OF TIME UNITS)";
  2090INPUTDELT(x);
  if((DELT(x)-FINT(DELT(x)))!=0)
    {
      THENGOTO2080;
    }
  2110TT=TT+DELT(x);
  2120ZZAT(x)=TT;
  2130PRINT"ENTER PUMPING RATE (GPM) FOR PUMPING PERIOD #";x;
  2140INPUTq(x);
  2150NEXTx;
  2160;
  2170cls;
  2180LCOUNT=5;
  2190PRINTTAB(15);"PUMPING SCHEDULE";
  2200PRINT"TOTAL TIME SIMULATED = ";TT;TU(TUI);
  2210PRINT;
  2220PRINT"PUMPING","Q","LENGTH","ACC. TIME";
  2230PRINT"PERIOD","GPM",TU(TUI),TU(TUI);
  2240FORx=1TOnp;
  2250PRINTx,q(x),DELT(x),ZZAT(x);
  2260LCOUNT=LCOUNT+1;
  if(LCOUNT==20)
    {
      printf("    press return to continue");
      *NUT=0;
      gets(InputBuffer);
      ScanError = scan(InputBuffer,"%s",NUT);

      *InputBuffer=0;
      LCOUNT=0;
    }
  2280NEXTx;
  2290;
  2300INPUT"DO YOU WANT TO CHANGE PUMPING SCHEDULE (Y OR N)";NUT;
  if(str_cmp(NUT,"Y")==0 || str_cmp(NUT,"y")==0)
    {
      THENGOTO1840;
    }
  2320;
  2330;
  2340cls;
  2350PRINT"A LARGER NUMER OF TIME STEPS CAN BE SELECTED BY ANSWERING THE NEXT QUESTION     WITH A YES(Y).";
  2360INPUT"DO YOU WANT DEPLETION PRINTED OR GRAPHED FOR TIMES OTHER THAN END OF EACH       PUMPING PERIOD (Y OR N)      ";PFOT;
  if(str_cmp(PFOT,"N")==0 || str_cmp(PFOT,"n")==0)
    {
      THENGOTO2420;
    }
  else
    {
      strcpy((char*)PFOT,"Y");
    }
  2380PRINT"ENTER NUMBER OF  ";TU(TUI);" BETWEEN PRINTOUTS.";
  2390INPUTTBP;
  if((TBP-FINT(TBP))>.0001)
    {
      printf("%s%s\n","YOU MUST SELECT A WHOLE NUMBER OF ",TU(TUI));
      THENGOTO2380;
    }
  2410;
  2420INPUT"DO YOU WANT DEPLETION GRAPHED (Y OR N)";GI;
  if(str_cmp(GI,"Y")==0 || str_cmp(GI,"y")==0)
    {
      strcpy((char*)GI,"Y");
    }
  else
    {
      strcpy((char*)GI,"N");
    }
  2440;
  2450lprint"";
  2460lprint"";
  2470lprint"********************************************************************************";
  2480cls;
  printf("%s\n","THINKING, PLEASE WAIT");
  2490lprint"";
  2500lprint"                   STREAM DEPLETION USING GLOVER TECHNIQUES";
  2510lprint"";
  2520TA1=(80-strlen(head1))/2;
  TA2=(80-strlen(HEAD2))/2;
  TA3=(80-strlen(DTE))/2;
  2530lprintTAB(TA1);head1;
  sprintf(BcxPtr_Buffer,"% d%s",(int)TAB(TA2),HEAD2);
  PrinterWrite(BcxPtr_Buffer);
  sprintf(BcxPtr_Buffer,"% d%s",(int)TAB(TA3),DTE);
  PrinterWrite(BcxPtr_Buffer);
  2540lprint"";
  2550lprint"********************************************************************************";
  2560lprint"";
  2570lprint"";
  if(str_cmp(ZZSEG,"N")==0)
    {
      THENGOTO2760;
    }
  2590lprint"Z1=";Z1;"FEET,  Z2=";Z2;"FEET";
  if(Z1<0)
    {
      goto 2630;
    }
  2610lprintTAB(15);":<--Z1-->";TAB(30);LL(1);
  2620GOTO2640;
  2630lprint"<------Z1---->:";TAB(30);LL(1);
  if(Z2<0)
    {
      THENGOTO2700;
    }
  if(Z2==B)
    {
      THENGOTO2680;
    }
  2660lprintTAB(15);":<----Z2---->";TAB(30);LL(2);
  2670GOTO2710;
  2680lprintTAB(15);":<-----Z2----->";LL(2);
  2690GOTO2710;
  2700lprint"        <-Z2->:";TAB(30);LL(2);
  2710lprintTAB(30);LL(3);
  2720FORx=4TO7;
  2730lprintL(x);TAB(30);LL(x);
  2740NEXTx;
  2750GOTO2790;
  2760FORx=1TO7;
  2770lprintL(x);TAB(30);LL(x);
  2780NEXTx;
  if(BI==4)
    {
      THENGOTO2930;
    }
  if(BI==2)
    {
      THENGOTO2830;
    }
  2810lprintTAB(11);DXX;"'";TAB(30);LL(8);
  2820GOTO2840;
  2830lprintTAB(11);DXX;"'";TAB(21);W;"'";TAB(30);LL(8);
  2840lprintL(9);TAB(30);LL(9);
  if(BI==3)
    {
      THENGOTO2880;
    }
  2860lprintL(10);TAB(30);LL(10);
  2870GOTO2890;
  2880lprintTAB(15);":<-";B;"'";TAB(28);"->";LL(10);
  2890FORx=11TO18;
  2900lprintL(x);TAB(30);LL(x);
  2910NEXTx;
  2920GOTO2980;
  2930FORx=8TO12;
  2940lprintL(x);
  2950NEXTx;
  2960lprint"SDF=";SDF;"DAYS";
  2970GOTO2990;
  2980lprint"T=";TR;"GPD/FT,   S=";S;
  2990lprint"NOT DRAWN TO SCALE";
  3000;
  if(str_cmp(PFOT,"Y")==0)
    {
      THENGOTO3090;
    }
  3020MN=1000000;
  3030FORx=1TOnp;
  if(DELT(x)>MN)
    {
      THENGOTO3060;
    }
  3050MN=DELT(x);
  3060NEXTx;
  3070dela=MN;
  3080GOTO3100;
  3090dela=TBP;
  3100ADF=0;
  3110FORx=1TOnp;
  3120A=DELT(x)/dela;
  if((A-FINT(A))==0)
    {
      goto 3150;
    }
  3140ADF=1;
  3150NEXTx;
  if((TBP/dela-FINT(TBP/dela))>0)
    {
      goto 3180;
    }
  if(ADF==0)
    {
      goto 3200;
    }
  3180dela=dela-1;
  3190GOTO3100;
  3200NPA=TT/dela;
  3210;
  3220GOSUB6790;
  3230GOSUB6550;
  3240PRINT"COMPUTATIONS BEGIN AT ";timef();
  if(str_cmp(ZZSEG,"Y")==0 || str_cmp(ZZSEG,"y")==0)
    {
      goto 4240;
    }
  if(BI==2)
    {
      goto 3360;
    }
  3270;
  3280FORx=1TONPA;
  3290T=T+dela;
  if(BI==4)
    {
      U=pow((SDF/(4*fact*T)),.5);
    }
  else
    {
      U=DXX/(pow((4*TR*fact*T/(S*7.481)),.5));
    }
  3310GOSUB5490;
  3320QQ(x)=BQQ;
  3330VV(x)=SVV;
  3340NEXTx;
  3350GOTO3600;
  3360;
  3370FORx=1TONPA;
  3380T=T+dela;
  3390QS=0;
  3400VS=0;
  3410YY=-DXX;
  3420FAC=1;
  3430YY=YY+2*DXX;
  3440U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  3450GOSUB5490;
  3460QS=QS+BQQ*FAC;
  3470VS=VS+SVV*FAC;
  if(BQQ==0)
    {
      goto 3570;
    }
  3490YY=YY-2*DXX+2*W;
  3500U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  3510GOSUB5490;
  3520QS=QS+BQQ*FAC;
  3530VS=VS+SVV*FAC;
  if(BQQ==0)
    {
      goto 3570;
    }
  3550FAC=FAC*(-1);
  3560GOTO3430;
  3570QQ(x)=QS;
  3580VV(x)=VS;
  3590NEXTx;
  3600;
  3610ADEL=1;
  3620FORJ=1TOnp;
  3630N=0;
  if(q(J)==0)
    {
      goto 3770;
    }
  3650FORx=ADELTONPA;
  3660N=N+1;
  3670qd(x)=qd(x)+QQ(N)*q(J);
  3680vd(x)=vd(x)+VV(N)*q(J)*fact*N*dela;
  3690NEXTx;
  3700N=0;
  if(np==1)
    {
      goto 3780;
    }
  3720FORx=(1+ZZAT(J)/dela)TONPA;
  3730N=N+1;
  3740qd(x)=qd(x)-QQ(N)*q(J);
  3750vd(x)=vd(x)-VV(N)*q(J)*fact*N*dela;
  3760NEXTx;
  3770ADEL=1+ZZAT(J)/dela;
  3780NEXTJ;
  3790;
  3800PRINT"COMPUTATIONS END AT ";timef();
  3810PRINT"PLEASE WAIT FOR OUTPUT.";
  3820lprint"";
  if (setjmp(GosubStack[GosubNdx++])==0) goto 8020;
  3830lprint"";
  3840lprint"";
  if(np>1)
    {
      goto 3920;
    }
  3860lprintTAB(34);"STREAM DEPLETION";
  3870lprint"";
  3880lprint"                                                                         VOL. OF DEP.";
  3890lprint"          TIME     DEP. RATE     DEP. RATE  VOL. OF DEP.  VOL. OF DEP.     THIS STEP";
  3900lprintZZ;"         (GPM)           (%)";ZZZ;"           (%)";ZZZ;
  3910GOTO3970;
  3920lprintTAB(20);"STREAM DEPLETION";
  3930lprint"";
  3940lprint"                                            VOL. OF DEP.";
  3950lprint"          TIME     DEP. RATE  VOL. OF DEP.     THIS STEP";
  3960lprintZZ;"         (GPM)";ZZZ;ZZZ;
  3970lprint"";
  if(str_cmp(VUNIT,"GALLONS")==0)
    {
      goto 4010;
    }
  3990VFAC=1440/325900;
  4000GOTO4030;
  4010VFAC=1440;
  4020VDTP=0;
  4030FORx=1TONPA;
  if(str_cmp(PFOT,"Y")==0)
    {
      goto 4140;
    }
  4050PFLG=0;
  4060TDELA=TDELA+dela;
  4070FORJ=1TOnp;
  if(TDELA==ZZAT(J))
    {
      goto 4100;
    }
  4090GOTO4110;
  4100PFLG=1;
  4110NEXTJ;
  if(PFLG==1)
    {
      goto 4160;
    }
  4130GOTO4210;
  if(((x*dela/TBP)-(FINT(x*dela/TBP)))==0)
    {
      goto 4160;
    }
  4150GOTO4210;
  printf("%s% d%s\n","np = ",(int)np,"at line 4150");
  geek=0;
  gets(InputBuffer);
  ScanError = scan(InputBuffer,"%d",&geek);

  *InputBuffer=0;
  if(np>1)
    {
      goto 4190;
    }
  if(opt5==2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 8200;
    }
  4170lprintUSING"############";dela*x;;
  sprintf(BcxPtr_Buffer,"% d",(int)TAB(15));
  PrinterWrite(BcxPtr_Buffer);
  sprintf(BcxPtr_Buffer,"% .15G% d% d% d% d% .16G",(double)USING"#########.####",(int)qd(x),(int)qd(x)*100/q(1),(int)vd(x)*VFAC,(int)vd(x)*100/(q(1)*dela*x*fact),(double)(vd(x)*VFAC-VDTP));
  PrinterWrite(BcxPtr_Buffer);
  4180GOTO4200;
  4190lprintUSING"############";dela*x;;
  sprintf(BcxPtr_Buffer,"% d",(int)TAB(15));
  PrinterWrite(BcxPtr_Buffer);
  sprintf(BcxPtr_Buffer,"% .15G% d% d% .16G",(double)USING"#########.####",(int)qd(x),(int)vd(x)*VFAC,(double)(vd(x)*VFAC-VDTP));
  PrinterWrite(BcxPtr_Buffer);
  if(opt5==2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 8200;
    }
  4200VDTP=vd(x)*VFAC;
  4210NEXTx;
  if(str_cmp(GI,"Y")==0)
    {
      goto 5650;
    }
  if(opt5==2)
    {
      if(FP3)
       {
         fflush(FP3);
         fclose(FP3);
       }
    }
  4230END;
  4240;
  if(Z1==-99999)
    {
      IFLG=1;
      Z1=0;
    }
  if(Z2==99999)
    {
      IFLG=1;
      Z2=0;
    }
  4270YY=DXX;
  4280QL=0;
  4290QN=0;
  4300ONBIGOTO4310,4540,5040;
  4310L1=Z1;
  4320L2=Z2;
  4330FORx=1TONPA;
  4340T=T+dela;
  4350U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  if(U>2.9)
    {
      goto 4510;
    }
  if(IFLG==0)
    {
      goto 4410;
    }
  4380GOSUB5490;
  4390QQ(x)=.5*BQQ;
  4400VV(x)=.5*SVV;
  4410U=4*TR*T*fact/(S*7.481);
  4420GOSUB7130;
  4430QQ(x)=QQ(x)+BQQ;
  4440QN=BQQ;
  4450U=4*TR*(T-dela/2)*fact/(S*7.481);
  4460GOSUB7130;
  4470QP=BQQ;
  4480GOSUB7300;
  4490AVV=AVV+SVV;
  4500VV(x)=VV(x)+AVV/T;
  4510QL=QN;
  4520NEXTx;
  4530GOTO3600;
  4540FAC=1;
  4550L1=Z1;
  4560L2=Z2;
  4570FORx=1TONPA;
  4580FAC=1;
  4590QL=QN;
  4600QN=0;
  4610QP=0;
  4620T=T+dela;
  4630YY=-DXX;
  4640YY=YY+2*DXX;
  4650WW=0;
  if(IFLG==0)
    {
      goto 4720;
    }
  4670U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  4680GOSUB5490;
  4690WW=WW+BQQ;
  4700QQ(x)=BQQ*FAC/2+QQ(x);
  4710VV(x)=VV(x)+SVV*FAC/2;
  4720U=4*TR*T*fact/(S*7.481);
  if(L1!=L2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 7130;
    }
  else
    {
      THENGOTO4810;
    }
  4740WW=WW+BQQ;
  4750QQ(x)=QQ(x)+BQQ*FAC;
  4760QN=QN+BQQ*FAC;
  4770U=4*TR*(T-dela/2)*fact/(S*7.481);
  4780XYZ=U;
  if(L1!=L2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 7130;
    }
  4800QP=QP+BQQ*FAC;
  4810YY=YY-2*DXX+2*W;
  if(IFLG==0)
    {
      goto 4880;
    }
  4830U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  4840GOSUB5490;
  4850WW=WW+BQQ;
  4860QQ(x)=QQ(x)+BQQ*FAC/2;
  4870VV(x)=VV(x)+SVV*FAC/2;
  4880U=XYZ;
  if(L1!=L2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 7130;
    }
  else
    {
      THENGOTO4960;
    }
  4900QP=QP+BQQ*FAC;
  4910U=4*TR*T*fact/(S*7.481);
  if(L1!=L2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 7130;
    }
  4930WW=WW+BQQ;
  4940QN=QN+BQQ*FAC;
  4950QQ(x)=QQ(x)+BQQ*FAC;
  if(Abs(WW)<.00001)
    {
      goto 4990;
    }
  4970FAC=FAC*(-1);
  4980GOTO4640;
  if(L1!=L2)
    {
      if (setjmp(GosubStack[GosubNdx++])==0) goto 7300;
    }
  else
    {
      THENGOTO5020;
    }
  5000AVV=AVV+SVV;
  5010VV(x)=VV(x)+AVV/T;
  5020NEXTx;
  5030GOTO3600;
  5040FORx=1TONPA;
  5050T=T+dela;
  5060U=YY/(pow((4*TR*fact*T/(S*7.481)),.5));
  if(U>2.9)
    {
      goto 5460;
    }
  if(IFLG==0)
    {
      goto 5260;
    }
  5090GOSUB5490;
  5100QQ(x)=BQQ;
  5110VV(x)=SVV;
  if(B==Z2)
    {
      goto 5470;
    }
  5130L1=Z2;
  5140L2=2*B-Z2;
  5150U=4*TR*T*fact/(S*7.481);
  5160GOSUB7130;
  5170QQ(x)=QQ(x)-BQQ;
  5180QN=BQQ;
  5190U=4*TR*(T=dela/2)*fact/(S*7.481);
  5200GOSUB7130;
  5210QP=BQQ;
  5220GOSUB7300;
  5230AVV=AVV+SVV;
  5240VV(x)=VV(x)-AVV/T;
  5250GOTO5460;
  5260L1=Z1;
  5270L2=Z2;
  5280U=4*TR*T*fact/(S*7.481);
  5290GOSUB7130;
  5300QQ(x)=BQQ;
  5310QN=BQQ;
  5320U=4*TR*(T-dela/2)*fact/(S*7.481);
  5330GOSUB7130;
  5340QP=BQQ;
  5350L1=2*B-Z2;
  5360L2=2*B-Z1;
  5370GOSUB7130;
  5380QP=BQQ+QP;
  5390U=4*TR*T*fact/(S*7.481);
  5400GOSUB7130;
  5410QN=QN+BQQ;
  5420QQ(x)=QQ(x)+BQQ;
  5430GOSUB7300;
  5440AVV=AVV+SVV;
  5450VV(x)=AVV/T;
  5460QL=QN;
  5470NEXTx;
  5480GOTO3600;
  5490;
  5500;
  if(U>2.9)
    {
      BQQ=0;
      SVV=0;
      longjmp (GosubStack [--GosubNdx],1 );

    }
  5520SUM=U;
  5530M=0;
  5540N=pow(U,2);
  5550TERM=U;
  5560P=1;
  5570M=M+1;
  5580P=P+2;
  5590TERM=((-1)*TERM*N*(P-2))/(P*M);
  5600SUM=SUM+TERM;
  if((Abs(TERM))>1E-8)
    {
      goto 5570;
    }
  5620BQQ=1-2*SUM/(pow(PI,.5));
  5630SVV=BQQ*(1+2*(pow(U,2)))-(2*U*pow(E,(-(pow(U,2)))))/(pow(PI,.).5);
  5640RETURN;
  5650;
  5660MAXVD=-1000000;
  5670MAXQD=-1000000;
  5680MINVD=1000000;
  5690MINQD=1000000;
  5700lprintchr(15);;
  5710lprintchr(27);"1";;
  5720FORx=1TONPA;
  if(qd(x)>MAXQD)
    {
      MAXQD=qd(x);
    }
  if(qd(x)<MINQD)
    {
      MINQD=qd(x);
    }
  if(vd(x)>MAXVD)
    {
      MAXVD=vd(x);
    }
  if(vd(x)<MINVD)
    {
      MINVD=vd(x);
    }
  5770NEXTx;
  if(MINVD>0)
    {
      MINVD=0;
    }
  if(MINQD>0)
    {
      MINQD=0;
    }
  5800LVD=MAXVD-MINVD;
  5810LQD=MAXQD-MINQD;
  5820LBP=FINT(50/NPA)+1;
  5830lprint"";
  5840lprint"";
  5850lprint"";
  5860lprintTAB(20);"STREAM DEPLETION VS TIME (";TU(TUI);")";
  5870lprint"";
  5880lprint"";
  5890lprintTAB(10);"***** RATE OF DEPLETION (GPM)";
  5900lprint"";
  5910FORIZ=1TO6;
  if(IZ!=1)
    {
      sprintf(BcxPtr_Buffer,"% d",(int)TAB(IZ*20-19));
      PrinterWrite(BcxPtr_Buffer);
    }
  5930lprintUSING"##########.##";MINQD+(IZ-1)*LQD/5;;
  5940NEXTIZ;
  5950lprint"";
  5960lprintTAB(10);"+-------------------+-------------------+-------------------+-------------------+-------------------+";
  5970lprint"";
  5980lprint"";
  5990lprintTAB(10);"+++++ VOL. OF DEPLETION (";VUNIT;")";
  6000lprint"";
  6010FORIZ=1TO6;
  if(IZ!=1)
    {
      sprintf(BcxPtr_Buffer,"% d",(int)TAB(IZ*20-19));
      PrinterWrite(BcxPtr_Buffer);
    }
  6030lprintUSING"##########.##";MINVD*VFAC+(IZ-1)*VFAC*LVD/5;;
  6040NEXTIZ;
  6050lprint"";
  6060lprintTAB(7);"0  +-------------------+-------------------+-------------------+-------------------+-------------------+";
  6070FORx=1TONPA;
  6080qd(x)=(qd(x)-MINQD)*100/LQD+10;
  6090vd(x)=(vd(x)-MINVD)*100/LVD+10;
  if(LBP==1)
    {
      goto 6140;
    }
  6110FORJ=1TO(LBP-1);
  6120lprintTAB(10);"!";
  6130NEXTJ;
  if(Cint(vd(x))>Cint(qd(x)))
    {
      goto 6380;
    }
  if(Cint(vd(x))==Cint(qd(x)))
    {
      goto 6270;
    }
  if((x/5-FINT(x/5))==0)
    {
      goto 6220;
    }
  if(vd(x)<10.5)
    {
      goto 6200;
    }
  6180lprintTAB(10);"!";TAB(vd(x));"+";TAB(qd(x));"*";
  6190GOTO6480;
  6200lprintTAB(10);"+";TAB(qd(x));"*";
  6210GOTO6480;
  if(vd(x)<10.5)
    {
      goto 6250;
    }
  6230lprintTAB(5);(x*dela);TAB(10);"+";TAB(vd(x));"+";TAB(qd(x));"*";
  6240GOTO6480;
  6250lprintTAB(5);(x*dela);TAB(10);"+";TAB(qd(x));"*";
  6260GOTO6480;
  if((x/5-FINT(x/5))==0)
    {
      goto 6330;
    }
  if(qd(x)<10.5)
    {
      goto 6310;
    }
  6290lprintTAB(10);"!";TAB(qd(x));"*";
  6300GOTO6480;
  6310lprintTAB(10);"*";
  6320GOTO6480;
  if(qd(x)<10.5)
    {
      goto 6360;
    }
  6340lprintTAB(5);(x*dela);TAB(10);"!";TAB(qd(x));"*";
  6350GOTO6480;
  6360lprintTAB(5);(x*dela);TAB(10);"*";
  6370GOTO6480;
  if((x/5-FINT(x/5))==0)
    {
      goto 6440;
    }
  if(qd(x)<10.5)
    {
      goto 6420;
    }
  6400lprintTAB(10);"!";TAB(qd(x));"*";TAB(vd(x));"+";
  6410GOTO6480;
  6420lprintTAB(10);"*";TAB(vd(x));"+";
  6430GOTO6480;
  if(qd(x)<10.5)
    {
      goto 6470;
    }
  6450lprintTAB(5);(x*dela);TAB(10);"+";TAB(qd(x));"*";TAB(vd(x));"+";
  6460GOTO6480;
  6470lprintTAB(5);(x*dela);TAB(10);"*";TAB(vd(x));"+";
  6480NEXTx;
  6490lprint"";
  6500lprintTAB(10);"+-------------------+-------------------+-------------------+-------------------+-------------------+";
  6510lprint"";
  6520lprint"IF RATE AND VOLUME HAVE SAME COORDINATES ONLY AN (*) IS PRINTED.";
  6530lprintchr(27);"@";;
  6540END;
  6550;
  6560lprint"";
  6570lprint"";
  6580lprint"";
  6590lprintTAB(31);"PUMPING SCHEDULE";
  6600lprintTAB(23);"TOTAL TIME SIMULATED=";TT;TU(TUI);
  6610lprint"";
  6620lprint"                                                    VOL. PUMPED     CUM. VOL.";
  6630lprint"PUMPING           Q          LENGTH     CUM. TIME   THIS PERIOD      PUMPED";
  sprintf(BCX_STR,"%s%s%s","(",TU( TUI ) ,")");
  strcpy(6640ZZ,BCX_STR);
  for(II=1; II<=(14-strlen(ZZ)); II+=1)
    {
      sprintf(BCX_STR,"%s%s"," ",ZZ);
      strcpy(ZZ,BCX_STR);
    }
  sprintf(BCX_STR,"%s%s%s","(",VUNIT,")");
  strcpy(6650ZZZ,BCX_STR);
  for(II=1; II<=(14-strlen(ZZZ)); II+=1)
    {
      sprintf(BCX_STR,"%s%s"," ",ZZZ);
      strcpy(ZZZ,BCX_STR);
    }
  6660lprint"PERIOD          (GPM)";ZZ;ZZ;ZZZ;ZZZ;
  6670lprint"";
  6680FORx=1TOnp;
  6690lprintUSING"####";x;;
  sprintf(BcxPtr_Buffer,"% d",(int)TAB(8));
  PrinterWrite(BcxPtr_Buffer);
  6700lprintUSING"##########.###";q(x);;
  6710lprintUSING"##############";DELT(x);ZZAT(x);;
  6720lprintUSING"##########.###";VPTP(x);AVP(x);
  6730NEXTx;
  6740lprint"";
  6750lprint"";
  6760lprint"";
  6770lprint"";
  6780RETURN;
  6790;
  6800FORx=1TOnp;
  6810VPTP(x)=q(x)*DELT(x)*fact*1440;
  6820AVSUM=AVSUM+VPTP(x);
  6830AVP(x)=AVSUM;
  6840NEXTx;
  if((Abs(AVSUM))>1000000)
    {
      goto 6880;
    }
  strcpy(6860VUNIT,(char*)"GALLONS");
  6870RETURN;
  6880FORx=1TOnp;
  6890VPTP(x)=VPTP(x)/325900;
  6900AVP(x)=AVP(x)/325900;
  6910NEXTx;
  strcpy(6920VUNIT,(char*)"ACRE-FEET");
  6930RETURN;
  6940;
  6950FORx=1TONPPC;
  6960PRINT"FOR PUMPING PERIOD #";x;"ENTER LENGTH OF PUMPING PERIOD IN ";TU(TUI);"  (LENGTH MUST BE WHOLE NUMBER OF TIME UNITS)";
  6970INPUTLTP(x);
  6980PRINT"ENTER PUMPING RATE(GPM) FOR PUMPING PERIOD #";x;
  6990INPUTQTP(x);
  7000NEXTx;
  7010I=0;
  7020TT=0;
  7030FORx=1TONC;
  7040FORJ=1TONPPC;
  7050I=I+1;
  7060DELT(I)=LTP(J);
  7070TT=TT+DELT(I);
  7080ZZAT(I)=TT;
  7090q(I)=QTP(J);
  7100NEXTJ;
  7110NEXTx;
  7120RETURN;
  7130;
  7140SIMN=Abs(FINT((L2-L1)/YY))*20;
  if(SIMN<40)
    {
      SIMN=40;
    }
  if(SIMN>100)
    {
      SIMN=100;
    }
  7170DELX=(L2-L1)/SIMN;
  7180XC=L1-DELX;
  7190FORN=1TO(SIMN+1);
  7200XC=XC+DELX;
  7210AB=(pow(YY,2))+(pow(XC,2));
  if((AB/U)>60)
    {
      H(N)=0;
    }
  else
    {
      H(N)=(pow(E,(-(AB/U))))/AB;
    }
  7230NEXTN;
  7240BQQ=H(1)+H(SIMN+1);
  7250FORN=2TOSIMN;
  if((N/2-FINT(N/2))==0)
    {
      BQQ=BQQ+4*H(N);
    }
  else
    {
      BQQ=BQQ+2*H(N);
    }
  7270NEXTN;
  if(BQQ<1E-33)
    {
      BQQ=0;
    }
  else
    {
      BQQ=BQQ*DELX*YY/(3*3.1415927);
    }
  7290RETURN;
  7300;
  7310SVV=((QN+QL)*dela/2+(QP-(QL+QN)/2)*dela*2/3);
  7320RETURN;
  if(ERR==53)
    {
      printf("%s\n","no files currently on disk");
      RESUME8090;
    }
  printf("%s\n","disk problem");
  opt5=1;
  longjmp (GosubStack [--GosubNdx],1 );

  //   open file to save results on disk 
  8020;
  printf("%s\n","Do you want to save results on disk?");
  printf("%s\n","1 = no");
  printf("%s\n","2 = yes");
  opt5=0;
  gets(InputBuffer);
  ScanError = scan(InputBuffer,"%d",&opt5);

  *InputBuffer=0;
  if(opt5==1)
    {
      longjmp (GosubStack [--GosubNdx],1 );

    }
  while(1)
  {
  if(ERROR==1)
    {
      goto 8000;
    }
  break;
  }
// PRINT "Directory of drive B": FILES "b:*.*" 
8090PRINT"Enter file name for output file";
*filout=0;
gets(InputBuffer);
ScanError = scan(InputBuffer,"%s",filout);

*InputBuffer=0;
if((FP3=fopen(filout,"w"))==0)
 {
fprintf(stderr,"Can't open file %s\n",filout);exit(1);
 }
while(1)
{
if(ERROR==1)
  {
    goto 0;
  }
break;
}
fprintf(FP3,"% d%s\n",(int)opt5,head1);
fprintf(FP3,"%s\n","step     q/Q                v/V");
longjmp (GosubStack [--GosubNdx],1 );

8200;
fprintf(FP3,"% d% d% d\n",(int)dela*x,(int)qd(x)/q(1),(int)vd(x)/(q(1)*dela*x*fact));
longjmp (GosubStack [--GosubNdx],1 );

fflush(stdout);
ExitProcess(0);
  return 0;   //  End of main program
}

// *************************************************
//                 Run Time Functions
// *************************************************

char *BCX_TmpStr (size_t Bites)
{
  static int   StrCnt;
  static char *StrFunc[2048];
  StrCnt=(StrCnt + 1) & 2047;
  if(StrFunc[StrCnt]) free (StrFunc[StrCnt]);
  return StrFunc[StrCnt]=(char*)calloc(Bites+128,sizeof(char));
}


int str_cmp (char *a, char *b)
{
  register int counter;
  counter=counter^counter;
  while(1)
   {
    if((a[counter]^b[counter]))
     {
      if((UINT) a[counter]>= (UINT) b[counter])
      return  1;
      return -1;
     }
     if(!a[counter]) return 0;
     counter++;
   }
#if !defined( __cplusplus )
 return 0;
#endif
}


char *left (char *S, int length)
{
  register int tmplen = strlen(S);
  char *strtmp = BCX_TmpStr(tmplen);
  strcpy (strtmp,S);
  if(length>tmplen)
    strtmp[tmplen] = 0;
  else
    strtmp[length] = 0;
  return strtmp;
}


char *mid (char *S, int start, int length)
{
  register int tmplen = strlen(S);
  char *strtmp;
  if(start>tmplen||start<1) return BCX_TmpStr(1);
  if (length < 0) length = tmplen - start + 1;
  strtmp = BCX_TmpStr(length);
  strncpy(strtmp,&S[start-1],length);
  strtmp[length] = 0;
  return strtmp;
}


char *extract (char *mane, char *match)
{
  register char *a;
  register char *strtmp = BCX_TmpStr(strlen(mane));
  strcpy(strtmp,mane);
  a=strstr(mane,match);
  if(a) strtmp[a-mane]=0;
  return strtmp;
}


char *RemoveStr (char *a, char *b)
{
  register int i, tmplen = strlen(b);
  register char *strtmp = BCX_TmpStr(strlen(a));
  strcpy(strtmp,a);
  while(1)
   {
     i=instr(strtmp,b);
     if(i==0) break;
     strcpy(&strtmp[i-1],&strtmp[i+tmplen-1]);
   }
  return strtmp;
}


char *str (double d)
{
  register char *strtmp = BCX_TmpStr(16);
  sprintf(strtmp,"% .15G",d);
  return strtmp;
}


char *timef (int t)
{
  unsigned long elapse_time;
  struct tm *tp;
  char A[256];
  char *strtmp = BCX_TmpStr(256);
  time (&elapse_time);
  tp = localtime(&elapse_time);
  A[0] = 0;
  switch (t)
  {
   case 0:
   strftime(A,256,"%H:%M:%S",tp); break;
   case 1:
   strftime(A,256,"%H",tp); break;
   case 2:
   strftime(A,256,"%M",tp); break;
   case 3:
   strftime(A,256,"%S",tp); break;
   case 4:
   strftime(A,256,"%p",tp); break;
   case 5:
   strftime(A,256,"%Y",tp); break;
   case 6:
   strftime(A,256,"%m",tp); break;
   case 7:
   strftime(A,256,"%d",tp); break;
   case 8:
   strftime(A,256,"%A",tp); break;
   case 9:
   strftime(A,256,"%w",tp); break;
   case 10:
   strftime(A,256,"%j",tp); break;
   case 11:
   strftime(A,256,"%U",tp); break;
  }
  strcpy(strtmp,A);
  return strtmp;
}





char *chr (int a,int b,int c,int d,int e,int f,int g,int h,int i,int j)
{
  register char *strtmp = BCX_TmpStr(11);
  strtmp[0]  = a;
  strtmp[1]  = b;
  strtmp[2]  = c;
  strtmp[3]  = d;
  strtmp[4]  = e;
  strtmp[5]  = f;
  strtmp[6]  = g;
  strtmp[7]  = h;
  strtmp[8]  = i;
  strtmp[9]  = j;
  strtmp[10] = 0;
  return strtmp;
}


char * join(int n, ...)
{
  register int i = n, tmplen = 0;
  register char *s_;
  register char *strtmp; // = 0;
  va_list marker;
  va_start(marker, n); // Initialize variable arguments
  while(i-- > 0)
  {
    s_ = va_arg(marker, char *);
    tmplen += strlen(s_);
  }
  strtmp = BCX_TmpStr(tmplen);
  va_end(marker); // Reset variable arguments
  i = n;
  va_start(marker, n); // Initialize variable arguments
  while(i-- > 0)
  {
    s_ = va_arg(marker, char *);
    strtmp = strcat(strtmp, s_);
  }
  va_end(marker); // Reset variable arguments
  return strtmp;
}


int instr(char* mane,char* match,int offset,int sensflag)
{
  register char *s;
  if (!mane || !match || ! *match || offset>(int)strlen(mane)) return 0;
  if (sensflag)
    s = _stristr_(offset>0 ? mane+offset-1 : mane,match);
  else
    s = strstr (offset>0 ? mane+offset-1 : mane,match);
  return s ? (int)(s-mane)+1 : 0;
}


char *_stristr_(char *String, char *Pattern)
{
  register char *pptr, *sptr, *start;
  register UINT  slen, plen;
  for (start = (char *)String,
    pptr  = (char *)Pattern,
    slen  = strlen(String),
    plen  = strlen(Pattern);
    slen >= plen;
    start++, slen--)
    {
     while (toupper(*start) != toupper(*Pattern))
       {
        start++;
        slen--;
        if (slen < plen)
        return(0);
       }
      sptr = start;
      while (toupper(*sptr) == toupper(*pptr))
        {
         sptr++;
         pptr++;
         if (!*pptr) return (start);
        }
    }
 return(0);
}


double Abs (double a)
{
  if(a<0) return -a;
  return  a;
}


void locate (int row,int col,int show,int shape)
{
  CONSOLE_CURSOR_INFO cci = {0};
  cursor.X = col-1;
  cursor.Y = row-1;
  SetConsoleCursorPosition(hConsole,cursor);
  cci.bVisible = show;
  cci.dwSize   = shape;
  SetConsoleCursorInfo(hConsole,&cci);
}


void cls (void)
{
  COORD coordScreen = {0,0};
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi = {0};
  DWORD dwConSize;
  register int attr;
  cursor.X = 0;
  cursor.Y = 0;
  GetConsoleScreenBufferInfo( hConsole, &csbi );
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter (hConsole, 32, dwConSize,coordScreen, &cCharsWritten);
  attr = color_fg + color_bg * 16;
  FillConsoleOutputAttribute (hConsole, attr, dwConSize,coordScreen, &cCharsWritten);
  locate(1,1,1);
}


int scan(char *input, char *format, ... )
{
  register int c,d ;
  char     *s_;
  int      *intptr;
  float    *floatptr;
  double   *doubleptr;
  char     A[50][2048];
  va_list  marker;
  c = 0;
  d = Split(A,input,",");
  va_start(marker, format); //Initialize arguments
  while(d && *format)
   {
     if(*format == '%') format++;
     if(*format == 's')
       {
         s_ = va_arg(marker, char *);
         strcpy(s_, A[c]);
         c++;
         d--;
       }
  if(*format == 'd')
    {
      intptr = va_arg(marker, int *);
      *intptr = atoi(A[c]);
      c++;
      d--;
    }
  if(*format == 'g')
    {
      floatptr = va_arg(marker, float *);
      *floatptr = atof(A[c]);
      c++;
      d--;
    }
 if(*format == 'l')
    {
      format++;
      doubleptr = va_arg(marker, double *);
      *doubleptr = atof(A[c]);
      c++;
      d--;
     }
  format++;
   }
  va_end(marker);              // Reset variable arguments
  if(d) return(1);             // More data than variables
  if(*format == 0) return(0);  // OK
  return(-1);                  // More variables than data
}


int Split (char Buf[][2048], char *T, char *Delim, int Flg)
{
  int  Begin = 0;
  int  Count = 0;
  int  Quote = 0;
  int  Index;
  int  lenT  = strlen(T);
  char Chr34[2]={34,0};
  for(Index=1;Index<=lenT;Index++)
    {
      if(instr(Delim,mid(T,Index,1))&&!Quote)
        {
          strcpy(Buf[Count],(char*)mid(T,Begin,Index-Begin));
          if ((Flg & 2) == 0)  // 0 if old version
           Count++;
          else
           if (Buf[Count][0] != 0) Count++;
           Begin=0;
           if((Flg & 1) == 1)   // 1 if true
              strcpy(Buf[Count++],(char*)mid(T,Index,1));
        }
   else
     {
       if(strcmp(mid(T,Index,1),Chr34)==0) Quote=!Quote;
       if(Begin==0) Begin=Index;
     }
   }
  if(Begin)
     strcpy(Buf[Count++],(char*)mid(T,Begin,Index-Begin));
  if((Flg & 1) == 0)   // 0 if false
      for(int i=0;i<=Count;i++) strcpy(Buf[i],(char*)RemoveStr(Buf[i],Chr34));
  return Count;
}


int PrinterOpen (void)
{
  int  PointSize=12;
  char zPrinter[2048];
  GetProfileString("WINDOWS","DEVICE","",zPrinter,127);
  strcpy(zPrinter,(char*)extract(zPrinter,","));
  strcpy(BcxPtr_Text,"Printing ...");
  BcxPtr_hDC=CreateDC("",zPrinter,"",0);
  if(!BcxPtr_hDC) return 0;
  BcxPtr_di.cbSize=sizeof(BcxPtr_di);
  BcxPtr_di.lpszDocName=BcxPtr_Text;
  StartDoc(BcxPtr_hDC,&BcxPtr_di);
  StartPage(BcxPtr_hDC);
  SetTextAlign(BcxPtr_hDC,TA_BASELINE | TA_NOUPDATECP | TA_LEFT);
  SetBkMode(BcxPtr_hDC,TRANSPARENT);
  BcxPtr_Lf.lfHeight=PointSize*GetDeviceCaps(BcxPtr_hDC,LOGPIXELSY)/72;
  BcxPtr_Lf.lfWidth=0;
  BcxPtr_Lf.lfEscapement=0;
  BcxPtr_Lf.lfOrientation=0;
  BcxPtr_Lf.lfWeight=FW_NORMAL;
  BcxPtr_Lf.lfItalic=0;
  BcxPtr_Lf.lfUnderline=0;
  BcxPtr_Lf.lfStrikeOut=0;
  BcxPtr_Lf.lfCharSet=ANSI_CHARSET;
  BcxPtr_Lf.lfOutPrecision=OUT_DEFAULT_PRECIS;
  BcxPtr_Lf.lfClipPrecision=CLIP_DEFAULT_PRECIS;
  BcxPtr_Lf.lfQuality=PROOF_QUALITY;
  BcxPtr_Lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
  strcpy(BcxPtr_Lf.lfFaceName,TEXT("Courier New"));
  BcxPtr_hFont=CreateFontIndirect(&BcxPtr_Lf);
  BcxPtr_hFontOld=(HFONT)SelectObject(BcxPtr_hDC,BcxPtr_hFont);
  GetTextMetrics(BcxPtr_hDC,&BcxPtr_tm);
  BcxPtr_FontMetrix=BcxPtr_Lf.lfHeight;
  BcxPtr_PrinterOn=1;
  return   1;
}


void PrinterWrite (char *TextIn)
{
  int LPP=60;
  int CPL=80;
  char sTemp[2048]={0};
  if(!BcxPtr_PrinterOn)
    {
      MessageBox (GetActiveWindow(),"Problem with Printer","",0);
      return;
    }
  strcpy(sTemp,TextIn);
  while(1)
    {
      if(strlen(sTemp)>CPL)
        {
          strcpy(BcxPtr_Text,(char*)left(sTemp,CPL));
          strcpy(sTemp,(char*)mid(sTemp,CPL+1));
        }
      else
        {
          strcpy(BcxPtr_Text,sTemp);
          *sTemp=0;
        }
      BcxPtr_LineCtr+=1;
      if(BcxPtr_LineCtr>=LPP)
        {
          EndPage(BcxPtr_hDC);
          BcxPtr_LineCtr=0;
          StartPage(BcxPtr_hDC);
        }
      TextOut(BcxPtr_hDC,20,BcxPtr_FontMetrix*BcxPtr_LineCtr,BcxPtr_Text,strlen(BcxPtr_Text));
      if(sTemp[0]==0) break;
    }
}


void PrinterClose (void)
{
  if(!BcxPtr_PrinterOn) return;
  SelectObject(BcxPtr_hDC,BcxPtr_hFontOld);
  DeleteObject(BcxPtr_hFont);
  EndPage(BcxPtr_hDC);
  EndDoc(BcxPtr_hDC);
  DeleteDC(BcxPtr_hDC);
  BcxPtr_LineCtr=0;
  BcxPtr_PrinterOn=0;
}


void EjectPage(void)
{
  EndPage(BcxPtr_hDC);
  BcxPtr_LineCtr=0;
  StartPage(BcxPtr_hDC);
}


