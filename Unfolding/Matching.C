#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>

using namespace std;

void Matching(){
  gStyle->SetOptStat(0);
  // gStyle->SetPadLeftMargin(0.14);
  // gStyle->SetPadRightMargin(0.1);
  // gStyle->SetErrorX(0);
  //Load data
  TFile *tt = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root", "READ");

  TH1D* test = (TH1D*)tt->Get("gen_mass/tau32_gtj1");
  TH1D* matched = (TH1D*)tt->Get("gen_mass_matched/tau32_gtj1");
  TH1D* unmatched = (TH1D*)tt->Get("gen_mass_unmatched/tau32_gtj1");

  TCanvas *matching_fraction = new TCanvas("matching_fraction", "", 800, 600);
  matching_fraction->cd();
  gPad->SetTicks();

  test->SetLineColor(kGray+1);
  test->SetFillColor(kGray);
  test->GetXaxis()->SetTitle("#tau_{3/2}");
  test->GetXaxis()->SetTitleSize(0.06);
  test->GetXaxis()->SetTitleOffset(0.8);
  test->GetYaxis()->SetTitleSize(0.06);
  test->GetYaxis()->SetTitleOffset(0.9);
  test->GetYaxis()->SetTitle("Events");
  test->GetYaxis()->SetRangeUser(0, 260);
  test->SetTitle("");
  test->DrawCopy("hist e0");
  matched->SetLineColor(kGreen+2);
  matched->DrawCopy("hist same e0");
  unmatched->SetLineColor(kRed);
  unmatched->DrawCopy("hist same e0");

  // text4->Draw();
  TLegend *legend_fraction = new TLegend(0.11, 0.7, 0.41, 0.9, "");
  legend_fraction->SetBorderSize(0);
  legend_fraction->SetFillStyle(0);
  legend_fraction->AddEntry(test, "TTbar total", "l");
  legend_fraction->AddEntry(matched, "TTbar matched", "l");
  legend_fraction->AddEntry(unmatched, "TTbar unmatched", "l");
  legend_fraction->Draw();
  matching_fraction->SaveAs("/afs/desy.de/user/s/skottkej/Plots/Unfolding/matching_fraction.eps");
}
