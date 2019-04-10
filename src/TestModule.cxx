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
  class TestModule: public AnalysisModule {
  public:
    explicit TestModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    bool isMC, isTTbar;
    bool passed_rec, passed_gen, passed_gen_sel, passed_rec_sel;
    bool matched_rec, matched_gen;
    bool passed_rec_mass, passed_gen_mass;

    bool passed_rec_2;

    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> ntopjet2_sel, ntopjet1_sel;
    std::unique_ptr<Selection> mass_sel1, dr_sel;
    std::unique_ptr<Selection> pt_topjet_sel1, pt_topjet_sel2, pt_topjet_sel3, pt_topjet_sel4, pt_topjet_sel5, pt_topjet_sel6, pt_topjet_sel7, pt_topjet_sel8, pt_topjet_sel9, pt_topjet_sel10, pt_topjet_sel11;

    std::unique_ptr<Selection> ntopjet2_gen, dr_gen, mass_gen, genmatching;
    std::unique_ptr<Selection> recmatching;
    std::unique_ptr<Selection> pt_topjet_gen1, pt_topjet_gen2, pt_topjet_gen3, pt_topjet_gen4, pt_topjet_gen5, pt_topjet_gen6, pt_topjet_gen7, pt_topjet_gen8, pt_topjet_gen9, pt_topjet_gen10, pt_topjet_gen11, pt_topjet_gen12, pt_topjet_gen13, pt_topjet_gen14, pt_topjet_gen15, pt_topjet_gen16, pt_topjet_gen17, pt_topjet_gen18, pt_topjet_gen19, pt_topjet_gen20, pt_topjet_gen21, pt_topjet_gen22, pt_topjet_gen23, pt_topjet_gen24, pt_topjet_gen25, pt_topjet_gen26, pt_topjet_gen27, pt_topjet_gen28, pt_topjet_gen29, pt_topjet_gen30, pt_topjet_gen31, pt_topjet_gen32, pt_topjet_gen33, pt_topjet_gen34, pt_topjet_gen35, pt_topjet_gen36, pt_topjet_gen37, pt_topjet_gen38, pt_topjet_gen39, pt_topjet_gen40, pt_topjet_gen41;

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> cleaner;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_gen_pt_topjet1, h_gen_pt_topjet1_matched, h_gen_pt_topjet1_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet2, h_gen_pt_topjet2_matched, h_gen_pt_topjet2_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet3, h_gen_pt_topjet3_matched, h_gen_pt_topjet3_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet4, h_gen_pt_topjet4_matched, h_gen_pt_topjet4_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet5, h_gen_pt_topjet5_matched, h_gen_pt_topjet5_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet6, h_gen_pt_topjet6_matched, h_gen_pt_topjet6_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet7, h_gen_pt_topjet7_matched, h_gen_pt_topjet7_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet8, h_gen_pt_topjet8_matched, h_gen_pt_topjet8_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet9, h_gen_pt_topjet9_matched, h_gen_pt_topjet9_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet10, h_gen_pt_topjet10_matched, h_gen_pt_topjet10_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet11, h_gen_pt_topjet11_matched, h_gen_pt_topjet11_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet12, h_gen_pt_topjet12_matched, h_gen_pt_topjet12_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet13, h_gen_pt_topjet13_matched, h_gen_pt_topjet13_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet14, h_gen_pt_topjet14_matched, h_gen_pt_topjet14_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet15, h_gen_pt_topjet15_matched, h_gen_pt_topjet15_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet16, h_gen_pt_topjet16_matched, h_gen_pt_topjet16_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet17, h_gen_pt_topjet17_matched, h_gen_pt_topjet17_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet18, h_gen_pt_topjet18_matched, h_gen_pt_topjet18_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet19, h_gen_pt_topjet19_matched, h_gen_pt_topjet19_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet20, h_gen_pt_topjet20_matched, h_gen_pt_topjet20_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet21, h_gen_pt_topjet21_matched, h_gen_pt_topjet21_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet22, h_gen_pt_topjet22_matched, h_gen_pt_topjet22_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet23, h_gen_pt_topjet23_matched, h_gen_pt_topjet23_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet24, h_gen_pt_topjet24_matched, h_gen_pt_topjet24_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet25, h_gen_pt_topjet25_matched, h_gen_pt_topjet25_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet26, h_gen_pt_topjet26_matched, h_gen_pt_topjet26_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet27, h_gen_pt_topjet27_matched, h_gen_pt_topjet27_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet28, h_gen_pt_topjet28_matched, h_gen_pt_topjet28_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet29, h_gen_pt_topjet29_matched, h_gen_pt_topjet29_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet30, h_gen_pt_topjet30_matched, h_gen_pt_topjet30_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet31, h_gen_pt_topjet31_matched, h_gen_pt_topjet31_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet32, h_gen_pt_topjet32_matched, h_gen_pt_topjet32_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet33, h_gen_pt_topjet33_matched, h_gen_pt_topjet33_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet34, h_gen_pt_topjet34_matched, h_gen_pt_topjet34_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet35, h_gen_pt_topjet35_matched, h_gen_pt_topjet35_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet36, h_gen_pt_topjet36_matched, h_gen_pt_topjet36_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet37, h_gen_pt_topjet37_matched, h_gen_pt_topjet37_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet38, h_gen_pt_topjet38_matched, h_gen_pt_topjet38_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet39, h_gen_pt_topjet39_matched, h_gen_pt_topjet39_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet40, h_gen_pt_topjet40_matched, h_gen_pt_topjet40_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet41, h_gen_pt_topjet41_matched, h_gen_pt_topjet41_unmatched;
    std::unique_ptr<Hists> h_gen_ntopjet2, h_gen_ntopjet2_matched, h_gen_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_gen_dr, h_gen_dr_matched, h_gen_dr_unmatched;
    std::unique_ptr<Hists> h_gen_mass, h_gen_mass_matched, h_gen_mass_unmatched;

    std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_pt_topjet1, h_pt_topjet1_matched, h_pt_topjet1_unmatched;
    std::unique_ptr<Hists> h_pt_topjet2, h_pt_topjet2_matched, h_pt_topjet2_unmatched;
    std::unique_ptr<Hists> h_pt_topjet3, h_pt_topjet3_matched, h_pt_topjet3_unmatched;
    std::unique_ptr<Hists> h_pt_topjet4, h_pt_topjet4_matched, h_pt_topjet4_unmatched;
    std::unique_ptr<Hists> h_pt_topjet5, h_pt_topjet5_matched, h_pt_topjet5_unmatched;
    std::unique_ptr<Hists> h_pt_topjet6, h_pt_topjet6_matched, h_pt_topjet6_unmatched;
    std::unique_ptr<Hists> h_pt_topjet7, h_pt_topjet7_matched, h_pt_topjet7_unmatched;
    std::unique_ptr<Hists> h_pt_topjet8, h_pt_topjet8_matched, h_pt_topjet8_unmatched;
    std::unique_ptr<Hists> h_pt_topjet9, h_pt_topjet9_matched, h_pt_topjet9_unmatched;
    std::unique_ptr<Hists> h_pt_topjet10, h_pt_topjet10_matched, h_pt_topjet10_unmatched;
    std::unique_ptr<Hists> h_pt_topjet11, h_pt_topjet11_matched, h_pt_topjet11_unmatched;
    std::unique_ptr<Hists> h_ntopjet1, h_ntopjet1_matched, h_ntopjet1_unmatched;
    std::unique_ptr<Hists> h_pt_topjet, h_pt_topjet_matched, h_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_dr, h_dr_matched, h_dr_unmatched;
    std::unique_ptr<Hists> h_mass, h_mass_matched, h_mass_unmatched;
    std::unique_ptr<Hists> h_passedgen_rec, h_passedrec_gen;
    std::unique_ptr<Hists> h_ttbar_hist;

    JetId Btag_tight;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;

    uhh2::Event::Handle<bool> h_passed_rec;
    uhh2::Event::Handle<bool> h_passed_gen;
  };


  TestModule::TestModule(Context & ctx){
    Btag_tight        = CSVBTag(CSVBTag::WP_TIGHT);

    h_passed_rec = ctx.get_handle<bool>("h_passed_rec");
    h_passed_gen = ctx.get_handle<bool>("h_passed_gen");
    h_rec_weight = ctx.get_handle<double>("h_rec_weight");
    h_gen_weight = ctx.get_handle<double>("h_gen_weight");


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
      pt_topjet_gen1.reset(new PtSelection(200, 200));
      pt_topjet_gen2.reset(new PtSelection(220, 200));
      pt_topjet_gen3.reset(new PtSelection(240, 200));
      pt_topjet_gen4.reset(new PtSelection(260, 200));
      pt_topjet_gen5.reset(new PtSelection(280, 200));
      pt_topjet_gen6.reset(new PtSelection(300, 200));
      pt_topjet_gen7.reset(new PtSelection(320, 200));
      pt_topjet_gen8.reset(new PtSelection(340, 200));
      pt_topjet_gen9.reset(new PtSelection(360, 200));
      pt_topjet_gen10.reset(new PtSelection(380, 200));
      pt_topjet_gen11.reset(new PtSelection(400, 200));
      pt_topjet_gen12.reset(new PtSelection(420, 200));
      pt_topjet_gen13.reset(new PtSelection(440, 200));
      pt_topjet_gen14.reset(new PtSelection(460, 200));
      pt_topjet_gen15.reset(new PtSelection(480, 200));
      pt_topjet_gen16.reset(new PtSelection(500, 200));
      pt_topjet_gen17.reset(new PtSelection(520, 200));
      pt_topjet_gen18.reset(new PtSelection(540, 200));
      pt_topjet_gen19.reset(new PtSelection(560, 200));
      pt_topjet_gen20.reset(new PtSelection(580, 200));
      pt_topjet_gen21.reset(new PtSelection(600, 200));
      pt_topjet_gen22.reset(new PtSelection(620, 200));
      pt_topjet_gen23.reset(new PtSelection(640, 200));
      pt_topjet_gen24.reset(new PtSelection(660, 200));
      pt_topjet_gen25.reset(new PtSelection(680, 200));
      pt_topjet_gen26.reset(new PtSelection(700, 200));
      pt_topjet_gen27.reset(new PtSelection(720, 200));
      pt_topjet_gen28.reset(new PtSelection(740, 200));
      pt_topjet_gen29.reset(new PtSelection(760, 200));
      pt_topjet_gen30.reset(new PtSelection(780, 200));
      pt_topjet_gen31.reset(new PtSelection(800, 200));
      pt_topjet_gen32.reset(new PtSelection(820, 200));
      pt_topjet_gen33.reset(new PtSelection(840, 200));
      pt_topjet_gen34.reset(new PtSelection(860, 200));
      pt_topjet_gen35.reset(new PtSelection(880, 200));
      pt_topjet_gen36.reset(new PtSelection(900, 200));
      pt_topjet_gen37.reset(new PtSelection(920, 200));
      pt_topjet_gen38.reset(new PtSelection(940, 200));
      pt_topjet_gen39.reset(new PtSelection(960, 200));
      pt_topjet_gen40.reset(new PtSelection(980, 200));
      pt_topjet_gen41.reset(new PtSelection(1000, 200));

      genmatching.reset(new GenMatching(ctx));
      recmatching.reset(new RecMatching(ctx));
    }


    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    ntopjet1_sel.reset(new NTopJetSelection(1,1));
    mass_sel1.reset(new RecMassSelection(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
    dr_sel.reset(new RecdRSelection());

    pt_topjet_sel1.reset(new RecPtSelection(200, 200));
    pt_topjet_sel2.reset(new RecPtSelection(220, 200));
    pt_topjet_sel3.reset(new RecPtSelection(240, 200));
    pt_topjet_sel4.reset(new RecPtSelection(260, 200));
    pt_topjet_sel5.reset(new RecPtSelection(280, 200));
    pt_topjet_sel6.reset(new RecPtSelection(300, 200));
    pt_topjet_sel7.reset(new RecPtSelection(320, 200));
    pt_topjet_sel8.reset(new RecPtSelection(340, 200));
    pt_topjet_sel9.reset(new RecPtSelection(360, 200));
    pt_topjet_sel10.reset(new RecPtSelection(380, 200));
    pt_topjet_sel11.reset(new RecPtSelection(400, 200));


    // 3. Set up Hists classes:
    h_gen_ntopjet2.reset(new GenHists(ctx, "gen_ntopjet2"));
    h_gen_ntopjet2_matched.reset(new GenHists(ctx, "gen_ntopjet2_matched"));
    h_gen_ntopjet2_unmatched.reset(new GenHists(ctx, "gen_ntopjet2_unmatched"));

    h_gen_dr.reset(new GenHists(ctx, "gen_dr"));
    h_gen_dr_matched.reset(new GenHists(ctx, "gen_dr_matched"));
    h_gen_dr_unmatched.reset(new GenHists(ctx, "gen_dr_unmatched"));

    h_gen_mass.reset(new GenHists(ctx, "gen_mass"));
    h_gen_mass_matched.reset(new GenHists(ctx, "gen_mass_matched"));
    h_gen_mass_unmatched.reset(new GenHists(ctx, "gen_mass_unmatched"));

    h_gen_pt_topjet1.reset(new GenHists(ctx, "gen_pt_topjet1"));
    h_gen_pt_topjet1_matched.reset(new GenHists(ctx, "gen_pt_topjet1_matched"));
    h_gen_pt_topjet1_unmatched.reset(new GenHists(ctx, "gen_pt_topjet1_unmatched"));

    h_gen_pt_topjet2.reset(new GenHists(ctx, "gen_pt_topjet2"));
    h_gen_pt_topjet2_matched.reset(new GenHists(ctx, "gen_pt_topjet2_matched"));
    h_gen_pt_topjet2_unmatched.reset(new GenHists(ctx, "gen_pt_topjet2_unmatched"));

    h_gen_pt_topjet3.reset(new GenHists(ctx, "gen_pt_topjet3"));
    h_gen_pt_topjet3_matched.reset(new GenHists(ctx, "gen_pt_topjet3_matched"));
    h_gen_pt_topjet3_unmatched.reset(new GenHists(ctx, "gen_pt_topjet3_unmatched"));

    h_gen_pt_topjet4.reset(new GenHists(ctx, "gen_pt_topjet4"));
    h_gen_pt_topjet4_matched.reset(new GenHists(ctx, "gen_pt_topjet4_matched"));
    h_gen_pt_topjet4_unmatched.reset(new GenHists(ctx, "gen_pt_topjet4_unmatched"));

    h_gen_pt_topjet5.reset(new GenHists(ctx, "gen_pt_topjet5"));
    h_gen_pt_topjet5_matched.reset(new GenHists(ctx, "gen_pt_topjet5_matched"));
    h_gen_pt_topjet5_unmatched.reset(new GenHists(ctx, "gen_pt_topjet5_unmatched"));

    h_gen_pt_topjet6.reset(new GenHists(ctx, "gen_pt_topjet6"));
    h_gen_pt_topjet6_matched.reset(new GenHists(ctx, "gen_pt_topjet6_matched"));
    h_gen_pt_topjet6_unmatched.reset(new GenHists(ctx, "gen_pt_topjet6_unmatched"));

    h_gen_pt_topjet7.reset(new GenHists(ctx, "gen_pt_topjet7"));
    h_gen_pt_topjet7_matched.reset(new GenHists(ctx, "gen_pt_topjet7_matched"));
    h_gen_pt_topjet7_unmatched.reset(new GenHists(ctx, "gen_pt_topjet7_unmatched"));

    h_gen_pt_topjet8.reset(new GenHists(ctx, "gen_pt_topjet8"));
    h_gen_pt_topjet8_matched.reset(new GenHists(ctx, "gen_pt_topjet8_matched"));
    h_gen_pt_topjet8_unmatched.reset(new GenHists(ctx, "gen_pt_topjet8_unmatched"));

    h_gen_pt_topjet9.reset(new GenHists(ctx, "gen_pt_topjet9"));
    h_gen_pt_topjet9_matched.reset(new GenHists(ctx, "gen_pt_topjet9_matched"));
    h_gen_pt_topjet9_unmatched.reset(new GenHists(ctx, "gen_pt_topjet9_unmatched"));

    h_gen_pt_topjet10.reset(new GenHists(ctx, "gen_pt_topjet10"));
    h_gen_pt_topjet10_matched.reset(new GenHists(ctx, "gen_pt_topjet10_matched"));
    h_gen_pt_topjet10_unmatched.reset(new GenHists(ctx, "gen_pt_topjet10_unmatched"));

    h_gen_pt_topjet11.reset(new GenHists(ctx, "gen_pt_topjet11"));
    h_gen_pt_topjet11_matched.reset(new GenHists(ctx, "gen_pt_topjet11_matched"));
    h_gen_pt_topjet11_unmatched.reset(new GenHists(ctx, "gen_pt_topjet11_unmatched"));

    h_gen_pt_topjet12.reset(new GenHists(ctx, "gen_pt_topjet12"));
    h_gen_pt_topjet12_matched.reset(new GenHists(ctx, "gen_pt_topjet12_matched"));
    h_gen_pt_topjet12_unmatched.reset(new GenHists(ctx, "gen_pt_topjet12_unmatched"));

    h_gen_pt_topjet13.reset(new GenHists(ctx, "gen_pt_topjet13"));
    h_gen_pt_topjet13_matched.reset(new GenHists(ctx, "gen_pt_topjet13_matched"));
    h_gen_pt_topjet13_unmatched.reset(new GenHists(ctx, "gen_pt_topjet13_unmatched"));

    h_gen_pt_topjet14.reset(new GenHists(ctx, "gen_pt_topjet14"));
    h_gen_pt_topjet14_matched.reset(new GenHists(ctx, "gen_pt_topjet14_matched"));
    h_gen_pt_topjet14_unmatched.reset(new GenHists(ctx, "gen_pt_topjet14_unmatched"));

    h_gen_pt_topjet15.reset(new GenHists(ctx, "gen_pt_topjet15"));
    h_gen_pt_topjet15_matched.reset(new GenHists(ctx, "gen_pt_topjet15_matched"));
    h_gen_pt_topjet15_unmatched.reset(new GenHists(ctx, "gen_pt_topjet15_unmatched"));

    h_gen_pt_topjet16.reset(new GenHists(ctx, "gen_pt_topjet16"));
    h_gen_pt_topjet16_matched.reset(new GenHists(ctx, "gen_pt_topjet16_matched"));
    h_gen_pt_topjet16_unmatched.reset(new GenHists(ctx, "gen_pt_topjet16_unmatched"));

    h_gen_pt_topjet17.reset(new GenHists(ctx, "gen_pt_topjet17"));
    h_gen_pt_topjet17_matched.reset(new GenHists(ctx, "gen_pt_topjet17_matched"));
    h_gen_pt_topjet17_unmatched.reset(new GenHists(ctx, "gen_pt_topjet17_unmatched"));

    h_gen_pt_topjet18.reset(new GenHists(ctx, "gen_pt_topjet18"));
    h_gen_pt_topjet18_matched.reset(new GenHists(ctx, "gen_pt_topjet18_matched"));
    h_gen_pt_topjet18_unmatched.reset(new GenHists(ctx, "gen_pt_topjet18_unmatched"));

    h_gen_pt_topjet19.reset(new GenHists(ctx, "gen_pt_topjet19"));
    h_gen_pt_topjet19_matched.reset(new GenHists(ctx, "gen_pt_topjet19_matched"));
    h_gen_pt_topjet19_unmatched.reset(new GenHists(ctx, "gen_pt_topjet19_unmatched"));

    h_gen_pt_topjet20.reset(new GenHists(ctx, "gen_pt_topjet20"));
    h_gen_pt_topjet20_matched.reset(new GenHists(ctx, "gen_pt_topjet20_matched"));
    h_gen_pt_topjet20_unmatched.reset(new GenHists(ctx, "gen_pt_topjet20_unmatched"));

    h_gen_pt_topjet21.reset(new GenHists(ctx, "gen_pt_topjet21"));
    h_gen_pt_topjet21_matched.reset(new GenHists(ctx, "gen_pt_topjet21_matched"));
    h_gen_pt_topjet21_unmatched.reset(new GenHists(ctx, "gen_pt_topjet21_unmatched"));

    h_gen_pt_topjet22.reset(new GenHists(ctx, "gen_pt_topjet22"));
    h_gen_pt_topjet22_matched.reset(new GenHists(ctx, "gen_pt_topjet22_matched"));
    h_gen_pt_topjet22_unmatched.reset(new GenHists(ctx, "gen_pt_topjet22_unmatched"));

    h_gen_pt_topjet23.reset(new GenHists(ctx, "gen_pt_topjet23"));
    h_gen_pt_topjet23_matched.reset(new GenHists(ctx, "gen_pt_topjet23_matched"));
    h_gen_pt_topjet23_unmatched.reset(new GenHists(ctx, "gen_pt_topjet23_unmatched"));

    h_gen_pt_topjet24.reset(new GenHists(ctx, "gen_pt_topjet24"));
    h_gen_pt_topjet24_matched.reset(new GenHists(ctx, "gen_pt_topjet24_matched"));
    h_gen_pt_topjet24_unmatched.reset(new GenHists(ctx, "gen_pt_topjet24_unmatched"));

    h_gen_pt_topjet25.reset(new GenHists(ctx, "gen_pt_topjet25"));
    h_gen_pt_topjet25_matched.reset(new GenHists(ctx, "gen_pt_topjet25_matched"));
    h_gen_pt_topjet25_unmatched.reset(new GenHists(ctx, "gen_pt_topjet25_unmatched"));

    h_gen_pt_topjet26.reset(new GenHists(ctx, "gen_pt_topjet26"));
    h_gen_pt_topjet26_matched.reset(new GenHists(ctx, "gen_pt_topjet26_matched"));
    h_gen_pt_topjet26_unmatched.reset(new GenHists(ctx, "gen_pt_topjet26_unmatched"));

    h_gen_pt_topjet27.reset(new GenHists(ctx, "gen_pt_topjet27"));
    h_gen_pt_topjet27_matched.reset(new GenHists(ctx, "gen_pt_topjet27_matched"));
    h_gen_pt_topjet27_unmatched.reset(new GenHists(ctx, "gen_pt_topjet27_unmatched"));

    h_gen_pt_topjet28.reset(new GenHists(ctx, "gen_pt_topjet28"));
    h_gen_pt_topjet28_matched.reset(new GenHists(ctx, "gen_pt_topjet28_matched"));
    h_gen_pt_topjet28_unmatched.reset(new GenHists(ctx, "gen_pt_topjet28_unmatched"));

    h_gen_pt_topjet29.reset(new GenHists(ctx, "gen_pt_topjet29"));
    h_gen_pt_topjet29_matched.reset(new GenHists(ctx, "gen_pt_topjet29_matched"));
    h_gen_pt_topjet29_unmatched.reset(new GenHists(ctx, "gen_pt_topjet29_unmatched"));

    h_gen_pt_topjet30.reset(new GenHists(ctx, "gen_pt_topjet30"));
    h_gen_pt_topjet30_matched.reset(new GenHists(ctx, "gen_pt_topjet30_matched"));
    h_gen_pt_topjet30_unmatched.reset(new GenHists(ctx, "gen_pt_topjet30_unmatched"));

    h_gen_pt_topjet31.reset(new GenHists(ctx, "gen_pt_topjet31"));
    h_gen_pt_topjet31_matched.reset(new GenHists(ctx, "gen_pt_topjet31_matched"));
    h_gen_pt_topjet31_unmatched.reset(new GenHists(ctx, "gen_pt_topjet31_unmatched"));

    h_gen_pt_topjet32.reset(new GenHists(ctx, "gen_pt_topjet32"));
    h_gen_pt_topjet32_matched.reset(new GenHists(ctx, "gen_pt_topjet32_matched"));
    h_gen_pt_topjet32_unmatched.reset(new GenHists(ctx, "gen_pt_topjet32_unmatched"));

    h_gen_pt_topjet33.reset(new GenHists(ctx, "gen_pt_topjet33"));
    h_gen_pt_topjet33_matched.reset(new GenHists(ctx, "gen_pt_topjet33_matched"));
    h_gen_pt_topjet33_unmatched.reset(new GenHists(ctx, "gen_pt_topjet33_unmatched"));

    h_gen_pt_topjet34.reset(new GenHists(ctx, "gen_pt_topjet34"));
    h_gen_pt_topjet34_matched.reset(new GenHists(ctx, "gen_pt_topjet34_matched"));
    h_gen_pt_topjet34_unmatched.reset(new GenHists(ctx, "gen_pt_topjet34_unmatched"));

    h_gen_pt_topjet35.reset(new GenHists(ctx, "gen_pt_topjet35"));
    h_gen_pt_topjet35_matched.reset(new GenHists(ctx, "gen_pt_topjet35_matched"));
    h_gen_pt_topjet35_unmatched.reset(new GenHists(ctx, "gen_pt_topjet35_unmatched"));

    h_gen_pt_topjet36.reset(new GenHists(ctx, "gen_pt_topjet36"));
    h_gen_pt_topjet36_matched.reset(new GenHists(ctx, "gen_pt_topjet36_matched"));
    h_gen_pt_topjet36_unmatched.reset(new GenHists(ctx, "gen_pt_topjet36_unmatched"));

    h_gen_pt_topjet37.reset(new GenHists(ctx, "gen_pt_topjet37"));
    h_gen_pt_topjet37_matched.reset(new GenHists(ctx, "gen_pt_topjet37_matched"));
    h_gen_pt_topjet37_unmatched.reset(new GenHists(ctx, "gen_pt_topjet37_unmatched"));

    h_gen_pt_topjet38.reset(new GenHists(ctx, "gen_pt_topjet38"));
    h_gen_pt_topjet38_matched.reset(new GenHists(ctx, "gen_pt_topjet38_matched"));
    h_gen_pt_topjet38_unmatched.reset(new GenHists(ctx, "gen_pt_topjet38_unmatched"));

    h_gen_pt_topjet39.reset(new GenHists(ctx, "gen_pt_topjet39"));
    h_gen_pt_topjet39_matched.reset(new GenHists(ctx, "gen_pt_topjet39_matched"));
    h_gen_pt_topjet39_unmatched.reset(new GenHists(ctx, "gen_pt_topjet39_unmatched"));

    h_gen_pt_topjet40.reset(new GenHists(ctx, "gen_pt_topjet40"));
    h_gen_pt_topjet40_matched.reset(new GenHists(ctx, "gen_pt_topjet40_matched"));
    h_gen_pt_topjet40_unmatched.reset(new GenHists(ctx, "gen_pt_topjet40_unmatched"));

    h_gen_pt_topjet41.reset(new GenHists(ctx, "gen_pt_topjet41"));
    h_gen_pt_topjet41_matched.reset(new GenHists(ctx, "gen_pt_topjet41_matched"));
    h_gen_pt_topjet41_unmatched.reset(new GenHists(ctx, "gen_pt_topjet41_unmatched"));

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

    h_pt_topjet1.reset(new TopSubstructureRecoHists(ctx, "pt_topjet1"));
    h_pt_topjet1_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet1_matched"));
    h_pt_topjet1_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet1_unmatched"));

    h_pt_topjet2.reset(new TopSubstructureRecoHists(ctx, "pt_topjet2"));
    h_pt_topjet2_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet2_matched"));
    h_pt_topjet2_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet2_unmatched"));

    h_pt_topjet3.reset(new TopSubstructureRecoHists(ctx, "pt_topjet3"));
    h_pt_topjet3_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet3_matched"));
    h_pt_topjet3_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet3_unmatched"));

    h_pt_topjet4.reset(new TopSubstructureRecoHists(ctx, "pt_topjet4"));
    h_pt_topjet4_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet4_matched"));
    h_pt_topjet4_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet4_unmatched"));

    h_pt_topjet5.reset(new TopSubstructureRecoHists(ctx, "pt_topjet5"));
    h_pt_topjet5_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet5_matched"));
    h_pt_topjet5_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet5_unmatched"));

    h_pt_topjet6.reset(new TopSubstructureRecoHists(ctx, "pt_topjet6"));
    h_pt_topjet6_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet6_matched"));
    h_pt_topjet6_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet6_unmatched"));

    h_pt_topjet7.reset(new TopSubstructureRecoHists(ctx, "pt_topjet7"));
    h_pt_topjet7_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet7_matched"));
    h_pt_topjet7_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet7_unmatched"));

    h_pt_topjet8.reset(new TopSubstructureRecoHists(ctx, "pt_topjet8"));
    h_pt_topjet8_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet8_matched"));
    h_pt_topjet8_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet8_unmatched"));

    h_pt_topjet9.reset(new TopSubstructureRecoHists(ctx, "pt_topjet9"));
    h_pt_topjet9_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet9_matched"));
    h_pt_topjet9_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet9_unmatched"));

    h_pt_topjet10.reset(new TopSubstructureRecoHists(ctx, "pt_topjet10"));
    h_pt_topjet10_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet10_matched"));
    h_pt_topjet10_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet10_unmatched"));

    h_pt_topjet11.reset(new TopSubstructureRecoHists(ctx, "pt_topjet11"));
    h_pt_topjet11_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet11_matched"));
    h_pt_topjet11_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet11_unmatched"));

    h_passedgen_rec.reset(new TopSubstructureRecoHists(ctx, "passedgen_rec"));
    h_passedrec_gen.reset(new GenHists(ctx, "passedrec_gen"));

    h_ttbar_hist.reset(new TTbarGenHists(ctx, "ttbar_hist"));
  }


  bool TestModule::process(Event & event) {
    cout << "TestModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
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

            passed_gen = pt_topjet_gen1->passes(event);
            if(passed_gen){
              h_gen_pt_topjet1->fill(event);
              matched_gen = genmatching->passes(event);
              if(matched_gen) h_gen_pt_topjet1_matched->fill(event);
              else h_gen_pt_topjet1_unmatched->fill(event);

              passed_gen = pt_topjet_gen2->passes(event);
              if(passed_gen){
                h_gen_pt_topjet2->fill(event);
                matched_gen = genmatching->passes(event);
                if(matched_gen) h_gen_pt_topjet2_matched->fill(event);
                else h_gen_pt_topjet2_unmatched->fill(event);

                passed_gen = pt_topjet_gen3->passes(event);
                if(passed_gen){
                  h_gen_pt_topjet3->fill(event);
                  matched_gen = genmatching->passes(event);
                  if(matched_gen) h_gen_pt_topjet3_matched->fill(event);
                  else h_gen_pt_topjet3_unmatched->fill(event);

                  passed_gen = pt_topjet_gen4->passes(event);
                  if(passed_gen){
                    h_gen_pt_topjet4->fill(event);
                    matched_gen = genmatching->passes(event);
                    if(matched_gen) h_gen_pt_topjet4_matched->fill(event);
                    else h_gen_pt_topjet4_unmatched->fill(event);

                    passed_gen = pt_topjet_gen5->passes(event);
                    if(passed_gen){
                      h_gen_pt_topjet5->fill(event);
                      matched_gen = genmatching->passes(event);
                      if(matched_gen) h_gen_pt_topjet5_matched->fill(event);
                      else h_gen_pt_topjet5_unmatched->fill(event);

                      passed_gen = pt_topjet_gen6->passes(event);
                      if(passed_gen){
                        h_gen_pt_topjet6->fill(event);
                        matched_gen = genmatching->passes(event);
                        if(matched_gen) h_gen_pt_topjet6_matched->fill(event);
                        else h_gen_pt_topjet6_unmatched->fill(event);

                        passed_gen = pt_topjet_gen7->passes(event);
                        if(passed_gen){
                          h_gen_pt_topjet7->fill(event);
                          matched_gen = genmatching->passes(event);
                          if(matched_gen) h_gen_pt_topjet7_matched->fill(event);
                          else h_gen_pt_topjet7_unmatched->fill(event);

                          passed_gen = pt_topjet_gen8->passes(event);
                          if(passed_gen){
                            h_gen_pt_topjet8->fill(event);
                            matched_gen = genmatching->passes(event);
                            if(matched_gen) h_gen_pt_topjet8_matched->fill(event);
                            else h_gen_pt_topjet8_unmatched->fill(event);

                            passed_gen = pt_topjet_gen9->passes(event);
                            if(passed_gen){
                              h_gen_pt_topjet9->fill(event);
                              matched_gen = genmatching->passes(event);
                              if(matched_gen) h_gen_pt_topjet9_matched->fill(event);
                              else h_gen_pt_topjet9_unmatched->fill(event);

                              passed_gen = pt_topjet_gen10->passes(event);
                              if(passed_gen){
                                h_gen_pt_topjet10->fill(event);
                                matched_gen = genmatching->passes(event);
                                if(matched_gen) h_gen_pt_topjet10_matched->fill(event);
                                else h_gen_pt_topjet10_unmatched->fill(event);

                                passed_gen = pt_topjet_gen11->passes(event);
                                if(passed_gen){
                                  h_gen_pt_topjet11->fill(event);
                                  matched_gen = genmatching->passes(event);
                                  if(matched_gen) h_gen_pt_topjet11_matched->fill(event);
                                  else h_gen_pt_topjet11_unmatched->fill(event);

                                  passed_gen = pt_topjet_gen12->passes(event);
                                  if(passed_gen){
                                    h_gen_pt_topjet12->fill(event);
                                    matched_gen = genmatching->passes(event);
                                    if(matched_gen) h_gen_pt_topjet12_matched->fill(event);
                                    else h_gen_pt_topjet12_unmatched->fill(event);

                                    passed_gen = pt_topjet_gen13->passes(event);
                                    if(passed_gen){
                                      h_gen_pt_topjet13->fill(event);
                                      matched_gen = genmatching->passes(event);
                                      if(matched_gen) h_gen_pt_topjet13_matched->fill(event);
                                      else h_gen_pt_topjet13_unmatched->fill(event);

                                      passed_gen = pt_topjet_gen14->passes(event);
                                      if(passed_gen){
                                        h_gen_pt_topjet14->fill(event);
                                        matched_gen = genmatching->passes(event);
                                        if(matched_gen) h_gen_pt_topjet14_matched->fill(event);
                                        else h_gen_pt_topjet14_unmatched->fill(event);

                                        passed_gen = pt_topjet_gen15->passes(event);
                                        if(passed_gen){
                                          h_gen_pt_topjet15->fill(event);
                                          matched_gen = genmatching->passes(event);
                                          if(matched_gen) h_gen_pt_topjet15_matched->fill(event);
                                          else h_gen_pt_topjet15_unmatched->fill(event);

                                          passed_gen = pt_topjet_gen16->passes(event);
                                          if(passed_gen){
                                            h_gen_pt_topjet16->fill(event);
                                            matched_gen = genmatching->passes(event);
                                            if(matched_gen) h_gen_pt_topjet16_matched->fill(event);
                                            else h_gen_pt_topjet16_unmatched->fill(event);

                                            passed_gen = pt_topjet_gen17->passes(event);
                                            if(passed_gen){
                                              h_gen_pt_topjet17->fill(event);
                                              matched_gen = genmatching->passes(event);
                                              if(matched_gen) h_gen_pt_topjet17_matched->fill(event);
                                              else h_gen_pt_topjet17_unmatched->fill(event);

                                              passed_gen = pt_topjet_gen18->passes(event);
                                              if(passed_gen){
                                                h_gen_pt_topjet18->fill(event);
                                                matched_gen = genmatching->passes(event);
                                                if(matched_gen) h_gen_pt_topjet18_matched->fill(event);
                                                else h_gen_pt_topjet18_unmatched->fill(event);

                                                passed_gen = pt_topjet_gen19->passes(event);
                                                if(passed_gen){
                                                  h_gen_pt_topjet19->fill(event);
                                                  matched_gen = genmatching->passes(event);
                                                  if(matched_gen) h_gen_pt_topjet19_matched->fill(event);
                                                  else h_gen_pt_topjet19_unmatched->fill(event);

                                                  passed_gen = pt_topjet_gen20->passes(event);
                                                  if(passed_gen){
                                                    h_gen_pt_topjet20->fill(event);
                                                    matched_gen = genmatching->passes(event);
                                                    if(matched_gen) h_gen_pt_topjet20_matched->fill(event);
                                                    else h_gen_pt_topjet20_unmatched->fill(event);

                                                    passed_gen = pt_topjet_gen21->passes(event);
                                                    if(passed_gen){
                                                      h_gen_pt_topjet21->fill(event);
                                                      matched_gen = genmatching->passes(event);
                                                      if(matched_gen) h_gen_pt_topjet21_matched->fill(event);
                                                      else h_gen_pt_topjet21_unmatched->fill(event);

                                                      passed_gen = pt_topjet_gen22->passes(event);
                                                      if(passed_gen){
                                                        h_gen_pt_topjet22->fill(event);
                                                        matched_gen = genmatching->passes(event);
                                                        if(matched_gen) h_gen_pt_topjet22_matched->fill(event);
                                                        else h_gen_pt_topjet22_unmatched->fill(event);

                                                        passed_gen = pt_topjet_gen23->passes(event);
                                                        if(passed_gen){
                                                          h_gen_pt_topjet23->fill(event);
                                                          matched_gen = genmatching->passes(event);
                                                          if(matched_gen) h_gen_pt_topjet23_matched->fill(event);
                                                          else h_gen_pt_topjet23_unmatched->fill(event);

                                                          passed_gen = pt_topjet_gen24->passes(event);
                                                          if(passed_gen){
                                                            h_gen_pt_topjet24->fill(event);
                                                            matched_gen = genmatching->passes(event);
                                                            if(matched_gen) h_gen_pt_topjet24_matched->fill(event);
                                                            else h_gen_pt_topjet24_unmatched->fill(event);

                                                            passed_gen = pt_topjet_gen25->passes(event);
                                                            if(passed_gen){
                                                              h_gen_pt_topjet25->fill(event);
                                                              matched_gen = genmatching->passes(event);
                                                              if(matched_gen) h_gen_pt_topjet25_matched->fill(event);
                                                              else h_gen_pt_topjet25_unmatched->fill(event);

                                                              passed_gen = pt_topjet_gen26->passes(event);
                                                              if(passed_gen){
                                                                h_gen_pt_topjet26->fill(event);
                                                                matched_gen = genmatching->passes(event);
                                                                if(matched_gen) h_gen_pt_topjet26_matched->fill(event);
                                                                else h_gen_pt_topjet26_unmatched->fill(event);

                                                                passed_gen = pt_topjet_gen27->passes(event);
                                                                if(passed_gen){
                                                                  h_gen_pt_topjet27->fill(event);
                                                                  matched_gen = genmatching->passes(event);
                                                                  if(matched_gen) h_gen_pt_topjet27_matched->fill(event);
                                                                  else h_gen_pt_topjet27_unmatched->fill(event);

                                                                  passed_gen = pt_topjet_gen28->passes(event);
                                                                  if(passed_gen){
                                                                    h_gen_pt_topjet28->fill(event);
                                                                    matched_gen = genmatching->passes(event);
                                                                    if(matched_gen) h_gen_pt_topjet28_matched->fill(event);
                                                                    else h_gen_pt_topjet28_unmatched->fill(event);

                                                                    passed_gen = pt_topjet_gen29->passes(event);
                                                                    if(passed_gen){
                                                                      h_gen_pt_topjet29->fill(event);
                                                                      matched_gen = genmatching->passes(event);
                                                                      if(matched_gen) h_gen_pt_topjet29_matched->fill(event);
                                                                      else h_gen_pt_topjet29_unmatched->fill(event);

                                                                      passed_gen = pt_topjet_gen30->passes(event);
                                                                      if(passed_gen){
                                                                        h_gen_pt_topjet30->fill(event);
                                                                        matched_gen = genmatching->passes(event);
                                                                        if(matched_gen) h_gen_pt_topjet30_matched->fill(event);
                                                                        else h_gen_pt_topjet30_unmatched->fill(event);

                                                                        passed_gen = pt_topjet_gen31->passes(event);
                                                                        if(passed_gen){
                                                                          h_gen_pt_topjet31->fill(event);
                                                                          matched_gen = genmatching->passes(event);
                                                                          if(matched_gen) h_gen_pt_topjet31_matched->fill(event);
                                                                          else h_gen_pt_topjet31_unmatched->fill(event);

                                                                          passed_gen = pt_topjet_gen32->passes(event);
                                                                          if(passed_gen){
                                                                            h_gen_pt_topjet32->fill(event);
                                                                            matched_gen = genmatching->passes(event);
                                                                            if(matched_gen) h_gen_pt_topjet32_matched->fill(event);
                                                                            else h_gen_pt_topjet32_unmatched->fill(event);

                                                                            passed_gen = pt_topjet_gen33->passes(event);
                                                                            if(passed_gen){
                                                                              h_gen_pt_topjet33->fill(event);
                                                                              matched_gen = genmatching->passes(event);
                                                                              if(matched_gen) h_gen_pt_topjet33_matched->fill(event);
                                                                              else h_gen_pt_topjet33_unmatched->fill(event);

                                                                              passed_gen = pt_topjet_gen34->passes(event);
                                                                              if(passed_gen){
                                                                                h_gen_pt_topjet34->fill(event);
                                                                                matched_gen = genmatching->passes(event);
                                                                                if(matched_gen) h_gen_pt_topjet34_matched->fill(event);
                                                                                else h_gen_pt_topjet34_unmatched->fill(event);

                                                                                passed_gen = pt_topjet_gen35->passes(event);
                                                                                if(passed_gen){
                                                                                  h_gen_pt_topjet35->fill(event);
                                                                                  matched_gen = genmatching->passes(event);
                                                                                  if(matched_gen) h_gen_pt_topjet35_matched->fill(event);
                                                                                  else h_gen_pt_topjet35_unmatched->fill(event);

                                                                                  passed_gen = pt_topjet_gen36->passes(event);
                                                                                  if(passed_gen){
                                                                                    h_gen_pt_topjet36->fill(event);
                                                                                    matched_gen = genmatching->passes(event);
                                                                                    if(matched_gen) h_gen_pt_topjet36_matched->fill(event);
                                                                                    else h_gen_pt_topjet36_unmatched->fill(event);

                                                                                    passed_gen = pt_topjet_gen37->passes(event);
                                                                                    if(passed_gen){
                                                                                      h_gen_pt_topjet37->fill(event);
                                                                                      matched_gen = genmatching->passes(event);
                                                                                      if(matched_gen) h_gen_pt_topjet37_matched->fill(event);
                                                                                      else h_gen_pt_topjet37_unmatched->fill(event);

                                                                                      passed_gen = pt_topjet_gen38->passes(event);
                                                                                      if(passed_gen){
                                                                                        h_gen_pt_topjet38->fill(event);
                                                                                        matched_gen = genmatching->passes(event);
                                                                                        if(matched_gen) h_gen_pt_topjet38_matched->fill(event);
                                                                                        else h_gen_pt_topjet38_unmatched->fill(event);

                                                                                        passed_gen = pt_topjet_gen39->passes(event);
                                                                                        if(passed_gen){
                                                                                          h_gen_pt_topjet39->fill(event);
                                                                                          matched_gen = genmatching->passes(event);
                                                                                          if(matched_gen) h_gen_pt_topjet39_matched->fill(event);
                                                                                          else h_gen_pt_topjet39_unmatched->fill(event);

                                                                                          passed_gen = pt_topjet_gen40->passes(event);
                                                                                          if(passed_gen){
                                                                                            h_gen_pt_topjet40->fill(event);
                                                                                            matched_gen = genmatching->passes(event);
                                                                                            if(matched_gen) h_gen_pt_topjet40_matched->fill(event);
                                                                                            else h_gen_pt_topjet40_unmatched->fill(event);

                                                                                            passed_gen = pt_topjet_gen41->passes(event);
                                                                                            if(passed_gen){
                                                                                              h_gen_pt_topjet41->fill(event);
                                                                                              matched_gen = genmatching->passes(event);
                                                                                              if(matched_gen) h_gen_pt_topjet41_matched->fill(event);
                                                                                              else h_gen_pt_topjet41_unmatched->fill(event);
                                                                                            }
                                                                                          }
                                                                                        }
                                                                                      }
                                                                                    }
                                                                                  }
                                                                                }
                                                                              }
                                                                            }
                                                                          }
                                                                        }
                                                                      }
                                                                    }
                                                                  }
                                                                }
                                                              }
                                                            }
                                                          }
                                                        }
                                                      }
                                                    }
                                                  }
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }


    if(passed_rec){
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

            passed_rec = pt_topjet_sel1->passes(event);
            if(passed_rec){
              h_pt_topjet1->fill(event);
              if(isTTbar){
                matched_rec = recmatching->passes(event);
                if(matched_rec) h_pt_topjet1_matched->fill(event);
                else h_pt_topjet1_unmatched->fill(event);
              }

              passed_rec = pt_topjet_sel2->passes(event);
              if(passed_rec){
                h_pt_topjet2->fill(event);
                if(isTTbar){
                  matched_rec = recmatching->passes(event);
                  if(matched_rec) h_pt_topjet2_matched->fill(event);
                  else h_pt_topjet2_unmatched->fill(event);
                }

                passed_rec = pt_topjet_sel3->passes(event);
                if(passed_rec){
                  h_pt_topjet3->fill(event);
                  if(isTTbar){
                    matched_rec = recmatching->passes(event);
                    if(matched_rec) h_pt_topjet3_matched->fill(event);
                    else h_pt_topjet3_unmatched->fill(event);
                  }

                  passed_rec = pt_topjet_sel4->passes(event);
                  if(passed_rec){
                    h_pt_topjet4->fill(event);
                    if(isTTbar){
                      matched_rec = recmatching->passes(event);
                      if(matched_rec) h_pt_topjet4_matched->fill(event);
                      else h_pt_topjet4_unmatched->fill(event);
                    }

                    passed_rec = pt_topjet_sel5->passes(event);
                    if(passed_rec){
                      h_pt_topjet5->fill(event);
                      if(isTTbar){
                        matched_rec = recmatching->passes(event);
                        if(matched_rec) h_pt_topjet5_matched->fill(event);
                        else h_pt_topjet5_unmatched->fill(event);
                      }

                      passed_rec = pt_topjet_sel6->passes(event);
                      if(passed_rec){
                        h_pt_topjet6->fill(event);
                        if(isTTbar){
                          matched_rec = recmatching->passes(event);
                          if(matched_rec) h_pt_topjet6_matched->fill(event);
                          else h_pt_topjet6_unmatched->fill(event);
                        }

                        passed_rec = pt_topjet_sel7->passes(event);
                        if(passed_rec){
                          h_pt_topjet7->fill(event);
                          if(isTTbar){
                            matched_rec = recmatching->passes(event);
                            if(matched_rec) h_pt_topjet7_matched->fill(event);
                            else h_pt_topjet7_unmatched->fill(event);
                          }

                          passed_rec = pt_topjet_sel8->passes(event);
                          if(passed_rec){
                            h_pt_topjet8->fill(event);
                            if(isTTbar){
                              matched_rec = recmatching->passes(event);
                              if(matched_rec) h_pt_topjet8_matched->fill(event);
                              else h_pt_topjet8_unmatched->fill(event);
                            }

                            passed_rec = pt_topjet_sel9->passes(event);
                            if(passed_rec){
                              h_pt_topjet9->fill(event);
                              if(isTTbar){
                                matched_rec = recmatching->passes(event);
                                if(matched_rec) h_pt_topjet9_matched->fill(event);
                                else h_pt_topjet9_unmatched->fill(event);
                              }

                              passed_rec = pt_topjet_sel10->passes(event);
                              if(passed_rec){
                                h_pt_topjet10->fill(event);
                                if(isTTbar){
                                  matched_rec = recmatching->passes(event);
                                  if(matched_rec) h_pt_topjet10_matched->fill(event);
                                  else h_pt_topjet10_unmatched->fill(event);
                                }

                                passed_rec = pt_topjet_sel11->passes(event);
                                if(passed_rec){
                                  h_pt_topjet11->fill(event);
                                  if(isTTbar){
                                    matched_rec = recmatching->passes(event);
                                    if(matched_rec) h_pt_topjet11_matched->fill(event);
                                    else h_pt_topjet11_unmatched->fill(event);
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    if((!passed_rec && !passed_gen)) return false;

    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(TestModule)

}
