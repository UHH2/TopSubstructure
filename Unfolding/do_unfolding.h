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
#include "Plots.h"
#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>
#include <TMarker.h>
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
TFile *outputFile;
TString save_LCurve_Unfolding, save_LCurve_Unfolding_norm, save_LCurve_Unfolding_correlation, save_LCurve_TauX,  save_LCurve_TauY, save_TauScan_LCurve, save_TauScan_RhoLogTau, save_TauScan_Unfolding, save_TauScan_Unfolding_norm, save_TauScan_Unfolding_correlation, save_TauScan_crosscheck, save_LCurve_Unfolding_all, save_LCurve_Unfolding_norm_all, save_LCurve_Unfolding_correlation_all, save_LCurve_crosscheck_all, save_TauScan_Unfolding_all, save_TauScan_Unfolding_norm_all, save_TauScan_Unfolding_correlation_all, save_TauScan_crosscheck_all, save_LCurve_crosscheck, save_projection_x, save_projection_y, save_response_matrix, save_purity;
TDOMParser parser;
TUnfoldBinning *binning_rec, *binning_gen;
TH1D *h_data, *h_unfold, *h_truth, *h_truth_all, *h_mc, *h_trutch_check, *h_trutch_check_all, *h_purity_all, *h_purity_samebin;
TH2D *mat_response;
TH2 *correlation_matrix, *correlation_matrix_2;

TH1 *unfolded_data_all, *unfolded_data_all_2;
TH2 *correlation_matrix_all, *correlation_matrix_all_2;
TH1 *unfolded_mc_test_all, *unfolded_mc_test_all_2;
std::vector<TH1D*> background;
std::vector<TString> background_names;
bool subtract_background;
