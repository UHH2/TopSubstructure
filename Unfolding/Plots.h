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

class Plotter{
public:
  Plotter(TString);
  void Plot_projections(TH1D* projections, TH1* h_input, TString save);
  void Plot_output(TH1* h_unfolded, TH1* h_truth, bool normalise, TString save);
  void Plot_correlation_matrix(TH2* h_corr_matrix, TString save);
  void Plot_LogTau(TSpline* logTau, double tau, double coordinate, TString save);
  void Plot_LCurve(TGraph* lcurve, std::vector<double> coordinates, std::vector<double> coordinates_2, TString save);
  void Plot_RhoLogTau(TSpline* rhologTau, double tau, TString save);
  void Plot_ResponseMatrix(TH2* response_matrix, TString save);

private:
  TString save_dir, name, x_axis_title, y_axis_title, dataset;
};
