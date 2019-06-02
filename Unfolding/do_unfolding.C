#include "do_unfolding.h"

using namespace std;
int counter = 1;
int main(int argc, char* argv[]){
  // switch on histogram errors
  if(argc < 4){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans> <value of tau>; if you don't want to use a custom value of tau, then set it to -1");
  }
  if(argv[1] != std::string("pseudo1") && argv[1] != std::string("pseudo2") && argv[1] != std::string("pseudo3") && argv[1] != std::string("svuu") && argv[1] != std::string("svud") && argv[1] != std::string("svdu") && argv[1] != std::string("svdd")){
    throw runtime_error("use pseudo1, pseudo2, pseudo3, svuu, svud, svdu or svdd");
  }
  TH1::SetDefaultSumw2();

  string filename;
  TString dataset = argv[1];
  filename = "Unfolding_" + dataset + ".root";
  TFile *outputFile = new TFile(filename.c_str(),"recreate");

  binning_xml = "Binning.xml";

  Int_t error = parser.ParseFile(binning_xml);
  if(error) cout << "error = " << error << " from TDOMParser\n";
  TXMLDocument const *XMLdocument = parser.GetXMLDocument();
  binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_rec");
  binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_gen");
  binning_rec->Write();
  binning_gen->Write();
  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  // define directory
  TString dir = "/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/TopSubstructure/Unfolding/";
  TFile *data_File = new TFile(dir + "Histograms.root");

  // fill background hists -- add further or remove unnecassary backgrounds in background_names
  background_names = {"DYJets", "QCD", "ST", "WJets", "Diboson_WW", "Diboson_WZ", "Diboson_ZZ"};

  // Setup everything for unfolding!
  if(dataset == std::string("pseudo1")){
    data_File->GetObject("mc_TTbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_1", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("mc_TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_rec_dist_1", h_mc_dist);
    data_File->GetObject("mc_TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("mc_TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo2")){
    data_File->GetObject("mc_TTbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_2", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("mc_TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_rec_dist_2", h_mc_dist);
    data_File->GetObject("mc_TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("mc_TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo3")){
    data_File->GetObject("mc_TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_3", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);              // fill histogram with truth
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("mc_TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_rec_dist_3", h_mc_dist);
    data_File->GetObject("mc_TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("mc_TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("svuu1")){
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("svuu2")){
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("svuu3")){
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_u_TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("svud1")){
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("svud2")){
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("svud3")){
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("SVMR_u_SVMF_d_TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("svdu1")){
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("svdu2")){
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("svdu3")){
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_u_TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("svdd1")){
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("svdd2")){
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("svdd3")){
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("SVMR_d_SVMF_d_TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("data")){
    data_File->GetObject("TTbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Data", h_data);                             // fill histogram with data which should be unfolded
    data_File->GetObject("TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_truth_3", h_truth);                   // fill histogram with truth
    data_File->GetObject("TTbar_truth_all_3", h_truth_all);                   // fill histogram with truth
    data_File->GetObject("TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_3", h_purity_samebin);
  }

  for(unsigned int i = 0; i < background_names.size(); i++){
    background.push_back((TH1D*) data_File->Get("background_rec_" + background_names.at(i)));
  }
  subtract_background = false;
  TString save_dir = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
  // TString dataset = argv[1];
  TString format = ".eps";
  TString regmode = "size";
  TString density_flag = "none";
  TString nscan_word = argv[2];
  int nscan = atoi(argv[2]);
  cout << "nscan: " << nscan << '\n';
  bool do_lcurve = true;
  bool do_lcurve_2 = false;
  double tau_value = atoi(argv[3]);
  TString tau_value_word = argv[3];

  if(tau_value < 0){
    save_LCurve_Unfolding = "LCurve/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_all = "LCurve/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm = "LCurve/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm_all = "LCurve/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation = "LCurve/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation_all = "LCurve/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauX = "LCurve/TauX_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauY = "LCurve/TauY_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_crosscheck = "LCurve/crosscheck_" + dataset + format;
    save_LCurve_crosscheck_all = "LCurve/crosscheck_all_" + dataset + format;
    save_LCurve_input_cov = "LCurve/Covariance_input_" + dataset + format;
    save_LCurve_input_cov_all = "LCurve/Covariance_input_all_" + dataset + format;
    save_LCurve_input_cov_matrix = "LCurve/Covariance_matrix_" + dataset + format;
    save_LCurve_input_cov_matrix_all = "LCurve/Covariance_matrix_all_" + dataset + format;
    save_LCurve_input_cov_total = "LCurve/Covariance_total_" + dataset + format;
    save_LCurve_input_cov_total_all = "LCurve/Covariance_total_all_" + dataset + format;

    save_TauScan_LCurve = "TauScan/LCurve_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_RhoLogTau = "TauScan/RhoLogTau_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding = "TauScan/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_all = "TauScan/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_norm = "TauScan/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_norm_all = "TauScan/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_correlation = "TauScan/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_correlation_all = "TauScan/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_crosscheck = "TauScan/crosscheck_" + dataset + format;
    save_TauScan_crosscheck_all = "TauScan/crosscheck_all_" + dataset + format;
    save_TauScan_input_cov = "TauScan/Covariance_input_" + dataset + format;
    save_TauScan_input_cov_all = "TauScan/Covariance_input_all_" + dataset + format;
    save_TauScan_input_cov_matrix = "TauScan/Covariance_matrix_" + dataset + format;
    save_TauScan_input_cov_matrix_all = "TauScan/Covariance_matrix_all_" + dataset + format;
    save_TauScan_input_cov_total = "TauScan/Covariance_total_" + dataset + format;
    save_TauScan_input_cov_total_all = "TauScan/Covariance_total_all_" + dataset + format;
  }
  else{
    save_LCurve_Unfolding = "CustomTau/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_all = "CustomTau/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm = "CustomTau/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_norm_all = "CustomTau/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation = "CustomTau/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_correlation_all = "CustomTau/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_crosscheck = "CustomTau/crosscheck_" + dataset  + format;
    save_LCurve_crosscheck_all = "CustomTau/crosscheck_all_" + dataset + format;
    save_LCurve_input_cov = "CustomTau/Covariance_input_" + dataset + format;
    save_LCurve_input_cov_all = "CustomTau/Covariance_input_all_" + dataset + format;
    save_LCurve_input_cov_matrix = "CustomTau/Covariance_matrix_" + dataset + format;
    save_LCurve_input_cov_matrix_all = "CustomTau/Covariance_matrix_all_" + dataset + format;
    save_LCurve_input_cov_total = "CustomTau/Covariance_total_" + dataset + format;
    save_LCurve_input_cov_total_all = "CustomTau/Covariance_total_all_" + dataset + format;
  }
  save_response_matrix = "Response_Matrix_" + dataset + format;
  save_projection_x = "Projection_X_" + dataset + format;
  save_projection_y = "Projection_Y_" + dataset + format;
  save_purity = "Purity_" + dataset + format;
  save_input = "Input_" + dataset + format;
  save_input_dist = "Input_dist_" + dataset + format;
  // what the unfolding class needs:
  // 1.    Set the data which should be unfolded.
  // 2.    Set reco histogram which fills the response matrix.
  // 3.    Set response matrix.
  // 4.    Set truth of unfolded data (for pseudo data, truth of pseudodata; for data, truth of gen level).
  // 5./6. Set binning of gen and reco level.
  // 7./8. Set background Histograms (and their names).
  // 9.    Set the number of scans.
  // 10.   Set the unfolding mode.
  // 11.   Set density flag.
  // 12.   Should  the L-Curve scan be done? Otherwise do the TauScan!
  // 13.   Should the background be subtracted?
  // 14.   Set a Tau value if wanted (default = -1 --> don't use a custom value)
  outputFile->cd();
  unfolding unfold(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve, subtract_background, tau_value);

  // unfold.get_output();
  // unfold.get_output_all();
  // unfold.get_correlation();
  //  unfold.get_output();
  //  unfold.get_correlation_all();
  //  unfold.get_input_statcov();
  //  unfold.get_input_statcov_all();
  //  unfold.get_matrix_statcov();
  //  unfold.get_matrix_statcov_all();
  //  unfold.get_total_statcov();
  //  unfold.get_total_statcov_all();
  //  unfold.get_output_check();
  //  unfold.get_output_check_all();
  // if(tau_value < 0){
  //    unfold.get_lcurve();
  //    unfold.get_coordinates();
  //    unfold.get_tau();
  //    unfold.get_logtau();
  //   if(!do_lcurve) unfold.get_rhologtau();
  // }
  // unfold.check_projection();
  unfolded_data = unfold.get_output();

  unfolded_data->Write();
  unfolded_data_all = unfold.get_output_all();
  unfolded_data_all->Write();
  correlation_matrix = unfold.get_correlation();
  correlation_matrix->Write();
  correlation_matrix_all = unfold.get_correlation_all();
  correlation_matrix_all->Write();
  covariance_input = unfold.get_input_statcov();
  covariance_input->Write();
  covariance_input_all = unfold.get_input_statcov_all();
  covariance_input_all->Write();
  covariance_matrix = unfold.get_matrix_statcov();
  covariance_matrix->Write();
  covariance_matrix_all = unfold.get_matrix_statcov_all();
  covariance_matrix_all->Write();
  covariance_total = unfold.get_total_statcov();
  covariance_total->Write();
  covariance_total_all = unfold.get_total_statcov_all();
  covariance_total_all->Write();
  unfolded_mc_test = unfold.get_output_check();
  unfolded_mc_test->Write();
  unfolded_mc_test_all = unfold.get_output_check_all();
  unfolded_mc_test_all->Write();
  if(tau_value < 0){
    lcurve = unfold.get_lcurve();
    lcurve->Write();
    coordinates = unfold.get_coordinates();
    tau = unfold.get_tau();
    logTau = unfold.get_logtau();
    logTau.at(0)->Write();
    logTau.at(1)->Write();
    if(!do_lcurve) {rhologTau = unfold.get_rhologtau();
  rhologTau->Write();}
  }
  std::vector<TH1D*> projections = unfold.check_projection();
  projections.at(0)->Write();
  projections.at(1)->Write();

  Plotter * plot = new Plotter(save_dir, binning_gen, binning_rec);
  plot->Plot_input(h_data, h_mc, save_input);
  plot->Plot_input(h_data_dist, h_mc_dist, save_input_dist);
  plot->Plot_covariance(covariance_input, save_LCurve_input_cov);
  plot->Plot_covariance(covariance_input_all, save_LCurve_input_cov_all);
  plot->Plot_covariance(covariance_matrix, save_LCurve_input_cov_matrix);
  plot->Plot_covariance(covariance_matrix_all, save_LCurve_input_cov_matrix_all);
  plot->Plot_covariance(covariance_total, save_LCurve_input_cov_total);
  plot->Plot_covariance(covariance_total_all, save_LCurve_input_cov_total_all);
  plot->Plot_projections(projections.at(0), h_unfold, save_projection_x);
  plot->Plot_projections(projections.at(1), h_mc, save_projection_y);
  plot->Plot_purity(h_purity_samebin, h_purity_all, save_purity);
  plot->Plot_output(unfolded_data, h_truth, false, save_LCurve_Unfolding);
  plot->Plot_output(unfolded_data_all, h_truth_all, false, save_LCurve_Unfolding_all);
  plot->Plot_output(unfolded_mc_test, h_trutch_check, false, save_LCurve_crosscheck);
  plot->Plot_output(unfolded_mc_test_all, h_trutch_check_all, false, save_LCurve_crosscheck_all);
  plot->Plot_output(unfolded_data, h_truth, true, save_LCurve_Unfolding_norm);
  plot->Plot_output(unfolded_data_all, h_truth_all, true, save_LCurve_Unfolding_norm_all);
  plot->Plot_correlation_matrix(correlation_matrix, save_LCurve_Unfolding_correlation);
  plot->Plot_correlation_matrix(correlation_matrix_all, save_LCurve_Unfolding_correlation_all);
  plot->Plot_ResponseMatrix(mat_response, save_response_matrix);
  if(tau_value < 0){
    plot->Plot_LogTau(logTau.at(0), tau, coordinates.at(0), save_LCurve_TauX);
    plot->Plot_LogTau(logTau.at(1), tau, coordinates.at(1), save_LCurve_TauY);


    unfolding unfold2(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve_2, subtract_background, tau_value);

    coordinates_2 = unfold2.get_coordinates();
    lcurve_2 = unfold2.get_lcurve();
    tau_2 = unfold2.get_tau();
    logTau_2 = unfold2.get_logtau();
    if(!do_lcurve_2) rhologTau_2 = unfold2.get_rhologtau();
    unfolded_data_2 = unfold2.get_output();
    covariance_input_2 = unfold2.get_input_statcov();
    covariance_input_all_2 = unfold2.get_input_statcov_all();
    covariance_matrix_2 = unfold2.get_matrix_statcov();
    covariance_matrix_all_2 = unfold2.get_matrix_statcov_all();
    covariance_total_2 = unfold2.get_total_statcov();
    covariance_total_all_2 = unfold2.get_total_statcov_all();
    unfolded_data_2->Write();
    h_truth->Write();
    unfolded_data_all_2 = unfold2.get_output_all();
    correlation_matrix_2 = unfold2.get_correlation();
    correlation_matrix_all_2 = unfold2.get_correlation_all();
    unfolded_mc_test_2 = unfold2.get_output_check();
    unfolded_mc_test_all_2 = unfold2.get_output_check_all();

    plot->Plot_output(unfolded_data_2, h_truth, false, save_TauScan_Unfolding);
    plot->Plot_output(unfolded_data_all_2, h_truth_all, false, save_TauScan_Unfolding_all);
    plot->Plot_covariance(covariance_input_2, save_TauScan_input_cov);
    plot->Plot_covariance(covariance_input_all_2, save_TauScan_input_cov_all);
    plot->Plot_covariance(covariance_matrix_2, save_TauScan_input_cov_matrix);
    plot->Plot_covariance(covariance_matrix_all_2, save_TauScan_input_cov_matrix_all);
    plot->Plot_covariance(covariance_total_2, save_TauScan_input_cov_total);
    plot->Plot_covariance(covariance_total_all_2, save_TauScan_input_cov_total_all);
    plot->Plot_output(unfolded_mc_test_2, h_trutch_check, false, save_TauScan_crosscheck);
    plot->Plot_output(unfolded_mc_test_all_2, h_trutch_check_all, false, save_TauScan_crosscheck_all);
    plot->Plot_output(unfolded_data_2, h_truth, true, save_TauScan_Unfolding_norm);
    // unfolded_data_2->Write("Unfolded Pseudodata TauScan");
    plot->Plot_output(unfolded_data_all_2, h_truth_all, true, save_TauScan_Unfolding_norm_all);
    plot->Plot_correlation_matrix(correlation_matrix_2, save_TauScan_Unfolding_correlation);
    plot->Plot_correlation_matrix(correlation_matrix_all_2, save_TauScan_Unfolding_correlation_all);
    plot->Plot_LCurve(lcurve_2, coordinates, coordinates_2, save_TauScan_LCurve);
    if(!do_lcurve_2) plot->Plot_RhoLogTau(rhologTau_2, tau_2, save_TauScan_RhoLogTau);
  }
  // delete binning_rec;
  // delete binning_gen;
  outputFile->Close();
  return 0;
}
