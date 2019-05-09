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
  // Double_t BINS_REC_TAU32[] = {0, 0.3555, 0.3947, 0.4274, 0.4536, 0.4788, 0.5014, 0.5232, 0.5450, 0.5656, 0.5851, 0.6060, 0.6259, 0.6455, 0.6660, 0.6861, 0.7085, 0.7283, 0.7504, 0.7739, 0.7958, 0.8160, 0.8343, 0.8554, 0.8766, 0.9040, 1};
  // Double_t BINS_REC_TAU32[] = {0, 0.3533, 0.3940, 0.4215, 0.4456, 0.4679, 0.4898, 0.5101, 0.5294, 0.5487, 0.5680, 0.5861, 0.6045, 0.6230, 0.6418, 0.6609, 0.6795, 0.6986, 0.7169, 0.7354, 0.7539, 0.7731, 0.7923, 0.8096, 0.8271, 0.8449, 0.8621, 0.8820, 0.9075, 1};
  Double_t BINS_REC_TAU32[] = {0, 0.3569, 0.3983, 0.4275, 0.4530, 0.4764, 0.4994, 0.5211, 0.5412, 0.5625, 0.5822, 0.6021, 0.6221, 0.6421, 0.6625, 0.6823, 0.7027, 0.7219, 0.7419, 0.7622, 0.7831, 0.8024, 0.8214, 0.8398, 0.8585, 0.8794, 0.9051, 1};
  Double_t BINS_REC_TAU32_PT_TOPJET[] = {0, 0.4924, 0.5912, 0.6648, 0.7186, 0.7619, 0.7987, 0.8310, 0.8621, 0.8928, 1};

  // calculate total number of bins
  int N_BINS_REC_TAU32 = sizeof(BINS_REC_TAU32)/sizeof(BINS_REC_TAU32[0]) - 1;
  int N_BINS_REC_TAU32_PT_TOPJET = sizeof(BINS_REC_TAU32_PT_TOPJET)/sizeof(BINS_REC_TAU32_PT_TOPJET[0]) - 1;

  /******************* GEN BINNING ************************************/
  // here some actions with arrays are needed to have different possible array sizes

  // 1. set-up binning in vectors
  std::vector<Double_t> TAU32 = {0, 0.429, 0.572, 0.688, 0.789, 0.871, 1};
  std::vector<Double_t> TAU32_PT_TOPJET = TAU32;

  // 2. get number of bins from every vector
  int N_BINS_GEN_TAU32 = TAU32.size() - 1;
  int N_BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.size() - 1;

  // 3. create arrays from vectors arrays with correct size
  Double_t *BINS_GEN_TAU32 = TAU32.data();
  Double_t *BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.data();

  // =======================================================================================================
  // REC BINNING
  TUnfoldBinning *binning_rec=new TUnfoldBinning("binning_rec");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_rec = binning_rec->AddBinning("measurement_rec");
  measurement_rec->AddAxis("#tau_{3/2}",N_BINS_REC_TAU32,BINS_REC_TAU32,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *rec_pt_topjet_sideband = binning_rec->AddBinning("rec_pt_topjet_sideband");
  rec_pt_topjet_sideband->AddAxis("#tau_{3/2}",N_BINS_REC_TAU32_PT_TOPJET,BINS_REC_TAU32_PT_TOPJET,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // =======================================================================================================
  // GEN BINNING
  TUnfoldBinning *binning_gen=new TUnfoldBinning("binning_gen");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_gen = binning_gen->AddBinning("measurement_gen");
  measurement_gen->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32,BINS_GEN_TAU32,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  TUnfoldBinning *gen_pt_topjet_sideband = binning_gen->AddBinning("gen_pt_topjet_sideband");
  gen_pt_topjet_sideband->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32_PT_TOPJET,BINS_GEN_TAU32_PT_TOPJET,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  cout << "write binning scheme to file" << endl;

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
