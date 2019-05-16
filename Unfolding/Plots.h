#include "iostream"
#include "TString.h"
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

class Plotter{
public:
  Plotter(TString);
  void Plot_projections(TH1D* projections, TH1* h_input, TString save);
  void Plot_output(TH1* h_unfolded, TH1* h_truth_, bool normalise, TString save);
  void Plot_correlation_matrix(TH2* h_corr_matrix, TString save);
  void Plot_LogTau(TSpline* logTau_, double tau, double coordinate, TString save);
  void Plot_LCurve(TGraph* lcurve, std::vector<double> coordinates, std::vector<double> coordinates_2, TString save);
  void Plot_RhoLogTau(TSpline* rhologTau, double tau, TString save);
  void Plot_ResponseMatrix(TH2* resp_matrix_, TString save);
  void Plot_purity(TH1* purity_same, TH1* purity_all, TString save);
  void Plot_input(TH1* data, TH1* mc, TString save);
  void Plot_covariance(TH2* matrix, TString save);

private:
  TString save_dir_, name, x_axis_title, y_axis_title, dataset;
  TLine *l_input = 0, *l_input_2 = 0;
};
