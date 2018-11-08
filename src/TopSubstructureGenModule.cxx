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
#include "UHH2/TopSubstructure/include/TopSubstructureHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  /* 
   * This is the central class which calls other AnalysisModules, Hists or Selection classes.
   * This AnalysisModule, in turn, is called (via AnalysisModuleRunner) by SFrame.
   */

  class TopSubstructureGenModule: public AnalysisModule {
  public:    
    explicit TopSubstructureGenModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:    
    std::unique_ptr<CommonModules> common;
    bool isMC;
    string sort_by;
   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> ntopjetcand_sel1, dphi_sel1, dphi_sel2, ntopjetcand_sel2;
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel;
    std::unique_ptr<Selection> matching, genmatching;

    std::unique_ptr<AnalysisModule> PUreweight;
    std::unique_ptr<AnalysisModule> topjetsort_by_dphi, topjetsort_by_mass, gentopjetsort_by_dphi, gentopjetsort_by_mass, jetsel;

    std::unique_ptr<AnalysisModule> ttgenprod;
    
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nocuts, h_njet_nocuts, h_muon_nocuts, h_topjet_nocuts;
    std::unique_ptr<Hists> h_nocuts_matched, h_njet_nocuts_matched, h_muon_nocuts_matched, h_topjet_nocuts_matched;
    std::unique_ptr<Hists> h_nocuts_genmatched, h_njet_nocuts_genmatched, h_muon_nocuts_genmatched, h_topjet_nocuts_genmatched;
    std::unique_ptr<Hists> h_nocuts_unmatched, h_njet_nocuts_unmatched, h_muon_nocuts_unmatched, h_topjet_nocuts_unmatched;

    std::unique_ptr<Hists> h_ntopjet1, h_njet_ntopjet1, h_muon_ntopjet1, h_topjet_ntopjet1;
    std::unique_ptr<Hists> h_ntopjet1_matched, h_njet_ntopjet1_matched, h_muon_ntopjet1_matched, h_topjet_ntopjet1_matched;
    std::unique_ptr<Hists> h_ntopjet1_genmatched, h_njet_ntopjet1_genmatched, h_muon_ntopjet1_genmatched, h_topjet_ntopjet1_genmatched;
    std::unique_ptr<Hists> h_ntopjet1_unmatched, h_njet_ntopjet1_unmatched, h_muon_ntopjet1_unmatched, h_topjet_ntopjet1_unmatched;

    std::unique_ptr<Hists> h_dphi1, h_njet_dphi1, h_muon_dphi1, h_topjet_dphi1;
    std::unique_ptr<Hists> h_dphi1_matched, h_njet_dphi1_matched, h_muon_dphi1_matched, h_topjet_dphi1_matched;
    std::unique_ptr<Hists> h_dphi1_genmatched, h_njet_dphi1_genmatched, h_muon_dphi1_genmatched, h_topjet_dphi1_genmatched;
    std::unique_ptr<Hists> h_dphi1_unmatched, h_njet_dphi1_unmatched, h_muon_dphi1_unmatched, h_topjet_dphi1_unmatched;

    std::unique_ptr<Hists> h_dphi25, h_njet_dphi25, h_muon_dphi25, h_topjet_dphi25;
    std::unique_ptr<Hists> h_dphi25_matched, h_njet_dphi25_matched, h_muon_dphi25_matched, h_topjet_dphi25_matched;
    std::unique_ptr<Hists> h_dphi25_genmatched, h_njet_dphi25_genmatched, h_muon_dphi25_genmatched, h_topjet_dphi25_genmatched;
    std::unique_ptr<Hists> h_dphi25_unmatched, h_njet_dphi25_unmatched, h_muon_dphi25_unmatched, h_topjet_dphi25_unmatched;

    std::unique_ptr<Hists> h_ntopjet2, h_njet_ntopjet2, h_muon_ntopjet2, h_topjet_ntopjet2;
    std::unique_ptr<Hists> h_ntopjet2_matched, h_njet_ntopjet2_matched, h_muon_ntopjet2_matched, h_topjet_ntopjet2_matched;
    std::unique_ptr<Hists> h_ntopjet2_genmatched, h_njet_ntopjet2_genmatched, h_muon_ntopjet2_genmatched, h_topjet_ntopjet2_genmatched;
    std::unique_ptr<Hists> h_ntopjet2_unmatched, h_njet_ntopjet2_unmatched, h_muon_ntopjet2_unmatched, h_topjet_ntopjet2_unmatched;
  };


  TopSubstructureGenModule::TopSubstructureGenModule(Context & ctx){

    // set up selections
       // decide how you want to sort your candidates
    sort_by = ctx.get("sort");
    const std::string ttbar_gen_label("ttbargen");

    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    if(sort_by == "dphi") gentopjetsort_by_dphi.reset(new GenTopJetSortDPhi(ctx));
    if(sort_by == "mass") gentopjetsort_by_mass.reset(new GenTopJetSortMass(ctx));
    jetsel.reset(new JetSelection(ctx));
    genmatching.reset(new QuarkGenJetMatching(ctx));
    ntopjetcand_sel1.reset(new NTopJet(ctx,1));
    dphi_sel1.reset(new DPhiSelection(ctx,1));
    dphi_sel2.reset(new DPhiSelection(ctx,2.5));
    ntopjetcand_sel2.reset(new NTopJet(ctx,1,2));

    // 3. Set up Hists classes:
    h_nocuts.reset(new TopSubstructureHists(ctx, "NoCuts", sort_by));
    h_njet_nocuts.reset(new JetHists(ctx, "Njet_nocuts"));
    h_topjet_nocuts.reset(new TopJetHists(ctx, "Topjet_nocuts"));
    h_muon_nocuts.reset(new MuonHists(ctx, "muon_nocuts"));

    h_nocuts_matched.reset(new TopSubstructureHists(ctx, "NoCuts_matched", sort_by));
    h_njet_nocuts_matched.reset(new JetHists(ctx, "Njet_nocuts_matched"));
    h_topjet_nocuts_matched.reset(new TopJetHists(ctx, "Topjet_nocuts_matched"));
    h_muon_nocuts_matched.reset(new MuonHists(ctx, "muon_nocuts_matched"));

    h_nocuts_genmatched.reset(new TopSubstructureHists(ctx, "NoCuts_genmatched", sort_by));
    h_njet_nocuts_genmatched.reset(new JetHists(ctx, "Njet_nocuts_genmatched"));
    h_topjet_nocuts_genmatched.reset(new TopJetHists(ctx, "Topjet_nocuts_genmatched"));
    h_muon_nocuts_genmatched.reset(new MuonHists(ctx, "muon_nocuts_genmatched"));

    h_nocuts_unmatched.reset(new TopSubstructureHists(ctx, "NoCuts_unmatched", sort_by));
    h_njet_nocuts_unmatched.reset(new JetHists(ctx, "Njet_nocuts_unmatched"));
    h_topjet_nocuts_unmatched.reset(new TopJetHists(ctx, "Topjet_nocuts_unmatched"));
    h_muon_nocuts_unmatched.reset(new MuonHists(ctx, "muon_nocuts_unmatched"));



    h_ntopjet1.reset(new TopSubstructureHists(ctx, "ntopjet1", sort_by));
    h_njet_ntopjet1.reset(new JetHists(ctx, "Njet_ntopjet1"));
    h_topjet_ntopjet1.reset(new TopJetHists(ctx, "Topjet_ntopjet1"));
    h_muon_ntopjet1.reset(new MuonHists(ctx, "muon_ntopjet1"));

    h_ntopjet1_matched.reset(new TopSubstructureHists(ctx, "ntopjet1_matched", sort_by));
    h_njet_ntopjet1_matched.reset(new JetHists(ctx, "Njet_ntopjet1_matched"));
    h_topjet_ntopjet1_matched.reset(new TopJetHists(ctx, "Topjet_ntopjet1_matched"));
    h_muon_ntopjet1_matched.reset(new MuonHists(ctx, "muon_ntopjet1_matched"));

    h_ntopjet1_genmatched.reset(new TopSubstructureHists(ctx, "ntopjet1_genmatched", sort_by));
    h_njet_ntopjet1_genmatched.reset(new JetHists(ctx, "Njet_ntopjet1_genmatched"));
    h_topjet_ntopjet1_genmatched.reset(new TopJetHists(ctx, "Topjet_ntopjet1_genmatched"));
    h_muon_ntopjet1_genmatched.reset(new MuonHists(ctx, "muon_ntopjet1_genmatched"));

    h_ntopjet1_unmatched.reset(new TopSubstructureHists(ctx, "ntopjet1_unmatched", sort_by));
    h_njet_ntopjet1_unmatched.reset(new JetHists(ctx, "Njet_ntopjet1_unmatched"));
    h_topjet_ntopjet1_unmatched.reset(new TopJetHists(ctx, "Topjet_ntopjet1_unmatched"));
    h_muon_ntopjet1_unmatched.reset(new MuonHists(ctx, "muon_ntopjet1_unmatched"));



    h_dphi1.reset(new TopSubstructureHists(ctx, "dphi1", sort_by));
    h_njet_dphi1.reset(new JetHists(ctx, "Njet_dphi1"));
    h_topjet_dphi1.reset(new TopJetHists(ctx, "Topjet_dphi1"));
    h_muon_dphi1.reset(new MuonHists(ctx, "muon_dphi1"));

    h_dphi1_matched.reset(new TopSubstructureHists(ctx, "dphi1_matched", sort_by));
    h_njet_dphi1_matched.reset(new JetHists(ctx, "Njet_dphi1_matched"));
    h_topjet_dphi1_matched.reset(new TopJetHists(ctx, "Topjet_dphi1_matched"));
    h_muon_dphi1_matched.reset(new MuonHists(ctx, "muon_dphi1_matched"));

    h_dphi1_genmatched.reset(new TopSubstructureHists(ctx, "dphi1_genmatched", sort_by));
    h_njet_dphi1_genmatched.reset(new JetHists(ctx, "Njet_dphi1_genmatched"));
    h_topjet_dphi1_genmatched.reset(new TopJetHists(ctx, "Topjet_dphi1_genmatched"));
    h_muon_dphi1_genmatched.reset(new MuonHists(ctx, "muon_dphi1_genmatched"));

    h_dphi1_unmatched.reset(new TopSubstructureHists(ctx, "dphi1_unmatched", sort_by));
    h_njet_dphi1_unmatched.reset(new JetHists(ctx, "Njet_dphi1_unmatched"));
    h_topjet_dphi1_unmatched.reset(new TopJetHists(ctx, "Topjet_dphi1_unmatched"));
    h_muon_dphi1_unmatched.reset(new MuonHists(ctx, "muon_dphi1_unmatched"));



    h_dphi25.reset(new TopSubstructureHists(ctx, "dphi25", sort_by));
    h_njet_dphi25.reset(new JetHists(ctx, "Njet_dphi25"));
    h_topjet_dphi25.reset(new TopJetHists(ctx, "Topjet_dphi25"));
    h_muon_dphi25.reset(new MuonHists(ctx, "muon_dphi25"));

    h_dphi25_matched.reset(new TopSubstructureHists(ctx, "dphi25_matched", sort_by));
    h_njet_dphi25_matched.reset(new JetHists(ctx, "Njet_dphi25_matched"));
    h_topjet_dphi25_matched.reset(new TopJetHists(ctx, "Topjet_dphi25_matched"));
    h_muon_dphi25_matched.reset(new MuonHists(ctx, "muon_dphi25_matched"));

    h_dphi25_genmatched.reset(new TopSubstructureHists(ctx, "dphi25_genmatched", sort_by));
    h_njet_dphi25_genmatched.reset(new JetHists(ctx, "Njet_dphi25_genmatched"));
    h_topjet_dphi25_genmatched.reset(new TopJetHists(ctx, "Topjet_dphi25_genmatched"));
    h_muon_dphi25_genmatched.reset(new MuonHists(ctx, "muon_dphi25_genmatched"));

    h_dphi25_unmatched.reset(new TopSubstructureHists(ctx, "dphi25_unmatched", sort_by));
    h_njet_dphi25_unmatched.reset(new JetHists(ctx, "Njet_dphi25_unmatched"));
    h_topjet_dphi25_unmatched.reset(new TopJetHists(ctx, "Topjet_dphi25_unmatched"));
    h_muon_dphi25_unmatched.reset(new MuonHists(ctx, "muon_dphi25_unmatched"));



    h_ntopjet2.reset(new TopSubstructureHists(ctx, "ntopjet2", sort_by));
    h_njet_ntopjet2.reset(new JetHists(ctx, "Njet_ntopjet2"));
    h_topjet_ntopjet2.reset(new TopJetHists(ctx, "Topjet_ntopjet2"));
    h_muon_ntopjet2.reset(new MuonHists(ctx, "muon_ntopjet2"));

    h_ntopjet2_matched.reset(new TopSubstructureHists(ctx, "ntopjet2_matched", sort_by));
    h_njet_ntopjet2_matched.reset(new JetHists(ctx, "Njet_ntopjet2_matched"));
    h_topjet_ntopjet2_matched.reset(new TopJetHists(ctx, "Topjet_ntopjet2_matched"));
    h_muon_ntopjet2_matched.reset(new MuonHists(ctx, "muon_ntopjet2_matched"));

    h_ntopjet2_genmatched.reset(new TopSubstructureHists(ctx, "ntopjet2_genmatched", sort_by));
    h_njet_ntopjet2_genmatched.reset(new JetHists(ctx, "Njet_ntopjet2_genmatched"));
    h_topjet_ntopjet2_genmatched.reset(new TopJetHists(ctx, "Topjet_ntopjet2_genmatched"));
    h_muon_ntopjet2_genmatched.reset(new MuonHists(ctx, "muon_ntopjet2_genmatched"));

    h_ntopjet2_unmatched.reset(new TopSubstructureHists(ctx, "ntopjet2_unmatched", sort_by));
    h_njet_ntopjet2_unmatched.reset(new JetHists(ctx, "Njet_ntopjet2_unmatched"));
    h_topjet_ntopjet2_unmatched.reset(new TopJetHists(ctx, "Topjet_ntopjet2_unmatched"));
    h_muon_ntopjet2_unmatched.reset(new MuonHists(ctx, "muon_ntopjet2_unmatched"));

  }


  bool TopSubstructureGenModule::process(Event & event) {    
    cout << "TopSubstructureGenModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    // 1. run all modules other modules.

    // sort the topjets for our needs
    if(sort_by == "dphi") gentopjetsort_by_dphi->process(event);
    if(sort_by == "mass") gentopjetsort_by_mass->process(event);

    ttgenprod->process(event);
    jetsel->process(event);



    // 2. test selections and fill histograms
    h_nocuts->fill(event);
    h_njet_nocuts->fill(event);
    h_topjet_nocuts->fill(event);
    h_muon_nocuts->fill(event);

    if(matching->passes(event)){
      h_nocuts_matched->fill(event);
      h_njet_nocuts_matched->fill(event);
      h_topjet_nocuts_matched->fill(event);
      h_muon_nocuts_matched->fill(event);
    }
    else{
      h_nocuts_unmatched->fill(event);
      h_njet_nocuts_unmatched->fill(event);
      h_topjet_nocuts_unmatched->fill(event);
      h_muon_nocuts_unmatched->fill(event);
    }
    if(genmatching->passes(event)){
      h_nocuts_genmatched->fill(event);
      h_njet_nocuts_genmatched->fill(event);
      h_topjet_nocuts_genmatched->fill(event);
      h_muon_nocuts_genmatched->fill(event);
    }

    //min 1 topjet_cand
    if(!ntopjetcand_sel1->passes(event)) return false;
    h_ntopjet1->fill(event);
    h_njet_ntopjet1->fill(event);
    h_topjet_ntopjet1->fill(event);
    h_muon_ntopjet1->fill(event);

    if(matching->passes(event)){
      h_ntopjet1_matched->fill(event);
      h_njet_ntopjet1_matched->fill(event);
      h_topjet_ntopjet1_matched->fill(event);
      h_muon_ntopjet1_matched->fill(event);
    }
    else{
      h_ntopjet1_unmatched->fill(event);
      h_njet_ntopjet1_unmatched->fill(event);
      h_topjet_ntopjet1_unmatched->fill(event);
      h_muon_ntopjet1_unmatched->fill(event);
    }
    if(genmatching->passes(event)){
      h_ntopjet1_genmatched->fill(event);
      h_njet_ntopjet1_genmatched->fill(event);
      h_topjet_ntopjet1_genmatched->fill(event);
      h_muon_ntopjet1_genmatched->fill(event);
    }

    //dphi(mu,jet1) > 1
    if(!dphi_sel1->passes(event)) return false;
    h_dphi1->fill(event);
    h_njet_dphi1->fill(event);
    h_topjet_dphi1->fill(event);
    h_muon_dphi1->fill(event);

    if(matching->passes(event)){
      h_dphi1_matched->fill(event);
      h_njet_dphi1_matched->fill(event);
      h_topjet_dphi1_matched->fill(event);
      h_muon_dphi1_matched->fill(event);
    }
    else{
      h_dphi1_unmatched->fill(event);
      h_njet_dphi1_unmatched->fill(event);
      h_topjet_dphi1_unmatched->fill(event);
      h_muon_dphi1_unmatched->fill(event);
    }
    if(genmatching->passes(event)){
      h_dphi1_genmatched->fill(event);
      h_njet_dphi1_genmatched->fill(event);
      h_topjet_dphi1_genmatched->fill(event);
      h_muon_dphi1_genmatched->fill(event);
    }

    //dphi(mu,jet1) > 2.5
    if(!dphi_sel2->passes(event)) return false;
    h_dphi25->fill(event);
    h_njet_dphi25->fill(event);
    h_topjet_dphi25->fill(event);
    h_muon_dphi25->fill(event);

    if(matching->passes(event)){
      h_dphi25_matched->fill(event);
      h_njet_dphi25_matched->fill(event);
      h_topjet_dphi25_matched->fill(event);
      h_muon_dphi25_matched->fill(event);
    }
    else{
      h_dphi25_unmatched->fill(event);
      h_njet_dphi25_unmatched->fill(event);
      h_topjet_dphi25_unmatched->fill(event);
      h_muon_dphi25_unmatched->fill(event);
    }
    if(genmatching->passes(event)){
      h_dphi25_genmatched->fill(event);
      h_njet_dphi25_genmatched->fill(event);
      h_topjet_dphi25_genmatched->fill(event);
      h_muon_dphi25_genmatched->fill(event);
    }

    //max 2 topjet_cand
    if(!ntopjetcand_sel2->passes(event)) return false;
    h_ntopjet2->fill(event);
    h_njet_ntopjet2->fill(event);
    h_topjet_ntopjet2->fill(event);
    h_muon_ntopjet2->fill(event);

    if(matching->passes(event)){
      h_ntopjet2_matched->fill(event);
      h_njet_ntopjet2_matched->fill(event);
      h_topjet_ntopjet2_matched->fill(event);
      h_muon_ntopjet2_matched->fill(event);
    }
    else{
      h_ntopjet2_unmatched->fill(event);
      h_njet_ntopjet2_unmatched->fill(event);
      h_topjet_ntopjet2_unmatched->fill(event);
      h_muon_ntopjet2_unmatched->fill(event);
    }
    if(genmatching->passes(event)){
      h_ntopjet2_genmatched->fill(event);
      h_njet_ntopjet2_genmatched->fill(event);
      h_topjet_ntopjet2_genmatched->fill(event);
      h_muon_ntopjet2_genmatched->fill(event);
    }


    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureGenModule)

}
