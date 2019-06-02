#include <iostream>
#include <map>
#include <cmath>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TH1.h>
#include <TDOMParser.h>
#include <TXMLDocument.h>
#include "TUnfoldBinningXML.h"
#include <time.h>
#include <vector>


using namespace std;


void fill_data(TTree *);
void fill_pseudodata(TTree *);
void fill_ttbar(TTree *, TString prefix);
void fill_background(TTree *, TString prefix);


TFile *outputFile;
TFile *outputFile_bins;

// binning schemes
TUnfoldBinning *binning_rec;
TUnfoldBinning *binning_gen;
const TUnfoldBinning *measurement_rec;
const TUnfoldBinning *measurement_gen;
const TUnfoldBinning *rec_pt_topjet_sideband;
const TUnfoldBinning *gen_pt_topjet_sideband;
const TUnfoldBinning *rec_mass_sideband;
const TUnfoldBinning *gen_mass_sideband;


// variables to store gen or rec info
Double_t tau32_rec, tau32_gen, mass_rec, mass_gen;             // variables
Bool_t passed_rec_final, passed_gen_final; // bools
Bool_t passed_rec_pt_topjet_sideband, passed_gen_pt_topjet_sideband, passed_rec_mass_sideband, passed_gen_mass_sideband; // bools
Double_t gen_weight, rec_weight;           //weights


//weights
Double_t w_gen, w_rec, w_sig_rec, w_central, w_nogen, w_norec, w_correction;
