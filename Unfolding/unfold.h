#include "iostream"
#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TUnfold.h"
#include "TUnfoldDensity.h"
#include "TGraph.h"
#include <TMath.h>
#include "TUnfoldBinningXML.h"
#include <TDOMParser.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TVectorD.h>

class unfolding{
public:
  unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response, TH1D* h_truth, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<std::vector<TH2*>> sys_matrix, std::vector<std::vector<TString>> sys_name, std::vector<TH1D*> background, std::vector<TString> background_names, int nscan = 100, TString regmode_ = "curvature", TString density_flag = "none", bool do_lcurve = true, bool subtract_background = true, double tau_value = -1);
  void get_output_check();
  void get_output_check_meas();
  void get_output_check_all();
  void get_output();
  void get_output_meas();
  void get_output_all();
  void get_probability_matrix();
  void get_input_statcov();
  void get_input_statcov_meas();
  void get_input_statcov_all();
  void get_matrix_statcov();
  void get_matrix_statcov_meas();
  void get_matrix_statcov_all();
  void get_total_statcov();
  void get_total_statcov_meas();
  void get_total_statcov_all();
  void get_lcurve();
  void get_correlation();
  void get_correlation_meas();
  void get_correlation_all();
  void get_coordinates();
  void get_logtau();
  void get_rhologtau();
  void get_tau();
  void check_projection();
  void get_sys_covariance();
  void get_sys_covariance_meas();
  void get_sys_covariance_all();
  void get_sys_delta();
  void get_sys_delta_meas();
  void get_sys_delta_all();
  void GetBgrStatCov();
  void GetBgrStatCov_meas();
  void GetBgrStatCov_all();
  void GetBgrScaleCov();
  void GetBgrScaleCov_meas();
  void GetBgrScaleCov_all();
  void get_bgr_delta();
  void get_bgr_delta_meas();
  void get_bgr_delta_all();
  void get_bias();
  TH2* CreateCovMatrixFromDelta(TH1*);

private:
  TUnfold::ERegMode regMode;
  TUnfoldDensity::EDensityMode densityFlags;
  std::vector<TH2*> CovBgrStat, CovBgrStat_meas, CovBgrStat_all;
  std::vector<TH1*> BgrDelta, BgrDelta_meas, BgrDelta_all;
  std::vector<TH2*> CovBgrScale, CovBgrScale_meas, CovBgrScale_all;
  std::vector<std::vector<TH1*>> sys_delta, sys_delta_meas, sys_delta_all;
  std::vector<std::vector<TH2*>> sys_covariance, sys_covariance_meas, sys_covariance_all;

  TVectorD x_value, y_value, tau;
  TH1 *h_check = 0, *h_check_meas = 0;
  TH1 *h_check_all = 0;
  TH1 *h_data_output = 0, *h_data_meas = 0;
  TH1 *h_data_output_all = 0;
  TSpline *rhoLogTau = 0;
  TSpline *rhoLogTau_check = 0;
  TGraph *l_curve_check = 0;

  TGraph *l_curve = 0;
  TSpline* logTauX = 0;
  TSpline* logTauY = 0;

  TH1* bias = 0;
  TH2  *h_data_rho = 0, *h_data_rho_meas = 0, *h_data_rho_all = 0;
  TH2  *h_prob_matrix = 0;
  TH2D *response_matrix = 0;
  TH2  *h_covarianceinputstat = 0, *h_covarianceinputstat_meas = 0, *h_covarianceinputstat_all = 0;
  TH2  *h_covartiancematrixstat = 0, *h_covartiancematrixstat_meas = 0, *h_covartiancematrixstat_all = 0;
  TH2  *h_covariancetotal = 0, *h_covariancetotal_meas = 0, *h_covariancetotal_all = 0;
  TH1D *hist_truth = 0, *hist_mc = 0;
  std::vector<TSpline*> logTau;
  double tau_result;
  bool lcurve;
};
