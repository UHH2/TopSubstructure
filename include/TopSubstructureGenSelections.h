#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/Selection.h"
#include <UHH2/core/include/AnalysisModule.h>
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/CommonModules.h"
#include <UHH2/common/include/TTbarGen.h>

#include <iostream>
#include <memory>
#include <vector>


namespace uhh2examples {

  class GenDPhiSelection : public uhh2::Selection{
  public:
    explicit GenDPhiSelection(uhh2::Context& ctx, double dphi_min = 0., double dphi_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    double dphi_min, dphi_max;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
  };

  class GenQuarkGenJetMatching : public uhh2::Selection{
  public:
    explicit GenQuarkGenJetMatching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
  };

  class GenNTopJet : public uhh2::Selection{
  public:
    explicit GenNTopJet(uhh2::Context &, double n_min = 0, double n_max = -1);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
    double n_min, n_max;
  };

}
