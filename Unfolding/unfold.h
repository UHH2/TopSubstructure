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
    unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response, TH1D* h_truth, TH1D* h_test, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<TH1D*> background, std::vector<TString> background_names, bool subtract_background = true);
    TH1* get_output_Lcurve();
    TH2* get_correlation_Lcurve();
    TH1* get_output();
    TH2* get_correlation();
    std::vector<TH1D*> check_projection();

  private:
    TString directory;
    TString input_file;
    TString output_file;
    TString binning_xml;
    TFile *outputFile;
    TH1 *h_data_LCURVE;
    TH2 *h_data_LCURVErho;
    TH1 *h_data_tau;
    TH2 *h_data_taurho;
    TH2D *response_matrix;
    TH1D *hist_truth;
};
