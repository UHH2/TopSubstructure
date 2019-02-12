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

#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class QCD_test: public AnalysisModule {
  public:
    explicit QCD_test(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_QCD;

    uhh2::Event::Handle<double> h_gen_weight;
  };


  QCD_test::QCD_test(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");

    // 2. set up selections


    // 3. Set up Hists classes:
    h_QCD.reset(new TopSubstructureRecoHists(ctx, "QCD"));
  }


  bool QCD_test::process(Event & event) {
    cout << "QCD_test: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    event.set(h_gen_weight, 1);
    // 1. run all modules other modules.
    h_QCD->fill(event);

    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(QCD_test)

}
