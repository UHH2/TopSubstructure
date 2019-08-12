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
  class RecPuppiJetLeptonCleaner: public uhh2::AnalysisModule{
  public:
    explicit RecPuppiJetLeptonCleaner(uhh2::Context &, int mode);
    virtual bool process(uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;
    int mode;

  };

  class PuppiJetPtSelection : public uhh2::Selection{
  public:
    explicit PuppiJetPtSelection(uhh2::Context& ctx, double pt_min = 0., double pt_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    double pt_min, pt_max;
  };

  class NPuppiJet : public uhh2::Selection{
  public:
    explicit NPuppiJet(uhh2::Context &, double n_min = 0, double n_max = -1);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    double n_min, n_max;
  };

  class PuppiMassCompare : public uhh2::Selection{
  public:
    explicit PuppiMassCompare(uhh2::Context& ctx, int mode1, int mode2);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    int mode1, mode2;
  };

  class PuppidRSelection : public uhh2::Selection{
  public:
    explicit PuppidRSelection(uhh2::Context& ctx, int mode1, int mode2, double dr_min = 0.8);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    int mode1, mode2;
    double dr_min;
  };

  class PuppiPtSelection:public uhh2::Selection{
  public:
    explicit PuppiPtSelection(uhh2::Context& ctx, double pt_jet1_min = 0, double pt_jet2_min = 0, int mode = 0);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    double pt_jet1_min, pt_jet2_min;
    int mode;
  };

  class MassPuppiJetSelection : public uhh2::Selection{
  public:
    explicit MassPuppiJetSelection(uhh2::Context& ctx, int n, double mass_min = 0, double mass_max = -1);
    virtual bool passes(const uhh2::Event &) override;
  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    int n;
    double mass_min, mass_max;
  };
}
