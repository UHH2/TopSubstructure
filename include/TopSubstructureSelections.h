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

  /* Select events with at least two jets in which the leading two jets have deltaphi > 2.7 and the third jet pt is
  * below 20% of the average of the leading two jets, where the minimum deltaphi and
  * maximum third jet pt fraction can be changed in the constructor.
  * The jets are assumed to be sorted in pt.
  */

  class METSelection : public uhh2::Selection{
  public:
    explicit METSelection(double MET_min = 0., double MET_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    double MET_min, MET_max;
  };

  class LeptonPtSelection : public uhh2::Selection{
  public:
    explicit LeptonPtSelection(int mode, double pt_min = 0., double pt_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    int mode;
    double pt_min, pt_max;
  };

  class TopJetptSelection : public uhh2::Selection{
  public:
    explicit TopJetptSelection(uhh2::Context& ctx, double pt_min = 0., double pt_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet;
    double pt_min, pt_max;
  };

  class TwoDCut : public uhh2::Selection {
  public:
    explicit TwoDCut(int mode, double min_deltaR = 0.4, double min_pTrel = 40.);
    virtual bool passes(const uhh2::Event & event) override;

  private:
    int mode;
    float min_deltaR, min_pTrel;
  };

  class DPhiSelection : public uhh2::Selection{
  public:
    explicit DPhiSelection(uhh2::Context& ctx, double dphi_min = 0., double dphi_max = -1);
    virtual bool passes(const uhh2::Event & event) override;
  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet;
    double dphi_min, dphi_max;
  };

  class Matching : public uhh2::Selection{
  public:
    explicit Matching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };

  class QuarkCandJetMatching : public uhh2::Selection{
  public:
    explicit QuarkCandJetMatching(uhh2::Context &);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
  };

  class NTopJet : public uhh2::Selection{
  public:
    explicit NTopJet(uhh2::Context &, double n_min = 0, double n_max = -1);
    virtual bool passes(const uhh2::Event &) override;

  private:
    uhh2::Event::Handle<std::vector<TopJet>> h_topjet_cand;
    double n_min, n_max;
  };

  class MassTopJetSelection : public uhh2::Selection{
  public:
    explicit MassTopJetSelection(int n, double mass_min = 0, double mass_max = -1);
    virtual bool passes(const uhh2::Event &) override;
  private:
    int n;
    double mass_min, mass_max;
  };
}
