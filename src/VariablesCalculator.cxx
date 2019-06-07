#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

double calc_Nsubjettiness::tau_one(uhh2::Event & event){
  std::cout << "Hello World from calc_Nsubjettiness!" << '\n';
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }

    cout << "number of daughters of the first topjet: " << event.topjets->at(0).numberOfDaughters() << '\n';
    cout << "number of particles with pfcand_index: " << particles.size() << '\n';

    std::vector<PseudoJet> psj;
    int user_index = 0;

    for(const auto & cand: particles){
      PseudoJet temp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      temp_psj.set_user_index(user_index);
      psj.push_back(temp_psj);
      user_index++;
    }

    JetDefinition jet_def(antikt_algorithm, 0.8);
    ClusterSequence cs(psj, jet_def);
    vector<PseudoJet> akt_jets = cs.inclusive_jets();
    vector<PseudoJet> test = cs.constituents(akt_jets.at(0));
    cout << "number of constituents of new jet: " << test.size() << '\n';

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found

    Nsubjettiness nSub1(1, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8));
    tau = nSub1(akt_jets.at(0));
    cout << "Own calculated tau1: " << tau1 << '\n';
    cout << "Saved tau1:          " << event.topjets->at(0).tau1() << '\n';
  }
  else {
    cout << "There are no TopJets in this event!" << '\n';
  }

  return tau;
}
