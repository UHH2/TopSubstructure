#include <TH1D.h>

void Genlevel(){

TFile *ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Muon/uhh2.AnalysisModuleRunner.MC.TTbar.root", "READ");

TH1D *tt = (TH1D*) ttbar->Get("gen_mass/M_gtj1");
TH1D *tt_m = (TH1D*) ttbar->Get("gen_mass_matched/M_gtj1");
TH1D *tt_u = (TH1D*) ttbar->Get("gen_mass_unmatched/M_gtj1");

// TCanvas* c = new TCanvas("asd","asd",1000,1000);
// c->cd();
// c->UseCurrentStyle();
// tt->SetTitle("");
// tt->GetYaxis()->SetLabelSize(0.04);
// tt->GetYaxis()->SetTitleSize(0.045);
// tt->GetYaxis()->SetTitleOffset(1.6);
// tt->GetXaxis()->SetLabelSize(0.04);
// tt->GetXaxis()->SetTitleSize(0.045);
// tt->GetXaxis()->SetTitleOffset(0.9);
// tt->SetLineColor(kGray);
// tt->SetFillColor(kGray);
// tt->SetLineWidth(3);
// tt->SetTitle("");
// tt->GetXaxis()->SetTitle("M_{gen AK8 jet}");
// tt->GetXaxis()->SetTitleOffset(1.2);
// tt->GetYaxis()->SetTitle("Events");
// tt->Draw("hist");
// tt_m->SetLineColor(kGreen+2);
// tt_m->SetLineWidth(3);
// tt_m->Draw("same hist");
// tt_u->SetLineColor(kRed);
// tt_u->SetLineWidth(3);
// tt_u->Draw("same hist");
//
//
//   TLegend* legend = new TLegend(0.6,0.7,0.85,0.85);
// legend->SetFillStyle(0);
// legend->SetBorderSize(0);
// legend->AddEntry(tt,"TTbar","l");
// legend->AddEntry(tt_m,"Matched TTbar","l");
// legend->AddEntry(tt_u,"Unmatched TTbar","l");
// legend->SetTextSize(0.04);
// legend->Draw();
// c->SaveAs("/afs/desy.de/user/s/skottkej/private/masterthesis/Plots/gen_mass_dist.pdf");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TH1D *tt_tau = (TH1D*) ttbar->Get("gen_mass/tau32_gtj1");
TH1D *tt_tau_m = (TH1D*) ttbar->Get("gen_mass_matched/tau32_gtj1");
TH1D *tt_tau_u = (TH1D*) ttbar->Get("gen_mass_unmatched/tau32_gtj1");

TCanvas* c_tau = new TCanvas("asd_tau","asd_tau",1000,1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.10);
gStyle->SetPadLeftMargin(0.115);
gStyle->SetPadRightMargin(0.005);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
c_tau->cd();
c_tau->UseCurrentStyle();
tt_tau->SetTitle("");
tt_tau->GetYaxis()->SetLabelSize(0.04);
tt_tau->GetYaxis()->SetTitleSize(0.045);
tt_tau->GetYaxis()->SetTitleOffset(1.35);
tt_tau->GetXaxis()->SetLabelSize(0.04);
tt_tau->GetXaxis()->SetTitleSize(0.045);
tt_tau->GetXaxis()->SetTitleOffset(1.05);
tt_tau->SetLineColor(kGray+1);
tt_tau->SetFillColor(kGray);
tt_tau->SetLineWidth(3);
tt_tau->GetYaxis()->SetRangeUser(0.0001,350);
tt_tau->SetTitle("");
tt_tau->GetXaxis()->SetTitle("#tau_{32}");
tt_tau->GetYaxis()->SetTitle("Events");
tt_tau->Draw("hist");
// tt_tau_m->Rebin(2);
tt_tau_m->SetLineColor(kGreen);
tt_tau_m->SetLineWidth(3);
tt_tau_m->Draw("same hist");
// tt_tau_u->Rebin(2);
tt_tau_u->SetLineColor(kRed);
tt_tau_u->SetLineWidth(3);
tt_tau_u->Draw("same hist");


TLegend* legend_tau = new TLegend(0.55,0.8,0.80,0.95);
legend_tau->SetFillStyle(0);
legend_tau->SetBorderSize(0);
legend_tau->AddEntry(tt_tau,"t#bar{t}","f");
legend_tau->AddEntry(tt_tau_m,"fully merged t#bar{t}","l");
legend_tau->AddEntry(tt_tau_u,"not merged t#bar{t}","l");
legend_tau->SetTextSize(0.04);
legend_tau->Draw();
c_tau->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/gen_tau_dist.pdf");
}
