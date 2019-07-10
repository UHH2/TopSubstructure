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
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
    double dphi_min, dphi_max;
  };

  class GenTopJetPtSelection : public uhh2::Selection{
  public:
    explicit GenTopJetPtSelection(double pt_min = 0);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    double pt_min;
  };

  class GenQuarkGenJetMatching : public uhh2::Selection{
  public:
    explicit GenQuarkGenJetMatching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };

  class GenNTopJetCand : public uhh2::Selection{
  public:
    explicit GenNTopJetCand(uhh2::Context &, double n_min = 0, double n_max = -1);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
    double n_min, n_max;
  };

  class GenNTopJet : public uhh2::Selection{
  public:
    explicit GenNTopJet(uhh2::Context &, double n_min = 0, double n_max = -1, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    double n_min, n_max;
  };

  class TTbarSemilep : public uhh2::Selection {

  public:
    explicit TTbarSemilep(Context&);
    virtual bool passes(const Event&) override;

  protected:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };

  class GenMassCompare : public uhh2::Selection{
  public:
    explicit GenMassCompare(uhh2::Context& ctx, int mode, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    int mode;
  };

  class PtSelection:public uhh2::Selection{
  public:
    explicit PtSelection(uhh2::Context& ctx, double pt_jet1_min = 0, double pt_jet2_min = 0, int mode = 0, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    double pt_jet1_min, pt_jet2_min;
    int mode;
  };

  class dRSelection:public uhh2::Selection{
  public:
    explicit dRSelection(uhh2::Context& ctx, double dr_min = 0.8, int mode = 0, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    double dr_min;
    int mode;
  };

  class GenMuonPtSelection:public uhh2::Selection{
  public:
    explicit GenMuonPtSelection(uhh2::Context& ctx, double pt_min = 0, double pt_max = -1);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    double pt_min, pt_max;
  };

  class GenMassTopJet : public uhh2::Selection{
  public:
    explicit GenMassTopJet(uhh2::Context& ctx, double mass_min = 0, double mass_max = -1, int mode = 0, std::string const & label_ = "gentopjets");
    virtual bool passes(const uhh2::Event &) override;
  private:
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    double mass_min, mass_max;
    int mode;
  };

}
