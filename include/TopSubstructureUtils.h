#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

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
  uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand_by_dphi;

};

class TopJetSortMass: public uhh2::AnalysisModule{
 public:
  explicit TopJetSortMass(uhh2::Context &);
  virtual bool process(uhh2::Event &) override;

 private:
  uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand_by_mass;

};
