#include "do_unfolding.h"

using namespace std;
int counter = 1;

int main(int argc, char* argv[]){
  // switch on histogram errors
  if(argc != 5){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans> <value of tau> <mu/ele/comb>; if you don't want to use a custom value of tau, then set it to -1");
  }
  if(argv[1] != std::string("Data") && argv[1] != std::string("Data_sd") &&
  argv[1] != std::string("Data_puppi") && argv[1] != std::string("Data_puppi_sd") &&
  argv[1] != std::string("Pseudo_1") && argv[1] != std::string("Pseudo_1_sd") &&
  argv[1] != std::string("Pseudo_1_puppi") && argv[1] != std::string("Pseudo_1_puppi_sd") &&
  argv[1] != std::string("Pseudo_2") && argv[1] != std::string("Pseudo_2_sd") &&
  argv[1] != std::string("Pseudo_2_puppi") && argv[1] != std::string("Pseudo_2_puppi_sd") &&
  argv[1] != std::string("Pseudo_3") && argv[1] != std::string("Pseudo_3_sd") &&
  argv[1] != std::string("Pseudo_3_puppi") && argv[1] != std::string("Pseudo_3_puppi_sd") &&
  argv[1] != std::string("SCALEdowndown") && argv[1] != std::string("SCALEdowndown_sd") &&
  argv[1] != std::string("SCALEdowndown_puppi") && argv[1] != std::string("SCALEdowndown_puppi_sd") &&
  argv[1] != std::string("SCALEdownnone") && argv[1] != std::string("SCALEdownnone_sd") &&
  argv[1] != std::string("SCALEdownnone_puppi") && argv[1] != std::string("SCALEdownnone_puppi_sd") &&
  argv[1] != std::string("SCALEnonedown") && argv[1] != std::string("SCALEnonedown_sd") &&
  argv[1] != std::string("SCALEnonedown_puppi") && argv[1] != std::string("SCALEnonedown_puppi_sd") &&
  argv[1] != std::string("SCALEnoneup") && argv[1] != std::string("SCALEnoneup_sd") &&
  argv[1] != std::string("SCALEnoneup_puppi") && argv[1] != std::string("SCALEnoneup_puppi_sd") &&
  argv[1] != std::string("SCALEupup") && argv[1] != std::string("SCALEupup_sd") &&
  argv[1] != std::string("SCALEupup_puppi") && argv[1] != std::string("SCALEupup_puppi_sd") &&
  argv[1] != std::string("SCALEupnone") && argv[1] != std::string("SCALEupnone_sd") &&
  argv[1] != std::string("SCALEupnone_puppi") && argv[1] != std::string("SCALEupnone_puppi_sd") &&
  argv[1] != std::string("mtop1695") && argv[1] != std::string("mtop1695_sd") &&
  argv[1] != std::string("mtop1695_puppi") && argv[1] != std::string("mtop1695_puppi_sd") &&
  argv[1] != std::string("mtop1715") && argv[1] != std::string("mtop1715_sd") &&
  argv[1] != std::string("mtop1715_puppi") && argv[1] != std::string("mtop1715_puppi_sd") &&
  argv[1] != std::string("mtop1735") && argv[1] != std::string("mtop1735_sd") &&
  argv[1] != std::string("mtop1735_puppi") && argv[1] != std::string("mtop1735_puppi_sd") &&
  argv[1] != std::string("mtop1755") && argv[1] != std::string("mtop1755_sd") &&
  argv[1] != std::string("mtop1755_puppi") && argv[1] != std::string("mtop1755_puppi_sd") &&
  argv[1] != std::string("madgraph") && argv[1] != std::string("madgraph_sd") &&
  argv[1] != std::string("madgraph_puppi") && argv[1] != std::string("madgraph_puppi_sd") &&
  argv[1] != std::string("isrup") && argv[1] != std::string("isrup_sd") &&
  argv[1] != std::string("isrup_puppi") && argv[1] != std::string("isrup_puppi_sd") &&
  argv[1] != std::string("isrdown") && argv[1] != std::string("isrdown_sd") &&
  argv[1] != std::string("isrdown_puppi") && argv[1] != std::string("isrdown_puppi_sd") &&
  argv[1] != std::string("fsrup") && argv[1] != std::string("fsrup_sd") &&
  argv[1] != std::string("fsrup_puppi") && argv[1] != std::string("fsrup_puppi_sd") &&
  argv[1] != std::string("fsrdown") && argv[1] != std::string("fsrdown_sd") &&
  argv[1] != std::string("fsrdown_puppi") && argv[1] != std::string("fsrdown_puppi_sd") &&
  argv[1] != std::string("hdampup") && argv[1] != std::string("hdampup_sd") &&
  argv[1] != std::string("hdampup_puppi") && argv[1] != std::string("hdampup_puppi_sd") &&
  argv[1] != std::string("hdampdown") && argv[1] != std::string("hdampdown_sd") &&
  argv[1] != std::string("hdampdown_puppi") && argv[1] != std::string("hdampdown_puppi_sd")
){
  throw runtime_error("use Pseudo_1, Pseudo_1_sd, Pseudo_1_puppi, Pseudo_1_puppi_sd, ...");
}
if(argv[4] != std::string("mu") && argv[4] != std::string("ele") && argv[4] != std::string("comb")){
  throw runtime_error("use mu, ele or comb");
}
TH1::SetDefaultSumw2();

string filename;
TString suffix = "";
TString pu = "";
TString dataset = argv[1];
TString data_file_name = argv[4];
filename = "Unfoldings/Unfolding_" + dataset + "_" + data_file_name + ".root";
TFile *outputFile = new TFile(filename.c_str(),"recreate");
if(dataset.Contains("Pseudo_1"))      suffix = "_1";
else if(dataset.Contains("Pseudo_2")) suffix = "_2";
else if(dataset.Contains("Pseudo_3")) suffix = "_3";
if(dataset.Contains("puppi_sd"))   pu = "_puppi_sd";
else if(dataset.Contains("puppi")) pu = "_puppi";
else if(dataset.Contains("sd"))    pu = "_sd";
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
TFile *data_File;
if(data_file_name.Contains("mu")) data_File = new TFile(dir + "Histograms_Muon.root");
else if(data_file_name.Contains("ele")) data_File = new TFile(dir + "Histograms_Electron.root");
else if(data_file_name.Contains("comb")) data_File = new TFile(dir + "Histograms_Combined.root");

// fill background hists -- add further or remove unnecassary backgrounds in background_names
// background_names = {"DYJets", "QCD", "ST", "WJets", "Diboson"};
background_names = {"DYJets", "ST", "Diboson", "WJets"};
if(data_file_name.Contains("mu")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
else if(data_file_name.Contains("ele")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
else if(data_file_name.Contains("comb")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};

// Setup everything for unfolding!
data_File->GetObject("mc"+pu+"_matrix"+suffix, mat_response);          // fill response matrix
data_File->GetObject(dataset+"_rec", h_data);                  // fill histogram with data which should be unfolded
data_File->GetObject(dataset+"_rec_dist", h_data_dist);           // fill histogram with data which should be unfolded
if(!dataset.Contains("Data")) data_File->GetObject(dataset+"_truth", h_truth);
else data_File->GetObject("mc"+pu+"_truth", h_truth);
if(!dataset.Contains("Data")) data_File->GetObject(dataset+"_truth_all", h_truth_all);
else data_File->GetObject("mc"+pu+"_truth_all", h_truth_all);
data_File->GetObject("mc"+pu+"_gen"+suffix, h_unfold);                 // fill histogram with what data should be unfolded
data_File->GetObject("mc"+pu+"_rec"+suffix, h_mc);                     // fill histogram to check if ratio between data and mc is appropiate
data_File->GetObject("mc"+pu+"_rec_dist"+suffix, h_mc_dist);
data_File->GetObject("mc"+pu+"_truth"+suffix, h_trutch_check);          // fill histogram to check if ratio between data and mc is appropiate
data_File->GetObject("mc"+pu+"_truth_all"+suffix, h_trutch_check_all);  // fill histogram to check if ratio between data and mc is appropiate
data_File->GetObject("mc"+pu+"_purity_all"+suffix, h_purity_all);
data_File->GetObject("mc"+pu+"_purity_samebin"+suffix, h_purity_samebin);
data_File->GetObject("mc"+pu+"_stability_all"+suffix, h_stability_all);
data_File->GetObject("mc"+pu+"_stability_samebin"+suffix, h_stability_samebin);

cout << "test1: " << mat_response->GetName() << '\n';
cout << "test2: " << h_data->GetName() << '\n';
cout << "test3: " << h_data_dist->GetName() << '\n';
cout << "test4: " << h_truth->GetName() << '\n';
cout << "test5: " << h_truth_all->GetName() << '\n';
cout << "test6: " << h_unfold->GetName() << '\n';
cout << "test7: " << h_mc->GetName() << '\n';
cout << "test8: " << h_mc_dist->GetName() << '\n';
cout << "test9: " << h_trutch_check->GetName() << '\n';
cout << "test10: " << h_trutch_check_all->GetName() << '\n';
cout << "test11: " << h_purity_all->GetName() << '\n';
cout << "test12: " << h_purity_samebin->GetName() << '\n';
cout << "test13: " << h_stability_all->GetName() << '\n';
cout << "test14: " << h_stability_samebin->GetName() << '\n';

background.clear();
subtract_background = false;
if(dataset.Contains("Data")){
  for(unsigned int i = 0; i < background_names.size(); i++){
    cout << "Background name: " << background_names.at(i) << '\n';
    background.push_back((TH1D*) data_File->Get("Background_" + background_names.at(i)+pu+"_rec"));
  }
  subtract_background = true;
}
sys_matrix.clear();
for(unsigned int i = 0; i < sys_name.size(); i++){
  for(unsigned int j = 0; j < sys_name.at(i).size(); j++){
    vector<TH2*> dummy;
    sys_matrix.push_back(dummy);
    TString blub = sys_name[i][j] + pu + "_matrix";
    cout << "Test: " << blub << '\n';
    sys_matrix[i].push_back((TH2*) data_File->Get(sys_name[i][j] + pu + "_matrix"));
  }
}

// cout << "subtract_background: " << subtract_background << '\n';
TString save_dir = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
TString format = ".eps";
TString regmode = "derivative";
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
unfold.get_output_meas();
unfold.get_output_all();
unfold.get_input_statcov();
unfold.get_input_statcov_meas();
unfold.get_input_statcov_all();
unfold.get_matrix_statcov();
unfold.get_matrix_statcov_meas();
unfold.get_matrix_statcov_all();
unfold.get_total_statcov();
unfold.get_total_statcov_meas();
unfold.get_total_statcov_all();
unfold.get_correlation();
unfold.get_correlation_meas();
unfold.get_correlation_all();
unfold.get_output_check();
unfold.get_output_check_meas();
unfold.get_output_check_all();
unfold.get_probability_matrix();
unfold.check_projection();
if(subtract_background){
  unfold.get_sys_covariance();
  unfold.get_sys_covariance_meas();
  unfold.get_sys_covariance_all();
  unfold.get_sys_delta();
  unfold.get_sys_delta_meas();
  unfold.get_sys_delta_all();
  unfold.GetBgrStatCov();
  unfold.GetBgrStatCov_meas();
  unfold.GetBgrStatCov_all();
  unfold.GetBgrScaleCov();
  unfold.GetBgrScaleCov_meas();
  unfold.GetBgrScaleCov_all();
  unfold.get_bgr_delta();
  unfold.get_bgr_delta_meas();
  unfold.get_bgr_delta_all();
}

if(tau_value < 0){
  unfold.get_coordinates();
  unfold.get_lcurve();
  unfold.get_tau();
  unfold.get_logtau();
  if(!do_lcurve) {
    unfold.get_rhologtau();
  }
  unfold.get_bias();

  unfolding unfold2(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, sys_matrix, sys_name, background, background_names, nscan, regmode, density_flag, do_lcurve_2, subtract_background, tau_value);
  unfold2.get_coordinates();
  unfold2.get_lcurve();
  unfold2.get_tau();
  unfold2.get_logtau();
  if(!do_lcurve_2) unfold2.get_rhologtau();
  unfold2.get_output();
  unfold2.get_output_meas();
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
  unfold2.get_probability_matrix();
  unfold2.check_projection();
  if(subtract_background){
    unfold2.get_sys_covariance();
    unfold2.get_sys_covariance_meas();
    unfold2.get_sys_covariance_all();
    unfold2.get_sys_delta();
    unfold2.get_sys_delta_meas();
    unfold2.get_sys_delta_all();
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
  unfold2.get_bias();
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
h_stability_all->Write("Stability total");
h_stability_samebin->Write("Stability samebin");

outputFile->Close();
return 0;
}
