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
#include <TGaxis.h>

TString binning_xml;
TFile *outputFile;
TDOMParser parser;

TUnfoldBinning *binning_rec, *binning_gen;
const TUnfoldBinning *measurement_rec, *measurement_gen;
TString save_projection_x, save_projection_y, save_LCurve_Unfolding, save_LCurve_Unfolding_correlation, save_LCurve_TauX,  save_LCurve_TauY, save_TauScan, save_TauScan_RhoLogTau, save_TauScan_Unfolding, save_TauScan_Unfolding_correlation, save_TauScan_crosscheck, save_LCurve_crosscheck;
TH2D *mat_response;
TH2 *correlation_matrix, *correlation_matrix_2;
TH1D *h_data, *h_unfold, *h_truth, *h_mc, *h_trutch_check, *h_purity_all, *h_purity_samebin;
TH1 *unfolded_data, *unfolded_data_2, *unfolded_mc_test, *unfolded_mc_test_2;
TGraph *lcurve, *lcurve_2;
TSpline *rhologTau, *rhologTau_2;
double tau, tau_2, tau_value;
std::vector<double> coordinates, coordinates_2;
std::vector<TSpline*> logTau, logTau_2;
std::vector<TString> background_names;
std::vector<TH1D*> background;
bool subtract_background;
TMarker *p1, *p2, *p3, *p4, *LCurve_tau, *TauScan_tau;
