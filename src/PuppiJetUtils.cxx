#include "UHH2/TopSubstructure/include/PuppiJetUtils.h"
#include "UHH2/core/include/Event.h"

#include <stdexcept>

using namespace uhh2examples;
using namespace uhh2;


RecPuppiJetLeptonCleaner::RecPuppiJetLeptonCleaner(uhh2::Context & ctx):h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), h_topjet_cand(ctx.declare_event_output<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")){}
bool RecPuppiJetLeptonCleaner::process(uhh2::Event& event){
  sort_by_pt<Muon>(*event.muons);
  std::vector<TopJet> topjets;
  if(event.is_valid(h_puppi)) topjets = event.get(h_puppi);

  if(event.muons->size() > 0 && topjets.size() > 0){
    for (unsigned int i = 0; i < topjets.size(); i++){
      if (deltaR(event.muons->at(0), topjets.at(i)) < 0.8){
        topjets.at(i).set_v4(topjets.at(i).v4() - event.muons->at(0).v4());
      }
    }
  }
  sort_by_pt<TopJet>(topjets);
  event.set(h_topjet_cand, topjets);
  return true;
}

PuppiJetPtSelection::PuppiJetPtSelection(uhh2::Context& ctx, double pt_min_, double pt_max_):h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), pt_min(pt_min_), pt_max(pt_max_){}
bool PuppiJetPtSelection::passes(const Event & event){

  bool pass = false;
  std::vector<TopJet> topjets = event.get(h_puppi);

  if(topjets.size() <= 0){
    std::cout << "\n DPhiSelection::passes: There are no topjets in the event. returning 'false'\n" << std::endl;
    return false;
  }

  for(int i=1; i<fabs(topjets.size()); i++){
    double diff = topjets.at(0).pt()-topjets.at(i).pt();
    if(!(diff > 0)) std::cout << "\n In PuppiSelection::passes: event.topjets did not get sorted corretly by pt!\n" << std::endl;;
  }
  double pt = topjets.at(0).pt();

  pass = pt >= pt_min && (pt <= pt_max || pt_max < 0);
  return pass;
}

NPuppiJet::NPuppiJet(uhh2::Context & ctx, double n_min_, double n_max_):h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), n_min(n_min_), n_max(n_max_){}
bool NPuppiJet::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_puppi)){
    std::vector<TopJet> topjet_cand = event.get(h_puppi);
    pass = topjet_cand.size() >= n_min && (topjet_cand.size() <= n_max || n_max < 0);
  }
  return pass;
}

PuppiMassCompare::PuppiMassCompare(uhh2::Context & ctx, int n_):h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), n(n_){}
bool PuppiMassCompare::passes(const Event & event){

  bool pass = false;
  if(event.is_valid(h_puppi)){
    std::vector<TopJet> topjet_cand = event.get(h_puppi);

    switch(n){
      case 0:
      if(topjet_cand.size() > 1 && event.muons->size() > 0){
        double mass;
        mass = topjet_cand.at(0).v4().M();
        const auto dummy = topjet_cand.at(1).v4() + event.muons->at(0).v4();
        if(mass > (dummy.M())) pass = true;
      }
      break;

      case 1:
      if(topjet_cand.size() > 1 && event.muons->size() > 0){
        LorentzVector subjet_sum1;
        for (const auto s : topjet_cand.at(0).subjets()) {
          subjet_sum1 += s.v4();
        }
        double mass1 = subjet_sum1.M();

        LorentzVector subjet_sum2;
        for (const auto s : topjet_cand.at(1).subjets()) {
          subjet_sum2 += s.v4();
        }
        subjet_sum2 += event.muons->at(0).v4();
        double mass2 = subjet_sum2.M();
        if(mass1 > mass2) pass = true;
      }
      break;
    }
  }
  return pass;
}

PuppidRSelection::PuppidRSelection(uhh2::Context & ctx, double dr_min_, int mode_) : h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), dr_min(dr_min_), mode(mode_){}
bool PuppidRSelection::passes(const Event & event){
  bool pass = false;
  if(!event.is_valid(h_puppi)) return pass;
  std::vector<TopJet> topjet = event.get(h_puppi);

  if(!(event.muons->size() > 0)) return pass;
  if(!(topjet.size() > 1)) return pass;
  switch (mode) {
    case 0:
    pass = deltaR(topjet.at(1), event.muons->at(0)) < dr_min;
    break;

    case 1:
    LorentzVector subjet_sum2;
    for (const auto s : topjet.at(1).subjets()) {
      subjet_sum2 += s.v4();
    }
    pass = deltaR(subjet_sum2, event.muons->at(0)) < dr_min;
    break;
  }
  return pass;
}

PuppiPtSelection::PuppiPtSelection(uhh2::Context & ctx, double pt_jet1_min_, double pt_jet2_min_, int mode_): h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), pt_jet1_min(pt_jet1_min_), pt_jet2_min(pt_jet2_min_), mode(mode_){}
bool PuppiPtSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_puppi)){
    std::vector<TopJet> topjet_cand = event.get(h_puppi);
    switch (mode) {
      case 0:
      if(topjet_cand.size() > 0){
        pass = (topjet_cand.at(0).pt() > pt_jet1_min || pt_jet1_min < 0);
      }
      if(topjet_cand.size() > 1){
        pass = (topjet_cand.at(0).pt() > pt_jet1_min || pt_jet1_min < 0) && (topjet_cand.at(1).pt() > pt_jet2_min || pt_jet2_min < 0);
      }
      break;

      case 1:
      double pt1 = 0;
      if(topjet_cand.size() > 0){
        LorentzVector subjet_sum1;
        for (const auto s : topjet_cand.at(0).subjets()) {
          subjet_sum1 += s.v4();
        }
        pt1 = subjet_sum1.pt();

        pass = (pt1 > pt_jet1_min || pt_jet1_min < 0);
      }
      if(topjet_cand.size() > 1){
        LorentzVector subjet_sum2;
        for (const auto s : topjet_cand.at(1).subjets()) {
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

MassPuppiJetSelection::MassPuppiJetSelection(uhh2::Context & ctx, int n_, double mass_min_, double mass_max_): h_puppi(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi")), n(n_), mass_min(mass_min_), mass_max(mass_max_){}
bool MassPuppiJetSelection::passes(const Event & event){
  bool pass = false;
  if(event.is_valid(h_puppi)){
    std::vector<TopJet> topjet_cand = event.get(h_puppi);

    switch(n){
      case 0:
      if(topjet_cand.size() > 0){
        double mass_topjet = topjet_cand.at(0).v4().M();
        if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
      }
      break;

      case 1:
      if(topjet_cand.size() > 0){
        LorentzVector subjet_sum1;
        for (const auto s : topjet_cand.at(0).subjets()) {
          subjet_sum1 += s.v4();
        }
        double mass_topjet = subjet_sum1.M();
        if(mass_topjet >= mass_min && (mass_topjet < mass_max || mass_max < 0)) pass = true;
      }
      break;
    }
  }
  return pass;
}
