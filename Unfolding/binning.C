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
  std::vector<Double_t> TAU32 = {0, 0.362, 0.533, 0.732, 0.867, 1};
  std::vector<Double_t> ECF21 = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5};
  std::vector<Double_t> ECF22 = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5};
  std::vector<Double_t> ECF31 = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5};
  std::vector<Double_t> ECF32 = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5};
  // std::vector<Double_t> TAU32 = {0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  std::vector<Double_t> TAU32_MASS_SPLIT = {0, 155, 1000};
  std::vector<Double_t> TAU32_PT_TOPJET = TAU32;
  std::vector<Double_t> ECF21_PT_TOPJET = ECF21;
  std::vector<Double_t> ECF22_PT_TOPJET = ECF22;
  std::vector<Double_t> ECF31_PT_TOPJET = ECF31;
  std::vector<Double_t> ECF32_PT_TOPJET = ECF32;
  std::vector<Double_t> TAU32_MASS = TAU32;
  std::vector<Double_t> ECF21_MASS = ECF21;
  std::vector<Double_t> ECF22_MASS = ECF22;
  std::vector<Double_t> ECF31_MASS = ECF31;
  std::vector<Double_t> ECF32_MASS = ECF32;

  // 2. get number of bins from every vector
  int N_BINS_GEN_TAU32 = TAU32.size() - 1;
  int N_BINS_GEN_ECF21 = ECF21.size() - 1;
  int N_BINS_GEN_ECF22 = ECF22.size() - 1;
  int N_BINS_GEN_ECF31 = ECF31.size() - 1;
  int N_BINS_GEN_ECF32 = ECF32.size() - 1;
  int N_BINS_GEN_TAU32_MASS_SPLIT = TAU32_MASS_SPLIT.size() - 1;
  int N_BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.size() - 1;
  int N_BINS_GEN_ECF21_PT_TOPJET = ECF21_PT_TOPJET.size() - 1;
  int N_BINS_GEN_ECF22_PT_TOPJET = ECF22_PT_TOPJET.size() - 1;
  int N_BINS_GEN_ECF31_PT_TOPJET = ECF31_PT_TOPJET.size() - 1;
  int N_BINS_GEN_ECF32_PT_TOPJET = ECF32_PT_TOPJET.size() - 1;
  int N_BINS_GEN_TAU32_MASS = TAU32_MASS.size() - 1;
  int N_BINS_GEN_ECF21_MASS = ECF21_MASS.size() - 1;
  int N_BINS_GEN_ECF22_MASS = ECF22_MASS.size() - 1;
  int N_BINS_GEN_ECF31_MASS = ECF31_MASS.size() - 1;
  int N_BINS_GEN_ECF32_MASS = ECF32_MASS.size() - 1;

  // 3. create arrays from vectors arrays with correct size
  Double_t *BINS_GEN_TAU32 = TAU32.data();
  Double_t *BINS_GEN_ECF21 = ECF21.data();
  Double_t *BINS_GEN_ECF22 = ECF22.data();
  Double_t *BINS_GEN_ECF31 = ECF31.data();
  Double_t *BINS_GEN_ECF32 = ECF32.data();
  Double_t *BINS_GEN_TAU32_MASS_SPLIT = TAU32_MASS_SPLIT.data();
  Double_t *BINS_GEN_TAU32_PT_TOPJET = TAU32_PT_TOPJET.data();
  Double_t *BINS_GEN_ECF21_PT_TOPJET = ECF21_PT_TOPJET.data();
  Double_t *BINS_GEN_ECF22_PT_TOPJET = ECF22_PT_TOPJET.data();
  Double_t *BINS_GEN_ECF31_PT_TOPJET = ECF31_PT_TOPJET.data();
  Double_t *BINS_GEN_ECF32_PT_TOPJET = ECF32_PT_TOPJET.data();
  Double_t *BINS_GEN_TAU32_MASS = TAU32_MASS.data();
  Double_t *BINS_GEN_ECF21_MASS = ECF21_MASS.data();
  Double_t *BINS_GEN_ECF22_MASS = ECF22_MASS.data();
  Double_t *BINS_GEN_ECF31_MASS = ECF31_MASS.data();
  Double_t *BINS_GEN_ECF32_MASS = ECF32_MASS.data();

  cout << "set up binning" << endl;
  /******************* RECO BINNING ***********************************/
  // std::vector<Double_t> REC_TAU32 = {0, 0.3569, 0.3983, 0.4275, 0.4530, 0.4764, 0.4994, 0.5211, 0.5412, 0.5625, 0.5822, 0.6021, 0.6221, 0.6421, 0.6625, 0.6823, 0.7027, 0.7219, 0.7419, 0.7622, 0.7831, 0.8024, 0.8214, 0.8398, 0.8585, 0.8794, 0.9051, 1};
  // std::vector<Double_t> REC_TAU32 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1};
  TString file_name = "/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection/Muon/uhh2.AnalysisModuleRunner.MC.2016v3_TTbar.root";
  std::vector<Double_t> REC_TAU32 = bin_range(file_name, "mass", "/tau32_tj1_rebin2", 120, TAU32);
  // std::vector<Double_t> REC_ECF21 = bin_range(file_name, "mass", "/ecfN2_beta1_tj1_rebin", 120, ECF21);
  // std::vector<Double_t> REC_ECF22 = bin_range(file_name, "mass", "/ecfN2_beta2_tj1_rebin", 120, ECF22);
  // std::vector<Double_t> REC_ECF31 = bin_range(file_name, "mass", "/ecfN3_beta1_tj1_rebin", 120, ECF31);
  // std::vector<Double_t> REC_ECF32 = bin_range(file_name, "mass", "/ecfN3_beta2_tj1_rebin", 120, ECF32);
  std::vector<Double_t> REC_ECF21 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1,2};
  std::vector<Double_t> REC_ECF22 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1,2};
  std::vector<Double_t> REC_ECF31 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1,2};
  std::vector<Double_t> REC_ECF32 = {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1,2};
  // cout << "REC_TAU32 size: " << REC_TAU32.size() << '\n';
  // {0, 0.365, 0.409, 0.441, 0.470, 0.498, 0.524, 0.549, 0.575, 0.599, 0.623, 0.647, 0.672, 0.697, 0.721, 0.746, 0.772, 0.797, 0.820, 0.844, 0.868, 0.898, 1};

  std::vector<Double_t> REC_TAU32_MASS_SPLIT = {0, 152, 1000};

  // std::vector<Double_t> REC_TAU32_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1};
  std::vector<Double_t> REC_TAU32_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "/tau32_tj1_rebin2", 120, TAU32);
  // std::vector<Double_t> REC_ECF21_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "ecfN2_beta1_tj1_rebin", 120, ECF21);
  // std::vector<Double_t> REC_ECF22_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "ecfN2_beta2_tj1_rebin", 120, ECF22);
  // std::vector<Double_t> REC_ECF31_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "ecfN3_beta1_tj1_rebin", 120, ECF31);
  // std::vector<Double_t> REC_ECF32_PT_TOPJET = bin_range(file_name, "rec_pt_topjet_sideband", "ecfN3_beta2_tj1_rebin", 120, ECF32);
  std::vector<Double_t> REC_ECF21_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1,2};
  std::vector<Double_t> REC_ECF22_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1,2};
  std::vector<Double_t> REC_ECF31_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1,2};
  std::vector<Double_t> REC_ECF32_PT_TOPJET = {0, 0.520, 0.626, 0.704, 0.761, 0.806, 0.845, 0.885, 1,2};

  // std::vector<Double_t> REC_TAU32_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1};
  std::vector<Double_t> REC_TAU32_MASS = bin_range(file_name, "rec_mass_sideband", "/tau32_tj1_rebin2", 120, TAU32);
  // std::vector<Double_t> REC_ECF21_MASS = bin_range(file_name, "rec_mass_sideband", "ecfN2_beta1_tj1_rebin", 120, ECF21);
  // std::vector<Double_t> REC_ECF22_MASS = bin_range(file_name, "rec_mass_sideband", "ecfN2_beta2_tj1_rebin", 120, ECF22);
  // std::vector<Double_t> REC_ECF31_MASS = bin_range(file_name, "rec_mass_sideband", "ecfN3_beta1_tj1_rebin", 120, ECF31);
  // std::vector<Double_t> REC_ECF32_MASS = bin_range(file_name, "rec_mass_sideband", "ecfN3_beta2_tj1_rebin", 120, ECF32);
  std::vector<Double_t> REC_ECF21_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1,2};
  std::vector<Double_t> REC_ECF22_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1,2};
  std::vector<Double_t> REC_ECF31_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1,2};
  std::vector<Double_t> REC_ECF32_MASS = {0, 0.615, 0.691, 0.727, 0.755, 0.774, 0.791, 0.805, 0.817, 0.828, 0.838, 0.848, 0.857, 0.867, 0.877, 0.889, 0.902, 0.922, 1,2};

  // calculate total number of bins
  int N_BINS_REC_TAU32 = REC_TAU32.size() - 1;
  int N_BINS_REC_ECF21 = REC_ECF21.size() - 1;
  int N_BINS_REC_ECF22 = REC_ECF22.size() - 1;
  int N_BINS_REC_ECF31 = REC_ECF31.size() - 1;
  int N_BINS_REC_ECF32 = REC_ECF32.size() - 1;
  int N_BINS_REC_TAU32_MASS_SPLIT = REC_TAU32_MASS_SPLIT.size() - 1;
  int N_BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.size() - 1;
  int N_BINS_REC_ECF21_PT_TOPJET = REC_ECF21_PT_TOPJET.size() - 1;
  int N_BINS_REC_ECF22_PT_TOPJET = REC_ECF22_PT_TOPJET.size() - 1;
  int N_BINS_REC_ECF31_PT_TOPJET = REC_ECF31_PT_TOPJET.size() - 1;
  int N_BINS_REC_ECF32_PT_TOPJET = REC_ECF32_PT_TOPJET.size() - 1;
  int N_BINS_REC_TAU32_MASS = REC_TAU32_MASS.size() - 1;
  int N_BINS_REC_ECF21_MASS = REC_ECF21_MASS.size() - 1;
  int N_BINS_REC_ECF22_MASS = REC_ECF22_MASS.size() - 1;
  int N_BINS_REC_ECF31_MASS = REC_ECF31_MASS.size() - 1;
  int N_BINS_REC_ECF32_MASS = REC_ECF32_MASS.size() - 1;
  // int N_BINS_REC_TAU32_MASS = sizeof(BINS_REC_TAU32_MASS)/sizeof(BINS_REC_TAU32_MASS[0]) - 1;

  Double_t *BINS_REC_TAU32 = REC_TAU32.data();
  Double_t *BINS_REC_ECF21 = REC_ECF21.data();
  Double_t *BINS_REC_ECF22 = REC_ECF22.data();
  Double_t *BINS_REC_ECF31 = REC_ECF31.data();
  Double_t *BINS_REC_ECF32 = REC_ECF32.data();
  Double_t *BINS_REC_TAU32_MASS_SPLIT = REC_TAU32_MASS_SPLIT.data();
  Double_t *BINS_REC_TAU32_PT_TOPJET = REC_TAU32_PT_TOPJET.data();
  Double_t *BINS_REC_ECF21_PT_TOPJET = REC_ECF21_PT_TOPJET.data();
  Double_t *BINS_REC_ECF22_PT_TOPJET = REC_ECF22_PT_TOPJET.data();
  Double_t *BINS_REC_ECF31_PT_TOPJET = REC_ECF31_PT_TOPJET.data();
  Double_t *BINS_REC_ECF32_PT_TOPJET = REC_ECF32_PT_TOPJET.data();
  Double_t *BINS_REC_TAU32_MASS = REC_TAU32_MASS.data();
  Double_t *BINS_REC_ECF21_MASS = REC_ECF21_MASS.data();
  Double_t *BINS_REC_ECF22_MASS = REC_ECF22_MASS.data();
  Double_t *BINS_REC_ECF31_MASS = REC_ECF31_MASS.data();
  Double_t *BINS_REC_ECF32_MASS = REC_ECF32_MASS.data();
  // =======================================================================================================
  // REC BINNING
  TUnfoldBinning *binning_rec=new TUnfoldBinning("binning_rec");
  TUnfoldBinning *binning_ecf21_rec=new TUnfoldBinning("binning_ecf21_rec");
  TUnfoldBinning *binning_ecf22_rec=new TUnfoldBinning("binning_ecf22_rec");
  TUnfoldBinning *binning_ecf31_rec=new TUnfoldBinning("binning_ecf31_rec");
  TUnfoldBinning *binning_ecf32_rec=new TUnfoldBinning("binning_ecf32_rec");

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


  TUnfoldBinning *measurement_ecf21_rec = binning_ecf21_rec->AddBinning("measurement_ecf21_rec");
  measurement_ecf21_rec->AddAxis("ECF21",N_BINS_REC_ECF21,BINS_REC_ECF21,
                                    false, // underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf21_rec->AddAxis("mass",N_BINS_REC_TAU32_MASS_SPLIT,BINS_REC_TAU32_MASS_SPLIT,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *rec_pt_topjet_ecf21_sideband = binning_ecf21_rec->AddBinning("rec_pt_topjet_ecf21_sideband");
  rec_pt_topjet_ecf21_sideband->AddAxis("ECF21",N_BINS_REC_ECF21_PT_TOPJET,BINS_REC_ECF21_PT_TOPJET,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *rec_mass_ecf21_sideband = binning_ecf21_rec->AddBinning("rec_mass_ecf21_sideband");
  rec_mass_ecf21_sideband->AddAxis("ECF21",N_BINS_REC_ECF21_MASS,BINS_REC_ECF21_MASS,
                                    false, // underflow bin
                                    false // overflow bin
                                  );


  TUnfoldBinning *measurement_ecf22_rec = binning_ecf22_rec->AddBinning("measurement_ecf22_rec");
  measurement_ecf22_rec->AddAxis("ECF22",N_BINS_REC_ECF22,BINS_REC_ECF22,
                                    false, // underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf22_rec->AddAxis("mass",N_BINS_REC_TAU32_MASS_SPLIT,BINS_REC_TAU32_MASS_SPLIT,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *rec_pt_topjet_ecf22_sideband = binning_ecf22_rec->AddBinning("rec_pt_topjet_ecf22_sideband");
  rec_pt_topjet_ecf22_sideband->AddAxis("ECF22",N_BINS_REC_ECF22_PT_TOPJET,BINS_REC_ECF22_PT_TOPJET,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *rec_mass_ecf22_sideband = binning_ecf22_rec->AddBinning("rec_mass_ecf22_sideband");
  rec_mass_ecf22_sideband->AddAxis("ECF22",N_BINS_REC_ECF22_MASS,BINS_REC_ECF22_MASS,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *measurement_ecf31_rec = binning_ecf31_rec->AddBinning("measurement_ecf31_rec");
  measurement_ecf31_rec->AddAxis("ECF31",N_BINS_REC_ECF31,BINS_REC_ECF31,
                                    false, // underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf31_rec->AddAxis("mass",N_BINS_REC_TAU32_MASS_SPLIT,BINS_REC_TAU32_MASS_SPLIT,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *rec_pt_topjet_ecf31_sideband = binning_ecf31_rec->AddBinning("rec_pt_topjet_ecf31_sideband");
  rec_pt_topjet_ecf31_sideband->AddAxis("ECF31",N_BINS_REC_ECF31_PT_TOPJET,BINS_REC_ECF31_PT_TOPJET,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *rec_mass_ecf31_sideband = binning_ecf31_rec->AddBinning("rec_mass_ecf31_sideband");
  rec_mass_ecf31_sideband->AddAxis("ECF31",N_BINS_REC_ECF31_MASS,BINS_REC_ECF31_MASS,
                                    false, // underflow bin
                                    false // overflow bin
                                  );


  TUnfoldBinning *measurement_ecf32_rec = binning_ecf32_rec->AddBinning("measurement_ecf32_rec");
  measurement_ecf32_rec->AddAxis("ECF32",N_BINS_REC_ECF32,BINS_REC_ECF32,
                                    false, // underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf32_rec->AddAxis("mass",N_BINS_REC_TAU32_MASS_SPLIT,BINS_REC_TAU32_MASS_SPLIT,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *rec_pt_topjet_ecf32_sideband = binning_ecf32_rec->AddBinning("rec_pt_topjet_ecf32_sideband");
  rec_pt_topjet_ecf32_sideband->AddAxis("ECF32",N_BINS_REC_ECF32_PT_TOPJET,BINS_REC_ECF32_PT_TOPJET,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  TUnfoldBinning *rec_mass_ecf32_sideband = binning_ecf32_rec->AddBinning("rec_mass_ecf32_sideband");
  rec_mass_ecf32_sideband->AddAxis("ECF32",N_BINS_REC_ECF32_MASS,BINS_REC_ECF32_MASS,
                                    false, // underflow bin
                                    false // overflow bin
                                  );

  // =======================================================================================================
  // GEN BINNING
  TUnfoldBinning *binning_gen=new TUnfoldBinning("binning_gen");
  TUnfoldBinning *binning_ecf21_gen=new TUnfoldBinning("binning_ecf21_gen");
  TUnfoldBinning *binning_ecf22_gen=new TUnfoldBinning("binning_ecf22_gen");
  TUnfoldBinning *binning_ecf31_gen=new TUnfoldBinning("binning_ecf31_gen");
  TUnfoldBinning *binning_ecf32_gen=new TUnfoldBinning("binning_ecf32_gen");

  // define measurement phase space distribution
  TUnfoldBinning *measurement_gen = binning_gen->AddBinning("measurement_gen");
  measurement_gen->AddAxis("#tau_{3/2}",N_BINS_GEN_TAU32,BINS_GEN_TAU32,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  measurement_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
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




  TUnfoldBinning *measurement_ecf21_gen = binning_ecf21_gen->AddBinning("measurement_ecf21_gen");
  measurement_ecf21_gen->AddAxis("ECF21",N_BINS_GEN_ECF21,BINS_GEN_ECF21,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf21_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *gen_pt_topjet_ecf21_sideband = binning_ecf21_gen->AddBinning("gen_pt_topjet_ecf21_sideband");
  gen_pt_topjet_ecf21_sideband->AddAxis("ECF21",N_BINS_GEN_ECF21_PT_TOPJET,BINS_GEN_ECF21_PT_TOPJET,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  TUnfoldBinning *gen_mass_ecf21_sideband = binning_ecf21_gen->AddBinning("gen_mass_ecf21_sideband");
  gen_mass_ecf21_sideband->AddAxis("ECF21",N_BINS_GEN_ECF21_MASS,BINS_GEN_ECF21_MASS,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );





  TUnfoldBinning *measurement_ecf22_gen = binning_ecf22_gen->AddBinning("measurement_ecf22_gen");
  measurement_ecf22_gen->AddAxis("ecf",N_BINS_GEN_ECF22,BINS_GEN_ECF22,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf22_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *gen_pt_topjet_ecf22_sideband = binning_ecf22_gen->AddBinning("gen_pt_topjet_ecf22_sideband");
  gen_pt_topjet_ecf22_sideband->AddAxis("ECF22",N_BINS_GEN_ECF22_PT_TOPJET,BINS_GEN_ECF22_PT_TOPJET,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  TUnfoldBinning *gen_mass_ecf22_sideband = binning_ecf22_gen->AddBinning("gen_mass_ecf22_sideband");
  gen_mass_ecf22_sideband->AddAxis("ECF22",N_BINS_GEN_ECF22_MASS,BINS_GEN_ECF22_MASS,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );






  TUnfoldBinning *measurement_ecf31_gen = binning_ecf31_gen->AddBinning("measurement_ecf31_gen");
  measurement_ecf31_gen->AddAxis("ecf",N_BINS_GEN_ECF31,BINS_GEN_ECF31,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf31_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *gen_pt_topjet_ecf31_sideband = binning_ecf31_gen->AddBinning("gen_pt_topjet_ecf31_sideband");
  gen_pt_topjet_ecf31_sideband->AddAxis("ECF31",N_BINS_GEN_ECF31_PT_TOPJET,BINS_GEN_ECF31_PT_TOPJET,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  TUnfoldBinning *gen_mass_ecf31_sideband = binning_ecf31_gen->AddBinning("gen_mass_ecf31_sideband");
  gen_mass_ecf31_sideband->AddAxis("ECF31",N_BINS_GEN_ECF31_MASS,BINS_GEN_ECF31_MASS,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );






  TUnfoldBinning *measurement_ecf32_gen = binning_ecf32_gen->AddBinning("measurement_ecf32_gen");
  measurement_ecf32_gen->AddAxis("ecf",N_BINS_GEN_ECF32,BINS_GEN_ECF32,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  measurement_ecf32_gen->AddAxis("mass",N_BINS_GEN_TAU32_MASS_SPLIT,BINS_GEN_TAU32_MASS_SPLIT,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );

  // define sideband distributions
  TUnfoldBinning *gen_pt_topjet_ecf32_sideband = binning_ecf32_gen->AddBinning("gen_pt_topjet_ecf32_sideband");
  gen_pt_topjet_ecf32_sideband->AddAxis("ECF32",N_BINS_GEN_ECF32_PT_TOPJET,BINS_GEN_ECF32_PT_TOPJET,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );
  TUnfoldBinning *gen_mass_ecf32_sideband = binning_ecf32_gen->AddBinning("gen_mass_ecf32_sideband");
  gen_mass_ecf32_sideband->AddAxis("ECF32",N_BINS_GEN_ECF32_MASS,BINS_GEN_ECF32_MASS,
                                    false, // no underflow bin
                                    false // overflow bin
                                  );







  cout << "write binning scheme to file" << endl;
  binningSchemes->cd();
  binning_rec->Write();
  binning_gen->Write();
  binning_ecf21_rec->Write();
  binning_ecf21_gen->Write();
  binning_ecf22_rec->Write();
  binning_ecf22_gen->Write();
  binning_ecf31_rec->Write();
  binning_ecf31_gen->Write();
  binning_ecf32_rec->Write();
  binning_ecf32_gen->Write();

  ofstream xmlOut(binning_xml);
  TUnfoldBinningXML::ExportXML(*binning_rec, xmlOut, kTRUE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_gen, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf21_rec, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf21_gen, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf22_rec, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf22_gen, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf31_rec, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf31_gen, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf32_rec, xmlOut, kFALSE, kFALSE);
  TUnfoldBinningXML::ExportXML(*binning_ecf32_gen, xmlOut, kFALSE, kTRUE);
  TUnfoldBinningXML::WriteDTD();
  xmlOut.close();

  binningSchemes->Close();
  delete binningSchemes;

  cout << "finished" << endl;
  return 0;
}
