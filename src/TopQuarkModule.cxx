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
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class TopQuarkModule: public AnalysisModule {
  public:
    explicit TopQuarkModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> genparticle_rm;
    std::unique_ptr<AnalysisModule> pfparticle_rm;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_pt_top_quark;

    uhh2::Event::Handle<double> h_gen_weight;
    uhh2::Event::Handle<std::vector<GenParticle>> hndl;
  };


  TopQuarkModule::TopQuarkModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");
    hndl = ctx.get_handle<vector<GenParticle>>("genparticles");

    // 2. set up selections
    const std::string ttbar_gen_label("ttbargen");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    genparticle_rm.reset(new GenParticleRemover(ctx));
    pfparticle_rm.reset(new PFParticleRemover(ctx));


    // 3. Set up Hists classes:
    h_pt_top_quark.reset(new GenHists(ctx, "pt_top_quark"));
  }


  bool TopQuarkModule::process(Event & event) {
    cout << "TopQuarkModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    event.set(h_gen_weight, 1);
    // 1. run all modules other modules.

    ttgenprod->process(event);

    std::vector<GenParticle> genp;
    for(unsigned int i=0; i < event.genparticles->size(); i++){
      if(abs(event.genparticles->at(i).pdgId()) == 6){
        genp.push_back(event.genparticles->at(i));
      }
    }

    if(genp.size() == 2){
      std::vector<GenParticle> dummy;
      if(genp[0].v4().pt() > genp[1].v4().pt()){
        dummy.push_back(genp[0]);
        dummy.push_back(genp[1]);
      }
      else{
        dummy.push_back(genp[1]);
        dummy.push_back(genp[0]);
      }
      if(dummy.at(0).pt() > 130) {
        h_pt_top_quark->fill(event);
        genparticle_rm->process(event);
        pfparticle_rm->process(event);
        return true;
      }
    }





    return false;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(TopQuarkModule)

}
