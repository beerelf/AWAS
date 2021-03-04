
// *** WARNING: this is copied from the SDFdaily project.  DO not edit this!!

#ifndef _SDFDATA_H
#define _SDFDATA_H

#include <vector>
#include <string>
#include <fstream>

#include "utils.h"
#include "DataClass.h"
#include "CustOutputManager.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class OutputRec {
public:
  OutputRec();
  OutputRec(const OutputRec&);
  OutputRec& operator=(const OutputRec&);

  double m_time;		// time period of record
  double m_depRate;		// depletion rate (GPM)
  double m_depRatePerc;		// depletion rate Percent
  double m_depVol;		// cumulative volume of depletion (GPM).
  double m_depVolPerc;		// cumulative volume of depletion (%).
  double m_depVolThis;		// volume of depletion for this timestep.
};


// A class for representing a pumping record.
class PumpingRecTD : public TimeData {
public:
  enum PumpingDataEnum {
    Q /*pumping rate*/, Delta /* span of time*/, NumPumpingRecTD, Volume
  };

  PumpingRecTD();
  PumpingRecTD(const PumpingRecTD&);
  PumpingRecTD& operator=(const PumpingRecTD&);

  //! Convert flows into a volume.  This is useful when you need to work with montly volumes of water.
  void ConvertToVolume();
  
  //! Unconvert volumes back into flows.
  void UnconvertFromVolume();

  void ConvertToFlow();

  // TimeData overrides.
  virtual double GetValue(int whichitem, int year, int period);
  virtual double GetValue(int whichitem, int period);
  virtual double GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichitem, int year, int month=-1, int day=-1);
  virtual void SetValue(int whichitem, int year, int period, double val);
  virtual void SetValue(int whichitem, int period, double val);
  virtual void SetValueDate(ItemData::YearTypeEnum yearType, int whichitem, double val, int year, int month=-1, int day=-1);
  void SetConversionFactor(double cf);

  // Should this be static in order to allow consistent units for all sites?
  double m_cf;	     // the conversion factor to use when calculating volume

};

class WellPumpingTD : public TimeData {
public:
  enum WellDataEnum {
    Q /* pump rate */, AppEff, On /*if 1, then pumping is on (for GPM and CFS)*/, NumWellPumpingTD, Volume /* rate * On * AppEff */
  };

  WellPumpingTD();
  WellPumpingTD(const WellPumpingTD&);
  WellPumpingTD& operator=(const WellPumpingTD&);

  friend ifstream& operator>>(ifstream& istr, WellPumpingTD& wpm);
  friend ofstream& operator<<(ofstream& ostr, WellPumpingTD& wpm);

  virtual double GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichitem, int year, int month=-1, int day=-1);

  typedef enum { CFS, GPM, Acft } wellUnitsEnum;

  static wellUnitsEnum m_pumpingUnits;	// Units to use for pumping records (these will
				//   always get converted to AcFt).

  // Returns "CFS", "GPM", or "Ac-Ft"
  string GetPumpingUnitsStr();
};

class RechargeTD : public TimeData {
public:
  enum RechargeDataEnum {
    Inflow, Outflow, EOMstorage, Acres, PondEvap, DaysEvap, NumRechargeTD, Volume
  };

  RechargeTD();
  RechargeTD(const RechargeTD&);
  RechargeTD& operator=(const RechargeTD&);

  friend ifstream& operator>>(ifstream& istr, RechargeTD& rm);
  friend ofstream& operator<<(ofstream& ostr, RechargeTD& rm);

  virtual double GetValueDate(ItemData::YearTypeEnum yearType, ItemData::TimeEnum timemode, int whichitem, int year, int month=-1, int day=-1);

  double m_i_EOM;		// initial end-of-month storage.

  double Calculate(int irec, int month_irec, int year, int month);
};

// A class for representing how volume was calculated at this time period.
class MetaDataTD : public TimeData {
public:
  enum MetaDataEnum {
    MetaData, NumMetaDataTD
  };

  enum MetaDataTypeEnum {USER_CALC, PUMP_REC};

  MetaDataTD();
  MetaDataTD(const MetaDataTD&);
  MetaDataTD& operator=(const MetaDataTD&);
};

//! Storage of output.
class OutputDataTD : public TimeData {
public:
  enum OutputDataEnum {
    Diversion, Depletion, NumOutputDataTD // If positive, then consider these recharge & accretion
  };

  OutputDataTD();
  OutputDataTD(const OutputDataTD&);
};

class SitesManager;

// A class for storing information for an SDF site.
class SiteData {
public:
  typedef enum TimeUnitsEnum {DAYS, WEEKS, MONTHS, YEARS};

  SiteData();
  SiteData(const SiteData&);
  SiteData& operator=(const SiteData&);
  boolean operator<(const SiteData&);

  //! Fill in values from the rhs into this.
  void UpdateFrom(const SiteData& rhs);

  void SetTimeUnits(TimeUnitsEnum newUnits);
  //! Only use this if in modified mode.
  void SetPeriod(int startYear, int endYear);

  // Fill in output members based on input members.  If using modified
  //  monthly, then useAvgDaily flag will convert monthly pumping to average
  //  daily pumping.  npa_prime will have the number of days in the period,
  //  which will replace npa, the number of months in period.
  typedef void (SiteData::*errorFuncPtr)(double u, double *bqq, double *svv);
  int Calculate(bool useAvgDaily, errorFuncPtr, int displayMode);

  // Computes length of pumping periods and number of timesteps (line 3000)
  int CalcNumPumpPeriods(int *dela);
  
  int CalcMinPumpPeriod();

  // This is subroutine 5490
  void CalcErrorFunc(double u, double *bqq, double *svv);
  //! Kenny Fritzler's modification.
  void CalcErrorFuncNew(double u, double *bqq, double *svv);
  
  // Subroutine 7130
  double CalcStreamQ(double l1, double l2, double yy, double u);

  // Subroutine 7300
  double CalcStreamV(double qn, double ql, double qp, double dela);

  // Save output results to ostr.
  void WriteOutput(ofstream &ostr);
  string WriteOutput();

  //! Read results from file rather than using execute function.
  //! npa is the number of timeperiods in the output if using modified mode.
  string ReadOutput(ifstream &istr, int npa=-1);

  //! Functions for adding blocks of records
  void AddRecords(int loc, int nrec);
  void RemoveRecords(vector<int> &recVector);

  //****************** Input parameters ******************//
  string m_header1;
  string m_header2;

  typedef enum BoundaryConditionsEnum {INFINITE_AQUIFER, ALLUVIAL_AQUIFER, NO_FLOW, EFFECTIVE_SDF, URF};
  BoundaryConditionsEnum m_bi;

  bool m_computeDepletionForSegment;

  double m_tr;			// transmissivity
  double m_s;			// specific yield
  double m_dxx;			// distance from the well to the stream.

  // number of days in selected time unit.
  double m_fact;

  // number of time units between printouts (if > 0 pfot is true)
  int m_tbp;

  // number of cycles (seasons) to be simulated
  int m_nc;

  // pumping record storage for a single cycle (nppc).
  PumpingRecTD m_pumpingRecTD;

  // Running total of volume (if cycles > 1, size = m_ppc * m_nc; otherwise
  //   the size is just the number of periods).
  vector<double> m_avp;

  // Time period of the given interval.
  vector<double> m_delt;

  // Q of the given interval.
  vector<double> m_q;

  // Running total of time (if cycles > 1, size = m_ppc * m_nc; otherwise
  //   the size is just the number of periods).
  vector<double> m_zzat;

  // Volume of the given interval
  vector<double> m_vptp;

  int m_tt;			// the total time of the simulation
				//   (sum of the pumping record intervals)

  // effective SDF members:
  double m_sdf;

  // Alluvial Aquifer members:
  double m_w;// distance from the stream to the parallel impermeable boundary.
  
  // No flow boundary members.
  double m_b;

  // Members for computinmg depletion for a segment of the stream
  bool m_zzseg;			// if TRUE, then calc depletion for segment
				//   of stream
  double m_z1, m_z2;

  //****************** Output parameters ******************//
  string m_vunits;

  // Dimension is npa
  vector<double> m_qq;
  vector<double> m_vv;

  vector<double> m_qd;
  vector<double> m_vd;

  // Output members (rather than printing to screen, results are stored here).
  vector<OutputRec> m_outputRecArray;

  TimeUnitsEnum m_timeUnits;

  static char* m_timeUnitsStr[4];

  // Extras -- pumping records and recharge parameters
  typedef enum SiteTypeEnum {WELL, RECHARGE};
  SiteTypeEnum m_siteType;
  void SetSiteType(SiteTypeEnum newType);

  WellPumpingTD m_wellPumpingTD;
  RechargeTD m_rechargeTD;

  // meta data relating to how the volume at a particular record was
  //   calculated.
  MetaDataTD m_metaDataTD;

  typedef enum volumeUnitsEnum {GALLONS, ACFT};
  void SetOutputUnits(volumeUnitsEnum units);

  //! Storage of output.
  OutputDataTD m_outputTD;

  // If using monthly and modified format, use this vector to get the number
  //   of days in the current time period.
  static vector<int> daysPerMonth;
  // If the size of daysPerMonth > 0, then return daysPerMonth; otherwise
  //   return constant time factor according to time mode.
  double GetFact(int iper, bool useActualDays=false);

  // Synthesize data.
  void Forecast(int startYear, int m_histEndYear, int endYear, int forecast_mode, vector<int> *yearList);

  string GetPumpingUnitsStr();

  //! Controls whether to not include the site in output.

  //! Now we have several criteria that can be used for determining whether
  //! to process the site in the model.
  vector<bool> m_outputCols; //! analogous to CustOutputManager.m_activeCols

  //! Decide if this site should be displayed in output.
  /*!
    \param com The custom output manager for the project.
    \return True if the site should be displayed.
  */
  bool ShowInOutput(CustOutputManager &com);

  //! Remove values that fall outside the year range and calendar type.
  void CleanData(ItemData::YearTypeEnum yearType);


  //! URF data.  Allow the user to specify different URFs depending on time period.
  vector<double> m_URF_MonthlyData;
  vector<double> m_URF_DailyData;

  //! This will be the URF values that actually get fed into the calculation so that monthly values
  //   can get converted into daily timesteps.
  vector<double> m_URF_DataCalc;
};



class SummaryOutputDataTD : public TimeData {
public:
  enum SummaryOutputDataEnum {
    Total_Sel_div/* total of selection*/, Total_Sel_dep/* total of selection*/, Diversion, Depletion, Recharge, Accretion, Net, NumSummaryOutputDataTD
  };

  SummaryOutputDataTD();
  SummaryOutputDataTD(const SummaryOutputDataTD&);
};



class SummaryCustomOutputDataTD : public TimeData {
public:
  enum SummaryCustomOutputDataEnum {
    Diversion, Depletion, NumSummaryCustomOutputDataTD
  };

  SummaryCustomOutputDataTD();
  SummaryCustomOutputDataTD(const SummaryCustomOutputDataTD&);
};




class SitesManager {
public:
  SitesManager();

  static float m_currentVersion;

  string ReadProject(string fname, bool ignoreOutput=false, bool checkForYearType=true, bool addInPlace=false);
  //! if checkForYearType is false, then use the supplied year type.
  string ImportProject(string fname, ItemData::YearTypeEnum yearType, bool addInPlace=false, bool checkForYearType=false);
  //! Import from spreadsheet using a new format that will load a complete dataset.  The old format is limited in forecasting and others.
  string ImportProjectNew(string fname, bool addInPlace=FALSE, bool checkForYearType=false);
  string ImportSDFView(string fname, bool addInPlace=false);
  //! Add or overwrite a site.
  bool ImportSite(string name, string descrip, double w, double b, double sdf, double sy, double trans, double x);
  string ReadOutput(string fname);
  void ClearOutput();
  //! Return the period of record of the output adjusted by the simulation starting month.  For example, if the year type is irrigation and the start month is 3 (January), then the simulation start year will actually be the year after the simulation starting year because Nov and Dec are not needed.
  /*!
    \param startYear The start year of the output period of record.
    \param startMonth The start month of the output period of record.
    \param endYear The end year of the output period of record.
    \param endMonth The end month of the output period of record.
  */
  void GetOutputPeriod(int &startYear, int &startMonth, int &endYear, int &ignoreYear, int &ignoreMonth);
  string Execute();		// write to baseName + .dsd
  string WriteProject(string fname, bool outputSettingsOnly=false); // Execute will generate output file???

  //! Save URF data to file.
  bool WriteURF_Data(string fname);
  bool ReadURF_Data(string fname, float version);

  //! Clear out values in date variables that are out of bounds when the year type is non-calendar.
  void FixOutOfBoundsData();

  // Set to true if there are sites that contain daily URFs and monthly URFs.
  void CheckURFs(BOOL& hasDailyURFSites, BOOL& hasMonthlyURFSites);

  //! Return index of site, -1 if not found.
  int FindSite(string name);
  //! add an empty well to the project
  int AddSite(string h1, string h2);

  void DelSite(int idx);	// delete the idx_th well.
  SiteData::TimeUnitsEnum GetTimeUnits();
  void SetTimeUnits(SiteData::TimeUnitsEnum newUnits);
  //! Conversion to daily or monthly needs to adjust for actual days in the month.
  void AdjustPumping(SiteData::TimeUnitsEnum newUnits);
  void AdjustPumping(SiteData &sd, SiteData::TimeUnitsEnum newUnits);

  bool SetPeriod(int startYear, int endYear, int newSitePos=-1, int forecast_mode=-1, vector<int> *yearList=NULL, int prec=-1);
  bool AddRecords(int nrecs);	// add nrecs to end of every site's record list.
				// return TRUE if period of rec is changed
  string m_baseName;		// assigned in ReadProject
  vector<SiteData> m_siteList;

  void SetOutputUnits(SiteData::volumeUnitsEnum units);

  double GetSiteData(bool *usesPumpingRec, int whichSiteIndex, int whichDatum, int year, int month, int day=-1);
  bool SetSiteData(double dval, double initialEOMstorage, int whichSiteIndex, int whichDatum, int year, int month, int day=-1);

  //! Set the units used to convert pumping records to volume.
  void SetPumpingUnits(WellPumpingTD::wellUnitsEnum);

  //! Return true if at least one site has a URF boundary condition.
  bool HasSitesWithURF();

  // Period of record
  int m_startYear;
  int m_endYear;

  // Projected data goes from histEndYear+1 to endYear.  This way there
  //    is minimal impact to existing code.
  int m_histEndYear;
  int m_histStartYear;

  //! Start and end of simulation
  int m_simStartYear;
  int m_simEndYear;
  int m_simStartMonth;

  //! Zero out pumping data after this year.
  bool m_useIgnoreYear;
  int m_runIgnoreMonth;
  int m_runIgnoreYear;

  // How the projected data was synthesized.
  string m_synthDataLabel;
  string m_preSynthDataLabel;

  //! Years used in forecasting
  vector<int> m_yearList;
  vector<int> m_preYearList;

  //! How data was forecasted
  ItemData::ForecastEnum m_forecastMode;
  ItemData::ForecastEnum m_preForecastMode;

  SiteData::TimeUnitsEnum m_timeUnits;
  
  // Set year type.  Native format will always be calendar, but can
  //   be displayed in irrigation or USGS.
  typedef enum YearModeEnum {YEAR_CALENDAR, YEAR_IRRIGATION, YEAR_USGS};
  YearModeEnum m_yearMode;

  void SetYearMode(YearModeEnum);

  string GetMonthName(int m);
  int GetMonthIdx(int m);
  //! Adjust year if the year type is not calendar when printing
  int GetYear(int m, int y);

  //! Return the start year of the historical period of record adjusted for the year type.
  int GetHistStartYear();
  //! Return the end year of the historical period of record adjusted for the year type.
  int GetHistEndYear();

  //! Return the start year of the period of record adjusted for the year type.  Thus irrigation year 1993 will appear to the user as 1993 even though the actual start year is 1992.
  int GetStartYear();
  //! Return the end year of the period of record adjusted for the year type.
  int GetEndYear();

  //! Return the start year of the output period of record adjusted for the year type.  Thus irrigation year 1993 will appear to the user as 1993 even though the actual start year is 1992.
  int GetSimStartYear();
  //! Return the end year of the output period of record adjusted for the year type.
  int GetSimEndYear();

  //! Get the simulation start month adjusted by year format.
  int GetSimStartMonth();
  //! Get the simulation end month adjusted by year format.
  int GetRunIgnoreMonth();

  //! Set the simulation start month.
  void SetSimStartMonth(int simStartMonth);
  //! Set the simulation ignore after month.
  void SetIgnoreMonth(int ignoreMonth);

  static int NoData;

  typedef enum DisplayModeEnum {DISPLAY_ORIGINAL, DISPLAY_MODIFIED};
  DisplayModeEnum m_displayMode;

  double GetOutputData(int whichSite, int timeMode, int whichType, int prec, int year, int month, int day);

  //! Because sites can be set to not show up in output, use this funtion to get the ith output site.
  SiteData& GetOutputSite(int whichSite);

  //! Because sites can be set to not show up in output, use this funtion to get where the site falls in m_siteList.
  int GetOutputSiteIdx(int whichSite);

  //! Because sites can be set to not show up in output, use this funtion to get where the number of sites that are in the output.
  int GetOutputSiteCount();

  // For modified output, creates summaries of recharge and irrigation
  //   diversions and depletions.
  void BuildSummaryData();
  double GetSummaryOutputData(int whichSummary, int timeMode, int whichType, int prec, int year, int month, int day);
  //! Set summary time scale to daily (1) or monthly (0).
  void SetSummaryTimeMode(int mode);

  // Summary data
  SummaryOutputDataTD m_summaryTD;

  // Custom output totals
  vector<SummaryCustomOutputDataTD> m_summaryCustomTD;

  //! Initialize custom output data structure.
  void InitCustOutputData();

  //! Return the custom output total data.
  double GetCustomOutputData(int whichSite, int timeMode, int whichType, int prec, int year, int month, int day);

  //! Helper function to help simplify getting data values.  This
  //! will add the year type to the call to TimeData::GetValueDate.
  double GetValueDate(TimeData *td, ItemData::TimeEnum, int whichitem, int year, int month=-1, int day=-1);
  //! This form assumes the data's smallest (native) time mode.
  double GetValueDate(TimeData *td, int whichitem, int year, int month=-1, int day=-1);

  //! Helper function to help simplify setting data values.  This
  //! will add the year type to the call to TimeData::GetValueDate.
  void SetValueDate(TimeData *td, int whichitem, double val, int year, int month=-1, int day=-1);

  //! Until the model can run in a monthly timestep, the model will convert the input internally from monthly to average daily and generate daily output.  The output module must then convert the output back into a monthly timestep.
  bool m_convertOutputToMonths;

  //! In order to speed up processing, the model can be set to run using the average days in a month.
  bool m_useAverageDaysInMonth;

  // If true, then just treat monthly URF values as using a daily timestep.  Otherwise the monthly URF will
  //   be applied to every day in the month.
  bool m_useMonthlyURFForDaily;

  //! Background color to draw synthetic data in.
  COLORREF m_projDataColor;

  string CreateSynthLabel(int forecast_mode, vector<int> &yearList);

  //! Initialize members that control forecasting.
  void ResetForecasting();

  //! Multiply projected data by these values.  0 = Jan, 11 = December.
  double m_pMult[12];

  //! Sort sites by name.
  void Sort();

  //! Structures for limiting output by an arbitrary number of fields.
  CustOutputManager m_custOutput;

  //! Each SDFData site has m_activeCols that needs to be resized if customized
  //! columns have been added or removed.
  void UpdateCustOutput();

  //! If true, then copy operations will include the header of the table.
  bool m_copyIncludesHeader;

  //! If true, then use Kenny Fritzler's new error function.
  bool m_useNewErrorFunc;

  bool UseNewErrorFunc();
};


class InputError
{
protected:
  string m_errorMessage;

 public:
  InputError(string msg="") : m_errorMessage(msg)  {};
  ~InputError(){};
  string ErrorMsg() { return m_errorMessage; }
};

#endif
