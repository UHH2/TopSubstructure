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
  class PuppiRecoHists: public uhh2::Hists {
  public:
    PuppiRecoHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    TH2* TwoDCut;
    uhh2::Event::Handle<double> h_weight, h_weight_kin;
    uhh2::Event::Handle<double> h_tau2_start_puppi, h_tau2_common_puppi, h_tau2_corrector_puppi, h_tau2_lepcleaner_puppi, h_tau2_cleaner_puppi;
    uhh2::Event::Handle<double> h_tau3_start_puppi, h_tau3_common_puppi, h_tau3_corrector_puppi, h_tau3_lepcleaner_puppi, h_tau3_cleaner_puppi;
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    bool isTTbar;
  };

}
