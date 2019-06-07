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
#include <UHH2/core/include/AnalysisModule.h>
#include <UHH2/core/include/Selection.h>
#include <UHH2/common/include/LumiSelection.h>
#include <UHH2/common/include/JetCorrections.h>
#include <UHH2/common/include/ObjectIdUtils.h>
#include <UHH2/common/include/Utils.h>
#include <UHH2/common/include/AdditionalSelections.h>

#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"
#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"
#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class PreSelectionModule: public AnalysisModule {
  public:
    explicit PreSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    bool isMC, isTTbar;
    bool passed_rec, passed_gen;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<uhh2::Selection> genmttbar_sel;
    std::unique_ptr<Selection> met_sel, nmu_sel, njet_sel;
    std::unique_ptr<Selection> nmu_gen, pt_mu_gen, pt_topjet_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<calc_Nsubjettiness> calculator_tau;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_start, h_gen_nmu, h_gen_pt_mu, h_gen_pt_topjet;
    std::unique_ptr<Hists> h_start, h_nmu, h_njet, h_met;

    uhh2::Event::Handle<bool> h_passed_gen_pre;
    uhh2::Event::Handle<bool> h_passed_rec_pre;
  };


  PreSelectionModule::PreSelectionModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_rec_pre = ctx.declare_event_output<bool>("h_passed_rec_pre");
    h_passed_gen_pre = ctx.declare_event_output<bool>("h_passed_gen_pre");

    isMC = (ctx.get("dataset_type") == "MC");
    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v3");

    if(ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3") genmttbar_sel.reset(new MttbarGenSelection(0., 700.));
    else genmttbar_sel.reset(new uhh2::AndSelection(ctx));

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      nmu_gen.reset(new TTbarSemilep(ctx));
      pt_mu_gen.reset(new GenMuonPtSelection(ctx, 45));
      pt_topjet_gen.reset(new GenTopJetPtSelection(200));
    }

    met_sel.reset(new METSelection(30, -1));
    njet_sel.reset(new NJetSelection(1, -1, JetId(PtEtaCut(20, 2.4))));
    nmu_sel.reset(new NMuonSelection(1, -1, MuonId(PtEtaCut(45, 2.4))));

    // 3. Set up Hists classes:
    h_gen_start.reset(new GenHists(ctx, "gen_start"));
    h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));
    h_gen_pt_topjet.reset(new GenHists(ctx, "gen_pt_topjet"));

    h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
    h_njet.reset(new TopSubstructureRecoHists(ctx, "njet"));
    h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
    h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
  }


  bool PreSelectionModule::process(Event & event) {
    cout << "PreSelectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    calculator_tau->tau_one(event);

    h_start->fill(event);
    if(isMC) h_gen_start->fill(event);

    // 1. run all modules other modules.
    passed_gen = false;
    passed_rec = false;

    if(isTTbar){
      ttgenprod->process(event);
      if(!genmttbar_sel->passes(event)) return false;
      passed_gen = nmu_gen->passes(event);
      if(passed_gen){
        h_gen_nmu->fill(event);

        passed_gen = pt_mu_gen->passes(event);
        if(passed_gen){
          h_gen_pt_mu->fill(event);

          passed_gen = pt_topjet_gen->passes(event);
          if(passed_gen) h_gen_pt_topjet->fill(event);
        }
      }
    }

    passed_rec = njet_sel->passes(event);
    if(passed_rec){
      h_njet->fill(event);

      passed_rec = nmu_sel->passes(event);
      if(passed_rec){
        h_nmu->fill(event);

        passed_rec = met_sel->passes(event);
        if(passed_rec) h_met->fill(event);
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
