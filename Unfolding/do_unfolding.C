#include "do_unfolding.h"

using namespace std;
int counter = 1;

int main(int argc, char* argv[]){
  // switch on histogram errors
  if(argc < 4){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans> <value of tau>; if you don't want to use a custom value of tau, then set it to -1");
  }
  if(argv[1] != std::string("data") && argv[1] != std::string("data_sd") &&
  argv[1] != std::string("data_puppi") && argv[1] != std::string("data_puppi_sd") &&
  argv[1] != std::string("pseudo1") && argv[1] != std::string("pseudo1_sd") &&
  argv[1] != std::string("pseudo1_puppi") && argv[1] != std::string("pseudo1_puppi_sd") &&
  argv[1] != std::string("pseudo2") && argv[1] != std::string("pseudo2_sd") &&
  argv[1] != std::string("pseudo2_puppi") && argv[1] != std::string("pseudo2_puppi_sd") &&
  argv[1] != std::string("pseudo3") && argv[1] != std::string("pseudo3_sd") &&
  argv[1] != std::string("pseudo3_puppi") && argv[1] != std::string("pseudo3_puppi_sd") &&
  argv[1] != std::string("scale_dd") && argv[1] != std::string("scale_dd_sd") &&
  argv[1] != std::string("scale_dd_puppi") && argv[1] != std::string("scale_dd_puppi_sd") &&
  argv[1] != std::string("scale_dn") && argv[1] != std::string("scale_dn_sd") &&
  argv[1] != std::string("scale_dn_puppi") && argv[1] != std::string("scale_dn_puppi_sd") &&
  argv[1] != std::string("scale_nd") && argv[1] != std::string("scale_nd_sd") &&
  argv[1] != std::string("scale_nd_puppi") && argv[1] != std::string("scale_nd_puppi_sd") &&
  argv[1] != std::string("scale_nu") && argv[1] != std::string("scale_nu_sd") &&
  argv[1] != std::string("scale_nu_puppi") && argv[1] != std::string("scale_nu_puppi_sd") &&
  argv[1] != std::string("scale_uu") && argv[1] != std::string("scale_uu_sd") &&
  argv[1] != std::string("scale_uu_puppi") && argv[1] != std::string("scale_uu_puppi_sd") &&
  argv[1] != std::string("scale_un") && argv[1] != std::string("scale_un_sd") &&
  argv[1] != std::string("scale_un_puppi") && argv[1] != std::string("scale_un_puppi_sd") &&
  argv[1] != std::string("mtop1695") && argv[1] != std::string("mtop1695_sd") &&
  argv[1] != std::string("mtop1695_puppi") && argv[1] != std::string("mtop1695_puppi_sd") &&
  argv[1] != std::string("mtop1715") && argv[1] != std::string("mtop1715_sd") &&
  argv[1] != std::string("mtop1715_puppi") && argv[1] != std::string("mtop1715_puppi_sd") &&
  argv[1] != std::string("mtop1735") && argv[1] != std::string("mtop1735_sd") &&
  argv[1] != std::string("mtop1735_puppi") && argv[1] != std::string("mtop1735_puppi_sd") &&
  argv[1] != std::string("mtop1755") && argv[1] != std::string("mtop1755_sd") &&
  argv[1] != std::string("mtop1755_puppi") && argv[1] != std::string("mtop1755_puppi_sd") &&
  argv[1] != std::string("madgraph") && argv[1] != std::string("madgraph_sd") &&
  argv[1] != std::string("madgraph_puppi") && argv[1] != std::string("madgraph_puppi_sd")
){
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
background_names = {"DYJets", "ST", "Diboson"};
sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"MUScaleup", "MUScaledown"}, {"MUTriggerup", "MUTriggerdown"}, {"PUup", "PUdown"}};

// Setup everything for unfolding!
if(dataset == std::string("pseudo1")){
  data_File->GetObject("mc_matrix_1", mat_response);          // fill response matrix
  data_File->GetObject("Pseudodata_1", h_data);                  // fill histogram with data which should be unfolded
  data_File->GetObject("Pseudodata_dist_1", h_data_dist);           // fill histogram with data which should be unfolded
  data_File->GetObject("Pseudodata_truth_1", h_truth);
  data_File->GetObject("Pseudodata_truth_all_1", h_truth_all);
  data_File->GetObject("mc_gen_1", h_unfold);                 // fill histogram with what data should be unfolded
  data_File->GetObject("mc_rec_1", h_mc);                     // fill histogram to check if ratio between data and mc is appropiate
  data_File->GetObject("mc_rec_dist_1", h_mc_dist);
  data_File->GetObject("mc_truth_1", h_trutch_check);          // fill histogram to check if ratio between data and mc is appropiate
  data_File->GetObject("mc_truth_all_1", h_trutch_check_all);  // fill histogram to check if ratio between data and mc is appropiate
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
else if(dataset == std::string("mtop1695")){
  data_File->GetObject("mc_matrix", mat_response);
  data_File->GetObject("mtop1695_rec", h_data);
  data_File->GetObject("mtop1695_rec_dist", h_data_dist);
  data_File->GetObject("mtop1695_truth", h_truth);
  data_File->GetObject("mtop1695_truth_all", h_truth_all);
  data_File->GetObject("mc_gen", h_unfold);
  data_File->GetObject("mc_rec", h_mc);
  data_File->GetObject("mc_rec_dist", h_mc_dist);
  data_File->GetObject("mc_truth", h_trutch_check);
  data_File->GetObject("mc_truth_all", h_trutch_check_all);
  data_File->GetObject("mc_purity_all", h_purity_all);
  data_File->GetObject("mc_purity_samebin", h_purity_samebin);
}
else if(dataset == std::string("mtop1695_sd")){
  data_File->GetObject("mc_matrix_sd", mat_response);
  data_File->GetObject("mtop1695_rec_sd", h_data);
  data_File->GetObject("mtop1695_rec_dist_sd", h_data_dist);
  data_File->GetObject("mtop1695_truth_sd", h_truth);
  data_File->GetObject("mtop1695_truth_all_sd", h_truth_all);
  data_File->GetObject("mc_gen_sd", h_unfold);
  data_File->GetObject("mc_rec_sd", h_mc);
  data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
  data_File->GetObject("mc_truth_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1695_puppi")){
  data_File->GetObject("mc_matrix_puppi", mat_response);
  data_File->GetObject("mtop1695_rec_puppi", h_data);
  data_File->GetObject("mtop1695_rec_dist_puppi", h_data_dist);
  data_File->GetObject("mtop1695_truth_puppi", h_truth);
  data_File->GetObject("mtop1695_truth_all_puppi", h_truth_all);
  data_File->GetObject("mc_gen_puppi", h_unfold);
  data_File->GetObject("mc_rec_puppi", h_mc);
  data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
  data_File->GetObject("mc_truth_puppi", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
}
else if(dataset == std::string("mtop1695_puppi_sd")){
  data_File->GetObject("mc_matrix_puppi_sd", mat_response);
  data_File->GetObject("mtop1695_rec_puppi_sd", h_data);
  data_File->GetObject("mtop1695_rec_dist_puppi_sd", h_data_dist);
  data_File->GetObject("mtop1695_truth_puppi_sd", h_truth);
  data_File->GetObject("mtop1695_truth_all_puppi_sd", h_truth_all);
  data_File->GetObject("mc_gen_puppi_sd", h_unfold);
  data_File->GetObject("mc_rec_puppi_sd", h_mc);
  data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
  data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1715")){
  data_File->GetObject("mc_matrix", mat_response);
  data_File->GetObject("mtop1715_rec", h_data);
  data_File->GetObject("mtop1715_rec_dist", h_data_dist);
  data_File->GetObject("mtop1715_truth", h_truth);
  data_File->GetObject("mtop1715_truth_all", h_truth_all);
  data_File->GetObject("mc_gen", h_unfold);
  data_File->GetObject("mc_rec", h_mc);
  data_File->GetObject("mc_rec_dist", h_mc_dist);
  data_File->GetObject("mc_truth", h_trutch_check);
  data_File->GetObject("mc_truth_all", h_trutch_check_all);
  data_File->GetObject("mc_purity_all", h_purity_all);
  data_File->GetObject("mc_purity_samebin", h_purity_samebin);
}
else if(dataset == std::string("mtop1715_sd")){
  data_File->GetObject("mc_matrix_sd", mat_response);
  data_File->GetObject("mtop1715_rec_sd", h_data);
  data_File->GetObject("mtop1715_rec_dist_sd", h_data_dist);
  data_File->GetObject("mtop1715_truth_sd", h_truth);
  data_File->GetObject("mtop1715_truth_all_sd", h_truth_all);
  data_File->GetObject("mc_gen_sd", h_unfold);
  data_File->GetObject("mc_rec_sd", h_mc);
  data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
  data_File->GetObject("mc_truth_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1715_puppi")){
  data_File->GetObject("mc_matrix_puppi", mat_response);
  data_File->GetObject("mtop1715_rec_puppi", h_data);
  data_File->GetObject("mtop1715_rec_dist_puppi", h_data_dist);
  data_File->GetObject("mtop1715_truth_puppi", h_truth);
  data_File->GetObject("mtop1715_truth_all_puppi", h_truth_all);
  data_File->GetObject("mc_gen_puppi", h_unfold);
  data_File->GetObject("mc_rec_puppi", h_mc);
  data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
  data_File->GetObject("mc_truth_puppi", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
}
else if(dataset == std::string("mtop1715_puppi_sd")){
  data_File->GetObject("mc_matrix_puppi_sd", mat_response);
  data_File->GetObject("mtop1715_rec_puppi_sd", h_data);
  data_File->GetObject("mtop1715_rec_dist_puppi_sd", h_data_dist);
  data_File->GetObject("mtop1715_truth_puppi_sd", h_truth);
  data_File->GetObject("mtop1715_truth_all_puppi_sd", h_truth_all);
  data_File->GetObject("mc_gen_puppi_sd", h_unfold);
  data_File->GetObject("mc_rec_puppi_sd", h_mc);
  data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
  data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1735")){
  data_File->GetObject("mc_matrix", mat_response);
  data_File->GetObject("mtop1735_rec", h_data);
  data_File->GetObject("mtop1735_rec_dist", h_data_dist);
  data_File->GetObject("mtop1735_truth", h_truth);
  data_File->GetObject("mtop1735_truth_all", h_truth_all);
  data_File->GetObject("mc_gen", h_unfold);
  data_File->GetObject("mc_rec", h_mc);
  data_File->GetObject("mc_rec_dist", h_mc_dist);
  data_File->GetObject("mc_truth", h_trutch_check);
  data_File->GetObject("mc_truth_all", h_trutch_check_all);
  data_File->GetObject("mc_purity_all", h_purity_all);
  data_File->GetObject("mc_purity_samebin", h_purity_samebin);
}
else if(dataset == std::string("mtop1735_sd")){
  data_File->GetObject("mc_matrix_sd", mat_response);
  data_File->GetObject("mtop1735_rec_sd", h_data);
  data_File->GetObject("mtop1735_rec_dist_sd", h_data_dist);
  data_File->GetObject("mtop1735_truth_sd", h_truth);
  data_File->GetObject("mtop1735_truth_all_sd", h_truth_all);
  data_File->GetObject("mc_gen_sd", h_unfold);
  data_File->GetObject("mc_rec_sd", h_mc);
  data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
  data_File->GetObject("mc_truth_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1735_puppi")){
  data_File->GetObject("mc_matrix_puppi", mat_response);
  data_File->GetObject("mtop1735_rec_puppi", h_data);
  data_File->GetObject("mtop1735_rec_dist_puppi", h_data_dist);
  data_File->GetObject("mtop1735_truth_puppi", h_truth);
  data_File->GetObject("mtop1735_truth_all_puppi", h_truth_all);
  data_File->GetObject("mc_gen_puppi", h_unfold);
  data_File->GetObject("mc_rec_puppi", h_mc);
  data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
  data_File->GetObject("mc_truth_puppi", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
}
else if(dataset == std::string("mtop1735_puppi_sd")){
  data_File->GetObject("mc_matrix_puppi_sd", mat_response);
  data_File->GetObject("mtop1735_rec_puppi_sd", h_data);
  data_File->GetObject("mtop1735_rec_dist_puppi_sd", h_data_dist);
  data_File->GetObject("mtop1735_truth_puppi_sd", h_truth);
  data_File->GetObject("mtop1735_truth_all_puppi_sd", h_truth_all);
  data_File->GetObject("mc_gen_puppi_sd", h_unfold);
  data_File->GetObject("mc_rec_puppi_sd", h_mc);
  data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
  data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1755")){
  data_File->GetObject("mc_matrix", mat_response);
  data_File->GetObject("mtop1755_rec", h_data);
  data_File->GetObject("mtop1755_rec_dist", h_data_dist);
  data_File->GetObject("mtop1755_truth", h_truth);
  data_File->GetObject("mtop1755_truth_all", h_truth_all);
  data_File->GetObject("mc_gen", h_unfold);
  data_File->GetObject("mc_rec", h_mc);
  data_File->GetObject("mc_rec_dist", h_mc_dist);
  data_File->GetObject("mc_truth", h_trutch_check);
  data_File->GetObject("mc_truth_all", h_trutch_check_all);
  data_File->GetObject("mc_purity_all", h_purity_all);
  data_File->GetObject("mc_purity_samebin", h_purity_samebin);
}
else if(dataset == std::string("mtop1755_sd")){
  data_File->GetObject("mc_matrix_sd", mat_response);
  data_File->GetObject("mtop1755_rec_sd", h_data);
  data_File->GetObject("mtop1755_rec_dist_sd", h_data_dist);
  data_File->GetObject("mtop1755_truth_sd", h_truth);
  data_File->GetObject("mtop1755_truth_all_sd", h_truth_all);
  data_File->GetObject("mc_gen_sd", h_unfold);
  data_File->GetObject("mc_rec_sd", h_mc);
  data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
  data_File->GetObject("mc_truth_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
}
else if(dataset == std::string("mtop1755_puppi")){
  data_File->GetObject("mc_matrix_puppi", mat_response);
  data_File->GetObject("mtop1755_rec_puppi", h_data);
  data_File->GetObject("mtop1755_rec_dist_puppi", h_data_dist);
  data_File->GetObject("mtop1755_truth_puppi", h_truth);
  data_File->GetObject("mtop1755_truth_all_puppi", h_truth_all);
  data_File->GetObject("mc_gen_puppi", h_unfold);
  data_File->GetObject("mc_rec_puppi", h_mc);
  data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
  data_File->GetObject("mc_truth_puppi", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
}
else if(dataset == std::string("mtop1755_puppi_sd")){
  data_File->GetObject("mc_matrix_puppi_sd", mat_response);
  data_File->GetObject("mtop1755_rec_puppi_sd", h_data);
  data_File->GetObject("mtop1755_rec_dist_puppi_sd", h_data_dist);
  data_File->GetObject("mtop1755_truth_puppi_sd", h_truth);
  data_File->GetObject("mtop1755_truth_all_puppi_sd", h_truth_all);
  data_File->GetObject("mc_gen_puppi_sd", h_unfold);
  data_File->GetObject("mc_rec_puppi_sd", h_mc);
  data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
  data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
}
else if(dataset == std::string("madgraph")){
  data_File->GetObject("mc_matrix", mat_response);
  data_File->GetObject("madgraph_rec", h_data);
  data_File->GetObject("madgraph_rec_dist", h_data_dist);
  data_File->GetObject("madgraph_truth", h_truth);
  data_File->GetObject("madgraph_truth_all", h_truth_all);
  data_File->GetObject("mc_gen", h_unfold);
  data_File->GetObject("mc_rec", h_mc);
  data_File->GetObject("mc_rec_dist", h_mc_dist);
  data_File->GetObject("mc_truth", h_trutch_check);
  data_File->GetObject("mc_truth_all", h_trutch_check_all);
  data_File->GetObject("mc_purity_all", h_purity_all);
  data_File->GetObject("mc_purity_samebin", h_purity_samebin);
}
else if(dataset == std::string("madgraph_sd")){
  data_File->GetObject("mc_matrix_sd", mat_response);
  data_File->GetObject("madgraph_rec_sd", h_data);
  data_File->GetObject("madgraph_rec_dist_sd", h_data_dist);
  data_File->GetObject("madgraph_truth_sd", h_truth);
  data_File->GetObject("madgraph_truth_all_sd", h_truth_all);
  data_File->GetObject("mc_gen_sd", h_unfold);
  data_File->GetObject("mc_rec_sd", h_mc);
  data_File->GetObject("mc_rec_dist_sd", h_mc_dist);
  data_File->GetObject("mc_truth_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_sd", h_purity_samebin);
}
else if(dataset == std::string("madgraph_puppi")){
  data_File->GetObject("mc_matrix_puppi", mat_response);
  data_File->GetObject("madgraph_rec_puppi", h_data);
  data_File->GetObject("madgraph_rec_dist_puppi", h_data_dist);
  data_File->GetObject("madgraph_truth_puppi", h_truth);
  data_File->GetObject("madgraph_truth_all_puppi", h_truth_all);
  data_File->GetObject("mc_gen_puppi", h_unfold);
  data_File->GetObject("mc_rec_puppi", h_mc);
  data_File->GetObject("mc_rec_dist_puppi", h_mc_dist);
  data_File->GetObject("mc_truth_puppi", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi", h_purity_samebin);
}
else if(dataset == std::string("madgraph_puppi_sd")){
  data_File->GetObject("mc_matrix_puppi_sd", mat_response);
  data_File->GetObject("madgraph_rec_puppi_sd", h_data);
  data_File->GetObject("madgraph_rec_dist_puppi_sd", h_data_dist);
  data_File->GetObject("madgraph_truth_puppi_sd", h_truth);
  data_File->GetObject("madgraph_truth_all_puppi_sd", h_truth_all);
  data_File->GetObject("mc_gen_puppi_sd", h_unfold);
  data_File->GetObject("mc_rec_puppi_sd", h_mc);
  data_File->GetObject("mc_rec_dist_puppi_sd", h_mc_dist);
  data_File->GetObject("mc_truth_puppi_sd", h_trutch_check);
  data_File->GetObject("mc_truth_all_puppi_sd", h_trutch_check_all);
  data_File->GetObject("mc_purity_all_puppi_sd", h_purity_all);
  data_File->GetObject("mc_purity_samebin_puppi_sd", h_purity_samebin);
}
background.clear();
for(unsigned int i = 0; i < background_names.size(); i++){
  cout << "Background name: " << background_names.at(i) << '\n';
  if (dataset.Contains("_puppi_sd")) background.push_back((TH1D*) data_File->Get("Background_puppi_sd_" + background_names.at(i)));
  else if (dataset.Contains("_puppi")) background.push_back((TH1D*) data_File->Get("Background_puppi_" + background_names.at(i)));
  else if (dataset.Contains("_sd")) background.push_back((TH1D*) data_File->Get("Background_sd_" + background_names.at(i)));
  else  background.push_back((TH1D*) data_File->Get("Background_" + background_names.at(i)));
}
sys_matrix.clear();
for(unsigned int i = 0; i < sys_name.size(); i++){
  for(unsigned int j = 0; j < sys_name.at(i).size(); j++){
    vector<TH2*> dummy;
    sys_matrix.push_back(dummy);
    if (dataset.Contains("_puppi_sd")) sys_matrix[i].push_back((TH2*) data_File->Get(sys_name[i][j] + "_matrix" + "_puppi_sd"));
    else if (dataset.Contains("_puppi")) sys_matrix[i].push_back((TH2*) data_File->Get(sys_name[i][j] + "_matrix" + "_puppi"));
    else if (dataset.Contains("_sd")) sys_matrix[i].push_back((TH2*) data_File->Get(sys_name[i][j] + "_matrix" + "_sd"));
    else  sys_matrix[i].push_back((TH2*) data_File->Get(sys_name[i][j] + "_matrix"));
  }
}

subtract_background = false;
if(dataset == std::string("data") || dataset == std::string("data_sd") || dataset == std::string("data_puppi") || dataset == std::string("data_puppi_sd") || dataset == std::string("madgraph") || dataset == std::string("madgraph_sd") || dataset == std::string("madgraph_puppi") || dataset == std::string("madgraph_puppi_sd")){
  subtract_background = true;
}
// cout << "subtract_background: " << subtract_background << '\n';
TString save_dir = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
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
unfold.get_output_meas();
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
unfold.get_probability_matrix();
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
  unfold2.get_output_meas();
  unfold2.get_output();
  unfold2.get_output_all();
  unfold2.get_input_statcov();
  unfold2.get_input_statcov_meas();
  unfold2.get_input_statcov_all();
  unfold2.get_matrix_statcov();
  unfold2.get_matrix_statcov_meas();
  unfold2.get_matrix_statcov_all();
  unfold2.get_total_statcov();
  unfold2.get_total_statcov_meas();
  unfold2.get_total_statcov_all();
  unfold2.get_correlation();
  unfold2.get_correlation_meas();
  unfold2.get_correlation_all();
  unfold2.get_output_check();
  unfold2.get_output_check_meas();
  unfold2.get_output_check_all();
  unfold2.get_sys_covariance();
  unfold2.get_sys_covariance_meas();
  unfold2.get_sys_covariance_all();
  unfold2.get_sys_delta();
  unfold2.get_sys_delta_meas();
  unfold2.get_sys_delta_all();
  unfold2.get_probability_matrix();
  if(subtract_background){
    unfold2.GetBgrStatCov();
    unfold2.GetBgrStatCov_meas();
    unfold2.GetBgrStatCov_all();
    unfold2.GetBgrScaleCov();
    unfold2.GetBgrScaleCov_meas();
    unfold2.GetBgrScaleCov_all();
    unfold2.get_bgr_delta();
    unfold2.get_bgr_delta_meas();
    unfold2.get_bgr_delta_all();
  }
}
h_data->Write("Data input");
h_data_dist->Write("Data Distribution");
h_mc->Write("MC rec input");
h_mc_dist->Write("MC rec Distribution");
h_truth->Write("Data Truth");
h_truth_all->Write("Whole Data Truth");
h_trutch_check->Write("MC Truth");
h_trutch_check_all->Write("Whole MC Truth");
h_unfold->Write("MC gen Distribution");
mat_response->Write("response matrix");
h_purity_all->Write("Purity total");
h_purity_samebin->Write("Purity samebin");

outputFile->Close();
return 0;
}
