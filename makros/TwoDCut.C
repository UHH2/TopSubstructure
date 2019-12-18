#include <TH1D.h>
#include <TLine.h>
#include <TLatex.h>
#include <string>
using namespace std;

void TwoDCut(){

  TFile *ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Selection_2/Muon/uhh2.AnalysisModuleRunner.MC.TTbar.root", "READ");
  TFile *qcd = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Selection_2/Muon/uhh2.AnalysisModuleRunner.MC.QCD.root", "READ");

  TH2D *tt = (TH2D*) ttbar->Get("nlep_0/TwoDCut");
  TH2D *q = (TH2D*) qcd->Get("nlep_0/TwoDCut");

  TCanvas* c1 = new TCanvas("1","1", 1150,1000);
  double normtt = 1/tt->Integral();
  gStyle->SetLegendBorderSize(0);
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  c1->cd();
  c1->UseCurrentStyle();
  c1->SetLogz();
  tt->SetTitle("");
  tt->GetYaxis()->SetLabelSize(0.04);
  tt->GetYaxis()->SetTitleSize(0.045);
  tt->GetYaxis()->SetTitleOffset(1.4);
  tt->GetXaxis()->SetLabelSize(0.04);
  tt->GetXaxis()->SetTitleSize(0.045);
  tt->GetXaxis()->SetTitleOffset(1.05);
  tt->SetLineWidth(3);
  tt->GetXaxis()->SetTitle("#Delta R(lepton, next AK4 jet)");
  tt->GetYaxis()->SetTitle("p_{T}^{rel}(lepton, next AK4 jet)");
  tt->Scale(normtt);
  tt->GetZaxis()->SetRangeUser(0.0000001, 0.5);
  tt->Draw("colz");

  TLine* t1t = new TLine(0.4,0,0.4,40);
  TLine* t2t = new TLine(0,40,0.4,40);
  t1t->SetLineColor(kRed);
  t1t->SetLineWidth(3);
  t1t->Draw("same");
  t2t->SetLineColor(kRed);
  t2t->SetLineWidth(3);
  t2t->Draw("same");
  TLatex latext;
  latext.SetTextAlign(11);
  latext.SetTextSize(0.055);
  latext.SetTextFont(42);
  latext.DrawLatex(0.935,206,"t#bar{t}");
  c1->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/twod_tt.pdf");

  TCanvas* c2 = new TCanvas("2","2", 1150,1000);
  double normqcd = 1/q->Integral();
  c2->cd();
  c2->UseCurrentStyle();
  c2->SetLogz();
  q->SetTitle("");
  q->GetYaxis()->SetLabelSize(0.04);
  q->GetYaxis()->SetTitleSize(0.045);
  q->GetYaxis()->SetTitleOffset(1.4);
  q->GetXaxis()->SetLabelSize(0.04);
  q->GetXaxis()->SetTitleSize(0.045);
  q->GetXaxis()->SetTitleOffset(1.05);
  q->SetLineWidth(3);
  q->GetXaxis()->SetTitle("#Delta R(lepton, next AK4 jet)");
  q->GetYaxis()->SetTitle("p_{T}^{rel}(lepton, next AK4 jet)");
  q->Scale(normqcd);
  q->GetZaxis()->SetRangeUser(0.0000001, 0.5);
  q->Draw("colz");

  TLine* t1q = new TLine(0.4,0,0.4,40);
  TLine* t2q = new TLine(0,40,0.4,40);
  t1q->SetLineColor(kRed);
  t1q->SetLineWidth(3);
  t1q->Draw("same");
  t2q->SetLineColor(kRed);
  t2q->SetLineWidth(3);
  t2q->Draw("same");
  TLatex latexq;
  latexq.SetTextAlign(11);
  latexq.SetTextSize(0.055);
  latexq.SetTextFont(42);
  latexq.DrawLatex(0.83,206,"QCD");
  c2->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/twod_qcd.pdf");
}
