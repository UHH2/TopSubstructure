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

  /**  \brief Example class for booking and filling histograms
   *
   * NOTE: This class uses the 'hist' method to retrieve histograms.
   * This requires a string lookup and is therefore slow if you have
   * many histograms. Therefore, it is recommended to use histogram
   * pointers as member data instead, like in 'common/include/ElectronHists.h'.
   */
  class TopSubstructureGenHists: public uhh2::Hists {
  public:
    // use the same constructor arguments as Hists for forwarding:
    TopSubstructureGenHists(uhh2::Context & ctx, const std::string & dirname);

    virtual void fill(const uhh2::Event & ev) override;
    /* virtual ~TopSubstructureGenHists(); */

  protected:
    TH2* TwoDCut;
    uhh2::Event::Handle<std::vector<Particle>> h_genjetsel;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_cand;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet_lep_cand;
    uhh2::Event::Handle<TTbarGen> h_ttbargen;
    std::string sort_by;
  };

}
