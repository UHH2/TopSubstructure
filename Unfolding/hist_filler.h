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
void fill_scale(TTree *, TString prefix);
void fill_background(TTree *, TString prefix);

bool Muon_done;

TFile *outputFile, *outputFile2;
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

// variables
Double_t tau32_rec, tau32_rec_puppi, tau32_gen, tau32_calc_gen, tau32_calc_rec, tau32_calc_rec_puppi;
Double_t pt_gen, pt_gen_sd, pt_rec_puppi, pt_rec, mass_gen, mass_gen_sd, mass_rec, mass_rec_sd, mass_rec_puppi, mass_rec_puppi_sd;
Double_t gen_weight, rec_weight;           //weights
Double_t tau32_rec_sd, tau32_rec_puppi_sd, tau32_gen_sd;
Bool_t passed_gen_final, passed_gen_final_sd; // bools
Bool_t passed_rec_final, passed_rec_final_sd, passed_rec_final_puppi, passed_rec_final_puppi_sd;

// bool for different sidebands
Bool_t passed_gen_pt_mu_sideband, passed_gen_pt_mu_sideband_sd, passed_gen_dr_sideband, passed_gen_dr_sideband_sd, passed_gen_mass_sideband, passed_gen_mass_sideband_sd, passed_gen_pt_topjet_sideband, passed_gen_pt_topjet_sideband_sd;
Bool_t passed_rec_pt_mu_sideband, passed_rec_pt_mu_sideband_sd, passed_rec_pt_mu_sideband_puppi, passed_rec_pt_mu_sideband_puppi_sd;
Bool_t passed_rec_dr_sideband, passed_rec_dr_sideband_sd, passed_rec_dr_sideband_puppi, passed_rec_dr_sideband_puppi_sd;
Bool_t passed_rec_mass_sideband, passed_rec_mass_sideband_sd, passed_rec_mass_sideband_puppi, passed_rec_mass_sideband_puppi_sd;
Bool_t passed_rec_pt_topjet_sideband, passed_rec_pt_topjet_sideband_sd, passed_rec_pt_topjet_sideband_puppi, passed_rec_pt_topjet_sideband_puppi_sd;

//weights
Double_t w_gen, w_rec, w_sig_rec, w_central, w_nogen, w_norec, w_correction;
Double_t w_gen_scaled, w_rec_scaled, w_sig_rec_scaled, w_central_scaled, w_nogen_scaled, w_norec_scaled, w_correction_scaled;
