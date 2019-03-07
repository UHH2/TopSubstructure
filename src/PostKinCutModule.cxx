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

    bool isMC, isTTbar;
    bool passed_rec, passed_gen, passed_gen_sel, passed_rec_sel;
    bool matched_rec, matched_gen;
    bool passed_rec_mass, passed_gen_mass;

    bool passed_rec_2;

    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> ntopjet2_sel, ntopjet1_sel, nbtag_medium_sel, nbtag_tight_sel;
    std::unique_ptr<Selection> pt_topjet_sel1;
    std::unique_ptr<Selection> mass_sel0, mass_sel1, dr_sel;
    std::unique_ptr<Selection> twodcut_sel1, twodcut_sel2, twodcut_sel3, twodcut_sel4, twodcut_sel5, twodcut_sel6, twodcut_sel7, twodcut_sel8, twodcut_sel9, twodcut_sel10, twodcut_sel11, twodcut_sel12, twodcut_sel13, twodcut_sel14, twodcut_sel15, twodcut_sel16;

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

    JetId Btag_tight;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;

    uhh2::Event::Handle<bool> h_passed_rec, h_passed_rec_final;
    uhh2::Event::Handle<bool> h_passed_gen, h_passed_gen_final;

    uhh2::Event::Handle<double> h_pt_rec;
    uhh2::Event::Handle<double> h_pt_gen;
  };


  PostKinCutModule::PostKinCutModule(Context & ctx){
    Btag_tight        = CSVBTag(CSVBTag::WP_TIGHT);

    h_passed_rec = ctx.get_handle<bool>("h_passed_rec");
    h_passed_gen = ctx.get_handle<bool>("h_passed_gen");
    h_passed_rec_final = ctx.declare_event_output<bool>("h_passed_rec_final");
    h_passed_gen_final = ctx.declare_event_output<bool>("h_passed_gen_final");
    h_rec_weight = ctx.get_handle<double>("h_rec_weight");
    h_gen_weight = ctx.get_handle<double>("h_gen_weight");
    h_pt_rec = ctx.declare_event_output<double>("h_pt_rec");
    h_pt_gen = ctx.declare_event_output<double>("h_pt_gen");


    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft");
    isMC = (ctx.get("dataset_type") == "MC");

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      cleaner.reset(new GenTopJetLeptonCleaner(ctx));
      gentopjetcleaner.reset(new GenTopJetCleaner(ctx));
      ntopjet2_gen.reset(new GenNTopJet(2,2));
      dr_gen.reset(new dRSelection(ctx));
      mass_gen.reset(new MassSelection(ctx, 1));
      pt_topjet_gen.reset(new PtSelection(400, 200));
      genmatching.reset(new GenMatching(ctx));
      recmatching.reset(new RecMatching(ctx));
    }


    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    ntopjet1_sel.reset(new NTopJetSelection(1,1));
    mass_sel0.reset(new RecMassSelection(0));   // 0: compare mass between first and second topjet
    mass_sel1.reset(new RecMassSelection(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
    dr_sel.reset(new RecdRSelection());
    pt_topjet_sel1.reset(new RecPtSelection(400, 200));
    nbtag_tight_sel.reset(new NJetSelection(1, -1, Btag_tight));


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

    h_ttbar_hist.reset(new TTbarGenHists(ctx, "ttbar_hist"));
  }


  bool PostKinCutModule::process(Event & event) {
    cout << "PostKinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    // event.weight = event.get(h_weight);
    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen)) passed_gen = event.get(h_passed_gen);
    else passed_gen = false;

    if(event.is_valid(h_passed_rec)) passed_rec = event.get(h_passed_rec);
    else passed_rec = false;
    if(isTTbar) ttgenprod->process(event);
    passed_rec_mass = false;
    passed_gen_mass = false;

    matched_rec = false;
    matched_gen = false;
    passed_rec_2 = false;
    if(isTTbar && passed_gen){
      cleaner->process(event); // Do this always!
      gentopjetcleaner->process(event);

      passed_gen = pt_topjet_gen->passes(event);
      if(passed_gen){
        h_gen_pt_topjet->fill(event);
        matched_gen = genmatching->passes(event);
        if(matched_gen) h_gen_pt_topjet_matched->fill(event);
        else h_gen_pt_topjet_unmatched->fill(event);

        passed_gen = ntopjet2_gen->passes(event);
        if(passed_gen){
          h_gen_ntopjet2->fill(event);
          matched_gen = genmatching->passes(event);
          if(matched_gen) h_gen_ntopjet2_matched->fill(event);
          else h_gen_ntopjet2_unmatched->fill(event);

          passed_gen = dr_gen->passes(event);
          if(passed_gen){
            h_gen_dr->fill(event);
            matched_gen = genmatching->passes(event);
            if(matched_gen) h_gen_dr_matched->fill(event);
            else h_gen_dr_unmatched->fill(event);

            passed_gen_mass = mass_gen->passes(event);
            if(passed_gen_mass){
              h_gen_mass->fill(event);
              h_passedgen_rec->fill(event);
              matched_gen = genmatching->passes(event);
              if(matched_gen) h_gen_mass_matched->fill(event);
              else h_gen_mass_unmatched->fill(event);
            }
          }
        }
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


    if(passed_rec){
      // pT(first Topjet) > 400
      passed_rec = pt_topjet_sel1->passes(event);
      if(passed_rec){
        h_pt_topjet->fill(event);
        if(isTTbar){
          matched_rec = recmatching->passes(event);
          if(matched_rec) h_pt_topjet_matched->fill(event);
          else h_pt_topjet_unmatched->fill(event);
        }


        passed_rec_2 = ntopjet1_sel->passes(event);
        if(passed_rec_2){
          h_ntopjet1->fill(event);
          if(isTTbar){
            matched_rec = recmatching->passes(event);
            if(matched_rec) h_ntopjet1_matched->fill(event);
            else h_ntopjet1_unmatched->fill(event);
          }
        }

        // >= 2 TopJets
        passed_rec = ntopjet2_sel->passes(event);
        if(passed_rec){
          h_ntopjet2->fill(event);
          if(isTTbar){
            matched_rec = recmatching->passes(event);
            if(matched_rec) h_ntopjet2_matched->fill(event);
            else h_ntopjet2_unmatched->fill(event);
          }

          // dR(muon, second TopJet) < 0.8
          passed_rec = dr_sel->passes(event);
          if(passed_rec){
          h_dr->fill(event);
          if(isTTbar){
            matched_rec = recmatching->passes(event);
            if(matched_rec) h_dr_matched->fill(event);
            else h_dr_unmatched->fill(event);
          }

            // M(first TopJet) > M(second TopJet + Muon)
            passed_rec_mass = mass_sel1->passes(event);
            if(passed_rec_mass){
              h_mass->fill(event);
              if(isTTbar){
                h_passedrec_gen->fill(event);
                h_ttbar_hist->fill(event);
                matched_rec = recmatching->passes(event);
                if(matched_rec) h_mass_matched->fill(event);
                else h_mass_unmatched->fill(event);
              }
            }
          }
        }
      }
    }
    event.set(h_passed_rec_final, passed_rec_mass);
    event.set(h_passed_gen_final, passed_gen_mass);
    if(event.topjets->size() > 0) event.set(h_pt_rec, event.topjets->at(0).pt());
    if(isTTbar && event.gentopjets->size() > 0) event.set(h_pt_gen, event.gentopjets->at(0).pt());
    if((!passed_rec && !passed_gen)) return false;

    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PostKinCutModule)

}
