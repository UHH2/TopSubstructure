#include <UHH2/TopSubstructure/include/TopSubstructureUtils.h>

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

GenTopJetSortDPhi::GenTopJetSortDPhi(uhh2::Context & ctx): h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")){}
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

GenTopJetSortMass::GenTopJetSortMass(uhh2::Context & ctx): h_gentopjet_cand(ctx.declare_event_output<std::vector<GenTopJet>>("gentopjet_cand")){}
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

JetSelection::JetSelection(uhh2::Context & ctx):h_jetsel(ctx.declare_event_output<std::vector<Jet>>("jetsel")), h_topjet_cand(ctx.get_handle<std::vector<TopJet>>("topjet_cand")){}
bool JetSelection::process(uhh2::Event& event){
  std::vector<Jet> own_jet;

  if(event.is_valid(h_topjet_cand)){
    std::vector<TopJet> topjet_cand = event.get(h_topjet_cand);
    if(event.jets->size() > 0 && topjet_cand.size() > 0){
      for(unsigned int i=0; i < event.jets->size(); i++){
	double dr=9999999;
	double diff = 0;
	for(unsigned int j=0; j < topjet_cand.size(); j++){
	  diff = deltaR(event.jets->at(i), topjet_cand.at(j));
	  if(diff < dr) dr = diff;
	}
	if(dr > 0.8) own_jet.push_back(event.jets->at(i));
      }
    }
  }
  event.set(h_jetsel, own_jet);
  return true;
}
