#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

double calc_Nsubjettiness::tau_one(uhh2::Event & event){
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    // recreate the first topjet
    JetDefinition jet_def(antikt_algorithm, 0.8);
    ClusterSequence cs(pfpart_psj, jet_def);
    vector<PseudoJet> antikt_jets = cs.inclusive_jets();
    // cs.constituents(akt_jets.at(0)) -- get the constituents which made the new jet

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found

    // calculate Nsubjettiness
    Nsubjettiness nSub(1, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    tau = nSub(antikt_jets.at(0));
    cout << "calculated tau1: " << tau << '\n';
    cout << "save tau1:       " << event.topjets->at(0).tau1() << '\n';
  }

  return tau;
}

double calc_Nsubjettiness::tau_two(uhh2::Event & event){
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    // recreate the first topjet
    JetDefinition jet_def(antikt_algorithm, 0.8);
    ClusterSequence cs(pfpart_psj, jet_def);
    vector<PseudoJet> antikt_jets = cs.inclusive_jets();
    // cs.constituents(akt_jets.at(0)) -- get the constituents which made the new jet

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found

    // calculate Nsubjettiness
    Nsubjettiness nSub(2, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    tau = nSub(antikt_jets.at(0));
    cout << "calculated tau2: " << tau << '\n';
    cout << "save tau2:       " << event.topjets->at(0).tau2() << '\n';
  }

  return tau;
}


double calc_Nsubjettiness::tau_three(uhh2::Event & event){
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    // recreate the first topjet
    JetDefinition jet_def(antikt_algorithm, 0.8);
    ClusterSequence cs(pfpart_psj, jet_def);
    vector<PseudoJet> antikt_jets = cs.inclusive_jets();
    // cs.constituents(akt_jets.at(0)) -- get the constituents which made the new jet

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found

    // calculate Nsubjettiness
    Nsubjettiness nSub(3, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    tau = nSub(antikt_jets.at(0));
    cout << "calculated tau3: " << tau << '\n';
    cout << "save tau3:       " << event.topjets->at(0).tau3() << '\n';
  }

  return tau;
}
