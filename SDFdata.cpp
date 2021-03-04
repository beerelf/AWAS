#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TextImportDlg.h"
#include "SDFdata.h"
#include "utils.h"
#include "stl_utils.h"

char* SiteData::m_timeUnitsStr[] = {"Days", "Weeks", "Months", "Years"};
vector<int> SiteData::daysPerMonth;

WellPumpingTD::wellUnitsEnum WellPumpingTD::m_pumpingUnits = WellPumpingTD::Acft;

// *** WARNING: this is copied from the SDFdaily project.  DO not edit this!!

//######################### SitesManager  ########################//

int SitesManager::NoData = -999;
float SitesManager::m_currentVersion = 2.9f;

SitesManager::SitesManager()
: m_startYear(0), m_endYear(0), m_histEndYear(0), m_histStartYear(0),
m_simStartYear(0), m_simEndYear(0), 
m_simStartMonth(0), m_runIgnoreMonth(11),
m_runIgnoreYear(0), m_useIgnoreYear(false),
m_displayMode(DISPLAY_ORIGINAL), m_yearMode(YEAR_CALENDAR),
m_projDataColor(RGB(200, 255, 0)),
m_forecastMode(ItemData::FORECAST_NONE), m_preForecastMode(ItemData::FORECAST_NONE),
m_useAverageDaysInMonth(false), m_useMonthlyURFForDaily(false),
m_copyIncludesHeader(false), m_useNewErrorFunc(false)
{
	for (int m=0; m<12; m_pMult[m++] = 1);
}

string
SitesManager::ReadProject(string fname, bool ignoreOutput, bool checkForYearType, bool addInPlace)
{
	// Check for text file import from SPCU.
	{
		int s = fname.length();
		if (s > 3) {
			if (fname.substr(s-3, 3) == "txt") {
				// Check for IDSCU export
				string retval = ImportProjectNew(fname, addInPlace, checkForYearType);
				if (retval == "IDSCU version")  {
					retval = ImportProject(fname, ItemData::CALENDAR, addInPlace, checkForYearType);
				}

				return retval;

				// *** disabled old format
#if 0
				// If this is an old format of the spreadsheet, then ask for year type.
				ItemData::YearTypeEnum yearType = ItemData::CALENDAR;

				ifstream istr(fname.c_str());
				istr.getline(buf, sizeof(buf));
				vector<string> tokens;
				stringtok(tokens, buf, "\t");
				if (tokens.size() > 5) {
					// New format, year type is stored in the spreadsheet.
					return ImportProjectNew(fname, addInPlace);
				}
				else {
					if (checkForYearType) {
						TextImportDlg dlg;
						if (dlg.DoModal() == IDOK) {
							yearType = (ItemData::YearTypeEnum)dlg.m_yearType;
						}
						else {
							return "";
						}
					}
					return ImportProject(fname, yearType, addInPlace);
				}
				// *** end disabled code
#endif
			}
			if (fname.substr(s-3, 3) == "sdf") {
				return ImportSDFView(fname, addInPlace);
			}
		}
	}

	if (*(fname.end()-1) == 'o') {
		ReadOutput(fname);
		// Request for output file, but read input first.
		//*(fname.end() -1) = 'i';
		return "";
	}

	string retval;
	int ival;

	ifstream istr(fname.c_str());
	if (!istr.is_open()) {
		retval += "Unable to open project file " + fname + "\n";
		return retval;
	}

	// Save basename so we know where to write results to.
	int loc = fname.rfind('.');
	m_baseName = fname.substr(0, loc);

	// First line is version (0 == original, 1 == modified
	istr.getline(buf, sizeof(buf), '=');
	if (strstr(buf, "version") == NULL) {
		retval += "Parse error looking for 'version=' header in file " + fname + "\n";
		return retval;
	}

	float version;
	istr >> version;

	if (version > m_currentVersion) {
		return "Error: the input file has a version number that is newer than the program version.  Please update the software by clicking \"Check for Updates\" in the help menu.";
	}

	if (!addInPlace)
	{
		m_yearMode = YEAR_CALENDAR;

		if (version >= 1.1f) {
			// Read period of record.  Makes sense only for daily form.
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "start year") == NULL) {
				retval += "Parse error looking for 'start year=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_startYear;

			// Read historical start year.
			if (version >= 1.7f) {
				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "historical start year") == NULL) {
					retval += "Parse error looking for 'historical start year=' header in file " + fname + "\n";
					return retval;
				}
				istr >> m_histStartYear;
			}

			if (version >= 1.6f) {
				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "historical end year") == NULL) {
					retval += "Parse error looking for 'historical end year=' header in file " + fname + "\n";
					return retval;
				}
				istr >> m_histEndYear;
			}

			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "end year") == NULL) {
				retval += "Parse error looking for 'end year=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_endYear;

			if (version >= 1.7f) {
				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "synth label") == NULL) {
					retval += "Parse error looking for 'synth label=' header in file " + fname + "\n";
					return retval;
				}
				istr.getline(buf, sizeof(buf));
				m_synthDataLabel = buf;

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "presynth label") == NULL) {
					retval += "Parse error looking for 'presynth label=' header in file " + fname + "\n";
					return retval;
				}
				istr.getline(buf, sizeof(buf));
				m_preSynthDataLabel = buf;

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "synth mode") == NULL) {
					retval += "Parse error looking for 'synth mode=' header in file " + fname + "\n";
					return retval;
				}
				int ival;
				istr >> ival;
				m_forecastMode = (ItemData::ForecastEnum)ival;

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "presynth mode") == NULL) {
					retval += "Parse error looking for 'presynth mode=' header in file " + fname + "\n";
					return retval;
				}
				istr >> ival;
				m_preForecastMode = (ItemData::ForecastEnum)ival;

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "number of synth years") == NULL) {
					retval += "Parse error looking for 'number of synth years=' header in file " + fname + "\n";
					return retval;
				}

				int nitems;
				istr >> nitems;

				m_yearList.resize(nitems);
				unsigned int i;
				for (i=0; i<nitems; i++) {
					istr >> m_yearList[i];
				}

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "number of presynth years") == NULL) {
					retval += "Parse error looking for 'number of presynth years=' header in file " + fname + "\n";
					return retval;
				}
				istr >> nitems;

				m_preYearList.resize(nitems);
				for (i=0; i<nitems; i++) {
					istr >> m_preYearList[i];
				}
			}

			if (version < 1.6f) m_histEndYear = m_endYear;

			if (version < 1.7f) m_histStartYear = m_startYear;

			if (version >= 1.3f)  {
				if (version < 1.5f) {
					istr.getline(buf, sizeof(buf), '=');
					if (strstr(buf, "native year mode") == NULL) {
						retval += "Parse error looking for 'native year mode=' header in file " + fname + "\n";
						return retval;
					}
					istr >> ival;
					//m_nativeYearMode = (SitesManager::YearModeEnum)ival;
				}

				istr.getline(buf, sizeof(buf), '=');
				if (strstr(buf, "view year mode") == NULL) {
					retval += "Parse error looking for 'view year mode=' header in file " + fname + "\n";
					return retval;
				}
				istr >> ival;
				m_yearMode = (SitesManager::YearModeEnum)ival;
			}
		}

		if (version >= 1.4f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "display mode") == NULL) {
				retval += "Parse error looking for 'display=' header in file " + fname + "\n";
				return retval;
			}
			istr >> ival;
			m_displayMode = (DisplayModeEnum)ival;
		}

		SiteData::TimeUnitsEnum timeUnits;
		if (version >= 1.7f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "time units") == NULL) {
				retval += "Parse error looking for 'time units=' header in file " + fname + "\n";
				return retval;
			}
			istr >> ival;
			timeUnits = (SiteData::TimeUnitsEnum)ival;
		}


		if (version >= 1.8f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "use average days per month") == NULL) {
				retval += "Parse error looking for 'use average days per month=' header in file " + fname + "\n";
				return retval;
			}
			istr >> ival;
			m_useAverageDaysInMonth = (bool)ival;
		}

		if (version >= 2.9f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "URF uses monthly values for daily calculation") == NULL) {
				retval += "Parse error looking for 'URF uses monthly values for daily calculation=' header in file " + fname + "\n";
				return retval;
			}
			istr >> ival;
			m_useMonthlyURFForDaily = (bool)ival;
		}

		if (version >= 2.6f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "use new error function") == NULL) {
				retval += "Parse error looking for 'use new error function=' header in file " + fname + "\n";
				return retval;
			}
			istr >> ival;
			m_useNewErrorFunc = (bool)ival;
		}

		if (version >= 1.9f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "run start") == NULL) {
				retval += "Parse error looking for 'run start=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_simStartYear;

			if (version >= 2.0f) {
				// Start month is also included.
				istr >> m_simStartMonth;
			}

			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "run end") == NULL) {
				retval += "Parse error looking for 'run end=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_simEndYear;

			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "run ignore year") == NULL) {
				retval += "Parse error looking for 'run ignore year=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_runIgnoreYear;

			if (version >= 2.0f) {
				// Start month is also included.
				istr >> m_runIgnoreMonth;
			}

			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "use run ignore year") == NULL) {
				retval += "Parse error looking for 'use run ignore year=' header in file " + fname + "\n";
				return retval;
			}
			int ival;
			istr >> ival;
			m_useIgnoreYear = ival;
		}
		else {
			m_simStartYear = m_startYear;
			m_simEndYear = m_endYear;
			m_runIgnoreYear = m_endYear;
		}

		if (version >= 2.1f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "projected data multipliers") == NULL) {
				retval += "Parse error looking for 'projected data multipliers=' header in file " + fname + "\n";
				return retval;
			}
			for (int m=0; m<12; m++) {
				istr >> m_pMult[m];
			}
		}

		if (version >= 2.5f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "copy includes header") == NULL) {
				retval += "Parse error looking for 'copy includes header=' header in file " + fname + "\n";
				return retval;
			}
			istr >> m_copyIncludesHeader;
		}
	}
	else
	{
		// Skip to the number of sites line.
		istr.getline(buf, sizeof(buf));
		// This is the line before number of sites
		while (strstr(buf, "copy includes header") == NULL) {
			istr.getline(buf, sizeof(buf));
		}
	}

	istr.getline(buf, sizeof(buf), '=');
	if (strstr(buf, "number of sites") == NULL) {
		retval += "Parse error looking for 'number of sites=' header in file " + fname + "\n";
		return retval;
	}

	int nsites;
	istr >> nsites;

	m_siteList.clear();

	int nyears = m_endYear - m_startYear + 1;

	for (int isite=0; isite<nsites; isite++) {
		SiteData sd;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "header1") == NULL) {
			retval += "Parse error looking for 'header1=' header in file " + fname + "\n";
			return retval;
		}

		istr.getline(buf, sizeof(buf));
		sd.m_header1 = buf;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "header2") == NULL) {
			retval += "Parse error looking for 'header2=' header in file " + fname + "\n";
			return retval;
		}

		istr.getline(buf, sizeof(buf));
		sd.m_header2 = buf;

		if (version >= 1.2f) {
			// Read in pumping type, well or recharge
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "pumping type") == NULL) {
				retval += "Parse error looking for 'pumping type=' header in file " + fname + "\n";
				return retval;
			}

			istr.getline(buf, sizeof(buf));
			string cbuf(buf);

			sd.m_siteType = (cbuf == "irrigation" ? SiteData::WELL : SiteData::RECHARGE);
		}

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "boundary condition") == NULL) {
			retval += "Parse error looking for 'boundary condition=' header in file " + fname + "\n";
			return retval;
		}

		int ival;
		istr >> ival;
		sd.m_bi = (SiteData::BoundaryConditionsEnum)ival;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "compute depletion for segment") == NULL) {
			retval += "Parse error looking for 'compute depletion for segment=' header in file " + fname + "\n";
			return retval;
		}

		istr >> ival;
		sd.m_computeDepletionForSegment = (ival == 1) ? TRUE : FALSE;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "transmissivity (tr)") == NULL) {
			retval += "Parse error looking for 'transmissivity (tr)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_tr;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "specific yield (s)") == NULL) {
			retval += "Parse error looking for 'specific yield (s)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_s;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "distance from well to stream (dxx)") == NULL) {
			retval += "Parse error looking for 'distance from well to stream (dxx)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_dxx;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "time units") == NULL) {
			retval += "Parse error looking for 'time units=' header in file " + fname + "\n";
			return retval;
		}

		istr >> ival;
		if (m_displayMode == DISPLAY_MODIFIED) {
			if (isite == 0) SetTimeUnits((SiteData::TimeUnitsEnum)ival);
			sd.SetTimeUnits((SiteData::TimeUnitsEnum)ival);
			sd.SetPeriod(m_startYear, m_endYear);
		}
		else {
			if (ival < 0 || ival > 3) ival = 0;
			sd.m_timeUnits = (SiteData::TimeUnitsEnum)ival;
		}

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "number of time units between printouts (tbp)") == NULL) {
			retval += "Parse error looking for 'number of time units between printouts (tbp)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_tbp;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "number of cycles (seasons) to be simulated (nc)") == NULL) {
			retval += "Parse error looking for 'number of cycles (seasons) to be simulated (nc)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_nc;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "effective sdf") == NULL) {
			retval += "Parse error looking for 'effective sdf=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_sdf;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "distance from the stream to the parallel impermeable boundary (w)") == NULL) {
			retval += "Parse error looking for 'distance from the stream to the parallel impermeable boundary (w)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_w;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "distance between well and no flow boundary (b)") == NULL) {
			retval += "Parse error looking for 'distance between well and no flow boundary (b)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_b;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "calculate depletion for segment of stream (zzseg)") == NULL) {
			retval += "Parse error looking for 'calculate depletion for segment of stream (zzseg)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_zzseg;

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "length of stream (z1, z2)") == NULL) {
			retval += "Parse error looking for 'length of stream (z1, z2)=' header in file " + fname + "\n";
			return retval;
		}

		istr >> sd.m_z1 >> sd.m_z2;

		bool showInOutput = true;
		if (version >= 2.2f && version < 2.4f) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "show in output") == NULL) {
				retval += "Parse error looking for 'show in output=' header in file " + fname + "\n";
				return retval;
			}

			string sval;
			istr >> sval;

			showInOutput = (sval == "yes");
		}

		istr.getline(buf, sizeof(buf), '=');
		if (strstr(buf, "number of pumping records") == NULL) {
			retval += "Parse error looking for 'number of pumping records=' header in file " + fname + "\n";
			return retval;
		}

		int nrecs;
		istr >> nrecs;

		if (m_displayMode == DISPLAY_ORIGINAL) {
			sd.m_pumpingRecTD.SetSpan(nrecs);
			sd.m_metaDataTD.SetSpan(nrecs);
			if (sd.m_siteType == SiteData::WELL) {
				sd.m_wellPumpingTD.SetSpan(nrecs);
			}
			else {
				sd.m_rechargeTD.SetSpan(nrecs);
			}
		}

		for (int irec=0; irec<nrecs; irec++) {
			istr.getline(buf, sizeof(buf), '=');
			if (strstr(buf, "delt,q,meta") == NULL) {
				retval += "Parse error looking for 'delt,q=' header in file " + fname + "\n";
				return retval;
			}

			double q;
			int delt;

			istr >> delt >> q;

			if (version < 2.6f && m_timeUnits == SiteData::MONTHS) {
				// Convert from actual days per month to average days per month.
				int m = irec % 12;
				int y = irec / 12;
				double ndays = getDaysInMonth(m, m_startYear + y);
				q = q * ndays / 30.41667;
			}

			sd.m_pumpingRecTD.SetValue(PumpingRecTD::Delta, irec, delt);
			sd.m_pumpingRecTD.SetValue(PumpingRecTD::Q, irec, q);

			sd.m_tt += delt;

			int meta;
			istr >> meta;
			sd.m_metaDataTD.SetValue(MetaDataTD::MetaData, irec, meta);
		}
		sd.m_tt *= sd.m_nc;

		try {
			if (sd.m_siteType == SiteData::WELL) {
				istr >> sd.m_wellPumpingTD;
			}
			else {
				istr >> sd.m_rechargeTD;
			}
		}
		catch (InputError err) {
			AfxMessageBox((err.ErrorMsg() + " in file " + fname).c_str());
			return err.ErrorMsg() + " in file " + fname;
		}

		if (version >= 2.3f) {
			// Read show in output values.
			try {
				nrecs = ParseInput<int>(istr, "Number of output values");
				sd.m_outputCols.resize(nrecs);
				for (int i=0; i<nrecs; i++) {
					string active = ParseInputString(istr, "Output value");
					sd.m_outputCols[i] = (active == "yes");
				}
			} catch (ParseInputException *pie) {
				AfxMessageBox((pie->ErrorMsg() + " in file " + fname).c_str());
				delete pie;
			}
		}
		else {
			// Older version use the first value.
			sd.m_outputCols.resize(1);
			sd.m_outputCols[0] = showInOutput;
		}

		// Change nodata to zero.
		sd.m_pumpingRecTD.Fill(PumpingRecTD::Q, 0, 0, -1, true);

		m_siteList.push_back(sd);
	}

	if (version >= 2.3f) {
		if (!m_custOutput.Read(istr)) {
			AfxMessageBox("Failed to read custom output.");
		}
	}

	// Check for output.
	istr.close();

	// Fix customized output masks.
	UpdateCustOutput();

	ReadURF_Data(fname, version);

	// This will correct junk values in out of period non-calendar year dates to nodata.
	FixOutOfBoundsData();

	if (!ignoreOutput) ReadOutput(fname);

	return "";
}

string
SitesManager::ImportProject(string fname, ItemData::YearTypeEnum yearType, bool addInPlace, bool checkForYearType)
{
	ifstream istr(fname.c_str());
	if (!istr.is_open()) {
		return "Unable to open project file " + fname + "\n";
	}

	// Save basename so we know where to write results to.
	int loc = fname.rfind('.');
	m_baseName = fname.substr(0, loc);

	YearModeEnum oldYearMode = m_yearMode;

	if (checkForYearType)
	{
		TextImportDlg dlg;
		if (dlg.DoModal() == IDOK) {
			m_yearMode = (YearModeEnum)dlg.m_yearType;
			yearType = (ItemData::YearTypeEnum)m_yearMode; 
		}
	}
	else
	{
		m_yearMode = (YearModeEnum)yearType;
	}

	istr.getline(buf, sizeof(buf));
	istringstream istrstr(buf);

	// Check for token count to make sure the input is legal.
	vector<string> tokens;
	stringtok(tokens, buf, "\t"); 
	bool valid = false;
	if (tokens.size() >= 4)
	{
		// make sure first three tokens are numbers.
		for (int i=0; i<3; i++)
		{
			if (!isNumber(tokens[i]))
			{
				break;
			}
		}
		valid = true;
	}
	if (!valid)
	{
		return "This is not a valid import file.  It must start with the project header (start year, end year, historical end year)";
	}

	int histEndYear;
	string timeType;
	int imp_startYear=-1, imp_endYear=-1, imp_histEndYear=-1;
	istrstr >> imp_startYear >> imp_endYear >> imp_histEndYear >> timeType;

	bool import = false;
	if (m_endYear < 1)
	{
		// new project, building from scratch
		m_startYear = imp_startYear;
		m_endYear = imp_endYear;
		histEndYear = imp_histEndYear;

		SetTimeUnits((timeType == "monthly") ? SiteData::MONTHS : SiteData::DAYS);

		// Allow for projected data from CUModel.
		TRACE0("Warning: projected input not handled.\n");

		// If the year is not calendar, then add an extra year at the beginning.
		int offsetYear(0);
		if (m_yearMode != ItemData::CALENDAR) {
			offsetYear = 1;
		}

		m_histStartYear = m_startYear - offsetYear;
		m_histEndYear = m_endYear;
		SetPeriod(m_startYear - offsetYear, m_endYear);
	}
	else
	{
		import = true;
	}

	int nyears = m_endYear - m_startYear + 1;

	int imp_nyears = imp_endYear - imp_startYear + 1;

	m_displayMode = DISPLAY_MODIFIED;

	// Keep a list of sites updated.
	vector<int> sitesUpdated;
	vector<string> sitesNotFound;

	int icnt = -1;
	while (istr.good()) {
		// Second record is well info
		istr.getline(buf, sizeof(buf));
		if (trim(buf).empty()) continue;
		//if (!istr.good()) break;
		istringstream istrstr(buf);

		char buf1[256];
		istrstr.getline(buf1, sizeof(buf1), '\t');

		string newSiteHeader = trim(buf1);

		icnt++;

		// Search for a site with the name and update its values; otherwise add
		//   a new site.
		int idx = -1;
		if (!addInPlace)
		{
			idx = FindSite(newSiteHeader);
		}
		else
		{
			if (icnt < m_siteList.size())
			{
				idx = icnt;
			}
		}
		bool found = idx >= 0;
		if (!found) {
			// Not found; add new.
			idx = AddSite(newSiteHeader, "imported from IDSCU");
			sitesNotFound.push_back(newSiteHeader);
		}
		else
		{
			// check that sites weren't updated multiple times.
			if (find(sitesUpdated, idx) != sitesUpdated.end())
			{
				AfxMessageBox(("Warning: site " + m_siteList[idx].m_header1 + " updated more than once by " + newSiteHeader).c_str());
			}
			sitesUpdated.push_back(idx);
		}


		SiteData &sd = m_siteList[idx];

		sd.SetTimeUnits(m_timeUnits);

		istrstr >> sd.m_sdf;

		enum SiteData::SiteTypeEnum st = SiteData::WELL;
		if (istrstr.good()) {
			string cbuf;
			istrstr >> cbuf;
			if (cbuf.empty() || cbuf[0] == '0') {
				st = SiteData::WELL;
			}
			else {
				st = SiteData::RECHARGE;
			}
		}

		sd.SetSiteType(st);

		// Read in other params.  Only update if they are > 0
		float params[5];
		for (int i=0; i<5; params[i++] = 0);
		if (istrstr.good()) istrstr >> params[0];
		if (istrstr.good()) istrstr >> params[1];
		if (istrstr.good()) istrstr >> params[2];
		if (istrstr.good()) istrstr >> params[3];
		if (istrstr.good()) istrstr >> params[4];

		if (params[0] > 0) sd.m_w = params[0];
		if (params[1] > 0) sd.m_b = params[1];
		if (params[2] > 0) sd.m_tr = params[2];
		if (params[3] > 0) sd.m_s = params[3];
		if (params[4] > 0) sd.m_dxx = params[4];

		//if (istrstr.good()) istrstr >> sd.m_w; // distance from the stream to the parallel impermeable boundary
		//if (istrstr.good()) istrstr >> sd.m_b; // distance between well and no flow boundary
		//if (istrstr.good()) istrstr >> sd.m_tr; // transmissivity
		//if (istrstr.good()) istrstr >> sd.m_s; // specific yield
		//if (istrstr.good()) istrstr >> sd.m_dxx; // distance from well to stream

		if (!found) {
			// Try to guess mode.
			if (sd.m_w > 0) sd.m_bi = SiteData::ALLUVIAL_AQUIFER;
			else if (sd.m_dxx > 0) sd.m_bi = SiteData::INFINITE_AQUIFER;
			else if (sd.m_b > 0) sd.m_bi = SiteData::NO_FLOW;
			else sd.m_bi = SiteData::EFFECTIVE_SDF;
		}

		sd.m_pumpingRecTD.SetNYears(nyears, m_startYear);
		sd.m_metaDataTD.SetNYears(nyears, m_startYear);

		// Read pumping
		for (int iyear=0; iyear<imp_nyears; iyear++) {
			if (!istr.good()) break;
			istr.getline(buf, sizeof(buf));
			istringstream istrstr(buf);

			int yearToken;
			istrstr >> yearToken;

			if (yearToken != imp_startYear + iyear) {
				AfxMessageBox(("Read pump year " + to_string<int>(yearToken)
					+ " for well " + sd.m_header1
					+ "; should be year "
					+ to_string<int>(imp_startYear + iyear)).c_str());
				return FALSE;
			}

			int cnt(0);
			while (istrstr.good()) {
				// Check for junk at the end.
				string sval;
				istrstr >> sval;
				if (sval.empty()) break;

				double q;
				q = atof(sval.c_str());

				//SetSiteData(q, -1, idx, 0, 

				double gpmval = q * 325851.0/1440;
				//double gpmval = q * 325900.0/1440;

				if (m_timeUnits == SiteData::MONTHS) {
					// convert daily -> monthly
					//int days = getDaysInMonth(cnt%12, m_startYear + iyear, yearType);

					// New try: use average days in month.  This will cause the modified form to conform to the model.
					double days = 30.41667;
					gpmval /= days; // convert monthly -> daily
				}

				int jm, jd;
				if (m_timeUnits == SiteData::DAYS) {
					jday2date(cnt, yearToken, &jd, &jm);
					// Change date to start from 0.
					jm--;
					jd--;
				}
				else {
					jm = cnt;
					jd = -1;
				}

				sd.m_pumpingRecTD.SetValueDate(yearType, PumpingRecTD::Delta, 1, yearToken, jm, jd);
				sd.m_pumpingRecTD.SetValueDate(yearType, PumpingRecTD::Q, gpmval, yearToken, jm, jd);
				sd.m_metaDataTD.SetValueDate(yearType, MetaDataTD::MetaData, gpmval, yearToken, jm, jd);

				cnt++;
			}
		}
	}

	if (sitesNotFound.size() > 0 && import)
	{
		string out_str;
		for (int i=0; i<sitesNotFound.size(); i++)
		{
			out_str += sitesNotFound[i] + "\r\n";
		}
		AfxMessageBox(("Sites in import list that were not matched in original project: \r\n" + out_str).c_str());
	}

	// Restore original year mode.
	if (oldYearMode != m_yearMode)
	{
		m_yearMode = oldYearMode;
	}

	return "";
}

string
SitesManager::ImportProjectNew(string fname, bool addInPlace, bool checkForYearType)
{
	ifstream istr(fname.c_str());
	if (!istr.is_open()) {
		return "Unable to open project file " + fname + "\n";
	}

	// Check for compact format.
	YearModeEnum oriYearMode = m_yearMode;

	bool compactFormat = false;
	istr.getline(buf, sizeof(buf));
	// Test for three numbers in a row.  If not found, then the format must be compact.
	vector<string> toks;
	stringtok(toks, buf, "\t");
	if (toks.size() == 4)
	{
		return "IDSCU version";
	}

	if (!(toks.size() >= 3 && toks[0].size() == 4 && toks[1].size() == 4 && toks[2].size() == 4 && atoi(toks[0].c_str()) > 1900 && atoi(toks[1].c_str()) > 1900 && atoi(toks[2].c_str()) > 1900)) { 
		if (GetStartYear() == 0) {
			// Flag error that compact format can only be used with an existing dataset.
			return "Error: this dataset can only be imported into an existing project.";
		}
		compactFormat = true;

		// Rewind back to beginning.
		istr.seekg(0);

		// Ask the user for year type.
		checkForYearType = TRUE;
	}

	if (checkForYearType)
	{
		TextImportDlg dlg;
		if (dlg.DoModal() == IDOK) {
			m_yearMode = (YearModeEnum)dlg.m_yearType;
		}
	}

	// Save basename so we know where to write results to.
	int loc = fname.rfind('.');
	m_baseName = fname.substr(0, loc);

	// This beginning is the reverse of CSDFDailyDoc::WriteSpreadsheetHeader
	istringstream istrstr(buf);

	int imp_nyears = 1; // compact format reads one year at a time.

	if (!compactFormat) {
		int histEndYear;
		string timeUnits;
		string sYearMode;

		// Check token count.  IDSCU export files have 4 tokens.
		vector<string> toks;
		stringtok(toks, buf);
		if (toks.size() >= 6)
		{
			istrstr >> m_startYear >> m_endYear >> m_histStartYear >> m_histEndYear >> timeUnits >> sYearMode;
		}
		else if (toks.size() == 4)
		{
			// IDSCU format
			istrstr >> m_startYear >> m_endYear >> m_histEndYear >> timeUnits;
			m_histStartYear = m_startYear;
			sYearMode = "calendar";
		}

		sYearMode = sToLower(sYearMode);
		if (sYearMode == "calendar") {
			m_yearMode = YEAR_CALENDAR;
		}
		else if (sYearMode == "irrigation") {
			m_yearMode = YEAR_IRRIGATION;
			// reduce the start years by one because the native format is calendar, which forces an extra year at the beginning to represent all the data.
			m_startYear--;
			m_histStartYear--;
		}
		else if (sYearMode == "usgs") {
			m_yearMode = YEAR_USGS;
			// reduce the start years by one because the native format is calendar, which forces an extra year at the beginning to represent all the data.
			m_startYear--;
			m_histStartYear--;
		}

		imp_nyears = m_endYear - m_startYear + 1;
		if (m_yearMode != YEAR_CALENDAR) { // Subtract a year because non-calendar year types start a year early.
			imp_nyears--;
		}

		m_displayMode = DISPLAY_MODIFIED;

		SetTimeUnits((sToLower(timeUnits) == "monthly") ? SiteData::MONTHS : SiteData::DAYS);

		// Read customized output options
		m_custOutput.SetSize(0);
		char fbuf[20];
		istrstr >> ws;
		istrstr.getline(fbuf, sizeof(fbuf), '\t');
		while (string(fbuf).length() > 0) // "Format is..."
		{
			if (!trim(fbuf).empty() && trim(fbuf).find("Format") == string::npos)
			{
				m_custOutput.AddColumn();
				m_custOutput.SetActive(m_custOutput.GetSize()-1, true);
				m_custOutput.SetLabel(m_custOutput.GetSize()-1, fbuf);
			}
			fbuf[0] = '\0';
			istrstr.getline(fbuf, sizeof(fbuf), '\t');
		}

		// Read forecasting data.
		if (m_histStartYear != m_startYear) {
			istr.getline(buf, sizeof(buf));
			istrstr.str(buf);
			istrstr.clear();
			istrstr.seekg(0);

			istrstr.getline(fbuf, sizeof(fbuf), '\t');
			string forcastMode = sToLower(fbuf);
			if (forcastMode == "none") {
				m_preForecastMode = ItemData::FORECAST_NONE;
			}
			else if (forcastMode == "average") {
				m_preForecastMode = ItemData::FORECAST_AVERAGE;
			}
			else if (forcastMode == "single year") {
				m_preForecastMode = ItemData::FORECAST_GIVEN_YEAR;
			}
			else if (forcastMode == "cycle") {
				m_preForecastMode = ItemData::FORECAST_HISTORICAL;
			}

			// Read years until we run out.
			m_preYearList.clear();
			while (istrstr.good()) {
				int year;
				istrstr >> year;
				m_preYearList.push_back(year);
			}

			// Build synthesize label.
			m_preSynthDataLabel = CreateSynthLabel(m_preForecastMode, m_preYearList);
		}
		if (m_histEndYear != m_endYear) {
			istr.getline(buf, sizeof(buf));
			istrstr.str(buf);
			istrstr.clear();
			istrstr.seekg(0);

			char fbuf[20];
			istrstr.getline(fbuf, sizeof(fbuf), '\t');
			string forcastMode = sToLower(fbuf);
			if (forcastMode == "none") {
				m_forecastMode = ItemData::FORECAST_NONE;
			}
			else if (forcastMode == "average") {
				m_forecastMode = ItemData::FORECAST_AVERAGE;
			}
			else if (forcastMode == "single year") {
				m_forecastMode = ItemData::FORECAST_GIVEN_YEAR;
			}
			else if (forcastMode == "cycle") {
				m_forecastMode = ItemData::FORECAST_HISTORICAL;
			}

			// Read years until we run out.
			m_yearList.clear();
			while (istrstr.good()) {
				int year;
				istrstr >> year;
				m_yearList.push_back(year);
			}

			m_synthDataLabel = CreateSynthLabel(m_forecastMode, m_yearList);
		}

		SetPeriod(m_startYear, m_endYear);
	}

	int nyears = GetEndYear() - GetStartYear() + 1;

	int icnt = -1;
	while (istr.good()) {
		// Second record is well info
		istr.getline(buf, sizeof(buf));
		if (trim(buf).empty()) continue;
		//if (!istr.good()) break;

		// Look for header.
		if (string(buf).find("Diversions/Recharge (ac-ft)") != string::npos) continue; // Skip this line
		// Check if we are out of input
		if (string(buf).find("Depletions/Accretions (ac-ft)") != string::npos) break;

		istringstream istrstr(buf);

		char buf1[256];
		istrstr.getline(buf1, sizeof(buf1), '\t');

		string name(trim(buf1));

		// Strip quotation marks if present
		if (name[0] == '\"') {
			name = name.substr(1, name.length()-2);
		}
		icnt++;

		// Search for a site with the name and update its values; otherwise add
		//   a new site.
		int idx = -1;
		if (!addInPlace)
		{
			idx = FindSite(name);
		}
		else
		{
			// Add to each site in project order.
			if (icnt < m_siteList.size())
			{
				idx = icnt;
			}
		}

		if (idx == -1) {
			// Not found; add new.
			// Check if header 2 is present by checking format string
			istrstr.getline(buf1, sizeof(buf1), '\t');
			string name2 = trim(buf1);
			// Strip quotation marks if present
			if (name2[0] == '\"') {
				name2 = name2.substr(1, name2.length()-2);
			}

			idx = AddSite(name, name2);
		}

		SiteData &sd = m_siteList[idx];

		sd.SetTimeUnits(m_timeUnits);

		if (!compactFormat) {
			istrstr >> sd.m_sdf;

			enum SiteData::SiteTypeEnum st = SiteData::WELL;
			int signchange = 1;    // Assume that diversions are negative, recharge is positive.
			if (istrstr.good()) {
				string cbuf;
				istrstr >> cbuf;

				if (cbuf == "Well") {
					signchange = -1;
					st = SiteData::WELL;
				}
				else {
					signchange = 1;
					st = SiteData::RECHARGE;
				}
			}

			sd.SetSiteType(st);

			// Read in other params
			if (istrstr.good()) istrstr >> sd.m_w; // distance from the stream to the parallel impermeable boundary
			if (istrstr.good()) istrstr >> sd.m_b; // distance between well and no flow boundary
			if (istrstr.good()) istrstr >> sd.m_tr; // transmissivity
			if (istrstr.good()) istrstr >> sd.m_s; // specific yield
			if (istrstr.good()) istrstr >> sd.m_dxx; // distance from well to stream
			if (istrstr.good()) istrstr >> sd.m_zzseg; // partial segments on?
			if (istrstr.good()) istrstr >> sd.m_z1; // left partial segments position
			if (istrstr.good()) istrstr >> sd.m_z2; // right partial segments position
			if (istrstr.good()) {
				// Remove leading tab
				istrstr >> ws;
				istrstr.getline(buf1, sizeof(buf1), '\t');
				string sbc = sToLower(buf1); // boundary condition
				if (sbc == "infinite aquifer") {
					sd.m_bi = SiteData::INFINITE_AQUIFER ;
				}
				else if (sbc == "alluvial aquifer") {
					sd.m_bi = SiteData::ALLUVIAL_AQUIFER;
				}
				else if (sbc == "no flow") {
					sd.m_bi = SiteData::NO_FLOW;
				}
				else if (sbc == "effective sdf") {
					sd.m_bi = SiteData::EFFECTIVE_SDF;
				}
				else if (sbc.substr(0, 3) == "urf") {
					sd.m_bi = SiteData::URF;

					// Read in URFs
					vector<string> tokens;
					stringtok(tokens, sbc);

					// First token is URF:
					// Second is count of URFs
					int nurf = atoi(tokens[1].c_str());
					sd.m_URF_MonthlyData.resize(nurf);

					for (int icol=0; icol<nurf; icol++) {
						sd.m_URF_MonthlyData[icol] = atof(tokens[icol+2].c_str());
					}

					// Next read daily URFs
					int nurf_daily = atoi(tokens[nurf + 2].c_str());
					sd.m_URF_DailyData.resize(nurf_daily);

					for (int icol=0; icol<nurf_daily; icol++) {
						sd.m_URF_DailyData[icol] = atof(tokens[nurf + icol + 3].c_str());
					}
				}

				// Read customize options.
				sd.m_outputCols.resize(0);

				string tok;
				istrstr.getline(buf1, sizeof(buf1), '\t');
				tok = buf1;

				// Check for old version where I accidently outputted the partial seg settings twice.
				if (tok.find("Partial Segments") != string::npos)
				{
					// Skip to next cell.
					istrstr.getline(buf1, sizeof(buf1), '\t');
					istrstr.getline(buf1, sizeof(buf1), '\t');
					istrstr.getline(buf1, sizeof(buf1), '\t');
					tok = buf1;
				}

				while (trim(tok).find("format") == string::npos && (istrstr.good() || tok.length() > 0)) // "Format is..."
				{
					sd.m_outputCols.push_back(tok == "1");
					tok = "";
					istrstr >> tok;
				}
			}

			// Don't use nyears because nyears will be wrong for non-calendar years.
			sd.m_pumpingRecTD.SetNYears(m_endYear - m_startYear + 1, m_startYear);
			sd.m_metaDataTD.SetNYears(m_endYear - m_startYear + 1, m_startYear);
		}

		int nmonths = 1;
		// If daily, then we need to read a line of daily pumping by month
		if (m_timeUnits == SiteData::DAYS) {
			nmonths = 12;
		}

		// Read pumping
		for (int iyear=0; iyear<imp_nyears; iyear++) {
			for (int imonth=0; imonth<nmonths; imonth++) {
				if (!istr.good()) break;
				if (!compactFormat || imonth > 0) {
					// Compact form does not read each line here
					istr.getline(buf, sizeof(buf));
				}
				istringstream istrstr(buf);

				if (compactFormat) {
					// This style has well_name year m1_val m2_val ..., so don't read new line and instead skip past well_name.
					istrstr.getline(buf1, sizeof(buf1), '\t');
				}

				int yearToken;
				istrstr >> yearToken;

				int yearTokenCal = yearToken;
				if (m_timeUnits == SiteData::DAYS)
				{
					// Adjust the year value for non-calendar year so that it matches the calendar year.
					//   ie, 11/99 should really be year 2000.
					yearTokenCal = getCalYearFromYear(imonth, yearToken, (ItemData::YearTypeEnum)m_yearMode);
				}

				if (yearTokenCal != GetStartYear() + iyear && !compactFormat) {
					AfxMessageBox(("Read pump year " + to_string<int>(yearToken)
						+ " for well " + sd.m_header1
						+ "; should be year "
						+ to_string<int>(m_startYear + iyear)).c_str());
					return FALSE;
				}

				// The number of tokens on each line.
				int ntokens = 12;

				int monthToken=-1;
				if (m_timeUnits == SiteData::DAYS) {
					// Read month
					istrstr >> monthToken;
					// Don't use this month because the output uses calendar-year counting, so Nov irrigation year == 11, etc.
					//   The data structures all assume that the month starts from 1 regardless of year type.
					ntokens = getDaysInMonth(imonth, yearTokenCal, (ItemData::YearTypeEnum)m_yearMode);
				}

				int cnt(0);
				while (istrstr.good() && cnt < ntokens) {
					double q;
					istrstr >> q;

					// Patterson: disabled because I don't 
					//q = fabs(q); // diversions should be positive

					double gpmval = q * 325851.0/1440;

					if (m_timeUnits == SiteData::MONTHS) {
						// convert daily -> monthly

						// New try: use average days in month.  This will cause the modified form to conform to the model.
						double days = 30.41667;
						gpmval /= days; // convert monthly -> daily
					}

					int day_date = m_timeUnits == SiteData::DAYS ? cnt : -1;
					int month_date = m_timeUnits == SiteData::MONTHS ? cnt : imonth;
					sd.m_pumpingRecTD.SetValueDate((ItemData::YearTypeEnum)m_yearMode, PumpingRecTD::Delta, 1, yearTokenCal, month_date, day_date);
					sd.m_pumpingRecTD.SetValueDate((ItemData::YearTypeEnum)m_yearMode, PumpingRecTD::Q, gpmval, yearTokenCal, month_date, day_date);
					sd.m_metaDataTD.SetValueDate((ItemData::YearTypeEnum)m_yearMode, MetaDataTD::MetaData, gpmval, yearTokenCal, month_date, day_date);

					cnt++;
				}
			}
		}
	}

	//m_yearMode = oriYearMode;

	return "";
}

string
SitesManager::ImportSDFView(string fname, bool addInPlace)
{
	ifstream istr(fname.c_str());
	if (!istr.is_open()) {
		return "Unable to open project file " + fname + "\n";
	}

	// Save basename so we know where to write results to.
	int loc = fname.rfind('.');
	m_baseName = fname.substr(0, loc);

	// Copied from SDFData.cpp:ReadInput

	// Read version number.
	istr.getline(buf, sizeof(buf));
	// Read year data.
	istr.getline(buf, sizeof(buf));

	int realInputStart(-1), realInputEnd(-1);
	int projYearStart(-1), projYearEnd(-1);
	int preprojYearStart(-1), preprojYearEnd(-1);
	int outputStart(-1), outputEnd(-1);
	int nwells=0, format=0, preprojection=4, projection=4, yearMode=1, unitMode=0, useAvgDaily=0;
	int nsynyears=0, npresynyears=0;
	int useIgnoreYear=0, ignoreYear=0;
	int sdfMode = 0;

	// Projected year data format has changed.  Now all years used in each
	//   calculation is saved.
	if (sscanf(buf, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&realInputStart, &realInputEnd,
		&npresynyears, &nsynyears,
		&outputStart, &outputEnd, &nwells, &format,
		&preprojection,
		&projection,
		&yearMode, &unitMode,
		&useAvgDaily,
		&useIgnoreYear, &ignoreYear, &sdfMode) != 16) {
			return "Wrong number of values read for first data card.  Expected realInputStart, realInputEnd, numPreSynthesizedYears, numSynthesizedYears, outputStart, outputEnd, wells, format, preprojection, projectionType, yearMode, unitMode, useAverageDaysPerMonth, useIgnoreYear, ignoreYear, sdfMode.  Line is: " + string(buf);
	}

	SetYearMode(YearModeEnum(yearMode));

	m_histStartYear = realInputStart;
	m_startYear = realInputStart - npresynyears;
	m_simStartYear = m_startYear;
	m_histEndYear = realInputEnd;
	m_endYear = realInputEnd + nsynyears;
	m_simEndYear = m_endYear;
	m_runIgnoreYear = m_endYear;

	int yr_add(0);
	if (m_yearMode != ItemData::CALENDAR) {
		// Add an extra year at the beginning in order to read irrigation or USGS year data.
		yr_add = 1;

		m_histStartYear--;
		m_startYear--;
		m_simStartYear--;
	}

	m_displayMode = DISPLAY_MODIFIED;
	SetTimeUnits(SiteData::MONTHS);
	SetPeriod(m_startYear, m_endYear);

	// Read number of years used in each projection calculation.
	int npreyears, npostyears;
	istr.getline(buf, sizeof(buf));

	if (sscanf(buf, "%d %d",
		&npreyears, &npostyears) != 2) {
			return "Wrong number of values read for second data card.  Expected npreyears, npostyears.  Line is: " + string(buf);
	}

	m_preForecastMode = (ItemData::ForecastEnum)preprojection;
	m_preYearList.resize(npreyears);
	int iprojyear;
	for (int iprojyear=0; iprojyear<npreyears; iprojyear++) {
		int year;
		istr >> year;
		m_preYearList[iprojyear] = year + m_histStartYear;
	}

	// If the forecast span is zero, then reset.
	if (m_startYear == m_histStartYear) {
		m_preForecastMode = ItemData::FORECAST_NONE;
		m_preYearList.resize(0);
		m_preSynthDataLabel = "";
	}
	else {
		m_preSynthDataLabel = CreateSynthLabel(m_preForecastMode, m_preYearList);
	}

	// Skip to next line.
	istr.getline(buf, sizeof(buf));

	m_forecastMode = (ItemData::ForecastEnum)projection;
	m_yearList.resize(npostyears);
	for (iprojyear=0; iprojyear<npostyears; iprojyear++) {
		int year;
		istr >> year;
		m_yearList[iprojyear] = year + m_histStartYear;
	}
	// If the forecast span is zero, then reset.
	if (m_endYear == m_histEndYear) {
		m_forecastMode = ItemData::FORECAST_NONE;
		m_yearList.resize(0);
		m_synthDataLabel = "";
	}
	else {
		m_synthDataLabel = CreateSynthLabel(m_forecastMode, m_yearList);
	}


	// Skip to next line.
	istr.getline(buf, sizeof(buf));

	// Skip multipliers
	istr.getline(buf, sizeof(buf));

	int nyears = m_endYear - m_startYear + 1;

	for (int isite=0; isite<nwells; isite++) {
		SiteData *sd = NULL;
		if (!addInPlace)
		{
			AddSite("", "");
			sd = &(m_siteList[isite]);
		}
		else
		{
			if (isite < m_siteList.size())
			{
				sd = &(m_siteList[isite]);
			}
			else
			{
				AddSite("", "");
				sd = &(m_siteList[isite]);
			}
		}

		sd->m_bi = SiteData::EFFECTIVE_SDF;

		istr.getline(buf, sizeof(buf));

		int pumpType, includeInOutput;
		float sdf;
		if (sscanf(buf, "%g %d %d",
			&sdf, &pumpType, &includeInOutput) != 3) {
				return "Wrong number of values read for second data card.  Expected sdf, pumpType, includeInOutput.  Line is: " + string(buf);
		}
		sd->m_sdf = sdf;

		if (pumpType == -1 /* SDF_SiteData::SDF_WITHDRAWAL */)
			sd->SetSiteType(SiteData::WELL);
		else
			sd->SetSiteType(SiteData::RECHARGE);

		istr.getline(buf, sizeof(buf));

		sd->m_header1 = trim(buf);
		sd->m_header2 = "Imported from " + fname;

		// Read data
		for (int year=m_startYear+yr_add; year<=m_endYear; year++) {
			istr.getline(buf, sizeof(buf));
			istringstream istrstr(buf);

			for (int m=0; m<12; m++) {
				double acft;
				istrstr >> acft;

				double gpmval = acft * 325851.0/1440;
				//double gpmval = acft * 325900.0/1440;

				if (m_timeUnits == SiteData::MONTHS) {
					// convert daily -> monthly
					//int days = getDaysInMonth(m, year, ItemData::YearTypeEnum(m_yearMode));
					// New try: use average days in month.  This will cause the modified form to conform to the model.
					double days = 30.41667;
					gpmval /= days; // convert monthly -> daily
				}

				try {
					SetValueDate(&sd->m_pumpingRecTD, PumpingRecTD::Delta, 1, year-m_startYear, m);
					SetValueDate(&sd->m_pumpingRecTD, PumpingRecTD::Q, gpmval, year-m_startYear, m);
					SetValueDate(&sd->m_metaDataTD, MetaDataTD::MetaData, MetaDataTD::USER_CALC, year-m_startYear, m);
				}
				catch (DataError err) {
				}
			}
		}
	}

	return "";
}

bool
SitesManager::ImportSite(string name, string descrip, double w, double b, double sdf, double sy, double trans, double x)
{
	// Search for a site with the name and update its values; otherwise add
	//   a new site.
	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		SiteData &sd = m_siteList[isite];

		bool found = false;

		if (trim(sd.m_header1) == trim(name)) {
			found = true;
		}
		else {
			// Try Altenhofen's format.
			vector<string> tokens;
			stringtok(tokens, sd.m_header1, ",");
			// First token is well id
			if (tokens.size() > 0) {
				try {
					int sd_id = int(lexical_cast<double, string>(tokens[0]));
					int name_id = int(lexical_cast<double, string>(name));

					if (sd_id > 0 && sd_id == name_id) {
						found = true;
					}
				}
				catch(std::exception const& ex)  {
					//std::cout << ex.what() << std::endl;
				}
			}
		}

		if (found) {
			// Bingo.
			if (!descrip.empty()) sd.m_header2 = descrip;
			if (w > 0) sd.m_w = w;
			if (b > 0) sd.m_b = b;
			if (sdf > 0) sd.m_sdf = sdf;
			if (sy > 0) sd.m_s = sy;
			if (trans > 0) sd.m_tr = trans;
			if (x > 0) sd.m_dxx = x;
			return true;
		}
	}

	// Not found, so add new site.
	int idx = AddSite(name, "imported from DB");
	SiteData &sd = m_siteList[idx];
	sd.m_w = w;
	sd.m_b = b;
	sd.m_sdf = sdf;
	sd.m_s = sy;
	sd.m_tr = trans;
	sd.m_dxx = x;

	// Try to guess mode.
	if (sd.m_sdf > -1) sd.m_bi = SiteData::EFFECTIVE_SDF;
	else if (sd.m_dxx > -1) sd.m_bi = SiteData::INFINITE_AQUIFER;
	else if (sd.m_w > -1) sd.m_bi = SiteData::ALLUVIAL_AQUIFER;
	else sd.m_bi = SiteData::NO_FLOW;

	return true;
}

string
SitesManager::ReadOutput(string fname)
{
	string outfname = fname;
	*(outfname.end()-1) = 'o';

	string retval;

	ifstream istr(outfname.c_str());
	if (!istr.is_open()) {
		retval += "Unable to open output file " + outfname + "\n";
		return retval;
	}

	int startMonth, startYear, endYear, ignoreMonth, ignoreYear;
	GetOutputPeriod(startYear, startMonth, endYear, ignoreYear, ignoreMonth);

	vector<SiteData>::iterator l;
	int npa(-1);
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		if (m_displayMode == DISPLAY_ORIGINAL) {
			// Reset span
			l->m_outputTD.SetSpan(0);
		}
		else {
			l->m_outputTD.SetPeriod(m_simStartYear, endYear);
			if (npa < 0) {
				// Adjust output size to fit simulation start and end year.
				npa = l->m_outputTD.GetNPeriods(OutputDataTD::Diversion);
			}
		}
		if (l->ShowInOutput(m_custOutput)) {
			l->ReadOutput(istr, npa);

			if (l->m_timeUnits == SiteData::MONTHS && m_displayMode == DISPLAY_MODIFIED) {
				// 	if (m_useAverageDaysInMonth) { 
				// 	  // Fix pumping so that output pumping matches input pumping.  This is necessary because the model generated flows using 30.4 days per month.
				// 	  int nyears = endYear - startYear + 1;
				// 	  for (int iyear=0; iyear<nyears; iyear++) {
				// 	    for (int m=0; m<12; m++) {
				// 	      double acft = l->m_outputTD.GetValue(OutputDataTD::Diversion, iyear, m);
				// 	      // Convert to average-daily flow.
				// 	      int ndays = getDaysInMonth(m, startYear + iyear);
				// 	      acft *= ndays / 30.41667;
				// 	      l->m_outputTD.SetValue(OutputDataTD::Diversion, iyear, m, acft);
				// 	    }
				// 	  }
				// 	}
			}
		}
	}

	// Test for end of file.  If there is more data, then we have a problem.
	string extra;
	istr >> extra;
	if (!extra.empty()) {
		AfxMessageBox("Warning: output contains extra data.");
	}

	// Don't bother creating summary output for original mode.
	if (m_displayMode == DISPLAY_MODIFIED) BuildSummaryData();

	return "";
}

void
SitesManager::ClearOutput()
{
	int startMonth, startYear, endYear, ignoreMonth, ignoreYear;
	GetOutputPeriod(startYear, startMonth, endYear, ignoreYear, ignoreMonth);

	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		if (m_displayMode == DISPLAY_ORIGINAL) {
			// Reset span
			l->m_outputTD.SetSpan(0);
		}
		else {
			l->m_outputTD.SetPeriod(startYear, endYear);
		}
	}
}

void
SitesManager::GetOutputPeriod(int &startYear, int &startMonth, int &endYear, int &ignoreYear, int &ignoreMonth)
{
	startMonth = m_simStartMonth;
	startYear = GetSimStartYear(); // adjust for year type.
	endYear = m_simEndYear;
	ignoreMonth = m_runIgnoreMonth;
	ignoreYear = m_runIgnoreYear;


	switch (m_yearMode) {
	  case YEAR_CALENDAR:
		  break;
	  case YEAR_IRRIGATION:
		  startMonth -= 2;
		  ignoreMonth -= 2;
		  break;
	  case YEAR_USGS:
		  startMonth -= 3;
		  ignoreMonth -= 3;
		  break;
	}

	if (m_yearMode != YEAR_CALENDAR) {
		if (startMonth < 0) {
			// Adjust start month.
			startMonth += 12;
			// When converting to calendar year, "1/2000" is really referring to 11/1999.

			// However, the sim start year is in the display format, so when converting to calendar year format, the sim start year is already "1999", so we need to refer to the previous year if the month is November or Dec.
			startYear--;
		}

		if (ignoreMonth < 0) {
			// Adjust month and year if necessary.  If the ignore month is negative, then this means that the previous calendar year is needed.
			ignoreMonth += 12;
			ignoreYear--;
		}
	}
}

string
SitesManager::Execute()
{
	string retval;

	ofstream ostr((m_baseName + ".dso").c_str());
	if (!ostr.is_open()) {
		retval += "Unable to open output save file " + m_baseName + ".dsi\n";
		return retval;
	}

	int ori_endYear = m_endYear;

	int startMonth, startYear, endYear, ignoreMonth, ignoreYear;
	GetOutputPeriod(startYear, startMonth, endYear, ignoreYear, ignoreMonth);

	// Adjust pumping data.
	if (m_displayMode == DISPLAY_MODIFIED) {
		SetPeriod(m_simStartYear, endYear); // use m_simStartYear because it is already adjusted for year type.
	}

	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end();) {
		// Remove site if this site is not to appear in output.
		if (!l->ShowInOutput(m_custOutput)) {
			l = m_siteList.erase(l);
		}
		else ++l;
	}

	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		// Error checks:
		if (l->m_bi == SiteData::ALLUVIAL_AQUIFER && l->m_w < l->m_dxx) {
			retval += "Error in " + l->m_header1 + ": distance from river to boundary is less than the distance from the well to the river.";
			return retval;
		}

		if (startMonth == 0) {
			if (startYear > m_startYear) {
				l->m_pumpingRecTD.Fill(PumpingRecTD::Q, 0, m_startYear, startYear-1);
			}
		}
		else {
			l->m_pumpingRecTD.FillMonth(PumpingRecTD::Q, 0, m_startYear, 0, startYear, startMonth-1);
		}

		if (m_useIgnoreYear) {
			l->m_pumpingRecTD.FillMonth(PumpingRecTD::Q, 0, ignoreYear, ignoreMonth+1, m_endYear, 11);
		}

		// Make sure extra years are zeroed out.
		if (ori_endYear < m_endYear) {
			l->m_pumpingRecTD.Fill(PumpingRecTD::Q, 0, ori_endYear+1, m_simEndYear);

		}
	}

	int npa_prime = 0;

	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		// Sometimes due to data limitations we may not be able to use the average daily mode,
		//   so this will override the user setting.
		bool useAverageDaysInMonth = m_useAverageDaysInMonth;

		if (m_displayMode == DISPLAY_MODIFIED)  {
			l->m_outputTD.SetPeriod(m_simStartYear, endYear);

			if (m_timeUnits == SiteData::MONTHS) {
				if (l->m_bi == SiteData::URF) {
					if (l->m_URF_DailyData.size() == 0 && l->m_URF_MonthlyData.size() > 0) {
						// Site contains monthly URF values only, so this site must run in monthly mode.
						useAverageDaysInMonth = true;
					}
					if (l->m_URF_MonthlyData.size() == 0 && l->m_URF_DailyData.size() > 0) {
						// Site contains daily URF values only, so this site must run in daily mode.
						useAverageDaysInMonth = false;
					}
				}

				if (!useAverageDaysInMonth) {
					// Calculation will be run in daily mode unless URFs are monthly.
					// Convert to daily mode
					l->SetTimeUnits(SiteData::DAYS);
					AdjustPumping(*l, SiteData::DAYS);
				}
			}
			else {
				// Daily timestep
				if (l->m_bi == SiteData::URF) {
					if (l->m_URF_DailyData.size() == 0 && l->m_URF_MonthlyData.size() > 0) {
						// Site contains monthly URF values only, so this site must run in monthly mode.
						l->SetTimeUnits(SiteData::MONTHS);
					}
				}
			}
		}

		// Assign URF source.
		if (l->m_timeUnits == SiteData::DAYS)
		{
			l->m_URF_DataCalc = l->m_URF_DailyData;
		}
		else 
		{
			l->m_URF_DataCalc = l->m_URF_MonthlyData;
		}

		cout << "Processing " << l->m_header1 << "...";

		time_t start_t;
		time(&start_t);
		int ncycles = l->Calculate(useAverageDaysInMonth, m_useNewErrorFunc ? &SiteData::CalcErrorFuncNew : &SiteData::CalcErrorFunc,
			m_displayMode);
		time_t end_t;
		time(&end_t);
		cout << " done in " << difftime(end_t, start_t) << " seconds" << endl;

		// Check if we need to adjust the output records
		if (m_displayMode == DISPLAY_MODIFIED && l->m_timeUnits != m_timeUnits)
		{
			// Fix the output rec array to the new units.
			if (m_timeUnits == SiteData::MONTHS)
			{
				// Changing daily to monthly.
				vector<OutputRec> newOutput;
				vector<double> new_q;
				vector<double> new_delt;
				vector<double> new_zzat;
				vector<double> new_vptp;
				vector<double> new_avp;

				int nyears = endYear - m_simStartYear + 1;
				int irec=-1;
				for (int iyear=0; iyear<nyears; iyear++) {
					for (int imonth=0; imonth<12; imonth++) {
						double ndays = getDaysInMonth(imonth, iyear + m_simStartYear);
						OutputRec or;
						or.m_time = iyear*12 + imonth + 1;

						double q = 0;
						double delt = 0;
						double zzat = 0;
						double vptp = 0;
						double avp = 0;

						for (int iday=0; iday<ndays; iday++) {
							irec++;
							or.m_depRate += l->m_outputRecArray[irec].m_depRate;
							or.m_depVol += l->m_outputRecArray[irec].m_depVol;
							or.m_depVolThis += l->m_outputRecArray[irec].m_depVolThis;

							q += l->m_q[irec];
							delt = l->m_delt[irec];
							zzat += l->m_zzat[irec];
							vptp += l->m_vptp[irec];
							avp += l->m_avp[irec];
						}
						newOutput.push_back(or);
						new_q.push_back(q);
						new_delt.push_back(delt);
						new_zzat.push_back(zzat);
						new_vptp.push_back(vptp);
						new_avp.push_back(avp);
					}
				}
				l->m_outputRecArray = newOutput;
				l->m_q = new_q;
				l->m_delt = new_delt;
				l->m_zzat = new_zzat;
				l->m_vptp = new_vptp;
				l->m_avp = new_avp;
			}
			else
			{
				// Changing monthly to average daily.
				vector<OutputRec> newOutput;
				vector<double> new_q;
				vector<double> new_delt;
				vector<double> new_zzat;
				vector<double> new_vptp;
				vector<double> new_avp;

				int nyears = endYear - m_simStartYear + 1;
				int irec=0;
				for (int iyear=0; iyear<nyears; iyear++) {
					for (int imonth=0; imonth<12; imonth++) {
						double ndays = getDaysInMonth(imonth, iyear + m_simStartYear);
						for (int iday=0; iday<ndays; iday++) {
							OutputRec or;

							// Convert everything to average daily.
							or.m_time = ++irec;
							or.m_depRate = l->m_outputRecArray[iyear*12 + imonth].m_depRate / ndays;
							or.m_depVol = l->m_outputRecArray[iyear*12 + imonth].m_depVol / ndays;
							or.m_depVolThis = l->m_outputRecArray[iyear*12 + imonth].m_depVolThis / ndays;

							newOutput.push_back(or);

							new_q.push_back(l->m_q[iyear*12 + imonth] / ndays);
							new_delt.push_back(1);
							new_zzat.push_back(l->m_zzat[iyear*12 + imonth] / ndays);
							new_vptp.push_back(l->m_vptp[iyear*12 + imonth] / ndays);
							new_avp.push_back(l->m_avp[iyear*12 + imonth] / ndays);
						}
					}
				}
				l->m_outputRecArray = newOutput;
				l->m_q = new_q;
				l->m_delt = new_delt;
				l->m_zzat = new_zzat;
				l->m_vptp = new_vptp;
				l->m_avp = new_avp;
			}

			// Convert to project time units.
			l->SetTimeUnits(m_timeUnits);
		}

		l->WriteOutput(ostr);
	}

	return "";
}


string
SitesManager::WriteProject(string fname, bool outputSettingsOnly)
{
	string retval;

	ofstream ostr(fname.c_str());
	if (!ostr.is_open()) {
		cerr << "Unable to open output save file " + fname + "\n";
		return retval;
	}

	// Save basename so we know where to write results to.
	int loc = fname.rfind('.');
	m_baseName = fname.substr(0, loc);

	// First line is version
	ostr << "version=" << m_currentVersion << endl;

	// Next two lines are the period of record.
	ostr << "start year=" << m_startYear << endl;
	ostr << "historical start year=" << m_histStartYear << endl;
	ostr << "historical end year=" << m_histEndYear << endl;
	ostr << "end year=" << m_endYear << endl;
	ostr << "synth label=" << m_synthDataLabel << endl;
	ostr << "presynth label=" << m_preSynthDataLabel << endl;
	ostr << "synth mode=" << int(m_forecastMode) << endl;
	ostr << "presynth mode=" << int(m_preForecastMode) << endl;
	ostr << "number of synth years=" << m_yearList.size() << endl;
	for (unsigned int i=0; i<m_yearList.size(); i++) {
		ostr << m_yearList[i] << " ";
	}
	ostr << endl;
	ostr << "number of presynth years=" << m_preYearList.size() << endl;
	int i;
	for (i=0; i<m_preYearList.size(); i++) {
		ostr << m_preYearList[i] << " ";
	}
	ostr << endl;

	ostr << "view year mode=" << (int)m_yearMode << endl;
	ostr << "display mode=" << (int)m_displayMode << endl;
	ostr << "time units=" << (int)m_timeUnits << endl;
	ostr << "use average days per month=" << (int)m_useAverageDaysInMonth << endl;
	ostr << "URF uses monthly values for daily calculation=" << (int)m_useMonthlyURFForDaily << endl;
	ostr << "use new error function=" << (int)m_useNewErrorFunc << endl;
	ostr << "run start=" << m_simStartYear << " " << m_simStartMonth << endl;
	ostr << "run end=" << m_simEndYear << endl;
	ostr << "run ignore year=" << m_runIgnoreYear << " " << m_runIgnoreMonth << endl;
	ostr << "use run ignore year=" << (m_useIgnoreYear ? 1 : 0) << endl;
	ostr << "projected data multipliers=";
	for (int m=0; m<12; m++) {
		ostr << m_pMult[m] << " ";
	}
	ostr << endl;
	ostr << "copy includes header=" << m_copyIncludesHeader << endl;

	// Next line is number of sites.
	ostr << "number of sites=" << m_siteList.size() << endl;

	// Write each site.
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		ostr << "header1=" << l->m_header1 << endl;
		ostr << "header2=" << l->m_header2 << endl;

		ostr << "pumping type=" << (l->m_siteType == SiteData::WELL ? "irrigation" : "recharge") << endl;
		ostr << "boundary condition=" << l->m_bi << endl;
		ostr << "compute depletion for segment=" << (l->m_computeDepletionForSegment ? 1 : 0) << endl;
		ostr << "transmissivity (tr)=" << l->m_tr << endl;
		ostr << "specific yield (s)=" << l->m_s << endl;
		ostr << "distance from well to stream (dxx)=" << l->m_dxx << endl;

		ostr << "time units=" << l->m_timeUnits << endl;
		ostr << "number of time units between printouts (tbp)=" << l->m_tbp << endl;
		ostr << "number of cycles (seasons) to be simulated (nc)=" << l->m_nc << endl;

		ostr << "effective sdf=" << l->m_sdf << endl;
		ostr << "distance from the stream to the parallel impermeable boundary (w)=" << l->m_w << endl;
		ostr << "distance between well and no flow boundary (b)=" << l->m_b << endl;
		ostr << "calculate depletion for segment of stream (zzseg)=" << (l->m_zzseg ? 1 : 0) << endl;
		ostr << "length of stream (z1, z2)=" << l->m_z1 << " " << l->m_z2 << endl;

		int nrecs = l->m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q);
		ostr << "number of pumping records=" << nrecs << endl;
		for (int iyear=0; iyear<l->m_pumpingRecTD.GetNYears(); iyear++) {
			// This nrecs is the number of periods in the current year.
			nrecs = l->m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q, iyear);

			ostr.precision(20);
			for (int irec=0; irec<nrecs; irec++) {
				ostr << "delt,q,meta=" << l->m_pumpingRecTD.GetValue(PumpingRecTD::Delta, iyear, irec)
					<< " " << l->m_pumpingRecTD.GetValue(PumpingRecTD::Q, iyear, irec)
					<< " " << (int)l->m_metaDataTD.GetValue(MetaDataTD::MetaData, iyear, irec) << endl;
			}
		}

		if (l->m_siteType == SiteData::WELL) {
			ostr << l->m_wellPumpingTD;
		}
		else {
			ostr << l->m_rechargeTD;
		}

		// Write output column values.
		ostr << "Number of output values=" << l->m_outputCols.size() << endl;
		for (int i=0; i<l->m_outputCols.size(); i++) {
			ostr << "Output value=" << (l->m_outputCols[i] ? "yes" : "no") << endl;
		}
	}

	m_custOutput.Write(ostr);

	WriteURF_Data(fname);

	return "";
}

bool
SitesManager::WriteURF_Data(string fname)
{
	*(fname.end() -1) = 'u'; // Change filename to ".dsu"

	ofstream ostr(fname.c_str());
	if (!ostr.is_open()) {
		cerr << "Unable to open URF file " + fname + "\n";
		return false;
	}

	// Each site gets a row.
	for (int isite=0; isite<m_siteList.size(); isite++) {
		ostr << m_siteList[isite].m_header1 << endl;
		ostr << m_siteList[isite].m_URF_MonthlyData.size();
		for (int icol=0; icol<m_siteList[isite].m_URF_MonthlyData.size(); icol++) {
			ostr << " " << m_siteList[isite].m_URF_MonthlyData[icol];
		}
		ostr << endl;

		// Daily values
		ostr << m_siteList[isite].m_URF_DailyData.size();
		for (int icol=0; icol<m_siteList[isite].m_URF_DailyData.size(); icol++) {
			ostr << " " << m_siteList[isite].m_URF_DailyData[icol];
		}
		ostr << endl;
	}

	return true;
}

bool
SitesManager::ReadURF_Data(string fname, float version)
{
	*(fname.end() -1) = 'u'; // Change filename to ".dsu"

	ifstream istr(fname.c_str());
	if (!istr.is_open()) {
		cerr << "Unable to open URF file " + fname + "\n";
		return false;
	}

	// Each site gets a row.
	for (int isite=0; isite<m_siteList.size(); isite++) {
		// Read site name
		istr.getline(buf, sizeof(buf));
		if (string(buf) != m_siteList[isite].m_header1) {
			return false;
		}

		// If this is an older version of the input, then decide whether this is monthly or daily.
		if (version >= 2.8f || m_timeUnits == SiteData::MONTHS)
		{
			int nurf_monthly;
			istr >> nurf_monthly;
			m_siteList[isite].m_URF_MonthlyData.resize(nurf_monthly);
			for (int icol=0; icol<m_siteList[isite].m_URF_MonthlyData.size(); icol++) {
				istr >> m_siteList[isite].m_URF_MonthlyData[icol];
			}

			// Skip to next line
			istr.getline(buf, sizeof(buf));
		}

		if (version >= 2.8f || m_timeUnits == SiteData::DAYS)
		{
			int nurf_daily;
			istr >> nurf_daily;
			m_siteList[isite].m_URF_DailyData.resize(nurf_daily);
			for (int icol=0; icol<m_siteList[isite].m_URF_DailyData.size(); icol++) {
				istr >> m_siteList[isite].m_URF_DailyData[icol];
			}

			// Skip to next line
			istr.getline(buf, sizeof(buf));
		}
	}

	return true;
}

void 
SitesManager::CheckURFs(BOOL& hasDailyURFSites, BOOL &hasMonthlyURFSites)
{
	hasDailyURFSites = FALSE;
	hasMonthlyURFSites = FALSE;

	for (int isite=0; isite<m_siteList.size() && (!hasDailyURFSites || !hasMonthlyURFSites); isite++) {
		if (m_siteList[isite].m_URF_DailyData.size() > 0)
		{
			hasDailyURFSites = TRUE;
		}
		if (m_siteList[isite].m_URF_MonthlyData.size() > 0)
		{
			hasMonthlyURFSites = TRUE;
		}
	}
}

void
SitesManager::FixOutOfBoundsData()
{
	// Search for a site with the name.
	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		m_siteList[isite].CleanData((ItemData::YearTypeEnum)m_yearMode);
	}
}

int
SitesManager::FindSite(string name)
{
	// Search for a site with the name.
	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		SiteData &sd = m_siteList[isite];

		if (trim(sd.m_header1) == trim(name)) {
			return isite;
		}
	}

	// Exact match failed, try partial

	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		SiteData &sd = m_siteList[isite];
		// Search for group.
		vector<string> tokens;
		stringtok(tokens, sd.m_header1, ",");
		if (tokens.size() > 1) {
			// First token is well id
			int sd_id = int(lexical_cast<double, string>(tokens[0]));
			int name_id = int(lexical_cast<double, string>(name));

			// Enforce that name has to be a numerical value
			if (tokens[0] == name || (name == to_string<int>(name_id) && sd_id == name_id)) {
				return isite;
			}
		}

		// Disabled per Jon Altenhofen 4/2/o09
		//if (sd.m_header1.find(name) != string::npos)
		//{
		//  return isite;
		//}
	}

	return -1;
}

int
SitesManager::AddSite(string h1, string h2)
{
	SiteData sd;
	sd.m_header1 = h1;
	sd.m_header2 = h2;
	sd.SetTimeUnits(GetTimeUnits());
	if (m_displayMode == DISPLAY_MODIFIED) {
		sd.SetPeriod(m_startYear, m_endYear);
	}

	sd.m_outputCols.resize(m_custOutput.GetSize());
	if (m_custOutput.GetSize() > 0)
	{
		sd.m_outputCols[0] = true;
		for (int icol=1; icol<m_custOutput.GetSize(); icol++) {
			sd.m_outputCols[icol] = false;
		}
	}

	m_siteList.push_back(sd);

	return m_siteList.size()-1;
}

void
SitesManager::DelSite(int idx)
{
	m_siteList.erase(m_siteList.begin() + idx);
}

bool
SitesManager::HasSitesWithURF()
{
	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		SiteData &sd = m_siteList[isite];

		if (sd.m_bi == SiteData::URF) {
			return TRUE;
		}
	}

	return FALSE;
}



SiteData::TimeUnitsEnum
SitesManager::GetTimeUnits()
{
	return m_timeUnits;
}

void
SitesManager::InitCustOutputData()
{
	ItemData::ItemAggEnum cust_aggs[] = {ItemData::TOTAL, ItemData::TOTAL};
	if (m_timeUnits == SiteData::MONTHS) {
		ItemData::TimeEnum cust_times[] = {ItemData::MONTHLY, ItemData::MONTHLY};

		for (int icust=0; icust<m_summaryCustomTD.size(); icust++) {
			m_summaryCustomTD[icust].Initialize(cust_times, cust_aggs);
		}
	}
	else if (m_timeUnits == SiteData::DAYS) {
		ItemData::TimeEnum cust_times[] = {ItemData::DAILY, ItemData::DAILY};

		for (int icust=0; icust<m_summaryCustomTD.size(); icust++) {
			m_summaryCustomTD[icust].Initialize(cust_times, cust_aggs);
		}
	}

	int nyears = m_simEndYear - m_simStartYear + 1;

	for (int icust=0; icust<m_summaryCustomTD.size(); icust++) {
		m_summaryCustomTD[icust].SetNYears(nyears, m_simStartYear);
	}
}

void
SitesManager::SetTimeUnits(SiteData::TimeUnitsEnum newUnits)
{
	int nyears = m_endYear - m_startYear + 1;

	if (newUnits != m_timeUnits)
	{
		// Convert all sites to use new time units.  Useful only for modified
		//   interface mode.

		// Adjust run years.
		if (m_simStartYear == 0) {
			m_simStartYear = m_startYear;
		}
		if (m_simEndYear == 0) {
			m_simEndYear = m_endYear;
		}
		if (m_runIgnoreYear == 0) m_runIgnoreYear = m_endYear;

		vector<SiteData>::iterator l = m_siteList.begin();
		for (; l != m_siteList.end(); ++l) {
			l->SetTimeUnits(newUnits);
		}

		AdjustPumping(newUnits);
	}

	if (m_simStartYear > 0) {
		ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY};

			m_summaryTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY};

			m_summaryTD.Initialize(times, aggs);      
		}

		nyears = m_simEndYear - m_simStartYear + 1;
		m_summaryTD.SetNYears(nyears, m_simStartYear);
	}

	m_timeUnits = newUnits;

	InitCustOutputData();
}

void
SitesManager::AdjustPumping(SiteData &sd, SiteData::TimeUnitsEnum newUnits)
{
	int startYear = sd.m_pumpingRecTD[PumpingRecTD::Q].m_startyear;
	int nyears = sd.m_pumpingRecTD.GetNYears();
	for (int iyear=0; iyear<nyears; iyear++) {
		for (int imonth=0; imonth<12; imonth++) {
			double ndays = getDaysInMonth(imonth, iyear + startYear);
			double val = sd.m_pumpingRecTD.GetValueDate(ItemData::CALENDAR, ItemData::MONTHLY, PumpingRecTD::Q, iyear, imonth);
			if (val > NoData)
			{
				if (newUnits == SiteData::DAYS) {
					// If the conversion is from monthly to daily, then we need to apply
					//   a conversion from actual days in the month to average days.
					val *= 30.41667 / ndays;
				}
				else if (newUnits == SiteData::MONTHS) {
					// If the conversion is from daily to monthly, then we need to apply
					//   a conversion from average days in the month to actual days.
					val *= ndays / 30.41667;
				}
			}
			sd.m_pumpingRecTD.SetValueDate(ItemData::CALENDAR, PumpingRecTD::Q, val, iyear, imonth);
		}
	}
}

void
SitesManager::AdjustPumping(SiteData::TimeUnitsEnum newUnits)
{
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		AdjustPumping(*l, newUnits);
	}
}

bool
SitesManager::SetPeriod(int startYear, int endYear, int newSitePos, int forecast_mode, vector<int> *yearList, int prec)
{
	// Do nothing if period of record is incomplete.
	if (startYear != 0 && endYear != 0) {
		ItemData::ForecastEnum fe = ItemData::ForecastEnum(forecast_mode);

		// This determines when the data should start.  For example, if the data is in irrigation year, then we want to keep the first 9 months of the year as no data.
		int startMonth(0);
		switch (m_yearMode) {
		case YEAR_CALENDAR:
			break;
		case YEAR_IRRIGATION:
			startMonth += 10;
			break;
		case YEAR_USGS:
			startMonth += 9;
			break;
		}

		int sitePos = newSitePos;
		if (sitePos < 0) sitePos = 0;

		vector<SiteData>::iterator l = m_siteList.begin() + sitePos;
		while (l != m_siteList.end()) {
			// Convert flows to volume so that the projections don't get screwed up from leap year issues.
			l->m_pumpingRecTD.ConvertToVolume();
			l->m_pumpingRecTD.SetPeriod(ItemData::YearTypeEnum(m_yearMode), startYear, endYear, fe, yearList, m_pMult, prec);
			// non-calendar years need to have the months padded at the beginning set to nodata.
			if (m_yearMode != YEAR_CALENDAR) {
				l->m_pumpingRecTD.FillMonth(PumpingRecTD::Q, NoData, 0, 0, 0, startMonth-1, false);
			}

			// Replace nodatas with zeroes.
			l->m_pumpingRecTD.FillMonth(PumpingRecTD::Q, 0, 0, 0, -1, 11, true);
			// Force all deltas to be one.
			l->m_pumpingRecTD.Fill(PumpingRecTD::Delta, 1, -1, -1, false);
			//l->m_pumpingRecTD.FillMonth(PumpingRecTD::Delta, 1, 0, startMonth, -1, 11, true);

			l->m_pumpingRecTD.ConvertToFlow();

			l->m_metaDataTD.SetPeriod(ItemData::YearTypeEnum(m_yearMode), startYear, endYear, fe, yearList, m_pMult, prec);
			if (l->m_siteType == SiteData::WELL) {
				l->m_wellPumpingTD.SetPeriod(ItemData::YearTypeEnum(m_yearMode), startYear, endYear, fe, yearList, m_pMult, prec);
				for (int irec=0; irec<WellPumpingTD::NumWellPumpingTD; irec++) {
					l->m_wellPumpingTD.Fill(irec, 0, -1, -1, true);
				}
			}
			else {
				l->m_rechargeTD.SetPeriod(ItemData::YearTypeEnum(m_yearMode), startYear, endYear, fe, yearList, m_pMult, prec);
				for (int irec=0; irec<RechargeTD::NumRechargeTD; irec++) {
					l->m_rechargeTD.Fill(irec, 0, -1, -1, true);
				}
			}

			// Check if we are updating a single site only
			if (newSitePos >= 0) l = m_siteList.end();
			else ++l;
		}
	}

	//   if (forecast_mode == ItemData::FORECAST_NONE && (m_startYear != startYear || m_endYear != endYear)) {
	m_startYear = startYear;
	//if (m_simStartYear < m_startYear) m_simStartYear = m_startYear;
	m_simStartYear = m_startYear;
	m_endYear = endYear;
	//if (m_simEndYear < m_endYear) m_simEndYear = m_endYear;
	m_simEndYear = m_endYear;

	return TRUE;
	//   }
	//   return FALSE;
}

void
SitesManager::SetOutputUnits(SiteData::volumeUnitsEnum units)
{
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		l->SetOutputUnits(units);
	}
}


bool
SitesManager::AddRecords(int nrecs)
{
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		// use -1 for appending
		l->AddRecords(-1, nrecs);
	}

	return TRUE;
}

void
SitesManager::SetYearMode(SitesManager::YearModeEnum currentYearMode)
{
	m_yearMode = currentYearMode;

	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		// use -1 for appending
		l->CleanData((ItemData::YearTypeEnum)m_yearMode);
	}
}

int
SitesManager::GetHistStartYear()
{
	if (m_yearMode != YEAR_CALENDAR) {
		// Non-calendar years need a fake year at the front to allow for Oct-Dec.
		return m_histStartYear+1;
	}
	return m_histStartYear;
}

int
SitesManager::GetHistEndYear()
{
	// End year never needs adjustment.
	return m_histEndYear;
}

int
SitesManager::GetStartYear()
{
	if (m_yearMode != YEAR_CALENDAR) {
		// Non-calendar years need a fake year at the front to allow for Oct-Dec.
		return m_startYear+1;
	}
	return m_startYear;
}

int
SitesManager::GetEndYear()
{
	// End year never needs adjustment.
	return m_endYear;
}

int
SitesManager::GetSimStartYear()
{
	if (m_yearMode != YEAR_CALENDAR) {
		// Non-calendar years need a fake year at the front to allow for Oct-Dec.
		return m_simStartYear+1;
	}
	return m_simStartYear;
}

int
SitesManager::GetSimEndYear()
{
	return m_simEndYear;
}

int
SitesManager::GetSimStartMonth()
{
	// Get the simulation start month.
	return m_simStartMonth;
}

int
SitesManager::GetRunIgnoreMonth()
{
	// Get the ignore month.
	return m_runIgnoreMonth;
}

void
SitesManager::SetSimStartMonth(int simStartMonth)
{
	// Set the simulation start month.  We don't care what the year type is because this is just an offset from the beginning month (Jan/Nov/Oct).
	m_simStartMonth = simStartMonth;
}

void
SitesManager::SetIgnoreMonth(int ignoreMonth)
{
	// Set the simulation end month.
	m_runIgnoreMonth = ignoreMonth;
}

string
SitesManager::GetMonthName(int m)
{
	int monthOffset = 0;
	switch (m_yearMode) {
  case SitesManager::YEAR_IRRIGATION:
	  // The first month (Jan) is index 2.
	  monthOffset = 10;
	  break;
  case SitesManager::YEAR_USGS:
	  // The first month (Oct) is index 11.
	  monthOffset = 9;
	  break;
	}

	char *mnames[] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	return mnames[(m+monthOffset)%12];
}

int
SitesManager::GetMonthIdx(int m)
{
	// Convert calendar to actual year.
	switch (m_yearMode) {
  case ItemData::CALENDAR:
	  return m;
  case ItemData::IRRIGATION:
	  return (m+10)%12;
  case ItemData::USGS:
	  return (m+9)%12;
	}
	return 0;
}

int
SitesManager::GetYear(int m, int y)
{
	return getYearFromCalYear(m, y, (ItemData::YearTypeEnum)m_yearMode);
}

void
SitesManager::BuildSummaryData()
{
	// Check for no data.
	if (m_siteList.begin() == m_siteList.end()) return;

	if (m_displayMode == DISPLAY_MODIFIED) {
		int startMonth, startYear, endYear, ignoreMonth, ignoreYear;
		GetOutputPeriod(startYear, startMonth, endYear, ignoreYear, ignoreMonth);

		int nyears = endYear - m_simStartYear + 1;

		m_summaryTD.SetNYears(nyears, m_simStartYear);

		int npa = -1;
		vector<SiteData>::iterator l;

		m_summaryTD.Fill(SummaryOutputDataTD::Diversion, 0);
		m_summaryTD.Fill(SummaryOutputDataTD::Depletion, 0);
		m_summaryTD.Fill(SummaryOutputDataTD::Recharge, 0);
		m_summaryTD.Fill(SummaryOutputDataTD::Accretion, 0);
		m_summaryTD.Fill(SummaryOutputDataTD::Net, 0);

		// Include custom output totals.
		int ncustomout = m_custOutput.GetActiveCols();
		m_summaryCustomTD.resize(ncustomout);

		InitCustOutputData();

		for (int icust=0; icust<ncustomout; icust++) {
			m_summaryCustomTD[icust].SetNYears(nyears, m_simStartYear);
			m_summaryCustomTD[icust].Fill(SummaryCustomOutputDataTD::Diversion, 0);
			m_summaryCustomTD[icust].Fill(SummaryCustomOutputDataTD::Depletion, 0);
		}

		for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
			if (l->ShowInOutput(m_custOutput)) {
				int nppc = l->m_vptp.size();

				npa = l->m_outputRecArray.size();

				int inpa(-1);
				for (int iyear=0; iyear<nyears; iyear++) {
					int ndays = m_summaryTD.GetNPeriods(SummaryOutputDataTD::Diversion, iyear);
					for (int iday=0; iday<ndays; iday++) {
						inpa++;

						if (l->m_siteType == SiteData::WELL) {
							double acft = l->m_outputTD.GetValue(OutputDataTD::Diversion, iyear, iday);

							m_summaryTD.AddValue(SummaryOutputDataTD::Diversion, iyear, iday, -acft);
							m_summaryTD.AddValue(SummaryOutputDataTD::Depletion, iyear, iday, -l->m_outputRecArray[inpa].m_depVolThis);
						}
						else if (l->m_siteType == SiteData::RECHARGE) {
							double acft = l->m_outputTD.GetValue(OutputDataTD::Diversion, iyear, iday);
							m_summaryTD.AddValue(SummaryOutputDataTD::Recharge, iyear, iday, acft);
							// But accretion is always output as negative.
							m_summaryTD.AddValue(SummaryOutputDataTD::Accretion, iyear, iday, -l->m_outputRecArray[inpa].m_depVolThis);
						}
						m_summaryTD.AddValue(SummaryOutputDataTD::Net, iyear, iday, -l->m_outputRecArray[inpa].m_depVolThis);
					}
				}
			}

			// Repeat for custom output.
			int nextrow = -1;
			for (int icust=0; icust<ncustomout; icust++) {
				nextrow = m_custOutput.GetNextRow(nextrow);
				if (l->m_outputCols[nextrow]) {
					int nppc = l->m_vptp.size();

					npa = l->m_outputRecArray.size();

					int inpa(-1);
					for (int iyear=0; iyear<nyears; iyear++) {
						int ndays = m_summaryCustomTD[icust].GetNPeriods(SummaryCustomOutputDataTD::Diversion, iyear);
						for (int iday=0; iday<ndays; iday++) {
							inpa++;

							double acft = l->m_outputTD.GetValue(OutputDataTD::Diversion, iyear, iday);
							double depVol = -l->m_outputRecArray[inpa].m_depVolThis;
							if (l->m_siteType == SiteData::RECHARGE) {
								acft = -acft;
							}
							m_summaryCustomTD[icust].AddValue(SummaryCustomOutputDataTD::Diversion, iyear, iday, acft);
							m_summaryCustomTD[icust].AddValue(SummaryCustomOutputDataTD::Depletion, iyear, iday, depVol);
						}
					}
				}
			}
		}
	}
}

double
SitesManager::GetOutputData(int whichSite, int timeMode, int whichType, int prec, int year, int month, int day)
{
	double retval = NoData;

	vector<SiteData>::iterator l = m_siteList.begin() + GetOutputSiteIdx(whichSite);
	if (l != m_siteList.end()) {
		if (whichType == 0) {
			// Diversion data
			retval = GetValueDate(&l->m_outputTD, OutputDataTD::Diversion, year, month, day);
		}
		else {
			retval = GetValueDate(&l->m_outputTD, OutputDataTD::Depletion, year, month, day);
		}

		// Make pumping negative
		if (whichType == 0) {
			if (retval != NoData) {
				retval *= (l->m_siteType == SiteData::RECHARGE ? 1 : -1);
			}
		}
		else {
			// Show accretions as positive numbers.
			if (retval != NoData) {
				retval = -retval;
			}
		}
	}

	if (prec >= 0) return Round(retval, prec);
	else return retval;
}

double
SitesManager::GetCustomOutputData(int whichSite, int timeMode, int whichType, int prec, int year, int month, int day)
{
	double retval = NoData;

	if (m_summaryCustomTD.size() > 0)  {
		if (whichType == 0) {
			// Diversion data.  Always assumed to be depletion
			retval = -GetValueDate(&m_summaryCustomTD[whichSite], SummaryCustomOutputDataTD::Diversion, year, month, day);
		}
		else {
			retval = GetValueDate(&m_summaryCustomTD[whichSite], SummaryCustomOutputDataTD::Depletion, year, month, day);
		}
	}

	if (prec >= 0) return Round(retval, prec);
	else return retval;
}

SiteData&
SitesManager::GetOutputSite(int whichSite)
{
	return m_siteList[GetOutputSiteIdx(whichSite)];
}

int
SitesManager::GetOutputSiteIdx(int whichSite)
{
	int cnt(-1);
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		if (l->ShowInOutput(m_custOutput)) {
			cnt++;
			if (cnt == whichSite) return distance(m_siteList.begin(), l);
		}
	}

	return -1;
}

int
SitesManager::GetOutputSiteCount()
{
	int cnt(0);
	vector<SiteData>::iterator l;
	for (l = m_siteList.begin(); l != m_siteList.end(); ++l) {
		if (l->ShowInOutput(m_custOutput)) {
			cnt++;
		}
	}

	return cnt;
}

// whichType: 0 == diversions, 1 == depletions
double
SitesManager::GetSummaryOutputData(int whichSummary, int timeMode, int whichType, int prec, int year, int month, int day)
{
	double retval = NoData;

	SummaryOutputDataTD::SummaryOutputDataEnum dt;

	switch (whichSummary) {
  case 0:
	  dt = whichType == 0 ? SummaryOutputDataTD::Total_Sel_div : SummaryOutputDataTD::Total_Sel_dep;
	  break;
  case 1:			// recharge
	  dt = whichType == 0 ? SummaryOutputDataTD::Recharge : SummaryOutputDataTD::Accretion;
	  break;
  case 2:			// irrigation
	  dt = whichType == 0 ? SummaryOutputDataTD::Diversion : SummaryOutputDataTD::Depletion;
	  break;
  case 3:			// net
	  if (whichType == 0) {
		  // Sum the total recharge and consumptive use
		  float val = 0;
		  if (prec >= 0) val = Round(GetValueDate(&m_summaryTD, SummaryOutputDataTD::Recharge, year, month, day), prec);
		  else val = GetValueDate(&m_summaryTD, SummaryOutputDataTD::Recharge, year, month, day);

		  if (prec >= 0) val += Round(GetValueDate(&m_summaryTD, SummaryOutputDataTD::Diversion, year, month, day), prec);
		  else val += GetValueDate(&m_summaryTD, SummaryOutputDataTD::Diversion, year, month, day);

		  return val;
	  }
	  else if (whichType == 1) {
		  dt = SummaryOutputDataTD::Net;
	  }
	  else return 0;
	  break;
	}

	if (prec >= 0) return Round(GetValueDate(&m_summaryTD, dt, year, month, day), prec);
	else return GetValueDate(&m_summaryTD, dt, year, month, day);
}

void
SitesManager::SetSummaryTimeMode(int mode)
{
	ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL, ItemData::TOTAL};
	ItemData::ItemAggEnum cust_aggs[] = {ItemData::TOTAL, ItemData::TOTAL};
	if (m_timeUnits == SiteData::MONTHS) {
		if (mode == 0) { // monthly
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY};

			m_summaryTD.Initialize(times, aggs);
		}
	}
	else if (m_timeUnits == SiteData::DAYS) {
		if (mode == 0) { // monthly
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY};

			m_summaryTD.Initialize(times, aggs);
		}
		else if (mode == 1) { // daily
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY, ItemData::DAILY};

			m_summaryTD.Initialize(times, aggs);
		}
	}

	int startMonth, startYear, endYear, ignoreMonth, ignoreYear;
	GetOutputPeriod(startYear, startMonth, endYear, ignoreYear, ignoreMonth);

	m_summaryTD.SetNYears(endYear - m_simStartYear + 1, m_simStartYear);

	InitCustOutputData();
}

double
SitesManager::GetValueDate(TimeData *td, ItemData::TimeEnum timemode, int whichitem, int year, int month, int day)
{
	return td->GetValueDate(ItemData::YearTypeEnum(m_yearMode), timemode, whichitem, year, month, day);
}

double
SitesManager::GetValueDate(TimeData *td, int whichitem, int year, int month, int day)
{
	// It should be safe to use 0 for time mode check because the first element always uses the display mode.
	if (td->GetNYears() > 0) {
		return td->GetValueDate(ItemData::YearTypeEnum(m_yearMode), (*td)[0].m_timeMode, whichitem, year, month, day);
	}
	else return NoData;
}

void
SitesManager::SetValueDate(TimeData *td, int whichItem, double val, int year, int month, int day)
{
	td->SetValueDate(ItemData::YearTypeEnum(m_yearMode), whichItem, val, year, month, day);
}

string
SitesManager::CreateSynthLabel(int forecast_mode, vector<int> &yearList)
{
	// Build synthesize label.
	string label;
	switch (forecast_mode) {
  case 0:
	  label = "zeroed.";
	  break;
  case 1:			// average of years
	  {
		  label = "average of years ";
		  label += to_string<int>(yearList[0]);
		  for (int iyear=1; iyear<yearList.size(); iyear++) {
			  label += ", " + to_string<int>(yearList[iyear]);
		  }
	  }
	  break;
  case 2:			// repeat a year
	  label = "repeat of year ";
	  label += to_string<int>(yearList[0]);
	  break;
  case 3:			// cycle of years
	  label = "cycle of years from ";
	  label += to_string<int>(yearList[0]);
	  label += " to ";
	  label += to_string<int>(yearList[1]);
	  break;
	}

	return label;
}

void
SitesManager::ResetForecasting()
{
	m_histStartYear = m_startYear;
	m_histEndYear = m_endYear;
	m_yearList.clear();
	m_preYearList.clear();
	m_synthDataLabel = "";
	m_preSynthDataLabel = "";

	if (m_simStartYear < m_startYear) m_simStartYear = m_startYear;
}

void
SitesManager::Sort()
{
	sort(m_siteList.begin(), m_siteList.end());
}

void
SitesManager::UpdateCustOutput()
{
	// Fix the size of the output column vector.
	for (unsigned int isite=0; isite<m_siteList.size(); isite++) {
		SiteData &sd = m_siteList[isite];
		sd.m_outputCols.resize(m_custOutput.GetSize());
	}
}

void
SitesManager::SetPumpingUnits(WellPumpingTD::wellUnitsEnum units)
{
	WellPumpingTD::m_pumpingUnits = units;
}



SummaryOutputDataTD::SummaryOutputDataTD()
: TimeData(NumSummaryOutputDataTD)
{
}

SummaryOutputDataTD::SummaryOutputDataTD(const SummaryOutputDataTD &rhs)
: TimeData(rhs)
{
}


SummaryCustomOutputDataTD::SummaryCustomOutputDataTD()
: TimeData(NumSummaryCustomOutputDataTD)
{
}

SummaryCustomOutputDataTD::SummaryCustomOutputDataTD(const SummaryCustomOutputDataTD &rhs)
: TimeData(rhs)
{
}


//######################### SiteData  ########################//

SiteData::SiteData()
: m_bi(INFINITE_AQUIFER), m_b(0.0), m_w(0.0), m_tr(0.0),
m_s(0.0), m_dxx(0.0), m_sdf(0.0), m_timeUnits(DAYS), m_tbp(1), m_tt(0),
m_zzseg(0), m_z1(0), m_z2(0),
m_nc(1), m_siteType(WELL)
{
}

SiteData::SiteData(const SiteData &rhs)
: m_pumpingRecTD(rhs.m_pumpingRecTD), m_wellPumpingTD(rhs.m_wellPumpingTD),
m_rechargeTD(rhs.m_rechargeTD), m_metaDataTD(rhs.m_metaDataTD),
m_outputTD(rhs.m_outputTD)
{
	m_header1 = rhs.m_header1;
	m_header2 = rhs.m_header2;

	m_bi = rhs.m_bi;

	m_computeDepletionForSegment = rhs.m_computeDepletionForSegment;

	m_tr = rhs.m_tr;
	m_s = rhs.m_s;
	m_dxx = rhs.m_dxx;

	m_timeUnits = rhs.m_timeUnits;

	m_fact = rhs.m_fact;
	m_tbp = rhs.m_tbp;
	m_nc = rhs.m_nc;

	m_delt = rhs.m_delt;
	m_q = rhs.m_q;
	m_vptp = rhs.m_vptp;
	m_avp = rhs.m_avp;

	m_tt = rhs.m_tt;

	m_zzat = rhs.m_zzat;

	m_sdf = rhs.m_sdf;
	m_w = rhs.m_w;
	m_b = rhs.m_b;

	m_zzseg = rhs.m_zzseg;
	m_z1 = rhs.m_z1;
	m_z2 = rhs.m_z2;

	m_vunits = rhs.m_vunits;

	m_qq = rhs.m_qq;
	m_vv = rhs.m_vv;
	m_qd = rhs.m_qd;
	m_vd = rhs.m_vd;

	m_outputRecArray = rhs.m_outputRecArray;

	m_siteType = rhs.m_siteType;

	m_outputCols = rhs.m_outputCols;

	m_URF_MonthlyData= rhs.m_URF_MonthlyData;
	m_URF_DailyData = rhs.m_URF_DailyData;
}

SiteData&
SiteData::operator=(const SiteData &rhs)
{
	if (this == &rhs) return *this;

	m_header1 = rhs.m_header1;
	m_header2 = rhs.m_header2;

	m_bi = rhs.m_bi;

	m_computeDepletionForSegment = rhs.m_computeDepletionForSegment;

	m_tr = rhs.m_tr;
	m_s = rhs.m_s;
	m_dxx = rhs.m_dxx;

	m_timeUnits = rhs.m_timeUnits;

	m_fact = rhs.m_fact;
	m_tbp = rhs.m_tbp;
	m_nc = rhs.m_nc;

	m_pumpingRecTD = rhs.m_pumpingRecTD;
	m_wellPumpingTD = rhs.m_wellPumpingTD;
	m_rechargeTD = rhs.m_rechargeTD;
	m_metaDataTD = rhs.m_metaDataTD;
	m_outputTD = rhs.m_outputTD;

	m_delt = rhs.m_delt;
	m_q = rhs.m_q;
	m_vptp = rhs.m_vptp;
	m_avp = rhs.m_avp;

	m_tt = rhs.m_tt;

	m_zzat = rhs.m_zzat;

	m_sdf = rhs.m_sdf;
	m_w = rhs.m_w;
	m_b = rhs.m_b;

	m_zzseg = rhs.m_zzseg;
	m_z1 = rhs.m_z1;
	m_z2 = rhs.m_z2;

	m_vunits = rhs.m_vunits;

	m_qq = rhs.m_qq;
	m_vv = rhs.m_vv;
	m_qd = rhs.m_qd;
	m_vd = rhs.m_vd;

	m_outputRecArray.resize(rhs.m_outputRecArray.size());
	for (int i=0; i<m_outputRecArray.size(); i++) {
		m_outputRecArray[i] = rhs.m_outputRecArray[i];
	}

	m_siteType = rhs.m_siteType;

	m_outputCols = rhs.m_outputCols;

	m_URF_MonthlyData= rhs.m_URF_MonthlyData;
	m_URF_DailyData = rhs.m_URF_DailyData;

	return *this;
}

boolean
SiteData::operator<(const SiteData& rhs)
{
	return m_header1 < rhs.m_header1;
}

void
SiteData::UpdateFrom(const SiteData& rhs)
{
	m_header1 = rhs.m_header1;
	m_header2 = rhs.m_header2;

	m_bi = rhs.m_bi;

	m_computeDepletionForSegment = rhs.m_computeDepletionForSegment;

	m_tr = rhs.m_tr;
	m_s = rhs.m_s;
	m_dxx = rhs.m_dxx;

	m_timeUnits = rhs.m_timeUnits;

	m_fact = rhs.m_fact;
	m_tbp = rhs.m_tbp;
	m_nc = rhs.m_nc;

	m_pumpingRecTD.UpdateFrom(rhs.m_pumpingRecTD);
	m_wellPumpingTD.UpdateFrom(rhs.m_wellPumpingTD);
	m_rechargeTD.UpdateFrom(rhs.m_rechargeTD);
	m_metaDataTD.UpdateFrom(rhs.m_metaDataTD);
	m_outputTD.UpdateFrom(rhs.m_outputTD);

	m_delt = rhs.m_delt;
	m_q = rhs.m_q;
	m_vptp = rhs.m_vptp;
	m_avp = rhs.m_avp;

	m_tt = rhs.m_tt;

	m_zzat = rhs.m_zzat;

	m_sdf = rhs.m_sdf;
	m_w = rhs.m_w;
	m_b = rhs.m_b;

	m_zzseg = rhs.m_zzseg;
	m_z1 = rhs.m_z1;
	m_z2 = rhs.m_z2;

	m_vunits = rhs.m_vunits;

	m_qq = rhs.m_qq;
	m_vv = rhs.m_vv;
	m_qd = rhs.m_qd;
	m_vd = rhs.m_vd;

	m_outputRecArray.resize(rhs.m_outputRecArray.size());
	for (int i=0; i<m_outputRecArray.size(); i++) {
		m_outputRecArray[i] = rhs.m_outputRecArray[i];
	}

	m_siteType = rhs.m_siteType;

	// Special case for customized output options.  If the source has a different number of options,
	//   then don't update because every site must have the same number of options.
	if (m_outputCols.size() == rhs.m_outputCols.size())
	{
		m_outputCols = rhs.m_outputCols;
	}

	m_URF_MonthlyData= rhs.m_URF_MonthlyData;
	m_URF_DailyData = rhs.m_URF_DailyData;
}

void
SiteData::SetTimeUnits(SiteData::TimeUnitsEnum newUnits)
{
	// Useful only for modified interface mode.

	// Reset time period count.
	m_tt = 0;

	{
		ItemData::ItemAggEnum aggs[] = {ItemData::AVERAGE, ItemData::LAST_VAL};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY};

			m_pumpingRecTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::DAILY};

			m_pumpingRecTD.Initialize(times, aggs);      
		}
	}

	{
		ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY};

			m_metaDataTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY};

			m_metaDataTD.Initialize(times, aggs);      
		}
	}

	// Go ahead and allocate space for output as well.
	{
		ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL, ItemData::TOTAL};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY};

			m_outputTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::DAILY};

			m_outputTD.Initialize(times, aggs);      
		}
	}

	if (m_siteType == WELL) {
		ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL, ItemData::AVERAGE, ItemData::TOTAL};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY};

			m_wellPumpingTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::MONTHLY, ItemData::DAILY};

			m_wellPumpingTD.Initialize(times, aggs);      
		}
	}
	else {
		ItemData::ItemAggEnum aggs[] = {ItemData::TOTAL, ItemData::TOTAL, ItemData::AVERAGE, ItemData::AVERAGE, ItemData::AVERAGE, ItemData::AVERAGE};
		if (newUnits == SiteData::MONTHS) {
			ItemData::TimeEnum times[] = {ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY};

			m_rechargeTD.Initialize(times, aggs);
		}
		else if (newUnits == SiteData::DAYS) {
			ItemData::TimeEnum times[] = {ItemData::DAILY, ItemData::DAILY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY, ItemData::MONTHLY};

			m_rechargeTD.Initialize(times, aggs);      
		}
	}

	m_timeUnits = newUnits;
}

void
SiteData::SetPeriod(int startYear, int endYear)
{
	m_pumpingRecTD.SetPeriod(startYear, endYear);
	m_metaDataTD.SetPeriod(startYear, endYear);
	if (m_siteType == SiteData::WELL) {
		m_wellPumpingTD.SetPeriod(startYear, endYear);
	}
	else {
		m_rechargeTD.SetPeriod(startYear, endYear);
	}

	// Initialize params to zero.
	m_pumpingRecTD.Fill(PumpingRecTD::Q, 0);
	m_wellPumpingTD.Fill(WellPumpingTD::Q, 0);
	m_wellPumpingTD.Fill(WellPumpingTD::AppEff, 0);
	for (int i=0; i<RechargeTD::NumRechargeTD; i++) {
		m_rechargeTD.Fill(i, 0);
	}
}

void
SiteData::AddRecords(int loc, int nrecs)
{
	m_pumpingRecTD.AddRecords(loc, nrecs);
	m_metaDataTD.AddRecords(loc, nrecs);
	if (m_siteType == SiteData::WELL) {
		m_wellPumpingTD.AddRecords(loc, nrecs);
	}
	else {
		m_rechargeTD.AddRecords(loc, nrecs);
	}
}

void
SiteData::RemoveRecords(vector<int> &recVector)
{
	m_pumpingRecTD.RemoveRecords(recVector);
	m_metaDataTD.RemoveRecords(recVector);
	if (m_siteType == SiteData::WELL) {
		m_wellPumpingTD.RemoveRecords(recVector);
	}
	else {
		m_rechargeTD.RemoveRecords(recVector);
	}
}

int
SiteData::Calculate(bool useAvgDaily, errorFuncPtr errorFunc, int displayMode)
{
	int retval = 0;

	// Generate vector of days in each month if timestep is monthly and using
	//   modified version.
	SiteData::daysPerMonth.clear();
	int npa_prime = 0; // This will be the number of days in the period.
	if (m_timeUnits == SiteData::MONTHS && displayMode == SitesManager::DISPLAY_MODIFIED) {
		for (int y=m_pumpingRecTD.GetStartYear(); y<=m_pumpingRecTD.GetEndYear(); y++) {
			for (int m=0; m<12; m++) {
				int d = getDaysInMonth(m, y, ItemData::CALENDAR);
				if (m == 1 && is_leap_year(y)) d++;
				SiteData::daysPerMonth.push_back(d);
				//npa_prime += d;
			}
		}
	}

	// Set q to negative if this is a recharge site.
	int m_pumpingSign = (m_siteType == WELL ? 1 : -1);

	// Get length of pumping periods and number of timesteps (line 3000)
	int dela;
	int nppc = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q);
	int npa = CalcNumPumpPeriods(&dela);

	int np = m_nc * nppc;

	// Allocate space
	m_qq.resize(npa);
	m_vv.resize(npa);
	m_qd.resize(npa);
	m_vd.resize(npa);

	// Initialize
	for (int inpa=0; inpa<m_qq.size(); inpa++) {
		m_qq[inpa] = 1.0; // initialize to 1 to allow for instantaneous effect
		m_vv[inpa] = 1.0; // when the calculation would evaluate to zero or otherwise fail.
		m_qd[inpa] = 0.0;
		m_vd[inpa] = 0.0;
	}

	//m_outputRecArray.resize(npa);
	m_outputRecArray.resize(0);

	// Get pumping volume
	m_vunits = "gallons";
	m_delt.resize(np);
	m_q.resize(np);
	m_vptp.resize(np);
	m_avp.resize(np);
	m_zzat.resize(np);

	int tt(0);
	double avsum(0);
	m_pumpingRecTD.SetConversionFactor(1440);

	for (int x=0; x<np; x++) {
		m_delt[x] = m_pumpingRecTD.GetValue(PumpingRecTD::Delta, x % nppc);
		m_q[x] = m_pumpingRecTD.GetValue(PumpingRecTD::Q, x % nppc);

		// Change no data values to zeros.  This is the case when we are using non-calendar years.
		if (m_delt[x] < 1) {
			m_delt[x] = 1;
		}
		if (m_q[x] == SitesManager::NoData) {
			m_q[x] = 0;
		}

		m_vptp[x] = m_q[x] * m_delt[x] * m_pumpingRecTD.m_cf * GetFact(x, !useAvgDaily);
		avsum += m_vptp[x];
		m_avp[x] = avsum;

		// Also calculate zzat
		if (m_delt[x] > 0) {
			tt += m_delt[x];
		}
		m_zzat[x] = tt;
	}

	//if (fabs(avsum) > 1000000) {
	// Always use acre-feet so that all sites will be consistent
	{
		// Convert to acre-feet
		avsum = 0;
		m_vunits = "acre-feet";
		m_pumpingRecTD.SetConversionFactor(1440/325851.0);
		//m_pumpingRecTD.SetConversionFactor(1440/325900.0);
		for (int x=0; x<np; x++) {
			m_vptp[x] = m_q[x] * m_delt[x] * m_pumpingRecTD.m_cf * GetFact(x, !useAvgDaily);
			avsum += m_vptp[x];
			m_avp[x] = avsum;
		}
	}

	// Now we can begin solving.
	int x;

	if (m_bi != URF) {
		if (m_zzseg) {		// depletion for portion of stream

			bool iflg = FALSE;
			if (m_z1 == -99999) {
				m_z1 = 0;
				iflg = TRUE;
			}
			if (m_z2 == 99999) {
				m_z2 = 0;
				iflg = TRUE;
			}

			double yy = m_dxx;
			double ql = 0, qn = 0;

			double qp = 0; //!! not in basic code
			double avv = 0; //!! not in basic code

			switch (m_bi) {
	  case EFFECTIVE_SDF:
		  break;
	  case INFINITE_AQUIFER: {
		  double l1 = m_z1, l2 = m_z2;
		  double t = 0;
		  if (m_tr > 0) {
			  for (x=0; x<npa; x++) {
				  t += dela;
				  m_qq[x] = 0;
				  m_vv[x] = 0;
				  double u = yy / sqrt(4.0 * m_tr * GetFact(x, !useAvgDaily) * t / (m_s * 7.48051945));
				  if (u <= 2.9) {
					  double bqq, svv;
					  if (iflg) {
						  CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv);

						  m_qq[x] = .5 * bqq;
						  m_vv[x] = .5 * svv;
					  }
					  u = 4 * m_tr * t * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);

					  bqq = CalcStreamQ(l1, l2, yy, u);
					  m_qq[x] += bqq;
					  qn = bqq;

					  u = 4 * m_tr * (t-dela/2.0) * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);
					  bqq = CalcStreamQ(l1, l2, yy, u);
					  qp = bqq;

					  svv = CalcStreamV(qn, ql, qp, dela);
					  avv += svv;

					  m_vv[x] += avv / t;

				  }	    
				  ql = qn;

			  }
		  }
							 }
							 break;
	  case ALLUVIAL_AQUIFER: {
		  double t = 0;
		  double l1 = m_z1, l2 = m_z2; // 4560
		  double xyz;

		  if (m_tr > 0) {
			  for (x=0; x<npa; x++) {
				  m_qq[x] = 0;
				  m_vv[x] = 0;

				  double fac = 1;	// 4580
				  ql = qn;		// 4590
				  qn = 0;		// 4600
				  qp = 0;		// 4610
				  t += dela;		// 4620
				  yy = -m_dxx;		// 4630

				  double ww;
				  do {
					  yy += 2 * m_dxx;	// 4640
					  ww = 0;		// 4650
					  double u = 0;
					  xyz = 0;

					  double bqq, svv;
					  if (iflg) {		// 4660
						  u = yy / sqrt(4 * m_tr * t * GetFact(x, !useAvgDaily) / (m_s * 7.48051945)); // 4670

						  CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv); // 4680 GOSUB 5490

						  ww += bqq;	// 4690
						  m_qq[x] += bqq * fac/2.0;	// 4700
						  m_vv[x] += svv * fac/2.0;	// 4710
					  } // close 4660

					  u = 4 * m_tr * t * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);	// 4720

					  if (l1 != l2) {	// 4730
						  bqq = CalcStreamQ(l1, l2, yy, u);	// GOSUB 7130 (SUBROUTINE FOR q/Q (STREAM SEGMENT))
						  ww += bqq;	// 4740
						  m_qq[x] += bqq * fac; // 4750
						  qn += bqq * fac;	// 4760
						  u = 4 * m_tr * (t - dela/2.0) * GetFact(x, !useAvgDaily) / (m_s * 7.48051945); // 4770
						  xyz = u;		// 4780

						  bqq = CalcStreamQ(l1, l2, yy, u);	// GOSUB 7130 (SUBROUTINE FOR q/Q (STREAM SEGMENT))
						  qp += bqq * fac;	// 4800
					  } // close 4730

					  yy = yy - 2*m_dxx + 2*m_w; // error in line 4810 // 4810

					  if (iflg) {		// 4820
						  u = yy / sqrt(4 * m_tr * GetFact(x, !useAvgDaily) * t / (m_s * 7.48051945)); // 4830

						  CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv); // 4840
						  ww += bqq;	// 4850
						  m_qq[x] += bqq * fac/2.0;	// 4860
						  m_vv[x] += svv * fac/2.0;	// 4870
					  } // close 4820
					  u = xyz;		// 4880

					  if (l1 != l2) {	// 4890
						  bqq = CalcStreamQ(l1, l2, yy, u);	// GOSUB 7130 (SUBROUTINE FOR q/Q (STREAM SEGMENT))
						  qp += bqq * fac;	// 4900
						  u = 4 * m_tr * t * GetFact(x, !useAvgDaily) / (m_s * 7.48051945); // 4910
						  bqq = CalcStreamQ(l1, l2, yy, u);	// 4920
						  ww += bqq;	// 4930
						  qn += bqq * fac;	// 4940
						  m_qq[x] += bqq * fac; // 4950
					  } // close 4890

					  fac *= -1;		// 4970
				  } while (fabs(ww) > 0.00001);	// 4960

				  if (l1 != l2) {	// 4990 SUBROUTINE FOR v/V (STREAM SEGMENT)
					  double svv = CalcStreamV(qn, ql, qp, dela);
					  avv += svv;		// 5000
					  m_vv[x] += avv/t;	// 5010
				  }
			  } // 5020
		  }
							 }
							 break;
	  case NO_FLOW: {
		  double t = 0;
		  if (m_tr > 0) {
			  for (x=0; x<npa; x++) {
				  m_qq[x] = 0;
				  m_vv[x] = 0;
				  t += dela;
				  double u = yy / sqrt(4 * m_tr * GetFact(x, !useAvgDaily) * t / (m_s * 7.48051945));
				  if (u <= 2.9) {
					  double bqq, svv;
					  if (iflg) {
						  CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv);
						  m_qq[x] = bqq;
						  m_vv[x] = svv;

						  if (m_b == m_z2) continue;

						  double l1 = m_z2, l2 = 2 * m_b - m_z2;
						  u = 4 * m_tr * GetFact(x, !useAvgDaily) * t / (m_s * 7.48051945);

						  bqq = CalcStreamQ(l1, l2, yy, u);
						  m_qq[x] -= bqq;
						  qn = bqq;

						  u = 4 * m_tr * (t - dela/2.0) * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);
						  bqq = CalcStreamQ(l1, l2, yy, u);
						  qp = bqq;

						  svv = CalcStreamV(qn, ql, qp, dela);
						  avv += svv;
						  m_vv[x] -= avv/t;
					  }
					  else {
						  double l1 = m_z1, l2 = m_z2;
						  u = 4 * m_tr * GetFact(x, !useAvgDaily) * t / (m_s * 7.48051945);
						  bqq = CalcStreamQ(l1, l2, yy, u);
						  m_qq[x] = bqq;
						  qn = bqq;
						  u = 4 * m_tr * (t - dela/2.0) * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);

						  bqq = CalcStreamQ(l1, l2, yy, u);
						  qp = bqq;
						  l1 = 2 * m_b - m_z2;
						  l2 = 2 * m_b - m_z1;
						  bqq = CalcStreamQ(l1, l2, yy, u);
						  qp += bqq;

						  u = 4 * m_tr * t * GetFact(x, !useAvgDaily) / (m_s * 7.48051945);
						  bqq = CalcStreamQ(l1, l2, yy, u);
						  qn += bqq;
						  m_qq[x] += bqq;

						  svv = CalcStreamV(qn, ql, qp, dela);
						  avv += svv;
						  m_vv[x] += avv / t;
					  }
				  }
				  ql = qn;
			  }
			  break;
		  }
					}
			}
		}
		else {
			double u;
			double bqq, svv;
			if (m_bi != ALLUVIAL_AQUIFER) {
				int t=0;
				if (m_tr > 0 || m_bi == EFFECTIVE_SDF) {
					for (x=0; x<npa; x++) {
						t += dela;
						if (m_bi == EFFECTIVE_SDF) {
							u = sqrt(m_sdf / (4 * (GetFact(x, !useAvgDaily)) * t));
						}
						else {
							u = m_dxx / sqrt((4 * m_tr * GetFact(x, !useAvgDaily) * t) / (m_s * 7.48051945));
						}

						CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv);

						m_qq[x] = bqq;
						m_vv[x] = svv;
					}
				}
			}
			else {
				int t=0;
				int n=0;
				int iz=0;
				if (m_tr > 0 && m_w > 0 && m_s > 0) {
					for (x=0; x<npa; x++) {
						// Added Kenny
						if (m_q[x] != 0)
						{
							n = 0; // Kenny
							for (iz = x; iz<npa; iz++) //    Kenny (Loop to take from when q(mon) is found)
							{
								t += dela;
								double qs = 0, vs = 0;
								double yy = -m_dxx;
								int fac = 1;

								n = n + 1;  // Kenny (counter for depletions to start at month/day one)

								do {
									yy = yy + 2 * m_dxx;
									u = yy / sqrt((4 * m_tr * GetFact(x, !useAvgDaily) * n / (m_s * 7.48051945))); // Kenny t becomes N

									CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv);

									qs += bqq * fac;
									vs += svv * fac;

									if (bqq != 0) {
										yy = yy - 2 * m_dxx + 2 * m_w;
										u = yy / sqrt((4 * m_tr * GetFact(x, !useAvgDaily) * n / (m_s * 7.48051945))); // Kenny t becomes N

										CALL_MEMBER_FN(*this, errorFunc)(u, &bqq, &svv);

										qs += bqq * fac;
										vs += svv * fac;
										if (bqq != 0) {
											fac *= -1;
										}
									}
								} while (bqq != 0);
								m_qq[n-1] = qs;
								m_vv[n-1] = vs;
							}
							break;
						}
					}
				}
			}
		}
	}
	else {
		// For uniform return flow, just fill in the vv paramter.
		int np_urf = min(npa, m_URF_DataCalc.size());
		for (int j=0; j<np; j++) {
			// Carry out the effect of this month's pumping.
			if (m_q[j] != 0)
			{
				for (x=0; x<np_urf; x++) {
					if (j + x >= np) break;
					m_vd[j+x] += m_URF_DataCalc[x] * m_q[j] * m_pumpingSign * GetFact(j, !useAvgDaily) * dela;
				}
			}
			// Accumulate the volume.
			if (j > 0) m_vd[j] += m_vd[j-1];
		}
	}

	if (m_bi != URF) {
		// Generate depletion
		int adel = 0;
		for (int j=0; j<np; j++) {
			int n = -1;
			if (m_q[j] != 0)
			{
				for (x=adel; x<npa; x++) {
					n++;
					m_qd[x] += m_qq[n] * m_q[j] * m_pumpingSign;
					m_vd[x] += m_vv[n] * m_q[j] * m_pumpingSign * GetFact(j, !useAvgDaily) * (n+1) * dela;
				}

				n = -1;
				if (np == 1) continue;

				for (x=int(m_zzat[j] / dela); x<npa; x++) {
					n++;
					m_qd[x] -= m_qq[n] * m_q[j] * m_pumpingSign;
					m_vd[x] -= m_vv[n] * m_q[j] * m_pumpingSign * GetFact(j, !useAvgDaily) * (n+1) * dela;
				}

			}
			adel = int(m_zzat[j] / dela);
		}
	}

	// Set output size if original mode
	if (displayMode == SitesManager::DISPLAY_ORIGINAL)
	{
		m_outputTD.SetSpan(npa);
	}

	// Print depletion
	double tdela = 0;
	double vfac = 1440;		// gallons
	//if (m_vunits == "acre-feet") vfac /= 325900.0;
	if (m_vunits == "acre-feet") vfac /= 325851.0;
	double vdtp = 0.0;

	if (m_tbp > 0) {
		for (x=0; x<npa; x++) {
			if (x * dela / m_tbp == int(x * dela / m_tbp)) {
				OutputRec or;
				or.m_time = dela * (x+1);
				or.m_depRate = m_qd[x];
				or.m_depVol = m_vd[x] * vfac;
				or.m_depVolThis = m_vd[x] * vfac - vdtp;

				if (np == 1) {
					or.m_depRatePerc = m_qd[x] * 100.0 / m_q[0] * m_pumpingSign;
					or.m_depVolPerc = m_vd[x] * 100.0 / (m_q[0] * m_pumpingSign * dela * (x+1) * GetFact(x, !useAvgDaily));
				}

				m_outputRecArray.push_back(or);
			}
			vdtp = m_vd[x] * vfac;

			// Assign to output
			m_outputTD.SetValue(OutputDataTD::Diversion, x, m_vptp[x]);
			m_outputTD.SetValue(OutputDataTD::Depletion, x, m_outputRecArray[x].m_depVolThis);
		}
	}

	return retval;
}

int
SiteData::CalcNumPumpPeriods(int *dela)
{
	*dela = CalcMinPumpPeriod();

	int tt(0);
	//tt = m_tt;
	if (tt == 0) {
		// Calculate total time
		for (int icycle=0; icycle<m_nc; icycle++) {
			int ndays = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Delta);
			for (int iday=0; iday<ndays; iday++) {
				int t = m_pumpingRecTD.GetValue(PumpingRecTD::Delta, iday);
				// If nodata, then set time to 1.
				if (t < 1) t = 1;
				tt += t;
			}
		}
	}

	// If the time interval does not divide evenly into a pumping interval,
	//   then reduce dela by one
	bool isdone(false);
	int ndays = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Delta);
	for (int iday=0; iday<ndays && !isdone; iday++) {
		int t = m_pumpingRecTD.GetValue(PumpingRecTD::Delta, iday);
		if (t % *dela != 0) {
			(*dela)--;
			isdone = true;
		}
	}

	return tt / *dela;
}

int
SiteData::CalcMinPumpPeriod()
{
	if (m_tbp > 0)  return m_tbp;
	else {
		int mn = m_pumpingRecTD.GetValue(PumpingRecTD::Delta, 0, 0);
		if (mn < 1) mn = 1;

		for (int iyear=0; iyear<m_pumpingRecTD[PumpingRecTD::Delta].GetNYears(); iyear++) {
			int ndays = getDaysInYear(m_pumpingRecTD[PumpingRecTD::Delta].m_startyear + iyear);
			for (int iday=0; iday<ndays; iday++) {
				int test_mn = m_pumpingRecTD.GetValue(PumpingRecTD::Delta, iyear, iday);
				mn = min(test_mn, mn);
			}
		}

		return mn;
	}
}

void
SiteData::CalcErrorFunc(double u, double *bqq, double *svv)
{
	double pi = 3.141592653589793; //3.1415926535897932384626433832795;
	double e = 2.718281828459045; //2.71828182845904523536028747135266;

	//********* SUB TO EVALUATE COMPLIMENTARY ERROR FUNCTION AND 2nd REPEATED
	//********* INTEGRAL OF COMPLIMENTARY ERROR FUNCTION ***********************
	if (u > 3.9) {
		*bqq = 0;
		*svv = 0;
	}
	else {
		double sum = u;
		double m = 0;
		double n = u * u;
		double term = u;
		double p = 1;

		do {
			m++;
			p += 2;
			term = (-1 * term * n * (p-2)) / (p * m);
			sum += term;
			//} while (fabs(term) > 1e-8);  // was 1e-8
		} while (fabs(term) > 1e-14);  // was 1e-8

		*bqq = 1 - 2 * sum / sqrt(pi);
		*svv = (*bqq) * (1 + 2 * (u * u)) - (2 * u * pow(e, -(u * u))) / sqrt(pi);
	}
}

void
SiteData::CalcErrorFuncNew(double u, double *bqq, double *svv)
{
	double pi = 3.141592653589793; //3.1415926535897932384626433832795;
	double e = 2.718281828459045; //2.71828182845904523536028747135266;

	//********* SUB TO EVALUATE COMPLIMENTARY ERROR FUNCTION AND 2nd REPEATED
	//********* INTEGRAL OF COMPLIMENTARY ERROR FUNCTION ***********************
	//if (u > 2.9) {
	if (u >= 2) {
		double a0=1, b0=0;
		double a1 = 0, b1 = 1;
		double f1 = 0, f2 = 0;
		int i=0;
		do {
			i++;
			double g = 2 - (i % 2);
			double a2 = g * u * a1 + i * a0;
			double b2 = g * u * b1 + i * b0;
			f2 = a2 / b2;
			double d = fabs(f2 - f1);
			if (d < 1e-14) break;
			a0 = a1; b0 = b1;
			a1 = a2; b1 = b2;
			f1 = f2;
		} while (true);
		*bqq = 2 * pow(e, (-u * u)) / (2 * u + f2) / sqrt(pi); 
		*svv = *bqq * (1 + 2 * (u * u)) - (2 * u * pow(e, -(u * u))) / sqrt(pi);
	}
	else {
		double sum = u;
		double m = 0;
		double n = u * u;
		double term = u;
		double p = 1;

		do {
			m++;
			p += 2;
			term = (-1 * term * n * (p-2)) / (p * m);
			sum += term;
			//} while (fabs(term) > 1e-8);  // was 1e-8
		} while (fabs(term) > 1e-14);  // was 1e-8

		*bqq = 1 - 2 * sum / sqrt(pi);
		*svv = (*bqq) * (1 + 2 * (u * u)) - (2 * u * pow(e, -(u * u))) / sqrt(pi);
	}
}


double
SiteData::CalcStreamQ(double l1, double l2, double yy, double u)
{
	double pi = 3.1415926535897932384626433832795;
	double e = 2.71828182845904523536028747135266;

	int simn = fabs(int(l2-l1) / yy) * 20 + .5;
	if (simn < 40) simn = 40;
	else if (simn > 100) simn = 100;

	double delx = (l2 - l1) / simn;
	double xc = l1 - delx;

	double *h = new double [simn+1];

	int n;
	for (n=0; n<simn+1; n++) {
		xc += delx;
		double ab = yy * yy + xc * xc;

		if (ab / u > 60)  h[n] = 0;
		else h[n] = (pow(e, -(ab / u))) / ab;
	}

	double bqq = h[0] + h[simn];

	for (n=1; n<simn; n++) {
		if ((n+1) % 2 == 0) {
			// even case
			bqq += 4 * h[n];
		}
		else {
			// odd case
			bqq += 2 * h[n];
		}
	}
	if (bqq<1e-33) bqq=0;
	else bqq = bqq * delx * yy / (3*pi);

	delete [] h;

	return bqq;
}

double
SiteData::CalcStreamV(double qn, double ql, double qp, double dela)
{
	return (qn + ql) * dela/2.0 + (qp - (ql+qn)/2.0) * dela *2.0/3.0;
}

void
SiteData::WriteOutput(ofstream &ostr)
{
	char *timeunits[] = {"(days)", "(weeks)", "(months)", "(years)"};

	ostr << "Pumping Schedule" << endl;
	ostr << "Pumping Period \t Q (GPM) \t Length " << timeunits[m_timeUnits]
	<< " Cum. Time " << timeunits[m_timeUnits]
	<< " Vol. Pumped This Period (" << m_vunits
		<< ") Cum. Vol Pumped (" << m_vunits << ")" << endl;

	int old_prec = ostr.precision();
	ostr.precision(16);

	int nppc = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q);
	int np = m_nc * nppc;
	for (int i=0; i<np; i++) {
		ostr << i+1 << " \t " << m_q[i] << " \t " << m_delt[i] << " \t "
			<< m_zzat[i] << " \t " << m_vptp[i] << " \t " << m_avp[i] << endl;
	}

	ostr << "Stream Depletion" << endl;
	ostr << "Time " << timeunits[m_timeUnits] << " \t "
		<< "Dep. Rate (GPM) \t Vol. of Dep. (" << m_vunits << ") \t "
		<< "Vol. of Dep. This Step (" << m_vunits << ")" << endl;

	int nitems = m_outputRecArray.size();
	for (int iitems=0; iitems<nitems; iitems++) {
		ostr << m_outputRecArray[iitems].m_time << "\t"
			<< m_outputRecArray[iitems].m_depRate << "\t"
			//<< m_outputRecArray[iitems].m_depRatePerc << "\t"
			<< m_outputRecArray[iitems].m_depVol << "\t"
			// << m_outputRecArray[iitems].m_depVolPerc << "\t"
			<< m_outputRecArray[iitems].m_depVolThis << endl;
	}

	ostr.precision(old_prec);
}

string
SiteData::WriteOutput()
{
	string ostr;

	string timeunits[] = {"(days)", "(weeks)", "(months)", "(years)"};

	ostr += "Pumping Schedule\r\n";
	ostr += "Pumping Period \t Q (GPM) \t Length " + timeunits[m_timeUnits]
	+ " Cum. Time " + timeunits[m_timeUnits]
	+ " Vol. Pumped This Period (" + m_vunits
		+ ") Cum. Vol Pumped (" + m_vunits + ")" + "\r\n";

	int nppc = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q);
	int np = m_nc * nppc;
	for (int i=0; i<np; i++) {
		ostr += intToString(i+1) + " \t " + doubleToString(m_q[i])
			+ " \t " + doubleToString(m_delt[i]) + " \t "
			+ doubleToString(m_zzat[i]) + " \t " + doubleToString(m_vptp[i])
			+ " \t " + doubleToString(m_avp[i]) + "\r\n";
	}

	ostr += "Stream Depletion\n";
	ostr += "Time " + timeunits[m_timeUnits] + " \t "
		+ "Dep. Rate (GPM) \t Vol. of Dep. (" + m_vunits + ") \t "
		+ "Vol. of Dep. This Step (" + m_vunits + ")" + "\r\n";

	int nitems = m_outputRecArray.size();
	for (int iitems=0; iitems<nitems; iitems++) {
		ostr += to_string<double>(m_outputRecArray[iitems].m_time, 2) + "\t"
			+ to_string<double>(m_outputRecArray[iitems].m_depRate, 2) + "\t"
			+ to_string<double>(m_outputRecArray[iitems].m_depVol, 2) + "\t"
			+ to_string<double>(m_outputRecArray[iitems].m_depVolThis, 2) + "\r\n";
	}

	return ostr;
}

string
SiteData::ReadOutput(ifstream &istr, int npa)
{
	int nppc = m_pumpingRecTD.GetNPeriods(PumpingRecTD::Q);
	int np = m_nc * nppc;

	int dela;
	m_tt = 0;
	if (npa <= 0) {
		npa = CalcNumPumpPeriods(&dela);
	}
	else {
		// This is the modified output case where the number of output records may
		//   not be the same as the number of input records because the simulation
		//   years may not be the same as the starting and ending year.
		np = npa;
	}

	if (npa == 0) npa = 1;

	// Read pumping schedule.
	m_delt.resize(np);
	m_q.resize(np);
	m_vptp.resize(np);
	m_avp.resize(np);
	m_zzat.resize(np);

	istr.getline(buf, sizeof(buf));	// "Pumping Schedule" line
	istr.getline(buf, sizeof(buf));	// "Pumping Period" line

	// Scan for vunits.
	string cbuf(buf);
	int startIdx = cbuf.rfind('(');
	int endIdx = cbuf.rfind(')');
	m_vunits = cbuf.substr(startIdx+1, endIdx - startIdx -1);

	double dummy;
	if (m_outputTD.GetNPeriods(OutputDataTD::Diversion) == 0) {
		// This will be true in original mode.
		m_outputTD.SetSpan(npa);
	}

	int i;
	for (i=0; i<np; i++) {
		istr >> dummy >> m_q[i] >> m_delt[i] >> m_zzat[i] >> m_vptp[i] >> m_avp[i];
		// Update TimeData output
		m_outputTD.SetValue(OutputDataTD::Diversion, i, m_vptp[i]);
		// Add clear flag because the model can generate NAN values.
		istr.clear();
	}

	// Skip to next line.
	istr.getline(buf, sizeof(buf));
	istr.getline(buf, sizeof(buf));	// "Stream Depletion" line
	istr.getline(buf, sizeof(buf));	// Header line

	m_outputRecArray.resize(npa);

	bool showErr(true);
	for (i=0; i<npa; i++) {
		string inp[3];
		//     istr >> m_outputRecArray[i].m_time 
		// 	 >> m_outputRecArray[i].m_depRate
		// 	 >> m_outputRecArray[i].m_depVol
		// 	 >> m_outputRecArray[i].m_depVolThis;
		istr >> m_outputRecArray[i].m_time 
			>> inp[0]
		>> inp[1]
		>> inp[2];
		// Check for NAN.
		if (inp[0].find("nan") == string::npos) {
			m_outputRecArray[i].m_depRate = atof(inp[0].c_str());
			m_outputRecArray[i].m_depVol = atof(inp[1].c_str());
			m_outputRecArray[i].m_depVolThis = atof(inp[2].c_str());
		}
		else if (showErr) {
			AfxMessageBox((m_header1 + " generated an error in output.").c_str());
			showErr = false;
		}
		m_outputTD.SetValue(OutputDataTD::Depletion, i, m_outputRecArray[i].m_depVolThis);
	}

	// Skip to next line.
	istr.getline(buf, sizeof(buf));

	return "";
}

void
SiteData::SetSiteType(SiteTypeEnum newType)
{
	m_siteType = newType;
	// set time period for site data.
	SetTimeUnits(m_timeUnits);

	// Make sure data structures for new type are allocated
	// Initialize params to zero.
	int startYear = m_pumpingRecTD.GetStartYear();
	int endYear = startYear + m_pumpingRecTD.GetNYears() -1;

	if (m_siteType == SiteTypeEnum::WELL)
	{
		if (m_wellPumpingTD.GetStartYear() != startYear || m_wellPumpingTD.GetNYears() != m_pumpingRecTD.GetNYears())
		{
			m_wellPumpingTD.SetPeriod(startYear, endYear);
			m_wellPumpingTD.Fill(WellPumpingTD::Q, 0, -1, -1, TRUE);
			m_wellPumpingTD.Fill(WellPumpingTD::AppEff, 0, -1, -1, TRUE);
		}
	}
	else
	{
		if (m_rechargeTD.GetStartYear() != startYear || m_rechargeTD.GetNYears() != m_pumpingRecTD.GetNYears())
		{
			m_rechargeTD.SetPeriod(startYear, endYear);
			for (int i=0; i<RechargeTD::NumRechargeTD; i++) {
				m_rechargeTD.Fill(i, 0, -1, -1, TRUE);
			}
		}
	}
}

string
SiteData::GetPumpingUnitsStr()
{
	return m_wellPumpingTD.GetPumpingUnitsStr();
}

void
SiteData::SetOutputUnits(volumeUnitsEnum units)
{
	double cf = -1;
	if (m_vunits == "gallons") {
		if (units == ACFT) {
			//cf = 1440/325900.0;	// gallons->acre-feet
			cf = 1/325851.0;
			m_vunits = "acre-feet";
		}
	}
	else if (m_vunits == "acre-feet") {
		if (units == GALLONS) {
			cf = 325851.0;
			//cf = 325900.0/1440;
			m_vunits = "gallons";
		}
	}

	if (cf > 0) {
		int ninp = m_vptp.size();
		for (int iinp=0; iinp<ninp; iinp++) {
			m_vptp[iinp] *= cf;
			m_avp[iinp] *= cf;
		}

		int nrec = m_outputRecArray.size();
		for (int irec=0; irec<nrec; irec++) {
			m_outputRecArray[irec].m_depVol *= cf;
			m_outputRecArray[irec].m_depVolThis *= cf;
		}
	}
}

double
SiteData::GetFact(int iper, bool useActualDays)
{
	if (daysPerMonth.size() > 0 && useActualDays) return daysPerMonth[iper];
	else {
		const double ndays[4] = {1.0, 7.019231, 30.41667, 365};
		return ndays[m_timeUnits];
	}
}

bool
SiteData::ShowInOutput(CustOutputManager &com)
{
	return com.ShowInOutput(m_outputCols);
}

void
SiteData::CleanData(ItemData::YearTypeEnum yearType)
{
	m_pumpingRecTD.Clean(yearType);
	m_metaDataTD.Clean(yearType);
	m_wellPumpingTD.Clean(yearType);
	m_rechargeTD.Clean(yearType);
}


//######################### PumpingRec  ########################//

PumpingRecTD::PumpingRecTD()
: TimeData(NumPumpingRecTD)
{
}

PumpingRecTD::PumpingRecTD(const PumpingRecTD &rhs)
: TimeData(rhs)
{
}

PumpingRecTD&
PumpingRecTD::operator=(const PumpingRecTD &rhs)
{
	if (&rhs == this) return *this;

	TimeData::operator=(rhs);

	return *this;
}

void
PumpingRecTD::ConvertToVolume()
{
	// Only useful for monthly data.
	if (IsMonthly(Q)) {
		int nyears = GetNYears();
		for (int iyear=0; iyear<nyears; iyear++) {
			for (int imonth=0; imonth<12; imonth++) {
				double val = GetValueDate(ItemData::CALENDAR, ItemData::MONTHLY, Volume, iyear, imonth);
				SetValueDate(ItemData::CALENDAR, Q, val, iyear, imonth);
			}
		}
	}
}

void
PumpingRecTD::UnconvertFromVolume()
{
	// Only useful for monthly data.
	if (!IsMonthly(Q)) {
		int nyears = GetNYears();
		for (int iyear=0; iyear<nyears; iyear++) {
			for (int imonth=0; imonth<12; imonth++) {
				double val = GetValueDate(ItemData::CALENDAR, ItemData::MONTHLY, Q, iyear, imonth);
				if (val > SitesManager::NoData)
				{
					val = val / (1440/325851.0) / 30.41667;
				}
				SetValueDate(ItemData::CALENDAR, Q, val, iyear, imonth);
			}
		}
	}
}

void
PumpingRecTD::ConvertToFlow()
{
	// Only useful for monthly data.
	if (IsMonthly(Q)) {
		int nyears = GetNYears();
		for (int iyear=0; iyear<nyears; iyear++) {
			for (int imonth=0; imonth<12; imonth++) {
				//double days = getDaysInMonth(imonth, GetStartYear() + iyear);

				// New try: use average days in month.  This will cause the modified form to conform to the model.
				double days = 30.41667;

				double val = GetValueDate(ItemData::CALENDAR, ItemData::MONTHLY, Q, iyear, imonth);
				if (val > SitesManager::NoData)
				{
					val /= days;
					SetValueDate(ItemData::CALENDAR, Q, val / (1440/325851.0), iyear, imonth);
				}
			}
		}
	}
}


double
PumpingRecTD::GetValue(int whichItem, int year, int period)
{
	if (whichItem == Volume) {
		double q = TimeData::GetValue(Q, year, period);
		if (q <= SitesManager::NoData) return SitesManager::NoData;
		else {
			double delta = TimeData::GetValue(Delta, year, period);
			if (delta < 1) delta = 1;
			return q * delta;
		}
	}
	else {
		return TimeData::GetValue(whichItem, year, period);
	}
}

double
PumpingRecTD::GetValue(int whichItem, int period)
{
	if (whichItem == Volume) {
		double q = TimeData::GetValue(Q, period);
		if (q <= SitesManager::NoData) return SitesManager::NoData;
		else {
			double delta = TimeData::GetValue(Delta, period);
			if (delta < 1) delta = 1;
			return q * delta;
		}
	}
	else {
		return TimeData::GetValue(whichItem, period); 
	}
}

double
PumpingRecTD::GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichItem, int year, int month, int day)
{
	if (whichItem == Volume) {
		double q = TimeData::GetValueDate(yearType, timemode, Q, year, month, day);
		if (q > SitesManager::NoData)
		{
			double days(1);
			if (day < 0) {
				// convert monthly -> daily
				// 	if (year > 1000) {
				// 	  days = getDaysInMonth(month, year, yearType);
				// 	}
				// 	else {
				// 	  days = getDaysInMonth(month, GetStartYear() + year, yearType);
				// 	}

				// New try: use average days in month.  This will cause the modified form to conform to the model.
				days = 30.41667;
			}

			// Convert GPM to Ac-Ft
			return q * days * 1440/325851.0;
		}
		else {
			return q;
		}
	}
	else {
		return TimeData::GetValueDate(yearType, timemode, whichItem, year, month, day);
	}
}

void
PumpingRecTD::SetValue(int whichitem, int year, int period, double val)
{
	if (whichitem == Volume) {
		if (val != SitesManager::NoData) val = val / GetValue(Delta, year, period);
		TimeData::SetValue(Q, year, period, val);
	}
	else {
		TimeData::SetValue(whichitem, year, period, val);
	}
}

void
PumpingRecTD::SetValue(int whichitem, int period, double val)
{
	if (whichitem == Volume) {
		if (val != SitesManager::NoData) val = val / GetValue(Delta, period);
		TimeData::SetValue(Q, period, val);
	}
	else {
		TimeData::SetValue(whichitem, period, val);
	}
}

void
PumpingRecTD::SetValueDate(ItemData::YearTypeEnum yearType, int whichitem, double val, int year, int month, int day)
{
	if (whichitem == Volume) {
		if (day < 0 && val != SitesManager::NoData) {
			//       int days = 0;
			//       if (year > 1000) {
			// 	days = getDaysInMonth(month, year, yearType);
			//       }
			//       else {
			// 	days = getDaysInMonth(month, GetStartYear() + year, yearType);
			//       }

			// New try: use average days in month.  This will cause the modified form to conform to the model.
			double days = 30.41667;

			val /= days;	// convert daily -> monthly
		}

		// Convert from ac-ft to gpm.  Ignore delta and just assume that it's 1.
		if (val != SitesManager::NoData) val = val / (1440/325851.0);
		TimeData::SetValueDate(yearType, Q, val, year, month, day);
	}
	else {
		TimeData::SetValueDate(yearType, whichitem, val, year, month, day);
	}
}

void
PumpingRecTD::SetConversionFactor(double cf)
{
	m_cf = cf;
}

//###################### WellPumpingTD #######################//

WellPumpingTD::WellPumpingTD()
: TimeData(NumWellPumpingTD)
{
}

WellPumpingTD::WellPumpingTD(const WellPumpingTD& rhs)
: TimeData(rhs)
{
}

WellPumpingTD&
WellPumpingTD::operator=(const WellPumpingTD& rhs)
{
	if (&rhs == this) return *this;

	TimeData::operator=(rhs);

	return *this;
}

double
WellPumpingTD::GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichItem, int year, int month, int day)
{
	if (whichItem == Volume) {
		int ndays = 1;
		if (day < 0) {
			// Calculate number of days this month.
			ndays = getDaysInMonth(month, year, yearType);
		}
		// Native units are Ac-Ft.
		double cv(1);
		switch (m_pumpingUnits) {
	case GPM:
		cv = 1440/325851.0 * ndays; // daily GPM -> AcFt(day)
	case CFS:
		cv = 1.9835 * ndays; // CFS -> Ac-Ft (day)
		break;
	case Acft:
		cv = 1.0;
		//cv = 325851.0/1440; // AcFt -> daily GPM 
		//cv = 325900.0/1440; // AcFt -> daily GPM 
		break;
		}

		double on = GetValueDate(yearType, timemode, On, year, month, day);
		if (on <= 0 || m_pumpingUnits == Acft) on = 1;

		double q = GetValueDate(yearType, timemode, Q, year, month, day);
		double app_eff = GetValueDate(yearType, ItemData::MONTHLY, AppEff, year, month);
		if (app_eff < 0) app_eff = 0;

		return  q * on * app_eff * cv;
	}
	else {
		return TimeData::GetValueDate(yearType, timemode, whichItem, year, month, day);
	}
}

string
WellPumpingTD::GetPumpingUnitsStr()
{
	switch (m_pumpingUnits) {
  case CFS: return "CFS";
  case GPM: return "GPM";
  case Acft: return "Ac-Ft";
	}

	return "";
}

ifstream&
operator>>(ifstream& istr, WellPumpingTD& wpm)
{

	if (istr.getline(buf, sizeof(buf), '=') == NULL) {
		throw InputError("Parse error looking for 'well pumping recs (daily/monthly)='");
	}

	// See if units are specified.
	if (strstr(buf, "well pumping units") != NULL) {
		int units;
		istr >> units;
		wpm.m_pumpingUnits = WellPumpingTD::wellUnitsEnum(units);

		if (istr.getline(buf, sizeof(buf), '=') == NULL) {
			throw InputError("Parse error looking for 'well pumping recs (daily/monthly)='");
		}
	}

	if (strstr(buf, "well pumping recs (daily/monthly)") == NULL) {
		throw InputError("Parse error looking for 'well pumping recs (daily/monthly)=' header");
	}

	int nrec;
	istr >> nrec;

	int irec;
	for (irec=0; irec<nrec; irec++) {
		double q;
		int on;

		if (istr.getline(buf, sizeof(buf), '=') == NULL) {
			throw InputError("Parse error looking for 'q, is on='");
		}

		if (strstr(buf, "q, is on") == NULL) {
			throw InputError("Parse error looking for 'q, is on='");
		}

		istr >> q >> on;

		wpm.SetValue(WellPumpingTD::Q, irec, q);
		wpm.SetValue(WellPumpingTD::On, irec, on);
	}

	if (istr.getline(buf, sizeof(buf), '=') == NULL) {
		throw InputError("Parse error looking for 'well pumping recs (monthly)='");
	}
	if (strstr(buf, "well pumping recs (monthly)") == NULL) {
		throw InputError("Parse error looking for 'well pumping recs (monthly)=' header");
	}

	istr >> nrec;

	for (irec=0; irec<nrec; irec++) {
		double app_eff;

		if (istr.getline(buf, sizeof(buf), '=') == NULL) {
			throw InputError("Parse error looking for 'app efficiency='");
		}

		if (strstr(buf, "app efficiency") == NULL) {
			throw InputError("Parse error looking for 'app efficiency='");
		}

		istr >> app_eff;
		wpm.SetValue(WellPumpingTD::AppEff, irec, app_eff);
	}

	return istr;
}

ofstream&
operator<<(ofstream& ostr, WellPumpingTD& wpm)
{
	ostr << "well pumping units=" << wpm.m_pumpingUnits << endl;
	ostr << "well pumping recs (daily/monthly)=" << wpm.GetNPeriods(WellPumpingTD::Q) << endl;
	int iyear;
	for (iyear=0; iyear<wpm.GetNYears(); iyear++) {
		// This nrecs is the number of periods in the current year.
		int nrecs = wpm.GetNPeriods(WellPumpingTD::Q, iyear);
		for (int iday=0; iday<nrecs; iday++) {
			ostr << "q, is on=" << wpm.GetValue(WellPumpingTD::Q, iyear, iday) << " " << wpm.GetValue(WellPumpingTD::On, iyear, iday) << endl;
		}
	}
	ostr << "well pumping recs (monthly)=" << wpm.GetNPeriods(WellPumpingTD::AppEff) << endl;
	for (iyear=0; iyear<wpm.GetNYears(); iyear++) {
		// This nrecs is the number of periods in the current year.
		int nrecs = wpm.GetNPeriods(WellPumpingTD::AppEff, iyear);
		for (int iday=0; iday<nrecs; iday++) {
			ostr << "app efficiency=" << wpm.GetValue(WellPumpingTD::AppEff, iyear, iday) << endl;
		}
	}
	return ostr;
}


//###################### RechargeTD #######################//

RechargeTD::RechargeTD()
: TimeData(NumRechargeTD)
{
}

RechargeTD::RechargeTD(const RechargeTD& rhs)
: TimeData(rhs)
{
}

RechargeTD&
RechargeTD::operator=(const RechargeTD& rhs)
{
	if (&rhs == this) return *this;

	TimeData::operator=(rhs);

	return *this;
}

double
RechargeTD::GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichItem, int year, int month, int day)
{
	if (whichItem == Volume) {
		double i_EOM = m_i_EOM;
		double cv = 325851.0/1440; // AcFt -> daily GPM 
		//double cv = 325900.0/1440; // AcFt -> daily GPM 

		if (month > 0) {
			i_EOM = TimeData::GetValueDate(yearType, timemode, EOMstorage, year, month);
		}

		if (i_EOM >= 0) {
			double flow = TimeData::GetValueDate(yearType, timemode, Inflow, year, month, day) - TimeData::GetValueDate(yearType, timemode, Outflow, year, month, day) - (TimeData::GetValueDate(yearType, timemode, EOMstorage, year, month) - i_EOM);
			double evap = TimeData::GetValueDate(yearType, timemode, Acres, year, month) * TimeData::GetValueDate(yearType, timemode, PondEvap, year, month) / 12.0;

			int daysInMonth = getDaysInMonth(month, (*this)[Inflow].m_startyear + year, yearType);

			if (flow >= 0 && evap >= 0) {
				evap *= TimeData::GetValueDate(yearType, timemode, DaysEvap, year, month) / daysInMonth;
				if (evap >= 0) {
					return (flow - evap) * cv;
				}
			}
		}
	}
	else {
		return TimeData::GetValueDate(yearType, timemode, whichItem, year, month, day);
	}

	return SitesManager::NoData;
}

ifstream&
operator>>(ifstream& istr, RechargeTD& rm)
{
	if (istr.getline(buf, sizeof(buf), '=') == NULL) {
		throw InputError("Parse error looking for 'recharge recs (daily/monthly)='");
	}

	if (strstr(buf, "recharge recs (daily/monthly)") == NULL) {
		throw InputError("Parse error looking for 'recharge recs (daily/monthly)=' header");
	}

	int nrec;
	istr >> nrec;

	int irec;
	for (irec=0; irec<nrec; irec++) {
		double inflows, outflows;

		if (istr.getline(buf, sizeof(buf), '=') == NULL) {
			throw InputError("Parse error looking for 'inflows, outflows='");
		}

		if (strstr(buf, "inflows, outflows") == NULL) {
			throw InputError("Parse error looking for 'inflows, outflows='");
		}

		istr >> inflows >> outflows;

		rm.SetValue(RechargeTD::Inflow, irec, inflows);
		rm.SetValue(RechargeTD::Outflow, irec, outflows);
	}

	if (istr.getline(buf, sizeof(buf), '=') == NULL) {
		throw InputError("Parse error looking for 'recharge recs (monthly)='");
	}
	if (strstr(buf, "recharge recs (monthly)") == NULL) {
		throw InputError("Parse error looking for 'recharge recs (monthly)=' header");
	}

	istr >> nrec;

	for (irec=0; irec<nrec; irec++) {
		double EOMstorage, acres, pond_evap, days_evap;

		if (istr.getline(buf, sizeof(buf), '=') == NULL) {
			throw InputError("Parse error looking for 'EOMstorage, acres, pond_evap, days_evap='");
		}

		if (strstr(buf, "EOMstorage, acres, pond_evap, days_evap") == NULL) {
			throw InputError("Parse error looking for 'EOMstorage, acres, pond_evap, days_evap='");
		}

		istr >> EOMstorage >> acres >> pond_evap >> days_evap;

		rm.SetValue(RechargeTD::EOMstorage, irec, EOMstorage);
		rm.SetValue(RechargeTD::Acres, irec, acres);
		rm.SetValue(RechargeTD::PondEvap, irec, pond_evap);
		rm.SetValue(RechargeTD::DaysEvap, irec, days_evap);
	}

	return istr;
}

ofstream&
operator<<(ofstream& ostr, RechargeTD& rm)
{
	ostr << "recharge recs (daily/monthly)=" << rm.GetNPeriods(RechargeTD::Inflow) << endl;
	int iyear;
	for (iyear=0; iyear<rm.GetNYears(); iyear++) {
		// This nrecs is the number of periods in the current year.
		int nrecs = rm.GetNPeriods(RechargeTD::Inflow, iyear);
		for (int iday=0; iday<nrecs; iday++) {
			ostr << "inflows, outflows=" << rm.GetValue(RechargeTD::Inflow, iyear, iday) << " " << rm.GetValue(RechargeTD::Outflow, iyear, iday) << endl;
		}
	}
	ostr << "recharge recs (monthly)=" << rm.GetNPeriods(RechargeTD::EOMstorage) << endl;
	for (iyear=0; iyear<rm.GetNYears(); iyear++) {
		// This nrecs is the number of periods in the current year.
		int nrecs = rm.GetNPeriods(RechargeTD::EOMstorage, iyear);
		for (int iday=0; iday<nrecs; iday++) {
			ostr << "EOMstorage, acres, pond_evap, days_evap="
				<< rm.GetValue(RechargeTD::EOMstorage, iyear, iday) << " "
				<< rm.GetValue(RechargeTD::Acres, iyear, iday) << " "
				<< rm.GetValue(RechargeTD::PondEvap, iyear, iday) << " "
				<< rm.GetValue(RechargeTD::DaysEvap, iyear, iday) << endl;
		}
	}

	return ostr;
}




//###################### MetaDataTD #######################//

MetaDataTD::MetaDataTD()
: TimeData(NumMetaDataTD)
{
}

MetaDataTD::MetaDataTD(const MetaDataTD& rhs)
: TimeData(rhs)
{
}

MetaDataTD&
MetaDataTD::operator=(const MetaDataTD& rhs)
{
	if (&rhs == this) return *this;

	TimeData::operator=(rhs);

	return *this;
}

OutputDataTD::OutputDataTD()
: TimeData(NumOutputDataTD)
{
}

OutputDataTD::OutputDataTD(const OutputDataTD& rhs)
: TimeData(rhs)
{
}





//######################### OutputRec  ########################//



OutputRec::OutputRec()
:m_time(0), m_depRate(0), m_depVol(0), m_depVolThis(0)
{
}

OutputRec::OutputRec(const OutputRec &rhs)
{
	m_time = rhs.m_time;
	m_depRate = rhs.m_depRate;
	m_depRatePerc = rhs.m_depRatePerc;
	m_depVolThis = rhs.m_depVolThis;
	m_depVol = rhs.m_depVol;
}

OutputRec&
OutputRec::operator=(const OutputRec &rhs)
{
	if (&rhs == this) return *this;

	m_time = rhs.m_time;
	m_depRate = rhs.m_depRate;
	m_depRatePerc = rhs.m_depRatePerc;
	m_depVolThis = rhs.m_depVolThis;
	m_depVol = rhs.m_depVol;

	return *this;
}
