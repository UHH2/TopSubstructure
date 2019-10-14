#include "iostream"
#include <TROOT.h>
#include "TString.h"
#include "THStack.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <cmath>
#include "TFile.h"
#include "TH2D.h"
#include "TUnfold.h"
#include "TUnfoldDensity.h"
#include "TGraph.h"
#include <TMath.h>
#include "TUnfoldBinningXML.h"
#include <TDOMParser.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TLine.h>
#include <TLatex.h>
#include <TGraphAsymmErrors.h>

class Plotter{
public:
  Plotter(TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec);
  void Plot_projections(TH1D* projection_gen, TH1D* h_input_gen, TH1D* projection_rec, TH1D* h_input_rec, TString directory);
  void Plot_output(TH1D* h_unfolded, TH1D* h_truth_, bool normalise, TString directory, TString save);
  void Plot_ResponseMatrix(TH2D* resp_matrix_, TString directory);
  void Plot_uncertainty(std::vector<TH1D*> error, std::vector<TString> error_name, TString directory);
  void Plot_result_with_uncertainty(TH1D* h_unfolded, TH1D* h_unfolded_uncertainty, TH1D* h_data_truth, TH1D* h_mc_truth, bool normalise, TString sample, TString directory);
  void Plot_correlation_matrix(TH2D* h_corr_matrix, TString save);
  void Plot_LogTau(TSpline* logTau_, double tau, double coordinate, TString save);
  void Plot_LCurve(TGraph* lcurve, std::vector<double> coordinates, std::vector<double> coordinates_2, TString save);
  void Plot_RhoLogTau(TSpline* rhologTau, double tau, TString save);
  void Plot_purity(TH1D* purity_same, TH1D* purity_all, TString save);
  void Plot_input(TH1D* data, TH1D* mc, TH1D* background, TString save);
  void Plot_input(TH1D* data, TH1D* mc, TString save);
  void Plot_covariance(TH2D* matrix, TString save);
  void Plot_delta(std::vector<std::vector<TH1D*>> delta, std::vector<std::vector<TString>> delta_name, TString directory);
  void Plot_all_pseudo(TH1D* pseudo1, TH1D* pseudo1_truth_1, TH1D* pseudo2, TH1D* pseudo2_truth_1, TH1D* pseudo3, TH1D* pseudo3_truth_1, bool cs, TString directory);
  void Plot_compatibility(TH1D* mu_dummy, TH1D* mu_sys_dummy, TH1D* ele_dummy, TH1D* ele_sys_dummy, TString directory);

private:
  const TUnfoldBinning *measurement_rec, *measurement_gen, *rec_pt_topjet_sideband, *gen_pt_topjet_sideband, *rec_mass_sideband, *gen_mass_sideband;
  double n_measurement_rec, n_measurement_gen, n_rec_pt_topjet, n_gen_pt_topjet, n_mass_rec, n_mass_gen, n_mass_split_rec, n_mass_split_gen, n_regions;
  double dim_measurement_rec, dim_measurement_gen, dim_rec_pt_topjet, dim_gen_pt_topjet, dim_mass_rec, dim_mass_gen;

};
