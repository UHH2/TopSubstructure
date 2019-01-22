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
    std::unique_ptr<uhh2::AnalysisModule> lumiweight;
    bool isMC;
    string sort_by;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> mass_sel, ntopjet, ntopjetcand_sel1, dphi_sel1, dphi_sel2, ntopjetcand_sel2, dr_sel;
    std::unique_ptr<Selection> pt_sel1, pt_sel2, pt_sel3, pt_sel4, pt_sel5, pt_sel6, pt_sel7, pt_sel8, pt_sel9, pt_sel10, pt_sel11, pt_sel12, pt_sel13, pt_sel14, pt_sel15, pt_sel16, pt_sel17, pt_sel18, pt_sel19, pt_sel20, pt_sel21, pt_sel22, pt_sel23, pt_sel24, pt_sel25, pt_sel26, pt_sel27, pt_sel28, pt_sel29, pt_sel30, pt_sel31;
    std::unique_ptr<Selection> genmatching;
    std::unique_ptr<Selection> semilep;

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

    std::unique_ptr<Hists> h_dR, h_dR_matched, h_dR_unmatched;

    std::unique_ptr<Hists> h_topjets_pt1, h_topjets_pt1_matched, h_topjets_pt1_unmatched;

    std::unique_ptr<Hists> h_topjets_pt2, h_topjets_pt2_matched, h_topjets_pt2_unmatched;

    std::unique_ptr<Hists> h_topjets_pt3, h_topjets_pt3_matched, h_topjets_pt3_unmatched;

    std::unique_ptr<Hists> h_topjets_pt4, h_topjets_pt4_matched, h_topjets_pt4_unmatched;

    std::unique_ptr<Hists> h_topjets_pt5, h_topjets_pt5_matched, h_topjets_pt5_unmatched;

    std::unique_ptr<Hists> h_topjets_pt6, h_topjets_pt6_matched, h_topjets_pt6_unmatched;

    std::unique_ptr<Hists> h_topjets_pt7, h_topjets_pt7_matched, h_topjets_pt7_unmatched;

    std::unique_ptr<Hists> h_topjets_pt8, h_topjets_pt8_matched, h_topjets_pt8_unmatched;

    std::unique_ptr<Hists> h_topjets_pt9, h_topjets_pt9_matched, h_topjets_pt9_unmatched;

    std::unique_ptr<Hists> h_topjets_pt10, h_topjets_pt10_matched, h_topjets_pt10_unmatched;

    std::unique_ptr<Hists> h_topjets_pt11, h_topjets_pt11_matched, h_topjets_pt11_unmatched;

    std::unique_ptr<Hists> h_topjets_pt12, h_topjets_pt12_matched, h_topjets_pt12_unmatched;

    std::unique_ptr<Hists> h_topjets_pt13, h_topjets_pt13_matched, h_topjets_pt13_unmatched;

    std::unique_ptr<Hists> h_topjets_pt14, h_topjets_pt14_matched, h_topjets_pt14_unmatched;

    std::unique_ptr<Hists> h_topjets_pt15, h_topjets_pt15_matched, h_topjets_pt15_unmatched;

    std::unique_ptr<Hists> h_topjets_pt16, h_topjets_pt16_matched, h_topjets_pt16_unmatched;

    std::unique_ptr<Hists> h_topjets_pt17, h_topjets_pt17_matched, h_topjets_pt17_unmatched;

    std::unique_ptr<Hists> h_topjets_pt18, h_topjets_pt18_matched, h_topjets_pt18_unmatched;

    std::unique_ptr<Hists> h_topjets_pt19, h_topjets_pt19_matched, h_topjets_pt19_unmatched;

    std::unique_ptr<Hists> h_topjets_pt20, h_topjets_pt20_matched, h_topjets_pt20_unmatched;

    std::unique_ptr<Hists> h_topjets_pt21, h_topjets_pt21_matched, h_topjets_pt21_unmatched;

    std::unique_ptr<Hists> h_topjets_pt22, h_topjets_pt22_matched, h_topjets_pt22_unmatched;

    std::unique_ptr<Hists> h_topjets_pt23, h_topjets_pt23_matched, h_topjets_pt23_unmatched;

    std::unique_ptr<Hists> h_topjets_pt24, h_topjets_pt24_matched, h_topjets_pt24_unmatched;

    std::unique_ptr<Hists> h_topjets_pt25, h_topjets_pt25_matched, h_topjets_pt25_unmatched;

    std::unique_ptr<Hists> h_topjets_pt26, h_topjets_pt26_matched, h_topjets_pt26_unmatched;

    std::unique_ptr<Hists> h_topjets_pt27, h_topjets_pt27_matched, h_topjets_pt27_unmatched;

    std::unique_ptr<Hists> h_topjets_pt28, h_topjets_pt28_matched, h_topjets_pt28_unmatched;

    std::unique_ptr<Hists> h_topjets_pt29, h_topjets_pt29_matched, h_topjets_pt29_unmatched;

    std::unique_ptr<Hists> h_topjets_pt30, h_topjets_pt30_matched, h_topjets_pt30_unmatched;

    std::unique_ptr<Hists> h_topjets_pt31, h_topjets_pt31_matched, h_topjets_pt31_unmatched;

    std::unique_ptr<Hists> h_topjets_pt32, h_topjets_pt32_matched, h_topjets_pt32_unmatched;
  };


  TopSubstructureGenModule::TopSubstructureGenModule(Context & ctx){
    // set up selections
    // decide how you want to sort your candidates
    lumiweight.reset(new MCLumiWeight(ctx));
    cleaner.reset(new GenTopJetLeptonCleaner(ctx));


    sort_by = ctx.get("sort");
    // if(sort_by == "sel" || sort_by == "pt" || sort_by == "dphipt") gentjetcleaner.reset(new GenTopJetCleaner(ctx));
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

    // genjetsel.reset(new GenJetSelection(ctx));
    genmatching.reset(new GenQuarkGenJetMatching(ctx));
    if(sort_by == "dphimass1" || sort_by == "dphimasspi" || sort_by == "dphimass25" || sort_by == "sel") mass_sel.reset(new MassSelection(ctx,1));
    ntopjetcand_sel1.reset(new GenNTopJetCand(ctx,1));
    if(sort_by != "sel") dphi_sel1.reset(new GenDPhiSelection(ctx,1));
    if(sort_by == "dphimass1") dphi_sel2.reset(new GenDPhiSelection(ctx,1));
    else if(sort_by == "dphimass25" || sort_by == "dphipt" || sort_by == "pt") dphi_sel2.reset(new GenDPhiSelection(ctx,2.5));
    else if(sort_by == "dphimasspi") dphi_sel2.reset(new GenDPhiSelection(ctx,pif));
    if(sort_by == "sel") ntopjet.reset(new GenNTopJet(2,2));
    if(sort_by != "sel") ntopjetcand_sel2.reset(new GenNTopJetCand(ctx,1,2));
    if(sort_by == "sel") pt_sel1.reset(new PtSelection(400, 0));
    if(sort_by == "sel") pt_sel2.reset(new PtSelection(400, 10));
    if(sort_by == "sel") pt_sel3.reset(new PtSelection(400, 20));
    if(sort_by == "sel") pt_sel4.reset(new PtSelection(400, 30));
    if(sort_by == "sel") pt_sel5.reset(new PtSelection(400, 40));
    if(sort_by == "sel") pt_sel6.reset(new PtSelection(400, 50));
    if(sort_by == "sel") pt_sel7.reset(new PtSelection(400, 60));
    if(sort_by == "sel") pt_sel8.reset(new PtSelection(400, 70));
    if(sort_by == "sel") pt_sel9.reset(new PtSelection(400, 80));
    if(sort_by == "sel") pt_sel10.reset(new PtSelection(400, 90));
    if(sort_by == "sel") pt_sel11.reset(new PtSelection(400, 100));
    if(sort_by == "sel") pt_sel12.reset(new PtSelection(400, 110));
    if(sort_by == "sel") pt_sel13.reset(new PtSelection(400, 120));
    if(sort_by == "sel") pt_sel14.reset(new PtSelection(400, 130));
    if(sort_by == "sel") pt_sel15.reset(new PtSelection(400, 140));
    if(sort_by == "sel") pt_sel16.reset(new PtSelection(400, 150));
    if(sort_by == "sel") pt_sel17.reset(new PtSelection(400, 160));
    if(sort_by == "sel") pt_sel18.reset(new PtSelection(400, 170));
    if(sort_by == "sel") pt_sel19.reset(new PtSelection(400, 180));
    if(sort_by == "sel") pt_sel20.reset(new PtSelection(400, 190));
    if(sort_by == "sel") pt_sel21.reset(new PtSelection(400, 200));
    if(sort_by == "sel") pt_sel22.reset(new PtSelection(400, 210));
    if(sort_by == "sel") pt_sel23.reset(new PtSelection(400, 220));
    if(sort_by == "sel") pt_sel24.reset(new PtSelection(400, 230));
    if(sort_by == "sel") pt_sel25.reset(new PtSelection(400, 240));
    if(sort_by == "sel") pt_sel26.reset(new PtSelection(400, 250));
    if(sort_by == "sel") pt_sel27.reset(new PtSelection(400, 260));
    if(sort_by == "sel") pt_sel28.reset(new PtSelection(400, 270));
    if(sort_by == "sel") pt_sel29.reset(new PtSelection(400, 280));
    if(sort_by == "sel") pt_sel30.reset(new PtSelection(400, 290));
    if(sort_by == "sel") pt_sel31.reset(new PtSelection(400, 300));
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


      h_mass.reset(new TopSubstructureGenHists(ctx, "mass"));
      h_mass_matched.reset(new TopSubstructureGenHists(ctx, "mass_matched"));
      h_mass_unmatched.reset(new TopSubstructureGenHists(ctx, "mass_unmatched"));
    }
    else if(sort_by == "sel"){
      h_ntopjets.reset(new TopSubstructureGenHists(ctx, "ntopjets"));
      h_ntopjets_matched.reset(new TopSubstructureGenHists(ctx, "ntopjets_matched"));
      h_ntopjets_unmatched.reset(new TopSubstructureGenHists(ctx, "ntopjets_unmatched"));


      h_mass.reset(new TopSubstructureGenHists(ctx, "mass"));
      h_mass_matched.reset(new TopSubstructureGenHists(ctx, "mass_matched"));
      h_mass_unmatched.reset(new TopSubstructureGenHists(ctx, "mass_unmatched"));


      h_dR.reset(new TopSubstructureGenHists(ctx, "dR"));
      h_dR_matched.reset(new TopSubstructureGenHists(ctx, "dR_matched"));
      h_dR_unmatched.reset(new TopSubstructureGenHists(ctx, "dR_unmatched"));


      h_topjets_pt1.reset(new TopSubstructureGenHists(ctx, "topjets_pt1"));
      h_topjets_pt1_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt1_matched"));
      h_topjets_pt1_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt1_unmatched"));


      h_topjets_pt2.reset(new TopSubstructureGenHists(ctx, "topjets_pt2"));
      h_topjets_pt2_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt2_matched"));
      h_topjets_pt2_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt2_unmatched"));


      h_topjets_pt3.reset(new TopSubstructureGenHists(ctx, "topjets_pt3"));
      h_topjets_pt3_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt3_matched"));
      h_topjets_pt3_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt3_unmatched"));


      h_topjets_pt4.reset(new TopSubstructureGenHists(ctx, "topjets_pt4"));
      h_topjets_pt4_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt4_matched"));
      h_topjets_pt4_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt4_unmatched"));


      h_topjets_pt5.reset(new TopSubstructureGenHists(ctx, "topjets_pt5"));
      h_topjets_pt5_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt5_matched"));
      h_topjets_pt5_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt5_unmatched"));


      h_topjets_pt6.reset(new TopSubstructureGenHists(ctx, "topjets_pt6"));
      h_topjets_pt6_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt6_matched"));
      h_topjets_pt6_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt6_unmatched"));


      h_topjets_pt7.reset(new TopSubstructureGenHists(ctx, "topjets_pt7"));
      h_topjets_pt7_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt7_matched"));
      h_topjets_pt7_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt7_unmatched"));


      h_topjets_pt8.reset(new TopSubstructureGenHists(ctx, "topjets_pt8"));
      h_topjets_pt8_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt8_matched"));
      h_topjets_pt8_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt8_unmatched"));


      h_topjets_pt9.reset(new TopSubstructureGenHists(ctx, "topjets_pt9"));
      h_topjets_pt9_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt9_matched"));
      h_topjets_pt9_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt9_unmatched"));


      h_topjets_pt10.reset(new TopSubstructureGenHists(ctx, "topjets_pt10"));
      h_topjets_pt10_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt10_matched"));
      h_topjets_pt10_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt10_unmatched"));


      h_topjets_pt11.reset(new TopSubstructureGenHists(ctx, "topjets_pt11"));
      h_topjets_pt11_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt11_matched"));
      h_topjets_pt11_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt11_unmatched"));


      h_topjets_pt12.reset(new TopSubstructureGenHists(ctx, "topjets_pt12"));
      h_topjets_pt12_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt12_matched"));
      h_topjets_pt12_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt12_unmatched"));


      h_topjets_pt13.reset(new TopSubstructureGenHists(ctx, "topjets_pt13"));
      h_topjets_pt13_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt13_matched"));
      h_topjets_pt13_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt13_unmatched"));


      h_topjets_pt14.reset(new TopSubstructureGenHists(ctx, "topjets_pt14"));
      h_topjets_pt14_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt14_matched"));
      h_topjets_pt14_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt14_unmatched"));


      h_topjets_pt15.reset(new TopSubstructureGenHists(ctx, "topjets_pt15"));
      h_topjets_pt15_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt15_matched"));
      h_topjets_pt15_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt15_unmatched"));


      h_topjets_pt16.reset(new TopSubstructureGenHists(ctx, "topjets_pt16"));
      h_topjets_pt16_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt16_matched"));
      h_topjets_pt16_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt16_unmatched"));


      h_topjets_pt17.reset(new TopSubstructureGenHists(ctx, "topjets_pt17"));
      h_topjets_pt17_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt17_matched"));
      h_topjets_pt17_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt17_unmatched"));


      h_topjets_pt18.reset(new TopSubstructureGenHists(ctx, "topjets_pt18"));
      h_topjets_pt18_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt18_matched"));
      h_topjets_pt18_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt18_unmatched"));


      h_topjets_pt19.reset(new TopSubstructureGenHists(ctx, "topjets_pt19"));
      h_topjets_pt19_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt19_matched"));
      h_topjets_pt19_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt19_unmatched"));


      h_topjets_pt20.reset(new TopSubstructureGenHists(ctx, "topjets_pt20"));
      h_topjets_pt20_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt20_matched"));
      h_topjets_pt20_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt20_unmatched"));


      h_topjets_pt21.reset(new TopSubstructureGenHists(ctx, "topjets_pt21"));
      h_topjets_pt21_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt21_matched"));
      h_topjets_pt21_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt21_unmatched"));


      h_topjets_pt22.reset(new TopSubstructureGenHists(ctx, "topjets_pt22"));
      h_topjets_pt22_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt22_matched"));
      h_topjets_pt22_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt22_unmatched"));


      h_topjets_pt23.reset(new TopSubstructureGenHists(ctx, "topjets_pt23"));
      h_topjets_pt23_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt23_matched"));
      h_topjets_pt23_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt23_unmatched"));


      h_topjets_pt24.reset(new TopSubstructureGenHists(ctx, "topjets_pt24"));
      h_topjets_pt24_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt24_matched"));
      h_topjets_pt24_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt24_unmatched"));


      h_topjets_pt25.reset(new TopSubstructureGenHists(ctx, "topjets_pt25"));
      h_topjets_pt25_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt25_matched"));
      h_topjets_pt25_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt25_unmatched"));


      h_topjets_pt26.reset(new TopSubstructureGenHists(ctx, "topjets_pt26"));
      h_topjets_pt26_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt26_matched"));
      h_topjets_pt26_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt26_unmatched"));


      h_topjets_pt27.reset(new TopSubstructureGenHists(ctx, "topjets_pt27"));
      h_topjets_pt27_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt27_matched"));
      h_topjets_pt27_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt27_unmatched"));


      h_topjets_pt28.reset(new TopSubstructureGenHists(ctx, "topjets_pt28"));
      h_topjets_pt28_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt28_matched"));
      h_topjets_pt28_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt28_unmatched"));


      h_topjets_pt29.reset(new TopSubstructureGenHists(ctx, "topjets_pt29"));
      h_topjets_pt29_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt29_matched"));
      h_topjets_pt29_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt29_unmatched"));


      h_topjets_pt30.reset(new TopSubstructureGenHists(ctx, "topjets_pt30"));
      h_topjets_pt30_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt30_matched"));
      h_topjets_pt30_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt30_unmatched"));


      h_topjets_pt31.reset(new TopSubstructureGenHists(ctx, "topjets_pt31"));
      h_topjets_pt31_matched.reset(new TopSubstructureGenHists(ctx, "topjets_pt31_matched"));
      h_topjets_pt31_unmatched.reset(new TopSubstructureGenHists(ctx, "topjets_pt31_unmatched"));
    }
  }

  bool TopSubstructureGenModule::process(Event & event) {
    cout << "TopSubstructureGenModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    lumiweight->process(event);

    // 1. run all modules other modules.
    ttgenprod->process(event);
    //1 mu
    if(!semilep->passes(event)) return false;
    cleaner->process(event); // Do this always!
    if(sort_by != "sel")gentopjetsort->process(event);
    // else gentjetcleaner->process(event);
    // genjetsel->process(event);


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

      if((sort_by == "dphimass1" || sort_by == "dphimasspi" || sort_by == "dphimass25") && !mass_sel->passes(event)) return false;
      h_mass->fill(event);

      if(genmatching->passes(event)) h_mass_matched->fill(event);
      else h_mass_unmatched->fill(event);



    }
    else if(sort_by == "sel"){
      // exactly two GenTopJets
      if(!ntopjet->passes(event)) return false;
      h_ntopjets->fill(event);

      if(genmatching->passes(event)) h_ntopjets_matched->fill(event);
      else h_ntopjets_unmatched->fill(event);

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

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel1->passes(event)) return false;
      h_topjets_pt1->fill(event);

      if(genmatching->passes(event)) h_topjets_pt1_matched->fill(event);
      else h_topjets_pt1_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel2->passes(event)) return false;
      h_topjets_pt2->fill(event);

      if(genmatching->passes(event)) h_topjets_pt2_matched->fill(event);
      else h_topjets_pt2_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel3->passes(event)) return false;
      h_topjets_pt3->fill(event);

      if(genmatching->passes(event)) h_topjets_pt3_matched->fill(event);
      else h_topjets_pt3_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel4->passes(event)) return false;
      h_topjets_pt4->fill(event);

      if(genmatching->passes(event)) h_topjets_pt4_matched->fill(event);
      else h_topjets_pt4_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel5->passes(event)) return false;
      h_topjets_pt5->fill(event);

      if(genmatching->passes(event)) h_topjets_pt5_matched->fill(event);
      else h_topjets_pt5_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel6->passes(event)) return false;
      h_topjets_pt6->fill(event);

      if(genmatching->passes(event)) h_topjets_pt6_matched->fill(event);
      else h_topjets_pt6_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel7->passes(event)) return false;
      h_topjets_pt7->fill(event);

      if(genmatching->passes(event)) h_topjets_pt7_matched->fill(event);
      else h_topjets_pt7_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel8->passes(event)) return false;
      h_topjets_pt8->fill(event);

      if(genmatching->passes(event)) h_topjets_pt8_matched->fill(event);
      else h_topjets_pt8_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel9->passes(event)) return false;
      h_topjets_pt9->fill(event);

      if(genmatching->passes(event)) h_topjets_pt9_matched->fill(event);
      else h_topjets_pt9_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel10->passes(event)) return false;
      h_topjets_pt10->fill(event);

      if(genmatching->passes(event)) h_topjets_pt10_matched->fill(event);
      else h_topjets_pt10_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel11->passes(event)) return false;
      h_topjets_pt11->fill(event);

      if(genmatching->passes(event)) h_topjets_pt11_matched->fill(event);
      else h_topjets_pt11_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel12->passes(event)) return false;
      h_topjets_pt12->fill(event);

      if(genmatching->passes(event)) h_topjets_pt12_matched->fill(event);
      else h_topjets_pt12_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel13->passes(event)) return false;
      h_topjets_pt13->fill(event);

      if(genmatching->passes(event)) h_topjets_pt13_matched->fill(event);
      else h_topjets_pt13_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel14->passes(event)) return false;
      h_topjets_pt14->fill(event);

      if(genmatching->passes(event)) h_topjets_pt14_matched->fill(event);
      else h_topjets_pt14_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel15->passes(event)) return false;
      h_topjets_pt15->fill(event);

      if(genmatching->passes(event)) h_topjets_pt15_matched->fill(event);
      else h_topjets_pt15_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel16->passes(event)) return false;
      h_topjets_pt16->fill(event);

      if(genmatching->passes(event)) h_topjets_pt16_matched->fill(event);
      else h_topjets_pt16_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel17->passes(event)) return false;
      h_topjets_pt17->fill(event);

      if(genmatching->passes(event)) h_topjets_pt17_matched->fill(event);
      else h_topjets_pt17_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel18->passes(event)) return false;
      h_topjets_pt18->fill(event);

      if(genmatching->passes(event)) h_topjets_pt18_matched->fill(event);
      else h_topjets_pt18_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel19->passes(event)) return false;
      h_topjets_pt19->fill(event);

      if(genmatching->passes(event)) h_topjets_pt19_matched->fill(event);
      else h_topjets_pt19_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel20->passes(event)) return false;
      h_topjets_pt20->fill(event);

      if(genmatching->passes(event)) h_topjets_pt20_matched->fill(event);
      else h_topjets_pt20_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel21->passes(event)) return false;
      h_topjets_pt21->fill(event);

      if(genmatching->passes(event)) h_topjets_pt21_matched->fill(event);
      else h_topjets_pt21_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel22->passes(event)) return false;
      h_topjets_pt22->fill(event);

      if(genmatching->passes(event)) h_topjets_pt22_matched->fill(event);
      else h_topjets_pt22_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel23->passes(event)) return false;
      h_topjets_pt23->fill(event);

      if(genmatching->passes(event)) h_topjets_pt23_matched->fill(event);
      else h_topjets_pt23_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel24->passes(event)) return false;
      h_topjets_pt24->fill(event);

      if(genmatching->passes(event)) h_topjets_pt24_matched->fill(event);
      else h_topjets_pt24_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel25->passes(event)) return false;
      h_topjets_pt25->fill(event);

      if(genmatching->passes(event)) h_topjets_pt25_matched->fill(event);
      else h_topjets_pt25_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel26->passes(event)) return false;
      h_topjets_pt26->fill(event);

      if(genmatching->passes(event)) h_topjets_pt26_matched->fill(event);
      else h_topjets_pt26_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel27->passes(event)) return false;
      h_topjets_pt27->fill(event);

      if(genmatching->passes(event)) h_topjets_pt27_matched->fill(event);
      else h_topjets_pt27_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel28->passes(event)) return false;
      h_topjets_pt28->fill(event);

      if(genmatching->passes(event)) h_topjets_pt28_matched->fill(event);
      else h_topjets_pt28_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel29->passes(event)) return false;
      h_topjets_pt29->fill(event);

      if(genmatching->passes(event)) h_topjets_pt29_matched->fill(event);
      else h_topjets_pt29_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel30->passes(event)) return false;
      h_topjets_pt30->fill(event);

      if(genmatching->passes(event)) h_topjets_pt30_matched->fill(event);
      else h_topjets_pt30_unmatched->fill(event);

      // pt jet1 > 400; pt jet2 > 200
      if(!pt_sel31->passes(event)) return false;
      h_topjets_pt31->fill(event);

      if(genmatching->passes(event)) h_topjets_pt31_matched->fill(event);
      else h_topjets_pt31_unmatched->fill(event);
    }


    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureGenModule)

}
