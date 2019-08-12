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
#include "UHH2/TopSubstructure/include/PuppiRecoHists.h"
#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"
#include "UHH2/TopSubstructure/include/PuppiJetUtils.h"
#include "UHH2/TopSubstructure/include/TopJetCorrections.h"
#include "UHH2/TopSubstructure/include/PuppiJetCorrections.h"
#include "UHH2/TopSubstructure/include/VariablesCalculator.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class KinCutModule: public AnalysisModule {
  public:
    explicit KinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    enum lepton { muon, ele };
    lepton channel_;

    std::unique_ptr<CommonModules>   common;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;
    // Calculator calculator;

    bool isMC, isMu, isEle, isTTbar;
    std::unique_ptr<TopJetCorrections> topjetCorr;
    std::unique_ptr<PuppiJetCorrections> puppijetCorr;
    bool error_test;

    //declare generator booleans
    bool do_tau_lepcleaner, do_tau_cleaner, do_cleaner;
    bool passed_pt_lep_gen,  passed_mu_gen, passed_ele_gen;
    bool passed_mu_gen_pre, passed_ele_gen_pre;
    bool passed_mu_rec_pre, passed_ele_rec_pre, passed_mu_rec, passed_ele_rec;

    //declare reconstruction booleans
    bool passed_rec, passed_trigger, passed_pv, passed_met, passed_nlep0, passed_nlep1, passed_pt_lep, passed_twodcut, passed_nbtag_medium;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<TopJetCleaner> topjetcleaner, puppijetcleaner;
    std::unique_ptr<BTagMCEfficiencyHists> hists_btag_eff;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    //declare generator selections
    std::unique_ptr<Selection> met_gen, pt_lep_gen;

    //declare reconstruction selections
    std::unique_ptr<Selection> elec_sel_triggerA, trigger_sel_A, trigger_sel_B, trigger_sel_C, elec_sel_120, pv_sel, nbtag_medium_sel, twodcut_sel, met_sel, nlep0_sel, nlep1_sel, pt_lep_sel;

    //declare general stuff
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> cleaner, cleaner_sd, rectopjetleptoncleaner, recpuppijetleptoncleaner;
    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> rm_particle;
    JetId Btag_medium;
    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner, gentopjetcleaner_sd;
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    //declare generator histograms
    std::unique_ptr<Hists> h_gen_pt_lep;

    //declare reconstruction histograms
    std::unique_ptr<Hists> h_test, h_test_puppi, h_common, h_common_puppi, h_topjetcorrections, h_topjetcorrections_puppi, h_puppijetcorrections, h_puppijetcorrections_puppi, h_topjetjer_smearing_puppi, h_jetcleaner, h_jetcleaner_puppi, h_muoncleaner, h_muoncleaner_puppi, h_elecleaner, h_elecleaner_puppi;

    std::unique_ptr<Hists>  h_pv, h_pv_puppi, h_nlep_1, h_nlep_1_puppi, h_tjlc, h_tjlc_puppi, h_pjlc, h_pjlc_puppi, h_tjc, h_tjc_puppi, h_pjc, h_pjc_puppi, h_met, h_met_puppi, h_pt_lep, h_pt_lep_puppi, h_nlep_0, h_nlep_0_puppi, h_twodcut, h_twodcut_puppi, h_nbtag_medium, h_nbtag_medium_puppi, h_trigger, h_trigger_puppi;


    //declare handles
    uhh2::Event::Handle<double> h_rec_weight_kin;
    uhh2::Event::Handle<double> h_gen_weight_kin;

    // 4 calculated taus (for PUPPI and CHS):
    // 1. at the start
    // 2. after top-/puppijetcorrector
    // 3. after top-/puppijetleptoncleaner
    // 4. after top-/puppijetcleaner
    uhh2::Event::Handle<double> h_gen_tau2_start, h_gen_tau2_lepcleaner, h_gen_tau2_cleaner;
    uhh2::Event::Handle<double> h_gen_tau3_start, h_gen_tau3_lepcleaner, h_gen_tau3_cleaner;

    uhh2::Event::Handle<double> h_pf_tau2_start, h_pf_tau2_common, h_pf_tau2_corrector, h_pf_tau2_lepcleaner, h_pf_tau2_cleaner;
    uhh2::Event::Handle<double> h_pf_tau2_start_puppi, h_pf_tau2_common_puppi, h_pf_tau2_corrector_puppi, h_pf_tau2_lepcleaner_puppi, h_pf_tau2_cleaner_puppi;
    uhh2::Event::Handle<double> h_pf_tau3_start, h_pf_tau3_common, h_pf_tau3_corrector, h_pf_tau3_lepcleaner, h_pf_tau3_cleaner;
    uhh2::Event::Handle<double> h_pf_tau3_start_puppi, h_pf_tau3_common_puppi, h_pf_tau3_corrector_puppi, h_pf_tau3_lepcleaner_puppi, h_pf_tau3_cleaner_puppi;

    uhh2::Event::Handle<bool> h_passed_mu_gen_pre, h_passed_ele_gen_pre, h_passed_mu_rec_pre, h_passed_ele_rec_pre;
    uhh2::Event::Handle<bool> h_passed_mu_gen, h_passed_mu_rec, h_passed_ele_gen, h_passed_ele_rec;
    uhh2::Event::Handle<std::vector<TopJet>> h_puppi;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    string puppi_jet_col = "jetsAk8PuppiSubstructure_SoftDropPuppi";
    string gentopjet_col = "genjetsAk8Substructure";
    std::unique_ptr<Calculator> calculator_chs;
    std::unique_ptr<Calculator> calculator_puppi;
    std::unique_ptr<Calculator> calculator_gen;
  };


  KinCutModule::KinCutModule(Context & ctx){
    error_test = false;
    if(error_test) cout << "test2" << '\n';
    isMC    = (ctx.get("dataset_type") == "MC");
    if(error_test) cout << "test3" << '\n';

    const std::string& channel = ctx.get("channel", ""); //define Channel
    if(error_test) cout << "test4" << '\n';
    if     (channel == "muon") channel_ = muon;
    else if(channel == "ele") channel_ = ele;
    else {
      std::string log("KinCutModule::KinCutModule -- ");
      log += "invalid argument for 'channel' key in xml file (must be 'muon' or 'ele'): \""+channel+"\"";
      throw std::runtime_error(log);
    }

    if(error_test) cout << "test5" << '\n';
    isEle = (ctx.get("dataset_version") == "DATA_2016v3_SingleElectronB_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronB_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronB_part2" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronC_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronC_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronD_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronD_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronE_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronE_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronF_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronF_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronG_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronG_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronG_part2" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronH_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronH_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleElectronH_part2");

    if(error_test) cout << "test6" << '\n';
    isMu = (ctx.get("dataset_version") == "DATA_2016v3_SingleMuonB_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonB_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonC" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonD"
    || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonE" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonF" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonG_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonG_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonH_part0" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonH_part1" || ctx.get("dataset_version") == "DATA_2016v3_SingleMuonH_part2");

    if(error_test) cout << "test7" << '\n';
    isTTbar = (ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft" || ctx.get("dataset_version") == "TTbar_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700_part1" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part1" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part2" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft_part1" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755_part1" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph_part0");

    // 1. setup other modules. CommonModules and the JetCleaner:
    if(error_test) cout << "test8" << '\n';
    h_passed_mu_gen_pre = ctx.get_handle<bool>("h_passed_mu_gen_pre");
    if(error_test) cout << "test9" << '\n';
    h_passed_ele_gen_pre = ctx.get_handle<bool>("h_passed_ele_gen_pre");
    if(error_test) cout << "test10" << '\n';
    h_passed_mu_rec_pre = ctx.get_handle<bool>("h_passed_mu_rec_pre");
    if(error_test) cout << "test11" << '\n';
    h_passed_ele_rec_pre = ctx.get_handle<bool>("h_passed_ele_rec_pre");
    if(error_test) cout << "test12" << '\n';

    h_puppi          = ctx.get_handle<std::vector<TopJet>>(puppi_jet_col);
    if(error_test) cout << "test13" << '\n';
    h_gentopjet      = ctx.get_handle<std::vector<GenTopJet>>(gentopjet_col);
    if(error_test) cout << "test14" << '\n';
    h_gen_weight_kin = ctx.declare_event_output<double>("h_gen_weight_kin");
    if(error_test) cout << "test15" << '\n';
    h_rec_weight_kin = ctx.declare_event_output<double>("h_rec_weight_kin");
    if(error_test) cout << "test16" << '\n';
    h_passed_mu_gen     = ctx.declare_event_output<bool>("h_passed_mu_gen");
    if(error_test) cout << "test17" << '\n';
    h_passed_mu_rec     = ctx.declare_event_output<bool>("h_passed_mu_rec");
    if(error_test) cout << "test18" << '\n';
    h_passed_ele_gen     = ctx.declare_event_output<bool>("h_passed_ele_gen");
    if(error_test) cout << "test19" << '\n';
    h_passed_ele_rec     = ctx.declare_event_output<bool>("h_passed_ele_rec");
    if(error_test) cout << "test20" << '\n';

    h_gen_tau2_start = ctx.declare_event_output<double>("h_gen_tau2_start");
    if(error_test) cout << "test21" << '\n';
    h_gen_tau2_lepcleaner = ctx.declare_event_output<double>("h_gen_tau2_lepcleaner");
    if(error_test) cout << "test22" << '\n';
    h_gen_tau2_cleaner = ctx.declare_event_output<double>("h_gen_tau2_cleaner");
    if(error_test) cout << "test23" << '\n';
    h_gen_tau3_start = ctx.declare_event_output<double>("h_gen_tau3_start");
    if(error_test) cout << "test24" << '\n';
    h_gen_tau3_lepcleaner = ctx.declare_event_output<double>("h_gen_tau3_lepcleaner");
    if(error_test) cout << "test25" << '\n';
    h_gen_tau3_cleaner = ctx.declare_event_output<double>("h_gen_tau3_cleaner");
    if(error_test) cout << "test26" << '\n';

    h_pf_tau2_start = ctx.declare_event_output<double>("h_pf_tau2_start");
    if(error_test) cout << "test27" << '\n';
    h_pf_tau2_start_puppi = ctx.declare_event_output<double>("h_pf_tau2_start_puppi");
    if(error_test) cout << "test28" << '\n';
    h_pf_tau2_common = ctx.declare_event_output<double>("h_pf_tau2_common");
    if(error_test) cout << "test29" << '\n';
    h_pf_tau2_common_puppi = ctx.declare_event_output<double>("h_pf_tau2_common_puppi");
    if(error_test) cout << "test30" << '\n';
    h_pf_tau2_corrector = ctx.declare_event_output<double>("h_pf_tau2_corrector");
    if(error_test) cout << "test31" << '\n';
    h_pf_tau2_corrector_puppi = ctx.declare_event_output<double>("h_pf_tau2_corrector_puppi");
    if(error_test) cout << "test32" << '\n';
    h_pf_tau2_lepcleaner = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner");
    if(error_test) cout << "test33" << '\n';
    h_pf_tau2_lepcleaner_puppi = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner_puppi");
    if(error_test) cout << "test34" << '\n';
    h_pf_tau2_cleaner = ctx.declare_event_output<double>("h_pf_tau2_cleaner");
    if(error_test) cout << "test35" << '\n';
    h_pf_tau2_cleaner_puppi = ctx.declare_event_output<double>("h_pf_tau2_cleaner_puppi");
    if(error_test) cout << "test36" << '\n';

    h_pf_tau3_start = ctx.declare_event_output<double>("h_pf_tau3_start");
    if(error_test) cout << "test37" << '\n';
    h_pf_tau3_start_puppi = ctx.declare_event_output<double>("h_pf_tau3_start_puppi");
    if(error_test) cout << "test38" << '\n';
    h_pf_tau3_common = ctx.declare_event_output<double>("h_pf_tau3_common");
    if(error_test) cout << "test39" << '\n';
    h_pf_tau3_common_puppi = ctx.declare_event_output<double>("h_pf_tau3_common_puppi");
    if(error_test) cout << "test40" << '\n';
    h_pf_tau3_corrector = ctx.declare_event_output<double>("h_pf_tau3_corrector");
    if(error_test) cout << "test41" << '\n';
    h_pf_tau3_corrector_puppi = ctx.declare_event_output<double>("h_pf_tau3_corrector_puppi");
    if(error_test) cout << "test42" << '\n';
    h_pf_tau3_lepcleaner = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner");
    if(error_test) cout << "test43" << '\n';
    h_pf_tau3_lepcleaner_puppi = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner_puppi");
    if(error_test) cout << "test44" << '\n';
    h_pf_tau3_cleaner = ctx.declare_event_output<double>("h_pf_tau3_cleaner");
    if(error_test) cout << "test45" << '\n';
    h_pf_tau3_cleaner_puppi = ctx.declare_event_output<double>("h_pf_tau3_cleaner_puppi");
    if(error_test) cout << "test46" << '\n';


    common.reset(new CommonModules());
    // lumiweight.reset(new MCLumiWeight(ctx));
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    if(error_test) cout << "test47" << '\n';
    topjetCorr.reset(new TopJetCorrections());
    if(error_test) cout << "test48" << '\n';
    topjetCorr->init(ctx);

    if(error_test) cout << "test49" << '\n';
    puppijetCorr.reset(new PuppiJetCorrections());
    if(error_test) cout << "test50" << '\n';
    puppijetCorr->init(ctx);

    if(error_test) cout << "test51" << '\n';
    common->disable_mclumiweight();
    if(error_test) cout << "test52" << '\n';
    common->switch_jetlepcleaner();
    if(error_test) cout << "test53" << '\n';
    common->disable_mcpileupreweight();
    if(error_test) cout << "test54" << '\n';
    common->init(ctx); // always last!

    if(error_test) cout << "test55" << '\n';
    MuonId muid       = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(55., 2.4));
    // this is only used for cleaner and electron veto
    if(error_test) cout << "test56" << '\n';
    ElectronId eleid_noiso55  = AndId<Electron>(ElectronID_Summer16_tight_noIso, PtEtaSCCut(55., 2.4));
    // this is used to decide which ele trigger is used
    if(error_test) cout << "test57" << '\n';
    ElectronId eleid_noiso120 = AndId<Electron>(PtEtaSCCut(120., 2.4), ElectronID_Summer16_tight_noIso);
    // this is used in combination with iso trigger
    if(error_test) cout << "test58" << '\n';
    ElectronId eleid_iso55    = AndId<Electron>(PtEtaSCCut(55., 2.4), ElectronID_Summer16_tight);
    if(error_test) cout << "test59" << '\n';
    Btag_medium       = CSVBTag(CSVBTag::WP_MEDIUM);

    if(error_test) cout << "test60" << '\n';
    cleaner.reset(new GenTopJetLeptonCleaner(ctx, "genjetsAk8Substructure"));
    if(error_test) cout << "test61" << '\n';
    cleaner_sd.reset(new GenTopJetLeptonCleaner(ctx));
    if(error_test) cout << "test62" << '\n';
    gentopjetcleaner.reset(new GenTopJetCleaner(ctx, "genjetsAk8Substructure"));
    if(error_test) cout << "test63" << '\n';
    gentopjetcleaner_sd.reset(new GenTopJetCleaner(ctx));
    if(error_test) cout << "test64" << '\n';
    muoSR_cleaner.reset(new MuonCleaner(muid));
    if(error_test) cout << "test65" << '\n';
    eleSR_cleaner.reset(new ElectronCleaner(eleid_noiso55));
    if(error_test) cout << "test66" << '\n';
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4));

    if(error_test) cout << "test67" << '\n';
    if(channel_ == muon) rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner(0));
    if(error_test) cout << "test68" << '\n';
    if(channel_ == ele) rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner(1));
    if(error_test) cout << "test69" << '\n';
    if(channel_ == muon) recpuppijetleptoncleaner.reset(new RecPuppiJetLeptonCleaner(ctx, 0));
    if(error_test) cout << "test70" << '\n';
    if(channel_ == ele) recpuppijetleptoncleaner.reset(new RecPuppiJetLeptonCleaner(ctx, 1));
    if(error_test) cout << "test71" << '\n';

    topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5)));
    if(error_test) cout << "test72" << '\n';
    puppijetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5), puppi_jet_col));

    // 2. set up selections

    if(error_test) cout << "test73" << '\n';
    calculator_chs.reset(new Calculator(ctx, "topjets"));
    if(error_test) cout << "test74" << '\n';
    calculator_puppi.reset(new Calculator(ctx, puppi_jet_col));
    if(error_test) cout << "test75" << '\n';
    calculator_gen.reset(new Calculator(ctx, "gentopjets"));

    //setup generator selections
    if(error_test) cout << "test76" << '\n';
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      if(channel_ == muon) pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 0, 55));
      if(channel_ == ele)  pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 1, 55));
    }
    // rm_particle.reset(new ParticleRemover(isMC));
    //setup reconstruction selections
    if(error_test) cout << "test77" << '\n';
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));

    if(error_test) cout << "test78" << '\n';
    if(channel_ == ele) nlep0_sel.reset(new NMuonSelection(0,0));
    if(error_test) cout << "test79" << '\n';
    if(channel_ == muon)  nlep0_sel.reset(new NElectronSelection(0, 0));

    if(error_test) cout << "test80" << '\n';
    met_sel.reset(new METSelection(50,-1));

    if(error_test) cout << "test81" << '\n';
    if(channel_ == muon) pt_lep_sel.reset(new LeptonPtSelection(0, 55));
    if(error_test) cout << "test82" << '\n';
    if(channel_ == ele)  pt_lep_sel.reset(new LeptonPtSelection(1, 55));

    if(error_test) cout << "test83" << '\n';
    if(channel_ == muon) nlep1_sel.reset(new NMuonSelection(1,1,muid));
    if(error_test) cout << "test84" << '\n';
    if(channel_ == ele)  nlep1_sel.reset(new NElectronSelection(1, 1, eleid_noiso55));

    if(error_test) cout << "test85" << '\n';
    if(channel_ == muon){
      if(error_test) cout << "test86" << '\n';
      trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
      if(error_test) cout << "test87" << '\n';
      trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");
    }
    else if(channel_ == ele){
      trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Ele27_WPTight_Gsf_v*");
      trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*");
      trigger_sel_C = uhh2::make_unique<TriggerSelection>("HLT_Photon175_v*");
    }

    if(error_test) cout << "test88" << '\n';
    elec_sel_triggerA.reset(new NElectronSelection(1, 1, eleid_iso55));
    if(error_test) cout << "test89" << '\n';
    elec_sel_120.reset(new NElectronSelection(1, 1, eleid_noiso120));

    if(error_test) cout << "test90" << '\n';
    if(channel_ == muon)     twodcut_sel.reset(new TwoDCut(0, 0.4, 40));
    else if(channel_ == ele) twodcut_sel.reset(new TwoDCut(1, 0.4, 40));
    if(error_test) cout << "test91" << '\n';
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));
    if(error_test) cout << "test92" << '\n';


    // 3. Set up Hists classes:
    h_gen_pt_lep.reset(new GenHists(ctx, "gen_pt_lep"));

    h_test.reset(new TopSubstructureRecoHists(ctx, "test"));
    h_test_puppi.reset(new TopSubstructureRecoHists(ctx, "test_puppi", puppi_jet_col));
    h_common.reset(new TopSubstructureRecoHists(ctx, "common"));
    h_common_puppi.reset(new TopSubstructureRecoHists(ctx, "common_puppi", puppi_jet_col));
    h_topjetcorrections.reset(new TopSubstructureRecoHists(ctx, "topjetcorrections"));
    h_topjetcorrections_puppi.reset(new TopSubstructureRecoHists(ctx, "topjetcorrections_puppi", puppi_jet_col));
    h_puppijetcorrections.reset(new TopSubstructureRecoHists(ctx, "puppijetcorrections"));
    h_puppijetcorrections_puppi.reset(new TopSubstructureRecoHists(ctx, "puppijetcorrections_puppi", puppi_jet_col));
    h_jetcleaner.reset(new TopSubstructureRecoHists(ctx, "jetcleaner"));
    h_jetcleaner_puppi.reset(new TopSubstructureRecoHists(ctx, "jetcleaner_puppi", puppi_jet_col));
    h_muoncleaner.reset(new TopSubstructureRecoHists(ctx, "muoncleaner"));
    h_muoncleaner_puppi.reset(new TopSubstructureRecoHists(ctx, "muoncleaner_puppi", puppi_jet_col));
    h_elecleaner.reset(new TopSubstructureRecoHists(ctx, "elecleaner"));
    h_elecleaner_puppi.reset(new TopSubstructureRecoHists(ctx, "elecleaner_puppi", puppi_jet_col));

    h_trigger.reset(new TopSubstructureRecoHists(ctx, "trigger"));
    h_trigger_puppi.reset(new TopSubstructureRecoHists(ctx, "trigger_puppi", puppi_jet_col));
    h_pv.reset(new TopSubstructureRecoHists(ctx, "pv"));
    h_pv_puppi.reset(new TopSubstructureRecoHists(ctx, "pv_puppi", puppi_jet_col));
    h_nlep_1.reset(new TopSubstructureRecoHists(ctx, "nlep_1"));
    h_nlep_1_puppi.reset(new TopSubstructureRecoHists(ctx, "nlep_1_puppi"));
    h_tjlc.reset(new TopSubstructureRecoHists(ctx, "tjlc"));
    h_tjlc_puppi.reset(new TopSubstructureRecoHists(ctx, "tjlc_puppi", puppi_jet_col));
    h_pjlc.reset(new TopSubstructureRecoHists(ctx, "pjlc"));
    h_pjlc_puppi.reset(new TopSubstructureRecoHists(ctx, "pjlc_puppi", puppi_jet_col));
    h_tjc.reset(new TopSubstructureRecoHists(ctx, "tjc"));
    h_tjc_puppi.reset(new TopSubstructureRecoHists(ctx, "tjc_puppi", puppi_jet_col));
    h_pjc.reset(new TopSubstructureRecoHists(ctx, "pjc"));
    h_pjc_puppi.reset(new TopSubstructureRecoHists(ctx, "pjc_puppi", puppi_jet_col));
    h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
    h_met_puppi.reset(new TopSubstructureRecoHists(ctx, "met_puppi", puppi_jet_col));
    h_pt_lep.reset(new TopSubstructureRecoHists(ctx, "pt_lep"));
    h_pt_lep_puppi.reset(new TopSubstructureRecoHists(ctx, "pt_elp_puppi", puppi_jet_col));
    h_nlep_0.reset(new TopSubstructureRecoHists(ctx, "nmu_0"));
    h_nlep_0_puppi.reset(new TopSubstructureRecoHists(ctx, "nmu_0_puppi", puppi_jet_col));
    h_twodcut.reset(new TopSubstructureRecoHists(ctx, "twodcut"));
    h_twodcut_puppi.reset(new TopSubstructureRecoHists(ctx, "twodcut_puppi", puppi_jet_col));
    hists_btag_eff.reset(new BTagMCEfficiencyHists(ctx,"BTagMedium", Btag_medium));
    h_nbtag_medium.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium"));
    h_nbtag_medium_puppi.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium_puppi", puppi_jet_col));
  }


  bool KinCutModule::process(Event & event) {
    if(error_test) cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    if(error_test) cout << "test93" << '\n';
    event.set(h_gen_weight_kin, event.weight);
    if(error_test) cout << "test94" << '\n';

    event.set(h_pf_tau2_start, calculator_chs->PF_tau2(event));
    if(error_test) cout << "test95" << '\n';
    event.set(h_pf_tau2_start_puppi, calculator_puppi->PF_tau2(event));
    if(error_test) cout << "test96" << '\n';
    event.set(h_pf_tau3_start, calculator_chs->PF_tau3(event));
    if(error_test) cout << "test97" << '\n';
    event.set(h_pf_tau3_start_puppi, calculator_puppi->PF_tau3(event));
    if(error_test) cout << "test98" << '\n';

    if(isMC){
      if(error_test) cout << "test99" << '\n';
      event.set(h_gen_tau2_start, calculator_gen->Gen_tau2(event));
      if(error_test) cout << "test100" << '\n';
      event.set(h_gen_tau3_start, calculator_gen->Gen_tau3(event));
      if(error_test) cout << "test101" << '\n';
    }
    else{
      event.set(h_gen_tau2_start, -100);
      event.set(h_gen_tau3_start, -100);
    }
    if(error_test) cout << "test102" << '\n';
    // 1. run all modules other modules.
    if(event.is_valid(h_passed_mu_gen_pre)) passed_mu_gen_pre = event.get(h_passed_mu_gen_pre);
    else passed_mu_gen_pre = false;
    if(error_test) cout << "test103" << '\n';
    if(event.is_valid(h_passed_ele_gen_pre)) passed_ele_gen_pre = event.get(h_passed_ele_gen_pre);
    else passed_ele_gen_pre = false;
    if(error_test) cout << "test104" << '\n';
    passed_pt_lep_gen  = false;  // passed pt muon gen selection
    passed_mu_gen      = false;  // passed full gen selection
    passed_ele_gen     = false;  // passed full gen selection
    do_tau_lepcleaner  = false;
    do_tau_cleaner     = false;
    do_cleaner         = false;

    if(error_test) cout << "test105" << '\n';
    if(isTTbar){
      if(error_test) cout << "test106" << '\n';
      ttgenprod->process(event);
      if(error_test) cout << "test107" << '\n';
      if((channel_ == muon && passed_mu_gen_pre) || (channel_ == ele && passed_ele_gen_pre)){
        if(error_test) cout << "test108" << '\n';
        cleaner->process(event);    // Do this always!
        if(error_test) cout << "test109" << '\n';
        cleaner_sd->process(event); // Do this always!
        if(error_test) cout << "test110" << '\n';
        do_tau_lepcleaner = true;
        gentopjetcleaner->process(event);
        if(error_test) cout << "test111" << '\n';
        gentopjetcleaner_sd->process(event);
        if(error_test) cout << "test112" << '\n';
        do_tau_cleaner = true;
      }
      if(error_test) cout << "test113" << '\n';
      if(do_tau_lepcleaner && do_tau_cleaner) do_cleaner = true;
      if(error_test) cout << "test114" << '\n';
      passed_pt_lep_gen = pt_lep_gen->passes(event);
      if(error_test) cout << "test115" << '\n';

      if((passed_mu_gen_pre || passed_ele_gen_pre) && do_cleaner && passed_pt_lep_gen){
        if(error_test) cout << "test116" << '\n';
        h_gen_pt_lep->fill(event);
        if(error_test) cout << "test117" << '\n';
        if(channel_ == muon)      passed_mu_gen = true;
        else if(channel_ == ele) passed_ele_gen = true;
        if(error_test) cout << "test118" << '\n';
      }
    }

    if(error_test) cout << "test119" << '\n';
    if(do_tau_lepcleaner){
      if(error_test) cout << "test120" << '\n';
      event.set(h_gen_tau2_lepcleaner, calculator_gen->Gen_tau2(event));
      if(error_test) cout << "test121" << '\n';
      event.set(h_gen_tau3_lepcleaner, calculator_gen->Gen_tau3(event));
      if(error_test) cout << "test122" << '\n';
    }
    else{
      event.set(h_gen_tau2_lepcleaner, -100);
      event.set(h_gen_tau3_lepcleaner, -100);
    }
    if(error_test) cout << "test123" << '\n';

    if(do_tau_cleaner){
      if(error_test) cout << "test124" << '\n';
      event.set(h_gen_tau2_cleaner, calculator_gen->Gen_tau2(event));
      if(error_test) cout << "test125" << '\n';
      event.set(h_gen_tau3_cleaner, calculator_gen->Gen_tau3(event));
      if(error_test) cout << "test126" << '\n';
    }
    else{
      event.set(h_gen_tau2_cleaner, -100);
      event.set(h_gen_tau3_cleaner, -100);
    }

    /*
    ██████  ███████  ██████  ██████
    ██   ██ ██      ██      ██    ██
    ██████  █████   ██      ██    ██
    ██   ██ ██      ██      ██    ██
    ██   ██ ███████  ██████  ██████
    */

    bool passed_trigger_1        = false;
    bool passed_trigger_2        = false;
    if(error_test) cout << "test127" << '\n';
    if(event.is_valid(h_passed_mu_rec_pre)) passed_mu_rec_pre = event.get(h_passed_mu_rec_pre);
    else passed_mu_rec_pre  = false;
    if(error_test) cout << "test128" << '\n';
    if(event.is_valid(h_passed_ele_rec_pre)) passed_ele_rec_pre = event.get(h_passed_ele_rec_pre);
    else passed_ele_rec_pre = false;
    if(error_test) cout << "test129" << '\n';
    passed_trigger          = false;
    passed_pv               = false;
    passed_nlep1            = false;
    passed_met              = false;
    passed_pt_lep           = false;
    passed_nlep0            = false;
    passed_twodcut          = false;
    passed_nbtag_medium     = false;
    passed_mu_rec           = false;  // passed full rec selection
    passed_ele_rec          = false;  // passed full rec selection

    if(error_test) cout << "test130" << '\n';
    bool common_passed = common->process(event);
    if(error_test) cout << "test131" << '\n';
    h_test->fill(event);
    if(error_test) cout << "test132" << '\n';
    h_test_puppi->fill(event);
    if(error_test) cout << "test133" << '\n';
    if(!common_passed) return false;
    if(error_test) cout << "test134" << '\n';

    event.set(h_pf_tau2_common, calculator_chs->PF_tau2(event));
    if(error_test) cout << "test135" << '\n';
    event.set(h_pf_tau3_common, calculator_chs->PF_tau3(event));
    if(error_test) cout << "test136" << '\n';

    event.set(h_pf_tau2_common_puppi, calculator_puppi->PF_tau2(event));
    if(error_test) cout << "test137" << '\n';
    event.set(h_pf_tau3_common_puppi, calculator_puppi->PF_tau3(event));
    if(error_test) cout << "test138" << '\n';

    h_common->fill(event);
    if(error_test) cout << "test139" << '\n';
    h_common_puppi->fill(event);
    if(error_test) cout << "test140" << '\n';


    topjetCorr->process(event);
    if(error_test) cout << "test141" << '\n';

    event.set(h_pf_tau2_corrector, calculator_chs->PF_tau2(event));
    if(error_test) cout << "test142" << '\n';
    event.set(h_pf_tau3_corrector, calculator_chs->PF_tau3(event));
    if(error_test) cout << "test143" << '\n';

    h_topjetcorrections->fill(event);
    if(error_test) cout << "test144" << '\n';
    h_topjetcorrections_puppi->fill(event);
    if(error_test) cout << "test145" << '\n';

    puppijetCorr->process(event);
    if(error_test) cout << "test146" << '\n';

    event.set(h_pf_tau2_corrector_puppi, calculator_puppi->PF_tau2(event));
    if(error_test) cout << "test147" << '\n';
    event.set(h_pf_tau3_corrector_puppi, calculator_puppi->PF_tau3(event));
    if(error_test) cout << "test148" << '\n';

    h_puppijetcorrections->fill(event);
    if(error_test) cout << "test149" << '\n';
    h_puppijetcorrections_puppi->fill(event);
    if(error_test) cout << "test150" << '\n';

    jetcleaner->process(event);
    if(error_test) cout << "test151" << '\n';
    h_jetcleaner->fill(event);
    if(error_test) cout << "test152" << '\n';
    h_jetcleaner_puppi->fill(event);
    if(error_test) cout << "test153" << '\n';

    muoSR_cleaner->process(event);
    if(error_test) cout << "test154" << '\n';
    h_muoncleaner->fill(event);
    if(error_test) cout << "test155" << '\n';
    h_muoncleaner_puppi->fill(event);
    if(error_test) cout << "test156" << '\n';

    eleSR_cleaner->process(event);
    if(error_test) cout << "test157" << '\n';
    h_elecleaner->fill(event);
    if(error_test) cout << "test158" << '\n';
    h_elecleaner_puppi->fill(event);
    if(error_test) cout << "test159" << '\n';

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    bool elec_is_isolated = false;

    if(channel_ == muon){
      if(!isMC && event.run < 274954) {
        passed_trigger = trigger_sel_A->passes(event);
      }
      else{
        passed_trigger = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
      }
    }
    else if(channel_ == ele){
      if(error_test) cout << "test160" << '\n';
      // only use triggerA and isolation if elec pt < 120
      // for pt > 120 use triggerB || triggerC
      if(!elec_sel_120->passes(event)){
        if(error_test) cout << "test161" << '\n';
        passed_trigger_1 = trigger_sel_A->passes(event);
        if(error_test) cout << "test162" << '\n';
        passed_trigger_2 = elec_sel_triggerA->passes(event);
        if(error_test) cout << "test163" << '\n';
        if(passed_trigger_1 && passed_trigger_2){
          elec_is_isolated = true;
          passed_trigger = true;
        }
      }
      else{
        if(error_test) cout << "test164" << '\n';
        if(isMC)       passed_trigger = (trigger_sel_B->passes(event) || trigger_sel_C->passes(event));
        else if(isEle) passed_trigger = trigger_sel_B->passes(event);
        if(error_test) cout << "test165" << '\n';
      }
    }

    if(error_test) cout << "test166" << '\n';
    passed_pv = pv_sel->passes(event); //at least 1 good primary vertex

    if(error_test) cout << "test167" << '\n';
    passed_nlep1 = nlep1_sel->passes(event); // exactly 1 lep
    if(error_test) cout << "test168" << '\n';

    rectopjetleptoncleaner->process(event); // remove lepton four-vector from topjet four-vector

    if(error_test) cout << "test169" << '\n';
    event.set(h_pf_tau2_lepcleaner, calculator_chs->PF_tau2(event));
    if(error_test) cout << "test170" << '\n';
    event.set(h_pf_tau3_lepcleaner, calculator_chs->PF_tau3(event));
    if(error_test) cout << "test171" << '\n';

    h_tjlc->fill(event);
    if(error_test) cout << "test172" << '\n';
    h_tjlc_puppi->fill(event);
    if(error_test) cout << "test173" << '\n';

    recpuppijetleptoncleaner->process(event);       // remove lepton four-vector from puppi four-vector
    if(error_test) cout << "test174" << '\n';

    event.set(h_pf_tau2_lepcleaner_puppi, calculator_puppi->PF_tau2(event));
    if(error_test) cout << "test175" << '\n';
    event.set(h_pf_tau3_lepcleaner_puppi, calculator_puppi->PF_tau3(event));
    if(error_test) cout << "test176" << '\n';

    h_pjlc->fill(event);
    if(error_test) cout << "test177" << '\n';
    h_pjlc_puppi->fill(event);
    if(error_test) cout << "test178" << '\n';

    topjetcleaner->process(event);
    if(error_test) cout << "test179" << '\n';


    event.set(h_pf_tau2_cleaner, calculator_chs->PF_tau2(event));
    if(error_test) cout << "test180" << '\n';
    event.set(h_pf_tau3_cleaner, calculator_chs->PF_tau3(event));
    if(error_test) cout << "test181" << '\n';

    h_tjc->fill(event);
    if(error_test) cout << "test182" << '\n';
    h_tjc_puppi->fill(event);
    if(error_test) cout << "test183" << '\n';


    puppijetcleaner->process(event);
    if(error_test) cout << "test184" << '\n';

    event.set(h_pf_tau2_cleaner_puppi, calculator_puppi->PF_tau2(event));
    if(error_test) cout << "test185" << '\n';
    event.set(h_pf_tau3_cleaner_puppi, calculator_puppi->PF_tau3(event));
    if(error_test) cout << "test186" << '\n';
    h_pjc->fill(event);
    if(error_test) cout << "test187" << '\n';
    h_pjc_puppi->fill(event);
    if(error_test) cout << "test188" << '\n';

    event.set(h_rec_weight_kin, event.weight);
    if(error_test) cout << "test189" << '\n';

    passed_met = met_sel->passes(event); // MET
    if(error_test) cout << "test190" << '\n';
    passed_pt_lep  = pt_lep_sel->passes(event); // pT(ele/muon)
    if(error_test) cout << "test191" << '\n';

    passed_nlep0 = nlep0_sel->passes(event); // exactly 0 muon/electron
    if(error_test) cout << "test192" << '\n';

    if(elec_is_isolated) passed_twodcut = true; // do not do 2D cut for isolated electrons
    else passed_twodcut = twodcut_sel->passes(event); // TwoDCut
    if(error_test) cout << "test193" << '\n';

    passed_nbtag_medium = nbtag_medium_sel->passes(event); // at least one medium b-tag
    if(error_test) cout << "test194" << '\n';

    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger){
      if(error_test) cout << "test195" << '\n';
      h_trigger->fill(event);
      if(error_test) cout << "test196" << '\n';
      h_trigger_puppi->fill(event);
      if(error_test) cout << "test197" << '\n';
    }
    if(error_test) cout << "test198" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv){
      if(error_test) cout << "test199" << '\n';
      h_pv->fill(event);
      if(error_test) cout << "test200" << '\n';
      h_pv_puppi->fill(event);
      if(error_test) cout << "test201" << '\n';
    }
    if(error_test) cout << "test202" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1){
      if(error_test) cout << "test203" << '\n';
      h_nlep_1->fill(event);
      if(error_test) cout << "test204" << '\n';
      h_nlep_1_puppi->fill(event);
      if(error_test) cout << "test205" << '\n';
    }
    if(error_test) cout << "test206" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met){
      if(error_test) cout << "test207" << '\n';
      h_met->fill(event);
      if(error_test) cout << "test208" << '\n';
      h_met_puppi->fill(event);
      if(error_test) cout << "test209" << '\n';
    }
    if(error_test) cout << "test210" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep){
      if(error_test) cout << "test211" << '\n';
      h_pt_lep->fill(event);
      if(error_test) cout << "test212" << '\n';
      h_pt_lep_puppi->fill(event);
      if(error_test) cout << "test213" << '\n';
    }
    if(error_test) cout << "test214" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0){
      if(error_test) cout << "test215" << '\n';
      h_nlep_0->fill(event);
      if(error_test) cout << "test216" << '\n';
      h_nlep_0_puppi->fill(event);
      if(error_test) cout << "test217" << '\n';
    }
    if(error_test) cout << "test218" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0 && passed_twodcut){
      if(error_test) cout << "test219" << '\n';
      h_twodcut->fill(event);
      if(error_test) cout << "test220" << '\n';
      h_twodcut_puppi->fill(event);
      if(error_test) cout << "test221" << '\n';
      hists_btag_eff->fill(event);
      if(error_test) cout << "test222" << '\n';
    }
    if(error_test) cout << "test223" << '\n';
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0 && passed_twodcut && passed_nbtag_medium){
      if(error_test) cout << "test224" << '\n';
      h_nbtag_medium->fill(event);
      if(error_test) cout << "test225" << '\n';
      h_nbtag_medium_puppi->fill(event);
      if(error_test) cout << "test226" << '\n';
      if(channel_ == muon) passed_mu_rec = true;
      else if(channel_ == ele) passed_ele_rec = true;
      if(error_test) cout << "test227" << '\n';
    }
    if(error_test) cout << "test228" << '\n';

    if((channel_ == muon && !passed_mu_rec && !passed_mu_gen) || (channel_ == ele && !passed_ele_rec && !passed_ele_gen)) return false;

    if(error_test) cout << "test229" << '\n';
    event.set(h_passed_mu_rec, passed_mu_rec);
    if(error_test) cout << "test230" << '\n';
    event.set(h_passed_mu_gen, passed_mu_gen);
    if(error_test) cout << "test231" << '\n';
    event.set(h_passed_ele_rec, passed_ele_rec);
    if(error_test) cout << "test232" << '\n';
    event.set(h_passed_ele_gen, passed_ele_gen);
    if(error_test) cout << "test233" << '\n';
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
