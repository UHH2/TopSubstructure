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

  class TopSubstructureModule: public AnalysisModule {
  public:    
    explicit TopSubstructureModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:    
    std::unique_ptr<CommonModules> common;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;
    bool isMC, isTTbar;
    string sort_by;
    
    std::unique_ptr<JetCleaner> jetcleaner;
   
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> nbtag_tight_sel, ntopjet_sel, twodcut_sel, met_sel, nmu_sel, pt_sel, nele_sel, pt_mu_sel, ntopjetcand_sel1, dphi_sel1, dphi_sel2, ntopjetcand_sel2;
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

    std::unique_ptr<Hists> h_MET50, h_njet_MET50, h_muon_MET50, h_topjet_MET50;
    std::unique_ptr<Hists> h_MET50_matched, h_njet_MET50_matched, h_muon_MET50_matched, h_topjet_MET50_matched;
    std::unique_ptr<Hists> h_MET50_genmatched, h_njet_MET50_genmatched, h_muon_MET50_genmatched, h_topjet_MET50_genmatched;
    std::unique_ptr<Hists> h_MET50_unmatched, h_njet_MET50_unmatched, h_muon_MET50_unmatched, h_topjet_MET50_unmatched;

    std::unique_ptr<Hists> h_1mu, h_njet_1mu, h_muon_1mu, h_topjet_1mu;
    std::unique_ptr<Hists> h_1mu_matched, h_njet_1mu_matched, h_muon_1mu_matched, h_topjet_1mu_matched;
    std::unique_ptr<Hists> h_1mu_genmatched, h_njet_1mu_genmatched, h_muon_1mu_genmatched, h_topjet_1mu_genmatched;
    std::unique_ptr<Hists> h_1mu_unmatched, h_njet_1mu_unmatched, h_muon_1mu_unmatched, h_topjet_1mu_unmatched;

    std::unique_ptr<Hists> h_mupt55, h_njet_mupt55, h_muon_mupt55, h_topjet_mupt55;
    std::unique_ptr<Hists> h_mupt55_matched, h_njet_mupt55_matched, h_muon_mupt55_matched, h_topjet_mupt55_matched;
    std::unique_ptr<Hists> h_mupt55_genmatched, h_njet_mupt55_genmatched, h_muon_mupt55_genmatched, h_topjet_mupt55_genmatched;
    std::unique_ptr<Hists> h_mupt55_unmatched, h_njet_mupt55_unmatched, h_muon_mupt55_unmatched, h_topjet_mupt55_unmatched;

    std::unique_ptr<Hists> h_0ele, h_njet_0ele, h_muon_0ele, h_topjet_0ele;
    std::unique_ptr<Hists> h_0ele_matched, h_njet_0ele_matched, h_muon_0ele_matched, h_topjet_0ele_matched;
    std::unique_ptr<Hists> h_0ele_genmatched, h_njet_0ele_genmatched, h_muon_0ele_genmatched, h_topjet_0ele_genmatched;
    std::unique_ptr<Hists> h_0ele_unmatched, h_njet_0ele_unmatched, h_muon_0ele_unmatched, h_topjet_0ele_unmatched;

    std::unique_ptr<Hists> h_twodcut, h_njet_twodcut, h_muon_twodcut, h_topjet_twodcut;
    std::unique_ptr<Hists> h_twodcut_matched, h_njet_twodcut_matched, h_muon_twodcut_matched, h_topjet_twodcut_matched;
    std::unique_ptr<Hists> h_twodcut_genmatched, h_njet_twodcut_genmatched, h_muon_twodcut_genmatched, h_topjet_twodcut_genmatched;
    std::unique_ptr<Hists> h_twodcut_unmatched, h_njet_twodcut_unmatched, h_muon_twodcut_unmatched, h_topjet_twodcut_unmatched;

    std::unique_ptr<Hists> h_1topjet, h_njet_1topjet, h_muon_1topjet, h_topjet_1topjet;
    std::unique_ptr<Hists> h_1topjet_matched, h_njet_1topjet_matched, h_muon_1topjet_matched, h_topjet_1topjet_matched;
    std::unique_ptr<Hists> h_1topjet_genmatched, h_njet_1topjet_genmatched, h_muon_1topjet_genmatched, h_topjet_1topjet_genmatched;
    std::unique_ptr<Hists> h_1topjet_unmatched, h_njet_1topjet_unmatched, h_muon_1topjet_unmatched, h_topjet_1topjet_unmatched;

    std::unique_ptr<Hists> h_1btagtight, h_njet_1btagtight, h_muon_1btagtight, h_topjet_1btagtight;
    std::unique_ptr<Hists> h_1btagtight_matched, h_njet_1btagtight_matched, h_muon_1btagtight_matched, h_topjet_1btagtight_matched;
    std::unique_ptr<Hists> h_1btagtight_genmatched, h_njet_1btagtight_genmatched, h_muon_1btagtight_genmatched, h_topjet_1btagtight_genmatched;
    std::unique_ptr<Hists> h_1btagtight_unmatched, h_njet_1btagtight_unmatched, h_muon_1btagtight_unmatched, h_topjet_1btagtight_unmatched;

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


    JetId Btag_tight;
  };


  TopSubstructureModule::TopSubstructureModule(Context & ctx){
    // In the constructor, the typical tasks are to initialize the
    // member variables, in particular the AnalysisModules such as
    // CommonModules or some cleaner module, Selections and Hists.
    // But you can do more and e.g. access the configuration, as shown below.
 
    
    // 1. setup other modules. CommonModules and the JetCleaner:
    common.reset(new CommonModules());
    // TODO: configure common here, e.g. by 
    // calling common->set_*_id or common->disable_*
    common->init(ctx);

    MuonId muid = AndId<Muon>(MuonIDTight(), PtEtaCut(55., 2.4));
    ElectronId eleid = AndId<Electron>(ElectronID_Spring16_medium_noIso, PtEtaCut(55., 2.4));

    muoSR_cleaner.reset(new     MuonCleaner(muid));
    eleSR_cleaner.reset(new ElectronCleaner(eleid));
    Btag_tight = CSVBTag(CSVBTag::WP_TIGHT);
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4)); 


    trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
    trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");
   
    // decide how you want to sort your candidates


    // note that the JetCleaner is only kept for the sake of example;
    // instead of constructing a jetcleaner explicitly,
    // the cleaning can also be achieved with less code via CommonModules with:
    // common->set_jet_id(PtEtaCut(30.0, 2.4));
    // before the 'common->init(ctx)' line.

    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft");
    
    // 2. set up selections
    const std::string ttbar_gen_label("ttbargen");
    sort_by = ctx.get("sort");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    if(sort_by == "dphi") topjetsort_by_dphi.reset(new TopJetSortDPhi(ctx));
    if(sort_by == "mass") topjetsort_by_mass.reset(new TopJetSortMass(ctx));
    if(isTTbar && sort_by == "dphi") gentopjetsort_by_dphi.reset(new GenTopJetSortDPhi(ctx));
    if(isTTbar && sort_by == "mass") gentopjetsort_by_mass.reset(new GenTopJetSortMass(ctx));
    jetsel.reset(new JetSelection(ctx));
    matching.reset(new QuarkCandJetMatching(ctx));
    genmatching.reset(new QuarkGenJetMatching(ctx));
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));
    PUreweight.reset(new MCPileupReweight(ctx, "central"));
    ntopjet_sel.reset(new NTopJetSelection(1)); // see common/include/NSelections.h
    nbtag_tight_sel.reset(new NJetSelection(1, -1, Btag_tight)); 
    met_sel.reset(new METSelection(50,-1));
    nmu_sel.reset(new NMuonSelection(1,1));
    twodcut_sel.reset(new TwoDCut(0.4, 40));
    nele_sel.reset(new NElectronSelection(0, 0));
    pt_mu_sel.reset(new MuonptSelection(55));
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



    h_MET50.reset(new TopSubstructureHists(ctx, "MET50", sort_by));
    h_njet_MET50.reset(new JetHists(ctx, "Njet_MET50"));
    h_topjet_MET50.reset(new TopJetHists(ctx, "Topjet_MET50"));
    h_muon_MET50.reset(new MuonHists(ctx, "muon_MET50"));

    h_MET50_matched.reset(new TopSubstructureHists(ctx, "MET50_matched", sort_by));
    h_njet_MET50_matched.reset(new JetHists(ctx, "Njet_MET50_matched"));
    h_topjet_MET50_matched.reset(new TopJetHists(ctx, "Topjet_MET50_matched"));
    h_muon_MET50_matched.reset(new MuonHists(ctx, "muon_MET50_matched"));

    h_MET50_genmatched.reset(new TopSubstructureHists(ctx, "MET50_genmatched", sort_by));
    h_njet_MET50_genmatched.reset(new JetHists(ctx, "Njet_MET50_genmatched"));
    h_topjet_MET50_genmatched.reset(new TopJetHists(ctx, "Topjet_MET50_genmatched"));
    h_muon_MET50_genmatched.reset(new MuonHists(ctx, "muon_MET50_genmatched"));

    h_MET50_unmatched.reset(new TopSubstructureHists(ctx, "MET50_unmatched", sort_by));
    h_njet_MET50_unmatched.reset(new JetHists(ctx, "Njet_MET50_unmatched"));
    h_topjet_MET50_unmatched.reset(new TopJetHists(ctx, "Topjet_MET50_unmatched"));
    h_muon_MET50_unmatched.reset(new MuonHists(ctx, "muon_MET50_unmatched"));



    h_1mu.reset(new TopSubstructureHists(ctx, "1mu", sort_by));
    h_njet_1mu.reset(new JetHists(ctx, "Njet_1mu"));
    h_topjet_1mu.reset(new TopJetHists(ctx, "Topjet_1mu"));
    h_muon_1mu.reset(new MuonHists(ctx, "muon_1mu"));

    h_1mu_matched.reset(new TopSubstructureHists(ctx, "1mu_matched", sort_by));
    h_njet_1mu_matched.reset(new JetHists(ctx, "Njet_1mu_matched"));
    h_topjet_1mu_matched.reset(new TopJetHists(ctx, "Topjet_1mu_matched"));
    h_muon_1mu_matched.reset(new MuonHists(ctx, "muon_1mu_matched"));

    h_1mu_genmatched.reset(new TopSubstructureHists(ctx, "1mu_genmatched", sort_by));
    h_njet_1mu_genmatched.reset(new JetHists(ctx, "Njet_1mu_genmatched"));
    h_topjet_1mu_genmatched.reset(new TopJetHists(ctx, "Topjet_1mu_genmatched"));
    h_muon_1mu_genmatched.reset(new MuonHists(ctx, "muon_1mu_genmatched"));

    h_1mu_unmatched.reset(new TopSubstructureHists(ctx, "1mu_unmatched", sort_by));
    h_njet_1mu_unmatched.reset(new JetHists(ctx, "Njet_1mu_unmatched"));
    h_topjet_1mu_unmatched.reset(new TopJetHists(ctx, "Topjet_1mu_unmatched"));
    h_muon_1mu_unmatched.reset(new MuonHists(ctx, "muon_1mu_unmatched"));



    h_mupt55.reset(new TopSubstructureHists(ctx, "mupt55", sort_by));
    h_njet_mupt55.reset(new JetHists(ctx, "Njet_mupt55"));
    h_topjet_mupt55.reset(new TopJetHists(ctx, "Topjet_mupt55"));
    h_muon_mupt55.reset(new MuonHists(ctx, "muon_mupt55"));

    h_mupt55_matched.reset(new TopSubstructureHists(ctx, "mupt55_matched", sort_by));
    h_njet_mupt55_matched.reset(new JetHists(ctx, "Njet_mupt55_matched"));
    h_topjet_mupt55_matched.reset(new TopJetHists(ctx, "Topjet_mupt55_matched"));
    h_muon_mupt55_matched.reset(new MuonHists(ctx, "muon_mupt55_matched"));

    h_mupt55_genmatched.reset(new TopSubstructureHists(ctx, "mupt55_genmatched", sort_by));
    h_njet_mupt55_genmatched.reset(new JetHists(ctx, "Njet_mupt55_genmatched"));
    h_topjet_mupt55_genmatched.reset(new TopJetHists(ctx, "Topjet_mupt55_genmatched"));
    h_muon_mupt55_genmatched.reset(new MuonHists(ctx, "muon_mupt55_genmatched"));

    h_mupt55_unmatched.reset(new TopSubstructureHists(ctx, "mupt55_unmatched", sort_by));
    h_njet_mupt55_unmatched.reset(new JetHists(ctx, "Njet_mupt55_unmatched"));
    h_topjet_mupt55_unmatched.reset(new TopJetHists(ctx, "Topjet_mupt55_unmatched"));
    h_muon_mupt55_unmatched.reset(new MuonHists(ctx, "muon_mupt55_unmatched"));



    h_0ele.reset(new TopSubstructureHists(ctx, "0ele", sort_by));
    h_njet_0ele.reset(new JetHists(ctx, "Njet_0ele"));
    h_topjet_0ele.reset(new TopJetHists(ctx, "Topjet_0ele"));
    h_muon_0ele.reset(new MuonHists(ctx, "muon_0ele"));

    h_0ele_matched.reset(new TopSubstructureHists(ctx, "0ele_matched", sort_by));
    h_njet_0ele_matched.reset(new JetHists(ctx, "Njet_0ele_matched"));
    h_topjet_0ele_matched.reset(new TopJetHists(ctx, "Topjet_0ele_matched"));
    h_muon_0ele_matched.reset(new MuonHists(ctx, "muon_0ele_matched"));

    h_0ele_genmatched.reset(new TopSubstructureHists(ctx, "0ele_genmatched", sort_by));
    h_njet_0ele_genmatched.reset(new JetHists(ctx, "Njet_0ele_genmatched"));
    h_topjet_0ele_genmatched.reset(new TopJetHists(ctx, "Topjet_0ele_genmatched"));
    h_muon_0ele_genmatched.reset(new MuonHists(ctx, "muon_0ele_genmatched"));

    h_0ele_unmatched.reset(new TopSubstructureHists(ctx, "0ele_unmatched", sort_by));
    h_njet_0ele_unmatched.reset(new JetHists(ctx, "Njet_0ele_unmatched"));
    h_topjet_0ele_unmatched.reset(new TopJetHists(ctx, "Topjet_0ele_unmatched"));
    h_muon_0ele_unmatched.reset(new MuonHists(ctx, "muon_0ele_unmatched"));



    h_twodcut.reset(new TopSubstructureHists(ctx, "twodcut", sort_by));
    h_njet_twodcut.reset(new JetHists(ctx, "Njet_twodcut"));
    h_topjet_twodcut.reset(new TopJetHists(ctx, "Topjet_twodcut"));
    h_muon_twodcut.reset(new MuonHists(ctx, "muon_twodcut"));

    h_twodcut_matched.reset(new TopSubstructureHists(ctx, "twodcut_matched", sort_by));
    h_njet_twodcut_matched.reset(new JetHists(ctx, "Njet_twodcut_matched"));
    h_topjet_twodcut_matched.reset(new TopJetHists(ctx, "Topjet_twodcut_matched"));
    h_muon_twodcut_matched.reset(new MuonHists(ctx, "muon_twodcut_matched"));

    h_twodcut_genmatched.reset(new TopSubstructureHists(ctx, "twodcut_genmatched", sort_by));
    h_njet_twodcut_genmatched.reset(new JetHists(ctx, "Njet_twodcut_genmatched"));
    h_topjet_twodcut_genmatched.reset(new TopJetHists(ctx, "Topjet_twodcut_genmatched"));
    h_muon_twodcut_genmatched.reset(new MuonHists(ctx, "muon_twodcut_genmatched"));

    h_twodcut_unmatched.reset(new TopSubstructureHists(ctx, "twodcut_unmatched", sort_by));
    h_njet_twodcut_unmatched.reset(new JetHists(ctx, "Njet_twodcut_unmatched"));
    h_topjet_twodcut_unmatched.reset(new TopJetHists(ctx, "Topjet_twodcut_unmatched"));
    h_muon_twodcut_unmatched.reset(new MuonHists(ctx, "muon_twodcut_unmatched"));



    h_1topjet.reset(new TopSubstructureHists(ctx, "1topjet", sort_by));
    h_njet_1topjet.reset(new JetHists(ctx, "Njet_1topjet"));
    h_topjet_1topjet.reset(new TopJetHists(ctx, "Topjet_1topjet"));
    h_muon_1topjet.reset(new MuonHists(ctx, "muon_1topjet"));

    h_1topjet_matched.reset(new TopSubstructureHists(ctx, "1topjet_matched", sort_by));
    h_njet_1topjet_matched.reset(new JetHists(ctx, "Njet_1topjet_matched"));
    h_topjet_1topjet_matched.reset(new TopJetHists(ctx, "Topjet_1topjet_matched"));
    h_muon_1topjet_matched.reset(new MuonHists(ctx, "muon_1topjet_matched"));

    h_1topjet_genmatched.reset(new TopSubstructureHists(ctx, "1topjet_genmatched", sort_by));
    h_njet_1topjet_genmatched.reset(new JetHists(ctx, "Njet_1topjet_genmatched"));
    h_topjet_1topjet_genmatched.reset(new TopJetHists(ctx, "Topjet_1topjet_genmatched"));
    h_muon_1topjet_genmatched.reset(new MuonHists(ctx, "muon_1topjet_genmatched"));

    h_1topjet_unmatched.reset(new TopSubstructureHists(ctx, "1topjet_unmatched", sort_by));
    h_njet_1topjet_unmatched.reset(new JetHists(ctx, "Njet_1topjet_unmatched"));
    h_topjet_1topjet_unmatched.reset(new TopJetHists(ctx, "Topjet_1topjet_unmatched"));
    h_muon_1topjet_unmatched.reset(new MuonHists(ctx, "muon_1topjet_unmatched"));



    h_1btagtight.reset(new TopSubstructureHists(ctx, "1btagtight", sort_by));
    h_njet_1btagtight.reset(new JetHists(ctx, "Njet_1btagtight"));
    h_topjet_1btagtight.reset(new TopJetHists(ctx, "Topjet_1btagtight"));
    h_muon_1btagtight.reset(new MuonHists(ctx, "muon_1btagtight"));

    h_1btagtight_matched.reset(new TopSubstructureHists(ctx, "1btagtight_matched", sort_by));
    h_njet_1btagtight_matched.reset(new JetHists(ctx, "Njet_1btagtight_matched"));
    h_topjet_1btagtight_matched.reset(new TopJetHists(ctx, "Topjet_1btagtight_matched"));
    h_muon_1btagtight_matched.reset(new MuonHists(ctx, "muon_1btagtight_matched"));

    h_1btagtight_genmatched.reset(new TopSubstructureHists(ctx, "1btagtight_genmatched", sort_by));
    h_njet_1btagtight_genmatched.reset(new JetHists(ctx, "Njet_1btagtight_genmatched"));
    h_topjet_1btagtight_genmatched.reset(new TopJetHists(ctx, "Topjet_1btagtight_genmatched"));
    h_muon_1btagtight_genmatched.reset(new MuonHists(ctx, "muon_1btagtight_genmatched"));

    h_1btagtight_unmatched.reset(new TopSubstructureHists(ctx, "1btagtight_unmatched", sort_by));
    h_njet_1btagtight_unmatched.reset(new JetHists(ctx, "Njet_1btagtight_unmatched"));
    h_topjet_1btagtight_unmatched.reset(new TopJetHists(ctx, "Topjet_1btagtight_unmatched"));
    h_muon_1btagtight_unmatched.reset(new MuonHists(ctx, "muon_1btagtight_unmatched"));



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



    isMC = (ctx.get("dataset_type") == "MC");


  }


  bool TopSubstructureModule::process(Event & event) {
    // This is the main procedure, called for each event. Typically,
    // do some pre-processing by calling the modules' process method
    // of the modules constructed in the constructor (1).
    // Then, test whether the event passes some selection and -- if yes --
    // use it to fill the histograms (2).
    // Finally, decide whether or not to keep the event in the output (3);
    // this is controlled by the return value of this method: If it
    // returns true, the event is kept; if it returns false, the event
    // is thrown away.
    
    cout << "TopSubstructureModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    
    // 1. run all modules other modules.

    common->process(event);
    jetcleaner->process(event);
    muoSR_cleaner->process(event);
    eleSR_cleaner->process(event);

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    if(!isMC && event.run < 274954) {
      if(!trigger_sel_A->passes(event)) return false;
    }else{
      if(!(trigger_sel_A->passes(event) || trigger_sel_B->passes(event))) return false;
    }

    //at least 1 good primary vertex 
    if(!pv_sel->passes(event)) return false;

    /** PU Reweighting *********************/
    PUreweight->process(event);

    // sort the topjets for our needs
    if(sort_by == "dphi") topjetsort_by_dphi->process(event);
    if(sort_by == "mass") topjetsort_by_mass->process(event);
    if(isTTbar){
      if(sort_by == "dphi") gentopjetsort_by_dphi->process(event);
      if(sort_by == "mass") gentopjetsort_by_mass->process(event);
    }
    if(isTTbar) ttgenprod->process(event);
    jetsel->process(event);



    // 2. test selections and fill histograms
    h_nocuts->fill(event);
    h_njet_nocuts->fill(event);
    h_topjet_nocuts->fill(event);
    h_muon_nocuts->fill(event);

    if(isTTbar && matching->passes(event)){
      h_nocuts_matched->fill(event);
      h_njet_nocuts_matched->fill(event);
      h_topjet_nocuts_matched->fill(event);
      h_muon_nocuts_matched->fill(event);
    }
    else if(isTTbar){
      h_nocuts_unmatched->fill(event);
      h_njet_nocuts_unmatched->fill(event);
      h_topjet_nocuts_unmatched->fill(event);
      h_muon_nocuts_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_nocuts_genmatched->fill(event);
      h_njet_nocuts_genmatched->fill(event);
      h_topjet_nocuts_genmatched->fill(event);
      h_muon_nocuts_genmatched->fill(event);
    }

    //MET50
    if(!met_sel->passes(event)) return false;
    h_MET50->fill(event);
    h_njet_MET50->fill(event);
    h_topjet_MET50->fill(event);
    h_muon_MET50->fill(event);

    if(isTTbar && matching->passes(event)){
      h_MET50_matched->fill(event);
      h_njet_MET50_matched->fill(event);
      h_topjet_MET50_matched->fill(event);
      h_muon_MET50_matched->fill(event);
    }
    else if(isTTbar){
      h_MET50_unmatched->fill(event);
      h_njet_MET50_unmatched->fill(event);
      h_topjet_MET50_unmatched->fill(event);
      h_muon_MET50_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_MET50_genmatched->fill(event);
      h_njet_MET50_genmatched->fill(event);
      h_topjet_MET50_genmatched->fill(event);
      h_muon_MET50_genmatched->fill(event);
    }

    //1 muon
    if(!nmu_sel->passes(event)) return false;
    h_1mu->fill(event);
    h_njet_1mu->fill(event);
    h_topjet_1mu->fill(event);
    h_muon_1mu->fill(event);

    if(isTTbar && matching->passes(event)){
      h_1mu_matched->fill(event);
      h_njet_1mu_matched->fill(event);
      h_topjet_1mu_matched->fill(event);
      h_muon_1mu_matched->fill(event);
    }
    else if(isTTbar){
      h_1mu_unmatched->fill(event);
      h_njet_1mu_unmatched->fill(event);
      h_topjet_1mu_unmatched->fill(event);
      h_muon_1mu_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_1mu_genmatched->fill(event);
      h_njet_1mu_genmatched->fill(event);
      h_topjet_1mu_genmatched->fill(event);
      h_muon_1mu_genmatched->fill(event);
    }

    //mu pt > 55GeV
    if(!pt_mu_sel->passes(event)) return false;
    h_mupt55->fill(event);
    h_njet_mupt55->fill(event);
    h_topjet_mupt55->fill(event);
    h_muon_mupt55->fill(event);

    if(isTTbar && matching->passes(event)){
      h_mupt55_matched->fill(event);
      h_njet_mupt55_matched->fill(event);
      h_topjet_mupt55_matched->fill(event);
      h_muon_mupt55_matched->fill(event);
    }
    else if(isTTbar){
      h_mupt55_unmatched->fill(event);
      h_njet_mupt55_unmatched->fill(event);
      h_topjet_mupt55_unmatched->fill(event);
      h_muon_mupt55_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_mupt55_genmatched->fill(event);
      h_njet_mupt55_genmatched->fill(event);
      h_topjet_mupt55_genmatched->fill(event);
      h_muon_mupt55_genmatched->fill(event);
    }

    //0 electron
    if(!nele_sel->passes(event)) return false;
    h_0ele->fill(event);
    h_njet_0ele->fill(event);
    h_topjet_0ele->fill(event);
    h_muon_0ele->fill(event);

    if(isTTbar && matching->passes(event)){
      h_0ele_matched->fill(event);
      h_njet_0ele_matched->fill(event);
      h_topjet_0ele_matched->fill(event);
      h_muon_0ele_matched->fill(event);  
    }
    else if(isTTbar){
      h_0ele_unmatched->fill(event);
      h_njet_0ele_unmatched->fill(event);
      h_topjet_0ele_unmatched->fill(event);
      h_muon_0ele_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_0ele_genmatched->fill(event);
      h_njet_0ele_genmatched->fill(event);
      h_topjet_0ele_genmatched->fill(event);
      h_muon_0ele_genmatched->fill(event);
    }

    //TwoDCut
    if(!twodcut_sel->passes(event)) return false;
    h_twodcut->fill(event);
    h_njet_twodcut->fill(event);
    h_topjet_twodcut->fill(event);
    h_muon_twodcut->fill(event);

    if(isTTbar && matching->passes(event)){
      h_twodcut_matched->fill(event);
      h_njet_twodcut_matched->fill(event);
      h_topjet_twodcut_matched->fill(event);
      h_muon_twodcut_matched->fill(event);
    }
    else if(isTTbar){
      h_twodcut_unmatched->fill(event);
      h_njet_twodcut_unmatched->fill(event);
      h_topjet_twodcut_unmatched->fill(event);
      h_muon_twodcut_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_twodcut_genmatched->fill(event);
      h_njet_twodcut_genmatched->fill(event);
      h_topjet_twodcut_genmatched->fill(event);
      h_muon_twodcut_genmatched->fill(event);
    }

    //1 topjet
    if(!ntopjet_sel->passes(event)) return false;
    h_1topjet->fill(event);
    h_njet_1topjet->fill(event);
    h_topjet_1topjet->fill(event);
    h_muon_1topjet->fill(event);

    if(isTTbar && matching->passes(event)){
      h_1topjet_matched->fill(event);
      h_njet_1topjet_matched->fill(event);
      h_topjet_1topjet_matched->fill(event);
      h_muon_1topjet_matched->fill(event);
    }
    else if(isTTbar){
      h_1topjet_unmatched->fill(event);
      h_njet_1topjet_unmatched->fill(event);
      h_topjet_1topjet_unmatched->fill(event);
      h_muon_1topjet_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_1topjet_genmatched->fill(event);
      h_njet_1topjet_genmatched->fill(event);
      h_topjet_1topjet_genmatched->fill(event);
      h_muon_1topjet_genmatched->fill(event);
    }

    //1 tight b-Jet
    if(!nbtag_tight_sel->passes(event)) return false;
    h_1btagtight->fill(event);
    h_njet_1btagtight->fill(event);
    h_topjet_1btagtight->fill(event);
    h_muon_1btagtight->fill(event);

    if(isTTbar && matching->passes(event)){
      h_1btagtight_matched->fill(event);
      h_njet_1btagtight_matched->fill(event);
      h_topjet_1btagtight_matched->fill(event);
      h_muon_1btagtight_matched->fill(event);
    }
    else if(isTTbar){
      h_1btagtight_unmatched->fill(event);
      h_njet_1btagtight_unmatched->fill(event);
      h_topjet_1btagtight_unmatched->fill(event);
      h_muon_1btagtight_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_1btagtight_genmatched->fill(event);
      h_njet_1btagtight_genmatched->fill(event);
      h_topjet_1btagtight_genmatched->fill(event);
      h_muon_1btagtight_genmatched->fill(event);
    }

    //min 1 topjet_cand
    if(!ntopjetcand_sel1->passes(event)) return false;
    h_ntopjet1->fill(event);
    h_njet_ntopjet1->fill(event);
    h_topjet_ntopjet1->fill(event);
    h_muon_ntopjet1->fill(event);

    if(isTTbar && matching->passes(event)){
      h_ntopjet1_matched->fill(event);
      h_njet_ntopjet1_matched->fill(event);
      h_topjet_ntopjet1_matched->fill(event);
      h_muon_ntopjet1_matched->fill(event);
    }
    else if(isTTbar){
      h_ntopjet1_unmatched->fill(event);
      h_njet_ntopjet1_unmatched->fill(event);
      h_topjet_ntopjet1_unmatched->fill(event);
      h_muon_ntopjet1_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
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

    if(isTTbar && matching->passes(event)){
      h_dphi1_matched->fill(event);
      h_njet_dphi1_matched->fill(event);
      h_topjet_dphi1_matched->fill(event);
      h_muon_dphi1_matched->fill(event);
    }
    else if(isTTbar){
      h_dphi1_unmatched->fill(event);
      h_njet_dphi1_unmatched->fill(event);
      h_topjet_dphi1_unmatched->fill(event);
      h_muon_dphi1_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
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

    if(isTTbar && matching->passes(event)){
      h_dphi25_matched->fill(event);
      h_njet_dphi25_matched->fill(event);
      h_topjet_dphi25_matched->fill(event);
      h_muon_dphi25_matched->fill(event);
    }
    else if(isTTbar){
      h_dphi25_unmatched->fill(event);
      h_njet_dphi25_unmatched->fill(event);
      h_topjet_dphi25_unmatched->fill(event);
      h_muon_dphi25_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
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

    if(isTTbar && matching->passes(event)){
      h_ntopjet2_matched->fill(event);
      h_njet_ntopjet2_matched->fill(event);
      h_topjet_ntopjet2_matched->fill(event);
      h_muon_ntopjet2_matched->fill(event);
    }
    else if(isTTbar){
      h_ntopjet2_unmatched->fill(event);
      h_njet_ntopjet2_unmatched->fill(event);
      h_topjet_ntopjet2_unmatched->fill(event);
      h_muon_ntopjet2_unmatched->fill(event);
    }
    if(isTTbar && genmatching->passes(event)){
      h_ntopjet2_genmatched->fill(event);
      h_njet_ntopjet2_genmatched->fill(event);
      h_topjet_ntopjet2_genmatched->fill(event);
      h_muon_ntopjet2_genmatched->fill(event);
    }


    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureModule)

}
