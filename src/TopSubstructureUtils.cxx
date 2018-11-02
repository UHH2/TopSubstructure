#include <UHH2/TopSubstructure/include/TopSubstructureUtils.h>

TopJetSort::TopJetSort(uhh2::Context & ctx): h_topjet_cand(ctx.declare_event_output<std::vector<TopJet>>("topjet_cand")){}
bool TopJetSort::process(uhh2::Event& event){

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
