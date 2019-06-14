#include "hist_filler.h"

int main(int argc, char* argv[]){


  // switch on histogram errors
  TH1::SetDefaultSumw2();

  /*
  ██████ ██████  ███████  █████  ████████ ███████      ██████  ██    ██ ████████ ██████  ██    ██ ████████     ███████ ██ ██      ███████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██   ██ ██    ██    ██        ██      ██ ██      ██
  ██      ██████  █████   ███████    ██    █████       ██    ██ ██    ██    ██    ██████  ██    ██    ██        █████   ██ ██      █████
  ██      ██   ██ ██      ██   ██    ██    ██          ██    ██ ██    ██    ██    ██      ██    ██    ██        ██      ██ ██      ██
  ██████ ██   ██ ███████ ██   ██    ██    ███████      ██████   ██████     ██    ██       ██████     ██        ██      ██ ███████ ███████
  */

  std::string filename;
  filename = "Histograms.root";
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
  binning_rec->Write();
  binning_gen->Write();

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
  TFile *data_File = new TFile(dir + prefix + "DATA.DATA.root");
  fill_data((TTree *) data_File->Get("AnalysisTree"));

  // fill pseudodata
  TFile *pseudodata_matrix_File = new TFile(dir+prefix+"MC.TTbar.root");
  fill_pseudodata((TTree *) pseudodata_matrix_File->Get("AnalysisTree"));

  // fill ttbar
  TFile *mc_matrix_File = new TFile(dir+prefix+"MC.TTbar.root");
  fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"), "mc");

  // fill scales
  // for(unsigned int i = 0; i < sv.size(); i++){
  //   TFile *sv_matrix_file =  new TFile(dir+"_"+sv[i]+prefix+"MC.TTbar_2016v3.root");
  //   fill_ttbar((TTree *) sv_matrix_file->Get("AnalysisTree"), sv[i]);
  // }

  // fill variation
  // vector<TString> jetcorrections = {"JECup", "JECdown", "JERup", "JERdown"};
  vector<TString> jetcorrections = {"JECup", "JECdown"};
  for(unsigned int i = 0; i < jetcorrections.size(); i++){
    TFile *jc_matrix_file =  new TFile(dir+"/"+jetcorrections.at(i)+prefix+"MC.TTbar.root");
    fill_ttbar((TTree *) jc_matrix_file->Get("AnalysisTree"), jetcorrections.at(i));
  }

  // fill background
  // std::vector<TString> background = {"DYJets", "QCD", "ST", "WJets", "WW", "WZ", "ZZ"};
  std::vector<TString> background = {"DYJets", "ST"};

  for(unsigned int i = 0; i < background.size(); i++){
    TFile *background_File = new TFile(dir+prefix+"MC."+background[i]+".root");
    fill_background((TTree *) background_File->Get("AnalysisTree"), background[i]);
  }
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
  TH1* h_data_calc = binning_rec->CreateHistogram("Data_calc");

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_calc_tau32_rec", &tau32_calc_rec);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    Int_t binNumber = 0, binNumber_calc = 0;
    if(passed_rec_final){
      binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
      binNumber_calc = measurement_rec->GetGlobalBinNumber(tau32_calc_rec, mass_rec);
    }
    else if(passed_rec_pt_topjet_sideband){
      binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
      binNumber_calc = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }
    else if(passed_rec_mass_sideband){
      binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);
      binNumber_calc = rec_mass_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband){
      h_data->Fill(binNumber);
      h_data_calc->Fill(binNumber_calc);
    }
  }

  h_data->Write();
  h_data_calc->Write();
  delete h_data;
  delete h_data_calc;
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
  if(!tree) cout << "could not read 'data' tree\n";
  else      cout << "Filling Pseudodata Histograms...\n";

  // setup hists
  TH1* h_pseudodata_1                = binning_rec->CreateHistogram("Pseudodata_1");
  TH1* h_pseudodata_dist_1           = measurement_rec->CreateHistogram("Pseudodata_dist_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_1      = binning_gen->CreateHistogram("Pseudodata_truth_all_1");
  TH1* h_pseudodata_truth_1          = measurement_gen->CreateHistogram("Pseudodata_truth_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_1           = binning_rec->CreateHistogram("Pseudodata_calc_1");
  TH1* h_pseudodata_calc_dist_1      = measurement_rec->CreateHistogram("Pseudodata_calc_dist_1",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_truth_all_1 = binning_gen->CreateHistogram("Pseudodata_calc_truth_all_1");
  TH1* h_pseudodata_calc_truth_1     = measurement_gen->CreateHistogram("Pseudodata_calc_truth_1",kTRUE,0,0, "mass[C]");

  TH1* h_pseudodata_2                = binning_rec->CreateHistogram("Pseudodata_2");
  TH1* h_pseudodata_dist_2           = measurement_rec->CreateHistogram("Pseudodata_dist_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_2      = binning_gen->CreateHistogram("Pseudodata_truth_all_2");
  TH1* h_pseudodata_truth_2          = measurement_gen->CreateHistogram("Pseudodata_truth_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_2           = binning_rec->CreateHistogram("Pseudodata_calc_2");
  TH1* h_pseudodata_calc_dist_2      = measurement_rec->CreateHistogram("Pseudodata_calc_dist_2",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_truth_all_2 = binning_gen->CreateHistogram("Pseudodata_calc_truth_all_2");
  TH1* h_pseudodata_calc_truth_2     = measurement_gen->CreateHistogram("Pseudodata_calc_truth_2",kTRUE,0,0, "mass[C]");

  TH1* h_pseudodata_3                = binning_rec->CreateHistogram("Pseudodata_3");
  TH1* h_pseudodata_dist_3           = measurement_rec->CreateHistogram("Pseudodata_dist_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_truth_all_3      = binning_gen->CreateHistogram("Pseudodata_truth_all_3");
  TH1* h_pseudodata_truth_3          = measurement_gen->CreateHistogram("Pseudodata_truth_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_3           = binning_rec->CreateHistogram("Pseudodata_calc_3");
  TH1* h_pseudodata_calc_dist_3      = measurement_rec->CreateHistogram("Pseudodata_calc_dist_3",kTRUE,0,0, "mass[C]");
  TH1* h_pseudodata_calc_truth_all_3 = binning_gen->CreateHistogram("Pseudodata_calc_truth_all_3");
  TH1* h_pseudodata_calc_truth_3     = measurement_gen->CreateHistogram("Pseudodata_calc_truth_3",kTRUE,0,0, "mass[C]");
  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_gen", &tau32_gen);
  tree->SetBranchAddress("h_calc_tau32_rec", &tau32_calc_rec);
  tree->SetBranchAddress("h_calc_tau32_gen", &tau32_calc_gen);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_gen", &mass_gen);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband", &passed_gen_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_gen_mass_sideband", &passed_gen_mass_sideband);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchAddress("h_gen_weight", &gen_weight);
  tree->SetBranchStatus("*", 1);

  int counter = 1;
  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central = rec_weight*10;
    w_nogen = rec_weight*10;
    w_norec = gen_weight*10;
    w_correction = gen_weight*10 - rec_weight*10;

    // get weight for gen and rec hists
    w_sig_rec = rec_weight*10;
    w_gen = gen_weight*10;


    Int_t rec_binNumber = 0, gen_binNumber = 0;
    Int_t rec_binNumber_calc = 0, gen_binNumber_calc = 0;
    if(passed_rec_final)                    rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband)  rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
    else if(passed_rec_mass_sideband)       rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);
    if(passed_rec_final)                    rec_binNumber_calc = measurement_rec->GetGlobalBinNumber(tau32_calc_rec, mass_rec);
    else if(passed_rec_pt_topjet_sideband)  rec_binNumber_calc = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_rec);
    else if(passed_rec_mass_sideband)       rec_binNumber_calc = rec_mass_sideband->GetGlobalBinNumber(tau32_calc_rec);

    if(passed_gen_final)                    gen_binNumber = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);
    if(passed_gen_final)                    gen_binNumber_calc = measurement_gen->GetGlobalBinNumber(tau32_calc_gen, mass_gen);
    else if(passed_gen_pt_topjet_sideband)  gen_binNumber_calc = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_gen);
    else if(passed_gen_mass_sideband)       gen_binNumber_calc = gen_mass_sideband->GetGlobalBinNumber(tau32_calc_gen);

    bool gen_info = false;
    bool rec_info = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband) gen_info = true;
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband) rec_info = true;


    if(rec_info){
      if(counter == 8){
        h_pseudodata_1->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_1->Fill(tau32_rec, w_sig_rec);
        h_pseudodata_calc_1->Fill(rec_binNumber_calc, w_sig_rec);
        h_pseudodata_calc_dist_1->Fill(tau32_calc_rec, w_sig_rec);
      }
      else if(counter == 9){
        h_pseudodata_2->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_2->Fill(tau32_rec, w_sig_rec);
        h_pseudodata_calc_2->Fill(rec_binNumber_calc, w_sig_rec);
        h_pseudodata_calc_dist_2->Fill(tau32_calc_rec, w_sig_rec);
      }
      else if(counter == 10){
        h_pseudodata_3->Fill(rec_binNumber, w_sig_rec);
        h_pseudodata_dist_3->Fill(tau32_rec, w_sig_rec);
        h_pseudodata_calc_3->Fill(rec_binNumber_calc, w_sig_rec);
        h_pseudodata_calc_dist_3->Fill(tau32_calc_rec, w_sig_rec);
      }
    }
    if(gen_info){
      if(counter == 8){
        h_pseudodata_truth_all_1->Fill(gen_binNumber, w_gen);
        h_pseudodata_calc_truth_all_1->Fill(gen_binNumber_calc, w_gen);
      }
      if(counter == 9){
        h_pseudodata_truth_all_2->Fill(gen_binNumber, w_gen);
        h_pseudodata_calc_truth_all_2->Fill(gen_binNumber_calc, w_gen);
      }
      if(counter == 10){
        h_pseudodata_truth_all_3->Fill(gen_binNumber, w_gen);
        h_pseudodata_calc_truth_all_3->Fill(gen_binNumber_calc, w_gen);
      }
    }
    if(passed_gen_final){
      if(counter == 8){
        h_pseudodata_truth_1->Fill(tau32_gen, w_gen);
        h_pseudodata_calc_truth_1->Fill(tau32_calc_gen, w_gen);
      }
      else if(counter == 9){
        h_pseudodata_truth_2->Fill(tau32_gen, w_gen);
        h_pseudodata_calc_truth_2->Fill(tau32_calc_gen, w_gen);
      }
      else if(counter == 10){
        h_pseudodata_truth_3->Fill(tau32_gen, w_gen);
        h_pseudodata_calc_truth_3->Fill(tau32_calc_gen, w_gen);
      }
    }
    if(counter != 10) counter++;
    else counter = 1;
  }

  h_pseudodata_1->Write();
  h_pseudodata_dist_1->Write();
  h_pseudodata_truth_1->Write();
  h_pseudodata_truth_all_1->Write();
  h_pseudodata_calc_1->Write();
  h_pseudodata_calc_dist_1->Write();
  h_pseudodata_calc_truth_1->Write();
  h_pseudodata_calc_truth_all_1->Write();

  h_pseudodata_2->Write();
  h_pseudodata_dist_2->Write();
  h_pseudodata_truth_2->Write();
  h_pseudodata_truth_all_2->Write();
  h_pseudodata_calc_2->Write();
  h_pseudodata_calc_dist_2->Write();
  h_pseudodata_calc_truth_2->Write();
  h_pseudodata_calc_truth_all_2->Write();

  h_pseudodata_3->Write();
  h_pseudodata_dist_3->Write();
  h_pseudodata_truth_3->Write();
  h_pseudodata_truth_all_3->Write();
  h_pseudodata_calc_3->Write();
  h_pseudodata_calc_dist_3->Write();
  h_pseudodata_calc_truth_3->Write();
  h_pseudodata_calc_truth_all_3->Write();

  delete h_pseudodata_1;
  delete h_pseudodata_dist_1;
  delete h_pseudodata_truth_1;
  delete h_pseudodata_truth_all_1;
  delete h_pseudodata_calc_1;
  delete h_pseudodata_calc_dist_1;
  delete h_pseudodata_calc_truth_1;
  delete h_pseudodata_calc_truth_all_1;
  delete h_pseudodata_2;
  delete h_pseudodata_dist_2;
  delete h_pseudodata_truth_2;
  delete h_pseudodata_truth_all_2;
  delete h_pseudodata_calc_2;
  delete h_pseudodata_calc_dist_2;
  delete h_pseudodata_calc_truth_2;
  delete h_pseudodata_calc_truth_all_2;
  delete h_pseudodata_3;
  delete h_pseudodata_dist_3;
  delete h_pseudodata_truth_3;
  delete h_pseudodata_truth_all_3;
  delete h_pseudodata_calc_3;
  delete h_pseudodata_calc_dist_3;
  delete h_pseudodata_calc_truth_3;
  delete h_pseudodata_calc_truth_all_3;
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

  // setup hists
  TH1* h_rec                       = binning_rec->CreateHistogram(prefix+"_rec");
  TH1* h_rec_dist                  = measurement_rec->CreateHistogram(prefix+"_rec_dist",kTRUE,0,0, "mass[C]");
  TH1* h_gen                       = binning_gen->CreateHistogram(prefix+"_gen");
  TH1* h_truth_all                 = binning_gen->CreateHistogram(prefix+"_truth_all");
  TH1* h_truth                     = measurement_gen->CreateHistogram(prefix+"_truth",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all                = measurement_gen->CreateHistogram(prefix+"_purity_all",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin            = measurement_gen->CreateHistogram(prefix+"_purity_samebin",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass",kTRUE,0,0, "mass[C]");
  TH1* h_rec_calc                  = binning_rec->CreateHistogram(prefix+"_rec_calc");
  TH1* h_rec_dist_calc             = measurement_rec->CreateHistogram(prefix+"_rec_dist_calc",kTRUE,0,0, "mass[C]");
  TH1* h_gen_calc                  = binning_gen->CreateHistogram(prefix+"_gen_cal1");
  TH1* h_truth_all_calc            = binning_gen->CreateHistogram(prefix+"_truth_all_calc");
  TH1* h_truth_calc                = measurement_gen->CreateHistogram(prefix+"_truth_calc",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_calc           = measurement_gen->CreateHistogram(prefix+"_purity_all_calc",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_calc       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_calc",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_calc  = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_calc",kTRUE,0,0, "mass[C]");

  TH1* h_rec_1                       = binning_rec->CreateHistogram(prefix+"_rec_1");
  TH1* h_rec_dist_1                  = measurement_rec->CreateHistogram(prefix+"_rec_dist_1",kTRUE,0,0, "mass[C]");
  TH1* h_gen_1                       = binning_gen->CreateHistogram(prefix+"_gen_1");
  TH1* h_truth_all_1                 = binning_gen->CreateHistogram(prefix+"_truth_all_1");
  TH1* h_truth_1                     = measurement_gen->CreateHistogram(prefix+"_truth_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_1                = measurement_gen->CreateHistogram(prefix+"_purity_all_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_1            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_1       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_1",kTRUE,0,0, "mass[C]");
  TH1* h_rec_calc_1                  = binning_rec->CreateHistogram(prefix+"_rec_calc_1");
  TH1* h_rec_dist_calc_1             = measurement_rec->CreateHistogram(prefix+"_rec_dist_calc_1",kTRUE,0,0, "mass[C]");
  TH1* h_gen_calc_1                  = binning_gen->CreateHistogram(prefix+"_gen_calc_1");
  TH1* h_truth_all_calc_1            = binning_gen->CreateHistogram(prefix+"_truth_all_calc_1");
  TH1* h_truth_calc_1                = measurement_gen->CreateHistogram(prefix+"_truth_calc_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_calc_1           = measurement_gen->CreateHistogram(prefix+"_purity_all_calc_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_calc_1       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_calc_1",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_calc_1  = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_calc_1",kTRUE,0,0, "mass[C]");

  TH1* h_rec_2                       = binning_rec->CreateHistogram(prefix+"_rec_2");
  TH1* h_rec_dist_2                  = measurement_rec->CreateHistogram(prefix+"_rec_dist_2",kTRUE,0,0, "mass[C]");
  TH1* h_gen_2                       = binning_gen->CreateHistogram(prefix+"_gen_2");
  TH1* h_truth_all_2                 = binning_gen->CreateHistogram(prefix+"_truth_all_2");
  TH1* h_truth_2                     = measurement_gen->CreateHistogram(prefix+"_truth_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_2                = measurement_gen->CreateHistogram(prefix+"_purity_all_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_2            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_2       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_2",kTRUE,0,0, "mass[C]");
  TH1* h_rec_calc_2                  = binning_rec->CreateHistogram(prefix+"_rec_calc_2");
  TH1* h_rec_dist_calc_2             = measurement_rec->CreateHistogram(prefix+"_rec_dist_calc_2",kTRUE,0,0, "mass[C]");
  TH1* h_gen_calc_2                  = binning_gen->CreateHistogram(prefix+"_gen_calc_2");
  TH1* h_truth_all_calc_2            = binning_gen->CreateHistogram(prefix+"_truth_all_calc_2");
  TH1* h_truth_calc_2                = measurement_gen->CreateHistogram(prefix+"_truth_calc_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_calc_2           = measurement_gen->CreateHistogram(prefix+"_purity_all_calc_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_calc_2       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_calc_2",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_calc_2  = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_calc_2",kTRUE,0,0, "mass[C]");

  TH1* h_rec_3                       = binning_rec->CreateHistogram(prefix+"_rec_3");
  TH1* h_rec_dist_3                  = measurement_rec->CreateHistogram(prefix+"_rec_dist_3",kTRUE,0,0, "mass[C]");
  TH1* h_gen_3                       = binning_gen->CreateHistogram(prefix+"_gen_3");
  TH1* h_truth_all_3                 = binning_gen->CreateHistogram(prefix+"_truth_all_3");
  TH1* h_truth_3                     = measurement_gen->CreateHistogram(prefix+"_truth_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_3                = measurement_gen->CreateHistogram(prefix+"_purity_all_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_3            = measurement_gen->CreateHistogram(prefix+"_purity_samebin_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_3       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_3",kTRUE,0,0, "mass[C]");
  TH1* h_rec_calc_3                  = binning_rec->CreateHistogram(prefix+"_rec_calc_3");
  TH1* h_rec_dist_calc_3             = measurement_rec->CreateHistogram(prefix+"_rec_dist_calc_3",kTRUE,0,0, "mass[C]");
  TH1* h_gen_calc_3                  = binning_gen->CreateHistogram(prefix+"_gen_calc_3");
  TH1* h_truth_all_calc_3            = binning_gen->CreateHistogram(prefix+"_truth_all_calc_3");
  TH1* h_truth_calc_3                = measurement_gen->CreateHistogram(prefix+"_truth_calc_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_all_calc_3           = measurement_gen->CreateHistogram(prefix+"_purity_all_calc_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_calc_3       = measurement_gen->CreateHistogram(prefix+"_purity_samebin_calc_3",kTRUE,0,0, "mass[C]");
  TH1* h_purity_samebin_mass_calc_3  = measurement_gen->CreateHistogram(prefix+"_purity_samebin_mass_calc_3",kTRUE,0,0, "mass[C]");

  TH2* h_mc_matrix = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix");
  TH2* h_mc_matrix_1 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_1");
  TH2* h_mc_matrix_2 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_2");
  TH2* h_mc_matrix_3 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_3");
  TH2* h_mc_matrix_calc = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_calc");
  TH2* h_mc_matrix_calc_1 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_calc_1");
  TH2* h_mc_matrix_calc_2 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_calc_2");
  TH2* h_mc_matrix_calc_3 = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, prefix+"_matrix_calc_3");

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_tau32_gen", &tau32_gen);
  tree->SetBranchAddress("h_calc_tau32_rec", &tau32_calc_rec);
  tree->SetBranchAddress("h_calc_tau32_gen", &tau32_calc_gen);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_mass_gen", &mass_gen);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_gen_pt_topjet_sideband", &passed_gen_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_passed_gen_mass_sideband", &passed_gen_mass_sideband);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchAddress("h_gen_weight", &gen_weight);
  tree->SetBranchStatus("*", 1);

  int counter = 1;
  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central_scaled = rec_weight*10/9;
    w_nogen_scaled = rec_weight*10/9;
    w_norec_scaled = gen_weight*10/9;
    w_correction_scaled = gen_weight*10/9 - rec_weight*10/9;

    w_central = rec_weight;
    w_nogen = rec_weight;
    w_norec = gen_weight;
    w_correction = gen_weight - rec_weight;

    // get weight for gen and rec hists
    w_sig_rec_scaled = rec_weight*10/9;
    w_gen_scaled = gen_weight*10/9;
    // get weight for gen and rec hists
    w_sig_rec = rec_weight;
    w_gen = gen_weight;

    Int_t rec_binNumber = 0, gen_binNumber = 0;
    Int_t rec_binNumber_calc = 0, gen_binNumber_calc = 0;

    if(passed_rec_final){
      rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
      rec_binNumber_calc = measurement_rec->GetGlobalBinNumber(tau32_calc_rec, mass_rec);
    }
    else if(passed_rec_pt_topjet_sideband){
      rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
      rec_binNumber_calc = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }
    else if(passed_rec_mass_sideband){
      rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);
      rec_binNumber_calc = rec_mass_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }

    if(passed_gen_final){
      gen_binNumber = measurement_gen->GetGlobalBinNumber(tau32_gen, mass_gen);
      gen_binNumber_calc = measurement_gen->GetGlobalBinNumber(tau32_calc_gen, mass_gen);
    }
    else if(passed_gen_pt_topjet_sideband){
      gen_binNumber = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_gen);
      gen_binNumber_calc = gen_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_gen);
    }
    else if(passed_gen_mass_sideband){
      gen_binNumber = gen_mass_sideband->GetGlobalBinNumber(tau32_gen);
      gen_binNumber_calc = gen_mass_sideband->GetGlobalBinNumber(tau32_calc_gen);
    }

    bool gen_info = false;
    bool rec_info = false;
    if(passed_gen_final || passed_gen_pt_topjet_sideband || passed_gen_mass_sideband) gen_info = true;
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband) rec_info = true;

    if(rec_info){
      h_rec->Fill(rec_binNumber, w_sig_rec);
      h_rec_dist->Fill(tau32_rec, w_sig_rec);
      h_rec_calc->Fill(rec_binNumber_calc, w_sig_rec);
      h_rec_dist_calc->Fill(tau32_calc_rec, w_sig_rec);

      if(counter != 8){
        h_rec_1->Fill(rec_binNumber, w_sig_rec_scaled);
        h_rec_dist_1->Fill(tau32_rec, w_sig_rec_scaled);
        h_rec_calc_1->Fill(rec_binNumber_calc, w_sig_rec_scaled);
        h_rec_dist_calc_1->Fill(tau32_calc_rec, w_sig_rec_scaled);
      }
      if(counter != 9){
        h_rec_2->Fill(rec_binNumber, w_sig_rec_scaled);
        h_rec_dist_2->Fill(tau32_rec, w_sig_rec_scaled);
        h_rec_calc_2->Fill(rec_binNumber_calc, w_sig_rec_scaled);
        h_rec_dist_calc_2->Fill(tau32_rec, w_sig_rec_scaled);
      }
      if(counter != 10){
        h_rec_3->Fill(rec_binNumber, w_sig_rec_scaled);
        h_rec_dist_3->Fill(tau32_rec, w_sig_rec_scaled);
        h_rec_calc_3->Fill(rec_binNumber_calc, w_sig_rec_scaled);
        h_rec_dist_calc_3->Fill(tau32_calc_rec, w_sig_rec_scaled);
      }
    }
    if(passed_gen_final){
      h_truth->Fill(tau32_gen, w_gen);
      h_truth_calc->Fill(tau32_calc_gen, w_gen);
      if(counter != 8){
        h_truth_1->Fill(tau32_gen, w_gen_scaled);
        h_truth_calc_1->Fill(tau32_calc_gen, w_gen_scaled);
      }
      if(counter != 9){
        h_truth_2->Fill(tau32_gen, w_gen_scaled);
        h_truth_calc_2->Fill(tau32_calc_gen, w_gen_scaled);
      }
      if(counter != 10){
        h_truth_3->Fill(tau32_gen, w_gen_scaled);
        h_truth_calc_3->Fill(tau32_calc_gen, w_gen_scaled);
      }
    }
    if(gen_info){
      h_gen->Fill(gen_binNumber, w_gen);
      h_truth_all->Fill(gen_binNumber, w_gen);
      h_gen_calc->Fill(gen_binNumber_calc, w_gen);
      h_truth_all_calc->Fill(gen_binNumber_calc, w_gen);

      if(counter != 8){
        h_gen_1->Fill(gen_binNumber, w_gen_scaled);
        h_truth_all_1->Fill(gen_binNumber, w_gen_scaled);
        h_gen_calc_1->Fill(gen_binNumber_calc, w_gen_scaled);
        h_truth_all_calc_1->Fill(gen_binNumber_calc, w_gen_scaled);
      }
      if(counter != 9){
        h_gen_2->Fill(gen_binNumber, w_gen_scaled);
        h_truth_all_2->Fill(gen_binNumber, w_gen_scaled);
        h_gen_calc_2->Fill(gen_binNumber_calc, w_gen_scaled);
        h_truth_all_calc_2->Fill(gen_binNumber_calc, w_gen_scaled);
      }
      if(counter != 10){
        h_gen_3->Fill(gen_binNumber, w_gen_scaled);
        h_truth_all_3->Fill(gen_binNumber, w_gen_scaled);
        h_gen_calc_3->Fill(gen_binNumber_calc, w_gen_scaled);
        h_truth_all_calc_3->Fill(gen_binNumber_calc, w_gen_scaled);
      }
    }

    if( rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_central);
      h_mc_matrix_calc->Fill(gen_binNumber_calc, rec_binNumber_calc, w_central);
      if(counter != 8){
        h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
        h_mc_matrix_calc_1->Fill(gen_binNumber_calc, rec_binNumber_calc, w_central_scaled);
      }
      if(counter != 9){
        h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
        h_mc_matrix_calc_2->Fill(gen_binNumber_calc, rec_binNumber_calc, w_central);
      }
      if(counter != 10){
        h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_central_scaled);
        h_mc_matrix_calc_3->Fill(gen_binNumber_calc, rec_binNumber_calc, w_central_scaled);
      }
    }
    if(!rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_norec);
      h_mc_matrix_calc->Fill(gen_binNumber_calc, rec_binNumber_calc, w_norec);
      if(counter != 8){
        h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
        h_mc_matrix_calc_1->Fill(gen_binNumber_calc, rec_binNumber_calc, w_norec_scaled);
      }
      if(counter != 9){
        h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
        h_mc_matrix_calc_2->Fill(gen_binNumber_calc, rec_binNumber_calc, w_norec_scaled);
      }
      if(counter != 10){
        h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_norec_scaled);
        h_mc_matrix_calc_3->Fill(gen_binNumber_calc, rec_binNumber_calc, w_norec_scaled);
      }
    }
    if( rec_info && !gen_info){
      h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_nogen);
      h_mc_matrix_calc->Fill(gen_binNumber_calc, rec_binNumber_calc, w_nogen);
      if(counter != 8){
        h_mc_matrix_1->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
        h_mc_matrix_calc_1->Fill(gen_binNumber_calc, rec_binNumber_calc, w_nogen_scaled);
      }
      if(counter != 9){
        h_mc_matrix_2->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
        h_mc_matrix_calc_2->Fill(gen_binNumber_calc, rec_binNumber_calc, w_nogen_scaled);
      }
      if(counter != 10){
        h_mc_matrix_3->Fill(gen_binNumber, rec_binNumber, w_nogen_scaled);
        h_mc_matrix_calc_3->Fill(gen_binNumber_calc, rec_binNumber_calc, w_nogen_scaled);
      }
    }
    if( rec_info &&  gen_info){
      h_mc_matrix->Fill(gen_binNumber, 0., w_correction);
      h_mc_matrix_calc->Fill(gen_binNumber_calc, 0., w_correction);
      if(counter != 8){
        h_mc_matrix_1->Fill(gen_binNumber, 0., w_correction_scaled);
        h_mc_matrix_calc_1->Fill(gen_binNumber_calc, 0., w_correction_scaled);
      }
      if(counter != 9){
        h_mc_matrix_2->Fill(gen_binNumber, 0., w_correction_scaled);
        h_mc_matrix_calc_2->Fill(gen_binNumber_calc, 0., w_correction_scaled);
      }
      if(counter != 10){
        h_mc_matrix_3->Fill(gen_binNumber, 0., w_correction_scaled);
        h_mc_matrix_calc_3->Fill(gen_binNumber_calc, 0., w_correction_scaled);
      }
    }

    //fill hists for purity
    int genBin_recInfo = 0;
    int genBin_recInfo_mass = 0;
    int genBin_recInfo_calc = 0;
    int genBin_recInfo_mass_calc = 0;

    if(passed_gen_final && rec_info){
      genBin_recInfo      = measurement_gen->GetGlobalBinNumber(tau32_rec,mass_gen);
      genBin_recInfo_mass = measurement_gen->GetGlobalBinNumber(tau32_rec,mass_rec);
      genBin_recInfo_calc     = measurement_gen->GetGlobalBinNumber(tau32_calc_rec,mass_gen);
      genBin_recInfo_mass_calc = measurement_gen->GetGlobalBinNumber(tau32_calc_rec,mass_rec);

      h_purity_all->Fill(tau32_gen, w_gen);
      if(genBin_recInfo == gen_binNumber)      h_purity_samebin->Fill(tau32_gen, w_gen);
      if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass->Fill(tau32_gen, w_gen);

      h_purity_all_calc->Fill(tau32_calc_gen, w_gen);
      if(genBin_recInfo_calc == gen_binNumber_calc)      h_purity_samebin_calc->Fill(tau32_calc_gen, w_gen);
      if(genBin_recInfo_mass_calc == gen_binNumber_calc) h_purity_samebin_mass_calc->Fill(tau32_calc_gen, w_gen);


      if(counter != 8){
        h_purity_all_1->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo == gen_binNumber)      h_purity_samebin_1->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_1->Fill(tau32_gen, w_gen_scaled);

        h_purity_all_calc_1->Fill(tau32_calc_gen, w_gen_scaled);
        if(genBin_recInfo_calc == gen_binNumber_calc)      h_purity_samebin_calc_1->Fill(tau32_calc_gen, w_gen_scaled);
        if(genBin_recInfo_mass_calc == gen_binNumber_calc) h_purity_samebin_mass_calc_1->Fill(tau32_calc_gen, w_gen_scaled);
      }
      if(counter != 9){
        h_purity_all_2->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo == gen_binNumber)      h_purity_samebin_2->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_2->Fill(tau32_gen, w_gen_scaled);

        h_purity_all_calc_2->Fill(tau32_calc_gen, w_gen_scaled);
        if(genBin_recInfo_calc == gen_binNumber_calc)      h_purity_samebin_calc_2->Fill(tau32_calc_gen, w_gen_scaled);
        if(genBin_recInfo_mass_calc == gen_binNumber_calc) h_purity_samebin_mass_calc_2->Fill(tau32_calc_gen, w_gen_scaled);
      }
      if(counter != 10){
        h_purity_all_3->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo == gen_binNumber)      h_purity_samebin_3->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo_mass == gen_binNumber) h_purity_samebin_mass_3->Fill(tau32_gen, w_gen_scaled);

        h_purity_all_calc_3->Fill(tau32_gen, w_gen_scaled);
        if(genBin_recInfo_calc == gen_binNumber_calc)      h_purity_samebin_calc_3->Fill(tau32_calc_gen, w_gen_scaled);
        if(genBin_recInfo_mass_calc == gen_binNumber_calc) h_purity_samebin_mass_calc_3->Fill(tau32_calc_gen, w_gen_scaled);
      }
    }
    counter++;
    if(counter > 10) counter = 1;
  }

  h_rec->Write();
  h_rec_dist->Write();
  h_gen->Write();
  h_truth_all->Write();
  h_truth->Write();
  h_purity_all->Write();
  h_purity_samebin->Write();
  h_purity_samebin_mass->Write();
  h_rec_calc->Write();
  h_rec_dist_calc->Write();
  h_gen_calc->Write();
  h_truth_all_calc->Write();
  h_truth_calc->Write();
  h_purity_all_calc->Write();
  h_purity_samebin_calc->Write();
  h_purity_samebin_mass_calc->Write();

  h_rec_1->Write();
  h_rec_dist_1->Write();
  h_gen_1->Write();
  h_truth_1->Write();
  h_mc_matrix_1->Write();
  h_purity_all_1->Write();
  h_purity_samebin_1->Write();
  h_truth_all_1->Write();
  h_rec_calc_1->Write();
  h_rec_dist_calc_1->Write();
  h_gen_calc_1->Write();
  h_truth_calc_1->Write();
  h_mc_matrix_calc_1->Write();
  h_purity_all_calc_1->Write();
  h_purity_samebin_1->Write();
  h_truth_all_calc_1->Write();

  h_rec_2->Write();
  h_rec_dist_2->Write();
  h_gen_2->Write();
  h_truth_2->Write();
  h_mc_matrix_2->Write();
  h_purity_all_2->Write();
  h_purity_samebin_2->Write();
  h_truth_all_2->Write();
  h_rec_calc_2->Write();
  h_rec_dist_calc_2->Write();
  h_gen_calc_2->Write();
  h_truth_calc_2->Write();
  h_mc_matrix_calc_2->Write();
  h_purity_all_calc_2->Write();
  h_purity_samebin_calc_2->Write();
  h_truth_all_calc_2->Write();

  h_rec_3->Write();
  h_rec_dist_3->Write();
  h_gen_3->Write();
  h_truth_3->Write();
  h_mc_matrix_3->Write();
  h_purity_all_3->Write();
  h_purity_samebin_3->Write();
  h_truth_all_3->Write();
  h_rec_calc_3->Write();
  h_rec_dist_calc_3->Write();
  h_gen_calc_3->Write();
  h_truth_calc_3->Write();
  h_mc_matrix_calc_3->Write();
  h_purity_all_calc_3->Write();
  h_purity_samebin_calc_3->Write();
  h_truth_all_calc_3->Write();

  delete h_rec;
  delete h_rec_dist;
  delete h_gen;
  delete h_truth_all;
  delete h_truth;
  delete h_purity_all;
  delete h_purity_samebin;
  delete h_purity_samebin_mass;
  delete h_rec_calc;
  delete h_rec_dist_calc;
  delete h_gen_calc;
  delete h_truth_all_calc;
  delete h_truth_calc;
  delete h_purity_all_calc;
  delete h_purity_samebin_calc;
  delete h_purity_samebin_mass_calc;
  delete h_rec_1;
  delete h_rec_dist_1;
  delete h_gen_1;
  delete h_truth_1;
  delete h_mc_matrix_1;
  delete h_truth_all_1;
  delete h_rec_2;
  delete h_rec_dist_2;
  delete h_gen_2;
  delete h_truth_2;
  delete h_mc_matrix_2;
  delete h_truth_all_2;
  delete h_rec_3;
  delete h_rec_dist_3;
  delete h_gen_3;
  delete h_truth_3;
  delete h_mc_matrix_3;
  delete h_truth_all_3;
  cout << "Finished: Filling TTbar" << '\n';
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
  TH1* h_background_rec = binning_rec->CreateHistogram("background_rec_"+prefix);
  TH1* h_background_calc_rec = binning_rec->CreateHistogram("background_calc_rec_"+prefix);

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_tau32_rec", &tau32_rec);
  tree->SetBranchAddress("h_calc_tau32_rec", &tau32_calc_rec);
  tree->SetBranchAddress("h_mass_rec", &mass_rec);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_rec_pt_topjet_sideband", &passed_rec_pt_topjet_sideband);
  tree->SetBranchAddress("h_passed_rec_mass_sideband", &passed_rec_mass_sideband);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);
  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central = rec_weight;

    Int_t rec_binNumber = 0;
    Int_t rec_binNumber_calc = 0;

    if(passed_rec_final){
      rec_binNumber = measurement_rec->GetGlobalBinNumber(tau32_rec, mass_rec);
      rec_binNumber_calc = measurement_rec->GetGlobalBinNumber(tau32_calc_rec, mass_rec);
    }
    else if(passed_rec_pt_topjet_sideband){
      rec_binNumber = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_rec);
      rec_binNumber_calc = rec_pt_topjet_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }
    else if(passed_rec_mass_sideband){
      rec_binNumber = rec_mass_sideband->GetGlobalBinNumber(tau32_rec);
      rec_binNumber_calc = rec_mass_sideband->GetGlobalBinNumber(tau32_calc_rec);
    }
    if(passed_rec_final || passed_rec_pt_topjet_sideband || passed_rec_mass_sideband){
      h_background_rec->Fill(rec_binNumber, w_central);
      h_background_calc_rec->Fill(rec_binNumber_calc, w_central);
    }
  }

  h_background_rec->Write();
  h_background_calc_rec->Write();
  delete h_background_rec;
  delete h_background_calc_rec;
  cout << "finished: filling background" << '\n';
  return;
}
