#include <UHH2/TopSubstructure/include/TopSubstructureUtils.h>

using namespace uhh2;
using namespace std;

TopJetSortDPhi::TopJetSortDPhi(uhh2::Context & ctx): h_topjet_cand(ctx.declare_event_output<std::vector<TopJet>>("topjet_cand")){}
bool TopJetSortDPhi::process(uhh2::Event& event){

  sort_by_pt<Muon>(*event.muons);
  std::vector<TopJet> candidates;

  double diff = 999999;
  int pos = -1;

  if(event.topjets->size() > 0 && event.muons->size() > 0){
    for(unsigned int i=0; i<event.topjets->size(); i++){
      if(event.topjets->at(i).pt() > 400) candidates.push_back(event.topjets->at(i));
    }
    for(unsigned int i=0; i < candidates.size(); i++){
      double res = fabs(M_PI - deltaPhi(candidates.at(i), event.muons->at(0)));
      if(res<diff){
      	diff = res;
      	pos = i;
      }
    }
    if(pos >= 0) std::swap(candidates.at(0), candidates.at(pos));
  }
  event.set(h_topjet_cand, candidates);

  return true;
}

TopJetSortMass::TopJetSortMass(uhh2::Context & ctx): h_topjet_cand(ctx.declare_event_output<std::vector<TopJet>>("topjet_cand")){}
bool TopJetSortMass::process(uhh2::Event& event){

  std::vector<TopJet> candidates;

  if(event.topjets->size() > 0){
    for(unsigned int i=0; i<event.topjets->size(); i++){
      if(event.topjets->at(i).pt() > 400) candidates.push_back(event.topjets->at(i));
    }
    for(unsigned int i=1; i < candidates.size(); i++){
      if(candidates.at(0).v4().M() < candidates.at(i).v4().M()){
        std::swap(candidates.at(0), candidates.at(i));
      }
    }
  }
  event.set(h_topjet_cand, candidates);

  return true;
}

RecTopJetLeptonCleaner::RecTopJetLeptonCleaner() {}
bool RecTopJetLeptonCleaner::process(uhh2::Event& event){

  sort_by_pt<Muon>(*event.muons);

  if(event.muons->size() > 0 && event.topjets->size() > 0){
    for (unsigned int i = 0; i < event.topjets->size(); i++){
      if (deltaR(event.muons->at(0), event.topjets->at(i)) < 0.8){
        event.topjets->at(i).set_v4(event.topjets->at(i).v4() - event.muons->at(0).v4());
      }
    }
  }
  sort_by_pt<TopJet>(*event.topjets);
  return true;
}


int NumberJets(uhh2::Event& event){
  std::vector<TopJet>* topjets = event.topjets;
  std::vector<Jet>* jets = event.jets;
  int number = jets->size();

  for(unsigned int i = 0; i < jets->size(); i++){
    for(unsigned int j = 0; j < topjets->size(); j++){
      if(deltaR(jets->at(i), topjets->at(j)) < 0.8) number--;
    }
  }
  return number;
}



/*
█   ██████  ███████ ███    ██               ██    ██ ████████ ██ ██      ███████  █
█  ██       ██      ████   ██               ██    ██    ██    ██ ██      ██       █
█  ██   ███ █████   ██ ██  ██     █████     ██    ██    ██    ██ ██      ███████  █
█  ██    ██ ██      ██  ██ ██               ██    ██    ██    ██ ██           ██  █
█   ██████  ███████ ██   ████                ██████     ██    ██ ███████ ███████  █
*/


GenParticleRemover::GenParticleRemover(Context & ctx, string const & label_):hndl(ctx.get_handle<vector<GenParticle>>(label_)){}
bool GenParticleRemover::process(uhh2::Event & event){
    if (!event.is_valid(hndl)) {
        cerr << "In GenParticleRemover: Handle not valid!\n";
        assert(false);
    }
    if(event.genparticles->size() > 0){
      vector<GenParticle> genparticle_new;

      event.set(hndl, genparticle_new);
    }
    return true;
}

PFParticleRemover::PFParticleRemover(Context & ctx, string const & label_):hndl(ctx.get_handle<vector<PFParticle>>(label_)){}
bool PFParticleRemover::process(uhh2::Event & event){
    if (!event.is_valid(hndl)) {
        cerr << "In PFParticleRemover: Handle not valid!\n";
        assert(false);
    }
    if(event.pfparticles->size() > 0){
      vector<PFParticle> pfparticle_new;

      event.set(hndl, pfparticle_new);
    }
    return true;
}



GenTopJetLeptonCleaner::GenTopJetLeptonCleaner(uhh2::Context & ctx): h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")){}
bool GenTopJetLeptonCleaner::process(uhh2::Event& event){

  if(event.is_valid(h_ttbargen) && event.gentopjets->size() > 0){
    const auto & ttbargen = event.get(h_ttbargen);
    for (unsigned int j = 0; j < event.gentopjets->size(); j++) {
      if (deltaR(ttbargen.ChargedLepton(), event.gentopjets->at(j)) < 0.8) {
        event.gentopjets->at(j).set_v4(event.gentopjets->at(j).v4() - ttbargen.ChargedLepton().v4());
      }
    }
  }
  sort_by_pt<GenTopJet> (*event.gentopjets);
  return true;
}

GenTopJetCleaner::GenTopJetCleaner(Context & ctx, string const & label_, double pt_min_, double eta_max_):hndl(ctx.get_handle<vector<GenTopJet>>(label_)), pt_min(pt_min_), eta_max(eta_max_){}
bool GenTopJetCleaner::process(uhh2::Event & event){
    if (!event.is_valid(hndl)) {
        cerr << "In GenTopJetCleaner: Handle not valid!\n";
        assert(false);
    }
    if(event.gentopjets->size() > 0){
      vector<GenTopJet> & gentopjet_old = event.get(hndl);
      vector<GenTopJet> gentopjet_new;

      for (unsigned int i = 0; i < gentopjet_old.size(); i++) {
        if((gentopjet_old.at(i).pt() > pt_min) && (gentopjet_old.at(i).eta() < eta_max)){
          gentopjet_new.push_back(gentopjet_old.at(i));
        }
      }

      event.set(hndl, gentopjet_new);
    }
    return true;
}

GenTopJetSortDPhi::GenTopJetSortDPhi(uhh2::Context & ctx):h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")){}
bool GenTopJetSortDPhi::process(uhh2::Event& event){

  sort_by_pt<Muon>(*event.muons);
  std::vector<GenTopJet> candidates;

  double diff = 999999;
  int pos = -1;

  if(event.gentopjets->size() > 0 && event.muons->size() > 0){
    for(unsigned int i=0; i<event.gentopjets->size(); i++){
      if(event.gentopjets->at(i).pt() > 400) candidates.push_back(event.gentopjets->at(i));
    }
    for(unsigned int i=0; i < candidates.size(); i++){
      double res = fabs(M_PI - deltaPhi(candidates.at(i), event.muons->at(0)));
      if(res<diff){
	      diff = res;
	      pos = i;
      }
    }
    if(pos >= 0) std::swap(candidates.at(0), candidates.at(pos));
  }
  event.set(h_gentopjet_cand, candidates);

  return true;
}

GenTopJetSortMass::GenTopJetSortMass(uhh2::Context & ctx):h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")){}
bool GenTopJetSortMass::process(uhh2::Event& event){

  std::vector<GenTopJet> candidates;

  if(event.gentopjets->size() > 0){
    for(unsigned int i=0; i<event.gentopjets->size(); i++){
      if(event.gentopjets->at(i).pt() > 400) candidates.push_back(event.gentopjets->at(i));
    }
    for(unsigned int i=1; i < candidates.size(); i++){
      if(candidates.at(0).v4().M() < candidates.at(i).v4().M()){
        std::swap(candidates.at(0), candidates.at(i));
      }
    }
  }
  event.set(h_gentopjet_cand, candidates);

  return true;
}

GenTopJetSortPt::GenTopJetSortPt(uhh2::Context & ctx):h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")){}
bool GenTopJetSortPt::process(uhh2::Event& event){

  std::vector<GenTopJet> candidates;

  for(unsigned int i=0; i < event.gentopjets->size(); i++){
    candidates.push_back(event.gentopjets->at(i));
  }

  if(candidates.size() > 0){
    for(unsigned int i=1; i < candidates.size(); i++){
      if(candidates.at(0).pt() < candidates.at(i).pt()){
        std::swap(candidates.at(0), candidates.at(i));
      }
    }
  }
  event.set(h_gentopjet_cand, candidates);

  return true;
}

GenTopJetSortDPhiMass::GenTopJetSortDPhiMass(uhh2::Context & ctx, double dphi_max_):h_gentopjet_lep_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_lep_cand")), h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), dphi_max(dphi_max_){}
bool GenTopJetSortDPhiMass::process(uhh2::Event& event){

  std::vector<GenTopJet> candidates, candidates_lep;

  if(event.is_valid(h_ttbargen)){
    const auto & ttbargen = event.get(h_ttbargen);

    if(event.gentopjets->size() > 0){
      for(unsigned int i=0; i<event.gentopjets->size(); i++){
      	if(event.gentopjets->at(i).pt() > 400 && (deltaPhi(event.gentopjets->at(i), ttbargen.ChargedLepton()) > dphi_max || dphi_max < 0)){
          candidates.push_back(event.gentopjets->at(i));
        }
      	else if(event.gentopjets->at(i).pt() > 200 && deltaPhi(event.gentopjets->at(i), ttbargen.ChargedLepton()) < dphi_max){
          candidates_lep.push_back(event.gentopjets->at(i));
        }
      }
      for(unsigned int i=1; i < candidates.size(); i++){
      	if(candidates.at(0).v4().M() < candidates.at(i).v4().M()){
      	  std::swap(candidates.at(0), candidates.at(i));
      	}
      }
      for(unsigned int i=1; i < candidates_lep.size(); i++){
      	if(candidates_lep.at(0).v4().M() < candidates_lep.at(i).v4().M()){
      	  std::swap(candidates_lep.at(0), candidates_lep.at(i));
      	}
      }
    }
  }
  event.set(h_gentopjet_cand, candidates);
  event.set(h_gentopjet_lep_cand, candidates_lep);

  return true;
}

GenTopJetSortDPhiPt::GenTopJetSortDPhiPt(uhh2::Context & ctx, double dphi_max_):h_gentopjet_lep_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_lep_cand")), h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")), h_ttbargen(ctx.get_handle<TTbarGen>("ttbargen")), dphi_max(dphi_max_){}
bool GenTopJetSortDPhiPt::process(uhh2::Event& event){

  std::vector<GenTopJet> candidates, candidates_lep, dummy;

  if(event.is_valid(h_ttbargen)){
    const auto & ttbargen = event.get(h_ttbargen);
    if(event.gentopjets->size() > 0){
      for(unsigned int i = 0; i < event.gentopjets->size(); i++){
        dummy.push_back(event.gentopjets->at(i));
      }

      for(unsigned int i=1; i < dummy.size(); i++){
        if(dummy.at(0).pt() < dummy.at(i).pt()) std::swap(dummy.at(0), dummy.at(i));
      }
      if(dummy.size() > 2) dummy.erase(dummy.begin() + 2, dummy.begin() + dummy.size());
      GenParticle muon = ttbargen.ChargedLepton();

      if(deltaPhi(muon, dummy.at(0)) > ((2./3.)*M_PI)){
        candidates.push_back(dummy.at(0));
        if(dummy.size() >= 2) candidates_lep.push_back(dummy.at(1));
      }
      else{
        if(dummy.size() >= 2) candidates.push_back(dummy.at(1));
        candidates_lep.push_back(dummy.at(0));
      }
    }
  }
  event.set(h_gentopjet_cand, candidates);
  event.set(h_gentopjet_lep_cand, candidates_lep);

  return true;
}


int NumberGenJets(uhh2::Event& event){
  std::vector<GenTopJet>* topjets = event.gentopjets;
  std::vector<Particle>* jets = event.genjets;
  int number = jets->size();

  for(unsigned int i = 0; i < jets->size(); i++){
    for(unsigned int j = 0; j < topjets->size(); j++){
      if(deltaR(jets->at(i), topjets->at(j)) < 0.8) number--;
    }
  }
  return number;
}
