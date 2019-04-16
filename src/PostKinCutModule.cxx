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

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class PostKinCutModule: public AnalysisModule {
  public:
    explicit PostKinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    // bool passed_rec_2;
    // bool isMC;
    bool isTTbar;
    bool passed_rec, passed_gen;
    bool matched_rec, matched_gen;

    bool passed_gen_ntopjet, passed_gen_dr, passed_gen_mass, passed_gen_pt_topjet;
    bool passed_rec_ntopjet, passed_rec_dr, passed_rec_mass, passed_rec_pt_topjet;

    bool passed_rec_met_sideband, passed_rec_pt_topjet_mu_sideband, passed_gen_met_sideband, passed_gen_pt_topjet_mu_sideband;
    bool passed_rec_ntopjet_sideband_l2, passed_rec_ntopjet_sideband_g2, passed_rec_dr_sideband, passed_rec_mass_sideband, passed_rec_pt_topjet_sideband;
    bool passed_gen_ntopjet_sideband_l2, passed_gen_ntopjet_sideband_g2, passed_gen_dr_sideband, passed_gen_mass_sideband, passed_gen_pt_topjet_sideband;


    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    // std::unique_ptr<Selection> nbtag_tight_sel;
    // std::unique_ptr<Selection> mass_sel0;
    // std::unique_ptr<Selection> ntopjet1_sel;

    std::unique_ptr<Selection> ntopjet2_sel;
    std::unique_ptr<Selection> pt_topjet_sel1;
    std::unique_ptr<Selection> mass_sel1, dr_sel;
    std::unique_ptr<Selection> ntopjet_gen_sideband_l2, ntopjet_gen_sideband_g2, dr_gen_sideband, mass_gen_sideband, pt_topjet_gen_sideband;
    std::unique_ptr<Selection> ntopjet_rec_sideband_g2, ntopjet_rec_sideband_l2, mass_rec_sideband, dr_rec_sideband, pt_topjet_rec_sideband;

    std::unique_ptr<Selection> pt_topjet_gen, ntopjet2_gen, dr_gen, mass_gen, genmatching;
    std::unique_ptr<Selection> recmatching;

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> cleaner;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_pt_topjet, h_gen_pt_topjet_matched, h_gen_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_gen_ntopjet2, h_gen_ntopjet2_matched, h_gen_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_gen_dr, h_gen_dr_matched, h_gen_dr_unmatched;
    std::unique_ptr<Hists> h_gen_mass, h_gen_mass_matched, h_gen_mass_unmatched;

    std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_ntopjet1, h_ntopjet1_matched, h_ntopjet1_unmatched;
    std::unique_ptr<Hists> h_pt_topjet, h_pt_topjet_matched, h_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_dr, h_dr_matched, h_dr_unmatched;
    std::unique_ptr<Hists> h_mass, h_mass_matched, h_mass_unmatched;
    std::unique_ptr<Hists> h_passedgen_rec, h_passedrec_gen;
    std::unique_ptr<Hists> h_ttbar_hist;

    std::unique_ptr<Hists> h_gen_pt_topjet_sideband, h_gen_ntopjet_sideband_l2, h_gen_ntopjet_sideband_g2, h_gen_dr_sideband, h_gen_mass_sideband;
    std::unique_ptr<Hists> h_rec_pt_topjet_sideband, h_rec_ntopjet_sideband_l2, h_rec_ntopjet_sideband_g2, h_rec_dr_sideband, h_rec_mass_sideband;

    // JetId Btag_tight;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;

    uhh2::Event::Handle<bool> h_passed_rec, h_passed_rec_final;
    uhh2::Event::Handle<bool> h_passed_gen, h_passed_gen_final;
    uhh2::Event::Handle<bool> h_passed_rec_met_sideband, h_passed_rec_pt_topjet_mu_sideband, h_passed_gen_met_sideband, h_passed_gen_pt_topjet_mu_sideband;

    uhh2::Event::Handle<double> h_pt_rec;
    uhh2::Event::Handle<double> h_pt_gen;
  };


  PostKinCutModule::PostKinCutModule(Context & ctx){
    // Btag_tight        = CSVBTag(CSVBTag::WP_TIGHT);

    h_passed_rec = ctx.get_handle<bool>("h_passed_rec");
    h_passed_gen = ctx.get_handle<bool>("h_passed_gen");
    h_passed_rec_met_sideband = ctx.get_handle<bool>("h_passed_rec_met_sideband");
    h_passed_rec_pt_topjet_mu_sideband = ctx.get_handle<bool>("h_passed_rec_pt_topjet_mu_sideband");
    h_passed_gen_met_sideband = ctx.get_handle<bool>("h_passed_gen_met_sideband");
    h_passed_gen_pt_topjet_mu_sideband = ctx.get_handle<bool>("h_passed_gen_pt_topjet_mu_sideband");
    h_passed_rec_final = ctx.declare_event_output<bool>("h_passed_rec_final");
    h_passed_gen_final = ctx.declare_event_output<bool>("h_passed_gen_final");
    h_rec_weight = ctx.get_handle<double>("h_rec_weight");
    h_gen_weight = ctx.get_handle<double>("h_gen_weight");
    h_pt_rec = ctx.declare_event_output<double>("h_pt_rec");
    h_pt_gen = ctx.declare_event_output<double>("h_pt_gen");


    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000_2016v3" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3");
    // isMC = (ctx.get("dataset_type") == "MC");
    
    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      cleaner.reset(new GenTopJetLeptonCleaner(ctx));
      gentopjetcleaner.reset(new GenTopJetCleaner(ctx));
      ntopjet2_gen.reset(new GenNTopJet(2,2));
      ntopjet_gen_sideband_l2.reset(new GenNTopJet(0,1));
      ntopjet_gen_sideband_g2.reset(new GenNTopJet(2,-1));
      dr_gen.reset(new dRSelection(ctx));
      dr_gen_sideband.reset(new dRSelection(ctx, 1.0));
      mass_gen.reset(new MassSelection(ctx, 1));
      mass_gen_sideband.reset(new MassSelection(ctx, 2));
      pt_topjet_gen.reset(new PtSelection(400, 200));
      pt_topjet_gen_sideband.reset(new PtSelection(300, 200));
      genmatching.reset(new GenMatching(ctx));
      recmatching.reset(new RecMatching(ctx));
    }


    // nbtag_tight_sel.reset(new NJetSelection(1, -1, Btag_tight));
    // mass_sel0.reset(new RecMassSelection(0));   // 0: compare mass between first and second topjet
    // ntopjet1_sel.reset(new NTopJetSelection(1,1));
    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    mass_sel1.reset(new RecMassSelection(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
    dr_sel.reset(new RecdRSelection());
    pt_topjet_sel1.reset(new RecPtSelection(400, 200));

    ntopjet_rec_sideband_g2.reset(new NTopJetSelection(2,-1));
    ntopjet_rec_sideband_l2.reset(new NTopJetSelection(0,1));
    mass_rec_sideband.reset(new RecMassSelection(2));   // 2: diff shoyuld be grater than -50
    dr_rec_sideband.reset(new RecdRSelection(1.0));
    pt_topjet_rec_sideband.reset(new RecPtSelection(300, 200));


    // 3. Set up Hists classes:
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
    h_gen_ntopjet_sideband_l2.reset(new GenHists(ctx, "gen_ntopjet_sideband_l2"));
    h_gen_ntopjet_sideband_g2.reset(new GenHists(ctx, "gen_ntopjet_sideband_g2"));
    h_gen_dr_sideband.reset(new GenHists(ctx, "gen_dr_sideband"));
    h_gen_mass_sideband.reset(new GenHists(ctx, "gen_mass_sideband"));

    h_pt_topjet.reset(new TopSubstructureRecoHists(ctx, "pt_topjet"));
    h_pt_topjet_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_matched"));
    h_pt_topjet_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_unmatched"));

    h_ntopjet2.reset(new TopSubstructureRecoHists(ctx, "ntopjet2"));
    h_ntopjet2_matched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_matched"));
    h_ntopjet2_unmatched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_unmatched"));

    h_ntopjet1.reset(new TopSubstructureRecoHists(ctx, "ntopjet1"));
    h_ntopjet1_matched.reset(new TopSubstructureRecoHists(ctx, "ntopjet1_matched"));
    h_ntopjet1_unmatched.reset(new TopSubstructureRecoHists(ctx, "ntopjet1_unmatched"));

    h_dr.reset(new TopSubstructureRecoHists(ctx, "dr"));
    h_dr_matched.reset(new TopSubstructureRecoHists(ctx, "dr_matched"));
    h_dr_unmatched.reset(new TopSubstructureRecoHists(ctx, "dr_unmatched"));

    h_mass.reset(new TopSubstructureRecoHists(ctx, "mass"));
    h_mass_matched.reset(new TopSubstructureRecoHists(ctx, "mass_matched"));
    h_mass_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass_unmatched"));

    h_passedgen_rec.reset(new TopSubstructureRecoHists(ctx, "passedgen_rec"));
    h_passedrec_gen.reset(new GenHists(ctx, "passedrec_gen"));

    h_rec_pt_topjet_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband"));
    h_rec_ntopjet_sideband_l2.reset(new TopSubstructureRecoHists(ctx, "rec_ntopjet_sideband_l2"));
    h_rec_ntopjet_sideband_g2.reset(new TopSubstructureRecoHists(ctx, "rec_ntopjet_sideband_g2"));
    h_rec_dr_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband"));
    h_rec_mass_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband"));

    h_ttbar_hist.reset(new TTbarGenHists(ctx, "ttbar_hist"));
  }


  bool PostKinCutModule::process(Event & event) {
    cout << "PostKinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    // event.weight = event.get(h_weight);
    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen)) passed_gen = event.get(h_passed_gen);
    else passed_gen = false;
    if(event.is_valid(h_passed_rec_met_sideband)) passed_rec_met_sideband = event.get(h_passed_rec_met_sideband);
    else passed_rec_met_sideband = false;
    if(event.is_valid(h_passed_rec_pt_topjet_mu_sideband)) passed_rec_pt_topjet_mu_sideband = event.get(h_passed_rec_pt_topjet_mu_sideband);
    else passed_rec_pt_topjet_mu_sideband = false;
    if(event.is_valid(h_passed_gen_met_sideband)) passed_gen_met_sideband = event.get(h_passed_gen_met_sideband);
    else passed_gen_met_sideband = false;
    if(event.is_valid(h_passed_gen_pt_topjet_mu_sideband)) passed_gen_pt_topjet_mu_sideband = event.get(h_passed_gen_pt_topjet_mu_sideband);
    else passed_gen_pt_topjet_mu_sideband = false;
    if(event.is_valid(h_passed_rec)) passed_rec = event.get(h_passed_rec);
    else passed_rec = false;

    passed_gen_ntopjet = false;
    passed_gen_dr = false;
    passed_gen_mass = false;
    passed_gen_pt_topjet = false;
    passed_rec_ntopjet = false;
    passed_rec_dr = false;
    passed_rec_mass = false;
    passed_rec_pt_topjet = false;
    passed_rec_ntopjet_sideband_l2 = false;
    passed_rec_ntopjet_sideband_g2 = false;
    passed_rec_dr_sideband = false;
    passed_rec_mass_sideband = false;
    passed_rec_pt_topjet_sideband = false;
    passed_gen_ntopjet_sideband_l2 = false;
    passed_gen_ntopjet_sideband_g2 = false;
    passed_gen_dr_sideband = false;
    passed_gen_mass_sideband = false;
    passed_gen_pt_topjet_sideband = false;

    matched_rec = false;
    matched_gen = false;

    if(isTTbar) {
      ttgenprod->process(event);

      if(passed_gen){
        cleaner->process(event); // Do this always!
        gentopjetcleaner->process(event);
      }
      passed_gen_pt_topjet = pt_topjet_gen->passes(event);
      if(passed_gen && passed_gen_pt_topjet){
        h_gen_pt_topjet->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_pt_topjet_matched->fill(event);
        else h_gen_pt_topjet_unmatched->fill(event);
      }

      passed_gen_ntopjet = ntopjet2_gen->passes(event);
      if(passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet){
        h_gen_ntopjet2->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_ntopjet2_matched->fill(event);
        else h_gen_ntopjet2_unmatched->fill(event);
      }

      passed_gen_dr = dr_gen->passes(event);
      if(passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr){
        h_gen_dr->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_dr_matched->fill(event);
        else h_gen_dr_unmatched->fill(event);
      }

      passed_gen_mass = mass_gen->passes(event);
      if(passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass){
        h_gen_mass->fill(event);
        h_passedgen_rec->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_mass_matched->fill(event);
        else h_gen_mass_unmatched->fill(event);
      }

      passed_gen_pt_topjet_sideband = pt_topjet_gen_sideband->passes(event);
      if(passed_gen && !passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && passed_gen_pt_topjet_sideband){
        h_gen_pt_topjet_sideband->fill(event);
    }

      passed_gen_ntopjet_sideband_l2 = ntopjet_gen_sideband_l2->passes(event);
      if(passed_gen && passed_gen_pt_topjet && !passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && passed_gen_ntopjet_sideband_l2){
        h_gen_ntopjet_sideband_l2->fill(event);
      }
      // maybe >2 & <2 ??
      passed_gen_ntopjet_sideband_g2 = ntopjet_gen_sideband_g2->passes(event);
      if(passed_gen && passed_gen_pt_topjet && !passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && passed_gen_ntopjet_sideband_g2){
        h_gen_ntopjet_sideband_g2->fill(event);
      }

      passed_gen_dr_sideband = dr_gen_sideband->passes(event);
      if(passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet && !passed_gen_dr && passed_gen_mass && passed_gen_dr_sideband){
        h_gen_dr_sideband->fill(event);
      }

      passed_gen_mass_sideband = mass_gen_sideband->passes(event);
      if(isTTbar && passed_gen && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && !passed_gen_mass && passed_gen_mass_sideband){
        h_gen_mass_sideband->fill(event);
      }
  }
    /*
    ████████  ██████  ██████   ██████
       ██    ██    ██ ██   ██ ██    ██
       ██    ██    ██ ██   ██ ██    ██
       ██    ██    ██ ██   ██ ██    ██
       ██     ██████  ██████   ██████

       Additional AK4 Jets: Don't save them in a handle. Either write a function which returns an integer of the number of AK4 jets or program it directly in the hist class. Check for both candidates! Hadronic and Leptonic!!
    */


    // pT(first Topjet) > 400
    passed_rec_pt_topjet = pt_topjet_sel1->passes(event);
    if(passed_rec && passed_rec_pt_topjet){
      h_pt_topjet->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_pt_topjet_matched->fill(event);
        else h_pt_topjet_unmatched->fill(event);
      }
    }

    // >= 2 TopJets
    passed_rec_ntopjet = ntopjet2_sel->passes(event);
    if(passed_rec && passed_rec_pt_topjet && passed_rec_ntopjet){
      h_ntopjet2->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_ntopjet2_matched->fill(event);
        else h_ntopjet2_unmatched->fill(event);
      }
    }

    // dR(muon, second TopJet) < 0.8
    passed_rec_dr = dr_sel->passes(event);
    if(passed_rec && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr){
      h_dr->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_dr_matched->fill(event);
        else h_dr_unmatched->fill(event);
      }
    }

    // M(first TopJet) > M(second TopJet + Muon)
    passed_rec_mass = mass_sel1->passes(event);
    if(passed_rec && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass){
      h_mass->fill(event);
      if(isTTbar){
        h_passedrec_gen->fill(event);
        h_ttbar_hist->fill(event);
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_mass_matched->fill(event);
        else h_mass_unmatched->fill(event);
      }
    }

    passed_rec_pt_topjet_sideband = pt_topjet_rec_sideband->passes(event);
    if(isTTbar && passed_rec && !passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && passed_rec_pt_topjet_sideband){
      h_rec_pt_topjet_sideband->fill(event);
    }

    passed_rec_ntopjet_sideband_l2 = ntopjet_rec_sideband_l2->passes(event);
    if(isTTbar && passed_rec && passed_rec_pt_topjet && !passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && passed_rec_ntopjet_sideband_l2){
      h_rec_ntopjet_sideband_l2->fill(event);
    }
    // maybe >2 & <2 ??
    passed_rec_ntopjet_sideband_g2 = ntopjet_rec_sideband_g2->passes(event);
    if(isTTbar && passed_rec && passed_rec_pt_topjet && !passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && passed_rec_ntopjet_sideband_g2){
      h_rec_ntopjet_sideband_g2->fill(event);
    }

    passed_rec_dr_sideband = dr_rec_sideband->passes(event);
    if(isTTbar && passed_rec && passed_rec_pt_topjet && passed_rec_ntopjet && !passed_rec_dr && passed_rec_mass && passed_rec_dr_sideband){
      h_rec_dr_sideband->fill(event);
    }


    event.set(h_passed_rec_final, passed_rec_mass);
    event.set(h_passed_gen_final, passed_gen_mass);
    if(event.topjets->size() > 0) event.set(h_pt_rec, event.topjets->at(0).pt());
    if(isTTbar && event.gentopjets->size() > 0) event.set(h_pt_gen, event.gentopjets->at(0).pt());

    // 3. decide whether or not to keep the current event in the output:
    return false;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PostKinCutModule)

}
