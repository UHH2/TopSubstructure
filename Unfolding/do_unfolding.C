#include "do_unfolding.h"

using namespace std;
int counter = 1;

int main(int argc, char* argv[]){
  // switch on histogram errors
  if(argc < 5){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans> <value of tau> <subtract_background>; if you don't want to use a custom value of tau, then set it to -1");
  }
  if(argv[1] != std::string("data") && argv[1] != std::string("data_sd") && argv[1] != std::string("data_puppi") && argv[1] != std::string("data_puppi_sd") && argv[1] != std::string("pseudo1") && argv[1] != std::string("pseudo1_sd") && argv[1] != std::string("pseudo1_puppi") && argv[1] != std::string("pseudo1_puppi_sd") && argv[1] != std::string("pseudo2") && argv[1] != std::string("pseudo2_sd") && argv[1] != std::string("pseudo2_puppi") && argv[1] != std::string("pseudo2_puppi_sd") && argv[1] != std::string("pseudo3") && argv[1] != std::string("pseudo3_sd") && argv[1] != std::string("pseudo3_puppi") && argv[1] != std::string("pseudo3_puppi_sd") && argv[1] != std::string("scale_dd") && argv[1] != std::string("scale_dd_sd") && argv[1] != std::string("scale_dd_puppi") && argv[1] != std::string("scale_dd_puppi_sd") && argv[1] != std::string("scale_dn") && argv[1] != std::string("scale_dn_sd") && argv[1] != std::string("scale_dn_puppi") && argv[1] != std::string("scale_dn_puppi_sd") && argv[1] != std::string("scale_nd") && argv[1] != std::string("scale_nd_sd") && argv[1] != std::string("scale_nd_puppi") && argv[1] != std::string("scale_nd_puppi_sd") && argv[1] != std::string("scale_nu") && argv[1] != std::string("scale_nu_sd") && argv[1] != std::string("scale_nu_puppi") && argv[1] != std::string("scale_nu_puppi_sd") && argv[1] != std::string("scale_uu") && argv[1] != std::string("scale_uu_sd") && argv[1] != std::string("scale_uu_puppi") && argv[1] != std::string("scale_uu_puppi_sd") && argv[1] != std::string("scale_un") && argv[1] != std::string("scale_un_sd") && argv[1] != std::string("scale_un_puppi") && argv[1] != std::string("scale_un_puppi_sd")){
    throw runtime_error("use pseudo1, pseudo1_sd, pseudo1_puppi, pseudo1_puppi_sd, ...");
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
  // background_names = {"DYJets", "QCD", "ST", "WJets", "Diboson_WW", "Diboson_WZ", "Diboson_ZZ"};
  background_names = {"DYJets", "ST"};
  sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}};

  // Setup everything for unfolding!
  if(dataset == std::string("pseudo1")){
    data_File->GetObject("mc_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_1", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
    data_File->GetObject("mc_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_1", h_mc_dist);
    data_File->GetObject("mc_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_1", h_purity_all);
    data_File->GetObject("mc_purity_samebin_1", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo1_sd")){
    data_File->GetObject("mc_matrix_sd_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_sd_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_sd_1", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_sd_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_sd_1", h_truth_all);
    data_File->GetObject("mc_gen_sd_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd_1", h_mc_dist);
    data_File->GetObject("mc_truth_sd_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd_1", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd_1", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo1_puppi")){
    data_File->GetObject("mc_matrix_puppi_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_1", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_1", h_truth_all);
    data_File->GetObject("mc_gen_puppi_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_1", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_1", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_1", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo1_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd_1", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_sd_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_sd_1", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_sd_1", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_sd_1", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd_1", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd_1", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd_1", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd_1", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo2")){
    data_File->GetObject("mc_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_2", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_2", h_truth_all);
    data_File->GetObject("mc_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_2", h_mc_dist);
    data_File->GetObject("mc_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_2", h_purity_all);
    data_File->GetObject("mc_purity_samebin_2", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo2_sd")){
    data_File->GetObject("mc_matrix_sd_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_sd_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_sd_2", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_sd_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_sd_2", h_truth_all);
    data_File->GetObject("mc_gen_sd_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd_2", h_mc_dist);
    data_File->GetObject("mc_truth_sd_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd_2", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd_2", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo2_puppi")){
    data_File->GetObject("mc_matrix_puppi_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_2", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_2", h_truth_all);
    data_File->GetObject("mc_gen_puppi_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_2", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_2", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_2", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo2_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd_2", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_sd_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_sd_2", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_sd_2", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_sd_2", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd_2", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd_2", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd_2", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd_2", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo3")){
    data_File->GetObject("mc_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_3", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_3", h_truth);              // fill histogram with truth
    data_File->GetObject("Pseudodata_truth_all_3", h_truth_all);
    data_File->GetObject("mc_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_3", h_mc_dist);
    data_File->GetObject("mc_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_3", h_purity_all);
    data_File->GetObject("mc_purity_samebin_3", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo3_sd")){
    data_File->GetObject("mc_matrix_sd_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_sd_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_sd_3", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_sd_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_sd_3", h_truth_all);
    data_File->GetObject("mc_gen_sd_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd_3", h_mc_dist);
    data_File->GetObject("mc_truth_sd_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd_3", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd_3", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo3_puppi")){
    data_File->GetObject("mc_matrix_puppi_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_3", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_3", h_truth_all);
    data_File->GetObject("mc_gen_puppi_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_3", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_3", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_3", h_purity_samebin);
  }
  else if(dataset == std::string("pseudo3_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd_3", mat_response);             // fill response matrix
    data_File->GetObject("Pseudodata_puppi_sd_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_dist_puppi_sd_3", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Pseudodata_truth_puppi_sd_3", h_truth);
    data_File->GetObject("Pseudodata_truth_all_puppi_sd_3", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd_3", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd_3", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd_3", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd_3", h_purity_samebin);
  }
  else if(dataset == std::string("scale_uu")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupup", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_truth", h_truth);
    data_File->GetObject("SCALEupup_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_uu_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupup_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_truth_sd", h_truth);
    data_File->GetObject("SCALEupup_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_uu_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupup_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_truth_puppi", h_truth);
    data_File->GetObject("SCALEupup_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_uu_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupup_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupup_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEupup_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_un")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupnone", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_truth", h_truth);
    data_File->GetObject("SCALEupnone_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_un_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupnone_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_truth_sd", h_truth);
    data_File->GetObject("SCALEupnone_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_un_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupnone_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_truth_puppi", h_truth);
    data_File->GetObject("SCALEupnone_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_un_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEupnone_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEupnone_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEupnone_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nu")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnoneup", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_truth", h_truth);
    data_File->GetObject("SCALEnoneup_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nu_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnoneup_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_truth_sd", h_truth);
    data_File->GetObject("SCALEnoneup_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nu_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnoneup_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_truth_puppi", h_truth);
    data_File->GetObject("SCALEnoneup_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nu_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnoneup_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnoneup_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEnoneup_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nd")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnonedown", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_truth", h_truth);
    data_File->GetObject("SCALEnonedown_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nd_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnonedown_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_truth_sd", h_truth);
    data_File->GetObject("SCALEnonedown_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nd_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnonedown_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_truth_puppi", h_truth);
    data_File->GetObject("SCALEnonedown_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_nd_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEnonedown_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEnonedown_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEnonedown_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dn")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdownnone", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_truth", h_truth);
    data_File->GetObject("SCALEdownnone_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dn_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdownnone_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_truth_sd", h_truth);
    data_File->GetObject("SCALEdownnone_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dn_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdownnone_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_truth_puppi", h_truth);
    data_File->GetObject("SCALEdownnone_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dn_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdownnone_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdownnone_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEdownnone_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dd")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdowndown", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_truth", h_truth);
    data_File->GetObject("SCALEdowndown_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dd_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdowndown_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_truth_sd", h_truth);
    data_File->GetObject("SCALEdowndown_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dd_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdowndown_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_truth_puppi", h_truth);
    data_File->GetObject("SCALEdowndown_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("scale_dd_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("SCALEdowndown_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("SCALEdowndown_truth_puppi_sd", h_truth);
    data_File->GetObject("SCALEdowndown_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }
  else if(dataset == std::string("data")){
    data_File->GetObject("mc_matrix", mat_response);             // fill response matrix
    data_File->GetObject("Data", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Data_dist", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("mc_truth", h_truth);
    data_File->GetObject("mc_truth_all", h_truth_all);
    data_File->GetObject("mc_gen", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist", h_mc_dist);
    data_File->GetObject("mc_truth", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all", h_purity_all);
    data_File->GetObject("mc_purity_samebin", h_purity_samebin);
  }
  else if(dataset == std::string("data_sd")){
    data_File->GetObject("mc_matrix_sd", mat_response);             // fill response matrix
    data_File->GetObject("Data_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Data_dist_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("mc_truth_sd", h_truth);
    data_File->GetObject("mc_truth_all_sd", h_truth_all);
    data_File->GetObject("mc_gen_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
    data_File->GetObject("mc_truth_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
  }
  else if(dataset == std::string("data_puppi")){
    data_File->GetObject("mc_matrix_puppi", mat_response);             // fill response matrix
    data_File->GetObject("Data_puppi", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Data_dist_puppi", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("mc_truth_puppi", h_truth);
    data_File->GetObject("mc_truth_all_puppi", h_truth_all);
    data_File->GetObject("mc_gen_puppi", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
    data_File->GetObject("mc_truth_puppi", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
  }
  else if(dataset == std::string("data_puppi_sd")){
    data_File->GetObject("mc_matrix_puppi_sd", mat_response);             // fill response matrix
    data_File->GetObject("Data_puppi_sd", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("Data_dist_puppi_sd", h_data_dist);                     // fill histogram with data which should be unfolded
    data_File->GetObject("mc_truth_puppi_sd", h_truth);
    data_File->GetObject("mc_truth_all_puppi_sd", h_truth_all);
    data_File->GetObject("mc_gen_puppi_sd", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("mc_rec_puppi_sd", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
    data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
    data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
  }

  for(unsigned int i = 0; i < background_names.size(); i++){
    cout << "Background name: " << background_names.at(i) << '\n';
    if (dataset.Contains("_puppi_sd")) background.push_back((TH1D*) data_File->Get("Background_puppi_sd_" + background_names.at(i)));
    else if (dataset.Contains("_puppi")) background.push_back((TH1D*) data_File->Get("Background_puppi_" + background_names.at(i)));
    else if (dataset.Contains("_sd")) background.push_back((TH1D*) data_File->Get("Background_sd_" + background_names.at(i)));
    else  background.push_back((TH1D*) data_File->Get("Background_" + background_names.at(i)));
  }

  for(unsigned int i = 0; i < sys_name.size(); i++){
    for(unsigned int j = 0; j < sys_name.at(i).size(); j++){
      vector<TH2*> dummy;
      sys_matrix.push_back(dummy);
      sys_matrix[i].push_back((TH2*)data_File->Get(sys_name[i][j] + "_matrix"));
    }
  }
  std::stringstream ss(argv[4]);
  // subtract_background = false;
  if(!(ss >> std::boolalpha >> subtract_background)) {
    throw runtime_error("no bool :(");
  }
  cout << "subtract_background: " << subtract_background << '\n';
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
  TString folder = argv[1];
  save_dir += folder+"/";

  if(tau_value < 0){
    save_LCurve_Unfolding = "/LCurve/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_all = "/LCurve/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm = "/LCurve/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm_all = "/LCurve/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation = "/LCurve/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation_all = "/LCurve/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauX = "/LCurve/TauX_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauY = "/LCurve/TauY_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_crosscheck = "/LCurve/crosscheck_" + dataset + format;
    save_LCurve_crosscheck_all = "/LCurve/crosscheck_all_" + dataset + format;
    save_LCurve_input_cov = "/LCurve/Covariance_input_" + dataset + format;
    save_LCurve_input_cov_all = "/LCurve/Covariance_input_all_" + dataset + format;
    save_LCurve_input_cov_matrix = "/LCurve/Covariance_matrix_" + dataset + format;
    save_LCurve_input_cov_matrix_all = "/LCurve/Covariance_matrix_all_" + dataset + format;
    save_LCurve_input_cov_total = "/LCurve/Covariance_total_" + dataset + format;
    save_LCurve_input_cov_total_all = "/LCurve/Covariance_total_all_" + dataset + format;

    save_TauScan_LCurve = "/TauScan/LCurve_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_RhoLogTau = "/TauScan/RhoLogTau_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding = "/TauScan/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_all = "/TauScan/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_norm = "/TauScan/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_norm_all = "/TauScan/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_correlation = "/TauScan/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_correlation_all = "/TauScan/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_crosscheck = "/TauScan/crosscheck_" + dataset + format;
    save_TauScan_crosscheck_all = "/TauScan/crosscheck_all_" + dataset + format;
    save_TauScan_input_cov = "/TauScan/Covariance_input_" + dataset + format;
    save_TauScan_input_cov_all = "/TauScan/Covariance_input_all_" + dataset + format;
    save_TauScan_input_cov_matrix = "/TauScan/Covariance_matrix_" + dataset + format;
    save_TauScan_input_cov_matrix_all = "/TauScan/Covariance_matrix_all_" + dataset + format;
    save_TauScan_input_cov_total = "/TauScan/Covariance_total_" + dataset + format;
    save_TauScan_input_cov_total_all = "/TauScan/Covariance_total_all_" + dataset + format;
  }
  else{
    save_LCurve_Unfolding = "/CustomTau/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_all = "/CustomTau/Unfolding_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm = "/CustomTau/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_norm_all = "/CustomTau/Unfolding_norm_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation = "/CustomTau/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_correlation_all = "/CustomTau/Unfolding_correlation_all_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_crosscheck = "/CustomTau/crosscheck_" + dataset  + format;
    save_LCurve_crosscheck_all = "/CustomTau/crosscheck_all_" + dataset + format;
    save_LCurve_input_cov = "/CustomTau/Covariance_input_" + dataset + format;
    save_LCurve_input_cov_all = "/CustomTau/Covariance_input_all_" + dataset + format;
    save_LCurve_input_cov_matrix = "/CustomTau/Covariance_matrix_" + dataset + format;
    save_LCurve_input_cov_matrix_all = "/CustomTau/Covariance_matrix_all_" + dataset + format;
    save_LCurve_input_cov_total = "/CustomTau/Covariance_total_" + dataset + format;
    save_LCurve_input_cov_total_all = "/CustomTau/Covariance_total_all_" + dataset + format;
  }
  save_response_matrix = "/Response_Matrix_" + dataset + format;
  save_projection_x = "/Projection_X_" + dataset + format;
  save_projection_y = "/Projection_Y_" + dataset + format;
  save_purity = "/Purity_" + dataset + format;
  save_input = "/Input_" + dataset + format;
  save_input_dist = "/Input_dist_" + dataset + format;
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
  unfolding unfold(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, sys_matrix, sys_name, background, background_names, nscan, regmode, density_flag, do_lcurve, subtract_background, tau_value);

  unfold.get_output();
  unfold.get_output_all();
  unfold.get_correlation();
  unfold.get_correlation_all();
  unfold.get_input_statcov();
  unfold.get_input_statcov_all();
  unfold.get_matrix_statcov();
  unfold.get_matrix_statcov_all();
  unfold.get_total_statcov();
  unfold.get_total_statcov_all();
  unfold.get_output_check();
  unfold.get_output_check_all();
  unfold.get_sys_delta();
  if(subtract_background) unfold.GetBgrStatCov();
  if(subtract_background) unfold.GetBgrScaleCov();
  if(subtract_background) unfold.get_bgr_delta();

  if(tau_value < 0){
    unfold.get_lcurve();
    unfold.get_coordinates();
    unfold.get_tau();
    unfold.get_logtau();
    if(!do_lcurve) {
      unfold.get_rhologtau();
    }
  }
  unfold.check_projection();

  if(tau_value < 0){
    unfolding unfold2(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, sys_matrix, sys_name, background, background_names, nscan, regmode, density_flag, do_lcurve_2, subtract_background, tau_value);

    unfold2.get_coordinates();
    unfold2.get_lcurve();
    unfold2.get_tau();
    unfold2.get_logtau();
    if(!do_lcurve_2) unfold2.get_rhologtau();
    unfold2.get_output();
    unfold2.get_input_statcov();
    unfold2.get_input_statcov_all();
    unfold2.get_matrix_statcov();
    unfold2.get_matrix_statcov_all();
    unfold2.get_total_statcov();
    unfold2.get_total_statcov_all();
    unfold2.get_output_all();
    unfold2.get_correlation();
    unfold2.get_correlation_all();
    unfold2.get_output_check();
    unfold2.get_output_check_all();
    unfold2.get_sys_covariance();
    unfold2.get_sys_delta();
    if(subtract_background) unfold2.GetBgrStatCov();
    if(subtract_background) unfold2.GetBgrScaleCov();
    if(subtract_background) unfold2.get_bgr_delta();
  }

  // // add up stat Cov
  // cout << "sum up stat cov matrices" << endl;
  // CovStat = (TH2*) covariance_input_2->Clone();
  // CovStat->Add(covariance_matrix_2);
  // for(auto bgrcov: CovBgrStat) CovStat->Add(bgrcov);
  // stat_delta = CreateDeltaFromCov(CovStat);
  // stat_rel = ConvertToRelative(stat_delta, unfolded_data_2);
  // // then add sys cov from backgrounds
  // cout << "sum up background sys cov matrices" << endl;
  // total_cov = (TH2*) CovStat->Clone();
  // for(auto bgrcov: CovBgrScale) total_cov->Add(bgrcov);
  // cov_sys = (TH2*) CovStat->Clone();
  // cov_sys->Reset();
  //
  // // then add sys cov (and convert used uncertainty to relative hist)
  // cout << "sum up experimental sys cov matrices" << endl;
  // for(unsigned int i=0; i<sys_del.size(); i++){
  //   int j = FindLargestVariation(sys_del[i]);
  //   total_cov->Add(sys_cov[i][j]);
  //   cov_sys->Add(sys_cov[i][j]);
  //   sys_rel.push_back(ConvertToRelative(sys_del[i][j], unfolded_data_2));
  //   cout << "using " << sys_name[i][j] << endl;
  // }
  // sys_rel.push_back(stat_rel);      // put in stat to get total
  // sys_rel_total = AddSys(sys_rel);  // calculate total
  // sys_rel.pop_back();               // remove stat from list
  //
  //
  // data_unfolded_sys = SetSysError(unfolded_data_2, total_cov);
  // data_unfolded_sys->SetName("Unfolded Data with sys errors");
  // data_unfolded_sys->Write();

  // TCanvas* c_sys_cov = new TCanvas("covariances sys", "cov", 800, 600);
  // gStyle->SetPadLeftMargin(0.1);
  // gStyle->SetPadRightMargin(0.12);
  // c_sys_cov->Divide(2, 2);
  // c_sys_cov->cd(1);
  // sys_cov[0][0]->Draw("colz");
  // c_sys_cov->cd(2);
  // sys_cov[0][1]->Draw("colz");
  // c_sys_cov->cd(3);
  // sys_cov[1][0]->Draw("colz");
  // c_sys_cov->cd(4);
  // sys_cov[1][1]->Draw("colz");
  // c_sys_cov->cd(1)->SaveAs(save_dir+"/JECup_cov.eps");
  // c_sys_cov->cd(2)->SaveAs(save_dir+"/JECdown_cov.eps");
  // c_sys_cov->cd(3)->SaveAs(save_dir+"/JERup_cov.eps");
  // c_sys_cov->cd(4)->SaveAs(save_dir+"/JERdown_cov.eps");

  // if(subtract_background){
  //     TCanvas* c_background_cov = new TCanvas("covariances background", "bck", 800, 600);
  //     gStyle->SetPadLeftMargin(0.1);
  //     gStyle->SetPadRightMargin(0.12);
  //     c_background_cov->Divide(2,2);
  //     c_background_cov->cd(1);
  //     CovBgrStat[0]->Draw("colz");
  //     c_background_cov->cd(2);
  //     CovBgrStat[1]->Draw("colz");
  //     c_background_cov->cd(3);
  //     CovBgrScale[0]->Draw("colz");
  //     c_background_cov->cd(4);
  //     CovBgrScale[1]->Draw("colz");
  //     c_background_cov->cd(1)->SaveAs(save_dir+"/BgrStat_DY.eps");
  //     c_background_cov->cd(2)->SaveAs(save_dir+"/BgrStat_ST.eps");
  //     c_background_cov->cd(3)->SaveAs(save_dir+"/BgrScale_DY.eps");
  //     c_background_cov->cd(4)->SaveAs(save_dir+"/BgrScale_ST.eps");
  // }
  // TCanvas* blub = new TCanvas("muh", "muhmuh", 800, 600);
  // blub->cd();
  // h_truth->GetYaxis()->SetRangeUser(0, 2000);
  // h_truth->GetYaxis()->SetTitle("Events");
  // h_truth->SetTitle("");
  // h_truth->GetYaxis()->SetTitleSize(0.06);
  // h_truth->GetYaxis()->SetTitleOffset(0.8);
  // h_truth->GetXaxis()->SetTitleSize(0.06);
  // h_truth->GetXaxis()->SetTitleOffset(0.7);
  // h_truth->Draw("hist");
  // h_trutch_check->SetLineColor(kViolet);
  // h_trutch_check->SetMarkerColor(kViolet);
  // h_trutch_check->Draw("same");
  // data_unfolded_sys->SetLineColor(kRed);
  // data_unfolded_sys->SetMarkerColor(kRed);
  // data_unfolded_sys->Draw("same E1");
  // unfolded_data_2->SetLineColor(kRed);
  // unfolded_data_2->SetMarkerColor(kRed);
  // unfolded_data_2->Draw("same E1");
  // TLegend *legend = new TLegend(0.15, 0.52, 0.45, 0.72, "");
  // legend->SetBorderSize(0);
  // legend->SetFillStyle(0);
  // legend->AddEntry(h_truth, "Truth", "l");
  // legend->AddEntry(h_trutch_check, "second Truth", "l");
  // legend->AddEntry(data_unfolded_sys, "Unfolded Data", "l");
  // legend->Draw();
  // blub->SaveAs(save_dir+"/added_uncer.eps");
  // blub->Delete();

  // TCanvas* blub2 = new TCanvas("muh2", "muhmuh2", 800, 600);
  // blub2->Divide(2, 2);
  // blub2->cd(1);
  // sys_del[0][0]->GetYaxis()->SetRangeUser(-100, 0);
  // sys_del[0][0]->Draw();
  // blub2->cd(2);
  // sys_del[0][1]->GetYaxis()->SetRangeUser(0, 100);
  // sys_del[0][1]->Draw();
  // blub2->cd(1)->SaveAs(save_dir+"/JECup.eps");
  // blub2->cd(2)->SaveAs(save_dir+"/JECdown.eps");
  // blub2->cd(3);
  // sys_del[1][0]->GetYaxis()->SetRangeUser(-30, 30);
  // sys_del[1][0]->Draw();
  // blub2->cd(4);
  // sys_del[1][1]->GetYaxis()->SetRangeUser(-30, 30);
  // sys_del[1][1]->Draw();
  // blub2->cd(3)->SaveAs(save_dir+"/JERup.eps");
  // blub2->cd(4)->SaveAs(save_dir+"/JERdown.eps");
  //
  // delete binning_rec;
  // delete binning_gen;
  outputFile->Close();
  return 0;
}
