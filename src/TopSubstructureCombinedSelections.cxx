#include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;

RecPtSelection::RecPtSelection(double pt_jet1_min_, double pt_jet2_min_, int mode_):pt_jet1_min(pt_jet1_min_), pt_jet2_min(pt_jet2_min_), mode(mode_){}
bool RecPtSelection::passes(const Event & event){
  bool pass = false;
  switch (mode) {
    case 0:
    if(event.topjets->size() > 0){
      pass = (event.topjets->at(0).pt() > pt_jet1_min || pt_jet1_min < 0);
    }
    if(event.topjets->size() > 1){
      pass = (event.topjets->at(0).pt() > pt_jet1_min || pt_jet1_min < 0) && (event.topjets->at(1).pt() > pt_jet2_min || pt_jet2_min < 0);
    }
    break;

    case 1:
    double pt1 = 0;
    if(event.topjets->size() > 0){
      LorentzVector subjet_sum1;
      for (const auto s : event.topjets->at(0).subjets()) {
        subjet_sum1 += s.v4();
      }
      pt1 = subjet_sum1.pt();
      pass = (pt1 > pt_jet1_min || pt_jet1_min < 0);
    }
    if(event.topjets->size() > 1){
      LorentzVector subjet_sum2;
      for (const auto s : event.topjets->at(1).subjets()) {
        subjet_sum2 += s.v4();
      }
      double pt2 = subjet_sum2.pt();
      pass = (pt1 > pt_jet1_min || pt_jet1_min < 0) && (pt2 > pt_jet2_min || pt_jet2_min < 0);
    }
    break;
  }
  return pass;
}


RecMassCompare::RecMassCompare(int n_): n(n_){}
bool RecMassCompare::passes(const Event & event){

  bool pass = false;
  switch(n){
    case 0:
    if(event.topjets->size() > 1 && event.muons->size() > 0){
      double mass;
      mass = event.topjets->at(0).v4().M();
      const auto dummy = event.topjets->at(1).v4() + event.muons->at(0).v4();
      if(mass > (dummy.M())) pass = true;
    }
    break;

    case 1:
    if(event.topjets->size() > 1 && event.muons->size() > 0){
      LorentzVector subjet_sum1;
      for (const auto s : event.topjets->at(0).subjets()) {
        subjet_sum1 += s.v4();
      }
      double mass1 = subjet_sum1.M();

      LorentzVector subjet_sum2;
      for (const auto s : event.topjets->at(1).subjets()) {
        subjet_sum2 += s.v4();
      }
      subjet_sum2 += event.muons->at(0).v4();
      double mass2 = subjet_sum2.M();
      if(mass1 > mass2) pass = true;
    }
    break;
  }
  return pass;
}

RecdRSelection::RecdRSelection(double dr_min_, int mode_):dr_min(dr_min_), mode(mode_){}
bool RecdRSelection::passes(const Event & event){
  bool pass = false;
  if(!(event.muons->size() > 0)) return pass;
  if(!(event.topjets->size() > 1)) return pass;
  switch (mode) {
    case 0:
    pass = deltaR(event.topjets->at(1), event.muons->at(0)) < dr_min;
    break;

    case 1:
    LorentzVector subjet_sum2;
    for (const auto s : event.topjets->at(1).subjets()) {
      subjet_sum2 += s.v4();
    }
    pass = deltaR(subjet_sum2, event.muons->at(0)) < dr_min;
    break;
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

GenMatching::GenMatching(uhh2::Context & ctx, std::string const & label_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)){}
bool GenMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_ttbargen) && event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);
    const auto ttbargen = event.get(h_ttbargen);
    if(ttbargen.DecayChannel() == TTbarGen::e_muhad){
      if(gentopjets.size() > 0) {

        GenParticle bq, q1, q2;
        if(ttbargen.IsTopHadronicDecay()){
          bq = ttbargen.bTop();
          q1 = ttbargen.Wdecay1();
          q2 = ttbargen.Wdecay2();
          pass = (deltaR(bq, gentopjets.at(0)) <= 0.8 && deltaR(q1, gentopjets.at(0)) <= 0.8 && deltaR(q2, gentopjets.at(0)) <= 0.8);
        }
        else if(ttbargen.IsAntiTopHadronicDecay()){
          bq = ttbargen.bAntitop();
          q1 = ttbargen.WMinusdecay1();
          q2 = ttbargen.WMinusdecay2();
          pass = (deltaR(bq, gentopjets.at(0)) <= 0.8 && deltaR(q1, gentopjets.at(0)) <= 0.8 && deltaR(q2, gentopjets.at(0)) <= 0.8);
        }
      }
    }
  }
  return pass;
}
