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
  class NSubjetsModule: public AnalysisModule {
  public:
    explicit NSubjetsModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> genmatching, nmu_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> genparticle_rm;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nsubjets;

    uhh2::Event::Handle<double> h_gen_weight;
  };


  NSubjetsModule::NSubjetsModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");

    // 2. set up selections
    const std::string ttbar_gen_label("ttbargen");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    genparticle_rm.reset(new GenParticleRemover(ctx));

    h_nsubjets.reset(new GenHists(ctx, "nsubjets"));

    // 3. Set up Hists classes:
  }


  bool NSubjetsModule::process(Event & event) {
    cout << "NSubjetsModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    event.set(h_gen_weight, 1);
    // 1. run all modules other modules.

    ttgenprod->process(event);
    sort_by_pt<GenTopJet>(*event.gentopjets);

    int size = event.gentopjets->size();
    switch (size){
      case 0:
        break;
      case 1:
        if(event.gentopjets->at(0).subjets().size() > 0){
          h_nsubjets->fill(event);
          // genparticle_rm->process(event);
          return true;
        }
        break;
      default:
        for(unsigned int i=0; i < event.gentopjets->size(); i++){
          if(event.gentopjets->at(i).subjets().size() > 0){
            h_nsubjets->fill(event);
            // genparticle_rm->process(event);
            return true;
          }
        }
        break;
    }

    return false;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(NSubjetsModule)

}
