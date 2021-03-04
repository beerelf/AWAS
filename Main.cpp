#include "Main.h"
#include "SDFdata.h"
#include <iostream>

int
main(int argc, char **argv)
{
  SitesManager sm;
  
  // Check for input arguments
  string retval;
  if (argc == 2) {
//      cerr << "Usage: " << argv[0] << " <name_of_input_file>" << endl;
//      return 1;
    //cout << "Reading project " << argv[1] << "." << endl;
    retval = sm.ReadProject(argv[1]);
    if (!retval.empty()) cout << retval << endl;
  }
  else {
    cout << "Enter name of input file:";
    char fname[100];
    cin >> fname;
    retval = sm.ReadProject(fname);
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
