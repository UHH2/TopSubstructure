#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenTopJet.h"

#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/CommonModules.h"
#include <UHH2/common/include/TTbarGen.h>

#include <iostream>
#include <memory>
#include <vector>

class TopJetSortDPhi: public uhh2::AnalysisModule{
 public:
  explicit TopJetSortDPhi(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;

};

class TopJetSortMass: public uhh2::AnalysisModule{
 public:
  explicit TopJetSortMass(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;

};

class JetSelection: public uhh2::AnalysisModule{
 public:
  explicit JetSelection(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<Jet>> h_jetsel;
  uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;
};

class GenTopJetLeptonCleaner: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetLeptonCleaner(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<TTbarGen> h_ttbargen;
};

class GenTopJetSortDPhi: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetSortDPhi(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;

};

class GenTopJetSortMass: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetSortMass(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
};

class GenTopJetSortPt: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetSortPt(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
};

class GenTopJetCleaner: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetCleaner(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_lep_cand;
};

class GenJetSelection: public uhh2::AnalysisModule{
 public:
  explicit GenJetSelection(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<Particle>> h_genjetsel;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
};

class GenTopJetSortDPhiMass: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetSortDPhiMass(uhh2::Context &, double dphi_max = -1);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<TTbarGen> h_ttbargen;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_lep_cand;
  double dphi_max;
};

class GenTopJetSortDPhiPt: public uhh2::AnalysisModule{
 public:
  explicit GenTopJetSortDPhiPt(uhh2::Context &, double dphi_max = -1);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<TTbarGen> h_ttbargen;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
  uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_lep_cand;
  double dphi_max;
};
