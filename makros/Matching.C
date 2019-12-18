#include <TH1D.h>
#include <string>
using namespace std;

void Matching(){

  TFile *ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/GenStudies/uhh2.AnalysisModuleRunner.MC.TTbar.root", "READ");

  TString file = "gen_pt_topjet";
  TH1D* number_all = new TH1D("number_all", "", 41, 200, 1020);
  TH1D* number_matched = new TH1D("number_matched", "", 41, 200, 1020);

  for(int i=1; i<42;i++){
    TH1D *tt;
    TH1D *tt_m;
    if(i<11){
      tt = (TH1D*) ttbar->Get(file+to_string(i)+"/sum_event_weights");
      tt_m = (TH1D*) ttbar->Get(file+to_string(i)+"_matched/sum_event_weights");
      number_all->SetBinContent(i, tt->Integral());
      number_all->SetBinError(i, tt->GetBinError(1));
      number_matched->SetBinContent(i, tt_m->Integral());
      number_matched->SetBinError(i, tt_m->GetBinError(1));
    }
    else if(i==11){
      tt = (TH1D*) ttbar->Get("gen_mass/sum_event_weights");
      tt_m = (TH1D*) ttbar->Get("gen_mass_matched/sum_event_weights");
      number_all->SetBinContent(i, tt->Integral());
      number_all->SetBinError(i, tt->GetBinError(1));
      number_matched->SetBinContent(i, tt_m->Integral());
      number_matched->SetBinError(i, tt_m->GetBinError(1));
      cout << "matching: " << number_matched->GetBinContent(i)/number_all->GetBinContent(i) << '\n';

    }
    else{
      int j=i-1;
      tt = (TH1D*) ttbar->Get(file+to_string(j)+"/sum_event_weights");
      tt_m = (TH1D*) ttbar->Get(file+to_string(j)+"_matched/sum_event_weights");
      number_all->SetBinContent(i, tt->Integral());
      number_all->SetBinError(i, tt->GetBinError(1));
      number_matched->SetBinContent(i, tt_m->Integral());
      number_matched->SetBinError(i, tt_m->GetBinError(1));
    }
  }

  TGraphAsymmErrors*  matching = new TGraphAsymmErrors(number_matched,number_all,"cl=0.683 b(1,1) mode");

  TCanvas* c1 = new TCanvas("1","1", 1000,1000);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadTopMargin(0.02);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.11);
  gStyle->SetPadRightMargin(0.015);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  // gStyle->SetEndErrorSize(3);
  c1->cd();
  c1->UseCurrentStyle();
  matching->SetTitle("");
  matching->GetYaxis()->SetLabelSize(0.04);
  matching->GetYaxis()->SetTitleSize(0.045);
  matching->GetXaxis()->SetLabelSize(0.04);
  matching->GetYaxis()->SetTitleOffset(1.17);
  matching->GetXaxis()->SetTitleSize(0.045);
  matching->GetXaxis()->SetTitleOffset(1.15);
  // matching->SetLineWidth(2);
  matching->GetXaxis()->SetTitle("p_{T, 1} threshold");
  matching->GetYaxis()->SetTitle("fully merged fraction");
  matching->GetYaxis()->SetRangeUser(0.0001, 1);
  matching->GetXaxis()->SetRangeUser(200, 1020);
  matching->SetLineColor(kRed);
  matching->SetLineWidth(3);
  matching->Draw("ap");
  c1->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/pt_gen_matching.pdf");

for(unsigned int i=1; i < matching->GetN(); i++){
  cout << "at: " <<200+20*i << " this value: "<< matching->Eval(200+20*i) << '\n';
}


  TCanvas* c2 = new TCanvas("2","2", 1000,1000);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadTopMargin(0.02);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.019);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  c2->cd();
  c2->UseCurrentStyle();
  number_all->SetTitle("");
  number_all->GetYaxis()->SetLabelSize(0.04);
  number_all->GetYaxis()->SetTitleSize(0.045);
  number_all->GetXaxis()->SetLabelSize(0.04);
  number_all->GetYaxis()->SetTitleOffset(1.85);
  number_all->GetXaxis()->SetTitleSize(0.045);
  number_all->GetXaxis()->SetTitleOffset(1.15);
  // number_all->SetLineWidth(2);
  number_all->GetXaxis()->SetTitle("p_{T, 1} threshold");
  number_all->GetYaxis()->SetTitle("Events");
  number_all->GetYaxis()->SetRangeUser(0.0001, 13000);
  number_all->SetLineColor(kRed);
  number_all->SetMarkerColor(kRed);
  number_all->SetLineWidth(3);
  // number->GetYaxis()->SetRangeUser(0, 100);
  number_all->Draw("e1");
  c2->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/pt_gen_events.pdf");
}
