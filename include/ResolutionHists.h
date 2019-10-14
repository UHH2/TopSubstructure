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
  class ResolutionHists: public uhh2::Hists {
  public:
    ResolutionHists(uhh2::Context & ctx, const std::string & dirname, std::string const & label1 = "genjetsAk8Substructure", std::string const & label2 = "topjets");

    virtual void fill(const uhh2::Event & ev) override;

  protected:
    uhh2::Event::Handle<std::vector<GenTopJet>> hndl1;
    uhh2::Event::Handle<std::vector<TopJet>> hndl2;
    uhh2::Event::Handle<double> h_rec_weight, h_gen_weight;
    bool isTTbar;
    bool gen_sd, rec_puppi;
  };

}
