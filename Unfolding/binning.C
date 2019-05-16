#include <iostream>
#include <fstream>
#include <TFile.h>
#include "TUnfoldBinningXML.h"
#include <TF2.h>
#include <bin_range.C>

using namespace std;
int main(int argc, char* argv[])
{
  cout << "create root file" << endl;

  // create root file to store binning schemes

  TString file_path = "/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root";
  std::vector<TString> name = {"mass", "rec_pt_topjet_sideband", "rec_mass_sideband"};

  TString binning_root;
  TString binning_xml;

  binning_root = "Binning.root";
  binning_xml = "Binning.xml";

  TFile *binningSchemes = new TFile(binning_root,"recreate");

  double n_events = 100;
  if(argc > 1) n_events = atoi(argv[1]);
  cout << "set up binning" << endl;
  /******************* RECO BINNING ***********************************/
  // Double_t BINS_REC_TAU32[] = {0, 0.3569, 0.3983, 0.4275, 0.4530, 0.4764, 0.4994, 0.5211, 0.5412, 0.5625, 0.5822, 0.6021, 0.6221, 0.6421, 0.6625, 0.6823, 0.7027, 0.7219, 0.7419, 0.7622, 0.7831, 0.8024, 0.8214, 0.8398, 0.8585, 0.8794, 0.9051, 1};
  // Double_t BINS_REC_TAU32_PT_TOPJET[] = {0, 0.4924, 0.5912, 0.6648, 0.7186, 0.7619, 0.7987, 0.8310, 0.8621, 0.8928, 1};
  std::vector<Double_t> REC_TAU32 = bin_range(file_path, name.at(0), n_events);
  // std::vector<Double_t> TAU32 = {0, 0.463, 0.620, 0.749, 0.860, 1};
  std::vector<Double_t> REC_TAU32_PT_TOPJET = bin_range(file_path, name.at(1), n_events);
  std::vector<Double_t> REC_TAU32_MASS = bin_range(file_path, name.at(2), n_events);
  // Double_t BINS_REC_TAU32_MASS[] = {0, 0.6014, 0.6762, 0.7160, 0.7426, 0.7626, 0.7784, 0.7919, 0.8040, 0.8143, 0.8240, 0.8330, 0.8404, 0.8489, 0.8570, 0.8654, 0.8739, 0.8838, 0.8945, 0.9064, 0.9247, 1};

  // calculate total number of bins
  int N_BINS_REC_TAU32 = REC_TAU32.size() - 1;
  int N_BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.size() - 1;
  int N_BINS_REC_TAU32_MASS = REC_TAU32_MASS.size() - 1;
  // int N_BINS_REC_TAU32_MASS = sizeof(BINS_REC_TAU32_MASS)/sizeof(BINS_REC_TAU32_MASS[0]) - 1;

  Double_t *BINS_REC_TAU32 = REC_TAU32.data();
  Double_t *BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.data();
  Double_t *BINS_REC_TAU32_MASS = REC_TAU32_MASS.data();

  /******************* GEN BINNING ************************************/
  // here some actions with arrays are needed to have different possible array sizes

  // 1. set-up binning in vectors
  std::vector<Double_t> TAU32 = {0, 0.433, 0.574, 0.689, 0.792, 0.873, 1};
  // std::vector<Double_t> TAU32 = {0, 0.463, 0.620, 0.749, 0.860, 1};
  std::vector<Double_t> TAU32_PT_TOPJET = TAU32;
  std::vector<Double_t> TAU32_MASS = TAU32;

  // 2. get number of bins from every vector
  int N_BINS_GEN_TAU32 = TAU32.size() - 1;
  int N_BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.size() - 1;
  int N_BINS_GEN_TAU32_MASS = TAU32_MASS.size() - 1;

  // 3. create arrays from vectors arrays with correct size
  Double_t *BINS_GEN_TAU32 = TAU32.data();
  Double_t *BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.data();
  Double_t *BINS_GEN_TAU32_MASS = TAU32_MASS.data();

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

  TUnfoldBinning *rec_mass_sideband = binning_rec->AddBinning("rec_mass_sideband");
  rec_mass_sideband->AddAxis("#tau_{3/2}",N_BINS_REC_TAU32_MASS,BINS_REC_TAU32_MASS,
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
  TUnfoldBinning *gen_mass_sideband = binning_gen->AddBinning("gen_mass_sideband");
  gen_mass_sideband->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32_MASS,BINS_GEN_TAU32_MASS,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  cout << "write binning scheme to file" << endl;
binningSchemes->cd();
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
