#include <TH1D.h>
#include <string>
using namespace std;

void Ratio(){

  TFile *wjets = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.WJets.root", "READ");
  TFile *ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.TTbar.root", "READ");
  TFile *st = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.ST.root", "READ");
  TFile *qcd = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.QCD.root", "READ");
  TFile *diboson = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.Diboson.root", "READ");
  TFile *dy = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection_event_output/Electron/uhh2.AnalysisModuleRunner.MC.DYJets.root", "READ");


  TH1D *wj = (TH1D*) wjets->Get("mass/sum_event_weights");
  TH1D *tt = (TH1D*) ttbar->Get("mass/sum_event_weights");
  TH1D *s = (TH1D*) st->Get("mass/sum_event_weights");
  TH1D *q = (TH1D*) qcd->Get("mass/sum_event_weights");
  TH1D *di = (TH1D*) diboson->Get("mass/sum_event_weights");
  TH1D *d = (TH1D*) dy->Get("mass/sum_event_weights");
// tt->Scale(0.77);
  double ratio = q->Integral()/(wj->Integral()+tt->Integral()+s->Integral()+q->Integral()+di->Integral()+d->Integral());

  cout << "ratio: " << ratio << '\n';
}
