#include "iostream"
#include <TStyle.h>
#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TUnfold.h"
#include "TGraph.h"
#include <TMath.h>
#include "TUnfoldBinningXML.h"
#include <TDOMParser.h>
#include <TRandom3.h>
#include <TLegend.h>
#include "unfold.h"
#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>
#include <TMarker.h>
#include <sstream>
#include <TGaxis.h>

TH1 *unfolded_data, *unfolded_data_2, *unfolded_mc_test, *unfolded_mc_test_2;
TGraph *lcurve = 0;
TGraph *lcurve_2 = 0;
TSpline *rhologTau = 0;
TSpline *rhologTau_2 = 0;
double tau, tau_2;
std::vector<double> coordinates, coordinates_2;
std::vector<TSpline*> logTau;
std::vector<TSpline*> logTau_2;


TString binning_xml;
TDOMParser parser;
TUnfoldBinning *binning_rec, *binning_gen;
TH1D *h_data, *h_data_dist, *h_unfold, *h_truth, *h_truth_all, *h_mc, *h_mc_dist, *h_trutch_check, *h_trutch_check_all, *h_purity_all, *h_purity_samebin;
TH2D *mat_response;
TH2 *correlation_matrix, *correlation_matrix_2;

TH1 *unfolded_data_all, *unfolded_data_all_2;
TH2 *correlation_matrix_all, *correlation_matrix_all_2, *covariance_input, *covariance_input_all, *covariance_matrix, *covariance_matrix_all, *covariance_input_2, *covariance_input_all_2, *covariance_matrix_2, *covariance_matrix_all_2, *covariance_total, *covariance_total_all, *covariance_total_2, *covariance_total_all_2;
TH1 *unfolded_mc_test_all, *unfolded_mc_test_all_2;
std::vector<TH1D*> background;
std::vector<TString> background_names;
std::vector<std::vector<TH2*>> sys_matrix;
std::vector<std::vector<TString>> sys_name;
std::vector<std::vector<TH2*>> sys_cov;
std::vector<std::vector<TH1*>> sys_del;
std::vector<TH2*> CovBgrStat;
std::vector<TH2*> CovBgrScale;
std::vector<TH1*> bgr_delta;
TH2 *total_cov, *cov_sys, *CovStat;
TH1 *stat_delta, *stat_rel, *bias_;
std::vector<TH1*> sys_rel;
TH1 *sys_rel_total, *data_unfolded_sys;
bool subtract_background;
