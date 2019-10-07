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

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class PreSelectionModule: public AnalysisModule {
  public:
    explicit PreSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    bool isMC, isTTbar, isEle, isMu;
    TString dataset_version_string;
    bool passed_nmu_gen, passed_nele_gen, passed_mu_pt_gen, passed_ele_pt_gen, passed_topjet_pt_gen, passed_mu_gen, passed_ele_gen, passed_mu_rec, passed_ele_rec;
    bool passed_njet_rec, passed_nmu_rec, passed_nele_rec, passed_met_rec;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> genmttbar_sel;
    std::unique_ptr<Selection> met_sel, nmu_sel, nele_sel, njet_sel;
    std::unique_ptr<Selection> nmu_gen, nele_gen, pt_mu_gen, pt_ele_gen, pt_topjet_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.

    uhh2::Event::Handle<bool>   h_passed_mu_gen_pre, h_passed_ele_gen_pre;
    uhh2::Event::Handle<bool>   h_passed_mu_rec_pre, h_passed_ele_rec_pre;
  };


  PreSelectionModule::PreSelectionModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_mu_gen_pre   = ctx.declare_event_output<bool>("h_passed_mu_gen_pre");
    h_passed_ele_gen_pre  = ctx.declare_event_output<bool>("h_passed_ele_gen_pre");
    h_passed_mu_rec_pre   = ctx.declare_event_output<bool>("h_passed_mu_rec_pre");
    h_passed_ele_rec_pre  = ctx.declare_event_output<bool>("h_passed_ele_rec_pre");

    dataset_version_string = ctx.get("dataset_version");

    isEle   = dataset_version_string.Contains("SingleElectron");
    isMu    = dataset_version_string.Contains("SingleMuon");
    isTTbar = dataset_version_string.Contains("TTbar");
    isMC = (ctx.get("dataset_type") == "MC");

    // cout << "isEle: " << isEle << '\n';
    // cout << "isMu: " << isMu << '\n';
    // cout << "isTTbar: " << isTTbar << '\n';
    // cout << "isMC: " << isMC << '\n';

    if(ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700") genmttbar_sel.reset(new MttbarGenSelection(0., 700.));
    else genmttbar_sel.reset(new uhh2::AndSelection(ctx));

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      nmu_gen.reset(new TTbarSemilep(ctx, 0));
      nele_gen.reset(new TTbarSemilep(ctx, 1));
      pt_topjet_gen.reset(new GenTopJetPtSelection(200));
      pt_mu_gen.reset(new GenLeptonPtSelection(ctx, 0, 45));
      pt_ele_gen.reset(new GenLeptonPtSelection(ctx, 1, 45));
    }

    met_sel.reset(new METSelection(30, -1));
    njet_sel.reset(new NJetSelection(1, -1, JetId(PtEtaCut(20, 2.4))));
    nmu_sel.reset(new NMuonSelection(1, -1, MuonId(PtEtaCut(45, 2.4))));
    nele_sel.reset(new NElectronSelection(1, -1, ElectronId(PtEtaCut(45, 2.4))));
  }


  bool PreSelectionModule::process(Event & event) {
    // cout << "PreSelectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    // 1. run all modules other modules.
    passed_mu_gen = false;
    passed_ele_gen = false;
    passed_mu_rec = false;
    passed_ele_rec = false;

    passed_nmu_gen = false;
    passed_nele_gen = false;
    passed_mu_pt_gen = false;
    passed_ele_pt_gen = false;
    passed_topjet_pt_gen = false;

    passed_njet_rec = false;
    passed_nmu_rec = false;
    passed_nele_rec = false;
    passed_met_rec = false;

    if(isTTbar){
      ttgenprod->process(event);
      if(!genmttbar_sel->passes(event)) return false;

      passed_nmu_gen = nmu_gen->passes(event);
      passed_nele_gen = nele_gen->passes(event);

      passed_mu_pt_gen = pt_mu_gen->passes(event);
      passed_ele_pt_gen = pt_ele_gen->passes(event);

      passed_topjet_pt_gen = pt_topjet_gen->passes(event);

      passed_mu_gen = (passed_nmu_gen && passed_mu_pt_gen && passed_topjet_pt_gen);
      passed_ele_gen = (passed_nele_gen && passed_ele_pt_gen && passed_topjet_pt_gen);
    }

    passed_njet_rec = njet_sel->passes(event);

    if(isMu || isMC) passed_nmu_rec = nmu_sel->passes(event);
    if(isEle || isMC) passed_nele_rec = nele_sel->passes(event);

    passed_met_rec = met_sel->passes(event);

    passed_mu_rec = (isMu || isMC) && passed_njet_rec && passed_nmu_rec && passed_met_rec;
    passed_ele_rec = (isEle || isMC) && passed_njet_rec && passed_nele_rec && passed_met_rec;

    if(!passed_mu_rec && !passed_ele_rec && !passed_mu_gen && !passed_ele_gen) return false;

    // 3. decide whether or not to keep the current event in the output:
    // cout << "passed_mu_gen: " << passed_mu_gen <<'\n';
    // cout << "passed_mu_rec: " << passed_mu_rec <<'\n';
    event.set(h_passed_mu_rec_pre, passed_mu_rec);
    event.set(h_passed_ele_rec_pre, passed_ele_rec);
    event.set(h_passed_mu_gen_pre, passed_mu_gen);
    event.set(h_passed_ele_gen_pre, passed_ele_gen);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PreSelectionModule)

}
