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
  class RecPtSelection:public uhh2::Selection{
  public:
    explicit RecPtSelection(double pt_jet1_min = 0, double pt_jet2_min = 0, int mode = 0);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    double pt_jet1_min, pt_jet2_min;
    int mode;
  };

  class RecMassCompare : public uhh2::Selection{
  public:
    explicit RecMassCompare(int mode1, int mode2);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    int mode1, mode2;
  };

  class RecdRSelection:public uhh2::Selection{
  public:
    explicit RecdRSelection(double dr_min = 0.8, int mode1 = 0, int mode2 = 0);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    double dr_min;
    int mode1, mode2;
  };

  class RecMatching : public uhh2::Selection{
  public:
    explicit RecMatching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };

  class GenMatching : public uhh2::Selection{
  public:
    explicit GenMatching(uhh2::Context& ctx, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
  };
}
