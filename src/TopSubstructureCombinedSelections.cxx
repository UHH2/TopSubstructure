#include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;

RecPtSelection::RecPtSelection(double pt_jet1_min_, double pt_jet2_min_):pt_jet1_min(pt_jet1_min_), pt_jet2_min(pt_jet2_min_){}
bool RecPtSelection::passes(const Event & event){
  bool pass = false;
  if(event.topjets->size() > 0){
    pass = (event.topjets->at(0).pt() > pt_jet1_min || pt_jet1_min < 0);
  }
  if(event.topjets->size() > 1){
    pass = (event.topjets->at(0).pt() > pt_jet1_min || pt_jet1_min < 0) && (event.topjets->at(1).pt() > pt_jet2_min || pt_jet2_min < 0);
  }
  return pass;
}


RecMassSelection::RecMassSelection(int n_):n(n_){}
bool RecMassSelection::passes(const Event & event){

  bool pass = false;
  switch(n){

    case 0:
      if(event.topjets->size() > 1 && event.muons->size() > 0){
        double mass, mass_lep;
        mass = event.topjets->at(0).v4().M();
        mass_lep = event.topjets->at(1).v4().M();

        if(mass > mass_lep) pass = true;
      }
      break;

    case 1:
      if(event.topjets->size() > 1 && event.muons->size() > 0){
        double mass;
        mass = event.topjets->at(0).v4().M();
        const auto dummy = event.topjets->at(1).v4() + event.muons->at(0).v4();
        if(mass > (dummy.M())) pass = true;
      }
    break;

    case 2:
      if(event.topjets->size() > 1 && event.muons->size() > 0){
        double mass;
        mass = event.topjets->at(0).v4().M();
        const auto dummy = event.topjets->at(1).v4() + event.muons->at(0).v4();
        double diff = mass - dummy.M();
        if(diff > -50) pass = true;
      }
    break;
  }
  return pass;
}

RecdRSelection::RecdRSelection(double dr_min_):dr_min(dr_min_){}
bool RecdRSelection::passes(const Event & event){
  bool pass = false;
  if(event.topjets->size() > 1 && event.muons->size() > 0){
    pass = deltaR(event.topjets->at(1), event.muons->at(0)) < dr_min;
  }
  return pass;
}

RecMatching::RecMatching(uhh2::Context & ctx):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool RecMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_ttbargen)){
  const auto ttbargen = event.get(h_ttbargen);
    if(ttbargen.DecayChannel() == TTbarGen::e_muhad){
      if(event.topjets->size() > 0) {
        std::vector<TopJet> topjet_cand;
        topjet_cand.push_back(event.topjets->at(0));

        GenParticle bq, q1, q2;
        if(ttbargen.IsTopHadronicDecay()){
      	  bq = ttbargen.bTop();
      	  q1 = ttbargen.Wdecay1();
      	  q2 = ttbargen.Wdecay2();
      	  pass = (deltaR(bq, topjet_cand.at(0)) <= 0.8 && deltaR(q1, topjet_cand.at(0)) <= 0.8 && deltaR(q2, topjet_cand.at(0)) <= 0.8);
        }
        else if(ttbargen.IsAntiTopHadronicDecay()){
      	  bq = ttbargen.bAntitop();
      	  q1 = ttbargen.WMinusdecay1();
      	  q2 = ttbargen.WMinusdecay2();
          pass = (deltaR(bq, topjet_cand.at(0)) <= 0.8 && deltaR(q1, topjet_cand.at(0)) <= 0.8 && deltaR(q2, topjet_cand.at(0)) <= 0.8);
        }
      }
    }
  }
  return pass;
}

GenMatching::GenMatching(uhh2::Context & ctx):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool GenMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_ttbargen)){
    const auto ttbargen = event.get(h_ttbargen);
    if(ttbargen.DecayChannel() == TTbarGen::e_muhad){
      if(event.gentopjets->size() > 0) {
        std::vector<GenTopJet> gentopjet_cand;
        gentopjet_cand.push_back(event.gentopjets->at(0));

        if (gentopjet_cand.size() > 0) {

          GenParticle bq, q1, q2;
          if(ttbargen.IsTopHadronicDecay()){
        	  bq = ttbargen.bTop();
        	  q1 = ttbargen.Wdecay1();
        	  q2 = ttbargen.Wdecay2();
        	  pass = (deltaR(bq, gentopjet_cand.at(0)) <= 0.8 && deltaR(q1, gentopjet_cand.at(0)) <= 0.8 && deltaR(q2, gentopjet_cand.at(0)) <= 0.8);
          }
          else if(ttbargen.IsAntiTopHadronicDecay()){
        	  bq = ttbargen.bAntitop();
        	  q1 = ttbargen.WMinusdecay1();
        	  q2 = ttbargen.WMinusdecay2();
            pass = (deltaR(bq, gentopjet_cand.at(0)) <= 0.8 && deltaR(q1, gentopjet_cand.at(0)) <= 0.8 && deltaR(q2, gentopjet_cand.at(0)) <= 0.8);
          }
        }
      }
    }
  }
  return pass;
}
