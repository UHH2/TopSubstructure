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

#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenHists.h"
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
    std::unique_ptr<Selection> nmu_sel, ntopjetcand_sel1, dphi_sel1, dphi_sel2, ntopjetcand_sel2;
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel;
    std::unique_ptr<Selection> matching, genmatching;

    std::unique_ptr<AnalysisModule> PUreweight;
    std::unique_ptr<AnalysisModule> topjetsort_by_dphi, topjetsort_by_mass, gentopjetsort_by_dphi, gentopjetsort_by_mass, genjetsel;

    std::unique_ptr<AnalysisModule> ttgenprod;
    
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nocuts, h_nocuts_matched, h_nocuts_unmatched;

    std::unique_ptr<Hists> h_1mu, h_1mu_matched, h_1mu_unmatched;

    std::unique_ptr<Hists> h_ntopjet1, h_ntopjet1_matched, h_ntopjet1_unmatched;

    std::unique_ptr<Hists> h_dphi1, h_dphi1_matched, h_dphi1_unmatched;

    std::unique_ptr<Hists> h_dphi25, h_dphi25_matched, h_dphi25_unmatched;

    std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
  };


  TopSubstructureGenModule::TopSubstructureGenModule(Context & ctx){

    // set up selections
       // decide how you want to sort your candidates
    sort_by = ctx.get("sort");
    const std::string ttbar_gen_label("ttbargen");

    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    if(sort_by == "dphi") gentopjetsort_by_dphi.reset(new GenTopJetSortDPhi(ctx));
    if(sort_by == "mass") gentopjetsort_by_mass.reset(new GenTopJetSortMass(ctx));
    nmu_sel.reset(new NMuonSelection(1,1));
    genjetsel.reset(new GenJetSelection(ctx));
    genmatching.reset(new GenQuarkGenJetMatching(ctx));
    ntopjetcand_sel1.reset(new GenNTopJet(ctx,1));
    dphi_sel1.reset(new GenDPhiSelection(ctx,1));
    dphi_sel2.reset(new GenDPhiSelection(ctx,2.5));
    ntopjetcand_sel2.reset(new GenNTopJet(ctx,1,2));

    // 3. Set up Hists classes:
    h_nocuts.reset(new TopSubstructureGenHists(ctx, "NoCuts", sort_by));
    h_nocuts_matched.reset(new TopSubstructureGenHists(ctx, "NoCuts_matched", sort_by));
    h_nocuts_unmatched.reset(new TopSubstructureGenHists(ctx, "NoCuts_unmatched", sort_by));



    h_1mu.reset(new TopSubstructureGenHists(ctx, "1mu", sort_by));
    h_1mu_matched.reset(new TopSubstructureGenHists(ctx, "1mu_matched", sort_by));
    h_1mu_unmatched.reset(new TopSubstructureGenHists(ctx, "1mu_unmatched", sort_by));


    h_ntopjet1.reset(new TopSubstructureGenHists(ctx, "ntopjet1", sort_by));
    h_ntopjet1_matched.reset(new TopSubstructureGenHists(ctx, "ntopjet1_matched", sort_by));
    h_ntopjet1_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjet1_unmatched", sort_by));



    h_dphi1.reset(new TopSubstructureGenHists(ctx, "dphi1", sort_by));
    h_dphi1_matched.reset(new TopSubstructureGenHists(ctx, "dphi1_matched", sort_by));
    h_dphi1_unmatched.reset(new TopSubstructureGenHists(ctx, "dphi1_unmatched", sort_by));



    h_dphi25.reset(new TopSubstructureGenHists(ctx, "dphi25", sort_by));
    h_dphi25_matched.reset(new TopSubstructureGenHists(ctx, "dphi25_matched", sort_by));
    h_dphi25_unmatched.reset(new TopSubstructureGenHists(ctx, "dphi25_unmatched", sort_by));



    h_ntopjet2.reset(new TopSubstructureGenHists(ctx, "ntopjet2", sort_by));
    h_ntopjet2_matched.reset(new TopSubstructureGenHists(ctx, "ntopjet2_matched", sort_by));
    h_ntopjet2_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjet2_unmatched", sort_by));

  }


  bool TopSubstructureGenModule::process(Event & event) {    
    cout << "TopSubstructureGenModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    // 1. run all modules other modules.

    // sort the topjets for our needs
    if(sort_by == "dphi") gentopjetsort_by_dphi->process(event);
    if(sort_by == "mass") gentopjetsort_by_mass->process(event);

    ttgenprod->process(event);
    genjetsel->process(event);


    // 2. test selections and fill histograms
    h_nocuts->fill(event);

    if(genmatching->passes(event)){
      h_nocuts_matched->fill(event);
    }
    else{
      h_nocuts_unmatched->fill(event);
    }`

    //1 muon
    if(!nmu_sel->passes(event)) return false;
    h_1mu->fill(event);

    if(genmatching->passes(event)){
      h_1mu_matched->fill(event);
    }
    else{
      h_1mu_unmatched->fill(event);
    }

    //min 1 topjet_cand
    if(!ntopjetcand_sel1->passes(event)) return false;
    h_ntopjet1->fill(event);

    if(genmatching->passes(event)){
      h_ntopjet1_matched->fill(event);
    }
    else{
      h_ntopjet1_unmatched->fill(event);
    }

    //dphi(mu,jet1) > 1
    if(!dphi_sel1->passes(event)) return false;
    h_dphi1->fill(event);

    if(genmatching->passes(event)){
      h_dphi1_matched->fill(event);
    }
    else{
      h_dphi1_unmatched->fill(event);
    }

    //dphi(mu,jet1) > 2.5
    if(!dphi_sel2->passes(event)) return false;
    h_dphi25->fill(event);

    if(genmatching->passes(event)){
      h_dphi25_matched->fill(event);
    }
    else{
      h_dphi25_unmatched->fill(event);
    }

    //max 2 topjet_cand
    if(!ntopjetcand_sel2->passes(event)) return false;
    h_ntopjet2->fill(event);

    if(genmatching->passes(event)){
      h_ntopjet2_matched->fill(event);
    }
    else{
      h_ntopjet2_unmatched->fill(event);
    }


    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureGenModule)

}
