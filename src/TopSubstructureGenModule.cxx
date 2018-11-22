#include <iostream>
#include <memory>
#include <math.h>

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
    std::unique_ptr<Selection> mass_sel, ntopjet, ntopjetcand_sel1, dphi_sel1, dphi_sel2, ntopjetcand_sel2, pt_sel, dr_sel;
    std::unique_ptr<Selection> genmatching;
    std::unique_ptr<Selection> semilep;

    std::unique_ptr<AnalysisModule> PUreweight;
    std::unique_ptr<AnalysisModule> cleaner, gentjetcleaner, topjetsort_by_dphi, topjetsort_by_mass, gentopjetsort, genjetsel;

    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_mu1, h_mu1_matched, h_mu1_unmatched;

    std::unique_ptr<Hists> h_mass, h_mass_matched, h_mass_unmatched;

    std::unique_ptr<Hists> h_ntopjetcand1, h_ntopjetcand1_matched, h_ntopjetcand1_unmatched;

    std::unique_ptr<Hists> h_dphi1, h_dphi1_matched, h_dphi1_unmatched;

    std::unique_ptr<Hists> h_dphi25, h_dphi25_matched, h_dphi25_unmatched;

    std::unique_ptr<Hists> h_ntopjetcand2, h_ntopjetcand2_matched, h_ntopjetcand2_unmatched;

    std::unique_ptr<Hists> h_ntopjets, h_ntopjets_matched, h_ntopjets_unmatched;

    std::unique_ptr<Hists> h_topjets_pt, h_topjets_pt_matched, h_topjets_pt_unmatched;

    std::unique_ptr<Hists> h_dR, h_dR_matched, h_dR_unmatched;
  };


  TopSubstructureGenModule::TopSubstructureGenModule(Context & ctx){
    // set up selections
    // decide how you want to sort your candidates
    cleaner.reset(new GenTopJetLeptonCleaner(ctx));


    sort_by = ctx.get("sort");
    if(sort_by == "sel" || sort_by == "pt" || sort_by == "dphipt") gentjetcleaner.reset(new GenTopJetCleaner(ctx));
    double pif = (2./3.)*M_PI;
    const std::string ttbar_gen_label("ttbargen");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    semilep.reset(new TTbarSemilep(ctx));

    if(sort_by == "dphi") gentopjetsort.reset(new GenTopJetSortDPhi(ctx));
    else if(sort_by == "mass") gentopjetsort.reset(new GenTopJetSortMass(ctx));
    else if(sort_by == "dphimass1") gentopjetsort.reset(new GenTopJetSortDPhiMass(ctx, 1));
    else if(sort_by == "dphimasspi") gentopjetsort.reset(new GenTopJetSortDPhiMass(ctx, pif));
    else if(sort_by == "dphimass25") gentopjetsort.reset(new GenTopJetSortDPhiMass(ctx, 2.5));
    else if(sort_by == "pt") gentopjetsort.reset(new GenTopJetSortPt(ctx));
    else if(sort_by == "dphipt") gentopjetsort.reset(new GenTopJetSortDPhiPt(ctx));

    genjetsel.reset(new GenJetSelection(ctx));
    genmatching.reset(new GenQuarkGenJetMatching(ctx));
    if(sort_by == "dphimass1" || sort_by == "dphimasspi" || sort_by == "dphimass25" || sort_by == "sel") mass_sel.reset(new MassSelection(ctx));
    ntopjetcand_sel1.reset(new GenNTopJetCand(ctx,1));
    if(sort_by != "sel") dphi_sel1.reset(new GenDPhiSelection(ctx,1));
    if(sort_by == "dphimass1") dphi_sel2.reset(new GenDPhiSelection(ctx,1));
    else if(sort_by == "dphimass25" || sort_by == "dphipt" || sort_by == "pt") dphi_sel2.reset(new GenDPhiSelection(ctx,2.5));
    else if(sort_by == "dphimasspi") dphi_sel2.reset(new GenDPhiSelection(ctx,pif));
    if(sort_by == "sel") ntopjet.reset(new GenNTopJet(ctx,2,2));
    if(sort_by != "sel") ntopjetcand_sel2.reset(new GenNTopJetCand(ctx,1,2));
    if(sort_by == "sel") pt_sel.reset(new PtSelection(ctx, 400, 200));
    if(sort_by == "sel") dr_sel.reset(new dRSelection(ctx));

    // 3. Set up Hists classes:
    h_mu1.reset(new TopSubstructureGenHists(ctx, "mu1"));
    h_mu1_matched.reset(new TopSubstructureGenHists(ctx, "mu1_matched"));
    h_mu1_unmatched.reset(new TopSubstructureGenHists(ctx, "mu1_unmatched"));

    if(sort_by != "sel"){
      h_ntopjetcand1.reset(new TopSubstructureGenHists(ctx, "ntopjetcand1"));
      h_ntopjetcand1_matched.reset(new TopSubstructureGenHists(ctx, "ntopjetcand1_matched"));
      h_ntopjetcand1_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjetcand1_unmatched"));


      h_dphi1.reset(new TopSubstructureGenHists(ctx, "dphi1"));
      h_dphi1_matched.reset(new TopSubstructureGenHists(ctx, "dphi1_matched"));
      h_dphi1_unmatched.reset(new TopSubstructureGenHists(ctx, "dphi1_unmatched"));


      h_dphi25.reset(new TopSubstructureGenHists(ctx, "dphi25"));
      h_dphi25_matched.reset(new TopSubstructureGenHists(ctx, "dphi25_matched"));
      h_dphi25_unmatched.reset(new TopSubstructureGenHists(ctx, "dphi25_unmatched"));


      h_ntopjetcand2.reset(new TopSubstructureGenHists(ctx, "ntopjetcand2"));
      h_ntopjetcand2_matched.reset(new TopSubstructureGenHists(ctx, "ntopjetcand2_matched"));
      h_ntopjetcand2_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjetcand2_unmatched"));

      //
      // h_mass.reset(new TopSubstructureGenHists(ctx, "mass"));
      // h_mass_matched.reset(new TopSubstructureGenHists(ctx, "mass_matched"));
      // h_mass_unmatched.reset(new TopSubstructureGenHists(ctx, "mass_unmatched"));
    }
    else if(sort_by == "sel"){
      h_ntopjets.reset(new TopSubstructureGenHists(ctx, "ntopjets"));
      h_ntopjets_matched.reset(new TopSubstructureGenHists(ctx, "ntopjets_matched"));
      h_ntopjets_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjets_unmatched"));


      h_topjets_pt.reset(new TopSubstructureGenHists(ctx, "topjets_pt"));
      h_topjets_pt_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt_matched"));
      h_topjets_pt_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt_unmatched"));


      h_mass.reset(new TopSubstructureGenHists(ctx, "mass"));
      h_mass_matched.reset(new TopSubstructureGenHists(ctx, "mass_matched"));
      h_mass_unmatched.reset(new TopSubstructureGenHists(ctx, "mass_unmatched"));


      h_dR.reset(new TopSubstructureGenHists(ctx, "dR"));
      h_dR_matched.reset(new TopSubstructureGenHists(ctx, "dR_matched"));
      h_dR_unmatched.reset(new TopSubstructureGenHists(ctx, "dR_unmatched"));
    }
  }


  bool TopSubstructureGenModule::process(Event & event) {
    cout << "TopSubstructureGenModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    // 1. run all modules other modules.
    ttgenprod->process(event);
    //1 mu
    if(!semilep->passes(event)) return false;
    cleaner->process(event); // Do this always!
    if(sort_by != "sel")gentopjetsort->process(event);
    else gentjetcleaner->process(event);
    genjetsel->process(event);


    h_mu1->fill(event);
    if(genmatching->passes(event)) h_mu1_matched->fill(event);
    else h_mu1_unmatched->fill(event);



    if(sort_by != "sel"){
      //min 1 topjet_cand
      if(!ntopjetcand_sel1->passes(event)) return false;
      h_ntopjetcand1->fill(event);

      if(genmatching->passes(event)) h_ntopjetcand1_matched->fill(event);
      else h_ntopjetcand1_unmatched->fill(event);

      //dphi(mu,jet1) > 1
      if(!dphi_sel1->passes(event)) return false;
      h_dphi1->fill(event);

      if(genmatching->passes(event)) h_dphi1_matched->fill(event);
      else h_dphi1_unmatched->fill(event);

      //dphi(mu,jet1) > 2.5
      if((sort_by == "dphimass25" || sort_by == "dphimasspi" || sort_by == "dphipt" || sort_by == "pt") && !dphi_sel2->passes(event)) return false;
      h_dphi25->fill(event);

      if(genmatching->passes(event)) h_dphi25_matched->fill(event);
      else h_dphi25_unmatched->fill(event);

      //max 2 topjet_cand
      if(!ntopjetcand_sel2->passes(event)) return false;
      h_ntopjetcand2->fill(event);
      if(genmatching->passes(event)) h_ntopjetcand2_matched->fill(event);
      else h_ntopjetcand2_unmatched->fill(event);
      //
      // if((sort_by == "dphimass1" || sort_by == "dphimasspi" || sort_by == "dphimass25") && !mass_sel->passes(event)) return false;
      // h_mass->fill(event);
      //
      // if(genmatching->passes(event)) h_mass_matched->fill(event);
      // else h_mass_unmatched->fill(event);
      //


    }
    else if(sort_by == "sel"){
      // exactly two GenTopJets
      if(!ntopjet->passes(event)) return false;
      h_ntopjets->fill(event);

      if(genmatching->passes(event)) h_ntopjets_matched->fill(event);
      else h_ntopjets_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel->passes(event)) return false;
      h_topjets_pt->fill(event);

      if(genmatching->passes(event)) h_topjets_pt_matched->fill(event);
      else h_topjets_pt_unmatched->fill(event);

      // mass of had jet > m of lep jet + muon
      if(!mass_sel->passes(event)) return false;
      h_mass->fill(event);

      if(genmatching->passes(event)) h_mass_matched->fill(event);
      else h_mass_unmatched->fill(event);

      // dR(mu,jet2) < 0.8
      if(!dr_sel->passes(event)) return false;
      h_dR->fill(event);

      if(genmatching->passes(event)) h_dR_matched->fill(event);
      else h_dR_unmatched->fill(event);
    }


    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureGenModule)

}
