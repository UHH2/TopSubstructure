#include "UHH2/TopSubstructure/include/TopSubstructureGenHists.h"
#include "UHH2/core/include/Event.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

TopSubstructureGenHists::TopSubstructureGenHists(Context & ctx, const string & dirname, const string sort_by): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{GenJets}", 15, 0, 15);  
  book<TH1F>("eta_jet1", "#eta^{GenJet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{GenJet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{GenJet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{GenJet 4}", 40, -2.5, 2.5);

  // Gen lvl
  // TopJets
  book<TH1F>("Ntopjet_cand_gen", "number GenTopJet candidates", 6, -0.5, 5.5);

  // first candidate
  book<TH1F>("M_cand1_gen", "M_{first GenTopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand1_gen", "p_{T} first GenTopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand1_gen", "#eta first GenTopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand_gen", "number subjets all candidates", 6, -0.5, 5.5);
  book<TH1F>("Nsubjet_cand1_gen", "number subjets first GenTopJet candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand1_gen", "#tau_{1} first GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand1_gen", "#tau_{2} first GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand1_gen", "#tau_{3} first GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand1_gen", "#tau_{3}/#tau_{2} first GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau21_cand1_gen", "#tau_{2}/#tau_{1} first GenTopJet candidate", 40, 0, 1.);
  
  // second candidate
  book<TH1F>("M_cand2_gen", "M_{second GenTopJet candidate} [GeV^{2}/c^{2}]", 70, 0, 700);
  book<TH1F>("pt_topjet_cand2_gen", "p_{T} second GenTopJet candidate", 150, 0, 1500);
  book<TH1F>("eta_topjet_cand2_gen", "#eta second GenTopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand2_gen", "number subjets second GenTopJet candidate", 6, -0.5, 5.5);
  book<TH1F>("tau1_topjet_cand2_gen", "#tau_{1} second GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand2_gen", "#tau_{2} second GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand2_gen", "#tau_{3} second GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand2_gen", "#tau_{3}/#tau_{2} second GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau21_cand2_gen", "#tau_{2}/#tau_{1} second GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("dphi_cand1_gen", "#Delta #Phi (#mu, cand1)", 40, 0, 4.0);
  book<TH1F>("dphi_cand2_gen", "#Delta #Phi (#mu, cand2)", 40, 0, 4.0);


  // only TTbar
  book<TH1F>("dR8_gen", "#Delta R gen", 50, 0, 5);


  book<TH1F>("dR8_topjet_genjet", "#Delta R(TopJet, GenTopJet)", 50, 0, 5);


  book<TH1F>("N_jets_clean", "N_{jets} no AK8", 15, 0, 15);


  //general
  book<TH1F>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);


  // get handles
  if(sort_by == "dphi") h_gentopjet_cand = ctx.get_handle<vector<GenTopJet>>("gentopjet_cand");
  if(sort_by == "mass") h_gentopjet_cand = ctx.get_handle<vector<GenTopJet>>("gentopjet_cand");
  h_genjetsel = ctx.get_handle<vector<Particle>>("genjetsel");
  h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
}


void TopSubstructureGenHists::fill(const Event & event){
  // fill the histograms. Please note the comments in the header file:
  // 'hist' is used here a lot for simplicity, but it will be rather
  // slow when you have many histograms; therefore, better
  // use histogram pointers as members as in 'UHH2/common/include/ElectronHists.h'
  
  // Don't forget to always use the weight when filling.
  double weight = event.weight;

  //general
  hist("sum_event_weights")->Fill(1, weight);

  // Jets
  std::vector<Particle>* jets = event.genjets;
  int Njets = jets->size();

  hist("N_jets")->Fill(Njets, weight);
  if(Njets>=1) hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  if(Njets>=2) hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  if(Njets>=3) hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  if(Njets>=4) hist("eta_jet4")->Fill(jets->at(3).eta(), weight);

  // GenTopJet
  if(event.is_valid(h_gentopjet_cand)) {
    std::vector<GenTopJet> gentopjet_cand;
    gentopjet_cand = event.get(h_gentopjet_cand);

    hist("Ntopjet_cand_gen")->Fill(gentopjet_cand.size(), weight);

    if(gentopjet_cand.size()>0){
      for(unsigned int i=0; i < gentopjet_cand.size(); i++){
	hist("Nsubjet_cand_gen")->Fill(gentopjet_cand.at(i).subjets().size(), weight);
      }
      hist("M_cand1_gen")->Fill(gentopjet_cand.at(0).v4().M(), weight);     
      hist("pt_topjet_cand1_gen")->Fill(gentopjet_cand.at(0).pt(), weight);      
      hist("eta_topjet_cand1_gen")->Fill(gentopjet_cand.at(0).eta(), weight);
      hist("Nsubjet_cand1_gen")->Fill(gentopjet_cand.at(0).subjets().size(), weight); 
      hist("tau1_topjet_cand1_gen")->Fill(gentopjet_cand.at(0).tau1(), weight);
      hist("tau2_topjet_cand1_gen")->Fill(gentopjet_cand.at(0).tau2(), weight);
      hist("tau3_topjet_cand1_gen")->Fill(gentopjet_cand.at(0).tau3(), weight);    
      hist("tau32_cand1_gen")->Fill(gentopjet_cand.at(0).tau3()/gentopjet_cand.at(0).tau2(), weight);
      hist("tau21_cand1_gen")->Fill(gentopjet_cand.at(0).tau2()/gentopjet_cand.at(0).tau1(), weight);

      if(event.muons->size() > 0){
	sort_by_pt<Muon>(*event.muons);
	const auto & muon = event.muons->at(0);
	hist("dphi_cand1_gen")->Fill(deltaPhi(gentopjet_cand.at(0), muon),weight);
	if(gentopjet_cand.size()>=2) hist("dphi_cand2_gen")->Fill(deltaPhi(gentopjet_cand.at(1), muon),weight);
      }

      if(event.is_valid(h_ttbargen)){
	const auto & ttbargen = event.get(h_ttbargen);

	GenParticle tophad;
	if(ttbargen.IsTopHadronicDecay()){
	  tophad = ttbargen.Top();
	  hist("dR8_gen")->Fill(deltaR(tophad, gentopjet_cand.at(0)), weight);
	}
	else if(ttbargen.IsAntiTopHadronicDecay()){
	  tophad = ttbargen.Antitop();
	  hist("dR8_gen")->Fill(deltaR(tophad, gentopjet_cand.at(0)), weight);
	}
      }
    } // closing brackets of gentopjet_cand.size()

    if(gentopjet_cand.size()>=2){
      hist("M_cand2_gen")->Fill(gentopjet_cand.at(1).v4().M(), weight);     
      hist("pt_topjet_cand2_gen")->Fill(gentopjet_cand.at(1).pt(), weight);      
      hist("eta_topjet_cand2_gen")->Fill(gentopjet_cand.at(1).eta(), weight);
      hist("Nsubjet_cand2_gen")->Fill(gentopjet_cand.at(1).subjets().size(), weight);
      hist("tau1_topjet_cand2_gen")->Fill(gentopjet_cand.at(1).tau1(), weight);
      hist("tau2_topjet_cand2_gen")->Fill(gentopjet_cand.at(1).tau2(), weight);
      hist("tau3_topjet_cand2_gen")->Fill(gentopjet_cand.at(1).tau3(), weight);
      hist("tau32_cand2_gen")->Fill(gentopjet_cand.at(1).tau3()/gentopjet_cand.at(1).tau2(), weight);
      hist("tau21_cand2_gen")->Fill(gentopjet_cand.at(1).tau2()/gentopjet_cand.at(1).tau1(), weight);
    }

  } // closing brackets of GenTopJet
}

// TopSubstructureGenHists::~TopSubstructureGenHists(){}
