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
    explicit RecPtSelection(double pt_jet1_min = 0, double pt_jet2_min = 0);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    double pt_jet1_min, pt_jet2_min;
  };

  class RecMassSelection : public uhh2::Selection{
  public:
    explicit RecMassSelection(int n);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    int n;
  };

  class RecdRSelection:public uhh2::Selection{
  public:
    explicit RecdRSelection(double dr_min = 0.8);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    double dr_min;
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
    explicit GenMatching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };
}
