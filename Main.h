#include <afxcmn.h>

// Added for STL support
#ifdef max        /* Get rid of confilcting max def ?  */
#  undef max
#  undef min
#endif

#include <algorithm>      /* STL definitions      */
using namespace std;

#pragma warning (disable:4786)	// removes 'identifier was truncated to '255' characters in the browser information'
#pragma warning (disable:4018) // removes signed/unsigned warning
#pragma warning (disable : 4800 ) // forcing value to bool 'true' or 'false'
				// (performance warning)
