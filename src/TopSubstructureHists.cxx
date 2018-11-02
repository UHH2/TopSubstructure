#include "UHH2/TopSubstructure/include/TopSubstructureHists.h"
#include "UHH2/core/include/Event.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

TopSubstructureHists::TopSubstructureHists(Context & ctx, const string & dirname, const string sort_by): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{jets}", 15, 0, 15);  
  book<TH1F>("eta_jet1", "#eta^{jet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{jet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{jet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{jet 4}", 40, -2.5, 2.5);

  // bJets
  book<TH1F>("N_bJets_loose", "N_{Bjets}^{loose}", 8, -0.5, 7.5);
  book<TH1F>("N_bJets_med", "N_{Bjets}^{medium}", 8, -0.5, 7.5);
  book<TH1F>("N_bJets_tight", "N_{Bjets}^{tight}", 8, -0.5, 7.5);

  // TopJets
  book<TH1F>("Ntopjet_cand", "number TopJet candidates", 6, -0.5, 5.5);

  // first candidate
  book<TH1F>("M_cand1", "M_{first TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand1", "p_{T} first TopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand1", "#eta first TopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand", "number subjets all candidates", 6, -0.5, 5.5);
  book<TH1F>("Nsubjet_cand1", "number subjets first TopJet candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand1", "#tau_{1} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand1", "#tau_{2} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand1", "#tau_{3} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand1", "#tau_{3}/#tau_{2} first TopJet candidate", 40, 0, 1.0);
  book<TH1F>("tau21_cand1", "#tau_{2}/#tau_{1} first TopJet candidate", 40, 0, 1.0);
  
  // second candidate
  book<TH1F>("M_cand2", "M_{second TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand2", "p_{T} second TopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand2", "#eta second TopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand2", "number subjets second TopJet candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand2", "#tau_{1} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand2", "#tau_{2} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand2", "#tau_{3} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand2", "#tau_{3}/#tau_{2} second TopJet candidate", 40, 0, 1.0);
  book<TH1F>("tau21_cand2", "#tau_{2}/#tau_{1} second TopJet candidate", 40, 0, 1.0);


  // // third candidate
  // book<TH1F>("M_cand3", "M_{third TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  // book<TH1F>("pt_topjet_cand3", "p_{T} third TopJet candidate", 150, 0, 1500);
  // book<TH1F>("eta_topjet_cand3", "#eta third TopJet candidate", 40, -2.5, 2.5);
  // book<TH1F>("Nsubjet_cand3", "number subjets third TopJet candidate", 6, -0.5, 5.5);
  // book<TH1F>("tau1_topjet_cand3", "#tau_{1} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau2_topjet_cand3", "#tau_{2} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau3_topjet_cand3", "#tau_{3} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau32_cand3", "#tau_{3}/#tau_{2} third TopJet candidate", 40, 0, 1.0);
  // book<TH1F>("tau21_cand3", "#tau_{2}/#tau_{1} third TopJet candidate", 40, 0, 1.0);


  // // fourth candidate
  // book<TH1F>("M_cand4", "M_{fourth TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  // book<TH1F>("pt_topjet_cand4", "p_{T} fourth TopJet candidate", 150, 0, 1500);
  // book<TH1F>("eta_topjet_cand4", "#eta fourth TopJet candidate", 40, -2.5, 2.5);
  // book<TH1F>("Nsubjet_cand4", "number subjets fourth TopJet candidate", 6, -0.5, 5.5);
  // book<TH1F>("tau1_topjet_cand4", "#tau_{1} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau2_topjet_cand4", "#tau_{2} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau3_topjet_cand4", "#tau_{3} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau32_cand4", "#tau_{3}/#tau_{2} fourth TopJet candidate", 40, 0, 1.0);
  // book<TH1F>("tau21_cand4", "#tau_{2}/#tau_{1} fourth TopJet candidate", 40, 0, 1.0);



  // only TTbar
  book<TH1F>("dR8", "#Delta R", 50, 0, 5);

  // first candidate
  book<TH1F>("M_cand1_ttbar", "M_{first TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand1_ttbar", "p_{T} first TopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand1_ttbar", "#eta first TopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand1_ttbar", "number subjet first candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand1_ttbar", "#tau_{1} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand1_ttbar", "#tau_{2} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand1_ttbar", "#tau_{3} first TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand1_ttbar", "#tau_{3}/#tau_{2} first TopJet candidate", 40, 0, 1.0);
  book<TH1F>("tau21_cand1_ttbar", "#tau_{2}/#tau_{1} first TopJet candidate", 40, 0, 1.0);
  book<TH1F>("dphi_cand1_ttbar", "#Delta #Phi (#mu, cand1)", 40, 0, 4.0);


  // second candidate
  book<TH1F>("M_cand2_ttbar", "M_{second TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand2_ttbar", "p_{T} second TopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand2_ttbar", "#eta second TopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand2_ttbar", "number subjet second TopJet candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand2_ttbar", "#tau_{1} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand2_ttbar", "#tau_{2} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand2_ttbar", "#tau_{3} second TopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand2_ttbar", "#tau_{3}/#tau_{2} second TopJet candidate", 40, 0, 1.0);
  book<TH1F>("tau21_cand2_ttbar", "#tau_{2}/#tau_{1} second TopJet candidate", 40, 0, 1.0);
  book<TH1F>("dphi_cand2_ttbar", "#Delta #Phi (#mu, cand2)", 40, 0, 4.0);


  // // third candidate
  // book<TH1F>("M_cand3_ttbar", "M_{third TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  // book<TH1F>("pt_topjet_cand3_ttbar", "p_{T} third TopJet candidate", 150, 0, 1500);
  // book<TH1F>("eta_topjet_cand3_ttbar", "#eta third TopJet candidate", 40, -2.5, 2.5);
  // book<TH1F>("Nsubjet_cand3_ttbar", "number subjet third TopJet candidate", 6, -0.5, 5.5);
  // book<TH1F>("tau1_topjet_cand3_ttbar", "#tau_{1} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau2_topjet_cand3_ttbar", "#tau_{2} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau3_topjet_cand3_ttbar", "#tau_{3} third TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau32_cand3_ttbar", "#tau_{3}/#tau_{2} third TopJet candidate", 40, 0, 1.0);
  // book<TH1F>("tau21_cand3_ttbar", "#tau_{2}/#tau_{1} third TopJet candidate", 40, 0, 1.0);
  book<TH1F>("dphi_cand3_ttbar", "#Delta #Phi (#mu, cand3)", 40, 0, 4.0);

  // // fourth candidate
  // book<TH1F>("M_cand4_ttbar", "M_{fourth TopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  // book<TH1F>("pt_topjet_cand4_ttbar", "p_{T} fourth TopJet candidate", 150, 0, 1500);
  // book<TH1F>("eta_topjet_cand4_ttbar", "#eta fourth TopJet candidate", 40, -2.5, 2.5);
  // book<TH1F>("Nsubjet_cand4_ttbar", "number subjet fourth TopJet candidate", 6, -0.5, 5.5);
  // book<TH1F>("tau1_topjet_cand4_ttbar", "#tau_{1} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau2_topjet_cand4_ttbar", "#tau_{2} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau3_topjet_cand4_ttbar", "#tau_{3} fourth TopJet candidate", 40, 0, 1.);
  // book<TH1F>("tau32_cand4_ttbar", "#tau_{3}/#tau_{2} fourth TopJet candidate", 40, 0, 1.0);
  // book<TH1F>("tau21_cand4_ttbar", "#tau_{2}/#tau_{1} fourth TopJet candidate", 40, 0, 1.0);
  book<TH1F>("dphi_cand4_ttbar", "#Delta #Phi (#mu, cand4)", 40, 0, 4.0);

  // leptons
  book<TH1F>("N_mu", "N^{#mu}", 6, -0.5, 5.5);
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV/c]", 40, 0, 200);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.5, 2.5);
  book<TH1F>("reliso_mu", "#mu rel. Iso", 50, 0, 0.5);
  book<TH1F>("N_ele", "N^{ele}", 6, -0.5, 5.5);

  //general
  book<TH1F>("E_Tmiss", "missing E_{T} [GeV]", 50, 0, 1000);
  book<TH1F>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);

  // primary vertices
  book<TH1F>("N_pv", "N^{PV}", 50, 0, 50);

  // 2-D plot
  TwoDCut = book<TH2F>("TwoDCut", "x=#Delta R y=p_{T}^{rel}", 50, 0, 2, 50, 0, 200);


  // get handles
  if(sort_by == "dphi") h_topjet_cand = ctx.get_handle<vector<TopJet>>("topjet_cand");
  h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
  
  // how to save handle to a variable
  // if(event.is_valid(h_fjet_nogen_inZ)) fjet_nogen_inZ = event.get(h_fjet_nogen_inZ);

  isMC = (ctx.get("dataset_type") == "MC"); 
  if(ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft") isTTbar = true;

}


void TopSubstructureHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  hist("sum_event_weights")->Fill(1, weight);

  //general
  auto met = event.met->pt();
  hist("E_Tmiss")->Fill(met, weight);

  // Jets
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();

  hist("N_jets")->Fill(Njets, weight);

  if(Njets>=1) hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  if(Njets>=2) hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  if(Njets>=3) hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  if(Njets>=4) hist("eta_jet4")->Fill(jets->at(3).eta(), weight);


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


  // leptons
  int Nmuons = event.muons->size();
  hist("N_mu")->Fill(Nmuons, weight);
  for (const Muon & thismu : *event.muons){
    hist("pt_mu")->Fill(thismu.pt(), weight);
    hist("eta_mu")->Fill(thismu.eta(), weight);
    hist("reliso_mu")->Fill(thismu.relIso(), weight);
  }
  
  int Nele = event.electrons->size();
  hist("N_ele")->Fill(Nele, weight);


  // primary vertices
  int Npvs = event.pvs->size();
  hist("N_pv")->Fill(Npvs, weight);


  // TopJet
  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand;
    topjet_cand = event.get(h_topjet_cand);

    hist("Ntopjet_cand")->Fill(topjet_cand.size(), weight);

    if(topjet_cand.size()>0){
      for(unsigned int i=0; i < topjet_cand.size(); i++){
	hist("Nsubjet_cand")->Fill(topjet_cand.at(i).subjets().size(), weight);
      }
      hist("Nsubjet_cand1")->Fill(topjet_cand.at(0).subjets().size(), weight); 
      hist("M_cand1")->Fill(topjet_cand.at(0).v4().M(), weight);     
      hist("pt_topjet_cand1")->Fill(topjet_cand.at(0).pt(), weight);      
      hist("eta_topjet_cand1")->Fill(topjet_cand.at(0).eta(), weight);
      hist("tau1_topjet_cand1")->Fill(topjet_cand.at(0).tau1(), weight);
      hist("tau2_topjet_cand1")->Fill(topjet_cand.at(0).tau2(), weight);
      hist("tau3_topjet_cand1")->Fill(topjet_cand.at(0).tau3(), weight);

      double drmin, ptrel;
      if(event.muons->size()){
	std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
	TwoDCut->Fill(drmin, ptrel, weight);
      }
  
    
      hist("tau32_cand1")->Fill(topjet_cand.at(0).tau3()/topjet_cand.at(0).tau2(), weight);
      hist("tau21_cand1")->Fill(topjet_cand.at(0).tau2()/topjet_cand.at(0).tau1(), weight);

      if(isTTbar){
	sort_by_pt<Muon>(*event.muons);
	const auto & muon = event.muons->at(0);

	hist("M_cand1_ttbar")->Fill(topjet_cand.at(0).v4().M(), weight);     
	hist("pt_topjet_cand1_ttbar")->Fill(topjet_cand.at(0).pt(), weight);      
	hist("eta_topjet_cand1_ttbar")->Fill(topjet_cand.at(0).eta(), weight);
	hist("Nsubjet_cand1_ttbar")->Fill(topjet_cand.at(0).subjets().size(), weight);
	hist("tau1_topjet_cand1_ttbar")->Fill(topjet_cand.at(0).tau1(), weight);
	hist("tau2_topjet_cand1_ttbar")->Fill(topjet_cand.at(0).tau2(), weight);
	hist("tau3_topjet_cand1_ttbar")->Fill(topjet_cand.at(0).tau3(), weight);
	hist("tau32_cand1_ttbar")->Fill(topjet_cand.at(0).tau3()/topjet_cand.at(0).tau2(), weight);
	hist("tau21_cand1_ttbar")->Fill(topjet_cand.at(0).tau2()/topjet_cand.at(0).tau1(), weight);

	hist("dphi_cand1_ttbar")->Fill(deltaPhi(topjet_cand.at(0), muon),weight);

	if(topjet_cand.size()==2){
	  hist("M_cand2_ttbar")->Fill(topjet_cand.at(1).v4().M(), weight);     
	  hist("pt_topjet_cand2_ttbar")->Fill(topjet_cand.at(1).pt(), weight);      
	  hist("eta_topjet_cand2_ttbar")->Fill(topjet_cand.at(1).eta(), weight);
	  hist("Nsubjet_cand2_ttbar")->Fill(topjet_cand.at(1).subjets().size(), weight);
	  hist("tau1_topjet_cand2_ttbar")->Fill(topjet_cand.at(1).tau1(), weight);
	  hist("tau2_topjet_cand2_ttbar")->Fill(topjet_cand.at(1).tau2(), weight);
	  hist("tau3_topjet_cand2_ttbar")->Fill(topjet_cand.at(1).tau3(), weight);
	  hist("tau32_cand2_ttbar")->Fill(topjet_cand.at(1).tau3()/topjet_cand.at(1).tau2(), weight);
	  hist("tau21_cand2_ttbar")->Fill(topjet_cand.at(1).tau2()/topjet_cand.at(1).tau1(), weight);
	  hist("dphi_cand2_ttbar")->Fill(deltaPhi(topjet_cand.at(1), muon),weight);
	}
	if(topjet_cand.size()==3){
	  // hist("M_cand3_ttbar")->Fill(topjet_cand.at(2).v4().M(), weight);     
	  // hist("pt_topjet_cand3_ttbar")->Fill(topjet_cand.at(2).pt(), weight);      
	  // hist("eta_topjet_cand3_ttbar")->Fill(topjet_cand.at(2).eta(), weight);
	  // hist("Nsubjet_cand3_ttbar")->Fill(topjet_cand.at(2).subjets().size(), weight);
	  // hist("tau1_topjet_cand3_ttbar")->Fill(topjet_cand.at(2).tau1(), weight);
	  // hist("tau2_topjet_cand3_ttbar")->Fill(topjet_cand.at(2).tau2(), weight);
	  // hist("tau3_topjet_cand3_ttbar")->Fill(topjet_cand.at(2).tau3(), weight);
	  // hist("tau32_cand3_ttbar")->Fill(topjet_cand.at(2).tau3()/topjet_cand.at(2).tau2(), weight);
	  // hist("tau21_cand3_ttbar")->Fill(topjet_cand.at(2).tau2()/topjet_cand.at(2).tau1(), weight);
	  hist("dphi_cand3_ttbar")->Fill(deltaPhi(topjet_cand.at(2), muon),weight);
	}
	if(topjet_cand.size()==4){
	  // hist("M_cand4_ttbar")->Fill(topjet_cand.at(3).v4().M(), weight);     
	  // hist("pt_topjet_cand4_ttbar")->Fill(topjet_cand.at(3).pt(), weight);      
	  // hist("eta_topjet_cand4_ttbar")->Fill(topjet_cand.at(3).eta(), weight);
	  // hist("Nsubjet_cand4_ttbar")->Fill(topjet_cand.at(3).subjets().size(), weight);
	  // hist("tau1_topjet_cand4_ttbar")->Fill(topjet_cand.at(3).tau1(), weight);
	  // hist("tau2_topjet_cand4_ttbar")->Fill(topjet_cand.at(3).tau2(), weight);
	  // hist("tau3_topjet_cand4_ttbar")->Fill(topjet_cand.at(3).tau3(), weight);
	  // hist("tau32_cand4_ttbar")->Fill(topjet_cand.at(3).tau3()/topjet_cand.at(3).tau2(), weight);
	  // hist("tau21_cand4_ttbar")->Fill(topjet_cand.at(3).tau2()/topjet_cand.at(3).tau1(), weight);
	  hist("dphi_cand4_ttbar")->Fill(deltaPhi(topjet_cand.at(3), muon),weight);
	}

	if(event.is_valid(h_ttbargen)){
	  const auto & ttbargen = event.get(h_ttbargen);

	  GenParticle tophad;
	  if(ttbargen.IsTopHadronicDecay()){
	    tophad = ttbargen.Top();
	    hist("dR8")->Fill(deltaR(tophad, topjet_cand.at(0)), weight);

	  }
	  else if(ttbargen.IsAntiTopHadronicDecay()){
	    tophad = ttbargen.Antitop();
	    hist("dR8")->Fill(deltaR(tophad, topjet_cand.at(0)), weight);
	  }
	}
      }
    } // closing brackets of topjet_cand.size()

  } // closing brackets of TopJet


}

TopSubstructureHists::~TopSubstructureHists(){}