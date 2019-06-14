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
#include "UHH2/common/include/TTbarGenHists.h"

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
  class PostKinCutModule: public AnalysisModule {
  public:
    explicit PostKinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool isTTbar, isPseudoData, isMC;
    bool passed_rec, passed_gen;
    bool matched_rec, matched_gen;

    bool passed_gen_ntopjet, passed_gen_dr, passed_gen_mass, passed_gen_pt_topjet, passed_gen_pt_mu;
    bool passed_rec_ntopjet, passed_rec_dr, passed_rec_mass, passed_rec_pt_topjet, passed_rec_pt_mu;

    bool passed_rec_met_sideband, passed_rec_pt_topjet_mu_sideband, passed_gen_met_sideband, passed_gen_pt_topjet_mu_sideband;
    bool passed_rec_ntopjet_sideband_l2, passed_rec_ntopjet_sideband_g2, passed_rec_dr_sideband, passed_rec_mass_sideband, passed_rec_pt_topjet_sideband;
    bool passed_gen_ntopjet_sideband_l2, passed_gen_ntopjet_sideband_g2, passed_gen_dr_sideband, passed_gen_mass_sideband, passed_gen_pt_topjet_sideband;
    bool passed_rec_pt_mu_sideband, passed_gen_pt_mu_sideband;
    bool passed_gen_final, passed_rec_final;



    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> ntopjet2_sel;
    std::unique_ptr<Selection> pt_topjet_sel1;
    std::unique_ptr<Selection> mass_sel1, dr_sel;
    std::unique_ptr<Selection> dr_gen_sideband, pt_topjet_gen_sideband, pt_mu_gen_sideband;
    std::unique_ptr<Selection> dr_rec_sideband, pt_topjet_rec_sideband, pt_mu_rec_sideband;
    std::unique_ptr<Selection> mass_lower, mass_higher;
    std::unique_ptr<Selection> gen_mass_lower, gen_mass_higher;

    std::unique_ptr<Selection> pt_topjet_gen, ntopjet2_gen, dr_gen, mass_gen, genmatching, pt_mu_gen, pt_mu_sel;
    std::unique_ptr<Selection> recmatching;

    std::unique_ptr<AnalysisModule> scale_variation;
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> ttgenprod;
    calc_Nsubjettiness calc_pf_tau3, calc_gen_tau3;
    calc_Nsubjettiness calc_pf_tau2, calc_gen_tau2;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_pt_topjet, h_gen_pt_topjet_matched, h_gen_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_gen_pt_mu, h_gen_pt_mu_matched, h_gen_pt_mu_unmatched;
    std::unique_ptr<Hists> h_gen_ntopjet2, h_gen_ntopjet2_matched, h_gen_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_gen_dr, h_gen_dr_matched, h_gen_dr_unmatched;
    std::unique_ptr<Hists> h_gen_mass, h_gen_mass_matched, h_gen_mass_unmatched;

    std::unique_ptr<Hists> h_pt_mu, h_pt_mu_matched, h_pt_mu_unmatched;
    std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_pt_topjet, h_pt_topjet_matched, h_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_dr, h_dr_matched, h_dr_unmatched;
    std::unique_ptr<Hists> h_mass, h_mass_matched, h_mass_unmatched;
    std::unique_ptr<Hists> h_mass_lower, h_mass_lower_matched, h_mass_lower_unmatched;
    std::unique_ptr<Hists> h_mass_higher, h_mass_higher_matched, h_mass_higher_unmatched;
    std::unique_ptr<Hists> h_gen_mass_lower, h_gen_mass_lower_matched, h_gen_mass_lower_unmatched;
    std::unique_ptr<Hists> h_gen_mass_higher, h_gen_mass_higher_matched, h_gen_mass_higher_unmatched;
    std::unique_ptr<Hists> h_passedgen_rec, h_passedrec_gen;
    std::unique_ptr<Hists> h_ttbar_hist;

    std::unique_ptr<Hists> h_gen_pt_topjet_sideband, h_gen_dr_sideband, h_gen_mass_sideband, h_gen_pt_mu_sideband;
    std::unique_ptr<Hists> h_rec_pt_topjet_sideband, h_rec_dr_sideband, h_rec_mass_sideband, h_rec_pt_mu_sideband;

    // JetId Btag_tight;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;
    uhh2::Event::Handle<double> h_rec_weight_kin;
    uhh2::Event::Handle<double> h_gen_weight_kin;

    uhh2::Event::Handle<bool> h_passed_rec, h_passed_rec_final;
    uhh2::Event::Handle<bool> h_passed_gen, h_passed_gen_final;

    uhh2::Event::Handle<bool> h_passed_gen_pt_mu_sideband, h_passed_gen_dr_sideband, h_passed_gen_mass_sideband, h_passed_gen_pt_topjet_sideband, h_passed_rec_dr_sideband, h_passed_rec_mass_sideband, h_passed_rec_pt_mu_sideband, h_passed_rec_pt_topjet_sideband;


    uhh2::Event::Handle<double> h_pt_rec;
    uhh2::Event::Handle<double> h_tau32_rec;
    uhh2::Event::Handle<double> h_mass_rec;
    uhh2::Event::Handle<double> h_pt_gen;
    uhh2::Event::Handle<double> h_tau32_gen;
    uhh2::Event::Handle<double> h_mass_gen;
    uhh2::Event::Handle<double> h_calc_tau32_gen, h_calc_tau32_rec;

    bool derror;
  };


  PostKinCutModule::PostKinCutModule(Context & ctx){
    // Btag_tight        = CSVBTag(CSVBTag::WP_TIGHT);
    ctx.undeclare_all_event_output();
    derror = false;

    h_passed_rec                    = ctx.get_handle<bool>("h_passed_rec");
    h_passed_gen                    = ctx.get_handle<bool>("h_passed_gen");
    h_rec_weight_kin                = ctx.get_handle<double>("h_rec_weight_kin");
    h_gen_weight_kin                = ctx.get_handle<double>("h_gen_weight_kin");
    h_passed_rec_final              = ctx.declare_event_output<bool>("h_passed_rec_final");
    h_passed_gen_final              = ctx.declare_event_output<bool>("h_passed_gen_final");
    h_pt_rec                        = ctx.declare_event_output<double>("h_pt_rec");
    h_tau32_rec                     = ctx.declare_event_output<double>("h_tau32_rec");
    h_mass_rec                      = ctx.declare_event_output<double>("h_mass_rec");
    h_pt_gen                        = ctx.declare_event_output<double>("h_pt_gen");
    h_tau32_gen                     = ctx.declare_event_output<double>("h_tau32_gen");
    h_mass_gen                      = ctx.declare_event_output<double>("h_mass_gen");
    h_gen_weight                    = ctx.declare_event_output<double>("h_gen_weight");
    h_rec_weight                    = ctx.declare_event_output<double>("h_rec_weight");
    h_passed_gen_pt_mu_sideband     = ctx.declare_event_output<bool>("h_passed_gen_pt_mu_sideband");
    h_passed_gen_dr_sideband        = ctx.declare_event_output<bool>("h_passed_gen_dr_sideband");
    h_passed_gen_mass_sideband      = ctx.declare_event_output<bool>("h_passed_gen_mass_sideband");
    h_passed_gen_pt_topjet_sideband = ctx.declare_event_output<bool>("h_passed_gen_pt_topjet_sideband");
    h_passed_rec_dr_sideband        = ctx.declare_event_output<bool>("h_passed_rec_dr_sideband");
    h_passed_rec_mass_sideband      = ctx.declare_event_output<bool>("h_passed_rec_mass_sideband");
    h_passed_rec_pt_mu_sideband     = ctx.declare_event_output<bool>("h_passed_rec_pt_mu_sideband");
    h_passed_rec_pt_topjet_sideband = ctx.declare_event_output<bool>("h_passed_rec_pt_topjet_sideband");
    h_calc_tau32_gen                = ctx.declare_event_output<double>("h_calc_tau32_gen");
    h_calc_tau32_rec                = ctx.declare_event_output<double>("h_calc_tau32_rec");

    //scale variation
    scale_variation.reset(new MCScaleVariation(ctx));
    lumiweight.reset(new MCLumiWeight(ctx));
    PUreweight.reset(new MCPileupReweight(ctx, "central"));


    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3");
    isPseudoData = (ctx.get("dataset_version") == "PseudoData");
    isMC = (ctx.get("dataset_type") == "MC");

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));

      pt_mu_gen.reset(new GenMuonPtSelection(ctx, 60));
      ntopjet2_gen.reset(new GenNTopJet(2,2));
      dr_gen.reset(new dRSelection(ctx));
      mass_gen.reset(new MassSelection(ctx, 1));
      pt_topjet_gen.reset(new PtSelection(400, 200));

      pt_mu_gen_sideband.reset(new GenMuonPtSelection(ctx, 55));
      dr_gen_sideband.reset(new dRSelection(ctx, 1.0));
      pt_topjet_gen_sideband.reset(new PtSelection(300, 200));

      gen_mass_lower.reset(new MassTopJet(0., 155.));
      gen_mass_higher.reset(new MassTopJet(155.));

      genmatching.reset(new GenMatching(ctx));
      recmatching.reset(new RecMatching(ctx));
    }
    // calculator_tau.reset(new calc_Nsubjettiness());
    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    mass_sel1.reset(new RecMassSelection(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
    dr_sel.reset(new RecdRSelection());
    pt_topjet_sel1.reset(new RecPtSelection(400, 200));
    pt_mu_sel.reset(new MuonptSelection(60));
    mass_lower.reset(new MassTopJet(0., 152.));
    mass_higher.reset(new MassTopJet(152.));

    pt_mu_rec_sideband.reset(new MuonptSelection(55));
    dr_rec_sideband.reset(new RecdRSelection(1.0));
    pt_topjet_rec_sideband.reset(new RecPtSelection(300, 200));


    // 3. Set up Hists classes:
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));
    h_gen_pt_mu_matched.reset(new GenHists(ctx, "gen_pt_mu_matched"));
    h_gen_pt_mu_unmatched.reset(new GenHists(ctx, "gen_pt_mu_unmatched"));

    h_gen_pt_topjet.reset(new GenHists(ctx, "gen_pt_topjet"));
    h_gen_pt_topjet_matched.reset(new GenHists(ctx, "gen_pt_topjet_matched"));
    h_gen_pt_topjet_unmatched.reset(new GenHists(ctx, "gen_pt_topjet_unmatched"));

    h_gen_ntopjet2.reset(new GenHists(ctx, "gen_ntopjet2"));
    h_gen_ntopjet2_matched.reset(new GenHists(ctx, "gen_ntopjet2_matched"));
    h_gen_ntopjet2_unmatched.reset(new GenHists(ctx, "gen_ntopjet2_unmatched"));

    h_gen_dr.reset(new GenHists(ctx, "gen_dr"));
    h_gen_dr_matched.reset(new GenHists(ctx, "gen_dr_matched"));
    h_gen_dr_unmatched.reset(new GenHists(ctx, "gen_dr_unmatched"));

    h_gen_mass.reset(new GenHists(ctx, "gen_mass"));
    h_gen_mass_matched.reset(new GenHists(ctx, "gen_mass_matched"));
    h_gen_mass_unmatched.reset(new GenHists(ctx, "gen_mass_unmatched"));

    h_gen_pt_topjet_sideband.reset(new GenHists(ctx, "gen_pt_topjet_sideband"));
    h_gen_pt_mu_sideband.reset(new GenHists(ctx, "gen_pt_mu_sideband"));
    h_gen_dr_sideband.reset(new GenHists(ctx, "gen_dr_sideband"));
    h_gen_mass_sideband.reset(new GenHists(ctx, "gen_mass_sideband"));

    h_pt_mu.reset(new TopSubstructureRecoHists(ctx, "pt_mu"));
    h_pt_mu_matched.reset(new TopSubstructureRecoHists(ctx, "pt_mu_matched"));
    h_pt_mu_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_mu_unmatched"));

    h_pt_topjet.reset(new TopSubstructureRecoHists(ctx, "pt_topjet"));
    h_pt_topjet_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_matched"));
    h_pt_topjet_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_unmatched"));

    h_ntopjet2.reset(new TopSubstructureRecoHists(ctx, "ntopjet2"));
    h_ntopjet2_matched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_matched"));
    h_ntopjet2_unmatched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_unmatched"));

    h_dr.reset(new TopSubstructureRecoHists(ctx, "dr"));
    h_dr_matched.reset(new TopSubstructureRecoHists(ctx, "dr_matched"));
    h_dr_unmatched.reset(new TopSubstructureRecoHists(ctx, "dr_unmatched"));

    h_mass.reset(new TopSubstructureRecoHists(ctx, "mass"));
    h_mass_matched.reset(new TopSubstructureRecoHists(ctx, "mass_matched"));
    h_mass_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass_unmatched"));

    h_passedgen_rec.reset(new TopSubstructureRecoHists(ctx, "passedgen_rec"));
    h_passedrec_gen.reset(new GenHists(ctx, "passedrec_gen"));

    h_mass_lower.reset(new TopSubstructureRecoHists(ctx, "mass_lower"));
    h_mass_lower_matched.reset(new TopSubstructureRecoHists(ctx, "mass_lower_matched"));
    h_mass_lower_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass_lower_unmatched"));

    h_mass_higher.reset(new TopSubstructureRecoHists(ctx, "mass_higher"));
    h_mass_higher_matched.reset(new TopSubstructureRecoHists(ctx, "mass_higher_matched"));
    h_mass_higher_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass_higher_unmatched"));

    h_gen_mass_lower.reset(new GenHists(ctx, "gen_mass_lower"));
    h_gen_mass_lower_matched.reset(new GenHists(ctx, "gen_mass_lower_matched"));
    h_gen_mass_lower_unmatched.reset(new GenHists(ctx, "gen_mass_lower_unmatched"));

    h_gen_mass_higher.reset(new GenHists(ctx, "gen_mass_higher"));
    h_gen_mass_higher_matched.reset(new GenHists(ctx, "gen_mass_higher_matched"));
    h_gen_mass_higher_unmatched.reset(new GenHists(ctx, "gen_mass_higher_unmatched"));

    h_rec_pt_mu_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_mu_sideband"));
    h_rec_pt_topjet_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband"));
    h_rec_dr_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband"));
    h_rec_mass_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband"));

    h_ttbar_hist.reset(new TTbarGenHists(ctx, "ttbar_hist"));
  }


  bool PostKinCutModule::process(Event & event) {
    cout << "PostKinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    if(event.is_valid(h_gen_weight_kin)) event.weight = event.get(h_gen_weight_kin);
    lumiweight->process(event);
    scale_variation->process(event); // here, it is only executed to be filled into the gen weight is has to be done again to appear in the event.weight


    event.set(h_gen_weight, event.weight);
    if(event.is_valid(h_rec_weight_kin)) event.weight *= event.get(h_rec_weight_kin);

    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen)) passed_gen = event.get(h_passed_gen);
    else passed_gen                    = false;
    passed_gen_pt_mu                   = false;  //passed pt muon gen selection
    passed_gen_ntopjet                 = false;
    passed_gen_dr                      = false;
    passed_gen_mass                    = false;
    passed_gen_pt_topjet               = false;
    passed_gen_pt_mu_sideband          = false;
    passed_gen_dr_sideband             = false;
    passed_gen_mass_sideband           = false;
    passed_gen_pt_topjet_sideband      = false;
    passed_gen_final                   = false;
    matched_gen                        = false;

    if(event.is_valid(h_passed_rec)) passed_rec = event.get(h_passed_rec);
    else passed_rec                    = false;
    // passed_rec_ntopjet_sideband_l2  = false;
    // passed_rec_ntopjet_sideband_g2  = false;
    passed_rec_pt_mu                   = false;
    passed_rec_ntopjet                 = false;
    passed_rec_dr                      = false;
    passed_rec_mass                    = false;
    passed_rec_pt_topjet               = false;
    passed_rec_dr_sideband             = false;
    passed_rec_mass_sideband           = false;
    passed_rec_pt_mu_sideband          = false;
    passed_rec_pt_topjet_sideband      = false;
    passed_rec_final                   = false;
    matched_rec                        = false;

    /** PU Reweighting *********************/
    if(isMC) PUreweight->process(event); //evtl spaeter fuer unsicherheit

    event.set(h_rec_weight, event.weight);


    if(isTTbar) {
      ttgenprod->process(event);

      passed_gen_pt_mu = pt_mu_gen->passes(event);
      if(passed_gen && passed_gen_pt_mu){
        h_gen_pt_mu->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_pt_mu_matched->fill(event);
        else h_gen_pt_mu_unmatched->fill(event);
      }

      passed_gen_pt_topjet = pt_topjet_gen->passes(event);
      if(passed_gen && passed_gen_pt_mu && passed_gen_pt_topjet){
        h_gen_pt_topjet->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_pt_topjet_matched->fill(event);
        else h_gen_pt_topjet_unmatched->fill(event);
      }

      passed_gen_ntopjet = ntopjet2_gen->passes(event);
      if(passed_gen && passed_gen_pt_mu && passed_gen_pt_topjet && passed_gen_ntopjet){
        h_gen_ntopjet2->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_ntopjet2_matched->fill(event);
        else h_gen_ntopjet2_unmatched->fill(event);
      }

      passed_gen_dr = dr_gen->passes(event);
      if(passed_gen && passed_gen_pt_mu && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr){
        h_gen_dr->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_dr_matched->fill(event);
        else h_gen_dr_unmatched->fill(event);
      }

      passed_gen_mass = mass_gen->passes(event);
      if(passed_gen && passed_gen_pt_mu && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass){
        h_gen_mass->fill(event);
        h_passedgen_rec->fill(event);
        passed_gen_final = true;
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_mass_matched->fill(event);
        else h_gen_mass_unmatched->fill(event);
      }

      if(passed_gen && !passed_gen_pt_mu && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && pt_mu_gen_sideband->passes(event)){
        h_gen_pt_mu_sideband->fill(event);
        passed_gen_pt_mu_sideband = true;
      }

      if(passed_gen && passed_gen_pt_mu && !passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && pt_topjet_gen_sideband->passes(event)){
        h_gen_pt_topjet_sideband->fill(event);
        passed_gen_pt_topjet_sideband = true;
      }

      if(passed_gen && passed_gen_pt_mu && passed_gen_pt_topjet && passed_gen_ntopjet && !passed_gen_dr && passed_gen_mass && dr_gen_sideband->passes(event)){
        h_gen_dr_sideband->fill(event);
        passed_gen_dr_sideband = true;
      }

      if(passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && !passed_gen_mass){
        h_gen_mass_sideband->fill(event);
        passed_gen_mass_sideband = true;
      }

      if(passed_gen_final && gen_mass_lower->passes(event)){
        h_gen_mass_lower->fill(event);
        if(isTTbar){
          matched_gen = genmatching->passes(event);
          if(matched_gen) h_gen_mass_lower_matched->fill(event);
          else h_gen_mass_lower_unmatched->fill(event);
        }
      }
      if(passed_gen_final && gen_mass_higher->passes(event)){
        h_gen_mass_higher->fill(event);
        if(isTTbar){
          matched_gen = genmatching->passes(event);
          if(matched_gen) h_gen_mass_higher_matched->fill(event);
          else h_gen_mass_higher_unmatched->fill(event);
        }
      }
    }

    /*
    ██████  ███████  ██████  ██████
    ██   ██ ██      ██      ██    ██
    ██████  █████   ██      ██    ██
    ██   ██ ██      ██      ██    ██
    ██   ██ ███████  ██████  ██████
    */

    // calculator_tau->tau_one(event);
    passed_rec_pt_mu = pt_mu_sel->passes(event);
    if(passed_rec && passed_rec_pt_mu){
      h_pt_mu->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_pt_mu_matched->fill(event);
        else h_pt_mu_unmatched->fill(event);
      }
    }


    // pT(first Topjet) > 400
    passed_rec_pt_topjet = pt_topjet_sel1->passes(event);
    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet){
      h_pt_topjet->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_pt_topjet_matched->fill(event);
        else h_pt_topjet_unmatched->fill(event);
      }
    }

    // >= 2 TopJets
    passed_rec_ntopjet = ntopjet2_sel->passes(event);
    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet){
      h_ntopjet2->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_ntopjet2_matched->fill(event);
        else h_ntopjet2_unmatched->fill(event);
      }
    }

    // dR(muon, second TopJet) < 0.8
    passed_rec_dr = dr_sel->passes(event);
    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr){
      h_dr->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_dr_matched->fill(event);
        else h_dr_unmatched->fill(event);
      }
    }

    // M(first TopJet) > M(second TopJet + Muon)
    passed_rec_mass = mass_sel1->passes(event);
    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass){
      h_mass->fill(event);
      passed_rec_final = true;
      if(isTTbar){
        h_passedrec_gen->fill(event);
        h_ttbar_hist->fill(event);
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_mass_matched->fill(event);
        else h_mass_unmatched->fill(event);
      }
    }

    if(passed_rec && !passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && pt_mu_rec_sideband->passes(event)){
      h_rec_pt_mu_sideband->fill(event);
      passed_rec_pt_mu_sideband = true;
    }

    if(passed_rec && passed_rec_pt_mu && !passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && pt_topjet_rec_sideband->passes(event)){
      h_rec_pt_topjet_sideband->fill(event);
      passed_rec_pt_topjet_sideband = true;
    }

    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet && !passed_rec_dr && passed_rec_mass && dr_rec_sideband->passes(event)){
      h_rec_dr_sideband->fill(event);
      passed_rec_dr_sideband = true;
    }

    if(passed_rec && passed_rec_pt_mu && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && !passed_rec_mass){
      h_rec_mass_sideband->fill(event);
      passed_rec_mass_sideband = true;
    }
    if(passed_rec_final && mass_lower->passes(event)){
      h_mass_lower->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_mass_lower_matched->fill(event);
        else h_mass_lower_unmatched->fill(event);
      }
    }
    if(passed_rec_final && mass_higher->passes(event)){
      h_mass_higher->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_mass_higher_matched->fill(event);
        else h_mass_higher_unmatched->fill(event);
      }
    }

    event.set(h_passed_rec_final, passed_rec_final);
    event.set(h_passed_gen_final, passed_gen_final);
    event.set(h_passed_gen_pt_mu_sideband, passed_gen_pt_mu_sideband);
    event.set(h_passed_gen_dr_sideband, passed_gen_dr_sideband);
    event.set(h_passed_gen_mass_sideband, passed_gen_mass_sideband);
    event.set(h_passed_gen_pt_topjet_sideband, passed_gen_pt_topjet_sideband);
    event.set(h_passed_rec_dr_sideband, passed_rec_dr_sideband);
    event.set(h_passed_rec_mass_sideband, passed_rec_mass_sideband);
    event.set(h_passed_rec_pt_mu_sideband, passed_rec_pt_mu_sideband);
    event.set(h_passed_rec_pt_topjet_sideband, passed_rec_pt_topjet_sideband);

    if(event.topjets->size() > 0){
      event.set(h_pt_rec, event.topjets->at(0).pt());
      double tau32_rec = event.topjets->at(0).tau3()/event.topjets->at(0).tau2();
      event.set(h_tau32_rec, tau32_rec);
      event.set(h_mass_rec, event.topjets->at(0).v4().M());
      event.set(h_calc_tau32_rec, calc_pf_tau3.PF_tau3(event)/calc_pf_tau2.PF_tau2(event));
    }
    else{
      event.set(h_pt_rec, -100);
      event.set(h_tau32_rec, -100);
      event.set(h_mass_rec, -100);
      event.set(h_calc_tau32_rec, -100);
    }
    if(isMC && event.gentopjets->size() > 0){
      event.set(h_pt_gen, event.gentopjets->at(0).pt());
      double tau32_gen = event.gentopjets->at(0).tau3()/event.gentopjets->at(0).tau2();
      event.set(h_tau32_gen, tau32_gen);
      event.set(h_mass_gen, event.gentopjets->at(0).v4().M());
      event.set(h_calc_tau32_gen, calc_gen_tau3.Gen_tau3(event)/calc_gen_tau2.Gen_tau2(event));
    }
    else{
      event.set(h_pt_gen, -100);
      event.set(h_tau32_gen, -100);
      event.set(h_mass_gen, -100);
      event.set(h_calc_tau32_gen, -100);
    }

    if(!passed_rec_final && !passed_gen_final && !passed_gen_pt_mu_sideband && !passed_gen_pt_topjet_sideband && !passed_gen_dr_sideband && !passed_gen_mass_sideband && !passed_rec_pt_mu_sideband & !passed_rec_pt_topjet_sideband && !passed_rec_dr_sideband && !passed_rec_mass_sideband) return false;

    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PostKinCutModule)

}
