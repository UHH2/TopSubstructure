#include "TH1.h"
#include "THStack.h"
#include "TFile.h"

void Script(){
TFile* data = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.DATA.DATA.root");
TFile* ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3_Mtt.root");
TFile* qcd = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.QCD.root");
TFile* di = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.Diboson.root");
TFile* wj = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.WJets.root");
TFile* st = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.ST.root");
TFile* dyj = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Test_Sel2/Muon/uhh2.AnalysisModuleRunner.MC.DYJets.root");


TH1D* data_d = (TH1D*) data->Get("nbtag_medium/sum_event_weights");
TH1D* ttbar_d = (TH1D*) ttbar->Get("nbtag_medium/sum_event_weights");
TH1D* qcd_d = (TH1D*) qcd->Get("nbtag_medium/sum_event_weights");
TH1D* di_d = (TH1D*) di->Get("nbtag_medium/sum_event_weights");
TH1D* wj_d = (TH1D*) wj->Get("nbtag_medium/sum_event_weights");
TH1D* st_d = (TH1D*) st->Get("nbtag_medium/sum_event_weights");
TH1D* dyj_d = (TH1D*) dyj->Get("nbtag_medium/sum_event_weights");

double ratio = (data_d->Integral()-qcd_d->Integral()-di_d->Integral()-wj_d->Integral()-st_d->Integral()-dyj_d->Integral())/ttbar_d->Integral();

cout << "ratio: " << ratio << '\n';
}
