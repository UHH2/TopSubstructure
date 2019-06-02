#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

TopSubstructureRecoHists::TopSubstructureRecoHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1D>("N_jets", "N_{Jets}", 21, -0.5, 20.5);
  book<TH1D>("eta_jet1", "#eta^{Jet 1}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet2", "#eta^{Jet 2}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet3", "#eta^{Jet 3}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet4", "#eta^{Jet 4}", 40, -2.5, 2.5);
  book<TH1D>("pt_jet1", "p_{T}^{Jet 1}", 120, 0, 1200);
  book<TH1D>("pt_jet2", "p_{T}^{Jet 2}", 120, 0, 1200);
  book<TH1D>("pt_jet3", "p_{T}^{Jet 3}", 120, 0, 1200);
  book<TH1D>("pt_jet4", "p_{T}^{Jet 4}", 120, 0, 1200);
  book<TH1D>("N_topjets", "N_{TopJets}", 10, -0.5, 9.5);

  // bJets
  book<TH1D>("N_bJets_loose", "N_{Bjets}^{loose}", 8, -0.5, 7.5);
  book<TH1D>("N_bJets_med", "N_{Bjets}^{medium}", 8, -0.5, 7.5);
  book<TH1D>("N_bJets_tight", "N_{Bjets}^{tight}", 8, -0.5, 7.5);

  book<TH1D>("M_diff1", "M_{had. TopJet candidate} - M_{lep. TopJet candidate} [GeV^{2}]", 80, -400, 400);
  book<TH1D>("M_diff2", "M_{had. TopJet candidate} - M_{lep. TopJet candidate + Muon} [GeV^{2}]", 80, -400, 400);

  // TopJets
  // first candidate
  book<TH1D>("M_tj1", "M_{had. TopJet candidate} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_tj1_rebin", "M_{had. TopJet candidate} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_tj1", "p_{T} had. TopJet candidate [GeV]", 120, 0, 1200);
  book<TH1D>("eta_tj1", "#eta had. TopJet candidate", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_tjs", "N_{subjets} all candidates", 4, -0.5, 3.5);
  book<TH1D>("Nsubjet_tj1", "N_{subjets} had. TopJet candidate", 4, -0.5, 3.5);
  book<TH1D>("pt_subjet1_tj1", "p_{T}^{subjet1} had. TopJet candidate [GeV]", 120, 0, 1200);
  book<TH1D>("pt_subjet2_tj1", "p_{T}^{subjet2} had. TopJet candidate [GeV]", 80, 0, 800);
  book<TH1D>("m_subjet1_tj1", "M_{subjet1} had. TopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1D>("m_subjet2_tj1", "M_{subjet2} had. TopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1D>("tau1_tj1", "#tau_{1} had. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau1_tj1_rebin", "#tau_{1} had. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau2_tj1", "#tau_{2} had. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau2_tj1_rebin", "#tau_{2} had. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau3_tj1", "#tau_{3} had. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau3_tj1_rebin", "#tau_{3} had. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau32_tj1", "#tau_{3}/#tau_{2} had. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau32_tj1_rebin1", "#tau_{3}/#tau_{2} had. TopJet candidate", 100, 0, 1.);
  book<TH1D>("tau32_tj1_rebin2", "#tau_{3}/#tau_{2} had. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau21_tj1", "#tau_{2}/#tau_{1} had. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau21_tj1_rebin", "#tau_{2}/#tau_{1} had. TopJet candidate", 1000, 0, 1.);

  book<TH1D>("dR_mu_tj1", "#Delta R(#mu, had. TopJet candidate)", 50, 0, 5.);
  book<TH1D>("dPhi_mu_tj1", "#Delta #Phi (#mu, had. TopJet candidate)", 40, 0, 4.);


  //  second candidate
  book<TH1D>("M_tj2", "M_{lep. TopJet candidate} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_tj2_rebin", "M_{lep. TopJet candidate} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_tj2", "p_{T} lep. TopJet candidate [GeV]", 120, 0, 1200);
  book<TH1D>("eta_tj2", "#eta lep. TopJet candidate", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_tj2", "N_{subjets} lep. TopJet candidate", 4, -0.5, 3.5);
  book<TH1D>("pt_subjet1_tj2", "p_{T}^{subjet1} lep. TopJet candidate [GeV]", 120, 0, 1200);
  book<TH1D>("pt_subjet2_tj2", "p_{T}^{subjet2} lep. TopJet candidate [GeV]", 80, 0, 800);
  book<TH1D>("m_subjet1_tj2", "M_{subjet1} lep. TopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1D>("m_subjet2_tj2", "M_{subjet2} lep. TopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1D>("tau1_tj2", "#tau_{1} lep. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau1_tj2_rebin", "#tau_{1} lep. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau2_tj2", "#tau_{2} lep. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau2_tj2_rebin", "#tau_{2} lep. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau3_tj2", "#tau_{3} lep. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau3_tj2_rebin", "#tau_{3} lep. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau32_tj2", "#tau_{3}/#tau_{2} lep. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau32_tj2_rebin1", "#tau_{3}/#tau_{2} lep. TopJet candidate", 100, 0, 1.);
  book<TH1D>("tau32_tj2_rebin2", "#tau_{3}/#tau_{2} lep. TopJet candidate", 1000, 0, 1.);
  book<TH1D>("tau21_tj2", "#tau_{2}/#tau_{1} lep. TopJet candidate", 40, 0, 1.);
  book<TH1D>("tau21_tj2_rebin", "#tau_{2}/#tau_{1} lep. TopJet candidate", 1000, 0, 1.);

  book<TH1D>("dR_mu_tj2", "#Delta R(#mu, lep. TopJet candidate)", 50, 0, 5.);
  book<TH1D>("dPhi_mu_tj2", "#Delta #Phi (#mu, lep. TopJet candidate)", 40, 0, 4.);


  // leptons
  book<TH1D>("N_mu", "N_{#mu}", 21, -0.5, 20.5);
  book<TH1D>("pt_mu", "p_{T}^{#mu} [GeV]", 100, 0, 1000);
  book<TH1D>("eta_mu", "#eta^{#mu}", 40, -2.5, 2.5);
  book<TH1D>("mass_mu", "M^{#mu} [GeV]", 50, 0, 0.5);

  //general
  book<TH1D>("E_Tmiss", "missing E_{T} [GeV]", 75, 0, 1500);
  book<TH1D>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);

  // 2-D plot
  TwoDCut = book<TH2F>("TwoDCut", "x=#Delta R y=p_{T}^{rel}", 50, 0, 2, 50, 0, 200);

  // get handles
  h_weight = ctx.get_handle<double>("h_rec_weight");
  h_weight_kin = ctx.get_handle<double>("h_gen_weight_kin");
  isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3");

}


void TopSubstructureRecoHists::fill(const Event & event){
  // Don't forget to always use the weight when filling.
  double weight;
  if(event.is_valid(h_weight))  weight = event.get(h_weight);
  else if(event.is_valid(h_weight_kin))  weight = event.get(h_weight_kin);
  else weight = event.weight;

  std::vector<TopJet>* topjet = event.topjets;

  //general
  hist("sum_event_weights")->Fill(1, weight);

  double drmin, ptrel;
  if(event.muons->size()){
    std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
    TwoDCut->Fill(drmin, ptrel, weight);
  }
  // Jets
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();

  hist("N_jets")->Fill(Njets, weight);
  if(Njets>=1) hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  if(Njets>=2) hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  if(Njets>=3) hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  if(Njets>=4) hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  if(Njets>=1) hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
  if(Njets>=2) hist("pt_jet2")->Fill(jets->at(1).pt(), weight);
  if(Njets>=3) hist("pt_jet3")->Fill(jets->at(2).pt(), weight);
  if(Njets>=4) hist("pt_jet4")->Fill(jets->at(3).pt(), weight);

  hist("N_topjets")->Fill(topjet->size(), weight);


  // bJets
  std::vector<Jet> bjets_loose, bjets_med, bjets_tight;
  CSVBTag Btag_loose = CSVBTag(CSVBTag::WP_LOOSE);
  CSVBTag Btag_medium = CSVBTag(CSVBTag::WP_MEDIUM);
  CSVBTag Btag_tight = CSVBTag(CSVBTag::WP_TIGHT);


  for (unsigned int i =0; i<jets->size(); ++i) {
    if(Btag_loose(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_loose.push_back(jets->at(i));
    }
    if(Btag_medium(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_med.push_back(jets->at(i));
    }
    if(Btag_tight(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_tight.push_back(jets->at(i));
    }
  }

  int NbJets_loose = bjets_loose.size();
  hist("N_bJets_loose")->Fill(NbJets_loose,weight);
  int NbJets_med = bjets_med.size();
  hist("N_bJets_med")->Fill(NbJets_med,weight);
  int NbJets_tight = bjets_tight.size();
  hist("N_bJets_tight")->Fill(NbJets_tight,weight);


  // TopJet
  if(topjet->size() > 0){
    for(unsigned int i=0; i < topjet->size(); i++){
      hist("Nsubjet_tjs")->Fill(topjet->at(i).subjets().size(), weight);
    }

    hist("M_tj1")->Fill(topjet->at(0).v4().M(), weight);
    hist("M_tj1_rebin")->Fill(topjet->at(0).v4().M(), weight);
    hist("pt_tj1")->Fill(topjet->at(0).pt(), weight);
    hist("eta_tj1")->Fill(topjet->at(0).eta(), weight);
    hist("Nsubjet_tj1")->Fill(topjet->at(0).subjets().size(), weight);
    if(topjet->at(0).subjets().size() >= 1) hist("pt_subjet1_tj1")->Fill(topjet->at(0).subjets().at(0).v4().pt(), weight);
    if(topjet->at(0).subjets().size() >= 2) hist("pt_subjet2_tj1")->Fill(topjet->at(0).subjets().at(1).v4().pt(), weight);
    if(topjet->at(0).subjets().size() >= 1) hist("m_subjet1_tj1")->Fill(topjet->at(0).subjets().at(0).v4().M(), weight);
    if(topjet->at(0).subjets().size() >= 2) hist("m_subjet2_tj1")->Fill(topjet->at(0).subjets().at(1).v4().M(), weight);
    hist("tau1_tj1")->Fill(topjet->at(0).tau1(), weight);
    hist("tau1_tj1_rebin")->Fill(topjet->at(0).tau1(), weight);
    hist("tau2_tj1")->Fill(topjet->at(0).tau2(), weight);
    hist("tau2_tj1_rebin")->Fill(topjet->at(0).tau2(), weight);
    hist("tau3_tj1")->Fill(topjet->at(0).tau3(), weight);
    hist("tau3_tj1_rebin")->Fill(topjet->at(0).tau3(), weight);
    hist("tau32_tj1")->Fill(topjet->at(0).tau3()/topjet->at(0).tau2(), weight);
    hist("tau32_tj1_rebin1")->Fill(topjet->at(0).tau3()/topjet->at(0).tau2(), weight);
    hist("tau32_tj1_rebin2")->Fill(topjet->at(0).tau3()/topjet->at(0).tau2(), weight);
    hist("tau21_tj1")->Fill(topjet->at(0).tau2()/topjet->at(0).tau1(), weight);
    hist("tau21_tj1_rebin")->Fill(topjet->at(0).tau2()/topjet->at(0).tau1(), weight);

    if(event.muons->size() > 0) hist("dPhi_mu_tj1")->Fill(deltaPhi(topjet->at(0), event.muons->at(0)), weight);

    if(event.muons->size() > 0) hist("dR_mu_tj1")->Fill(deltaR(event.muons->at(0), topjet->at(0)), weight);
    if(event.muons->size() > 0) hist("dPhi_mu_tj1")->Fill(deltaPhi(event.muons->at(0), topjet->at(0)), weight);

    if(topjet->size() > 1){
      if(event.muons->size() > 0) hist("dR_mu_tj2")->Fill(deltaR(event.muons->at(0), topjet->at(1)), weight);
      if(event.muons->size() > 0) hist("dPhi_mu_tj2")->Fill(deltaPhi(event.muons->at(0), topjet->at(1)), weight);

      hist("M_tj2")->Fill(topjet->at(1).v4().M(), weight);
      hist("M_tj2_rebin")->Fill(topjet->at(1).v4().M(), weight);
      hist("pt_tj2")->Fill(topjet->at(1).pt(), weight);
      hist("eta_tj2")->Fill(topjet->at(1).eta(), weight);
      hist("Nsubjet_tj2")->Fill(topjet->at(1).subjets().size(), weight);
      if(topjet->at(1).subjets().size() >= 1) hist("pt_subjet1_tj2")->Fill(topjet->at(1).subjets().at(0).v4().pt(), weight);
      if(topjet->at(1).subjets().size() >= 2) hist("pt_subjet2_tj2")->Fill(topjet->at(1).subjets().at(1).v4().pt(), weight);
      if(topjet->at(1).subjets().size() >= 1)  hist("m_subjet1_tj2")->Fill(topjet->at(1).subjets().at(0).v4().M(), weight);
      if(topjet->at(1).subjets().size() >= 2)  hist("m_subjet2_tj2")->Fill(topjet->at(1).subjets().at(1).v4().M(), weight);
      hist("tau1_tj2")->Fill(topjet->at(1).tau1(), weight);
      hist("tau1_tj2_rebin")->Fill(topjet->at(1).tau1(), weight);
      hist("tau2_tj2")->Fill(topjet->at(1).tau2(), weight);
      hist("tau2_tj2_rebin")->Fill(topjet->at(1).tau2(), weight);
      hist("tau3_tj2")->Fill(topjet->at(1).tau3(), weight);
      hist("tau3_tj2_rebin")->Fill(topjet->at(1).tau3(), weight);
      hist("tau32_tj2")->Fill(topjet->at(1).tau3()/topjet->at(1).tau2(), weight);
      hist("tau32_tj2_rebin1")->Fill(topjet->at(1).tau3()/topjet->at(1).tau2(), weight);
      hist("tau32_tj2_rebin2")->Fill(topjet->at(1).tau3()/topjet->at(1).tau2(), weight);
      hist("tau21_tj2")->Fill(topjet->at(1).tau2()/topjet->at(1).tau1(), weight);
      hist("tau21_tj2_rebin")->Fill(topjet->at(1).tau2()/topjet->at(1).tau1(), weight);


      hist("M_diff1")->Fill(topjet->at(0).v4().M() - topjet->at(1).v4().M(), weight);

      if(event.muons->size() > 0){
        const auto dummy_mass = topjet->at(1).v4() + event.muons->at(0).v4();
        hist("M_diff2")->Fill(topjet->at(0).v4().M() - dummy_mass.M(), weight);
      }
    }
  } // closing brackets of TopJet
  hist("N_mu")->Fill(event.muons->size(), weight);
  if(event.muons->size() > 0) hist("pt_mu")->Fill(event.muons->at(0).pt(), weight);
  if(event.muons->size() > 0) hist("eta_mu")->Fill(event.muons->at(0).eta(), weight);
  if(event.muons->size() > 0) hist("mass_mu")->Fill(event.muons->at(0).v4().M(), weight);

  // if(event.muons->size() > 0) hist("E_Tmiss")->Fill(event.met->pt(), weight);
  hist("E_Tmiss")->Fill(event.met->pt(), weight);
}
