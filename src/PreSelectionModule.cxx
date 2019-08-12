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
    bool passed_nmu_gen, passed_nele_gen, passed_mu_pt_gen, passed_ele_pt_gen, passed_topjet_pt_gen, passed_mu_gen, passed_ele_gen, passed_mu_rec, passed_ele_rec;
    bool passed_njet_rec, passed_nmu_rec, passed_nele_rec, passed_met_rec;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> genmttbar_sel;
    std::unique_ptr<Selection> met_sel, nmu_sel, nele_sel, njet_sel;
    std::unique_ptr<Selection> nmu_gen, nele_gen,pt_mu_gen, pt_ele_gen, pt_topjet_gen;

    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_nocuts, h_gen_nmu, h_gen_nele, h_gen_pt_mu, h_gen_pt_ele, h_gen_pt_topjet_mu, h_gen_pt_topjet_ele;
    std::unique_ptr<Hists> h_nocuts, h_njet, h_nmu, h_nele, h_met_mu, h_met_ele;

    uhh2::Event::Handle<bool> h_passed_mu_gen_pre, h_passed_ele_gen_pre;
    uhh2::Event::Handle<bool> h_passed_mu_rec_pre, h_passed_ele_rec_pre;
  };


  PreSelectionModule::PreSelectionModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_mu_gen_pre = ctx.declare_event_output<bool>("h_passed_mu_gen_pre");
    h_passed_ele_gen_pre = ctx.declare_event_output<bool>("h_passed_ele_gen_pre");
    h_passed_mu_rec_pre = ctx.declare_event_output<bool>("h_passed_mu_rec_pre");
    h_passed_ele_rec_pre = ctx.declare_event_output<bool>("h_passed_ele_rec_pre");

    isMC = (ctx.get("dataset_type") == "MC");

    isEle = (ctx.get("dataset_version") == "DATA_2016v3_SingleElectronB" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronC" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronD" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronE" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronF" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronG" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronH");

    isMu = (ctx.get("dataset_version") == "DATA_2016v3_SingleMuonB" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonC" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonD" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonE" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonF" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonG" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonH");

    isTTbar = (ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft" || ctx.get("dataset_version") == "TTbar_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph");

    if(ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700") genmttbar_sel.reset(new MttbarGenSelection(0., 700.));
    else genmttbar_sel.reset(new uhh2::AndSelection(ctx));

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      nmu_gen.reset(new TTbarSemilep(ctx, 0));
      nele_gen.reset(new TTbarSemilep(ctx, 1));
      pt_mu_gen.reset(new GenLeptonPtSelection(ctx, 0, 45));
      pt_ele_gen.reset(new GenLeptonPtSelection(ctx, 1, 45));
      pt_topjet_gen.reset(new GenTopJetPtSelection(200));
    }

    met_sel.reset(new METSelection(30, -1));
    njet_sel.reset(new NJetSelection(1, -1, JetId(PtEtaCut(20, 2.4))));
    nmu_sel.reset(new NMuonSelection(1, -1, MuonId(PtEtaCut(45, 2.4))));
    nele_sel.reset(new NElectronSelection(1, -1, ElectronId(PtEtaCut(45, 2.4))));

    // 3. Set up Hists classes:
    h_gen_nocuts.reset(new GenHists(ctx, "gen_nocuts"));
    h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
    h_gen_nele.reset(new GenHists(ctx, "gen_nele"));
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));
    h_gen_pt_ele.reset(new GenHists(ctx, "gen_pt_ele"));
    h_gen_pt_topjet_mu.reset(new GenHists(ctx, "gen_pt_topjet_mu"));
    h_gen_pt_topjet_ele.reset(new GenHists(ctx, "gen_pt_topjet_ele"));

    h_nocuts.reset(new TopSubstructureRecoHists(ctx, "nocuts"));
    h_njet.reset(new TopSubstructureRecoHists(ctx, "njet"));
    h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
    h_nele.reset(new TopSubstructureRecoHists(ctx, "nele"));
    h_met_mu.reset(new TopSubstructureRecoHists(ctx, "met_mu"));
    h_met_ele.reset(new TopSubstructureRecoHists(ctx, "met_ele"));
  }


  bool PreSelectionModule::process(Event & event) {
    cout << "PreSelectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    h_nocuts->fill(event);
    if(isTTbar) h_gen_nocuts->fill(event);

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
      if(passed_nmu_gen) h_gen_nmu->fill(event);
      if(passed_nele_gen) h_gen_nele->fill(event);

      passed_mu_pt_gen = pt_mu_gen->passes(event);
      passed_ele_pt_gen = pt_ele_gen->passes(event);
      if(passed_nmu_gen && passed_mu_pt_gen) h_gen_pt_mu->fill(event);
      if(passed_nele_gen && passed_ele_pt_gen) h_gen_pt_ele->fill(event);

      passed_topjet_pt_gen = pt_topjet_gen->passes(event);
      if(passed_nmu_gen && passed_mu_pt_gen && passed_topjet_pt_gen) h_gen_pt_topjet_mu->fill(event);
      if(passed_nele_gen && passed_ele_pt_gen && passed_topjet_pt_gen) h_gen_pt_topjet_ele->fill(event);

      passed_mu_gen = (passed_nmu_gen && passed_mu_pt_gen && passed_topjet_pt_gen);
      passed_ele_gen = (passed_nele_gen && passed_ele_pt_gen && passed_topjet_pt_gen);
    }

    passed_njet_rec = njet_sel->passes(event);
    if(passed_njet_rec) h_njet->fill(event);

    if(isMu || isMC) passed_nmu_rec = nmu_sel->passes(event);
    if(isEle || isMC) passed_nele_rec = nele_sel->passes(event);
    if((isMu || isMC) && passed_njet_rec && passed_nmu_rec)  h_nmu->fill(event);
    if((isEle || isMC) && passed_njet_rec && passed_nele_rec) h_nele->fill(event);

    passed_met_rec = met_sel->passes(event);
    if((isMu || isMC) && passed_njet_rec && passed_nmu_rec && passed_met_rec) h_met_mu->fill(event);
    if((isEle || isMC) && passed_njet_rec && passed_nele_rec && passed_met_rec) h_met_ele->fill(event);

    passed_mu_rec = (isMu || isMC) && passed_njet_rec && passed_nmu_rec && passed_met_rec;
    passed_ele_rec = (isEle || isMC) && passed_njet_rec && passed_nele_rec && passed_met_rec;

    if(!passed_mu_rec && !passed_ele_rec && !passed_mu_gen && !passed_ele_gen) return false;

    // 3. decide whether or not to keep the current event in the output:
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
