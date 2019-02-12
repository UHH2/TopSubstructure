#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/TopJetIds.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/TTbarGen.h"

#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class SplitFileModule: public AnalysisModule {
  public:
    explicit SplitFileModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool isTTbar, isTTbar2, isTTbar3;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> genmatching, nmu_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_mass_0_700, h_mass_700_1000, h_mass_1000_Inft;
    std::unique_ptr<Hists> h_rec_0_700, h_rec_700_1000, h_rec_1000_Inft;

    uhh2::Event::Handle<double> h_gen_weight;
  };


  SplitFileModule::SplitFileModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");

    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700");
    isTTbar2 = (ctx.get("dataset_version") == "TTbar_Mtt0700to1000");
    isTTbar3 = (ctx.get("dataset_version") == "TTbar_Mtt1000toInft");

    // 2. set up selections
    const std::string ttbar_gen_label("ttbargen");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));


    // 3. Set up Hists classes:
    if(isTTbar){
      h_mass_0_700.reset(new GenHists(ctx, "mass_0_700"));
      h_rec_0_700.reset(new TopSubstructureRecoHists(ctx, "rec_0_700"));
    }
    if(isTTbar2) {
      h_mass_700_1000.reset(new GenHists(ctx, "mass_700_1000"));
      h_rec_700_1000.reset(new TopSubstructureRecoHists(ctx, "rec_700_1000"));
    }
    if(isTTbar3){
      h_mass_1000_Inft.reset(new GenHists(ctx, "mass_1000_Inft"));
      h_rec_1000_Inft.reset(new TopSubstructureRecoHists(ctx, "rec_1000_Inft"));
    }
  }


  bool SplitFileModule::process(Event & event) {
    cout << "SplitFileModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    event.set(h_gen_weight, 1);
    // 1. run all modules other modules.

    ttgenprod->process(event);
    std::vector<GenParticle> genp;
    for(unsigned int i=0; i < event.genparticles->size(); i++){
      if(abs(event.genparticles->at(i).pdgId()) == 6){
        genp.push_back(event.genparticles->at(i));
      }
    }

    auto dummy = genp[0].v4() + genp[1].v4();

    if(isTTbar && dummy.M() >= 0 && dummy.M() < 700){
      h_mass_0_700->fill(event);
      h_rec_0_700->fill(event);
      return true;
    }
    else if(isTTbar2 && dummy.M() >= 700 && dummy.M() < 1000){
      h_mass_700_1000->fill(event);
      h_rec_700_1000->fill(event);
      return true;
    }
    else if(isTTbar3 && dummy.M() >= 1000){
      h_mass_1000_Inft->fill(event);
      h_rec_1000_Inft->fill(event);
      return true;
    }

    return false;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(SplitFileModule)

}
