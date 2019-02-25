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

  Int_t error=parser.ParseFile(binning_xml);
  if(error) cout<<"error="<<error<<" from TDOMParser\n";
  TXMLDocument const *XMLdocument=parser.GetXMLDocument();
  binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument,"binning_rec");
  binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument,"binning_gen");


  if(!binning_rec) cout<<"could not read 'rec' binning\n";
  if(!binning_gen) cout<<"could not read 'gen' binning\n";


  // get distributions from measurement phase space and sideband regions
  measurement_rec = binning_rec->FindNode("measurement_rec");
  measurement_gen = binning_gen->FindNode("measurement_gen");

  ptmigration_rec = binning_rec->FindNode("ptmigration_rec");
  ptmigration_gen = binning_gen->FindNode("ptmigration_gen");

  massmigration_rec = binning_rec->FindNode("massmigration_rec");
  massmigration_gen = binning_gen->FindNode("massmigration_gen");

  btagmigration_rec = binning_rec->FindNode("btagmigration_rec");

  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  // define directory
  TString dir = "/nfs/dust/cms/user/schwarzd/MTopJet/PostSelection/muon/";
  TString prefix = "/uhh2.AnalysisModuleRunner.";

  // fill data
  TFile *data_File=new TFile(dir+prefix+"DATA.DATA.root");
  fill_data((TTree *) data_File->Get("AnalysisTree"));

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
  tree->SetBranchAddress("Mass_Rec",&massRec);
  tree->SetBranchAddress("Pt_Rec",&ptRec);
  tree->SetBranchAddress("passed_measurement_rec",&passed_measurement_rec);
  tree->SetBranchAddress("passed_ptmigration_rec",&passed_ptmigration_rec);
  tree->SetBranchAddress("passed_massmigration_rec",&passed_massmigration_rec);
  tree->SetBranchAddress("passed_btagmigration_rec",&passed_btagmigration_rec);

  tree->SetBranchStatus("*",1);

  for(Int_t ievent=0; ievent < tree->GetEntriesFast(); ievent++) {
    if(tree->GetEntry(ievent)<=0) break;

    Int_t binNumber = 0;
    if     (passed_measurement_rec)    binNumber = measurement_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_ptmigration_rec)    binNumber = ptmigration_rec->GetGlobalBinNumber(massRec,ptRec);
    else if(passed_massmigration_rec)  binNumber = massmigration_rec->GetGlobalBinNumber(massRec);
    else if(passed_btagmigration_rec)  binNumber = btagmigration_rec->GetGlobalBinNumber(massRec);


    if(passed_measurement_rec || passed_ptmigration_rec || passed_massmigration_rec || passed_btagmigration_rec){
      h_data->Fill(binNumber);
    }
  }

  h_data->Write();
  delete h_data;
  return;
}
