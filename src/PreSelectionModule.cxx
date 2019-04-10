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

#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"
#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class PreSelectionModule: public AnalysisModule {
  public:
    explicit PreSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    bool isMC, isTTbar;
    bool passed, passed_rec;
    bool passed_gen, passed_gen_sel;
    bool matched_gen;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> met_sel, nmu_sel, njet_sel;
    std::unique_ptr<Selection> genmatching, nmu_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_start, h_gen_nmu, h_gen_nmu_matched, h_gen_nmu_unmatched;
    std::unique_ptr<Hists> h_start;

    std::unique_ptr<Hists> h_nmu, h_njet, h_met;

    uhh2::Event::Handle<double> h_rec_weight_pre;
    uhh2::Event::Handle<double> h_gen_weight_pre;
    uhh2::Event::Handle<bool> h_passed_gen_pre;
    uhh2::Event::Handle<bool> h_passed_rec_pre;
  };


  PreSelectionModule::PreSelectionModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_rec_pre = ctx.declare_event_output<bool>("h_passed_rec_pre");
    h_passed_gen_pre = ctx.declare_event_output<bool>("h_passed_gen_pre");

    isMC = (ctx.get("dataset_type") == "MC");
    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v2" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v2" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v2");

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      nmu_gen.reset(new TTbarSemilep(ctx));
      genmatching.reset(new GenMatching(ctx));
    }

    met_sel.reset(new METSelection(30, -1));

    njet_sel.reset(new NJetSelection(1, -1, JetId(PtEtaCut(20, 2.4))));
    nmu_sel.reset(new NMuonSelection(1, -1, MuonId(PtEtaCut(45, 2.4))));

    // 3. Set up Hists classes:
    h_gen_start.reset(new GenHists(ctx, "gen_start"));
    h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
    h_gen_nmu_matched.reset(new GenHists(ctx, "gen_nmu_matched"));
    h_gen_nmu_unmatched.reset(new GenHists(ctx, "gen_nmu_unmatched"));

    h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
    h_njet.reset(new TopSubstructureRecoHists(ctx, "njet"));
    h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
    h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
  }


  bool PreSelectionModule::process(Event & event) {
    cout << "PreSelectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    h_start->fill(event);
    h_gen_start->fill(event);

    // 1. run all modules other modules.
    passed = false;
    passed_gen_sel = false;
    passed_gen = false;
    passed_rec = false;

    if(isTTbar){
      ttgenprod->process(event);
      passed_gen_sel = nmu_gen->passes(event);
      if(passed_gen_sel){
        h_gen_nmu->fill(event);
        passed_gen = true;
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_nmu_matched->fill(event);
        else h_gen_nmu_unmatched->fill(event);
      }
    }

    passed = njet_sel->passes(event);
    if(passed){
      h_njet->fill(event);

      passed = nmu_sel->passes(event);
      if(passed){
        h_nmu->fill(event);

        passed = met_sel->passes(event);
        if(passed){
          h_met->fill(event);
          passed_rec = true;
        }
      }
    }

    if(!passed_rec && !passed_gen) return false;

    // 3. decide whether or not to keep the current event in the output:
    event.set(h_passed_rec_pre, passed_rec);
    event.set(h_passed_gen_pre, passed_gen);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PreSelectionModule)

}
