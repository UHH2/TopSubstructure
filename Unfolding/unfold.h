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

class unfolding{
public:
  unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response, TH1D* h_truth, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<TH1D*> background, std::vector<TString> background_names, int nscan = 100, TString regmode_ = "curvature", TString density_flag = "none", bool do_lcurve = true, bool subtract_background = true, double tau_value = -1);
  // void get_output_check();
  // void get_output_check_all();
  // void get_output();
  // void get_output_all();
  // void get_input_statcov();
  // void get_input_statcov_all();
  // void get_matrix_statcov();
  // void get_matrix_statcov_all();
  // void get_total_statcov();
  // void get_total_statcov_all();
  // void get_lcurve();
  // void get_correlation();
  // void get_correlation_all();
  // void get_coordinates();
  // void get_logtau();
  // void get_rhologtau();
  // void get_tau();
  // void check_projection();
  TH1* get_output_check();
  TH1* get_output_check_all();
  TH1* get_output();
  TH1* get_output_all();
  TH2* get_input_statcov();
  TH2* get_input_statcov_all();
  TH2* get_matrix_statcov();
  TH2* get_matrix_statcov_all();
  TH2* get_total_statcov();
  TH2* get_total_statcov_all();
  TGraph* get_lcurve();
  TH2* get_correlation();
  TH2* get_correlation_all();
  std::vector<double> get_coordinates();
  std::vector<TSpline*> get_logtau();
  TSpline* get_rhologtau();
  double get_tau();
  std::vector<TH1D*> check_projection();

private:
  TUnfold::ERegMode regMode;
  TUnfoldDensity::EDensityMode densityFlags;
  TH1 *h_check;
  TH1 *h_check_all;
  TH1 *h_data_output;
  TH1 *h_data_output_all;
  TSpline *rhoLogTau;
  TSpline *rhoLogTau_check;
  TGraph *l_curve_check;

  TGraph *l_curve = 0;
  TSpline *logTauX = 0;
  TSpline *logTauY = 0;

  TH2 *h_data_rho, *h_data_rho_all;
  TH2D *response_matrix;
  TH2 *h_covarianceinputstat, *h_covarianceinputstat_all, *h_covartiancematrixstat, *h_covartiancematrixstat_all, *h_covariancetotal, *h_covariancetotal_all;
  TH1D *hist_truth, *hist_mc;
  std::vector<double> coords;
  std::vector<TSpline*> logTau;
  double tau;
};
