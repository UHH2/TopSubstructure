#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

double calc_Nsubjettiness::PF_tau1(uhh2::Event & event){
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }
    if(particles.size() == 0) return tau;

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
    // cout << "calculated tau1: " << tau << '\n';
    // cout << "save tau1:       " << event.topjets->at(0).tau1() << '\n';
  }

  return tau;
}

double calc_Nsubjettiness::PF_tau2(uhh2::Event & event){
  double tau = -100;

  if(event.topjets->size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      particles.push_back(event.pfparticles->at(candInd));
    }
    if(particles.size() == 0) return tau;

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

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????

    // calculate Nsubjettiness
    Nsubjettiness nSub(2, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    tau = nSub(antikt_jets.at(0));
    // cout << "calculated tau2: " << tau << '\n';
    // cout << "save tau2:       " << event.topjets->at(0).tau2() << '\n';
  }

  return tau;
}

double calc_Nsubjettiness::PF_tau3(uhh2::Event & event){
  cout << "test1" << '\n';
  double tau = -100;
  cout << "test2" << '\n';
  if(event.topjets->size() > 0){
    cout << "test3" << '\n';
    vector<PFParticle> particles;
    cout << "test4" << '\n';
    vector<PFParticle> particles_test2;
    cout << "test5" << '\n';
    vector<PFParticle> particles_test3;
    cout << "test6" << '\n';

    // get the PFParticles inside the first topjet
    for(const auto candInd : event.topjets->at(0).pfcand_indexs()){
      cout << "test blub" << '\n';
      particles.push_back(event.pfparticles->at(candInd));
    }
    cout << "test7" << '\n';
    if(particles.size() == 0) return tau;
    cout << "test8" << '\n';

    if(event.topjets->size() > 1){
      for(const auto candInd : event.topjets->at(1).pfcand_indexs()){
        particles_test2.push_back(event.pfparticles->at(candInd));
      }
    }
    if(event.topjets->size() > 2){
      for(const auto candInd : event.topjets->at(2).pfcand_indexs()){
        particles_test3.push_back(event.pfparticles->at(candInd));
      }
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
    cout << "topjets: " << event.topjets->size() << '\n';
    for(unsigned int i = 0; i < event.topjets->size(); i++){
      cout << "topjet " << i << " pt: " << event.topjets->at(i).v4().pt() << '\n';
    }
    cout << "particles inside topjet 0: " << particles.size() << '\n';

    LorentzVector subjet_sum;
    for (const auto s : event.topjets->at(0).subjets()) {
      subjet_sum += s.v4();
    }
    cout << "mass of topjet 0 (softdrop): " << subjet_sum.M() << '\n';
    cout << "mass of topjet 0 (.M()): " << event.topjets->at(0).v4().M() << '\n';

    int counter = 0;
    for(unsigned int i = 0; i < event.pfparticles->size(); i++){
      if(deltaR(event.pfparticles->at(i), event.topjets->at(0)) < 0.8) counter++;
    }
    cout << "number of PFParticles with dR < 0.8 (topjet 0): " << counter << '\n';

    if(event.topjets->size() > 1){
      cout << "particles inside topjet 1: " << particles_test2.size() << '\n';

      LorentzVector subjet_sum1;
      for (const auto s : event.topjets->at(1).subjets()) {
        subjet_sum1 += s.v4();
      }
      cout << "mass of topjet 1 (softdrop): " << subjet_sum1.M() << '\n';
      cout << "mass of topjet 1 (.M()): " << event.topjets->at(1).v4().M() << '\n';
    }

    if(event.topjets->size() > 2){
      cout << "particles inside topjet 2: " << particles_test3.size() << '\n';

      LorentzVector subjet_sum2;
      for (const auto s : event.topjets->at(2).subjets()) {
        subjet_sum2 += s.v4();
      }
      cout << "mass of topjet 2 (softdrop): " << subjet_sum2.M() << '\n';
      cout << "mass of topjet 2 (.M()): " << event.topjets->at(2).v4().M() << '\n';
    }
    cout << "number pfparticles: " << event.pfparticles->size() << '\n';
    cout << "particles converted to PseudoJets: " << pfpart_psj.size() << '\n';
    JetDefinition jet_def(antikt_algorithm, 0.8);
    ClusterSequence cs(pfpart_psj, jet_def);
    vector<PseudoJet> antikt_jets = cs.inclusive_jets();
    cout << "antikt_jets: " << antikt_jets.size() << '\n';
    // cs.constituents(akt_jets.at(0)) -- get the constituents which made the new jet

    SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found

    // calculate Nsubjettiness
    Nsubjettiness nSub(3, OnePass_WTA_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    tau = nSub(antikt_jets.at(0));
    // cout << "calculated tau3: " << tau << '\n';
    // cout << "save tau3:       " << event.topjets->at(0).tau3() << '\n';
  }

  return tau;
}


double calc_Nsubjettiness::Gen_tau1(uhh2::Event & event){
  double tau = -100;

  if(event.gentopjets->size() > 0){
    vector<GenParticle> particles;

    // get the GenParticles inside the first gentopjet
    for(const auto candInd : event.gentopjets->at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return tau;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert GenParticles to PseudoJets
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
    // cout << "calculated tau1: " << tau << '\n';
    // cout << "save tau1:       " << event.gentopjets->at(0).tau1() << '\n';
  }

  return tau;
}



double calc_Nsubjettiness::Gen_tau2(uhh2::Event & event){
  double tau = -100;

  if(event.gentopjets->size() > 0){
    vector<GenParticle> particles;

    // get the GenParticles inside the first gentopjet
    for(const auto candInd : event.gentopjets->at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return tau;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert GenParticles to PseudoJets
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
    // cout << "calculated tau2: " << tau << '\n';
    // cout << "save tau2:       " << event.gentopjets->at(0).tau2() << '\n';
  }

  return tau;
}



double calc_Nsubjettiness::Gen_tau3(uhh2::Event & event){
  double tau = -100;

  if(event.gentopjets->size() > 0){
    vector<GenParticle> particles;

    // get the GenParticles inside the first gentopjet
    for(const auto candInd : event.gentopjets->at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return tau;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert GenParticles to PseudoJets
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
    // cout << "calculated tau3: " << tau << '\n';
    // cout << "save tau3:       " << event.gentopjets->at(0).tau3() << '\n';
  }

  return tau;
}
