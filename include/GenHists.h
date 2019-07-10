#pragma once

#include "UHH2/core/include/Hists.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/GenTopJet.h"

#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/TTbarGen.h"

#include "TH1F.h"
#include "TH2F.h"

namespace uhh2examples {
  class GenHists: public uhh2::Hists {
  public:
    GenHists(uhh2::Context & ctx, const std::string & dirname, std::string const & label = "gentopjets");

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    uhh2::Event::Handle<std::vector<GenTopJet>> hndl;
    TH2* TwoDCut;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    uhh2::Event::Handle<double> h_weight, h_weight_kin;
    uhh2::Event::Handle<double> h_tau2_start, h_tau2_lepcleaner, h_tau2_cleaner, h_tau3_start, h_tau3_lepcleaner, h_tau3_cleaner;
  };

}
