#include <iostream>
#include <fstream>
#include <TFile.h>
#include "TUnfoldBinningXML.h"
#include <TF2.h>
#include "bin_range.C"

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


  /******************* GEN BINNING ************************************/
  // here some actions with arrays are needed to have different possible array sizes

  // 1. set-up binning in vectors
  // std::vector<Double_t> TAU32 = {0, 0.4333, 0.5744, 0.6899, 0.7922, 0.8733, 1};
  // std::vector<Double_t> TAU32 = {0.2, 0.42, 0.53, 0.73, 0.87, 1};   //switch underflow off!
  std::vector<Double_t> TAU32 = {0.2, 0.45, 0.60, 0.73, 0.85, 1}; // CHS binning
  // std::vector<Double_t> TAU32 = {0.2, 0.42, 0.57, 0.7, 0.83, 1}; // PUPPI binning
  // std::vector<Double_t> TAU32 = {0.2, 0.45, 0.60, 0.73, 0.85, 1};  //looks good i guess, so keep it for now
  // std::vector<Double_t> TAU32 = {0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  std::vector<Double_t> TAU32_MASS_SPLIT = {0, 155, 1000};
  std::vector<Double_t> TAU32_PT_TOPJET = TAU32;
  std::vector<Double_t> TAU32_MASS = TAU32;

  // 2. get number of bins from every vector
  int N_BINS_GEN_TAU32 = TAU32.size() - 1;
  int N_BINS_GEN_TAU32_MASS_SPLIT = TAU32_MASS_SPLIT.size() - 1;
  int N_BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.size() - 1;
  int N_BINS_GEN_TAU32_MASS = TAU32_MASS.size() - 1;

  // 3. create arrays from vectors arrays with correct size
  Double_t *BINS_GEN_TAU32 = TAU32.data();
  Double_t *BINS_GEN_TAU32_MASS_SPLIT = TAU32_MASS_SPLIT.data();
  Double_t *BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.data();
  Double_t *BINS_GEN_TAU32_MASS = TAU32_MASS.data();

  cout << "set up binning" << endl;
  /******************* RECO BINNING ***********************************/
  // std::vector<Double_t> REC_TAU32 = {0, 0.3569, 0.3983, 0.4275, 0.4530, 0.4764, 0.4994, 0.5211, 0.5412, 0.5625, 0.5822, 0.6021, 0.6221, 0.6421, 0.6625, 0.6823, 0.7027, 0.7219, 0.7419, 0.7622, 0.7831, 0.8024, 0.8214, 0.8398, 0.8585, 0.8794, 0.9051, 1};
  // std::vector<Double_t> REC_TAU32 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1};
  TString file_name = "/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection/Muon/uhh2.AnalysisModuleRunner.MC.2016v3_TTbar.root";
  std::vector<Double_t> REC_TAU32 = bin_range(file_name, "mass", "/tau32_tj1_rebin2", 120, TAU32);
  // cout << "REC_TAU32 size: " << REC_TAU32.size() << '\n';
  // {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1};

  std::vector<Double_t> REC_TAU32_MASS_SPLIT = {0, 152, 1000};

  // std::vector<Double_t> REC_TAU32_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1};
  std::vector<Double_t> REC_TAU32_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "/tau32_tj1_rebin2", 120, TAU32);

  // std::vector<Double_t> REC_TAU32_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1};
  std::vector<Double_t> REC_TAU32_MASS = bin_range(file_name, "rec_mass_sideband", "/tau32_tj1_rebin2", 120, TAU32);

  // calculate total number of bins
  int N_BINS_REC_TAU32 = REC_TAU32.size() - 1;
  int N_BINS_REC_TAU32_MASS_SPLIT = REC_TAU32_MASS_SPLIT.size() - 1;
  int N_BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.size() - 1;
  int N_BINS_REC_TAU32_MASS = REC_TAU32_MASS.size() - 1;
  // int N_BINS_REC_TAU32_MASS = sizeof(BINS_REC_TAU32_MASS)/sizeof(BINS_REC_TAU32_MASS[0]) - 1;

  Double_t *BINS_REC_TAU32 = REC_TAU32.data();
  Double_t *BINS_REC_TAU32_MASS_SPLIT = REC_TAU32_MASS_SPLIT.data();
  Double_t *BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.data();
  Double_t *BINS_REC_TAU32_MASS = REC_TAU32_MASS.data();
  // =======================================================================================================
  // REC BINNING
  TUnfoldBinning *binning_rec=new TUnfoldBinning("binning_rec");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_rec = binning_rec->AddBinning("measurement_rec");
  measurement_rec->AddAxis("#tau_{3/2}",N_BINS_REC_TAU32,BINS_REC_TAU32,
  false, // underflow bin
  false // overflow bin
);
measurement_rec->AddAxis("mass",N_BINS_REC_TAU32_MASS_SPLIT,BINS_REC_TAU32_MASS_SPLIT,
false, // underflow bin
false // overflow bin
);

// define sideband distributions
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
true, // no underflow bin
false // overflow bin
);
measurement_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
false, // no underflow bin
false // overflow bin
);

// define sideband distributions
TUnfoldBinning *gen_pt_topjet_sideband = binning_gen->AddBinning("gen_pt_topjet_sideband");
gen_pt_topjet_sideband->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32_PT_TOPJET,BINS_GEN_TAU32_PT_TOPJET,
true, // no underflow bin
false // overflow bin
);
TUnfoldBinning *gen_mass_sideband = binning_gen->AddBinning("gen_mass_sideband");
gen_mass_sideband->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32_MASS,BINS_GEN_TAU32_MASS,
true, // no underflow bin
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

binningSchemes->Close();
delete binningSchemes;

cout << "finished" << endl;
return 0;
}
