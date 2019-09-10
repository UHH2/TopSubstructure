#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


METSelection::METSelection(double MET_min_, double MET_max_):MET_min(MET_min_), MET_max(MET_max_){}
bool METSelection::passes(const Event & event){

  bool pass = false;
  double met = event.met->pt();

  pass = (met >= MET_min && (met <= MET_max || MET_max < 0));
  return pass;
}

LeptonPtSelection::LeptonPtSelection(int mode_, double pt_min_, double pt_max_):mode(mode_), pt_min(pt_min_), pt_max(pt_max_){}
bool LeptonPtSelection::passes(const Event & event){
  bool pass = false;
  switch (mode) {
    case 0:
    if(event.muons->size()){
      sort_by_pt<Muon>(*event.muons);
      double pt = event.muons->at(0).pt();

      pass = (pt >= pt_min && (pt <= pt_max || pt_max < 0));
    }
    else{
      // std::cout << "\n MuonSelection::passes: There are no muons in the event. returning 'false'\n" << std::endl;
      return false;
    }
    break;

    case 1:
    if(event.electrons->size()){
      sort_by_pt<Electron>(*event.electrons);
      double pt = event.electrons->at(0).pt();

      pass = (pt >= pt_min && (pt <= pt_max || pt_max < 0));
    }
    else{
      // std::cout << "\n MuonSelection::passes: There are no electrons in the event. returning 'false'\n" << std::endl;
      return false;
    }
    break;
  }
  return pass;
}

TopJetptSelection::TopJetptSelection(uhh2::Context& ctx, double pt_min_, double pt_max_):h_topjet(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), pt_min(pt_min_), pt_max(pt_max_){}
bool TopJetptSelection::passes(const Event & event){

  bool pass = false;
  std::vector<TopJet> topjets = event.get(h_topjet);

  if(topjets.size() <= 0){
    // std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  for(int i=1; i<fabs(topjets.size()); i++){
    double diff = topjets.at(0).pt()-topjets.at(i).pt();
    if(!(diff > 0)) std::cout << "\n In TopJetSelection::passes: event.topjets did not get sorted corretly by pt!\n" << std::endl;
  }
  double pt = topjets.at(0).pt();

  pass = pt >= pt_min && (pt <= pt_max || pt_max < 0);
  return pass;
}

TwoDCut::TwoDCut(int mode_, double min_deltaR_, double min_pTrel_): mode(mode_), min_deltaR(min_deltaR_), min_pTrel(min_pTrel_) {}
bool TwoDCut::passes(const Event & event){
  bool pass = false;
  double drmin, ptrel;

  switch (mode) {
    case 0:
    assert(event.muons && event.jets);
    if(event.muons->size() != 1){
      // std::cout << "\n @@@ WARNING -- TwoDCut::passes -- unexpected number of muons in the event (!=1). returning 'false'\n";
      return false;
    }

    std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
    pass = ((drmin > min_deltaR) || (ptrel > min_pTrel));
    break;

    case 1:
    assert(event.electrons && event.jets);
    if(event.electrons->size()!= 1){
      // std::cout << "\n @@@ WARNING -- TwoDCut::passes -- unexpected number of electrons in the event (!=1). returning 'false'\n";
      return false;
    }

    std::tie(drmin, ptrel) = drmin_pTrel(event.electrons->at(0), *event.jets);
    pass = ((drmin > min_deltaR) || (ptrel > min_pTrel));
    break;
  }

  return pass;
}

DPhiSelection::DPhiSelection(uhh2::Context& ctx, double dphi_min_, double dphi_max_): h_topjet(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), dphi_min(dphi_min_), dphi_max(dphi_max_){}
bool DPhiSelection::passes(const Event & event){

  bool pass = false;

  std::vector<TopJet> topjets = event.get(h_topjet);

  if(topjets.size() <= 0){
    // std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  sort_by_pt<Muon>(*event.muons);
  const auto & topjet = topjets.at(0);
  const auto & muon = event.muons->at(0);
  double dphi = deltaPhi(topjet, muon);

  pass = dphi >= dphi_min && ( dphi <= dphi_max || dphi_max < 0);
  return pass;
}

Matching::Matching(uhh2::Context & ctx):h_topjet_cand(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool Matching::passes(const uhh2::Event& event){

  bool pass = false;
  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand = event.get(h_topjet_cand);
    if(topjet_cand.size()>0){
      if(event.is_valid(h_ttbargen)){
        const auto & ttbargen = event.get(h_ttbargen);

        GenParticle tophad;
        if(ttbargen.IsTopHadronicDecay()){
          tophad = ttbargen.Top();
          if(deltaR(tophad, topjet_cand.at(0)) <= 0.8) pass = true;
        }
        else if(ttbargen.IsAntiTopHadronicDecay()){
          tophad = ttbargen.Antitop();
          if(deltaR(tophad, topjet_cand.at(0)) <= 0.8) pass = true;
        }
      }
    }
  }
  return pass;
}

QuarkCandJetMatching::QuarkCandJetMatching(uhh2::Context & ctx):h_topjet_cand(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool QuarkCandJetMatching::passes(const uhh2::Event& event){
  bool pass = false;

  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand = event.get(h_topjet_cand);
    if (topjet_cand.size() > 0) {
      if(event.is_valid(h_ttbargen)){
        const auto ttbargen = event.get(h_ttbargen);

        GenParticle bq, q1, q2;
        if(ttbargen.IsTopHadronicDecay()){
          bq = ttbargen.bTop();
          q1 = ttbargen.Wdecay1();
          q2 = ttbargen.Wdecay2();
          if(deltaR(bq, topjet_cand.at(0)) <= 0.8 && deltaR(q1, topjet_cand.at(0)) <= 0.8 && deltaR(q2, topjet_cand.at(0)) <= 0.8) pass = true;
        }
        else if(ttbargen.IsAntiTopHadronicDecay()){
          bq = ttbargen.bAntitop();
          q1 = ttbargen.WMinusdecay1();
          q2 = ttbargen.WMinusdecay2();
          if(deltaR(bq, topjet_cand.at(0)) <= 0.8 && deltaR(q1, topjet_cand.at(0)) <= 0.8 && deltaR(q2, topjet_cand.at(0)) <= 0.8) pass = true;
        }
      }
    }
  }
  return pass;
}

NTopJet::NTopJet(uhh2::Context & ctx, double n_min_, double n_max_):h_topjet_cand(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), n_min(n_min_), n_max(n_max_){}
bool NTopJet::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand = event.get(h_topjet_cand);
    pass = topjet_cand.size() >= n_min && (topjet_cand.size() <= n_max || n_max < 0);
  }
  return pass;
}

MassTopJetSelection::MassTopJetSelection(int n_, double mass_min_, double mass_max_): n(n_),mass_min(mass_min_), mass_max(mass_max_){}
bool MassTopJetSelection::passes(const Event & event){
  bool pass = false;

  switch(n){
    case 0:
    if(event.topjets->size() > 0){
      double mass_topjet = event.topjets->at(0).v4().M();
      if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
    }
    break;

    case 1:
    if(event.topjets->size() > 0){
      LorentzVector subjet_sum1;
      for (const auto s : event.topjets->at(0).subjets()) {
        subjet_sum1 += s.v4();
      }
      double mass_topjet = subjet_sum1.M();
      if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
    }
    break;
  }
  return pass;
}
