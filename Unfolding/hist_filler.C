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

  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  // define directory
  TString dir = "/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_test/";
  TString prefix = "/uhh2.AnalysisModuleRunner.";

  // fill data
  TFile *data_File = new TFile(dir + prefix + "DATA.DATA.root");
  fill_data((TTree *) data_File->Get("AnalysisTree"));

  // fill ttbar
  TFile *mc_matrix_File = new TFile(dir+prefix+"MC.TTbar.root");
  fill_ttbar((TTree *) mc_matrix_File->Get("AnalysisTree"));

  // fill background
  std::vector<TString> background = {"DYJets", "QCD", "ST", "WJets", "WW", "WZ", "ZZ"};

  for(int i = 0; i < background.size(); i++){
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

  TH1* h_data = binning_rec->CreateHistogram("data");

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_pt_rec", &pt_rec);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);

  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    Int_t binNumber = 0;
    if(passed_rec_final) binNumber = measurement_rec->GetGlobalBinNumber(pt_rec);
    if(passed_rec_final) h_data->Fill(binNumber);
  }

  h_data->Write();
  delete h_data;
  cout << "finished: filling data" << '\n';
  return;
}



/*
███████ ██ ██      ██          ████████ ████████ ██████   █████  ██████
██      ██ ██      ██             ██       ██    ██   ██ ██   ██ ██   ██
█████   ██ ██      ██             ██       ██    ██████  ███████ ██████
██      ██ ██      ██             ██       ██    ██   ██ ██   ██ ██   ██
██      ██ ███████ ███████        ██       ██    ██████  ██   ██ ██   ██
*/

void fill_ttbar(TTree* tree){
  if(!tree) cout << "could not read 'mc signal' tree\n";
  else      cout << "Filling Matrix Histograms for TTbar ...\n";

  // setup hists
  TH1* h_ttbar_rec = binning_rec->CreateHistogram("TTbar_rec");
  TH1* h_ttbar_gen   = binning_gen->CreateHistogram("TTbar_gen");
  TH1* h_ttbar_truth   = binning_gen->CreateHistogram("TTbar_truth", kTRUE, 0, 0);

  TH2* h_mc_matrix = TUnfoldBinning::CreateHistogramOfMigrations(binning_gen, binning_rec, "ttbar_matrix");

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_pt_rec", &pt_rec);
  tree->SetBranchAddress("h_pt_gen", &pt_gen);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_passed_gen_final", &passed_gen_final);
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

    Int_t rec_binNumber = 0, gen_binNumber = 0;
    if(passed_rec_final) rec_binNumber = measurement_rec->GetGlobalBinNumber(pt_rec);
    if(passed_gen_final) gen_binNumber = measurement_gen->GetGlobalBinNumber(pt_gen);

    if(passed_rec_final){
      h_ttbar_rec->Fill(rec_binNumber, w_sig_rec);
    }
    if(passed_gen_final){
      h_ttbar_gen->Fill(gen_binNumber, w_gen);
      h_ttbar_truth->Fill(pt_gen, w_gen);
    }
    if( passed_rec_final &&  passed_gen_final) h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_central);
    if(!passed_rec_final &&  passed_gen_final) h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_norec);
    if( passed_rec_final && !passed_gen_final) h_mc_matrix->Fill(gen_binNumber, rec_binNumber, w_nogen);
    if( passed_rec_final &&  passed_gen_final) h_mc_matrix->Fill(gen_binNumber, 0., w_correction);
  }

  h_ttbar_rec->Write();
  h_ttbar_gen->Write();
  h_ttbar_truth->Write();
  h_mc_matrix->Write();
  delete h_ttbar_rec;
  delete h_ttbar_gen;
  delete h_ttbar_truth;
  delete h_mc_matrix;
  cout << "finished: filling ttbar" << '\n';
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

  outputFile->cd();

  tree->ResetBranchAddresses();
  tree->SetBranchAddress("h_pt_rec", &pt_rec);
  tree->SetBranchAddress("h_passed_rec_final", &passed_rec_final);
  tree->SetBranchAddress("h_rec_weight", &rec_weight);

  tree->SetBranchStatus("*", 1);

  for(Int_t ievent = 0; ievent < tree->GetEntriesFast(); ievent++){
    if(tree->GetEntry(ievent) <= 0) break;

    // get weights for migration matrix
    w_central = rec_weight;

    Int_t rec_binNumber = 0;
    if(passed_rec_final) rec_binNumber = measurement_rec->GetGlobalBinNumber(pt_rec);

    if(passed_rec_final) h_background_rec->Fill(rec_binNumber, w_central);
  }

  h_background_rec->Write();
  delete h_background_rec;
  cout << "finished: filling background" << '\n';
  return;
}
