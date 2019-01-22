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
    bool passed, passed_trig;
    bool passed_gen, passed_gen_sel, passed_rec;
    bool matched_gen;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<TopJetCleaner> topjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> nbtag_medium_sel, twodcut_sel, met_sel, nmu_sel, nele_sel, pt_mu_sel;
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel;
    std::unique_ptr<Selection> genmatching, nmu_gen;

    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> cleaner, rectopjetleptoncleaner;
    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_nmu, h_gen_nmu_matched, h_gen_nmu_unmatched;
    std::unique_ptr<Hists> h_start, h_lumi, h_common, h_jetcleaner, h_muoncleaner, h_elecleaner, h_pu, h_trigger;

    std::unique_ptr<Hists> h_pv, h_nmu, h_tjlc, h_tjc, h_met, h_pt_mu, h_nele, h_twodcut, h_nbtag_medium;

    JetId Btag_medium;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;
    uhh2::Event::Handle<bool> h_passed_gen;
    uhh2::Event::Handle<bool> h_passed_rec;
  };


  KinCutModule::KinCutModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");
    h_rec_weight = ctx.declare_event_output<double>("h_rec_weight");
    h_passed_rec = ctx.declare_event_output<bool>("h_passed_rec");
    h_passed_gen = ctx.declare_event_output<bool>("h_passed_gen");

    common.reset(new CommonModules());
    lumiweight.reset(new MCLumiWeight(ctx));
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    common->disable_mclumiweight();
    common->switch_jetlepcleaner();
    common->init(ctx); // always last!


    MuonId muid       = AndId<Muon>(MuonIDTight(), PtEtaCut(55., 2.4));
    ElectronId eleid  = AndId<Electron>(ElectronID_Spring16_medium_noIso, PtEtaCut(55., 2.4));
    Btag_medium        = CSVBTag(CSVBTag::WP_MEDIUM);

    muoSR_cleaner.reset(new MuonCleaner(muid));
    eleSR_cleaner.reset(new ElectronCleaner(eleid));
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4));
    rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner());
    topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5)));


    trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
    trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");

    isMC = (ctx.get("dataset_type") == "MC");
    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft");

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      nmu_gen.reset(new TTbarSemilep(ctx));
      genmatching.reset(new GenMatching(ctx));
    }

    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));
    PUreweight.reset(new MCPileupReweight(ctx, "central"));

    nmu_sel.reset(new NMuonSelection(1,1));
    met_sel.reset(new METSelection(50,-1));
    pt_mu_sel.reset(new MuonptSelection(55));
    nele_sel.reset(new NElectronSelection(0, 0));
    twodcut_sel.reset(new TwoDCut(0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));


    // 3. Set up Hists classes:
    h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
    h_gen_nmu_matched.reset(new GenHists(ctx, "gen_nmu_matched"));
    h_gen_nmu_unmatched.reset(new GenHists(ctx, "gen_nmu_unmatched"));


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
  }


  bool KinCutModule::process(Event & event) {
    cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    h_start->fill(event);
    lumiweight->process(event);
    h_lumi->fill(event);

    event.set(h_gen_weight, event.weight);
    // 1. run all modules other modules.
    passed = false;
    passed_trig = false;
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

    common->process(event);
    h_common->fill(event);
    jetcleaner->process(event);
    h_jetcleaner->fill(event);
    muoSR_cleaner->process(event);
    h_muoncleaner->fill(event);
    eleSR_cleaner->process(event);
    h_elecleaner->fill(event);

    /** PU Reweighting *********************/
    PUreweight->process(event);
    h_pu->fill(event);
    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    if(!isMC && event.run < 274954) {
      passed_trig = trigger_sel_A->passes(event);
    }else{
      passed_trig = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
    }
    if(!passed_trig) return false;
    h_trigger->fill(event);

    event.set(h_rec_weight, event.weight);

    //at least 1 good primary vertex
    passed = pv_sel->passes(event);
    if(passed){
      h_pv->fill(event);
      // exactly 1 muon
      passed = nmu_sel->passes(event);
      if(passed){
        h_nmu->fill(event);

        // remove muon four-vector from topjet four-vector
        rectopjetleptoncleaner->process(event);
        h_tjlc->fill(event);

        topjetcleaner->process(event);
        h_tjc->fill(event);

        // MET
        passed = met_sel->passes(event);
        if(passed){
          h_met->fill(event);
          // pT(muon)
          passed = pt_mu_sel->passes(event);
          if(passed){
            h_pt_mu->fill(event);
            // exactly 0 electron
            passed = nele_sel->passes(event);
            if(passed){
              h_nele->fill(event);
              // TwoDCut
              passed = twodcut_sel->passes(event);
              if(passed){
                h_twodcut->fill(event);
                // at least one tight b-tag
                passed = nbtag_medium_sel->passes(event);
                if(passed){
                  h_nbtag_medium->fill(event);
                  passed_rec = true;
                }
              }
            }
          }
        }
      }
    }
    if(!passed_rec && !passed_gen) return false;
    // 3. decide whether or not to keep the current event in the output:
    event.set(h_passed_rec, passed_rec);
    event.set(h_passed_gen, passed_gen);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
