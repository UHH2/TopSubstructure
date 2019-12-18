#include "TH1.h"
#include "THStack.h"
#include "TFile.h"
#include "/nfs/dust/cms/user/skottkej/SFramePlotter/Plots.h"


void SingleEPS_Plots(){

  TFile* Data = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.DATA.DATA.root");
  TFile* TTbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.MC.TTbar.root");
  TFile* DYJets = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.MC.DYJets.root");
  TFile* Diboson = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.MC.Diboson.root");
  TFile* WJets = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.MC.WJets.root");
  TFile* ST = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10_data/Electron/uhh2.AnalysisModuleRunner.MC.ST.root");

  TH1D* h_data    = (TH1D*) Data->Get("mass/tau32_tj1");
  TH1D* h_ttbar   = (TH1D*) TTbar->Get("mass/tau32_tj1");
  TH1D* h_dy      = (TH1D*) DYJets->Get("mass/tau32_tj1");
  TH1D* h_diboson = (TH1D*) Diboson->Get("mass/tau32_tj1");
  TH1D* h_wjets   = (TH1D*) WJets->Get("mass/tau32_tj1");
  TH1D* h_st      = (TH1D*) ST->Get("mass/tau32_tj1");

  vector<TObjArray*> harr;
  vector<TObjArray*> harr_sys;

  for (int i=0; i<InputFilenames->GetEntries(); ++i){
    TString file = ((TObjString*)InputFilenames->At(i))->GetString();
    TString legname = ((TObjString*)SampleNames->At(i))->GetString();
    fp.OpenFile(file, CycleName);
    fp.BrowseFile();
    float unc = 0.;
    if (SamplesUnc.GetSize()!=0) unc = SamplesUnc[i];
    fp.SetInfo(legname, SamplesWeight[i], HistColors[i], HistMarkers[i], unc);
    fp.CloseFile();
    harr.push_back( fp.GetHists() );
  }

  SPlotter pl;
  //pl.SetDebug();
  pl.DoStacking(harr, SamplesToStack);

  // ____________ set up the plotter ______________

  pl.SetShapeNorm(ShapeNorm);
  pl.SetScaleToWidth(ScaleToWidth);
  pl.SetPortraitMode(PortraitMode);
  pl.SetDrawEntries(DrawEntries);
  pl.SetPlotRatio(RatioPlot);
  pl.SetZScoreInRatio(zscore);
  pl.SetDrawLumi(DrawLumi);
  pl.SetDrawLegend(DrawLegend);
  pl.SetPsFilename(PsFilename);
  pl.SetLumi(Lumi);
  pl.SetEnergy(Energy);
  pl.SetNormError(SysErr);
  pl.SetSingleEPSMode(SingleEPS);
  pl.SetForPublication(ForPublication);
  pl.SetForPrelim(ForPrelim);
  pl.SetLogy(Logy);
  pl.SetIgnoreEmptyBins(IgnoreEmptyBins);
  pl.SetWorkInProgress(WorkInProgress);

  // _______________ do the plotting ______________

  pl.ProcessAndPlot(harr);

  // Done! Exit Root
  gSystem->Exit(0);

}
