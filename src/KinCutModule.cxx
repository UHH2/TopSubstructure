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
  class KinCutModule: public AnalysisModule {
  public:
    explicit KinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    std::unique_ptr<CommonModules>   common;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;

    bool isMC, isTTbar;

    //declare generator booleans
    bool passed_gen_pre, passed_rec_pre;
    bool passed_gen, passed_nmu_gen, passed_met_gen, passed_pt_mu_gen;
    bool passed_gen_met_sideband, passed_gen_pt_mu_sideband;

    //declare reconstruction booleans
    bool passed_rec, passed_trigger, passed_pv, passed_met, passed_nmu, passed_nele, passed_pt_mu, passed_twodcut, passed_nbtag_medium;
    bool passed_rec_met_sideband, passed_rec_pt_mu_sideband;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<TopJetCleaner> topjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    //declare generator selections
    std::unique_ptr<Selection> genmatching, nmu_gen, met_gen, pt_mu_gen;
    std::unique_ptr<Selection> gen_met_sideband, gen_pt_mu_sideband;

    //declare reconstruction selections
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel, nbtag_medium_sel, twodcut_sel, met_sel, nmu_sel, nele_sel, pt_mu_sel;
    std::unique_ptr<Selection> rec_met_sideband, rec_pt_mu_sideband;

    //declare general stuff
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> cleaner, rectopjetleptoncleaner;
    std::unique_ptr<AnalysisModule> ttgenprod;
    JetId Btag_medium;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    //declare generator histograms
    std::unique_ptr<Hists> h_gen_nmu, h_gen_met, h_gen_pt_mu, h_gen_met_sideband, h_gen_pt_mu_sideband, h_gen_matched, h_gen_unmatched;

    //declare reconstruction histograms
    std::unique_ptr<Hists> h_start, h_lumi, h_common, h_jetcleaner, h_muoncleaner, h_elecleaner, h_pu, h_trigger, h_pv, h_nmu, h_tjlc, h_tjc, h_met, h_pt_mu, h_nele, h_twodcut, h_nbtag_medium, h_rec_met_sideband, h_rec_pt_mu_sideband;

    //declare handles
    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;
    uhh2::Event::Handle<bool> h_passed_gen_pre, h_passed_rec_pre, h_passed_gen, h_passed_rec, h_passed_rec_met_sideband, h_passed_rec_pt_mu_sideband, h_passed_gen_met_sideband, h_passed_gen_pt_mu_sideband;
  };


  KinCutModule::KinCutModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");
    h_rec_weight = ctx.declare_event_output<double>("h_rec_weight");
    h_passed_gen_pre = ctx.get_handle<bool>("h_passed_gen_pre");
    h_passed_rec_pre = ctx.get_handle<bool>("h_passed_rec_pre");
    h_passed_rec = ctx.declare_event_output<bool>("h_passed_rec");
    h_passed_gen = ctx.declare_event_output<bool>("h_passed_gen");
    h_passed_rec_met_sideband = ctx.declare_event_output<bool>("h_passed_rec_met_sideband");
    h_passed_rec_pt_mu_sideband = ctx.declare_event_output<bool>("h_passed_rec_pt_mu_sideband");
    h_passed_gen_met_sideband = ctx.declare_event_output<bool>("h_passed_gen_met_sideband");
    h_passed_gen_pt_mu_sideband = ctx.declare_event_output<bool>("h_passed_gen_pt_mu_sideband");

    common.reset(new CommonModules());
    lumiweight.reset(new MCLumiWeight(ctx));
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    common->disable_mclumiweight();
    common->switch_jetlepcleaner();

    // common->disable_jersmear();
    // common->disable_jec();
    common->init(ctx); // always last!


    MuonId muid       = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(55., 2.4));
    ElectronId eleid  = AndId<Electron>(ElectronID_Summer16_medium_noIso, PtEtaCut(55., 2.4));
    Btag_medium        = CSVBTag(CSVBTag::WP_MEDIUM);

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
      nmu_gen.reset(new TTbarSemilep(ctx));
      met_gen.reset(new METSelection(50,-1));
      pt_mu_gen.reset(new MuonptSelection(55));
      genmatching.reset(new GenMatching(ctx));
      gen_met_sideband.reset(new GenMETSelection(ctx, 40));
      gen_pt_mu_sideband.reset(new GenMuonPtSelection(ctx, 40));
    }

    //setup reconstruction selections
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));
    PUreweight.reset(new MCPileupReweight(ctx, "central"));

    nmu_sel.reset(new NMuonSelection(1,1));
    met_sel.reset(new METSelection(50,-1));
    pt_mu_sel.reset(new MuonptSelection(55));
    nele_sel.reset(new NElectronSelection(0, 0));
    twodcut_sel.reset(new TwoDCut(0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));

    rec_met_sideband.reset(new METSelection(40,-1));
    rec_pt_mu_sideband.reset(new MuonptSelection(40));


    // 3. Set up Hists classes:
    h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
    h_gen_met.reset(new GenHists(ctx, "gen_met"));
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));
    h_gen_matched.reset(new GenHists(ctx, "gen_matched"));
    h_gen_unmatched.reset(new GenHists(ctx, "gen_unmatched"));
    h_gen_met_sideband.reset(new GenHists(ctx, "gen_met_sideband"));
    h_gen_pt_mu_sideband.reset(new GenHists(ctx, "gen_pt_mu_sideband"));


    h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
    h_lumi.reset(new TopSubstructureRecoHists(ctx, "lumi"));
    h_common.reset(new TopSubstructureRecoHists(ctx, "common"));
    h_jetcleaner.reset(new TopSubstructureRecoHists(ctx, "jetcleaner"));
    h_muoncleaner.reset(new TopSubstructureRecoHists(ctx, "muoncleaner"));
    h_elecleaner.reset(new TopSubstructureRecoHists(ctx, "elecleaner"));
    h_pu.reset(new TopSubstructureRecoHists(ctx, "pu"));
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
    h_rec_met_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_met_sideband"));
    h_rec_pt_mu_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_mu_sideband"));
  }


  bool KinCutModule::process(Event & event) {
    cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    h_start->fill(event);
    lumiweight->process(event);
    h_lumi->fill(event);

    event.set(h_gen_weight, event.weight);
    // 1. run all modules other modules.
    passed_gen                = false;  //passed full gen selection
    passed_rec                = false;  //passed full rec selection
    if(event.is_valid(h_passed_gen_pre)) passed_gen_pre = event.get(h_passed_gen_pre);
    else passed_gen_pre = false;
    if(event.is_valid(h_passed_rec_pre)) passed_rec_pre = event.get(h_passed_rec_pre);
    else passed_rec_pre = false;
    passed_nmu_gen            = false;  //passed nmu gen selection
    passed_met_gen            = false;  //passed met gen selection
    passed_pt_mu_gen          = false;  //passed pt muon gen selection
    passed_gen_met_sideband   = false;  //passed met gen sideband selection -- failed met gen selection, but passed all other cuts
    passed_rec_met_sideband   = false;  //passed met rec sideband selection -- failed met rec selection, but passed all other cuts
    passed_gen_pt_mu_sideband = false;  //passed pt muon gen sideband selection -- failed pt muon gen selection, but passed all other cuts
    passed_rec_pt_mu_sideband = false;  //passed pt muon rec sideband selection -- failed pt muon rec selection, but passed all other cuts
    passed_trigger            = false;
    passed_pv                 = false;
    passed_nmu                = false;
    passed_met                = false;
    passed_pt_mu              = false;
    passed_nele               = false;
    passed_twodcut            = false;
    passed_nbtag_medium       = false;

    if(isTTbar){
      ttgenprod->process(event);
      passed_nmu_gen = nmu_gen->passes(event);
      passed_met_gen = met_gen->passes(event);
      passed_pt_mu_gen = pt_mu_gen->passes(event);

      if(passed_gen_pre && passed_nmu_gen){
        h_gen_nmu->fill(event);
      }
      if(passed_gen_pre && passed_nmu_gen && passed_met_gen){
        h_gen_met->fill(event);
      }
      if(passed_gen_pre && passed_nmu_gen && passed_met_gen && passed_pt_mu_gen){
        h_gen_pt_mu->fill(event);
        passed_gen = true;
        if(genmatching->passes(event)) h_gen_matched->fill(event);
        else h_gen_unmatched->fill(event);
      }
      if(passed_gen_pre && passed_nmu_gen && !passed_met_gen && passed_pt_mu_gen && gen_met_sideband->passes(event)){
        h_gen_met_sideband->fill(event);
        passed_gen_met_sideband = true;
      }
      if(passed_gen_pre && passed_nmu_gen && passed_met_gen && !passed_pt_mu_gen && gen_pt_mu_sideband->passes(event)){
        h_gen_pt_mu_sideband->fill(event);
        passed_gen_pt_mu_sideband = true;
      }
    }

    common->process(event);
    h_common->fill(event);

    jetcleaner->process(event);
    h_jetcleaner->fill(event);

    muoSR_cleaner->process(event);
    h_muoncleaner->fill(event);

    eleSR_cleaner->process(event);
    h_elecleaner->fill(event);

    /** PU Reweighting *********************/
    PUreweight->process(event); //evtl spaeter fuer unsicherheit
    h_pu->fill(event);

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    if(!isMC && event.run < 274954) {
      passed_trigger = trigger_sel_A->passes(event);
    }else{
      passed_trigger = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
    }
    //at least 1 good primary vertex
    passed_pv = pv_sel->passes(event);

    // exactly 1 muon
    passed_nmu = nmu_sel->passes(event);
    // remove muon four-vector from topjet four-vector
    rectopjetleptoncleaner->process(event);
    h_tjlc->fill(event);

    topjetcleaner->process(event);
    h_tjc->fill(event);

    // MET
    passed_met = met_sel->passes(event);

    // pT(muon)
    passed_pt_mu = pt_mu_sel->passes(event);

    // exactly 0 electron
    passed_nele = nele_sel->passes(event);

    // TwoDCut
    passed_twodcut = twodcut_sel->passes(event);

    // at least one medium b-tag
    passed_nbtag_medium = nbtag_medium_sel->passes(event);

    event.set(h_rec_weight, event.weight);

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

    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && !passed_met && passed_pt_mu && passed_nele && passed_twodcut && passed_nbtag_medium && rec_met_sideband->passes(event)){
      h_rec_met_sideband->fill(event);
      passed_rec_met_sideband = true;
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && !passed_pt_mu && passed_nele && passed_twodcut && passed_nbtag_medium && rec_pt_mu_sideband->passes(event)){
      h_rec_pt_mu_sideband->fill(event);
      passed_rec_pt_mu_sideband = true;
    }

    if(!passed_rec_pre && !passed_gen_pre &&  !passed_rec && !passed_gen && !passed_gen_met_sideband && !passed_gen_pt_mu_sideband && !passed_rec_met_sideband && !passed_rec_pt_mu_sideband) return false;

    event.set(h_passed_rec, passed_rec);
    event.set(h_passed_gen, passed_gen);
    event.set(h_passed_rec_met_sideband, passed_rec_met_sideband);
    event.set(h_passed_rec_pt_mu_sideband, passed_rec_pt_mu_sideband);
    event.set(h_passed_gen_met_sideband, passed_gen_met_sideband);
    event.set(h_passed_gen_pt_mu_sideband, passed_gen_pt_mu_sideband);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
