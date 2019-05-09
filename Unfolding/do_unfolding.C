#include "do_unfolding.h"

using namespace std;
int counter = 1;
int main(int argc, char* argv[]){
  // switch on histogram errors
  TH1::SetDefaultSumw2();

  string filename;
  filename = "Unfolding.root";
  outputFile = new TFile(filename.c_str(),"recreate");

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
  if(argc < 4){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans> <value of tau>; if you don't want to use a custom value of tau, then set it to -1");
  }
  else if(argv[1] == std::string("pseudo1")){
    data_File->GetObject("ttbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_1", h_truth);
    data_File->GetObject("TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_truth_1", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(argv[1] == std::string("pseudo2")){
    data_File->GetObject("ttbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_2", h_truth);
    data_File->GetObject("TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_truth_2", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(argv[1] == std::string("pseudo3")){
    data_File->GetObject("ttbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_3", h_truth);              // fill histogram with truth
    data_File->GetObject("TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_truth_3", h_trutch_check);            // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(argv[1] == std::string("data")){
    data_File->GetObject("ttbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("data", h_data);                             // fill histogram with data which should be unfolded
    data_File->GetObject("TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_truth_3", h_truth);                   // fill histogram with truth
    data_File->GetObject("TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_3", h_purity_samebin);
  }
  else{
    throw runtime_error("use pseudo1, pseudo2 or pseudo3");
  }



  for(unsigned int i = 0; i < background_names.size(); i++){
    background.push_back((TH1D*) data_File->Get("background_rec_" + background_names.at(i)));
  }
  subtract_background = false;
  TString save_dir = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
  TString dataset = argv[1];
  TString format = ".eps";
  TString regmode = "size";
  TString density_flag = "none";
  TString nscan_word = argv[2];
  int nscan = atoi(argv[2]);
  bool do_lcurve = true;
  bool do_lcurve_2 = false;
  tau_value = atoi(argv[3]);
  TString tau_value_word = argv[3];

  if(tau_value < 0){
    save_projection_x = "Projection_X_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_projection_y = "Projection_Y_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding = "LCurve/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_norm = "LCurve/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_Unfolding_correlation = "LCurve/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauX = "LCurve/TauX_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_TauY = "LCurve/TauY_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_LCurve_crosscheck = "LCurve/crosscheck_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_LCurve = "TauScan/LCurve_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_RhoLogTau = "TauScan/RhoLogTau_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding = "TauScan/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_norm = "TauScan/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_Unfolding_correlation = "TauScan/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
    save_TauScan_crosscheck = "TauScan/crosscheck_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  }
  else{
    save_projection_x = "CustomTau/Projection_X_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_projection_y = "CustomTau/Projection_Y_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding = "CustomTau/Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_norm = "CustomTau/Unfolding_norm_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_Unfolding_correlation = "CustomTau/Unfolding_correlation_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
    save_LCurve_crosscheck = "CustomTau/crosscheck_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + "_" + tau_value_word + format;
  }
  save_response_matrix = "Response_Matrix_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + tau_value_word + format;

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
  unfolding unfold(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve, subtract_background, tau_value);

  unfolded_data = unfold.get_output();
  correlation_matrix = unfold.get_correlation();
  unfolded_mc_test = unfold.get_output_check();
  if(tau_value < 0){
    lcurve = unfold.get_lcurve();
    coordinates = unfold.get_coordinates();
    tau = unfold.get_tau();
    logTau = unfold.get_logtau();
    rhologTau = unfold.get_rhologtau();
  }
  std::vector<TH1D*> projections = unfold.check_projection();

  outputFile->cd();

  Plotter * plot = new Plotter(save_dir);
  plot->Plot_projections(projections.at(0), h_unfold, save_projection_x);
  plot->Plot_projections(projections.at(1), h_mc, save_projection_y);
  plot->Plot_output(unfolded_data, h_truth, false, save_LCurve_Unfolding);
  plot->Plot_output(unfolded_mc_test, h_trutch_check, false, save_LCurve_crosscheck);
  plot->Plot_output(unfolded_data, h_truth, true, save_LCurve_Unfolding_norm);
  plot->Plot_correlation_matrix(correlation_matrix, save_LCurve_Unfolding_correlation);
  plot->Plot_ResponseMatrix(mat_response, save_response_matrix);
  if(tau_value < 0){
    plot->Plot_LogTau(logTau.at(0), tau, coordinates.at(0), save_LCurve_TauX);
    plot->Plot_LogTau(logTau.at(1), tau, coordinates.at(1), save_LCurve_TauY);


    unfolding unfold2(h_data, h_mc, mat_response, h_truth, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve_2, subtract_background, tau_value);

    coordinates_2 = unfold2.get_coordinates();
    lcurve_2 = unfold2.get_lcurve();
    tau_2 = unfold2.get_tau();
    logTau_2 = unfold2.get_logtau();
    rhologTau_2 = unfold2.get_rhologtau();
    unfolded_data_2 = unfold2.get_output();
    correlation_matrix_2 = unfold2.get_correlation();
    unfolded_mc_test_2 = unfold2.get_output_check();

    plot->Plot_output(unfolded_data_2, h_truth, false, save_TauScan_Unfolding);
    plot->Plot_output(unfolded_mc_test_2, h_trutch_check, false, save_TauScan_crosscheck);
    plot->Plot_output(unfolded_data_2, h_truth, true, save_TauScan_Unfolding_norm);
    plot->Plot_correlation_matrix(correlation_matrix_2, save_TauScan_Unfolding_correlation);
    plot->Plot_LCurve(lcurve_2, coordinates, coordinates_2, save_TauScan_LCurve);
    plot->Plot_RhoLogTau(rhologTau_2, tau_2, save_TauScan_RhoLogTau);
  }

  outputFile->Close();
  return 0;
}
