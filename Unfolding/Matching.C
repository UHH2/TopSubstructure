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
  TFile *tt = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/uhh2.AnalysisModuleRunner.MC.TTbar.root", "READ");

  TH1D* test = (TH1D*)tt->Get("gen_mass_lower/tau32_gtj1");
  TH1D* matched = (TH1D*)tt->Get("gen_mass_lower_matched/tau32_gtj1");
  TH1D* unmatched = (TH1D*)tt->Get("gen_mass_lower_unmatched/tau32_gtj1");

  TH1D* test_2 = (TH1D*)tt->Get("gen_mass_higher/tau32_gtj1");
  TH1D* matched_2 = (TH1D*)tt->Get("gen_mass_higher_matched/tau32_gtj1");
  TH1D* unmatched_2 = (TH1D*)tt->Get("gen_mass_higher_unmatched/tau32_gtj1");

  TH1D* test_3 = (TH1D*)tt->Get("gen_mass/tau32_gtj1");
  TH1D* matched_3 = (TH1D*)tt->Get("gen_mass_matched/tau32_gtj1");
  TH1D* unmatched_3 = (TH1D*)tt->Get("gen_mass_unmatched/tau32_gtj1");

  // M_gtj1_rebin
  // TH1D* test = (TH1D*)tt->Get("gen_mass/tau32_gtj1");
  // TH1D* matched = (TH1D*)tt->Get("gen_mass_matched/tau32_gtj1");
  // TH1D* unmatched = (TH1D*)tt->Get("gen_mass_unmatched/tau32_gtj1");

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
  // test->GetYaxis()->SetRangeUser(0, 260);
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
  matching_fraction->SaveAs("/afs/desy.de/user/s/skottkej/Plots/Unfolding/matching_fraction_0_155.eps");

  TCanvas *matching_fraction_2 = new TCanvas("matching_fraction_2", "", 800, 600);
  matching_fraction_2->cd();
  gPad->SetTicks();

  test_2->SetLineColor(kGray+1);
  test_2->SetFillColor(kGray);
  test_2->GetXaxis()->SetTitle("#tau_{3/2}");
  test_2->GetXaxis()->SetTitleSize(0.06);
  test_2->GetXaxis()->SetTitleOffset(0.8);
  test_2->GetYaxis()->SetTitleSize(0.06);
  test_2->GetYaxis()->SetTitleOffset(0.9);
  test_2->GetYaxis()->SetTitle("Events");
  // test->GetYaxis()->SetRangeUser(0, 260);
  test_2->SetTitle("");
  test_2->DrawCopy("hist e0");
  matched_2->SetLineColor(kGreen+2);
  matched_2->DrawCopy("hist same e0");
  unmatched_2->SetLineColor(kRed);
  unmatched_2->DrawCopy("hist same e0");

  // text4->Draw();
  TLegend *legend_fraction_2 = new TLegend(0.11, 0.7, 0.41, 0.9, "");
  legend_fraction_2->SetBorderSize(0);
  legend_fraction_2->SetFillStyle(0);
  legend_fraction_2->AddEntry(test_2, "TTbar total", "l");
  legend_fraction_2->AddEntry(matched_2, "TTbar matched", "l");
  legend_fraction_2->AddEntry(unmatched_2, "TTbar unmatched", "l");
  legend_fraction_2->Draw();
  matching_fraction_2->SaveAs("/afs/desy.de/user/s/skottkej/Plots/Unfolding/matching_fraction_155_Inft.eps");

  TCanvas *matching_fraction_3 = new TCanvas("matching_fraction_3", "", 800, 600);
  matching_fraction_3->cd();
  gPad->SetTicks();

  test_3->SetLineColor(kGray+1);
  test_3->SetFillColor(kGray);
  test_3->GetXaxis()->SetTitle("#tau_{3/2}");
  test_3->GetXaxis()->SetTitleSize(0.06);
  test_3->GetXaxis()->SetTitleOffset(0.8);
  test_3->GetYaxis()->SetTitleSize(0.06);
  test_3->GetYaxis()->SetTitleOffset(0.9);
  test_3->GetYaxis()->SetTitle("Events");
  // test->GetYaxis()->SetRangeUser(0, 260);
  test_3->SetTitle("");
  test_3->DrawCopy("hist e0");
  matched_3->SetLineColor(kGreen+2);
  matched_3->DrawCopy("hist same e0");
  unmatched_3->SetLineColor(kRed);
  unmatched_3->DrawCopy("hist same e0");

  // text4->Draw();
  TLegend *legend_fraction_3 = new TLegend(0.11, 0.7, 0.41, 0.9, "");
  legend_fraction_3->SetBorderSize(0);
  legend_fraction_3->SetFillStyle(0);
  legend_fraction_3->AddEntry(test_3, "TTbar total", "l");
  legend_fraction_3->AddEntry(matched_3, "TTbar matched", "l");
  legend_fraction_3->AddEntry(unmatched_3, "TTbar unmatched", "l");
  legend_fraction_3->Draw();
  matching_fraction_3->SaveAs("/afs/desy.de/user/s/skottkej/Plots/Unfolding/matching_fraction.eps");
}
