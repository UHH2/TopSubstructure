#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>

using namespace std;

void Pseudo_unfolding(){
  gStyle->SetOptStat(0);
  gStyle->SetPadLeftMargin(0.14);
  // gStyle->SetPadRightMargin(0.1);
  // gStyle->SetErrorX(0);
  //Load data
  TFile *pseudo1 = new TFile("Unfolding_pseudo1.root", "READ");
  TFile *pseudo2 = new TFile("Unfolding_pseudo2.root", "READ");
  TFile *pseudo3 = new TFile("Unfolding_pseudo3.root", "READ");

  TH1D* unfolded_pseudo1 = (TH1D*)pseudo1->Get("Unfolded Pseudodata TauScan");
  TH1D* truth_pseudo1 = (TH1D*)pseudo1->Get("Pseudodata_truth_1");
  TH1D* unfolded_pseudo2 = (TH1D*)pseudo2->Get("Unfolded Pseudodata TauScan");
  TH1D* truth_pseudo2 = (TH1D*)pseudo2->Get("Pseudodata_truth_2");
  TH1D* unfolded_pseudo3 = (TH1D*)pseudo3->Get("Unfolded Pseudodata TauScan");
  TH1D* truth_pseudo3 = (TH1D*)pseudo3->Get("Pseudodata_truth_3");

  for(int i = 1; i <= truth_pseudo3->GetNbinsX(); i++){
    double content_1 = unfolded_pseudo1->GetBinContent(i);
    double error_bin_1 = unfolded_pseudo1->GetBinError(i);
    double bin_width_1 = unfolded_pseudo1->GetBinWidth(i);
    double new_content_1 = content_1/bin_width_1;
    double new_error_1 = error_bin_1/bin_width_1;

    double content_2 = unfolded_pseudo2->GetBinContent(i);
    double error_bin_2 = unfolded_pseudo2->GetBinError(i);
    double bin_width_2 = unfolded_pseudo2->GetBinWidth(i);
    double new_content_2 = content_2/bin_width_2;
    double new_error_2 = error_bin_2/bin_width_2;

    double content_3 = unfolded_pseudo3->GetBinContent(i);
    double error_bin_3 = unfolded_pseudo3->GetBinError(i);
    double bin_width_3 = unfolded_pseudo3->GetBinWidth(i);
    double new_content_3 = content_3/bin_width_3;
    double new_error_3 = error_bin_3/bin_width_3;

    double content_truth_1 = truth_pseudo1->GetBinContent(i);
    double error_truth_1 = truth_pseudo1->GetBinError(i);
    double bin_width_truth_1 = truth_pseudo1->GetBinWidth(i);
    double new_content_truth_1 = content_truth_1/bin_width_truth_1;
    double new_error_truth_1 = error_truth_1/bin_width_truth_1;

    double content_truth_2 = truth_pseudo2->GetBinContent(i);
    double error_truth_2 = truth_pseudo2->GetBinError(i);
    double bin_width_truth_2 = truth_pseudo2->GetBinWidth(i);
    double new_content_truth_2 = content_truth_2/bin_width_truth_2;
    double new_error_truth_2 = error_truth_2/bin_width_truth_2;

    double content_truth_3 = truth_pseudo3->GetBinContent(i);
    double error_truth_3 = truth_pseudo3->GetBinError(i);
    double bin_width_truth_3 = truth_pseudo3->GetBinWidth(i);
    double new_content_truth_3 = content_truth_3/bin_width_truth_3;
    double new_error_truth_3 = error_truth_3/bin_width_truth_3;

    truth_pseudo1->SetBinContent(i, new_content_truth_1);
    truth_pseudo1->SetBinError(i, new_error_truth_1);
    truth_pseudo2->SetBinContent(i, new_content_truth_2);
    truth_pseudo2->SetBinError(i, new_error_truth_2);
    truth_pseudo3->SetBinContent(i, new_content_truth_3);
    truth_pseudo3->SetBinError(i, new_error_truth_3);
    unfolded_pseudo1->SetBinContent(i, new_content_1);
    unfolded_pseudo1->SetBinError(i, new_error_1);
    unfolded_pseudo2->SetBinContent(i, new_content_2);
    unfolded_pseudo2->SetBinError(i, new_error_2);
    unfolded_pseudo3->SetBinContent(i, new_content_3);
    unfolded_pseudo3->SetBinError(i, new_error_3);
  }

  TH1D* dummy = (TH1D*) truth_pseudo1->Clone("dummy");
  dummy->Add(truth_pseudo2);
  dummy->Add(truth_pseudo3);
  dummy->Scale(1./3);

  const Int_t n = dummy->GetNbinsX();
  Double_t x[n];
  Double_t y[n];
  for(unsigned int i = 0; i < n; i++){
    x[i] = dummy->GetBinCenter(i+1);
    y[i] = dummy->GetBinContent(i+1);
  }
  TGraphAsymmErrors* gr = new TGraphAsymmErrors(n, x, y);
  for(unsigned int i = 1; i <= n; i++){
    double t1 = truth_pseudo1->GetBinContent(i);
    double t2 = truth_pseudo2->GetBinContent(i);
    double t3 = truth_pseudo3->GetBinContent(i);
    double error_high = 0;
    double error_low = 0;

    if(t1 > t2 && t1 > t3) error_high = t1 - dummy->GetBinContent(i);
    else if(t2 > t1 && t2 > t3) error_high = t2 - dummy->GetBinContent(i);
    else if(t3 > t1 && t3 > t2) error_high = t3 - dummy->GetBinContent(i);

    if(t1 < t2 && t1 < t3) error_low = dummy->GetBinContent(i) - t1;
    else if(t2 < t1 && t2 < t3) error_low = dummy->GetBinContent(i) - t2;
    else if(t3 < t1 && t3 < t2) error_low = dummy->GetBinContent(i) - t3;

    gr->SetPointEXlow(i-1, dummy->GetBinCenter(i) - dummy->GetXaxis()->GetBinLowEdge(i));
    gr->SetPointEXhigh(i-1, dummy->GetXaxis()->GetBinUpEdge(i) - dummy->GetBinCenter(i));
    gr->SetPointEYlow(i-1, error_low);
    gr->SetPointEYhigh(i-1, error_high);
  }

  int nbins_new = n*3+1;
  Double_t check[n];
  Double_t bin_width[nbins_new];
  for(unsigned int i = 0; i <= n; i++){
    if(i == 0) check[i] = 0;
    else {
      check[i] = check[i-1] + dummy->GetBinWidth(i);
    }
  }
  for(int i = 1; i <= n; i++){
    for(int j = (3*i-2); j <= 3*i; j++){
      if(j == 1) bin_width[j-1] = 0;
        bin_width[j] = bin_width[j-1] + dummy->GetBinWidth(i)/3;
    }
  }

  TH1D* unfold1 = new TH1D("pseudo1", "pseudo1", nbins_new-1, bin_width);
  TH1D* unfold2 = new TH1D("pseudo2", "pseudo2", nbins_new-1, bin_width);
  TH1D* unfold3 = new TH1D("pseudo3", "pseudo3", nbins_new-1, bin_width);

  for(unsigned int i = 1; i <= n; i++){
    unfold1->SetBinContent((3*i-2), unfolded_pseudo1->GetBinContent(i));
    unfold1->SetBinError((3*i-2), unfolded_pseudo1->GetBinError(i));
    unfold2->SetBinContent((3*i-1), unfolded_pseudo2->GetBinContent(i));
    unfold2->SetBinError((3*i-1), unfolded_pseudo2->GetBinError(i));
    unfold3->SetBinContent((3*i), unfolded_pseudo3->GetBinContent(i));
    unfold3->SetBinError((3*i), unfolded_pseudo3->GetBinError(i));
  }

  TCanvas* c1 = new TCanvas("test", "tests", 1000,800);
  c1->cd();
  gr->SetTitle("");
  gr->GetXaxis()->SetRangeUser(0, 1);
  gr->GetYaxis()->SetRangeUser(0, 12000);
  gr->GetXaxis()->SetTitle("#tau_{3/2}");
  gr->GetYaxis()->SetTitle("#frac{events}{binwidth}");
  gr->GetYaxis()->SetTitleSize(0.04);
  gr->GetYaxis()->SetTitleOffset(1.7);
  gr->GetXaxis()->SetTitleSize(0.06);
  gr->GetXaxis()->SetTitleOffset(0.7);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);
  gr->SetMarkerStyle(20);
  gr->Draw("AP");
  unfold1->SetLineColor(kBlue+2);
  unfold1->SetMarkerColor(kBlue+2);
  unfold1->SetMarkerStyle(21);
  unfold1->Draw("same");
  unfold2->SetLineColor(kBlue+2);
  unfold2->SetMarkerColor(kBlue+2);
  unfold2->SetMarkerStyle(22);
  unfold2->Draw("same");
  unfold3->SetLineColor(kBlue+2);
  unfold3->SetMarkerColor(kBlue+2);
  unfold3->SetMarkerStyle(23);
  unfold3->Draw("same");

  TLegend *t1 = new TLegend(0.15, 0.42, 0.45, 0.62, "");
  t1->SetBorderSize(0);
  t1->SetFillStyle(0);
  t1->AddEntry(gr, "Averaged Truth", "lp");
  t1->AddEntry(unfold1, "Pseudodata 1", "lp");
  t1->AddEntry(unfold2, "Pseudodata 2", "lp");
  t1->AddEntry(unfold3, "Pseudodata 3", "lp");
  t1->Draw();
  c1->SaveAs("/afs/desy.de/user/s/skottkej/Plots/Unfolding/Test.eps");
}
