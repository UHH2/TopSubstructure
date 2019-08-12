#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;
using namespace std;

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

GenTopJetPtSelection::GenTopJetPtSelection(double pt_min_):pt_min(pt_min_){}
bool GenTopJetPtSelection::passes(const Event & event){

  bool pass = false;

  for(unsigned int i = 0; i < event.gentopjets->size(); i++){
    if(event.gentopjets->at(i).pt() > pt_min) pass = true;
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

GenNTopJet::GenNTopJet(uhh2::Context & ctx, double n_min_, double n_max_, string const & label_):h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)), n_min(n_min_), n_max(n_max_){}
bool GenNTopJet::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);
    pass = gentopjets.size() >= n_min && (gentopjets.size() <= n_max || n_max < 0);
  }

  return pass;
}

TTbarSemilep::TTbarSemilep(uhh2::Context& ctx, int mode_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), mode(mode_){}
bool TTbarSemilep::passes(const uhh2::Event& event){
  const auto & ttbargen = event.get(h_ttbargen);
  bool semilep = false;
  switch(mode){
    case 0:
      semilep = ttbargen.DecayChannel() == TTbarGen::e_muhad;
    break;

    case 1:
      semilep = ttbargen.DecayChannel() == TTbarGen::e_ehad;
    break;
  }
  return semilep;
}

GenMassCompare::GenMassCompare(uhh2::Context& ctx, int mode1_, int mode2_, string const & label_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)), mode1(mode1_), mode2(mode2_){}
bool GenMassCompare::passes(const Event & event){
  bool pass = false;
  // mode1:  0: w/o SoftDrop; 1: w/ SoftDrop
  // mode2:  0: muons         1: electron
  if(event.is_valid(h_gentopjet)){
    if(event.is_valid(h_ttbargen)){
      std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);
      const auto & ttbargen = event.get(h_ttbargen);
      if(gentopjets.size() > 1 && ttbargen.IsSemiLeptonicDecay()){

        switch(mode1){
          case 0:{
            double mass;
            mass = gentopjets.at(0).v4().M();
            LorentzVector dummy;
            switch (mode2){
              case 0:
              if(!(ttbargen.DecayChannel() == TTbarGen::e_muhad)) return pass;
              dummy = gentopjets.at(1).v4() + ttbargen.ChargedLepton().v4();
              break;
              case 1:
              if(!(ttbargen.DecayChannel() == TTbarGen::e_ehad)) return pass;
              dummy = gentopjets.at(1).v4() + ttbargen.ChargedLepton().v4();
              break;
            }
            if(mass > (dummy.M())) pass = true;
          }
          break;

          case 1:{
            LorentzVector subjet_sum1;
            for (const auto s : gentopjets.at(0).subjets()) {
              subjet_sum1 += s.v4();
            }
            double mass1 = subjet_sum1.M();

            LorentzVector subjet_sum2;
            for (const auto s : gentopjets.at(1).subjets()) {
              subjet_sum2 += s.v4();
            }
            switch (mode2) {
              case 0:
              if(!(ttbargen.DecayChannel() == TTbarGen::e_muhad)) return pass;
              subjet_sum2 += ttbargen.ChargedLepton().v4();
              break;
              case 1:
              if(!(ttbargen.DecayChannel() == TTbarGen::e_ehad)) return pass;
              subjet_sum2 += ttbargen.ChargedLepton().v4();
              break;
            }
            double mass2 = subjet_sum2.M();
            if(mass1 > mass2) pass = true;
          }
          break;
        }
      }
    }
  }
  return pass;
}

PtSelection::PtSelection(uhh2::Context& ctx, double pt_jet1_min_, double pt_jet2_min_, int mode_, string const & label_):h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)), pt_jet1_min(pt_jet1_min_), pt_jet2_min(pt_jet2_min_), mode(mode_){}
bool PtSelection::passes(const Event & event){
  bool pass = false;

  if(event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);

    switch (mode) {
      case 0:
      if(gentopjets.size() >= 2){
        pass = (gentopjets.at(0).pt() > pt_jet1_min || pt_jet1_min < 0) && (gentopjets.at(1).pt() > pt_jet2_min || pt_jet2_min < 0);
      }
      break;

      case 1:
      if(gentopjets.size() >= 2){
        LorentzVector subjet_sum1;
        for (const auto s : gentopjets.at(0).subjets()) {
          subjet_sum1 += s.v4();
        }
        double pt1 = subjet_sum1.pt();

        LorentzVector subjet_sum2;
        for (const auto s : gentopjets.at(1).subjets()) {
          subjet_sum2 += s.v4();
        }
        double pt2 = subjet_sum2.pt();
        pass = (pt1 > pt_jet1_min || pt_jet1_min < 0) && (pt2 > pt_jet2_min || pt_jet2_min < 0);
      }
      break;
    }
  }
  return pass;
}

dRSelection::dRSelection(uhh2::Context& ctx, double dr_min_, int mode1_, int mode2_, string const & label_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)), dr_min(dr_min_), mode1(mode1_), mode2(mode2_){}
bool dRSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_ttbargen) && event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);
    const auto ttbargen = event.get(h_ttbargen);
    if(!ttbargen.IsSemiLeptonicDecay()) return pass;
    if(gentopjets.size() <= 1) return pass;

    switch (mode1) {
      case 0:
      switch (mode2) {
        case 0:
        if(!(ttbargen.DecayChannel() == TTbarGen::e_muhad)) return pass;
        pass = deltaR(gentopjets.at(1), ttbargen.ChargedLepton()) < dr_min;
        break;
        case 1:
        if(!(ttbargen.DecayChannel() == TTbarGen::e_ehad)) return pass;
        pass = deltaR(gentopjets.at(1), ttbargen.ChargedLepton()) < dr_min;
        break;
      }
      break;

      case 1:
      LorentzVector subjet_sum2;
      for (const auto s : gentopjets.at(1).subjets()) {
        subjet_sum2 += s.v4();
      }
      switch (mode2) {
        case 0:
        if(!(ttbargen.DecayChannel() == TTbarGen::e_muhad)) return pass;
        pass = deltaR(subjet_sum2, ttbargen.ChargedLepton()) < dr_min;
        break;
        case 1:
        if(!(ttbargen.DecayChannel() == TTbarGen::e_ehad)) return pass;
        pass = deltaR(subjet_sum2, ttbargen.ChargedLepton()) < dr_min;
        break;
      }
      break;
    }
  }
  return pass;
}

GenLeptonPtSelection::GenLeptonPtSelection(uhh2::Context& ctx, int mode_, double pt_min_, double pt_max_):h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), mode(mode_), pt_min(pt_min_), pt_max(pt_max_){}
bool GenLeptonPtSelection::passes(const Event & event){
  bool pass = false;

  if(event.is_valid(h_ttbargen)){
    const auto ttbargen = event.get(h_ttbargen);
    switch(mode){
      case 0:
      if(ttbargen.DecayChannel() != TTbarGen::e_muhad) return false;
      pass = ((ttbargen.ChargedLepton().pt() < pt_max || pt_max < 0) && ttbargen.ChargedLepton().pt() > pt_min);

      break;
      case 1:
      if(ttbargen.DecayChannel() != TTbarGen::e_ehad) return false;
      pass = ((ttbargen.ChargedLepton().pt() < pt_max || pt_max < 0) && ttbargen.ChargedLepton().pt() > pt_min);

      break;
    }
  }
  return pass;
}

GenMassTopJet::GenMassTopJet(uhh2::Context& ctx, double mass_min_, double mass_max_, int mode_, string const & label_):h_gentopjet(ctx.get_handle<std::vector<GenTopJet>>(label_)), mass_min(mass_min_), mass_max(mass_max_), mode(mode_){}
bool GenMassTopJet::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_gentopjet)){
    std::vector<GenTopJet> gentopjets = event.get(h_gentopjet);
    switch (mode) {
      case 0:
      if(gentopjets.size() > 0){
        double mass_topjet = gentopjets.at(0).v4().M();
        if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
      }
      break;
      case 1:
      if(gentopjets.size() > 0){
        LorentzVector subjet_sum;
        for (const auto s : gentopjets.at(0).subjets()) {
          subjet_sum += s.v4();
        }
        double mass_topjet = subjet_sum.M();
        if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
      }
      break;
    }
  }
  return pass;
}
