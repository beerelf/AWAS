#pragma once

#include <vector>
#include <string>
#include <fstream>

//! Class for limiting output by an arbitrary number of fields.
class CustOutputManager
{
public:
  CustOutputManager();
  CustOutputManager(const CustOutputManager&);
  CustOutputManager& operator=(const CustOutputManager&);

  bool Read(ifstream &istr);
  bool Write(ofstream &ostr);

  bool ShowInOutput(vector<bool> &outputCols);

  //! Return the total number of custom columns.
  int GetSize();

  //! Return the number of columns that are set to be displayed.
  int GetActiveCols();

  //! Return the index into the custom output manager from the active index.
  int GetIndexFromActive(int activecol);

  //! Return the active row after irow.  To start use -1.
  int GetNextRow(int irow);

  //! Set the number of columns.
  void SetSize(int s);

  //! Increase the size by one.
  void AddColumn();

  //! Return the short description of the column.
  string GetLabel(int irow);
  //! Return the long description of the column.
  string GetDescrip(int irow);
  //! Return whether the column is being used as a filter.
  bool GetActive(int irow);
  //! Return the label of the next column that is active.
  string GetActiveLabel(int irow);

  //! Set the short description of the column.
  void SetLabel(int irow, string val);
  //! Set the long description of the column.
  void SetDescrip(int irow, string val);
  //! Set whether the column is being used as a filter.
  void SetActive(int irow, bool val);

  //! If true, then use this column when filtering out sites from output.  Size is total number of columns.
  vector<string> m_labelCols;
  vector<string> m_descripCols;
  vector<bool> m_activeCols;
};
