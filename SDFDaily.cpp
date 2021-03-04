// SDFDaily.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "SDFDaily.h"
#include "SDFdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  int nRetCode = 0;

  // initialize MFC and print and error on failure
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    {
      // TODO: change error code to suit your needs
      _tprintf(_T("Fatal Error: MFC initialization failed\n"));
      nRetCode = 1;
    }
  else  {
    SitesManager sm;
  
    // Check for input arguments
    string retval;
    if (argc == 2) {
      //      cerr << "Usage: " << argv[0] << " <name_of_input_file>" << endl;
      //      return 1;
      cout << "Reading project..." << endl;
      retval = sm.ReadProject(argv[1], true);
      if (!retval.empty()) cout << retval << endl;
    }
    else {
      cout << "Enter name of input file:";
      char fname[100];
      cin >> fname;
      retval = sm.ReadProject(fname, true);
      if (!retval.empty()) cout << retval << endl;
    }

    if (retval.empty()) {
      retval = sm.Execute();
      if (!retval.empty()) {
	cout << retval << endl;
	return 1;
      }    
      else {
	cout << "Model run successful." << endl;
	return 0;
      }
    }
    else {
      cout << retval << endl;
      return 1;
    }
  }

  return nRetCode;
}
