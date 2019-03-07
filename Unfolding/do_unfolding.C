#include "do_unfolding.h"

using namespace std;
int counter = 1;
int main(int argc, char* argv[]){
  // switch on histogram errors
  TH1::SetDefaultSumw2();
  //
  //
  // binning_xml = "Binning.xml";
  //
  // Int_t error = parser.ParseFile(binning_xml);
  // if(error) cout << "error = " << error << " from TDOMParser\n";
  // TXMLDocument const *XMLdocument = parser.GetXMLDocument();
  // binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_rec");
  // binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_gen");
  // measurement_rec = binning_rec->FindNode("measurement_rec");
  // measurement_gen = binning_gen->FindNode("measurement_gen");

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
  TString dir = "/nfs/dust/cms/user/skottkej/CMSSW_8_0_24_patch1/src/UHH2/TopSubstructure/Unfolding/";


  TFile *data_File = new TFile(dir + "Histograms.root");

  // fill background hists -- add further or remove unnecassary backgrounds in background_names
  background_names = {"DYJets", "QCD", "ST", "WJets", "WW", "WZ", "ZZ"};

  // Setup everything for unfolding!
  data_File->GetObject("ttbar_matrix", mat_response);   // fill response matrix
  data_File->GetObject("data", h_data);                 // fill histogram with data which should be unfolded
  data_File->GetObject("TTbar_gen", h_unfold);          // fill histogram with what data should be unfolded
  data_File->GetObject("TTbar_truth", h_truth);         // fill histogram with truth
  data_File->GetObject("TTbar_rec", h_mc);              // fill histogram to check if ratio between data and mc is appropiate (should be around .75)
  for(unsigned int i = 0; i < background_names.size(); i++){
    background.push_back((TH1D*) data_File->Get("background_rec_" + background_names.at(i)));
  }
  subtract_background = true;                           // should the background be subtracted?

  unfolding unfold(h_data, h_unfold, mat_response, h_truth, h_mc, binning_gen, binning_rec, background, background_names, subtract_background);
  TH1* unfolded_data = unfold.get_output_Lcurve();
  TH2* correlation_matrix = unfold.get_correlation_Lcurve();
  TH1* unfolded_data_tau = unfold.get_output();
  TH2* correlation_matrix_tau = unfold.get_correlation();
  std::vector<TH1D*> projections = unfold.check_projection();

  TCanvas* c_unfolding = new TCanvas("Unfolding ", "Unfolded data with truth value", 300, 600);
  c_unfolding->Divide(2,3);
  c_unfolding->cd(1);
  unfolded_data->GetYaxis()->SetRangeUser(-2000, 2000);
  unfolded_data->SetLineColor(kRed);
  unfolded_data->Draw();
  h_truth->DrawCopy("same hist");
  TLegend *legend1 = new TLegend(0.25, 0.7, 0.85, 0.9, "");
  legend1->SetBorderSize(0);
  legend1->SetFillStyle(0);
  legend1->AddEntry(unfolded_data, "unfolded data","l");
  legend1->AddEntry(h_truth,"data truth","l");
  legend1->Draw();

  c_unfolding->cd(2);
  correlation_matrix->Draw("colz");

  c_unfolding->cd(3);
  projections.at(0)->Draw();
  h_unfold->SetLineColor(kRed);
  h_unfold->DrawCopy("same");

  c_unfolding->cd(4);
  projections.at(1)->Draw();
  h_mc->SetLineColor(kRed);
  h_mc->DrawCopy("same");

  c_unfolding->cd(5);
  unfolded_data_tau->GetYaxis()->SetRangeUser(-2000, 2000);
  unfolded_data_tau->SetLineColor(kRed);
  unfolded_data_tau->Draw();
  h_truth->DrawCopy("same hist");
  TLegend *legend2 = new TLegend(0.25, 0.7, 0.85, 0.9, "");
  legend2->SetBorderSize(0);
  legend2->SetFillStyle(0);
  legend2->AddEntry(unfolded_data_tau, "unfolded data","l");
  legend2->AddEntry(h_truth,"data truth","l");
  legend2->Draw();

  c_unfolding->cd(6);
  correlation_matrix_tau->Draw("colz");

  outputFile->cd();
  c_unfolding->Write();
  outputFile->Close();

  return 0;
}
