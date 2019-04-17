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
  class TopSubstructureRecoHists: public uhh2::Hists {
  public:
    TopSubstructureRecoHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    TH2* TwoDCut;
    uhh2::Event::Handle<double> h_weight, h_weight_kin;
    bool isTTbar;
  };

}
