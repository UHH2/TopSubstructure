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
#include "UHH2/TopSubstructure/include/TopJetCorrections.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class KinCutModule: public AnalysisModule {
  public:
    explicit KinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    std::unique_ptr<CommonModules>   common;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;

    bool isMC, isTTbar;
    std::unique_ptr<TopJetCorrections> topjetCorr;

    //declare generator booleans
    bool passed_gen_pre, passed_rec_pre;
    bool passed_gen, passed_nmu_gen, passed_pt_mu_gen;

    //declare reconstruction booleans
    bool passed_rec, passed_trigger, passed_pv, passed_met, passed_nmu, passed_nele, passed_pt_mu, passed_twodcut, passed_nbtag_medium;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<TopJetCleaner> topjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    //declare generator selections
    std::unique_ptr<Selection> met_gen, pt_mu_gen;

    //declare reconstruction selections
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel, nbtag_medium_sel, twodcut_sel, met_sel, nmu_sel, nele_sel, pt_mu_sel;

    //declare general stuff
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> cleaner, rectopjetleptoncleaner;
    std::unique_ptr<AnalysisModule> ttgenprod;
    JetId Btag_medium;
    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    //declare generator histograms
    std::unique_ptr<Hists> h_gen_pt_mu;

    //declare reconstruction histograms
    std::unique_ptr<Hists> h_start, h_common, h_topjetcorrections, h_topjetjer_smearing, h_jetcleaner, h_muoncleaner, h_elecleaner, h_trigger, h_pv, h_nmu, h_tjlc, h_tjc, h_met, h_pt_mu, h_nele, h_twodcut, h_nbtag_medium;

    //declare handles
    uhh2::Event::Handle<double> h_rec_weight_kin;
    uhh2::Event::Handle<double> h_gen_weight_kin;
    uhh2::Event::Handle<bool> h_passed_gen_pre, h_passed_rec_pre, h_passed_gen, h_passed_rec;
  };


  KinCutModule::KinCutModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight_kin = ctx.declare_event_output<double>("h_gen_weight_kin");
    h_rec_weight_kin = ctx.declare_event_output<double>("h_rec_weight_kin");
    h_passed_gen_pre = ctx.get_handle<bool>("h_passed_gen_pre");
    h_passed_rec_pre = ctx.get_handle<bool>("h_passed_rec_pre");
    h_passed_rec = ctx.declare_event_output<bool>("h_passed_rec");
    h_passed_gen = ctx.declare_event_output<bool>("h_passed_gen");

    common.reset(new CommonModules());
    // lumiweight.reset(new MCLumiWeight(ctx));
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    topjetCorr.reset(new TopJetCorrections());
    topjetCorr->init(ctx);

    common->disable_mclumiweight();
    common->switch_jetlepcleaner();
    common->disable_mcpileupreweight();
    common->init(ctx); // always last!


    MuonId muid       = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(55., 2.4));
    ElectronId eleid  = AndId<Electron>(ElectronID_Summer16_medium_noIso, PtEtaCut(55., 2.4));
    Btag_medium        = CSVBTag(CSVBTag::WP_MEDIUM);

    cleaner.reset(new GenTopJetLeptonCleaner(ctx));
    gentopjetcleaner.reset(new GenTopJetCleaner(ctx));
    muoSR_cleaner.reset(new MuonCleaner(muid));
    eleSR_cleaner.reset(new ElectronCleaner(eleid));
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4));
    rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner());
    topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5)));


    trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
    trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");

    isMC = (ctx.get("dataset_type") == "MC");
    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3");

    // 2. set up selections

    //setup generator selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      pt_mu_gen.reset(new GenMuonPtSelection(ctx, 55));
    }

    //setup reconstruction selections
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));

    nmu_sel.reset(new NMuonSelection(1,1));
    met_sel.reset(new METSelection(50,-1));
    pt_mu_sel.reset(new MuonptSelection(55));
    nele_sel.reset(new NElectronSelection(0, 0));
    twodcut_sel.reset(new TwoDCut(0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));


    // 3. Set up Hists classes:
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));

    h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
    h_common.reset(new TopSubstructureRecoHists(ctx, "common"));
    h_topjetcorrections.reset(new TopSubstructureRecoHists(ctx, "topjetcorrections"));
    h_jetcleaner.reset(new TopSubstructureRecoHists(ctx, "jetcleaner"));
    h_muoncleaner.reset(new TopSubstructureRecoHists(ctx, "muoncleaner"));
    h_elecleaner.reset(new TopSubstructureRecoHists(ctx, "elecleaner"));
    h_trigger.reset(new TopSubstructureRecoHists(ctx, "trigger"));

    h_pv.reset(new TopSubstructureRecoHists(ctx, "pv"));
    h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
    h_tjlc.reset(new TopSubstructureRecoHists(ctx, "tjlc"));
    h_tjc.reset(new TopSubstructureRecoHists(ctx, "tjc"));
    h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
    h_pt_mu.reset(new TopSubstructureRecoHists(ctx, "pt_mu"));
    h_nele.reset(new TopSubstructureRecoHists(ctx, "nele"));
    h_twodcut.reset(new TopSubstructureRecoHists(ctx, "twodcut"));
    h_nbtag_medium.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium"));
  }


  bool KinCutModule::process(Event & event) {
    cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    event.set(h_gen_weight_kin, event.weight);

    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen_pre)) passed_gen_pre = event.get(h_passed_gen_pre);
    else passed_gen_pre = false;
    passed_pt_mu_gen    = false;  // passed pt muon gen selection
    passed_gen          = false;  // passed full gen selection

    if(isTTbar){
      ttgenprod->process(event);
      if(passed_gen_pre){
        cleaner->process(event); // Do this always!
        gentopjetcleaner->process(event);
      }
      passed_pt_mu_gen = pt_mu_gen->passes(event);
      if(passed_gen_pre && passed_pt_mu_gen){
        h_gen_pt_mu->fill(event);
        passed_gen = true;
      }
    }

    /*
    ██████  ███████  ██████  ██████
    ██   ██ ██      ██      ██    ██
    ██████  █████   ██      ██    ██
    ██   ██ ██      ██      ██    ██
    ██   ██ ███████  ██████  ██████
    */

    if(event.is_valid(h_passed_rec_pre)) passed_rec_pre = event.get(h_passed_rec_pre);
    else passed_rec_pre = false;
    passed_trigger      = false;
    passed_pv           = false;
    passed_nmu          = false;
    passed_met          = false;
    passed_pt_mu        = false;
    passed_nele         = false;
    passed_twodcut      = false;
    passed_nbtag_medium = false;
    passed_rec          = false;  // passed full rec selection

    if(!common->process(event)) return false;
    h_common->fill(event);

    topjetCorr->process(event);
    h_topjetcorrections->fill(event);

    jetcleaner->process(event);
    h_jetcleaner->fill(event);

    muoSR_cleaner->process(event);
    h_muoncleaner->fill(event);

    eleSR_cleaner->process(event);
    h_elecleaner->fill(event);

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    if(!isMC && event.run < 274954) {
      passed_trigger = trigger_sel_A->passes(event);
    }else{
      passed_trigger = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
    }
    passed_pv = pv_sel->passes(event); //at least 1 good primary vertex
    passed_nmu = nmu_sel->passes(event); // exactly 1 muon

    rectopjetleptoncleaner->process(event); // remove muon four-vector from topjet four-vector
    h_tjlc->fill(event);
    topjetcleaner->process(event);
    h_tjc->fill(event);

    event.set(h_rec_weight_kin, event.weight);

    passed_met = met_sel->passes(event); // MET
    passed_pt_mu = pt_mu_sel->passes(event); // pT(muon)
    passed_nele = nele_sel->passes(event); // exactly 0 electron
    passed_twodcut = twodcut_sel->passes(event); // TwoDCut
    passed_nbtag_medium = nbtag_medium_sel->passes(event); // at least one medium b-tag

    if(passed_rec_pre && passed_trigger){
      h_trigger->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv){
      h_pv->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu){
      h_nmu->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met){
      h_met->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu){
      h_pt_mu->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele){
      h_nele->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele && passed_twodcut){
      h_twodcut->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele && passed_twodcut && passed_nbtag_medium){
      h_nbtag_medium->fill(event);
      passed_rec = true;
    }

    if(!passed_rec && !passed_gen) return false;

    event.set(h_passed_rec, passed_rec);
    event.set(h_passed_gen, passed_gen);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
