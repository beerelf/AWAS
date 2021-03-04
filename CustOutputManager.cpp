#include "stdafx.h"
#include "CustOutputManager.h"

CustOutputManager::CustOutputManager()
{
  // Default is to have a single column called "Show"
  SetSize(1);

  m_labelCols[0] = "Show in Output";
  m_descripCols[0] = "Show in Output";
  m_activeCols[0] = true;
}

CustOutputManager::CustOutputManager(const CustOutputManager& rhs)
{
  m_labelCols = rhs.m_labelCols;
  m_descripCols = rhs.m_descripCols;
  m_activeCols = rhs.m_activeCols;
}

CustOutputManager&
CustOutputManager::operator=(const CustOutputManager& rhs)
{
  if (this == &rhs) return *this;

  m_labelCols = rhs.m_labelCols;
  m_descripCols = rhs.m_descripCols;
  m_activeCols = rhs.m_activeCols;

  return *this;
}

bool
CustOutputManager::Read(ifstream &istr)
{
  istr.getline(buf, sizeof(buf), '=');
  if (strstr(buf, "custom output count") == NULL) {
    return false;
  }
  int cnt;
  istr >> cnt;

  SetSize(cnt);

  for (int i=0; i<cnt; i++) {
    istr.getline(buf, sizeof(buf), '=');
    if (strstr(buf, "label") == NULL) {
      return false;
    }
    istr.getline(buf, sizeof(buf));
    SetLabel(i, buf);

    istr.getline(buf, sizeof(buf), '=');
    if (strstr(buf, "descrip") == NULL) {
      return false;
    }
    istr.getline(buf, sizeof(buf));
    SetDescrip(i, buf);
    
    istr.getline(buf, sizeof(buf), '=');
    if (strstr(buf, "active") == NULL) {
      return false;
    }
    istr.getline(buf, sizeof(buf));
    SetActive(i, string(buf).substr(0, 3) == "yes");
  }

  return true;
}

bool
CustOutputManager::Write(ofstream &ostr)
{
  ostr << "custom output count=" << GetSize() << endl;
  for (int i=0; i<GetSize(); i++) {
    ostr << "label=" << GetLabel(i) << endl;
    ostr << "descrip=" << GetDescrip(i) << endl;
    ostr << "active=" << (GetActive(i) ? "yes" : "no") << endl;
  }

  return true;
}

bool
CustOutputManager::ShowInOutput(vector<bool> &outputCols)
{
  int ncustomout = GetActiveCols();

  int nextrow = -1;
  // Iterate over the active columns and OR the values (need at least one column true).
  bool retval = false;
  for (int icol=0; icol<ncustomout; icol++) {
    nextrow = GetNextRow(nextrow);
    retval = retval || outputCols[nextrow];
  }

  return retval;
}

int
CustOutputManager::GetSize()
{
  return m_labelCols.size();
}

void
CustOutputManager::SetSize(int s)
{
  m_labelCols.resize(s);
  m_descripCols.resize(s);
  m_activeCols.resize(s);
}

int
CustOutputManager::GetActiveCols()
{
  int cnt(0);
  for (int isize=0; isize<m_activeCols.size(); isize++) {
    if (m_activeCols[isize]) cnt++;
  }

  return cnt;
}

int
CustOutputManager::GetIndexFromActive(int activecol)
{
  int cnt(-1);
  for (int isize=0; isize<m_activeCols.size(); isize++) {
    if (m_activeCols[isize]) cnt++;
    if (cnt == activecol) return isize;
  }

  return -1;
}

int
CustOutputManager::GetNextRow(int testrow)
{
  for (int irow=testrow+1; irow<GetSize(); irow++) {
    if (GetActive(irow)) {
      return irow;
      break;
    }
  }

  return -1;
}

void
CustOutputManager::AddColumn()
{
  m_labelCols.push_back("");
  m_descripCols.push_back("");
  m_activeCols.push_back(false);
}

string
CustOutputManager::GetLabel(int irow)
{
  if (irow >= GetSize()) AddColumn();
  return m_labelCols[irow];
}

string
CustOutputManager::GetDescrip(int irow)
{
  if (irow >= GetSize()) AddColumn();
  return m_descripCols[irow];
}

bool
CustOutputManager::GetActive(int irow)
{
  if (irow >= GetSize()) AddColumn();
  return m_activeCols[irow];
}

string
CustOutputManager::GetActiveLabel(int irow)
{
  int cnt(-1);
  for (int isize=0; isize<m_activeCols.size(); isize++) {
    if (m_activeCols[isize]) cnt++;
    if (cnt == irow) return m_labelCols[isize];
  }

  return "<error>"; 
}

void
CustOutputManager::SetLabel(int irow, string val)
{
  if (irow >= GetSize()) AddColumn();
  m_labelCols[irow] = val;
}

void
CustOutputManager::SetDescrip(int irow, string val)
{
  if (irow >= GetSize()) AddColumn();
  m_descripCols[irow] = val;
}

void
CustOutputManager::SetActive(int irow, bool val)
{
  if (irow >= GetSize()) AddColumn();
  m_activeCols[irow] = val;
}
