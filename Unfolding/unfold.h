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
  TH1* get_output_check();
  TH1* get_output_check_all();
  TH1* get_output();
  TH1* get_output_all();
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
  TH1 *h_check, *h_check_all;
  TH1 *h_data_output, *h_data_output_all;
  TSpline *rhoLogTau;
  TSpline *rhoLogTau_check;
  TGraph *l_curve_check;

  TGraph *l_curve = 0;
  TSpline *logTauX = 0;
  TSpline *logTauY = 0;

  TH2 *h_data_rho, *h_data_rho_all;
  TH2D *response_matrix;
  TH1D *hist_truth, *hist_mc;
  std::vector<double> coords;
  std::vector<TSpline*> logTau;
  double tau;
};
