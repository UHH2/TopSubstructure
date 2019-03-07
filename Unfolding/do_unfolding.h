#include "iostream"
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

TString binning_xml;
TFile *outputFile;
TDOMParser parser;

TUnfoldBinning *binning_rec;
TUnfoldBinning *binning_gen;
const TUnfoldBinning *measurement_rec;
const TUnfoldBinning *measurement_gen;
TH2D *mat_response;
TH1D *h_data, *h_unfold, *h_truth, *h_mc;
std::vector<TString> background_names;
std::vector<TH1D*> background;
bool subtract_background;
