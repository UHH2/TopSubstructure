#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;

GenDPhiSelection::GenDPhiSelection(uhh2::Context& ctx, double dphi_min_, double dphi_max_): h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), dphi_min(dphi_min_), dphi_max(dphi_max_){}
bool GenDPhiSelection::passes(const Event & event){

  bool pass = false;

  if(event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);

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
      auto dphi = deltaPhi(gentopjet, gen.at(0));
      pass = dphi >= dphi_min && ( dphi <= dphi_max || dphi_max < 0);
  }

  return pass;
}

GenQuarkGenJetMatching::GenQuarkGenJetMatching(uhh2::Context & ctx):h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool GenQuarkGenJetMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_gentopjet_cand)){
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

GenNTopJet::GenNTopJet(uhh2::Context & ctx, double n_min_, double n_max_):h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), n_min(n_min_), n_max(n_max_){}
bool GenNTopJet::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_gentopjet_cand)){
    std::vector<GenTopJet> gentopjet_cand = event.get(h_gentopjet_cand);
    pass = gentopjet_cand.size() >= n_min && (gentopjet_cand.size() <= n_max || n_max < 0);
  }
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
