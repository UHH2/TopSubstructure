#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;

GenDPhiSelection::GenDPhiSelection(uhh2::Context& ctx, double dphi_min_, double dphi_max_): h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), dphi_min(dphi_min_), dphi_max(dphi_max_){}
bool GenDPhiSelection::passes(const Event & event){

  bool pass = false;

  if(event.is_valid(h_gentopjet_cand)) {
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet_cand);

    if(gentopjets.size() <= 0){
      std::cout << "\n DPhiSelection::passes: There are no gentopjets in the event. returning 'false'\n" << std::endl;
      return false;
    }

    const auto & gentopjet = gentopjets.at(0);

    std::vector<GenParticle> gen;
    for(unsigned int i=0; i < event.genparticles->size(); i++){
      if (abs(event.genparticles->at(i).pdgId()) == 13) {
	      gen.push_back(event.genparticles->at(i));
      }
    }
    double dphi = deltaPhi(gentopjet, gen.at(0));
    pass = dphi >= dphi_min && ( dphi <= dphi_max || dphi_max < 0);
  }

  return pass;
}

GenQuarkGenJetMatching::GenQuarkGenJetMatching(uhh2::Context & ctx):h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool GenQuarkGenJetMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_gentopjet_cand)) {
    std::vector<GenTopJet> gentopjet_cand = event.get(h_gentopjet_cand);

    if (gentopjet_cand.size() > 0) {
      if(event.is_valid(h_ttbargen)){
        const auto ttbargen = event.get(h_ttbargen);

        GenParticle bq, q1, q2;
        if(ttbargen.IsTopHadronicDecay()){
      	  bq = ttbargen.bTop();
      	  q1 = ttbargen.Wdecay1();
      	  q2 = ttbargen.Wdecay2();
      	  if(deltaR(bq, gentopjet_cand.at(0)) <= 0.8 && deltaR(q1, gentopjet_cand.at(0)) <= 0.8 && deltaR(q2, gentopjet_cand.at(0)) <= 0.8) pass = true;
        }
        else if(ttbargen.IsAntiTopHadronicDecay()){
      	  bq = ttbargen.bAntitop();
      	  q1 = ttbargen.WMinusdecay1();
      	  q2 = ttbargen.WMinusdecay2();
	        if(deltaR(bq, gentopjet_cand.at(0)) <= 0.8 && deltaR(q1, gentopjet_cand.at(0)) <= 0.8 && deltaR(q2, gentopjet_cand.at(0)) <= 0.8) pass = true;
        }
      }
    }
  }
  return pass;
}

GenNTopJetCand::GenNTopJetCand(uhh2::Context & ctx, double n_min_, double n_max_): h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), n_min(n_min_), n_max(n_max_){}
bool GenNTopJetCand::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_gentopjet_cand)){
    std::vector<GenTopJet> gentopjet_cand = event.get(h_gentopjet_cand);
    pass = gentopjet_cand.size() >= n_min && (gentopjet_cand.size() <= n_max || n_max < 0);
  }
  return pass;
}

GenNTopJet::GenNTopJet(double n_min_, double n_max_):n_min(n_min_), n_max(n_max_){}
bool GenNTopJet::passes(const Event & event){

  bool pass = false;
  pass = event.gentopjets->size() >= n_min && (event.gentopjets->size() <= n_max || n_max < 0);

  return pass;
}

TTbarSemilep::TTbarSemilep(uhh2::Context& ctx):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool TTbarSemilep::passes(const uhh2::Event& event){

  const auto & ttbargen = event.get(h_ttbargen);
  bool semilep = false;

  if(ttbargen.DecayChannel() == TTbarGen::e_muhad){
    semilep = true;
  }
  return semilep;
}

MassSelection::MassSelection(uhh2::Context& ctx, int n_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), n(n_){}
bool MassSelection::passes(const Event & event){

  bool pass = false;
  switch (n) {
    case 0:
    if(event.is_valid(h_ttbargen)){
      if(event.gentopjets->size() > 1){
        double mass, mass_lep;
        sort_by_pt<GenTopJet>(*event.gentopjets);
        mass = event.gentopjets->at(0).v4().M();
        mass_lep = event.gentopjets->at(1).v4().M();
        if(mass > (mass_lep)) pass = true;
      }
    }
    break;

    case 1:
    if(event.is_valid(h_ttbargen)){
      const auto ttbargen = event.get(h_ttbargen);
      if(event.gentopjets->size() > 1 && ttbargen.IsSemiLeptonicDecay()){
        double mass;
        sort_by_pt<GenTopJet>(*event.gentopjets);
        mass = event.gentopjets->at(0).v4().M();
        const auto dummy = event.gentopjets->at(1).v4() + ttbargen.ChargedLepton().v4();
        if(mass > (dummy.M())) pass = true;
      }
    }
    break;

    case 2:
    if(event.is_valid(h_ttbargen)){
      const auto ttbargen = event.get(h_ttbargen);
      if(event.gentopjets->size() > 1 && ttbargen.IsSemiLeptonicDecay()){
        double mass;
        sort_by_pt<GenTopJet>(*event.gentopjets);
        mass = event.gentopjets->at(0).v4().M();
        const auto dummy = event.gentopjets->at(1).v4() + ttbargen.ChargedLepton().v4();
        double diff = mass - dummy.M();
        if(diff > -50) pass = true;
      }
    }
    break;
  }
  return pass;
}

PtSelection::PtSelection(double pt_jet1_min_, double pt_jet2_min_):pt_jet1_min(pt_jet1_min_), pt_jet2_min(pt_jet2_min_){}
bool PtSelection::passes(const Event & event){
  bool pass = false;
  if(event.gentopjets->size() >= 2){
    pass = (event.gentopjets->at(0).pt() > pt_jet1_min || pt_jet1_min < 0) && (event.gentopjets->at(1).pt() > pt_jet2_min || pt_jet2_min < 0);
}
  return pass;
}

dRSelection::dRSelection(uhh2::Context& ctx, double dr_min_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), dr_min(dr_min_){}
bool dRSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_ttbargen)){
    const auto ttbargen = event.get(h_ttbargen);
    if(!ttbargen.IsSemiLeptonicDecay()) return false;
    if(event.gentopjets->size() > 1){
      pass = deltaR(event.gentopjets->at(1), ttbargen.ChargedLepton()) < dr_min;
    }
  }
  return pass;
}

GenMuonPtSelection::GenMuonPtSelection(uhh2::Context& ctx, double pt_min_, double pt_max_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), pt_min(pt_min_), pt_max(pt_max_){}
bool GenMuonPtSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_ttbargen)){
    const auto ttbargen = event.get(h_ttbargen);
    if((ttbargen.ChargedLepton().pt() < pt_max || pt_max < 0) && ttbargen.ChargedLepton().pt() > pt_min) pass = true;
  }
  return pass;
}

GenMETSelection::GenMETSelection(uhh2::Context& ctx, double met_min_, double met_max_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), met_min(met_min_), met_max(met_max_){}
bool GenMETSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_ttbargen)){
    const auto ttbargen = event.get(h_ttbargen);
    if((ttbargen.Neutrino().pt() < met_max || met_max < 0) && ttbargen.Neutrino().pt() > met_min) pass = true;
  }
  return pass;
}
