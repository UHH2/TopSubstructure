#include "hist_filler.h"

int main(int argc, char* argv[]){


  // switch on histogram errors
  TH1::SetDefaultSumw2();

  /*
  ██████  ██████  ███████  █████  ████████ ███████      ██████  ██    ██ ████████ ██████  ██    ██ ████████     ███████ ██ ██      ███████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██   ██ ██    ██    ██        ██      ██ ██      ██
  ██      ██████  █████   ███████    ██    █████       ██    ██ ██    ██    ██    ██████  ██    ██    ██        █████   ██ ██      █████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██      ██    ██    ██        ██      ██ ██      ██
  ██████  ██   ██ ███████ ██   ██    ██    ███████      ██████   ██████     ██    ██       ██████     ██        ██      ██ ███████ ███████
  */
  Muon_done = false;
  std::string filename, filename2;
  filename = "Histograms_Muon.root";
  filename2 = "Histograms_Electron.root";


  outputFile=new TFile(filename.c_str(),"recreate");
  outputFile->cd();

  /*
  ██████  ███████  █████  ██████      ██████  ██ ███    ██ ███    ██ ██ ███    ██  ██████
  ██   ██ ██      ██   ██ ██   ██     ██   ██ ██ ████   ██ ████   ██ ██ ████   ██ ██
  ██████  █████   ███████ ██   ██     ██████  ██ ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██   ███
  ██   ██ ██      ██   ██ ██   ██     ██   ██ ██ ██  ██ ██ ██  ██ ██ ██ ██  ██ ██ ██    ██
  ██   ██ ███████ ██   ██ ██████      ██████  ██ ██   ████ ██   ████ ██ ██   ████  ██████
  */

  // read binning schemes in XML format
  TDOMParser parser;
  TString binning_xml;
  binning_xml = "Binning.xml";

  Int_t error = parser.ParseFile(binning_xml);
  if(error) cout << "error = " << error << " from TDOMParser\n";
  TXMLDocument const *XMLdocument = parser.GetXMLDocument();
  binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_rec");
  binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_gen");
  // binning_rec->Write();
  // binning_gen->Write();

  if(!binning_rec) cout << "could not read 'rec' binning\n";
  if(!binning_gen) cout << "could not read 'gen' binning\n";


  // get distributions from measurement phase space and sideband regions
  measurement_rec = binning_rec->FindNode("measurement_rec");
  measurement_gen = binning_gen->FindNode("measurement_gen");
  rec_pt_topjet_sideband = binning_rec->FindNode("rec_pt_topjet_sideband");
  gen_pt_topjet_sideband = binning_gen->FindNode("gen_pt_topjet_sideband");
  rec_mass_sideband = binning_rec->FindNode("rec_mass_sideband");
  gen_mass_sideband = binning_gen->FindNode("gen_mass_sideband");

  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  // define directory
  TString dir = "/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data";
  TString prefix = "/uhh2.AnalysisModuleRunner.";
  // vector<TString> sv = {"SVMR_u_SVMF_u", "SVMR_u_SVMF_d", "SVMR_d_SVMF_u", "SVMR_d_SVMF_d"};

  // fill data
  TFile *data_File;
  data_File = new TFile(dir + "/Muon" + prefix + "DATA.DATA.root");
  fill_data((TTree *) data_File->Get("AnalysisTree"));

  // fill pseudodata
  TFile *pseudodata_matrix_File = new TFile(dir+"/Muon"+prefix+"MC.TTbar.root");
  fill_pseudodata((TTree *) pseudodata_matrix_File->Get("AnalysisTree"));

  // fill ttbar
  TFile *mc_matrix_File = new TFile(dir+"/Muon"+prefix+"MC.TTbar.root");
  fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "mc");

  // fill other mass samples
  vector<TString> diff_masses = {"mtop1695", "mtop1715", "mtop1735", "mtop1755", "madgraph"};
  for(unsigned int i = 0; i < diff_masses.size(); i++){
    TFile *masses_File = new TFile(dir+"/Muon/Other_TTbar"+prefix+"MC.TTbar_"+diff_masses.at(i)+".root");
    fill_ttbar((TTree *) masses_File->Get("AnalysisTree"), diff_masses.at(i));
  }
  // fill variation
  vector<TString> jetcorrections = {"JECup", "JECdown", "JERup", "JERdown", "BTagup", "BTagdown", "MUIDup", "MUIDdown", "MUTriggerup", "MUTriggerdown", "PUup", "PUdown"};
  for(unsigned int i = 0; i < jetcorrections.size(); i++){
    TFile *jc_matrix_file =  new TFile(dir+"/"+jetcorrections.at(i)+"/Muon"+prefix+"MC.TTbar.root");
    fill_ttbar((TTree *) jc_matrix_file->Get("AnalysisTree"), jetcorrections.at(i));
  }


    // fill central sample for elec uncertainties in muon channel and vice versa
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELEIDup");
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELEIDdown");
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELETriggerup");
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELETriggerdown");
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELERecoup");
      fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "ELERecodown");




  // fill scales
  vector<TString> sv = {"SCALEupup", "SCALEupnone", "SCALEdownnone", "SCALEdowndown", "SCALEnoneup", "SCALEnonedown"};
  for(unsigned int i = 0; i < sv.size(); i++){
    TFile *sv_matrix_file =  new TFile(dir+"/"+sv.at(i)+"/Muon"+prefix+"MC.TTbar.root");
    fill_scale((TTree *) sv_matrix_file->Get("AnalysisTree"), sv.at(i));
  }

  // fill background
  // std::vector<TString> background = {"DYJets", "QCD", "ST", "WJets", "WW", "WZ", "ZZ"};
  std::vector<TString> background = {"DYJets", "ST", "Diboson"};

  for(unsigned int i = 0; i < background.size(); i++){
    TFile *background_File = new TFile(dir+"/Muon"+prefix+"MC."+background[i]+".root");
    fill_background((TTree *) background_File->Get("AnalysisTree"), background[i]);
  }
  outputFile->Close();
  Muon_done = true;


  outputFile2=new TFile(filename2.c_str(),"recreate");
  outputFile2->cd();
  // binning_rec->Write();
  // binning_gen->Write();
  TFile *data_File2;
  data_File2 = new TFile(dir + prefix + "DATA.DATA.root");
  fill_data((TTree *) data_File2->Get("AnalysisTree"));

  // fill pseudodata
  TFile *pseudodata_matrix_File2 = new TFile(dir+"/Electron"+prefix+"MC.TTbar.root");
  fill_pseudodata((TTree *) pseudodata_matrix_File2->Get("AnalysisTree"));

  // fill ttbar
  TFile *mc_matrix_File2 = new TFile(dir+"/Electron"+prefix+"MC.TTbar.root");
  fill_ttbar((TTree *) mc_matrix_File2->Get("AnalysisTree"), "mc");

  // fill other mass samples
  vector<TString> diff_masses2 = {"mtop1695", "mtop1715", "mtop1735", "mtop1755", "madgraph"};
  for(unsigned int i = 0; i < diff_masses2.size(); i++){
    TFile *masses_File2 = new TFile(dir+"/Electron/Other_TTbar"+prefix+"MC.TTbar_"+diff_masses2.at(i)+".root");
    fill_ttbar((TTree *) masses_File2->Get("AnalysisTree"), diff_masses2.at(i));
  }
  // fill variation
  vector<TString> jetcorrections2 = {"JECup", "JECdown", "JERup", "JERdown", "BTagup", "BTagdown", "ELEIDup", "ELEIDdown", "ELETriggerup", "ELETriggerdown", "PUup", "PUdown", "ELERecodown", "ELERecoup"};
  for(unsigned int i = 0; i < jetcorrections2.size(); i++){
    TFile *jc_matrix_file2 =  new TFile(dir+"/"+jetcorrections2.at(i)+"/Electron"+prefix+"MC.TTbar.root");
    fill_ttbar((TTree *) jc_matrix_file2->Get("AnalysisTree"), jetcorrections2.at(i));
  }

  // fill central sample for elec uncertainties in muon channel and vice versa
  fill_ttbar((TTree *) mc_matrix_File2->Get("AnalysisTree"), "MUIDup");
  fill_ttbar((TTree *) mc_matrix_File2->Get("AnalysisTree"), "MUIDdown");
  fill_ttbar((TTree *) mc_matrix_File2->Get("AnalysisTree"), "MUTriggerup");
  fill_ttbar((TTree *) mc_matrix_File2->Get("AnalysisTree"), "MUTriggerdown");

  // fill scales
  vector<TString> sv2 = {"SCALEupup", "SCALEupnone", "SCALEdownnone", "SCALEdowndown", "SCALEnoneup", "SCALEnonedown"};
  for(unsigned int i = 0; i < sv2.size(); i++){
    TFile *sv_matrix_file2 =  new TFile(dir+"/"+sv2.at(i)+"/Electron"+prefix+"MC.TTbar.root");
    fill_scale((TTree *) sv_matrix_file2->Get("AnalysisTree"), sv2.at(i));
  }

  // fill background
  // std::vector<TString> background = {"DYJets", "QCD", "ST", "WJets", "WW", "WZ", "ZZ"};
  std::vector<TString> background2 = {"DYJets", "ST", "Diboson"};

  for(unsigned int i = 0; i < background2.size(); i++){
    TFile *background_File2 = new TFile(dir+"/Electron"+prefix+"MC."+background2[i]+".root");
    fill_background((TTree *) background_File2->Get("AnalysisTree"), background2[i]);
  }

  outputFile2->Close();

  system("hadd -f Histograms_Combined.root Histograms_Muon.root Histograms_Electron.root");

  return 0;
}

/*
███████ ██ ██      ██          ██████   █████  ████████  █████
██      ██ ██      ██          ██   ██ ██   ██    ██    ██   ██
█████   ██ ██      ██          ██   ██ ███████    ██    ███████
██      ██ ██      ██          ██   ██ ██   ██    ██    ██   ██
██      ██ ███████ ███████     ██████  ██   ██    ██    ██   ██
*/

void fill_data(TTree* tree){
  if(!tree) cout << "could not read 'data' tree\n";
  else      cout << "Filling Data Histograms...\n";

  TH1* h_data = binning_rec->CreateHistogram("Data");
  TH1* h_data_sd = binning_rec->CreateHistogram("Data_sd");
  TH1* h_data_puppi = binning_rec->CreateHistogram("Data_puppi");
  TH1* h_data_puppi_sd = binning_rec->CreateHistogram("Data_puppi_sd");
  TH1* h_data_dist = measurement_rec->CreateHistogram("Data_dist",kTRUE,0,0, "mass[C]");
  TH1* h_data_dist_sd = measurement_rec->CreateHistogram("Data_dist_sd",kTRUE,0,0, "mass[C]");
  TH1* h_data_dist_puppi = measurement_rec->CreateHistogram("Data_dist_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_data_dist_puppi_sd = measurement_rec->CreateHistogram("Data_dist_puppi_sd",kTRUE,0,0, "mass[C]");

  if(!Muon_done) outputFile->cd();
  else outputFile2->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_rec_sd", &tau32_rec_sd);
  tree->SetBranchAddress("h_tau32_rec_puppi", &tau32_rec_puppi);
  tree->SetBranchAddress("h_tau32_rec_puppi_sd", &tau32_rec_puppi_sd);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_rec_sd", &mass_rec_sd);
  tree->SetBranchAddress("h_mass_rec_puppi", &mass_rec_puppi);
  tree->SetBranchAddress("h_mass_rec_puppi_sd", &mass_rec_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_final_sd", &passed_rec_final_sd);
  tree->SetBranchAddress("h_passed_rec_final_puppi", &passed_rec_final_puppi);
  tree->SetBranchAddress("h_passed_rec_final_puppi_sd", &passed_rec_final_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_sd", &passed_rec_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi", &passed_rec_pt_topjet_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi_sd", &passed_rec_pt_topjet_sideband_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_sd", &passed_rec_mass_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi", &passed_rec_mass_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi_sd", &passed_rec_mass_sideband_puppi_sd);
  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    Int_t binNumber = 0;
    if(passed_rec_final)                   binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband) binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)      binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);

    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband){
      h_data->Fill(binNumber);
      h_data_dist->Fill(tau32_rec);
    }


    Int_t binNumber_sd = 0;
    if(passed_rec_final_sd)                   binNumber_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_sd, mass_rec_sd);
    else if(passed_rec_pt_topjet_sideband_sd) binNumber_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_sd);
    else if(passed_rec_mass_sideband_sd)      binNumber_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_sd);

    if(passed_rec_final_sd || passed_rec_pt_topjet_sideband_sd || passed_rec_mass_sideband_sd){
      h_data_sd->Fill(binNumber_sd);
      h_data_dist_sd->Fill(tau32_rec_sd);
    }


    Int_t binNumber_puppi = 0;
    if(passed_rec_final_puppi)                   binNumber_puppi = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi, mass_rec_puppi);
    else if(passed_rec_pt_topjet_sideband_puppi) binNumber_puppi = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi);
    else if(passed_rec_mass_sideband_puppi)      binNumber_puppi = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi);

    if(passed_rec_final_puppi || passed_rec_pt_topjet_sideband_puppi || passed_rec_mass_sideband_puppi){
      h_data_puppi->Fill(binNumber_puppi);
      h_data_dist_puppi->Fill(tau32_rec_puppi);
    }


    Int_t binNumber_puppi_sd = 0;
    if(passed_rec_final_puppi_sd)                   binNumber_puppi_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi_sd, mass_rec_puppi_sd);
    else if(passed_rec_pt_topjet_sideband_puppi_sd) binNumber_puppi_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);
    else if(passed_rec_mass_sideband_puppi_sd)      binNumber_puppi_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);

    if(passed_rec_final_puppi_sd || passed_rec_pt_topjet_sideband_puppi_sd || passed_rec_mass_sideband_puppi_sd){
      h_data_puppi_sd->Fill(binNumber_puppi_sd);
      h_data_dist_puppi_sd->Fill(tau32_rec_puppi_sd);
    }
  }

  h_data->Write();
  h_data_sd->Write();
  h_data_puppi->Write();
  h_data_puppi_sd->Write();
  h_data_dist->Write();
  h_data_dist_sd->Write();
  h_data_dist_puppi->Write();
  h_data_dist_puppi_sd->Write();
  delete h_data;
  delete h_data_sd;
  delete h_data_puppi;
  delete h_data_puppi_sd;
  delete h_data_dist;
  delete h_data_dist_sd;
  delete h_data_dist_puppi;
  delete h_data_dist_puppi_sd;
  cout << "finished: filling data" << '\n';
  return;
}

/*
███████ ██ ██      ██          ██████  ███████ ███████ ██    ██ ██████   ██████  ██████   █████  ████████  █████
██      ██ ██      ██          ██   ██ ██      ██      ██    ██ ██   ██ ██    ██ ██   ██ ██   ██    ██    ██   ██
█████   ██ ██      ██          ██████  ███████ █████   ██    ██ ██   ██ ██    ██ ██   ██ ███████    ██    ███████
██      ██ ██      ██          ██           ██ ██      ██    ██ ██   ██ ██    ██ ██   ██ ██   ██    ██    ██   ██
██      ██ ███████ ███████     ██      ███████ ███████  ██████  ██████   ██████  ██████  ██   ██    ██    ██   ██
*/


void fill_pseudodata(TTree* tree){
  if(!tree) cout << "could not read 'mc' tree\n";
  else      cout << "Filling Pseudodata Histograms...\n";

  // setup hists
  TH1* h_pseudodata_1                    = binning_rec->CreateHistogram("Pseudodata_1");
  TH1* h_pseudodata_sd_1                 = binning_rec->CreateHistogram("Pseudodata_sd_1");
  TH1* h_pseudodata_puppi_1              = binning_rec->CreateHistogram("Pseudodata_puppi_1");
  TH1* h_pseudodata_puppi_sd_1           = binning_rec->CreateHistogram("Pseudodata_puppi_sd_1");
  TH1* h_pseudodata_dist_1               = measurement_rec->CreateHistogram("Pseudodata_dist_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_sd_1            = measurement_rec->CreateHistogram("Pseudodata_dist_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_1         = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_sd_1      = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_1          = binning_gen->CreateHistogram("Pseudodata_truth_all_1");
  TH1* h_pseudodata_truth_all_sd_1       = binning_gen->CreateHistogram("Pseudodata_truth_all_sd_1");
  TH1* h_pseudodata_truth_all_puppi_1    = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_1");
  TH1* h_pseudodata_truth_all_puppi_sd_1 = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_sd_1");
  TH1* h_pseudodata_truth_1              = measurement_gen->CreateHistogram("Pseudodata_truth_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_sd_1           = measurement_gen->CreateHistogram("Pseudodata_truth_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_1        = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_sd_1     = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_sd_1",kTRUE,0,0, "mass[C]");

  TH1* h_pseudodata_2                    = binning_rec->CreateHistogram("Pseudodata_2");
  TH1* h_pseudodata_sd_2                 = binning_rec->CreateHistogram("Pseudodata_sd_2");
  TH1* h_pseudodata_puppi_2              = binning_rec->CreateHistogram("Pseudodata_puppi_2");
  TH1* h_pseudodata_puppi_sd_2           = binning_rec->CreateHistogram("Pseudodata_puppi_sd_2");
  TH1* h_pseudodata_dist_2               = measurement_rec->CreateHistogram("Pseudodata_dist_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_sd_2            = measurement_rec->CreateHistogram("Pseudodata_dist_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_2         = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_sd_2      = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_2          = binning_gen->CreateHistogram("Pseudodata_truth_all_2");
  TH1* h_pseudodata_truth_all_sd_2       = binning_gen->CreateHistogram("Pseudodata_truth_all_sd_2");
  TH1* h_pseudodata_truth_all_puppi_2    = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_2");
  TH1* h_pseudodata_truth_all_puppi_sd_2 = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_sd_2");
  TH1* h_pseudodata_truth_2              = measurement_gen->CreateHistogram("Pseudodata_truth_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_sd_2           = measurement_gen->CreateHistogram("Pseudodata_truth_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_2        = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_sd_2     = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_sd_2",kTRUE,0,0, "mass[C]");

  TH1* h_pseudodata_3                    = binning_rec->CreateHistogram("Pseudodata_3");
  TH1* h_pseudodata_sd_3                 = binning_rec->CreateHistogram("Pseudodata_sd_3");
  TH1* h_pseudodata_puppi_3              = binning_rec->CreateHistogram("Pseudodata_puppi_3");
  TH1* h_pseudodata_puppi_sd_3           = binning_rec->CreateHistogram("Pseudodata_puppi_sd_3");
  TH1* h_pseudodata_dist_3               = measurement_rec->CreateHistogram("Pseudodata_dist_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_sd_3            = measurement_rec->CreateHistogram("Pseudodata_dist_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_3         = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_sd_3      = measurement_rec->CreateHistogram("Pseudodata_dist_puppi_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_3          = binning_gen->CreateHistogram("Pseudodata_truth_all_3");
  TH1* h_pseudodata_truth_all_sd_3       = binning_gen->CreateHistogram("Pseudodata_truth_all_sd_3");
  TH1* h_pseudodata_truth_all_puppi_3    = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_3");
  TH1* h_pseudodata_truth_all_puppi_sd_3 = binning_gen->CreateHistogram("Pseudodata_truth_all_puppi_sd_3");
  TH1* h_pseudodata_truth_3              = measurement_gen->CreateHistogram("Pseudodata_truth_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_sd_3           = measurement_gen->CreateHistogram("Pseudodata_truth_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_3        = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_sd_3     = measurement_gen->CreateHistogram("Pseudodata_truth_puppi_sd_3",kTRUE,0,0, "mass[C]");

  if(!Muon_done) outputFile->cd();
  else outputFile2->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_gen", &tau32_gen);
  tree->SetBranchAddress("h_tau32_gen_sd", &tau32_gen_sd);
  tree->SetBranchAddress("h_mass_gen", &mass_gen);
  tree->SetBranchAddress("h_mass_gen_sd", &mass_gen_sd);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
  tree->SetBranchAddress("h_passed_gen_final_sd", &passed_gen_final_sd);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband", &passed_gen_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband_sd", &passed_gen_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_gen_mass_sideband", &passed_gen_mass_sideband);
  tree->SetBranchAddress("h_passed_gen_mass_sideband_sd", &passed_gen_mass_sideband_sd);

  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_rec_sd", &tau32_rec_sd);
  tree->SetBranchAddress("h_tau32_rec_puppi", &tau32_rec_puppi);
  tree->SetBranchAddress("h_tau32_rec_puppi_sd", &tau32_rec_puppi_sd);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_rec_sd", &mass_rec_sd);
  tree->SetBranchAddress("h_mass_rec_puppi", &mass_rec_puppi);
  tree->SetBranchAddress("h_mass_rec_puppi_sd", &mass_rec_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_final_sd", &passed_rec_final_sd);
  tree->SetBranchAddress("h_passed_rec_final_puppi", &passed_rec_final_puppi);
  tree->SetBranchAddress("h_passed_rec_final_puppi_sd", &passed_rec_final_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_sd", &passed_rec_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi", &passed_rec_pt_topjet_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi_sd", &passed_rec_pt_topjet_sideband_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_sd", &passed_rec_mass_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi", &passed_rec_mass_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi_sd", &passed_rec_mass_sideband_puppi_sd);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchAddress("h_gen_weight", &gen_weight);
  tree->SetBranchStatus("*", 1);

  int counter = 1;
  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    double scale = 5;
    // get weights for migration matrix
    w_central = rec_weight*scale;
    w_nogen = rec_weight*scale;
    w_norec = gen_weight*scale;
    w_correction = gen_weight*scale - rec_weight*scale;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight*scale;
    w_gen = gen_weight*scale;

    // fill histograms for CHS Jets w/o SoftDrop
    Int_t rec_binNumber = 0, gen_binNumber = 0;
    if(passed_rec_final)                    rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband)  rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)       rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);

    if(passed_gen_final)                    gen_binNumber = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info = false, gen_info = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband) gen_info = true;
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband) rec_info = true;


    if(rec_info){
      if(counter == 1 || counter == 2){
        h_pseudodata_1->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_1->Fill(tau32_rec, w_sig_rec);
      }
      else if(counter == 3 || counter == 4){
        h_pseudodata_2->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_2->Fill(tau32_rec, w_sig_rec);
      }
      else if(counter == 5 || counter == 6){
        h_pseudodata_3->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_3->Fill(tau32_rec, w_sig_rec);
      }
    }
    if(gen_info){
      if(counter == 1 || counter == 2)  h_pseudodata_truth_all_1->Fill(gen_binNumber, w_gen);
      if(counter == 3 || counter == 4)  h_pseudodata_truth_all_2->Fill(gen_binNumber, w_gen);
      if(counter == 5 || counter == 6) h_pseudodata_truth_all_3->Fill(gen_binNumber, w_gen);
    }
    if(passed_gen_final){
      if(counter == 1 || counter == 2)       h_pseudodata_truth_1->Fill(tau32_gen, w_gen);
      else if(counter == 3 || counter == 4)  h_pseudodata_truth_2->Fill(tau32_gen, w_gen);
      else if(counter == 5 || counter == 6) h_pseudodata_truth_3->Fill(tau32_gen, w_gen);
    }

    // fill histograms for CHS Jets w/ SoftDrop
    Int_t rec_binNumber_sd = 0, gen_binNumber_sd = 0;
    if(passed_rec_final_sd)                    rec_binNumber_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_sd, mass_rec_sd);
    else if(passed_rec_pt_topjet_sideband_sd)  rec_binNumber_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_sd);
    else if(passed_rec_mass_sideband_sd)       rec_binNumber_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_sd);

    if(passed_gen_final_sd)                    gen_binNumber_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd)  gen_binNumber_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)       gen_binNumber_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_sd = false, gen_info_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd) gen_info_sd = true;
    if(passed_rec_final_sd || passed_rec_pt_topjet_sideband_sd || passed_rec_mass_sideband_sd) rec_info_sd = true;

    if(rec_info_sd){
      if(counter == 1 || counter == 2){
        h_pseudodata_sd_1->Fill(rec_binNumber_sd, w_sig_rec);
        h_pseudodata_dist_sd_1->Fill(tau32_rec_sd, w_sig_rec);
      }
      else if(counter == 3 || counter == 4){
        h_pseudodata_sd_2->Fill(rec_binNumber_sd, w_sig_rec);
        h_pseudodata_dist_sd_2->Fill(tau32_rec_sd, w_sig_rec);
      }
      else if(counter == 5 || counter == 6){
        h_pseudodata_sd_3->Fill(rec_binNumber_sd, w_sig_rec);
        h_pseudodata_dist_sd_3->Fill(tau32_rec_sd, w_sig_rec);
      }
    }
    if(gen_info_sd){
      if(counter == 1 || counter == 2)  h_pseudodata_truth_all_sd_1->Fill(gen_binNumber_sd, w_gen);
      if(counter == 3 || counter == 4)  h_pseudodata_truth_all_sd_2->Fill(gen_binNumber_sd, w_gen);
      if(counter == 5 || counter == 6) h_pseudodata_truth_all_sd_3->Fill(gen_binNumber_sd, w_gen);
    }
    if(passed_gen_final_sd){
      if(counter == 1 || counter == 2)       h_pseudodata_truth_sd_1->Fill(tau32_gen_sd, w_gen);
      else if(counter == 3 || counter == 4)  h_pseudodata_truth_sd_2->Fill(tau32_gen_sd, w_gen);
      else if(counter == 5 || counter == 6) h_pseudodata_truth_sd_3->Fill(tau32_gen_sd, w_gen);
    }

    // fill histograms for PUPPI Jets w/o SoftDrop
    Int_t rec_binNumber_puppi = 0, gen_binNumber_puppi = 0;
    if(passed_rec_final_puppi)                    rec_binNumber_puppi = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi, mass_rec_puppi);
    else if(passed_rec_pt_topjet_sideband_puppi)  rec_binNumber_puppi = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi);
    else if(passed_rec_mass_sideband_puppi)       rec_binNumber_puppi = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi);

    if(passed_gen_final)                    gen_binNumber_puppi = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber_puppi = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber_puppi = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info_puppi = false, gen_info_puppi = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband)                   gen_info_puppi = true;
    if(passed_rec_final_puppi || passed_rec_pt_topjet_sideband_puppi || passed_rec_mass_sideband_puppi) rec_info_puppi = true;


    if(rec_info_puppi){
      if(counter == 1 || counter == 2){
        h_pseudodata_puppi_1->Fill(rec_binNumber_puppi, w_sig_rec);
        h_pseudodata_dist_puppi_1->Fill(tau32_rec_puppi, w_sig_rec);
      }
      else if(counter == 3 || counter == 4){
        h_pseudodata_puppi_2->Fill(rec_binNumber_puppi, w_sig_rec);
        h_pseudodata_dist_puppi_2->Fill(tau32_rec_puppi, w_sig_rec);
      }
      else if(counter == 5 || counter == 6){
        h_pseudodata_puppi_3->Fill(rec_binNumber_puppi, w_sig_rec);
        h_pseudodata_dist_puppi_3->Fill(tau32_rec_puppi, w_sig_rec);
      }
    }
    if(gen_info_puppi){
      if(counter == 1 || counter == 2)  h_pseudodata_truth_all_puppi_1->Fill(gen_binNumber_puppi, w_gen);
      if(counter == 3 || counter == 4)  h_pseudodata_truth_all_puppi_2->Fill(gen_binNumber_puppi, w_gen);
      if(counter == 5 || counter == 6) h_pseudodata_truth_all_puppi_3->Fill(gen_binNumber_puppi, w_gen);
    }
    if(passed_gen_final){
      if(counter == 1 || counter == 2)       h_pseudodata_truth_puppi_1->Fill(tau32_gen, w_gen);
      else if(counter == 3 || counter == 4)  h_pseudodata_truth_puppi_2->Fill(tau32_gen, w_gen);
      else if(counter == 5 || counter == 6) h_pseudodata_truth_puppi_3->Fill(tau32_gen, w_gen);
    }

    // fill histograms for PUPPI Jets w/ SoftDrop
    Int_t rec_binNumber_puppi_sd = 0, gen_binNumber_puppi_sd = 0;
    if(passed_rec_final_puppi_sd)                    rec_binNumber_puppi_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi_sd, mass_rec_puppi_sd);
    else if(passed_rec_pt_topjet_sideband_puppi_sd)  rec_binNumber_puppi_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);
    else if(passed_rec_mass_sideband_puppi_sd)       rec_binNumber_puppi_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);

    if(passed_gen_final_sd)                    gen_binNumber_puppi_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd)  gen_binNumber_puppi_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)       gen_binNumber_puppi_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_puppi_sd = false, gen_info_puppi_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd)                   gen_info_puppi_sd = true;
    if(passed_rec_final_puppi_sd || passed_rec_pt_topjet_sideband_puppi_sd || passed_rec_mass_sideband_puppi_sd) rec_info_puppi_sd = true;

    if(rec_info_puppi_sd){
      if(counter == 1 || counter == 2){
        h_pseudodata_puppi_sd_1->Fill(rec_binNumber_puppi_sd, w_sig_rec);
        h_pseudodata_dist_puppi_sd_1->Fill(tau32_rec_puppi_sd, w_sig_rec);
      }
      else if(counter == 3 || counter == 4){
        h_pseudodata_puppi_sd_2->Fill(rec_binNumber_puppi_sd, w_sig_rec);
        h_pseudodata_dist_puppi_sd_2->Fill(tau32_rec_puppi_sd, w_sig_rec);
      }
      else if(counter == 5 || counter == 6){
        h_pseudodata_puppi_sd_3->Fill(rec_binNumber_puppi_sd, w_sig_rec);
        h_pseudodata_dist_puppi_sd_3->Fill(tau32_rec_puppi_sd, w_sig_rec);
      }
    }
    if(gen_info_puppi_sd){
      if(counter == 1 || counter == 2)  h_pseudodata_truth_all_puppi_sd_1->Fill(gen_binNumber_puppi_sd, w_gen);
      else if(counter == 3 || counter == 4)  h_pseudodata_truth_all_puppi_sd_2->Fill(gen_binNumber_puppi_sd, w_gen);
      else if(counter == 5 || counter == 6)  h_pseudodata_truth_all_puppi_sd_3->Fill(gen_binNumber_puppi_sd, w_gen);
    }
    if(passed_gen_final_sd){
      if(counter == 1 || counter == 2)       h_pseudodata_truth_puppi_sd_1->Fill(tau32_gen_sd, w_gen);
      else if(counter == 3 || counter == 4)  h_pseudodata_truth_puppi_sd_2->Fill(tau32_gen_sd, w_gen);
      else if(counter == 5 || counter == 6)  h_pseudodata_truth_puppi_sd_3->Fill(tau32_gen_sd, w_gen);
    }
    if(counter != 10) counter++;
    else counter = 1;
  }

  h_pseudodata_1->Write();
  h_pseudodata_sd_1->Write();
  h_pseudodata_puppi_1->Write();
  h_pseudodata_puppi_sd_1->Write();
  h_pseudodata_dist_1->Write();
  h_pseudodata_dist_sd_1->Write();
  h_pseudodata_dist_puppi_1->Write();
  h_pseudodata_dist_puppi_sd_1->Write();
  h_pseudodata_truth_1->Write();
  h_pseudodata_truth_sd_1->Write();
  h_pseudodata_truth_puppi_1->Write();
  h_pseudodata_truth_puppi_sd_1->Write();
  h_pseudodata_truth_all_1->Write();
  h_pseudodata_truth_all_sd_1->Write();
  h_pseudodata_truth_all_puppi_1->Write();
  h_pseudodata_truth_all_puppi_sd_1->Write();

  h_pseudodata_2->Write();
  h_pseudodata_sd_2->Write();
  h_pseudodata_puppi_2->Write();
  h_pseudodata_puppi_sd_2->Write();
  h_pseudodata_dist_2->Write();
  h_pseudodata_dist_sd_2->Write();
  h_pseudodata_dist_puppi_2->Write();
  h_pseudodata_dist_puppi_sd_2->Write();
  h_pseudodata_truth_2->Write();
  h_pseudodata_truth_sd_2->Write();
  h_pseudodata_truth_puppi_2->Write();
  h_pseudodata_truth_puppi_sd_2->Write();
  h_pseudodata_truth_all_2->Write();
  h_pseudodata_truth_all_sd_2->Write();
  h_pseudodata_truth_all_puppi_2->Write();
  h_pseudodata_truth_all_puppi_sd_2->Write();

  h_pseudodata_3->Write();
  h_pseudodata_sd_3->Write();
  h_pseudodata_puppi_3->Write();
  h_pseudodata_puppi_sd_3->Write();
  h_pseudodata_dist_3->Write();
  h_pseudodata_dist_sd_3->Write();
  h_pseudodata_dist_puppi_3->Write();
  h_pseudodata_dist_puppi_sd_3->Write();
  h_pseudodata_truth_3->Write();
  h_pseudodata_truth_sd_3->Write();
  h_pseudodata_truth_puppi_3->Write();
  h_pseudodata_truth_puppi_sd_3->Write();
  h_pseudodata_truth_all_3->Write();
  h_pseudodata_truth_all_sd_3->Write();
  h_pseudodata_truth_all_puppi_3->Write();
  h_pseudodata_truth_all_puppi_sd_3->Write();

  delete h_pseudodata_1;
  delete h_pseudodata_sd_1;
  delete h_pseudodata_puppi_1;
  delete h_pseudodata_puppi_sd_1;
  delete h_pseudodata_dist_1;
  delete h_pseudodata_dist_sd_1;
  delete h_pseudodata_dist_puppi_1;
  delete h_pseudodata_dist_puppi_sd_1;
  delete h_pseudodata_truth_1;
  delete h_pseudodata_truth_sd_1;
  delete h_pseudodata_truth_puppi_1;
  delete h_pseudodata_truth_puppi_sd_1;
  delete h_pseudodata_truth_all_1;
  delete h_pseudodata_truth_all_sd_1;
  delete h_pseudodata_truth_all_puppi_1;
  delete h_pseudodata_truth_all_puppi_sd_1;
  delete h_pseudodata_2;
  delete h_pseudodata_sd_2;
  delete h_pseudodata_puppi_2;
  delete h_pseudodata_puppi_sd_2;
  delete h_pseudodata_dist_2;
  delete h_pseudodata_dist_sd_2;
  delete h_pseudodata_dist_puppi_2;
  delete h_pseudodata_dist_puppi_sd_2;
  delete h_pseudodata_truth_2;
  delete h_pseudodata_truth_sd_2;
  delete h_pseudodata_truth_puppi_2;
  delete h_pseudodata_truth_puppi_sd_2;
  delete h_pseudodata_truth_all_2;
  delete h_pseudodata_truth_all_sd_2;
  delete h_pseudodata_truth_all_puppi_2;
  delete h_pseudodata_truth_all_puppi_sd_2;
  delete h_pseudodata_3;
  delete h_pseudodata_sd_3;
  delete h_pseudodata_puppi_3;
  delete h_pseudodata_puppi_sd_3;
  delete h_pseudodata_dist_3;
  delete h_pseudodata_dist_sd_3;
  delete h_pseudodata_dist_puppi_3;
  delete h_pseudodata_dist_puppi_sd_3;
  delete h_pseudodata_truth_3;
  delete h_pseudodata_truth_sd_3;
  delete h_pseudodata_truth_puppi_3;
  delete h_pseudodata_truth_puppi_sd_3;
  delete h_pseudodata_truth_all_3;
  delete h_pseudodata_truth_all_sd_3;
  delete h_pseudodata_truth_all_puppi_3;
  delete h_pseudodata_truth_all_puppi_sd_3;
  cout << "Finished: Filling Pseudodata" << '\n';
  return;
}



/*
███████ ██ ██      ██          ████████ ████████ ██████   █████  ██████
██      ██ ██      ██             ██       ██    ██   ██ ██   ██ ██   ██
█████   ██ ██      ██             ██       ██    ██████  ███████ ██████
██      ██ ██      ██             ██       ██    ██   ██ ██   ██ ██   ██
██      ██ ███████ ███████        ██       ██    ██████  ██   ██ ██   ██
*/

void fill_ttbar(TTree* tree, TString prefix){
  if(!tree) cout << "could not read 'mc signal' tree\n";
  else      cout << "Filling Histograms for "+prefix+" ...\n";
  bool pseudo = true;
  if(!prefix.Contains("mc")) pseudo = false;

  // setup hists
  TH1* h_rec                              = binning_rec->CreateHistogram(prefix+"_rec");
  TH1* h_rec_sd                           = binning_rec->CreateHistogram(prefix+"_rec_sd");
  TH1* h_rec_puppi                        = binning_rec->CreateHistogram(prefix+"_rec_puppi");
  TH1* h_rec_puppi_sd                     = binning_rec->CreateHistogram(prefix+"_rec_puppi_sd");
  TH1* h_rec_dist                         = measurement_rec->CreateHistogram(prefix+"_rec_dist",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_sd                      = measurement_rec->CreateHistogram(prefix+"_rec_dist_sd",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi                   = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_sd                = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_sd",kTRUE,0,0, "mass[C]");
  TH1* h_gen                              = binning_gen->CreateHistogram(prefix+"_gen");
  TH1* h_gen_sd                           = binning_gen->CreateHistogram(prefix+"_gen_sd");
  TH1* h_gen_puppi                        = binning_gen->CreateHistogram(prefix+"_gen_puppi");
  TH1* h_gen_puppi_sd                     = binning_gen->CreateHistogram(prefix+"_gen_puppi_sd");
  TH1* h_truth_all                        = binning_gen->CreateHistogram(prefix+"_truth_all");
  TH1* h_truth_all_sd                     = binning_gen->CreateHistogram(prefix+"_truth_all_sd");
  TH1* h_truth_all_puppi                  = binning_gen->CreateHistogram(prefix+"_truth_all_puppi");
  TH1* h_truth_all_puppi_sd               = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_sd");
  TH1* h_truth                            = measurement_gen->CreateHistogram(prefix+"_truth",kTRUE,0,0, "mass[C]");
  TH1* h_truth_sd                         = measurement_gen->CreateHistogram(prefix+"_truth_sd",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi                      = measurement_gen->CreateHistogram(prefix+"_truth_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_sd                   = measurement_gen->CreateHistogram(prefix+"_truth_puppi_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all                       = measurement_gen->CreateHistogram(prefix+"_purity_all",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_sd                    = measurement_gen->CreateHistogram(prefix+"_purity_all_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi                 = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_sd              = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin                   = measurement_gen->CreateHistogram(prefix+"_purity_samebin",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_sd                = measurement_gen->CreateHistogram(prefix+"_purity_samebin_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi             = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_sd          = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass              = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_sd           = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_sd",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi        = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_sd     = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_sd",kTRUE,0,0, "mass[C]");
  TH2* h_mc_matrix          = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix");
  TH2* h_mc_matrix_sd       = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_sd");
  TH2* h_mc_matrix_puppi    = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi");
  TH2* h_mc_matrix_puppi_sd = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_sd");

  TH1* h_rec_1                            = binning_rec->CreateHistogram(prefix+"_rec_1");
  TH1* h_rec_sd_1                         = binning_rec->CreateHistogram(prefix+"_rec_sd_1");
  TH1* h_rec_puppi_1                      = binning_rec->CreateHistogram(prefix+"_rec_puppi_1");
  TH1* h_rec_puppi_sd_1                   = binning_rec->CreateHistogram(prefix+"_rec_puppi_sd_1");
  TH1* h_rec_dist_1                       = measurement_rec->CreateHistogram(prefix+"_rec_dist_1",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_sd_1                    = measurement_rec->CreateHistogram(prefix+"_rec_dist_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_1                 = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_sd_1              = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_gen_1                            = binning_gen->CreateHistogram(prefix+"_gen_1");
  TH1* h_gen_sd_1                         = binning_gen->CreateHistogram(prefix+"_gen_sd_1");
  TH1* h_gen_puppi_1                      = binning_gen->CreateHistogram(prefix+"_gen_puppi_1");
  TH1* h_gen_puppi_sd_1                   = binning_gen->CreateHistogram(prefix+"_gen_puppi_sd_1");
  TH1* h_truth_all_1                      = binning_gen->CreateHistogram(prefix+"_truth_all_1");
  TH1* h_truth_all_sd_1                   = binning_gen->CreateHistogram(prefix+"_truth_all_sd_1");
  TH1* h_truth_all_puppi_1                = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_1");
  TH1* h_truth_all_puppi_sd_1             = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_sd_1");
  TH1* h_truth_1                          = measurement_gen->CreateHistogram(prefix+"_truth_1",kTRUE,0,0, "mass[C]");
  TH1* h_truth_sd_1                       = measurement_gen->CreateHistogram(prefix+"_truth_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_1                    = measurement_gen->CreateHistogram(prefix+"_truth_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_sd_1                 = measurement_gen->CreateHistogram(prefix+"_truth_puppi_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_1                     = measurement_gen->CreateHistogram(prefix+"_purity_all_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_sd_1                  = measurement_gen->CreateHistogram(prefix+"_purity_all_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_1               = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_sd_1            = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_1                 = measurement_gen->CreateHistogram(prefix+"_purity_samebin_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_sd_1              = measurement_gen->CreateHistogram(prefix+"_purity_samebin_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_1           = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_sd_1        = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_1            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_sd_1         = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_sd_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_1      = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_sd_1   = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_sd_1",kTRUE,0,0, "mass[C]");

  TH1* h_rec_2                            = binning_rec->CreateHistogram(prefix+"_rec_2");
  TH1* h_rec_sd_2                         = binning_rec->CreateHistogram(prefix+"_rec_sd_2");
  TH1* h_rec_puppi_2                      = binning_rec->CreateHistogram(prefix+"_rec_puppi_2");
  TH1* h_rec_puppi_sd_2                   = binning_rec->CreateHistogram(prefix+"_rec_puppi_sd_2");
  TH1* h_rec_dist_2                       = measurement_rec->CreateHistogram(prefix+"_rec_dist_2",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_sd_2                    = measurement_rec->CreateHistogram(prefix+"_rec_dist_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_2                 = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_sd_2              = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_gen_2                            = binning_gen->CreateHistogram(prefix+"_gen_2");
  TH1* h_gen_sd_2                         = binning_gen->CreateHistogram(prefix+"_gen_sd_2");
  TH1* h_gen_puppi_2                      = binning_gen->CreateHistogram(prefix+"_gen_puppi_2");
  TH1* h_gen_puppi_sd_2                   = binning_gen->CreateHistogram(prefix+"_gen_puppi_sd_2");
  TH1* h_truth_all_2                      = binning_gen->CreateHistogram(prefix+"_truth_all_2");
  TH1* h_truth_all_sd_2                   = binning_gen->CreateHistogram(prefix+"_truth_all_sd_2");
  TH1* h_truth_all_puppi_2                = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_2");
  TH1* h_truth_all_puppi_sd_2             = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_sd_2");
  TH1* h_truth_2                          = measurement_gen->CreateHistogram(prefix+"_truth_2",kTRUE,0,0, "mass[C]");
  TH1* h_truth_sd_2                       = measurement_gen->CreateHistogram(prefix+"_truth_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_2                    = measurement_gen->CreateHistogram(prefix+"_truth_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_sd_2                 = measurement_gen->CreateHistogram(prefix+"_truth_puppi_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_2                     = measurement_gen->CreateHistogram(prefix+"_purity_all_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_sd_2                  = measurement_gen->CreateHistogram(prefix+"_purity_all_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_2               = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_sd_2            = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_2                 = measurement_gen->CreateHistogram(prefix+"_purity_samebin_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_sd_2              = measurement_gen->CreateHistogram(prefix+"_purity_samebin_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_2           = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_sd_2        = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_2            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_sd_2         = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_sd_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_2      = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_sd_2   = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_sd_2",kTRUE,0,0, "mass[C]");

  TH1* h_rec_3                            = binning_rec->CreateHistogram(prefix+"_rec_3");
  TH1* h_rec_sd_3                         = binning_rec->CreateHistogram(prefix+"_rec_sd_3");
  TH1* h_rec_puppi_3                      = binning_rec->CreateHistogram(prefix+"_rec_puppi_3");
  TH1* h_rec_puppi_sd_3                   = binning_rec->CreateHistogram(prefix+"_rec_puppi_sd_3");
  TH1* h_rec_dist_3                       = measurement_rec->CreateHistogram(prefix+"_rec_dist_3",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_sd_3                    = measurement_rec->CreateHistogram(prefix+"_rec_dist_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_3                 = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_rec_dist_puppi_sd_3              = measurement_rec->CreateHistogram(prefix+"_rec_dist_puppi_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_gen_3                            = binning_gen->CreateHistogram(prefix+"_gen_3");
  TH1* h_gen_sd_3                         = binning_gen->CreateHistogram(prefix+"_gen_sd_3");
  TH1* h_gen_puppi_3                      = binning_gen->CreateHistogram(prefix+"_gen_puppi_3");
  TH1* h_gen_puppi_sd_3                   = binning_gen->CreateHistogram(prefix+"_gen_puppi_sd_3");
  TH1* h_truth_all_3                      = binning_gen->CreateHistogram(prefix+"_truth_all_3");
  TH1* h_truth_all_sd_3                   = binning_gen->CreateHistogram(prefix+"_truth_all_sd_3");
  TH1* h_truth_all_puppi_3                = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_3");
  TH1* h_truth_all_puppi_sd_3             = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_sd_3");
  TH1* h_truth_3                          = measurement_gen->CreateHistogram(prefix+"_truth_3",kTRUE,0,0, "mass[C]");
  TH1* h_truth_sd_3                       = measurement_gen->CreateHistogram(prefix+"_truth_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_3                    = measurement_gen->CreateHistogram(prefix+"_truth_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_truth_puppi_sd_3                 = measurement_gen->CreateHistogram(prefix+"_truth_puppi_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_3                     = measurement_gen->CreateHistogram(prefix+"_purity_all_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_sd_3                  = measurement_gen->CreateHistogram(prefix+"_purity_all_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_3               = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_puppi_sd_3            = measurement_gen->CreateHistogram(prefix+"_purity_all_puppi_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_3                 = measurement_gen->CreateHistogram(prefix+"_purity_samebin_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_sd_3              = measurement_gen->CreateHistogram(prefix+"_purity_samebin_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_3           = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_puppi_sd_3        = measurement_gen->CreateHistogram(prefix+"_purity_samebin_puppi_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_3            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_sd_3         = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_sd_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_3      = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_puppi_sd_3   = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_puppi_sd_3",kTRUE,0,0, "mass[C]");

  TH2* h_mc_matrix_1          = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_1");
  TH2* h_mc_matrix_sd_1       = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_sd_1");
  TH2* h_mc_matrix_puppi_1    = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_1");
  TH2* h_mc_matrix_puppi_sd_1 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_sd_1");

  TH2* h_mc_matrix_2          = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_2");
  TH2* h_mc_matrix_sd_2       = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_sd_2");
  TH2* h_mc_matrix_puppi_2    = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_2");
  TH2* h_mc_matrix_puppi_sd_2 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_sd_2");

  TH2* h_mc_matrix_3          = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_3");
  TH2* h_mc_matrix_sd_3       = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_sd_3");
  TH2* h_mc_matrix_puppi_3    = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_3");
  TH2* h_mc_matrix_puppi_sd_3 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_puppi_sd_3");

  if(!Muon_done) outputFile->cd();
  else outputFile2->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_gen", &tau32_gen);
  tree->SetBranchAddress("h_tau32_gen_sd", &tau32_gen_sd);
  tree->SetBranchAddress("h_mass_gen", &mass_gen);
  tree->SetBranchAddress("h_mass_gen_sd", &mass_gen_sd);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
  tree->SetBranchAddress("h_passed_gen_final_sd", &passed_gen_final_sd);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband", &passed_gen_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband_sd", &passed_gen_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_gen_mass_sideband", &passed_gen_mass_sideband);
  tree->SetBranchAddress("h_passed_gen_mass_sideband_sd", &passed_gen_mass_sideband_sd);

  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_rec_sd", &tau32_rec_sd);
  tree->SetBranchAddress("h_tau32_rec_puppi", &tau32_rec_puppi);
  tree->SetBranchAddress("h_tau32_rec_puppi_sd", &tau32_rec_puppi_sd);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_rec_sd", &mass_rec_sd);
  tree->SetBranchAddress("h_mass_rec_puppi", &mass_rec_puppi);
  tree->SetBranchAddress("h_mass_rec_puppi_sd", &mass_rec_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_final_sd", &passed_rec_final_sd);
  tree->SetBranchAddress("h_passed_rec_final_puppi", &passed_rec_final_puppi);
  tree->SetBranchAddress("h_passed_rec_final_puppi_sd", &passed_rec_final_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_sd", &passed_rec_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi", &passed_rec_pt_topjet_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi_sd", &passed_rec_pt_topjet_sideband_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_sd", &passed_rec_mass_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi", &passed_rec_mass_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi_sd", &passed_rec_mass_sideband_puppi_sd);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchAddress("h_gen_weight", &gen_weight);
  tree->SetBranchStatus("*", 1);

  int counter = 1;
  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;
    // std::cout << "tau32_gen: " << tau32_gen << '\n';
    // std::cout << "tau32_gen_sd: " << tau32_gen_sd << '\n';
    double scale = 10/8;

    // get weights for migration matrix
    w_central_scaled = rec_weight*scale;
    w_nogen_scaled = rec_weight*scale;
    w_norec_scaled = gen_weight*scale;
    w_correction_scaled = gen_weight*scale - rec_weight*scale;

    w_central = rec_weight;
    w_nogen = rec_weight;
    w_norec = gen_weight;
    w_correction = gen_weight - rec_weight;

    // get weight for gen and rec hists
    w_sig_rec_scaled = rec_weight*scale;
    w_gen_scaled = gen_weight*scale;
    // get weight for gen and rec hists
    w_sig_rec = rec_weight;
    w_gen = gen_weight;

    // fill histograms for CHS Jets w/o SoftDrop
    Int_t rec_binNumber = 0, gen_binNumber = 0;
    if(passed_rec_final)                   rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband) rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)      rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);

    if(passed_gen_final)                   gen_binNumber = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband) gen_binNumber = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)      gen_binNumber = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info = false, gen_info = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband) gen_info = true;
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband) rec_info = true;

    if(rec_info){
      h_rec->Fill(rec_binNumber, w_sig_rec);
      h_rec_dist->Fill(tau32_rec, w_sig_rec);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_rec_1->Fill(rec_binNumber, w_sig_rec_scaled);
          h_rec_dist_1->Fill(tau32_rec, w_sig_rec_scaled);
        }
        if(counter != 3 || counter != 4){
          h_rec_2->Fill(rec_binNumber, w_sig_rec_scaled);
          h_rec_dist_2->Fill(tau32_rec, w_sig_rec_scaled);
        }
        if(counter != 5 || counter != 6){
          h_rec_3->Fill(rec_binNumber, w_sig_rec_scaled);
          h_rec_dist_3->Fill(tau32_rec, w_sig_rec_scaled);
        }
      }
    }
    if(passed_gen_final){
      h_truth->Fill(tau32_gen, w_gen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_truth_1->Fill(tau32_gen, w_gen_scaled);
        if(counter != 3 || counter != 4)  h_truth_2->Fill(tau32_gen, w_gen_scaled);
        if(counter != 5 || counter != 6) h_truth_3->Fill(tau32_gen, w_gen_scaled);
      }
    }
    if(gen_info){
      h_gen->Fill(gen_binNumber, w_gen);
      h_truth_all->Fill(gen_binNumber, w_gen);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_gen_1->Fill(gen_binNumber, w_gen_scaled);
          h_truth_all_1->Fill(gen_binNumber, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_gen_2->Fill(gen_binNumber, w_gen_scaled);
          h_truth_all_2->Fill(gen_binNumber, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_gen_3->Fill(gen_binNumber, w_gen_scaled);
          h_truth_all_3->Fill(gen_binNumber, w_gen_scaled);
        }
      }
    }

    if( rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_central);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
      }
    }
    if(!rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_norec);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
      }
    }
    if( rec_info && !gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_nogen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
      }
    }
    if( rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, 0., w_correction);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_1->Fill(gen_binNumber, 0., w_correction_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_2->Fill(gen_binNumber, 0., w_correction_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_3->Fill(gen_binNumber, 0., w_correction_scaled);
      }
    }

    //fill hists for purity
    int genBin_recInfo = 0, genBin_recInfo_mass = 0;

    if(passed_gen_final && rec_info){
      genBin_recInfo      = measurement_gen->GetGlobalBinNumber(tau32_rec,mass_gen);
      genBin_recInfo_mass = measurement_gen->GetGlobalBinNumber(tau32_rec,mass_rec);

      h_purity_all->Fill(tau32_gen, w_gen);
      if(genBin_recInfo == gen_binNumber)      h_purity_samebin->Fill(tau32_gen, w_gen);
      if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass->Fill(tau32_gen, w_gen);


      if(pseudo){
        if(counter != 1 || counter != 2){
          h_purity_all_1->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo == gen_binNumber)      h_purity_samebin_1->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_1->Fill(tau32_gen, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_purity_all_2->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo == gen_binNumber)      h_purity_samebin_2->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_2->Fill(tau32_gen, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_purity_all_3->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo == gen_binNumber)      h_purity_samebin_3->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_3->Fill(tau32_gen, w_gen_scaled);
        }
      }
    }

    // fill histograms for CHS Jets w/ SoftDrop
    Int_t rec_binNumber_sd = 0, gen_binNumber_sd = 0;
    if(passed_rec_final_sd)                   rec_binNumber_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_sd, mass_rec_sd);
    else if(passed_rec_pt_topjet_sideband_sd) rec_binNumber_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_sd);
    else if(passed_rec_mass_sideband_sd)      rec_binNumber_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_sd);

    if(passed_gen_final_sd)                   gen_binNumber_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd) gen_binNumber_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)      gen_binNumber_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_sd = false, gen_info_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd) gen_info_sd = true;
    if(passed_rec_final_sd || passed_rec_pt_topjet_sideband_sd || passed_rec_mass_sideband_sd) rec_info_sd = true;

    if(rec_info_sd){
      h_rec_sd->Fill(rec_binNumber_sd, w_sig_rec);
      h_rec_dist_sd->Fill(tau32_rec_sd, w_sig_rec);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_rec_sd_1->Fill(rec_binNumber_sd, w_sig_rec_scaled);
          h_rec_dist_sd_1->Fill(tau32_rec_sd, w_sig_rec_scaled);
        }
        if(counter != 3 || counter != 4){
          h_rec_sd_2->Fill(rec_binNumber_sd, w_sig_rec_scaled);
          h_rec_dist_sd_2->Fill(tau32_rec_sd, w_sig_rec_scaled);
        }
        if(counter != 5 || counter != 6){
          h_rec_sd_3->Fill(rec_binNumber_sd, w_sig_rec_scaled);
          h_rec_dist_sd_3->Fill(tau32_rec_sd, w_sig_rec_scaled);
        }
      }
    }
    if(passed_gen_final_sd){
      h_truth_sd->Fill(tau32_gen_sd, w_gen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_truth_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
        if(counter != 3 || counter != 4)  h_truth_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
        if(counter != 5 || counter != 6) h_truth_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
      }
    }
    if(gen_info_sd){
      h_gen_sd->Fill(gen_binNumber_sd, w_gen);
      h_truth_all_sd->Fill(gen_binNumber_sd, w_gen);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_gen_sd_1->Fill(gen_binNumber_sd, w_gen_scaled);
          h_truth_all_sd_1->Fill(gen_binNumber_sd, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_gen_sd_2->Fill(gen_binNumber_sd, w_gen_scaled);
          h_truth_all_sd_2->Fill(gen_binNumber_sd, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_gen_sd_3->Fill(gen_binNumber_sd, w_gen_scaled);
          h_truth_all_sd_3->Fill(gen_binNumber_sd, w_gen_scaled);
        }
      }
    }

    if( rec_info_sd &&  gen_info_sd){
      h_mc_matrix_sd->Fill(gen_binNumber_sd, rec_binNumber_sd, w_central);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_sd_1->Fill(gen_binNumber_sd, rec_binNumber_sd, w_central_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_sd_2->Fill(gen_binNumber_sd, rec_binNumber_sd, w_central_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_sd_3->Fill(gen_binNumber_sd, rec_binNumber_sd, w_central_scaled);
      }
    }
    if(!rec_info_sd &&  gen_info_sd){
      h_mc_matrix_sd->Fill(gen_binNumber_sd, rec_binNumber_sd, w_norec);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_sd_1->Fill(gen_binNumber_sd, rec_binNumber_sd, w_norec_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_sd_2->Fill(gen_binNumber_sd, rec_binNumber_sd, w_norec_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_sd_3->Fill(gen_binNumber_sd, rec_binNumber_sd, w_norec_scaled);
      }
    }
    if( rec_info_sd && !gen_info_sd){
      h_mc_matrix_sd->Fill(gen_binNumber_sd, rec_binNumber_sd, w_nogen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_sd_1->Fill(gen_binNumber_sd, rec_binNumber_sd, w_nogen_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_sd_2->Fill(gen_binNumber_sd, rec_binNumber_sd, w_nogen_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_sd_3->Fill(gen_binNumber_sd, rec_binNumber_sd, w_nogen_scaled);
      }
    }
    if( rec_info_sd &&  gen_info_sd){
      h_mc_matrix_sd->Fill(gen_binNumber_sd, 0., w_correction);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_sd_1->Fill(gen_binNumber_sd, 0., w_correction_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_sd_2->Fill(gen_binNumber_sd, 0., w_correction_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_sd_3->Fill(gen_binNumber_sd, 0., w_correction_scaled);
      }
    }

    //fill hists for purity
    int genBin_recInfo_sd = 0, genBin_recInfo_mass_sd = 0;

    if(passed_gen_final_sd && rec_info_sd){
      genBin_recInfo_sd      = measurement_gen->GetGlobalBinNumber(tau32_rec_sd,mass_gen_sd);
      genBin_recInfo_mass_sd = measurement_gen->GetGlobalBinNumber(tau32_rec_sd,mass_rec_sd);

      h_purity_all_sd->Fill(tau32_gen_sd, w_gen);
      if(genBin_recInfo_sd == gen_binNumber_sd)      h_purity_samebin_sd->Fill(tau32_gen_sd, w_gen);
      if(genBin_recInfo_mass_sd == gen_binNumber_sd) h_purity_samebin_mass_sd->Fill(tau32_gen_sd, w_gen);


      if(pseudo){
        if(counter != 1 || counter != 2){
          h_purity_all_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_sd == gen_binNumber_sd)      h_purity_samebin_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_sd == gen_binNumber_sd) h_purity_samebin_mass_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_purity_all_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_sd == gen_binNumber_sd)      h_purity_samebin_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_sd == gen_binNumber_sd) h_purity_samebin_mass_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_purity_all_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_sd == gen_binNumber_sd)      h_purity_samebin_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_sd == gen_binNumber_sd) h_purity_samebin_mass_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
        }
      }
    }

    // fill histograms for PUPPI Jets w/o SoftDrop
    Int_t rec_binNumber_puppi = 0, gen_binNumber_puppi = 0;
    if(passed_rec_final_puppi)                   rec_binNumber_puppi = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi, mass_rec_puppi);
    else if(passed_rec_pt_topjet_sideband_puppi) rec_binNumber_puppi = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi);
    else if(passed_rec_mass_sideband_puppi)      rec_binNumber_puppi = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi);

    if(passed_gen_final)                   gen_binNumber_puppi = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband) gen_binNumber_puppi = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)      gen_binNumber_puppi = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info_puppi = false, gen_info_puppi = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband)                   gen_info_puppi = true;
    if(passed_rec_final_puppi || passed_rec_pt_topjet_sideband_puppi || passed_rec_mass_sideband_puppi) rec_info_puppi = true;

    if(rec_info_puppi){
      h_rec_puppi->Fill(rec_binNumber_puppi, w_sig_rec);
      h_rec_dist_puppi->Fill(tau32_rec_puppi, w_sig_rec);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_rec_puppi_1->Fill(rec_binNumber_puppi, w_sig_rec_scaled);
          h_rec_dist_puppi_1->Fill(tau32_rec_puppi, w_sig_rec_scaled);
        }
        if(counter != 3 || counter != 4){
          h_rec_puppi_2->Fill(rec_binNumber_puppi, w_sig_rec_scaled);
          h_rec_dist_puppi_2->Fill(tau32_rec_puppi, w_sig_rec_scaled);
        }
        if(counter != 5 || counter != 6){
          h_rec_puppi_3->Fill(rec_binNumber_puppi, w_sig_rec_scaled);
          h_rec_dist_puppi_3->Fill(tau32_rec_puppi, w_sig_rec_scaled);
        }
      }
    }
    if(passed_gen_final){
      h_truth_puppi->Fill(tau32_gen, w_gen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_truth_puppi_1->Fill(tau32_gen, w_gen_scaled);
        if(counter != 3 || counter != 4)  h_truth_puppi_2->Fill(tau32_gen, w_gen_scaled);
        if(counter != 5 || counter != 6) h_truth_puppi_3->Fill(tau32_gen, w_gen_scaled);
      }
    }
    if(gen_info_puppi){
      h_gen_puppi->Fill(gen_binNumber_puppi, w_gen);
      h_truth_all_puppi->Fill(gen_binNumber_puppi, w_gen);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_gen_puppi_1->Fill(gen_binNumber_puppi, w_gen_scaled);
          h_truth_all_puppi_1->Fill(gen_binNumber_puppi, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_gen_puppi_2->Fill(gen_binNumber_puppi, w_gen_scaled);
          h_truth_all_puppi_2->Fill(gen_binNumber_puppi, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_gen_puppi_3->Fill(gen_binNumber_puppi, w_gen_scaled);
          h_truth_all_puppi_3->Fill(gen_binNumber_puppi, w_gen_scaled);
        }
      }
    }

    if( rec_info_puppi &&  gen_info_puppi){
      h_mc_matrix_puppi->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_central);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_1->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_central_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_2->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_central_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_3->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_central_scaled);
      }
    }
    if(!rec_info_puppi &&  gen_info_puppi){
      h_mc_matrix_puppi->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_norec);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_1->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_norec_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_2->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_norec_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_3->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_norec_scaled);
      }
    }
    if( rec_info_puppi && !gen_info_puppi){
      h_mc_matrix_puppi->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_nogen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_1->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_nogen_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_2->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_nogen_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_3->Fill(gen_binNumber_puppi, rec_binNumber_puppi, w_nogen_scaled);
      }
    }
    if( rec_info_puppi &&  gen_info_puppi){
      h_mc_matrix_puppi->Fill(gen_binNumber_puppi, 0., w_correction);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_1->Fill(gen_binNumber_puppi, 0., w_correction_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_2->Fill(gen_binNumber_puppi, 0., w_correction_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_3->Fill(gen_binNumber_puppi, 0., w_correction_scaled);
      }
    }

    //fill hists for purity
    int genBin_recInfo_puppi = 0, genBin_recInfo_mass_puppi = 0;

    if(passed_gen_final && rec_info_puppi){
      genBin_recInfo_puppi      = measurement_gen->GetGlobalBinNumber(tau32_rec_puppi,mass_gen);
      genBin_recInfo_mass_puppi = measurement_gen->GetGlobalBinNumber(tau32_rec_puppi,mass_rec_puppi);

      h_purity_all_puppi->Fill(tau32_gen, w_gen);
      if(genBin_recInfo_puppi == gen_binNumber_puppi)      h_purity_samebin_puppi->Fill(tau32_gen, w_gen);
      if(genBin_recInfo_mass_puppi == gen_binNumber_puppi) h_purity_samebin_mass_puppi->Fill(tau32_gen, w_gen);


      if(pseudo){
        if(counter != 1 || counter != 2){
          h_purity_all_puppi_1->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_puppi == gen_binNumber_puppi)      h_purity_samebin_puppi_1->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass_puppi == gen_binNumber_puppi) h_purity_samebin_mass_puppi_1->Fill(tau32_gen, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_purity_all_puppi_2->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_puppi == gen_binNumber_puppi)      h_purity_samebin_puppi_2->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass_puppi == gen_binNumber_puppi) h_purity_samebin_mass_puppi_2->Fill(tau32_gen, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_purity_all_puppi_3->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_puppi == gen_binNumber_puppi)      h_purity_samebin_puppi_3->Fill(tau32_gen, w_gen_scaled);
          if(genBin_recInfo_mass_puppi == gen_binNumber_puppi) h_purity_samebin_mass_puppi_3->Fill(tau32_gen, w_gen_scaled);
        }
      }
    }

    // fill histograms for PUPPI Jets w/ SoftDrop
    Int_t rec_binNumber_puppi_sd = 0, gen_binNumber_puppi_sd = 0;
    if(passed_rec_final_puppi_sd)                   rec_binNumber_puppi_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi_sd, mass_rec_puppi_sd);
    else if(passed_rec_pt_topjet_sideband_puppi_sd) rec_binNumber_puppi_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);
    else if(passed_rec_mass_sideband_puppi_sd)      rec_binNumber_puppi_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);

    if(passed_gen_final_sd)                   gen_binNumber_puppi_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd) gen_binNumber_puppi_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)      gen_binNumber_puppi_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_puppi_sd = false, gen_info_puppi_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd)                   gen_info_puppi_sd = true;
    if(passed_rec_final_puppi_sd || passed_rec_pt_topjet_sideband_puppi_sd || passed_rec_mass_sideband_puppi_sd) rec_info_puppi_sd = true;

    if(rec_info_puppi_sd){
      h_rec_puppi_sd->Fill(rec_binNumber_puppi_sd, w_sig_rec);
      h_rec_dist_puppi_sd->Fill(tau32_rec_puppi_sd, w_sig_rec);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_rec_puppi_sd_1->Fill(rec_binNumber_puppi_sd, w_sig_rec_scaled);
          h_rec_dist_puppi_sd_1->Fill(tau32_rec_puppi_sd, w_sig_rec_scaled);
        }
        if(counter != 3 || counter != 4){
          h_rec_puppi_sd_2->Fill(rec_binNumber_puppi_sd, w_sig_rec_scaled);
          h_rec_dist_puppi_sd_2->Fill(tau32_rec_puppi_sd, w_sig_rec_scaled);
        }
        if(counter != 5 || counter != 6){
          h_rec_puppi_sd_3->Fill(rec_binNumber_puppi_sd, w_sig_rec_scaled);
          h_rec_dist_puppi_sd_3->Fill(tau32_rec_puppi_sd, w_sig_rec_scaled);
        }
      }
    }
    if(passed_gen_final_sd){
      h_truth_puppi_sd->Fill(tau32_gen_sd, w_gen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_truth_puppi_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
        if(counter != 3 || counter != 4)  h_truth_puppi_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
        if(counter != 5 || counter != 6) h_truth_puppi_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
      }
    }
    if(gen_info_puppi_sd){
      h_gen_puppi_sd->Fill(gen_binNumber_puppi_sd, w_gen);
      h_truth_all_puppi_sd->Fill(gen_binNumber_puppi_sd, w_gen);

      if(pseudo){
        if(counter != 1 || counter != 2){
          h_gen_puppi_sd_1->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
          h_truth_all_puppi_sd_1->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_gen_puppi_sd_2->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
          h_truth_all_puppi_sd_2->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_gen_puppi_sd_3->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
          h_truth_all_puppi_sd_3->Fill(gen_binNumber_puppi_sd, w_gen_scaled);
        }
      }
    }

    if( rec_info_puppi_sd &&  gen_info_puppi_sd){
      h_mc_matrix_puppi_sd->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_central);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_sd_1->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_central_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_sd_2->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_central_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_sd_3->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_central_scaled);
      }
    }
    if(!rec_info_puppi_sd &&  gen_info_puppi_sd){
      h_mc_matrix_puppi_sd->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_norec);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_sd_1->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_norec_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_sd_2->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_norec_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_sd_3->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_norec_scaled);
      }
    }
    if( rec_info_puppi_sd && !gen_info_puppi_sd){
      h_mc_matrix_puppi_sd->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_nogen);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_sd_1->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_nogen_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_sd_2->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_nogen_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_sd_3->Fill(gen_binNumber_puppi_sd, rec_binNumber_puppi_sd, w_nogen_scaled);
      }
    }
    if( rec_info_puppi_sd &&  gen_info_puppi_sd){
      h_mc_matrix_puppi_sd->Fill(gen_binNumber_puppi_sd, 0., w_correction);
      if(pseudo){
        if(counter != 1 || counter != 2)  h_mc_matrix_puppi_sd_1->Fill(gen_binNumber_puppi_sd, 0., w_correction_scaled);
        if(counter != 3 || counter != 4)  h_mc_matrix_puppi_sd_2->Fill(gen_binNumber_puppi_sd, 0., w_correction_scaled);
        if(counter != 5 || counter != 6) h_mc_matrix_puppi_sd_3->Fill(gen_binNumber_puppi_sd, 0., w_correction_scaled);
      }
    }

    //fill hists for purity
    int genBin_recInfo_puppi_sd = 0, genBin_recInfo_mass_puppi_sd = 0;

    if(passed_gen_final_sd && rec_info_puppi_sd){
      genBin_recInfo_puppi_sd      = measurement_gen->GetGlobalBinNumber(tau32_rec_puppi_sd,mass_gen_sd);
      genBin_recInfo_mass_puppi_sd = measurement_gen->GetGlobalBinNumber(tau32_rec_puppi_sd,mass_rec_puppi_sd);

      h_purity_all_puppi_sd->Fill(tau32_gen_sd, w_gen);
      if(genBin_recInfo_puppi_sd == gen_binNumber_puppi_sd)      h_purity_samebin_puppi_sd->Fill(tau32_gen_sd, w_gen);
      if(genBin_recInfo_mass_puppi_sd == gen_binNumber_puppi_sd) h_purity_samebin_mass_puppi_sd->Fill(tau32_gen_sd, w_gen);


      if(pseudo){
        if(counter != 1 || counter != 2){
          h_purity_all_puppi_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_puppi_sd == gen_binNumber_puppi_sd)      h_purity_samebin_puppi_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_puppi_sd == gen_binNumber_puppi_sd) h_purity_samebin_mass_puppi_sd_1->Fill(tau32_gen_sd, w_gen_scaled);
        }
        if(counter != 3 || counter != 4){
          h_purity_all_puppi_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_puppi_sd == gen_binNumber_puppi_sd)      h_purity_samebin_puppi_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_puppi_sd == gen_binNumber_puppi_sd) h_purity_samebin_mass_puppi_sd_2->Fill(tau32_gen_sd, w_gen_scaled);
        }
        if(counter != 5 || counter != 6){
          h_purity_all_puppi_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_puppi_sd == gen_binNumber_puppi_sd)      h_purity_samebin_puppi_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
          if(genBin_recInfo_mass_puppi_sd == gen_binNumber_puppi_sd) h_purity_samebin_mass_puppi_sd_3->Fill(tau32_gen_sd, w_gen_scaled);
        }
      }
    }

    counter++;
    if(counter > 10) counter = 1;
  }

  h_rec->Write();
  h_rec_sd->Write();
  h_rec_puppi->Write();
  h_rec_puppi_sd->Write();
  h_rec_dist->Write();
  h_rec_dist_sd->Write();
  h_rec_dist_puppi->Write();
  h_rec_dist_puppi_sd->Write();
  h_gen->Write();
  h_gen_sd->Write();
  h_gen_puppi->Write();
  h_gen_puppi_sd->Write();
  h_truth_all->Write();
  h_truth_all_sd->Write();
  h_truth_all_puppi->Write();
  h_truth_all_puppi_sd->Write();
  h_truth->Write();
  h_truth_sd->Write();
  h_truth_puppi->Write();
  h_truth_puppi_sd->Write();
  h_mc_matrix->Write();
  h_mc_matrix_sd->Write();
  h_mc_matrix_puppi->Write();
  h_mc_matrix_puppi_sd->Write();
  h_purity_all->Write();
  h_purity_all_sd->Write();
  h_purity_all_puppi->Write();
  h_purity_all_puppi_sd->Write();
  h_purity_samebin->Write();
  h_purity_samebin_sd->Write();
  h_purity_samebin_puppi->Write();
  h_purity_samebin_puppi_sd->Write();
  h_purity_samebin_mass->Write();
  h_purity_samebin_mass_sd->Write();
  h_purity_samebin_mass_puppi->Write();
  h_purity_samebin_mass_puppi_sd->Write();

  delete h_rec;
  delete h_rec_sd;
  delete h_rec_puppi;
  delete h_rec_puppi_sd;
  delete h_rec_dist;
  delete h_rec_dist_sd;
  delete h_rec_dist_puppi;
  delete h_rec_dist_puppi_sd;
  delete h_gen;
  delete h_gen_sd;
  delete h_gen_puppi;
  delete h_gen_puppi_sd;
  delete h_truth_all;
  delete h_truth_all_sd;
  delete h_truth_all_puppi;
  delete h_truth_all_puppi_sd;
  delete h_truth;
  delete h_truth_sd;
  delete h_truth_puppi;
  delete h_truth_puppi_sd;
  delete h_mc_matrix;
  delete h_mc_matrix_sd;
  delete h_mc_matrix_puppi;
  delete h_mc_matrix_puppi_sd;
  delete h_purity_all;
  delete h_purity_all_sd;
  delete h_purity_all_puppi;
  delete h_purity_all_puppi_sd;
  delete h_purity_samebin;
  delete h_purity_samebin_sd;
  delete h_purity_samebin_puppi;
  delete h_purity_samebin_puppi_sd;
  delete h_purity_samebin_mass;
  delete h_purity_samebin_mass_sd;
  delete h_purity_samebin_mass_puppi;
  delete h_purity_samebin_mass_puppi_sd;

  if(pseudo){
    h_rec_1->Write();
    h_rec_sd_1->Write();
    h_rec_puppi_1->Write();
    h_rec_puppi_sd_1->Write();
    h_rec_dist_1->Write();
    h_rec_dist_sd_1->Write();
    h_rec_dist_puppi_1->Write();
    h_rec_dist_puppi_sd_1->Write();
    h_gen_1->Write();
    h_gen_sd_1->Write();
    h_gen_puppi_1->Write();
    h_gen_puppi_sd_1->Write();
    h_truth_all_1->Write();
    h_truth_all_sd_1->Write();
    h_truth_all_puppi_1->Write();
    h_truth_all_puppi_sd_1->Write();
    h_truth_1->Write();
    h_truth_sd_1->Write();
    h_truth_puppi_1->Write();
    h_truth_puppi_sd_1->Write();
    h_mc_matrix_1->Write();
    h_mc_matrix_sd_1->Write();
    h_mc_matrix_puppi_1->Write();
    h_mc_matrix_puppi_sd_1->Write();
    h_purity_all_1->Write();
    h_purity_all_sd_1->Write();
    h_purity_all_puppi_1->Write();
    h_purity_all_puppi_sd_1->Write();
    h_purity_samebin_1->Write();
    h_purity_samebin_sd_1->Write();
    h_purity_samebin_puppi_1->Write();
    h_purity_samebin_puppi_sd_1->Write();
    h_purity_samebin_mass_1->Write();
    h_purity_samebin_mass_sd_1->Write();
    h_purity_samebin_mass_puppi_1->Write();
    h_purity_samebin_mass_puppi_sd_1->Write();

    h_rec_2->Write();
    h_rec_sd_2->Write();
    h_rec_puppi_2->Write();
    h_rec_puppi_sd_2->Write();
    h_rec_dist_2->Write();
    h_rec_dist_sd_2->Write();
    h_rec_dist_puppi_2->Write();
    h_rec_dist_puppi_sd_2->Write();
    h_gen_2->Write();
    h_gen_sd_2->Write();
    h_gen_puppi_2->Write();
    h_gen_puppi_sd_2->Write();
    h_truth_all_2->Write();
    h_truth_all_sd_2->Write();
    h_truth_all_puppi_2->Write();
    h_truth_all_puppi_sd_2->Write();
    h_truth_2->Write();
    h_truth_sd_2->Write();
    h_truth_puppi_2->Write();
    h_truth_puppi_sd_2->Write();
    h_mc_matrix_2->Write();
    h_mc_matrix_sd_2->Write();
    h_mc_matrix_puppi_2->Write();
    h_mc_matrix_puppi_sd_2->Write();
    h_purity_all_2->Write();
    h_purity_all_sd_2->Write();
    h_purity_all_puppi_2->Write();
    h_purity_all_puppi_sd_2->Write();
    h_purity_samebin_2->Write();
    h_purity_samebin_sd_2->Write();
    h_purity_samebin_puppi_2->Write();
    h_purity_samebin_puppi_sd_2->Write();
    h_purity_samebin_mass_2->Write();
    h_purity_samebin_mass_sd_2->Write();
    h_purity_samebin_mass_puppi_2->Write();
    h_purity_samebin_mass_puppi_sd_2->Write();

    h_rec_3->Write();
    h_rec_sd_3->Write();
    h_rec_puppi_3->Write();
    h_rec_puppi_sd_3->Write();
    h_rec_dist_3->Write();
    h_rec_dist_sd_3->Write();
    h_rec_dist_puppi_3->Write();
    h_rec_dist_puppi_sd_3->Write();
    h_gen_3->Write();
    h_gen_sd_3->Write();
    h_gen_puppi_3->Write();
    h_gen_puppi_sd_3->Write();
    h_truth_all_3->Write();
    h_truth_all_sd_3->Write();
    h_truth_all_puppi_3->Write();
    h_truth_all_puppi_sd_3->Write();
    h_truth_3->Write();
    h_truth_sd_3->Write();
    h_truth_puppi_3->Write();
    h_truth_puppi_sd_3->Write();
    h_mc_matrix_3->Write();
    h_mc_matrix_sd_3->Write();
    h_mc_matrix_puppi_3->Write();
    h_mc_matrix_puppi_sd_3->Write();
    h_purity_all_3->Write();
    h_purity_all_sd_3->Write();
    h_purity_all_puppi_3->Write();
    h_purity_all_puppi_sd_3->Write();
    h_purity_samebin_3->Write();
    h_purity_samebin_sd_3->Write();
    h_purity_samebin_puppi_3->Write();
    h_purity_samebin_puppi_sd_3->Write();
    h_purity_samebin_mass_3->Write();
    h_purity_samebin_mass_sd_3->Write();
    h_purity_samebin_mass_puppi_3->Write();
    h_purity_samebin_mass_puppi_sd_3->Write();

    delete h_rec_1;
    delete h_rec_sd_1;
    delete h_rec_puppi_1;
    delete h_rec_puppi_sd_1;
    delete h_rec_dist_1;
    delete h_rec_dist_sd_1;
    delete h_rec_dist_puppi_1;
    delete h_rec_dist_puppi_sd_1;
    delete h_gen_1;
    delete h_gen_sd_1;
    delete h_gen_puppi_1;
    delete h_gen_puppi_sd_1;
    delete h_truth_all_1;
    delete h_truth_all_sd_1;
    delete h_truth_all_puppi_1;
    delete h_truth_all_puppi_sd_1;
    delete h_truth_1;
    delete h_truth_sd_1;
    delete h_truth_puppi_1;
    delete h_truth_puppi_sd_1;
    delete h_mc_matrix_1;
    delete h_mc_matrix_sd_1;
    delete h_mc_matrix_puppi_1;
    delete h_mc_matrix_puppi_sd_1;
    delete h_purity_all_1;
    delete h_purity_all_sd_1;
    delete h_purity_all_puppi_1;
    delete h_purity_all_puppi_sd_1;
    delete h_purity_samebin_1;
    delete h_purity_samebin_sd_1;
    delete h_purity_samebin_puppi_1;
    delete h_purity_samebin_puppi_sd_1;
    delete h_purity_samebin_mass_1;
    delete h_purity_samebin_mass_sd_1;
    delete h_purity_samebin_mass_puppi_1;
    delete h_purity_samebin_mass_puppi_sd_1;

    delete h_rec_2;
    delete h_rec_sd_2;
    delete h_rec_puppi_2;
    delete h_rec_puppi_sd_2;
    delete h_rec_dist_2;
    delete h_rec_dist_sd_2;
    delete h_rec_dist_puppi_2;
    delete h_rec_dist_puppi_sd_2;
    delete h_gen_2;
    delete h_gen_sd_2;
    delete h_gen_puppi_2;
    delete h_gen_puppi_sd_2;
    delete h_truth_all_2;
    delete h_truth_all_sd_2;
    delete h_truth_all_puppi_2;
    delete h_truth_all_puppi_sd_2;
    delete h_truth_2;
    delete h_truth_sd_2;
    delete h_truth_puppi_2;
    delete h_truth_puppi_sd_2;
    delete h_mc_matrix_2;
    delete h_mc_matrix_sd_2;
    delete h_mc_matrix_puppi_2;
    delete h_mc_matrix_puppi_sd_2;
    delete h_purity_all_2;
    delete h_purity_all_sd_2;
    delete h_purity_all_puppi_2;
    delete h_purity_all_puppi_sd_2;
    delete h_purity_samebin_2;
    delete h_purity_samebin_sd_2;
    delete h_purity_samebin_puppi_2;
    delete h_purity_samebin_puppi_sd_2;
    delete h_purity_samebin_mass_2;
    delete h_purity_samebin_mass_sd_2;
    delete h_purity_samebin_mass_puppi_2;
    delete h_purity_samebin_mass_puppi_sd_2;

    delete h_rec_3;
    delete h_rec_sd_3;
    delete h_rec_puppi_3;
    delete h_rec_puppi_sd_3;
    delete h_rec_dist_3;
    delete h_rec_dist_sd_3;
    delete h_rec_dist_puppi_3;
    delete h_rec_dist_puppi_sd_3;
    delete h_gen_3;
    delete h_gen_sd_3;
    delete h_gen_puppi_3;
    delete h_gen_puppi_sd_3;
    delete h_truth_all_3;
    delete h_truth_all_sd_3;
    delete h_truth_all_puppi_3;
    delete h_truth_all_puppi_sd_3;
    delete h_truth_3;
    delete h_truth_sd_3;
    delete h_truth_puppi_3;
    delete h_truth_puppi_sd_3;
    delete h_mc_matrix_3;
    delete h_mc_matrix_sd_3;
    delete h_mc_matrix_puppi_3;
    delete h_mc_matrix_puppi_sd_3;
    delete h_purity_all_3;
    delete h_purity_all_sd_3;
    delete h_purity_all_puppi_3;
    delete h_purity_all_puppi_sd_3;
    delete h_purity_samebin_3;
    delete h_purity_samebin_sd_3;
    delete h_purity_samebin_puppi_3;
    delete h_purity_samebin_puppi_sd_3;
    delete h_purity_samebin_mass_3;
    delete h_purity_samebin_mass_sd_3;
    delete h_purity_samebin_mass_puppi_3;
    delete h_purity_samebin_mass_puppi_sd_3;
  }
  cout << "Finished: Filling " << prefix << "!" << '\n';
  return;
}

/*
███████ ██ ██      ██          ███████  ██████  █████  ██      ███████
██      ██ ██      ██          ██      ██      ██   ██ ██      ██
█████   ██ ██      ██          ███████ ██      ███████ ██      █████
██      ██ ██      ██               ██ ██      ██   ██ ██      ██
██      ██ ███████ ███████     ███████  ██████ ██   ██ ███████ ███████
*/

void fill_scale(TTree* tree, TString prefix){
  if(!tree) cout << "could not read 'mc' tree\n";
  else      cout << "Filling " << prefix <<  " Histograms...\n";

  // setup hists
  TH1* h_pseudodata                    = binning_rec->CreateHistogram(prefix);
  TH1* h_pseudodata_sd                 = binning_rec->CreateHistogram(prefix+"_sd");
  TH1* h_pseudodata_puppi              = binning_rec->CreateHistogram(prefix+"_puppi");
  TH1* h_pseudodata_puppi_sd           = binning_rec->CreateHistogram(prefix+"_puppi_sd");
  TH1* h_pseudodata_dist               = measurement_rec->CreateHistogram(prefix+"_dist",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_sd            = measurement_rec->CreateHistogram(prefix+"_dist_sd",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi         = measurement_rec->CreateHistogram(prefix+"_dist_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_dist_puppi_sd      = measurement_rec->CreateHistogram(prefix+"_dist_puppi_sd",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all          = binning_gen->CreateHistogram(prefix+"_truth_all");
  TH1* h_pseudodata_truth_all_sd       = binning_gen->CreateHistogram(prefix+"_truth_all_sd");
  TH1* h_pseudodata_truth_all_puppi    = binning_gen->CreateHistogram(prefix+"_truth_all_puppi");
  TH1* h_pseudodata_truth_all_puppi_sd = binning_gen->CreateHistogram(prefix+"_truth_all_puppi_sd");
  TH1* h_pseudodata_truth              = measurement_gen->CreateHistogram(prefix+"_truth",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_sd           = measurement_gen->CreateHistogram(prefix+"_truth_sd",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi        = measurement_gen->CreateHistogram(prefix+"_truth_puppi",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_puppi_sd     = measurement_gen->CreateHistogram(prefix+"_truth_puppi_sd",kTRUE,0,0, "mass[C]");

  if(!Muon_done) outputFile->cd();
  else outputFile2->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_gen", &tau32_gen);
  tree->SetBranchAddress("h_tau32_gen_sd", &tau32_gen_sd);
  tree->SetBranchAddress("h_mass_gen", &mass_gen);
  tree->SetBranchAddress("h_mass_gen_sd", &mass_gen_sd);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
  tree->SetBranchAddress("h_passed_gen_final_sd", &passed_gen_final_sd);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband", &passed_gen_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband_sd", &passed_gen_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_gen_mass_sideband", &passed_gen_mass_sideband);
  tree->SetBranchAddress("h_passed_gen_mass_sideband_sd", &passed_gen_mass_sideband_sd);

  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_rec_sd", &tau32_rec_sd);
  tree->SetBranchAddress("h_tau32_rec_puppi", &tau32_rec_puppi);
  tree->SetBranchAddress("h_tau32_rec_puppi_sd", &tau32_rec_puppi_sd);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_rec_sd", &mass_rec_sd);
  tree->SetBranchAddress("h_mass_rec_puppi", &mass_rec_puppi);
  tree->SetBranchAddress("h_mass_rec_puppi_sd", &mass_rec_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_final_sd", &passed_rec_final_sd);
  tree->SetBranchAddress("h_passed_rec_final_puppi", &passed_rec_final_puppi);
  tree->SetBranchAddress("h_passed_rec_final_puppi_sd", &passed_rec_final_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_sd", &passed_rec_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi", &passed_rec_pt_topjet_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi_sd", &passed_rec_pt_topjet_sideband_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_sd", &passed_rec_mass_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi", &passed_rec_mass_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi_sd", &passed_rec_mass_sideband_puppi_sd);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchAddress("h_gen_weight", &gen_weight);
  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central = rec_weight;
    w_nogen = rec_weight;
    w_norec = gen_weight;
    w_correction = gen_weight - rec_weight;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight;
    w_gen = gen_weight;

    // fill histograms for CHS Jets w/o SoftDrop
    Int_t rec_binNumber = 0, gen_binNumber = 0;
    if(passed_rec_final)                    rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband)  rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)       rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);

    if(passed_gen_final)                    gen_binNumber = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info = false, gen_info = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband) gen_info = true;
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband) rec_info = true;


    if(rec_info){
      h_pseudodata->Fill(rec_binNumber, w_sig_rec);
      h_pseudodata_dist->Fill(tau32_rec, w_sig_rec);
    }
    if(gen_info)         h_pseudodata_truth_all->Fill(gen_binNumber, w_gen);
    if(passed_gen_final) h_pseudodata_truth->Fill(tau32_gen, w_gen);

    // fill histograms for CHS Jets w/ SoftDrop
    Int_t rec_binNumber_sd = 0, gen_binNumber_sd = 0;
    if(passed_rec_final_sd)                    rec_binNumber_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_sd, mass_rec_sd);
    else if(passed_rec_pt_topjet_sideband_sd)  rec_binNumber_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_sd);
    else if(passed_rec_mass_sideband_sd)       rec_binNumber_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_sd);

    if(passed_gen_final_sd)                    gen_binNumber_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd)  gen_binNumber_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)       gen_binNumber_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_sd = false, gen_info_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd) gen_info_sd = true;
    if(passed_rec_final_sd || passed_rec_pt_topjet_sideband_sd || passed_rec_mass_sideband_sd) rec_info_sd = true;

    if(rec_info_sd){
      h_pseudodata_sd->Fill(rec_binNumber_sd, w_sig_rec);
      h_pseudodata_dist_sd->Fill(tau32_rec_sd, w_sig_rec);
    }
    if(gen_info_sd)         h_pseudodata_truth_all_sd->Fill(gen_binNumber_sd, w_gen);
    if(passed_gen_final_sd) h_pseudodata_truth_sd->Fill(tau32_gen_sd, w_gen);

    // fill histograms for PUPPI Jets w/o SoftDrop
    Int_t rec_binNumber_puppi = 0, gen_binNumber_puppi = 0;
    if(passed_rec_final_puppi)                    rec_binNumber_puppi = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi, mass_rec_puppi);
    else if(passed_rec_pt_topjet_sideband_puppi)  rec_binNumber_puppi = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi);
    else if(passed_rec_mass_sideband_puppi)       rec_binNumber_puppi = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi);

    if(passed_gen_final)                    gen_binNumber_puppi = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber_puppi = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber_puppi = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);

    bool rec_info_puppi = false, gen_info_puppi = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband)                   gen_info_puppi = true;
    if(passed_rec_final_puppi || passed_rec_pt_topjet_sideband_puppi || passed_rec_mass_sideband_puppi) rec_info_puppi = true;


    if(rec_info_puppi){
      h_pseudodata_puppi->Fill(rec_binNumber_puppi, w_sig_rec);
      h_pseudodata_dist_puppi->Fill(tau32_rec_puppi, w_sig_rec);
    }
    if(gen_info_puppi)   h_pseudodata_truth_all_puppi->Fill(gen_binNumber_puppi, w_gen);
    if(passed_gen_final) h_pseudodata_truth_puppi->Fill(tau32_gen, w_gen);

    // fill histograms for PUPPI Jets w/ SoftDrop
    Int_t rec_binNumber_puppi_sd = 0, gen_binNumber_puppi_sd = 0;
    if(passed_rec_final_puppi_sd)                    rec_binNumber_puppi_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi_sd, mass_rec_puppi_sd);
    else if(passed_rec_pt_topjet_sideband_puppi_sd)  rec_binNumber_puppi_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);
    else if(passed_rec_mass_sideband_puppi_sd)       rec_binNumber_puppi_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);

    if(passed_gen_final_sd)                    gen_binNumber_puppi_sd = measurement_gen->GetGlobalBinNumber(tau32_gen_sd, mass_gen_sd);
    else if(passed_gen_pt_topjet_sideband_sd)  gen_binNumber_puppi_sd = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen_sd);
    else if(passed_gen_mass_sideband_sd)       gen_binNumber_puppi_sd = gen_mass_sideband->GetGlobalBinNumber(tau32_gen_sd);

    bool rec_info_puppi_sd = false, gen_info_puppi_sd = false;
    if(passed_gen_final_sd || passed_gen_pt_topjet_sideband_sd || passed_gen_mass_sideband_sd)                   gen_info_puppi_sd = true;
    if(passed_rec_final_puppi_sd || passed_rec_pt_topjet_sideband_puppi_sd || passed_rec_mass_sideband_puppi_sd) rec_info_puppi_sd = true;

    if(rec_info_puppi_sd){
      h_pseudodata_puppi_sd->Fill(rec_binNumber_puppi_sd, w_sig_rec);
      h_pseudodata_dist_puppi_sd->Fill(tau32_rec_puppi_sd, w_sig_rec);
    }
    if(gen_info_puppi_sd)   h_pseudodata_truth_all_puppi_sd->Fill(gen_binNumber_puppi_sd, w_gen);
    if(passed_gen_final_sd) h_pseudodata_truth_puppi_sd->Fill(tau32_gen_sd, w_gen);
  }

  h_pseudodata->Write();
  h_pseudodata_sd->Write();
  h_pseudodata_puppi->Write();
  h_pseudodata_puppi_sd->Write();
  h_pseudodata_dist->Write();
  h_pseudodata_dist_sd->Write();
  h_pseudodata_dist_puppi->Write();
  h_pseudodata_dist_puppi_sd->Write();
  h_pseudodata_truth->Write();
  h_pseudodata_truth_sd->Write();
  h_pseudodata_truth_puppi->Write();
  h_pseudodata_truth_puppi_sd->Write();
  h_pseudodata_truth_all->Write();
  h_pseudodata_truth_all_sd->Write();
  h_pseudodata_truth_all_puppi->Write();
  h_pseudodata_truth_all_puppi_sd->Write();

  delete h_pseudodata;
  delete h_pseudodata_sd;
  delete h_pseudodata_puppi;
  delete h_pseudodata_puppi_sd;
  delete h_pseudodata_dist;
  delete h_pseudodata_dist_sd;
  delete h_pseudodata_dist_puppi;
  delete h_pseudodata_dist_puppi_sd;
  delete h_pseudodata_truth;
  delete h_pseudodata_truth_sd;
  delete h_pseudodata_truth_puppi;
  delete h_pseudodata_truth_puppi_sd;
  delete h_pseudodata_truth_all;
  delete h_pseudodata_truth_all_sd;
  delete h_pseudodata_truth_all_puppi;
  delete h_pseudodata_truth_all_puppi_sd;
  cout << "Finished: Filling " << prefix << "!" << '\n';
  return;
}




/*
███████ ██ ██      ██          ██████   █████   ██████ ██   ██  ██████  ██████   ██████  ██    ██ ███    ██ ██████
██      ██ ██      ██          ██   ██ ██   ██ ██      ██  ██  ██       ██   ██ ██    ██ ██    ██ ████   ██ ██   ██
█████   ██ ██      ██          ██████  ███████ ██      █████   ██   ███ ██████  ██    ██ ██    ██ ██ ██  ██ ██   ██
██      ██ ██      ██          ██   ██ ██   ██ ██      ██  ██  ██    ██ ██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██
██      ██ ███████ ███████     ██████  ██   ██  ██████ ██   ██  ██████  ██   ██  ██████   ██████  ██   ████ ██████
*/



void fill_background(TTree* tree, TString prefix){
  if(!tree) cout << "could not read 'mc signal' tree\n";
  else      cout << "Filling Matrix Histograms for " + prefix + "...\n";

  // setup hists
  TH1* h_background               = binning_rec->CreateHistogram("Background_"+prefix);
  TH1* h_background_sd            = binning_rec->CreateHistogram("Background_sd_"+prefix);
  TH1* h_background_puppi         = binning_rec->CreateHistogram("Background_puppi_"+prefix);
  TH1* h_background_puppi_sd      = binning_rec->CreateHistogram("Background_puppi_sd_"+prefix);
  TH1* h_background_dist          = measurement_rec->CreateHistogram("Background_dist_"+prefix,kTRUE,0,0, "mass[C]");
  TH1* h_background_dist_sd       = measurement_rec->CreateHistogram("Background_dist_sd_"+prefix,kTRUE,0,0, "mass[C]");
  TH1* h_background_dist_puppi    = measurement_rec->CreateHistogram("Background_dist_puppi_"+prefix,kTRUE,0,0, "mass[C]");
  TH1* h_background_dist_puppi_sd = measurement_rec->CreateHistogram("Background_dist_puppi_sd_"+prefix,kTRUE,0,0, "mass[C]");

  if(!Muon_done) outputFile->cd();
  else outputFile2->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_rec_sd", &tau32_rec_sd);
  tree->SetBranchAddress("h_tau32_rec_puppi", &tau32_rec_puppi);
  tree->SetBranchAddress("h_tau32_rec_puppi_sd", &tau32_rec_puppi_sd);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_rec_sd", &mass_rec_sd);
  tree->SetBranchAddress("h_mass_rec_puppi", &mass_rec_puppi);
  tree->SetBranchAddress("h_mass_rec_puppi_sd", &mass_rec_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_final_sd", &passed_rec_final_sd);
  tree->SetBranchAddress("h_passed_rec_final_puppi", &passed_rec_final_puppi);
  tree->SetBranchAddress("h_passed_rec_final_puppi_sd", &passed_rec_final_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_sd", &passed_rec_pt_topjet_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi", &passed_rec_pt_topjet_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband_puppi_sd", &passed_rec_pt_topjet_sideband_puppi_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_sd", &passed_rec_mass_sideband_sd);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi", &passed_rec_mass_sideband_puppi);
  tree->SetBranchAddress("h_passed_rec_mass_sideband_puppi_sd", &passed_rec_mass_sideband_puppi_sd);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central = rec_weight;

    // fill background histograms for CHS Jets w/o SoftDrop
    Int_t rec_binNumber = 0;
    if(passed_rec_final)                   rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband) rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)      rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);

    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband){
      h_background->Fill(rec_binNumber, w_central);
      h_background_dist->Fill(tau32_rec);
    }

    // fill background histograms for CHS Jets w/ SoftDrop
    Int_t rec_binNumber_sd = 0;
    if(passed_rec_final_sd)                   rec_binNumber_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_sd, mass_rec_sd);
    else if(passed_rec_pt_topjet_sideband_sd) rec_binNumber_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_sd);
    else if(passed_rec_mass_sideband_sd)      rec_binNumber_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_sd);

    if(passed_rec_final_sd || passed_rec_pt_topjet_sideband_sd || passed_rec_mass_sideband_sd){
      h_background_sd->Fill(rec_binNumber_sd, w_central);
      h_background_dist_sd->Fill(tau32_rec_sd);
    }

    // fill background histograms for PUPPI Jets w/o SoftDrop
    Int_t rec_binNumber_puppi = 0;
    if(passed_rec_final_puppi)                   rec_binNumber_puppi = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi, mass_rec_puppi);
    else if(passed_rec_pt_topjet_sideband_puppi) rec_binNumber_puppi = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi);
    else if(passed_rec_mass_sideband_puppi)      rec_binNumber_puppi = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi);

    if(passed_rec_final_puppi || passed_rec_pt_topjet_sideband_puppi || passed_rec_mass_sideband_puppi){
      h_background_puppi->Fill(rec_binNumber_puppi, w_central);
      h_background_dist_puppi->Fill(tau32_rec_puppi);
    }

    // fill background histograms for PUPPI Jets w/ SoftDrop
    Int_t rec_binNumber_puppi_sd = 0;
    if(passed_rec_final_puppi_sd)                   rec_binNumber_puppi_sd = measurement_rec->GetGlobalBinNumber(tau32_rec_puppi_sd, mass_rec_puppi_sd);
    else if(passed_rec_pt_topjet_sideband_puppi_sd) rec_binNumber_puppi_sd = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);
    else if(passed_rec_mass_sideband_puppi_sd)      rec_binNumber_puppi_sd = rec_mass_sideband->GetGlobalBinNumber(tau32_rec_puppi_sd);

    if(passed_rec_final_puppi_sd || passed_rec_pt_topjet_sideband_puppi_sd || passed_rec_mass_sideband_puppi_sd){
      h_background_puppi_sd->Fill(rec_binNumber_puppi_sd, w_central);
      h_background_dist_puppi_sd->Fill(tau32_rec_puppi_sd);
    }

  }

  h_background->Write();
  h_background_sd->Write();
  h_background_puppi->Write();
  h_background_puppi_sd->Write();
  h_background_dist->Write();
  h_background_dist_sd->Write();
  h_background_dist_puppi->Write();
  h_background_dist_puppi_sd->Write();
  delete h_background;
  delete h_background_sd;
  delete h_background_puppi;
  delete h_background_puppi_sd;
  delete h_background_dist;
  delete h_background_dist_sd;
  delete h_background_dist_puppi;
  delete h_background_dist_puppi_sd;
  cout << "finished: filling background" << '\n';
  return;
}
