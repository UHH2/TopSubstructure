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
  class PostKinCutModule: public AnalysisModule {
  public:
    explicit PostKinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:

    bool isMC, isTTbar;
    bool passed_rec;
    bool matched_rec;

    bool passed_rec_2, passed_rec_3;
    bool matched_2, matched_rec_2;

    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    std::unique_ptr<Selection> ntopjet2_sel, ntopjet1_sel, nbtag_medium_sel, nbtag_tight_sel;
    std::unique_ptr<Selection> pt_topjet_sel1;
    std::unique_ptr<Selection> mass_sel0, mass_sel1, dr_sel;
    std::unique_ptr<Selection> twodcut_sel1, twodcut_sel2, twodcut_sel3, twodcut_sel4, twodcut_sel5, twodcut_sel6, twodcut_sel7, twodcut_sel8, twodcut_sel9, twodcut_sel10, twodcut_sel11, twodcut_sel12, twodcut_sel13, twodcut_sel14, twodcut_sel15, twodcut_sel16;

    std::unique_ptr<Selection> recmatching;

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> cleaner;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_ntopjet1, h_ntopjet1_matched, h_ntopjet1_unmatched;
    std::unique_ptr<Hists> h_pt_topjet, h_pt_topjet_matched, h_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_dr, h_dr_matched, h_dr_unmatched;
    std::unique_ptr<Hists> h_mass0, h_mass0_matched, h_mass0_unmatched;
    std::unique_ptr<Hists> h_mass1, h_mass1_matched, h_mass1_unmatched;

    std::unique_ptr<Hists> h_twodcut1, h_twodcut1_matched, h_twodcut1_unmatched;
    std::unique_ptr<Hists> h_nbtagm1, h_nbtagm1_matched, h_nbtagm1_unmatched;
    std::unique_ptr<Hists> h_nbtagt1, h_nbtagt1_matched, h_nbtagt1_unmatched;
    std::unique_ptr<Hists> h_twodcut2, h_twodcut2_matched, h_twodcut2_unmatched;
    std::unique_ptr<Hists> h_nbtagm2, h_nbtagm2_matched, h_nbtagm2_unmatched;
    std::unique_ptr<Hists> h_nbtagt2, h_nbtagt2_matched, h_nbtagt2_unmatched;
    std::unique_ptr<Hists> h_twodcut3, h_twodcut3_matched, h_twodcut3_unmatched;
    std::unique_ptr<Hists> h_nbtagm3, h_nbtagm3_matched, h_nbtagm3_unmatched;
    std::unique_ptr<Hists> h_nbtagt3, h_nbtagt3_matched, h_nbtagt3_unmatched;
    std::unique_ptr<Hists> h_twodcut4, h_twodcut4_matched, h_twodcut4_unmatched;
    std::unique_ptr<Hists> h_nbtagm4, h_nbtagm4_matched, h_nbtagm4_unmatched;
    std::unique_ptr<Hists> h_nbtagt4, h_nbtagt4_matched, h_nbtagt4_unmatched;
    std::unique_ptr<Hists> h_twodcut5, h_twodcut5_matched, h_twodcut5_unmatched;
    std::unique_ptr<Hists> h_nbtagm5, h_nbtagm5_matched, h_nbtagm5_unmatched;
    std::unique_ptr<Hists> h_nbtagt5, h_nbtagt5_matched, h_nbtagt5_unmatched;
    std::unique_ptr<Hists> h_twodcut6, h_twodcut6_matched, h_twodcut6_unmatched;
    std::unique_ptr<Hists> h_nbtagm6, h_nbtagm6_matched, h_nbtagm6_unmatched;
    std::unique_ptr<Hists> h_nbtagt6, h_nbtagt6_matched, h_nbtagt6_unmatched;
    std::unique_ptr<Hists> h_twodcut7, h_twodcut7_matched, h_twodcut7_unmatched;
    std::unique_ptr<Hists> h_nbtagm7, h_nbtagm7_matched, h_nbtagm7_unmatched;
    std::unique_ptr<Hists> h_nbtagt7, h_nbtagt7_matched, h_nbtagt7_unmatched;
    std::unique_ptr<Hists> h_twodcut8, h_twodcut8_matched, h_twodcut8_unmatched;
    std::unique_ptr<Hists> h_nbtagm8, h_nbtagm8_matched, h_nbtagm8_unmatched;
    std::unique_ptr<Hists> h_nbtagt8, h_nbtagt8_matched, h_nbtagt8_unmatched;
    std::unique_ptr<Hists> h_twodcut9, h_twodcut9_matched, h_twodcut9_unmatched;
    std::unique_ptr<Hists> h_nbtagm9, h_nbtagm9_matched, h_nbtagm9_unmatched;
    std::unique_ptr<Hists> h_nbtagt9, h_nbtagt9_matched, h_nbtagt9_unmatched;
    std::unique_ptr<Hists> h_twodcut10, h_twodcut10_matched, h_twodcut10_unmatched;
    std::unique_ptr<Hists> h_nbtagm10, h_nbtagm10_matched, h_nbtagm10_unmatched;
    std::unique_ptr<Hists> h_nbtagt10, h_nbtagt10_matched, h_nbtagt10_unmatched;
    std::unique_ptr<Hists> h_twodcut11, h_twodcut11_matched, h_twodcut11_unmatched;
    std::unique_ptr<Hists> h_nbtagm11, h_nbtagm11_matched, h_nbtagm11_unmatched;
    std::unique_ptr<Hists> h_nbtagt11, h_nbtagt11_matched, h_nbtagt11_unmatched;
    std::unique_ptr<Hists> h_twodcut12, h_twodcut12_matched, h_twodcut12_unmatched;
    std::unique_ptr<Hists> h_nbtagm12, h_nbtagm12_matched, h_nbtagm12_unmatched;
    std::unique_ptr<Hists> h_nbtagt12, h_nbtagt12_matched, h_nbtagt12_unmatched;
    std::unique_ptr<Hists> h_twodcut13, h_twodcut13_matched, h_twodcut13_unmatched;
    std::unique_ptr<Hists> h_nbtagm13, h_nbtagm13_matched, h_nbtagm13_unmatched;
    std::unique_ptr<Hists> h_nbtagt13, h_nbtagt13_matched, h_nbtagt13_unmatched;
    std::unique_ptr<Hists> h_twodcut14, h_twodcut14_matched, h_twodcut14_unmatched;
    std::unique_ptr<Hists> h_nbtagm14, h_nbtagm14_matched, h_nbtagm14_unmatched;
    std::unique_ptr<Hists> h_nbtagt14, h_nbtagt14_matched, h_nbtagt14_unmatched;
    std::unique_ptr<Hists> h_twodcut15, h_twodcut15_matched, h_twodcut15_unmatched;
    std::unique_ptr<Hists> h_nbtagm15, h_nbtagm15_matched, h_nbtagm15_unmatched;
    std::unique_ptr<Hists> h_nbtagt15, h_nbtagt15_matched, h_nbtagt15_unmatched;
    std::unique_ptr<Hists> h_twodcut16, h_twodcut16_matched, h_twodcut16_unmatched;
    std::unique_ptr<Hists> h_nbtagm16, h_nbtagm16_matched, h_nbtagm16_unmatched;
    std::unique_ptr<Hists> h_nbtagt16, h_nbtagt16_matched, h_nbtagt16_unmatched;



    JetId Btag_medium, Btag_tight;

    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;
  };


  PostKinCutModule::PostKinCutModule(Context & ctx){
    Btag_medium       = CSVBTag(CSVBTag::WP_MEDIUM);
    Btag_tight        = CSVBTag(CSVBTag::WP_TIGHT);

    gentopjetcleaner.reset(new GenTopJetCleaner(ctx));


    isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft");
    isMC = (ctx.get("dataset_type") == "MC");

    // 2. set up selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      recmatching.reset(new RecMatching(ctx));
    }


    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    ntopjet1_sel.reset(new NTopJetSelection(1,1));
    mass_sel0.reset(new RecMassSelection(0));   // 0: compare mass between first and second topjet
    mass_sel1.reset(new RecMassSelection(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
    dr_sel.reset(new RecdRSelection());
    pt_topjet_sel1.reset(new RecPtSelection(400, 200));
    twodcut_sel1.reset(new TwoDCut(0.4, 25));
    twodcut_sel2.reset(new TwoDCut(0.4, 26));
    twodcut_sel3.reset(new TwoDCut(0.4, 27));
    twodcut_sel4.reset(new TwoDCut(0.4, 28));
    twodcut_sel5.reset(new TwoDCut(0.4, 29));
    twodcut_sel6.reset(new TwoDCut(0.4, 30));
    twodcut_sel7.reset(new TwoDCut(0.4, 31));
    twodcut_sel8.reset(new TwoDCut(0.4, 32));
    twodcut_sel9.reset(new TwoDCut(0.4, 33));
    twodcut_sel10.reset(new TwoDCut(0.4, 34));
    twodcut_sel11.reset(new TwoDCut(0.4, 35));
    twodcut_sel12.reset(new TwoDCut(0.4, 36));
    twodcut_sel13.reset(new TwoDCut(0.4, 37));
    twodcut_sel14.reset(new TwoDCut(0.4, 38));
    twodcut_sel15.reset(new TwoDCut(0.4, 39));
    twodcut_sel16.reset(new TwoDCut(0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));
    nbtag_tight_sel.reset(new NJetSelection(1, -1, Btag_tight));


    // 3. Set up Hists classes:
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

    h_mass0.reset(new TopSubstructureRecoHists(ctx, "mass0"));
    h_mass0_matched.reset(new TopSubstructureRecoHists(ctx, "mass0_matched"));
    h_mass0_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass0_unmatched"));

    h_mass1.reset(new TopSubstructureRecoHists(ctx, "mass1"));
    h_mass1_matched.reset(new TopSubstructureRecoHists(ctx, "mass1_matched"));
    h_mass1_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass1_unmatched"));

    h_twodcut1.reset(new TopSubstructureRecoHists(ctx, "twodcut1"));
    h_twodcut1_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut1_matched"));
    h_twodcut1_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut1_unmatched"));

    h_nbtagm1.reset(new TopSubstructureRecoHists(ctx, "nbtagm1"));
    h_nbtagm1_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm1_matched"));
    h_nbtagm1_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm1_unmatched"));
    h_nbtagt1.reset(new TopSubstructureRecoHists(ctx, "nbtagt1"));
    h_nbtagt1_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt1_matched"));
    h_nbtagt1_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt1_unmatched"));

    h_twodcut2.reset(new TopSubstructureRecoHists(ctx, "twodcut2"));
    h_twodcut2_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut2_matched"));
    h_twodcut2_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut2_unmatched"));

    h_nbtagm2.reset(new TopSubstructureRecoHists(ctx, "nbtagm2"));
    h_nbtagm2_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm2_matched"));
    h_nbtagm2_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm2_unmatched"));
    h_nbtagt2.reset(new TopSubstructureRecoHists(ctx, "nbtagt2"));
    h_nbtagt2_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt2_matched"));
    h_nbtagt2_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt2_unmatched"));

    h_twodcut3.reset(new TopSubstructureRecoHists(ctx, "twodcut3"));
    h_twodcut3_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut3_matched"));
    h_twodcut3_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut3_unmatched"));

    h_nbtagm3.reset(new TopSubstructureRecoHists(ctx, "nbtagm3"));
    h_nbtagm3_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm3_matched"));
    h_nbtagm3_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm3_unmatched"));
    h_nbtagt3.reset(new TopSubstructureRecoHists(ctx, "nbtagt3"));
    h_nbtagt3_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt3_matched"));
    h_nbtagt3_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt3_unmatched"));

    h_twodcut4.reset(new TopSubstructureRecoHists(ctx, "twodcut4"));
    h_twodcut4_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut4_matched"));
    h_twodcut4_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut4_unmatched"));

    h_nbtagm4.reset(new TopSubstructureRecoHists(ctx, "nbtagm4"));
    h_nbtagm4_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm4_matched"));
    h_nbtagm4_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm4_unmatched"));
    h_nbtagt4.reset(new TopSubstructureRecoHists(ctx, "nbtagt4"));
    h_nbtagt4_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt4_matched"));
    h_nbtagt4_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt4_unmatched"));

    h_twodcut5.reset(new TopSubstructureRecoHists(ctx, "twodcut5"));
    h_twodcut5_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut5_matched"));
    h_twodcut5_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut5_unmatched"));

    h_nbtagm5.reset(new TopSubstructureRecoHists(ctx, "nbtagm5"));
    h_nbtagm5_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm5_matched"));
    h_nbtagm5_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm5_unmatched"));
    h_nbtagt5.reset(new TopSubstructureRecoHists(ctx, "nbtagt5"));
    h_nbtagt5_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt5_matched"));
    h_nbtagt5_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt5_unmatched"));

    h_twodcut6.reset(new TopSubstructureRecoHists(ctx, "twodcut6"));
    h_twodcut6_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut6_matched"));
    h_twodcut6_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut6_unmatched"));

    h_nbtagm6.reset(new TopSubstructureRecoHists(ctx, "nbtagm6"));
    h_nbtagm6_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm6_matched"));
    h_nbtagm6_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm6_unmatched"));
    h_nbtagt6.reset(new TopSubstructureRecoHists(ctx, "nbtagt6"));
    h_nbtagt6_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt6_matched"));
    h_nbtagt6_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt6_unmatched"));

    h_twodcut7.reset(new TopSubstructureRecoHists(ctx, "twodcut7"));
    h_twodcut7_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut7_matched"));
    h_twodcut7_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut7_unmatched"));

    h_nbtagm7.reset(new TopSubstructureRecoHists(ctx, "nbtagm7"));
    h_nbtagm7_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm7_matched"));
    h_nbtagm7_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm7_unmatched"));
    h_nbtagt7.reset(new TopSubstructureRecoHists(ctx, "nbtagt7"));
    h_nbtagt7_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt7_matched"));
    h_nbtagt7_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt7_unmatched"));

    h_twodcut8.reset(new TopSubstructureRecoHists(ctx, "twodcut8"));
    h_twodcut8_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut8_matched"));
    h_twodcut8_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut8_unmatched"));

    h_nbtagm8.reset(new TopSubstructureRecoHists(ctx, "nbtagm8"));
    h_nbtagm8_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm8_matched"));
    h_nbtagm8_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm8_unmatched"));
    h_nbtagt8.reset(new TopSubstructureRecoHists(ctx, "nbtagt8"));
    h_nbtagt8_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt8_matched"));
    h_nbtagt8_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt8_unmatched"));

    h_twodcut9.reset(new TopSubstructureRecoHists(ctx, "twodcut9"));
    h_twodcut9_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut9_matched"));
    h_twodcut9_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut9_unmatched"));

    h_nbtagm9.reset(new TopSubstructureRecoHists(ctx, "nbtagm9"));
    h_nbtagm9_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm9_matched"));
    h_nbtagm9_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm9_unmatched"));
    h_nbtagt9.reset(new TopSubstructureRecoHists(ctx, "nbtagt9"));
    h_nbtagt9_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt9_matched"));
    h_nbtagt9_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt9_unmatched"));

    h_twodcut10.reset(new TopSubstructureRecoHists(ctx, "twodcut10"));
    h_twodcut10_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut10_matched"));
    h_twodcut10_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut10_unmatched"));

    h_nbtagm10.reset(new TopSubstructureRecoHists(ctx, "nbtagm10"));
    h_nbtagm10_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm10_matched"));
    h_nbtagm10_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm10_unmatched"));
    h_nbtagt10.reset(new TopSubstructureRecoHists(ctx, "nbtagt10"));
    h_nbtagt10_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt10_matched"));
    h_nbtagt10_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt10_unmatched"));

    h_twodcut11.reset(new TopSubstructureRecoHists(ctx, "twodcut11"));
    h_twodcut11_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut11_matched"));
    h_twodcut11_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut11_unmatched"));

    h_nbtagm11.reset(new TopSubstructureRecoHists(ctx, "nbtagm11"));
    h_nbtagm11_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm11_matched"));
    h_nbtagm11_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm11_unmatched"));
    h_nbtagt11.reset(new TopSubstructureRecoHists(ctx, "nbtagt11"));
    h_nbtagt11_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt11_matched"));
    h_nbtagt11_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt11_unmatched"));

    h_twodcut12.reset(new TopSubstructureRecoHists(ctx, "twodcut12"));
    h_twodcut12_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut12_matched"));
    h_twodcut12_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut12_unmatched"));

    h_nbtagm12.reset(new TopSubstructureRecoHists(ctx, "nbtagm12"));
    h_nbtagm12_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm12_matched"));
    h_nbtagm12_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm12_unmatched"));
    h_nbtagt12.reset(new TopSubstructureRecoHists(ctx, "nbtagt12"));
    h_nbtagt12_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt12_matched"));
    h_nbtagt12_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt12_unmatched"));

    h_twodcut13.reset(new TopSubstructureRecoHists(ctx, "twodcut13"));
    h_twodcut13_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut13_matched"));
    h_twodcut13_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut13_unmatched"));

    h_nbtagm13.reset(new TopSubstructureRecoHists(ctx, "nbtagm13"));
    h_nbtagm13_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm13_matched"));
    h_nbtagm13_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm13_unmatched"));
    h_nbtagt13.reset(new TopSubstructureRecoHists(ctx, "nbtagt13"));
    h_nbtagt13_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt13_matched"));
    h_nbtagt13_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt13_unmatched"));

    h_twodcut14.reset(new TopSubstructureRecoHists(ctx, "twodcut14"));
    h_twodcut14_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut14_matched"));
    h_twodcut14_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut14_unmatched"));

    h_nbtagm14.reset(new TopSubstructureRecoHists(ctx, "nbtagm14"));
    h_nbtagm14_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm14_matched"));
    h_nbtagm14_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm14_unmatched"));
    h_nbtagt14.reset(new TopSubstructureRecoHists(ctx, "nbtagt14"));
    h_nbtagt14_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt14_matched"));
    h_nbtagt14_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt14_unmatched"));

    h_twodcut15.reset(new TopSubstructureRecoHists(ctx, "twodcut15"));
    h_twodcut15_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut15_matched"));
    h_twodcut15_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut15_unmatched"));

    h_nbtagm15.reset(new TopSubstructureRecoHists(ctx, "nbtagm15"));
    h_nbtagm15_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm15_matched"));
    h_nbtagm15_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm15_unmatched"));
    h_nbtagt15.reset(new TopSubstructureRecoHists(ctx, "nbtagt15"));
    h_nbtagt15_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt15_matched"));
    h_nbtagt15_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt15_unmatched"));

    h_twodcut16.reset(new TopSubstructureRecoHists(ctx, "twodcut16"));
    h_twodcut16_matched.reset(new TopSubstructureRecoHists(ctx, "twodcut16_matched"));
    h_twodcut16_unmatched.reset(new TopSubstructureRecoHists(ctx, "twodcut16_unmatched"));

    h_nbtagm16.reset(new TopSubstructureRecoHists(ctx, "nbtagm16"));
    h_nbtagm16_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagm16_matched"));
    h_nbtagm16_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagm16_unmatched"));
    h_nbtagt16.reset(new TopSubstructureRecoHists(ctx, "nbtagt16"));
    h_nbtagt16_matched.reset(new TopSubstructureRecoHists(ctx, "nbtagt16_matched"));
    h_nbtagt16_unmatched.reset(new TopSubstructureRecoHists(ctx, "nbtagt16_unmatched"));
  }


  bool PostKinCutModule::process(Event & event) {
    cout << "PostKinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    if(isTTbar) ttgenprod->process(event);
    // 1. run all modules other modules.
    passed_rec = false;
    matched_rec = false;
    passed_rec_2 = false;
    passed_rec_3 = false;
    matched_rec_2 = false;

    /*
    ████████  ██████  ██████   ██████
       ██    ██    ██ ██   ██ ██    ██
       ██    ██    ██ ██   ██ ██    ██
       ██    ██    ██ ██   ██ ██    ██
       ██     ██████  ██████   ██████

       Additional AK4 Jets: Don't save them in a handle. Either write a function which returns an integer of the number of AK4 jets or program it directly in the hist class. Check for both candidates! Hadronic and Leptonic!!
    */

    // pT(first Topjet) > 400
    passed_rec = pt_topjet_sel1->passes(event);
    if(passed_rec){
      h_pt_topjet->fill(event);
      if(isTTbar){
        matched_rec = recmatching->passes(event);
        if(matched_rec) h_pt_topjet_matched->fill(event);
        else h_pt_topjet_unmatched->fill(event);
      }


      passed_rec_3 = ntopjet1_sel->passes(event);
      if(passed_rec_3){
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

          // // M(first TopJet) > M(second TopJet)
          // passed_rec = mass_sel0->passes(event);
          // if(passed_rec){
          //   h_mass0->fill(event);
          //   if(isTTbar){
          //     matched_rec = recmatching->passes(event);
          //     if(matched_rec) h_mass0_matched->fill(event);
          //     else h_mass0_unmatched->fill(event);
          //   }
          // }

          // M(first TopJet) > M(second TopJet + Muon)
          passed_rec_2 = mass_sel1->passes(event);
          if(passed_rec_2){
            h_mass1->fill(event);
            if(isTTbar){
              matched_rec_2 = recmatching->passes(event);
              if(matched_rec_2) h_mass1_matched->fill(event);
              else h_mass1_unmatched->fill(event);
            }

            passed_rec_2 = twodcut_sel1->passes(event);
            if(passed_rec_2){
              h_twodcut1->fill(event);
              if(isTTbar){
                matched_rec_2 = recmatching->passes(event);
                if(matched_rec_2) h_twodcut1_matched->fill(event);
                else h_twodcut1_unmatched->fill(event);
              }
              if(nbtag_medium_sel->passes(event)){
                h_nbtagm1->fill(event);
                if(isTTbar){
                  matched_rec_2 = recmatching->passes(event);
                  if(matched_rec_2) h_nbtagm1_matched->fill(event);
                  else h_nbtagm1_unmatched->fill(event);
                }
                if(nbtag_tight_sel->passes(event)){
                  h_nbtagt1->fill(event);
                  if(isTTbar){
                    matched_rec_2 = recmatching->passes(event);
                    if(matched_rec_2) h_nbtagt1_matched->fill(event);
                    else h_nbtagt1_unmatched->fill(event);
                  }
                }
              }

              passed_rec_2 = twodcut_sel2->passes(event);
              if(passed_rec_2){
                h_twodcut2->fill(event);
                if(isTTbar){
                  matched_rec_2 = recmatching->passes(event);
                  if(matched_rec_2) h_twodcut2_matched->fill(event);
                  else h_twodcut2_unmatched->fill(event);
                }
                if(nbtag_medium_sel->passes(event)){
                  h_nbtagm2->fill(event);
                  if(isTTbar){
                    matched_rec_2 = recmatching->passes(event);
                    if(matched_rec_2) h_nbtagm2_matched->fill(event);
                    else h_nbtagm2_unmatched->fill(event);
                  }
                  if(nbtag_tight_sel->passes(event)){
                    h_nbtagt2->fill(event);
                    if(isTTbar){
                      matched_rec_2 = recmatching->passes(event);
                      if(matched_rec_2) h_nbtagt2_matched->fill(event);
                      else h_nbtagt2_unmatched->fill(event);
                    }
                  }
                }

                passed_rec_2 = twodcut_sel3->passes(event);
                if(passed_rec_2){
                  h_twodcut3->fill(event);
                  if(isTTbar){
                    matched_rec_2 = recmatching->passes(event);
                    if(matched_rec_2) h_twodcut3_matched->fill(event);
                    else h_twodcut3_unmatched->fill(event);
                  }
                  if(nbtag_medium_sel->passes(event)){
                    h_nbtagm3->fill(event);
                    if(isTTbar){
                      matched_rec_2 = recmatching->passes(event);
                      if(matched_rec_2) h_nbtagm3_matched->fill(event);
                      else h_nbtagm3_unmatched->fill(event);
                    }
                    if(nbtag_tight_sel->passes(event)){
                      h_nbtagt3->fill(event);
                      if(isTTbar){
                        matched_rec_2 = recmatching->passes(event);
                        if(matched_rec_2) h_nbtagt3_matched->fill(event);
                        else h_nbtagt3_unmatched->fill(event);
                      }
                    }
                  }


                  passed_rec_2 = twodcut_sel4->passes(event);
                  if(passed_rec_2){
                    h_twodcut4->fill(event);
                    if(isTTbar){
                      matched_rec_2 = recmatching->passes(event);
                      if(matched_rec_2) h_twodcut4_matched->fill(event);
                      else h_twodcut4_unmatched->fill(event);
                    }
                    if(nbtag_medium_sel->passes(event)){
                      h_nbtagm4->fill(event);
                      if(isTTbar){
                        matched_rec_2 = recmatching->passes(event);
                        if(matched_rec_2) h_nbtagm4_matched->fill(event);
                        else h_nbtagm4_unmatched->fill(event);
                      }
                      if(nbtag_tight_sel->passes(event)){
                        h_nbtagt4->fill(event);
                        if(isTTbar){
                          matched_rec_2 = recmatching->passes(event);
                          if(matched_rec_2) h_nbtagt4_matched->fill(event);
                          else h_nbtagt4_unmatched->fill(event);
                        }
                      }
                    }

                    passed_rec_2 = twodcut_sel5->passes(event);
                    if(passed_rec_2){
                      h_twodcut5->fill(event);
                      if(isTTbar){
                        matched_rec_2 = recmatching->passes(event);
                        if(matched_rec_2) h_twodcut5_matched->fill(event);
                        else h_twodcut5_unmatched->fill(event);
                      }
                      if(nbtag_medium_sel->passes(event)){
                        h_nbtagm5->fill(event);
                        if(isTTbar){
                          matched_rec_2 = recmatching->passes(event);
                          if(matched_rec_2) h_nbtagm5_matched->fill(event);
                          else h_nbtagm5_unmatched->fill(event);
                        }
                        if(nbtag_tight_sel->passes(event)){
                          h_nbtagt5->fill(event);
                          if(isTTbar){
                            matched_rec_2 = recmatching->passes(event);
                            if(matched_rec_2) h_nbtagt5_matched->fill(event);
                            else h_nbtagt5_unmatched->fill(event);
                          }
                        }
                      }

                      passed_rec_2 = twodcut_sel6->passes(event);
                      if(passed_rec_2){
                        h_twodcut6->fill(event);
                        if(isTTbar){
                          matched_rec_2 = recmatching->passes(event);
                          if(matched_rec_2) h_twodcut6_matched->fill(event);
                          else h_twodcut6_unmatched->fill(event);
                        }
                        if(nbtag_medium_sel->passes(event)){
                          h_nbtagm6->fill(event);
                          if(isTTbar){
                            matched_rec_2 = recmatching->passes(event);
                            if(matched_rec_2) h_nbtagm6_matched->fill(event);
                            else h_nbtagm6_unmatched->fill(event);
                          }
                          if(nbtag_tight_sel->passes(event)){
                            h_nbtagt6->fill(event);
                            if(isTTbar){
                              matched_rec_2 = recmatching->passes(event);
                              if(matched_rec_2) h_nbtagt6_matched->fill(event);
                              else h_nbtagt6_unmatched->fill(event);
                            }
                          }
                        }

                        passed_rec_2 = twodcut_sel7->passes(event);
                        if(passed_rec_2){
                          h_twodcut7->fill(event);
                          if(isTTbar){
                            matched_rec_2 = recmatching->passes(event);
                            if(matched_rec_2) h_twodcut7_matched->fill(event);
                            else h_twodcut7_unmatched->fill(event);
                          }
                          if(nbtag_medium_sel->passes(event)){
                            h_nbtagm7->fill(event);
                            if(isTTbar){
                              matched_rec_2 = recmatching->passes(event);
                              if(matched_rec_2) h_nbtagm7_matched->fill(event);
                              else h_nbtagm7_unmatched->fill(event);
                            }
                            if(nbtag_tight_sel->passes(event)){
                              h_nbtagt7->fill(event);
                              if(isTTbar){
                                matched_rec_2 = recmatching->passes(event);
                                if(matched_rec_2) h_nbtagt7_matched->fill(event);
                                else h_nbtagt7_unmatched->fill(event);
                              }
                            }
                          }

                          passed_rec_2 = twodcut_sel8->passes(event);
                          if(passed_rec_2){
                            h_twodcut8->fill(event);
                            if(isTTbar){
                              matched_rec_2 = recmatching->passes(event);
                              if(matched_rec_2) h_twodcut8_matched->fill(event);
                              else h_twodcut8_unmatched->fill(event);
                            }
                            if(nbtag_medium_sel->passes(event)){
                              h_nbtagm8->fill(event);
                              if(isTTbar){
                                matched_rec_2 = recmatching->passes(event);
                                if(matched_rec_2) h_nbtagm8_matched->fill(event);
                                else h_nbtagm8_unmatched->fill(event);
                              }
                              if(nbtag_tight_sel->passes(event)){
                                h_nbtagt8->fill(event);
                                if(isTTbar){
                                  matched_rec_2 = recmatching->passes(event);
                                  if(matched_rec_2) h_nbtagt8_matched->fill(event);
                                  else h_nbtagt8_unmatched->fill(event);
                                }
                              }
                            }

                            passed_rec_2 = twodcut_sel9->passes(event);
                            if(passed_rec_2){
                              h_twodcut9->fill(event);
                              if(isTTbar){
                                matched_rec_2 = recmatching->passes(event);
                                if(matched_rec_2) h_twodcut9_matched->fill(event);
                                else h_twodcut9_unmatched->fill(event);
                              }
                              if(nbtag_medium_sel->passes(event)){
                                h_nbtagm9->fill(event);
                                if(isTTbar){
                                  matched_rec_2 = recmatching->passes(event);
                                  if(matched_rec_2) h_nbtagm9_matched->fill(event);
                                  else h_nbtagm9_unmatched->fill(event);
                                }
                                if(nbtag_tight_sel->passes(event)){
                                  h_nbtagt9->fill(event);
                                  if(isTTbar){
                                    matched_rec_2 = recmatching->passes(event);
                                    if(matched_rec_2) h_nbtagt9_matched->fill(event);
                                    else h_nbtagt9_unmatched->fill(event);
                                  }
                                }
                              }

                              passed_rec_2 = twodcut_sel10->passes(event);
                              if(passed_rec_2){
                                h_twodcut10->fill(event);
                                if(isTTbar){
                                  matched_rec_2 = recmatching->passes(event);
                                  if(matched_rec_2) h_twodcut10_matched->fill(event);
                                  else h_twodcut10_unmatched->fill(event);
                                }
                                if(nbtag_medium_sel->passes(event)){
                                  h_nbtagm10->fill(event);
                                  if(isTTbar){
                                    matched_rec_2 = recmatching->passes(event);
                                    if(matched_rec_2) h_nbtagm10_matched->fill(event);
                                    else h_nbtagm10_unmatched->fill(event);
                                  }
                                  if(nbtag_tight_sel->passes(event)){
                                    h_nbtagt10->fill(event);
                                    if(isTTbar){
                                      matched_rec_2 = recmatching->passes(event);
                                      if(matched_rec_2) h_nbtagt10_matched->fill(event);
                                      else h_nbtagt10_unmatched->fill(event);
                                    }
                                  }
                                }

                                passed_rec_2 = twodcut_sel11->passes(event);
                                if(passed_rec_2){
                                  h_twodcut11->fill(event);
                                  if(isTTbar){
                                    matched_rec_2 = recmatching->passes(event);
                                    if(matched_rec_2) h_twodcut11_matched->fill(event);
                                    else h_twodcut11_unmatched->fill(event);
                                  }
                                  if(nbtag_medium_sel->passes(event)){
                                    h_nbtagm11->fill(event);
                                    if(isTTbar){
                                      matched_rec_2 = recmatching->passes(event);
                                      if(matched_rec_2) h_nbtagm11_matched->fill(event);
                                      else h_nbtagm11_unmatched->fill(event);
                                    }
                                    if(nbtag_tight_sel->passes(event)){
                                      h_nbtagt11->fill(event);
                                      if(isTTbar){
                                        matched_rec_2 = recmatching->passes(event);
                                        if(matched_rec_2) h_nbtagt11_matched->fill(event);
                                        else h_nbtagt11_unmatched->fill(event);
                                      }
                                    }
                                  }

                                  passed_rec_2 = twodcut_sel12->passes(event);
                                  if(passed_rec_2){
                                    h_twodcut12->fill(event);
                                    if(isTTbar){
                                      matched_rec_2 = recmatching->passes(event);
                                      if(matched_rec_2) h_twodcut12_matched->fill(event);
                                      else h_twodcut12_unmatched->fill(event);
                                    }
                                    if(nbtag_medium_sel->passes(event)){
                                      h_nbtagm12->fill(event);
                                      if(isTTbar){
                                        matched_rec_2 = recmatching->passes(event);
                                        if(matched_rec_2) h_nbtagm12_matched->fill(event);
                                        else h_nbtagm12_unmatched->fill(event);
                                      }
                                      if(nbtag_tight_sel->passes(event)){
                                        h_nbtagt12->fill(event);
                                        if(isTTbar){
                                          matched_rec_2 = recmatching->passes(event);
                                          if(matched_rec_2) h_nbtagt12_matched->fill(event);
                                          else h_nbtagt12_unmatched->fill(event);
                                        }
                                      }
                                    }

                                    passed_rec_2 = twodcut_sel13->passes(event);
                                    if(passed_rec_2){
                                      h_twodcut13->fill(event);
                                      if(isTTbar){
                                        matched_rec_2 = recmatching->passes(event);
                                        if(matched_rec_2) h_twodcut13_matched->fill(event);
                                        else h_twodcut13_unmatched->fill(event);
                                      }
                                      if(nbtag_medium_sel->passes(event)){
                                        h_nbtagm13->fill(event);
                                        if(isTTbar){
                                          matched_rec_2 = recmatching->passes(event);
                                          if(matched_rec_2) h_nbtagm13_matched->fill(event);
                                          else h_nbtagm13_unmatched->fill(event);
                                        }
                                        if(nbtag_tight_sel->passes(event)){
                                          h_nbtagt13->fill(event);
                                          if(isTTbar){
                                            matched_rec_2 = recmatching->passes(event);
                                            if(matched_rec_2) h_nbtagt13_matched->fill(event);
                                            else h_nbtagt13_unmatched->fill(event);
                                          }
                                        }
                                      }

                                      passed_rec_2 = twodcut_sel14->passes(event);
                                      if(passed_rec_2){
                                        h_twodcut14->fill(event);
                                        if(isTTbar){
                                          matched_rec_2 = recmatching->passes(event);
                                          if(matched_rec_2) h_twodcut14_matched->fill(event);
                                          else h_twodcut14_unmatched->fill(event);
                                        }
                                        if(nbtag_medium_sel->passes(event)){
                                          h_nbtagm14->fill(event);
                                          if(isTTbar){
                                            matched_rec_2 = recmatching->passes(event);
                                            if(matched_rec_2) h_nbtagm14_matched->fill(event);
                                            else h_nbtagm14_unmatched->fill(event);
                                          }
                                          if(nbtag_tight_sel->passes(event)){
                                            h_nbtagt14->fill(event);
                                            if(isTTbar){
                                              matched_rec_2 = recmatching->passes(event);
                                              if(matched_rec_2) h_nbtagt14_matched->fill(event);
                                              else h_nbtagt14_unmatched->fill(event);
                                            }
                                          }
                                        }

                                        passed_rec_2 = twodcut_sel15->passes(event);
                                        if(passed_rec_2){
                                          h_twodcut15->fill(event);
                                          if(isTTbar){
                                            matched_rec_2 = recmatching->passes(event);
                                            if(matched_rec_2) h_twodcut15_matched->fill(event);
                                            else h_twodcut15_unmatched->fill(event);
                                          }
                                          if(nbtag_medium_sel->passes(event)){
                                            h_nbtagm15->fill(event);
                                            if(isTTbar){
                                              matched_rec_2 = recmatching->passes(event);
                                              if(matched_rec_2) h_nbtagm15_matched->fill(event);
                                              else h_nbtagm15_unmatched->fill(event);
                                            }
                                            if(nbtag_tight_sel->passes(event)){
                                              h_nbtagt15->fill(event);
                                              if(isTTbar){
                                                matched_rec_2 = recmatching->passes(event);
                                                if(matched_rec_2) h_nbtagt15_matched->fill(event);
                                                else h_nbtagt15_unmatched->fill(event);
                                              }
                                            }
                                          }

                                          passed_rec_2 = twodcut_sel16->passes(event);
                                          if(passed_rec_2){
                                            h_twodcut16->fill(event);
                                            if(isTTbar){
                                              matched_rec_2 = recmatching->passes(event);
                                              if(matched_rec_2) h_twodcut16_matched->fill(event);
                                              else h_twodcut16_unmatched->fill(event);
                                            }
                                            if(nbtag_medium_sel->passes(event)){
                                              h_nbtagm16->fill(event);
                                              if(isTTbar){
                                                matched_rec_2 = recmatching->passes(event);
                                                if(matched_rec_2) h_nbtagm16_matched->fill(event);
                                                else h_nbtagm16_unmatched->fill(event);
                                              }
                                              passed_rec_2 = nbtag_tight_sel->passes(event);
                                              if(passed_rec_2){
                                                h_nbtagt16->fill(event);
                                                if(isTTbar){
                                                  matched_rec_2 = recmatching->passes(event);
                                                  if(matched_rec_2) h_nbtagt16_matched->fill(event);
                                                  else h_nbtagt16_unmatched->fill(event);
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
    if((!passed_rec) || (!passed_rec_2)) return false;

    // 3. decide whether or not to keep the current event in the output:
    return true;
  }

  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(PostKinCutModule)

}
