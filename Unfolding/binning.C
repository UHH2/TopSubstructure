#include <iostream>
#include <fstream>
#include <TFile.h>
#include "TUnfoldBinningXML.h"
#include <TF2.h>

using namespace std;
int main(int argc, char* argv[])
{
  cout << "create root file" << endl;

  // create root file to store binning schemes

  TString binning_root;
  TString binning_xml;

  binning_root = "Binning.root";
  binning_xml = "Binning.xml";

  TFile *binningSchemes = new TFile(binning_root,"recreate");

  cout << "set up binning" << endl;
  /******************* RECO BINNING ***********************************/
  Double_t BINS_REC_PT[] = {400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700};

  // calculate total number of bins
  int N_BINS_REC_PT = sizeof(BINS_REC_PT)/sizeof(BINS_REC_PT[0]) - 1;

  /******************* GEN BINNING ************************************/
  // here some actions with arrays are needed to have different possible array sizes

  // 1. set-up binning in vectors
  std::vector<Double_t> PT = {400, 420, 440, 460, 480, 500, 520, 540, 560, 580, 600, 620, 640, 660, 680, 700};

  // 2. get number of bins from every vector
  int N_BINS_GEN_PT = PT.size() - 1;

  // 3. create arrays from vectors arrays with correct size
  Double_t *BINS_GEN_PT = PT.data();

  // =======================================================================================================
  // REC BINNING
  TUnfoldBinning *binning_rec=new TUnfoldBinning("binning_rec");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_rec = binning_rec->AddBinning("measurement_rec");
  measurement_rec->AddAxis("pt",N_BINS_REC_PT,BINS_REC_PT,
                                false, // underflow bin
                                true // overflow bin
                                );

  // =======================================================================================================
  // GEN BINNING
  TUnfoldBinning *binning_gen=new TUnfoldBinning("binning_gen");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_gen = binning_gen->AddBinning("measurement_gen");
  measurement_gen->AddAxis("pt",N_BINS_GEN_PT,BINS_GEN_PT,
                                false, // no underflow bin
                                true // overflow bin
                                );

  cout << "wirte binning scheme to file" << endl;

  binning_rec->Write();
  binning_gen->Write();

  ofstream xmlOut(binning_xml);
  TUnfoldBinningXML::ExportXML(*binning_rec, xmlOut, kTRUE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_gen, xmlOut, kFALSE, kTRUE);
  TUnfoldBinningXML::WriteDTD();
  xmlOut.close();

  delete binningSchemes;

  cout << "finished" << endl;
  return 0;
}
