#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

Calculator::Calculator(Context & ctx, string const & label_){
  label = label_;
  if(label.find("gen") != std::string::npos) hndl_gen = ctx.get_handle<vector<GenTopJet>>(label);
  else hndl = ctx.get_handle<vector<TopJet>>(label);
}

double Calculator::PF_tau1(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<TopJet> topjets = event.get(hndl);
  vector<PFParticle>* pfparticles = event.pfparticles;

  if (label.find("puppi_sd") != std::string::npos || label.find("puppi") != std::string::npos){
    for (const auto candInd : topjets.at(0).pfcand_indexs()){
      float puppiWeight = pfparticles->at(candInd).puppiWeight();
      pfparticles->at(candInd).set_v4(pfparticles->at(candInd).v4() * puppiWeight);
    }
  }

  if(topjets.size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : topjets.at(0).pfcand_indexs()){
      particles.push_back(pfparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(1, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    // cout << "calculated tau1: " << tau << '\n';
    // cout << "save tau1:       " << topjets.at(0).tau1() << '\n';
  }

  return tau;
}

double Calculator::PF_tau2(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<TopJet> topjets = event.get(hndl);
  vector<PFParticle>* pfparticles = event.pfparticles;

  if (label.find("puppi_sd") != std::string::npos || label.find("puppi") != std::string::npos){
    for (const auto candInd : topjets.at(0).pfcand_indexs()){
      float puppiWeight = pfparticles->at(candInd).puppiWeight();
      pfparticles->at(candInd).set_v4(pfparticles->at(candInd).v4() * puppiWeight);
    }
  }

  if(topjets.size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : topjets.at(0).pfcand_indexs()){
      particles.push_back(pfparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(2, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    // cout << "calculated tau2: " << tau << '\n';
    // cout << "save tau2:       " << topjets.at(0).tau2() << '\n';
  }

  return tau;
}

double Calculator::PF_tau3(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<TopJet> topjets = event.get(hndl);
  vector<PFParticle>* pfparticles = event.pfparticles;

  if (label.find("puppi_sd") != std::string::npos || label.find("puppi") != std::string::npos){
    for (const auto candInd : topjets.at(0).pfcand_indexs()){
      float puppiWeight = pfparticles->at(candInd).puppiWeight();
      pfparticles->at(candInd).set_v4(pfparticles->at(candInd).v4() * puppiWeight);
    }
  }

  if(topjets.size() > 0){
    vector<PFParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : topjets.at(0).pfcand_indexs()){
      particles.push_back(pfparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(3, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    // cout << "calculated tau3: " << tau << '\n';
    // cout << "save tau3:       " << topjets.at(0).tau3() << '\n';
  }

  return tau;
}


double Calculator::Gen_tau1(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl_gen)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<GenTopJet> gentopjet = event.get(hndl_gen);

  if(gentopjet.size() > 0){
    vector<GenParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : gentopjet.at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(1, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of gentopjet: " << gentopjet.at(0).pt() << '\n';
    // cout << "calculated tau1: " << tau << '\n';
    // cout << "save tau1:       " << gentopjet.at(0).tau1() << '\n';
  }

  return tau;
}

double Calculator::Gen_tau2(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl_gen)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<GenTopJet> gentopjet = event.get(hndl_gen);

  if(gentopjet.size() > 0){
    vector<GenParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : gentopjet.at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(2, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of gentopjet: " << gentopjet.at(0).pt() << '\n';
    // cout << "calculated tau2: " << tau << '\n';
    // cout << "save tau2:       " << gentopjet.at(0).tau2() << '\n';
  }
  return tau;
}

double Calculator::Gen_tau3(uhh2::Event & event){
  double tau = -100;

  if (!event.is_valid(hndl_gen)) {
    std::cerr << "In VariablesCalculator: Handle not valid!\n";
    assert(false);
  }

  vector<GenTopJet> gentopjet = event.get(hndl_gen);

  if(gentopjet.size() > 0){
    vector<GenParticle> particles;

    // get the PFParticles inside the first topjet
    for(const auto candInd : gentopjet.at(0).genparticles_indices()){
      particles.push_back(event.genparticles->at(candInd));
    }
    if(particles.size() == 0) return -50;

    std::vector<PseudoJet> pfpart_psj;
    int user_index = 0;

    // convert PFParticles to PseudoJets
    for(const auto & cand: particles){
      PseudoJet tmp_psj = PseudoJet(cand.v4().px(), cand.v4().py(), cand.v4().pz(), cand.v4().energy());
      tmp_psj.set_user_index(user_index);
      pfpart_psj.push_back(tmp_psj);
      user_index++;
    }

    PseudoJet tester = join(pfpart_psj);

    // recreate the first topjet
    // JetDefinition jet_def(antikt_algorithm, 0.8);
    // ClusterSequence cs(pfpart_psj, jet_def);
    // vector<PseudoJet> antikt_jets = cs.inclusive_jets(10.0);
    // PseudoJet blub = join(antikt_jets);
    // std::vector<PseudoJet> v = blub.constituents();
    // cout << blub.has_constituents() << '\n';
    // cout << "pt of merged jets: " << blub.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v.size() << '\n';
    // std::vector<PseudoJet> v2 = tester.constituents();
    // cout << tester.has_constituents() << '\n';
    // cout << "pt of merged jets: " << tester.pt() << '\n';
    // cout << "number of constituents of merged jets: " << v2.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of calculated jets (PF_tau2): " << antikt_jets.size() << '\n';
    // cout << "VariablesCalculator.cxx: size of topjets (PF_tau2): " << topjets.size() << '\n';
    // vector<PseudoJet> test;
    // if(antikt_jets.size() > 0){
    //   test = cs.constituents(antikt_jets.at(0)); //get the constituents which made the new jet
    //   cout << "pt of topjet: " << topjets.at(0).pt() << '\n';
    //   cout << "pt of calc first topjet: " << antikt_jets.at(0).pt() << '\n';
    //   // cout << "number of subjets: " << antikt_jets.at(0).subjets().size() << '\n';
    // }
    // vector<PseudoJet> test2;
    // if(antikt_jets.size() > 1) test2 = cs.constituents(antikt_jets.at(1)); //get the constituents which made the new jet
    // cout << "VariablesCalculator.cxx: number of constituents: " << test.size() << '\n';
    // cout << "VariablesCalculator.cxx: number of input constituents: " << pfpart_psj.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "VariablesCalculator.cxx: number of constituents: " << test2.size() << '\n';
    // if(antikt_jets.size() > 1) cout << "pt of calc second topjet: " << antikt_jets.at(1).pt() << '\n';
    // cout << "VariablesCalculator.cxx: number of constituents 'old jet': " << particles.size() << '\n';

    // double tau_test = nSub(antikt_jets.at(0));
    // cout << "before doing anything to the new jet tau: " << tau_test << '\n';

    // if(antikt_jets.size() > 1) throw runtime_error("VariablesCalculator.cxx: got more then 1 Jet! (PF_tau2)");

    // SoftDrop sd(0.0, 0.1, 0.8); //beta = 0.0, zcut = 0.1, R0 = 0.8  -- for now the only values I found  --  maybe we dont need this?????
    // actually... didn't use it properly... further should be:
    // PseudoJet sdjet;
    // if(antikt_jets.size() > 0) sdjet = sd(antikt_jets.at(0));
    // if(antikt_jets.size() > 0) cout << "after softdrop: " << sdjet.pt() << '\n';

    // calculate Nsubjettiness
    Nsubjettiness nSub(3, OnePass_KT_Axes(), NormalizedMeasure(1.0, 0.8)); // beta = 1.0, R0 = 0.8
    // tau = nSub(antikt_jets.at(0));
    tau = nSub(tester);
    // cout << "pt of gentopjet: " << gentopjet.at(0).pt() << '\n';
    // cout << "calculated tau3: " << tau << '\n';
    // cout << "save tau3:       " << gentopjet.at(0).tau3() << '\n';
  }

  return tau;
}
