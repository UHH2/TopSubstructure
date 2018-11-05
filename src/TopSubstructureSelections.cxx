#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


METSelection::METSelection(double MET_min_, double MET_max_):MET_min(MET_min_), MET_max(MET_max_){}
bool METSelection::passes(const Event & event){

  bool pass = true;
  auto met = event.met->pt();

  pass = met >= MET_min && (met <= MET_max || MET_max < 0);
  return pass;
}

MuonptSelection::MuonptSelection(double pt_min_, double pt_max_):pt_min(pt_min_), pt_max(pt_max_){}
bool MuonptSelection::passes(const Event & event){

  bool pass = true;
  sort_by_pt<Muon>(*event.muons);
  if(event.muons->size()){
    auto pt = event.muons->at(0).pt();

    pass = pt >= pt_min && (pt <= pt_max || pt_max < 0);
    return pass;
  }
  else{
    std::cout << "\n MuonSelection::passes: There are no muons in the event. returning 'false'\n" << std::endl;
    return false;
  }
}

TopJetptSelection::TopJetptSelection(uhh2::Context& ctx, double pt_min_, double pt_max_):h_topjet(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), pt_min(pt_min_), pt_max(pt_max_){}
bool TopJetptSelection::passes(const Event & event){

  bool pass = true;
  std::vector<TopJet> topjets = event.get(h_topjet);

  if(topjets.size() <= 0){
    std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  for(int i=1; i<fabs(topjets.size()); i++){
    auto diff = topjets.at(0).pt()-topjets.at(i).pt();
    if(!(diff > 0)) std::cout << "\n In TopJetSelection::passes: event.topjets did not get sorted corretly by pt!\n" << std::endl;;
  }
  auto pt = topjets.at(0).pt();

  pass = pt >= pt_min && (pt <= pt_max || pt_max < 0);
  return pass;
}

TopJetptSelectionold::TopJetptSelectionold(double pt_min_, double pt_max_):pt_min(pt_min_), pt_max(pt_max_){}
bool TopJetptSelectionold::passes(const Event & event){

  bool pass = true;
  sort_by_pt<TopJet> (*event.topjets);
  std::vector<TopJet>* topjets = event.topjets;

  if(topjets->size() <= 0){
    std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  for(int i=1; i<fabs(topjets->size()); i++){
    auto diff = topjets->at(0).pt()-topjets->at(i).pt();
    if(!(diff > 0)) std::cout << "\n In TopJetSelection::passes: event.topjets did not get sorted corretly by pt!\n" << std::endl;;
  }
  auto pt = topjets->at(0).pt();

  pass = pt >= pt_min && (pt <= pt_max || pt_max < 0);
  return pass;
}

TwoDCut::TwoDCut(double min_deltaR_, double min_pTrel_): min_deltaR(min_deltaR_), min_pTrel(min_pTrel_) {}
bool TwoDCut::passes(const Event & event){

  assert(event.muons && event.electrons && event.jets);
  if((event.muons->size()+event.electrons->size()) != 1){
    std::cout << "\n @@@ WARNING -- TwoDCut::passes -- unexpected number of muons+electrons in the event (!=1). returning 'false'\n";
    return false;
  }

  double drmin, ptrel;
  std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);

  return (drmin > min_deltaR) || (ptrel > min_pTrel);
}

DPhiSelection::DPhiSelection(uhh2::Context& ctx, double dphi_min_, double dphi_max_): h_topjet(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), dphi_min(dphi_min_), dphi_max(dphi_max_){}
bool DPhiSelection::passes(const Event & event){

  bool pass = false;

  std::vector<TopJet> topjets = event.get(h_topjet);

  if(topjets.size() <= 0){
    std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  sort_by_pt<Muon>(*event.muons);
  const auto & topjet = topjets.at(0);
  const auto & muon = event.muons->at(0);
  auto dphi = deltaPhi(topjet, muon);

  pass = dphi >= dphi_min && ( dphi <= dphi_max || dphi_max < 0);
  return pass;
}

DPhiSelectionold::DPhiSelectionold(double dphi_min_, double dphi_max_):dphi_min(dphi_min_), dphi_max(dphi_max_){}
bool DPhiSelectionold::passes(const Event & event){

  bool pass = false;

  sort_by_pt<TopJet> (*event.topjets);
  std::vector<TopJet>* topjets = event.topjets;

  if(topjets->size() <= 0){
    std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  sort_by_pt<Muon>(*event.muons);
  const auto & topjet = topjets->at(0);
  const auto & muon = event.muons->at(0);
  auto dphi = deltaPhi(topjet, muon);

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

QuarkGenJetMatching::QuarkGenJetMatching(uhh2::Context & ctx):h_gentopjet_cand(ctx.get_handle<std::vector<GenTopJet>>("gentopjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool QuarkGenJetMatching::passes(const uhh2::Event& event){
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

NTopJet::NTopJet(uhh2::Context & ctx, double n_min_, double n_max_):h_topjet_cand(ctx.get_handle<std::vector<TopJet>>("topjet_cand")), n_min(n_min_), n_max(n_max_){}
bool NTopJet::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand = event.get(h_topjet_cand);
    pass = topjet_cand.size() >= n_min && (topjet_cand.size() <= n_max || n_max < 0);
  }
  return pass;
}
