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
    std::unique_ptr<CommonModules>   common;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;
    // Calculator calculator;

    bool isMC, isTTbar;
    std::unique_ptr<TopJetCorrections> topjetCorr;
    std::unique_ptr<PuppiJetCorrections> puppijetCorr;
    bool error_test;

    //declare generator booleans
    bool passed_gen_pre, passed_rec_pre;
    bool passed_gen, passed_nmu_gen, passed_pt_mu_gen;

    //declare reconstruction booleans
    bool passed_rec, passed_trigger, passed_pv, passed_met, passed_nmu, passed_nele, passed_pt_mu, passed_twodcut, passed_nbtag_medium;

    std::unique_ptr<JetCleaner> jetcleaner;
    std::unique_ptr<TopJetCleaner> topjetcleaner, puppijetcleaner;
    std::unique_ptr<BTagMCEfficiencyHists> hists_btag_eff;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    //declare generator selections
    std::unique_ptr<Selection> met_gen, pt_mu_gen;

    //declare reconstruction selections
    std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B, pv_sel, nbtag_medium_sel, twodcut_sel, met_sel, nmu_sel, nele_sel, pt_mu_sel;

    //declare general stuff
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
    std::unique_ptr<AnalysisModule> cleaner, cleaner_sd, rectopjetleptoncleaner, recpuppijetleptoncleaner;
    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> rm_particle;
    JetId Btag_medium;
    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner, gentopjetcleaner_sd;
    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    //declare generator histograms
    std::unique_ptr<Hists> h_gen_pt_mu;

    //declare reconstruction histograms
    std::unique_ptr<Hists> h_test, h_common, h_topjetcorrections, h_puppijetcorrections, h_topjetjer_smearing, h_jetcleaner, h_muoncleaner, h_elecleaner, h_trigger, h_pv, h_nmu, h_tjlc, h_pjlc, h_tjc, h_pjc, h_met, h_pt_mu, h_nele, h_twodcut, h_nbtag_medium;
    std::unique_ptr<Hists> h_test_puppi, h_common_puppi, h_topjetcorrections_puppi, h_puppijetcorrections_puppi, h_topjetjer_smearing_puppi, h_jetcleaner_puppi, h_muoncleaner_puppi, h_elecleaner_puppi, h_trigger_puppi, h_pv_puppi, h_nmu_puppi, h_tjlc_puppi, h_pjlc_puppi, h_tjc_puppi, h_pjc_puppi, h_met_puppi, h_pt_mu_puppi, h_nele_puppi, h_twodcut_puppi, h_nbtag_medium_puppi;

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

    uhh2::Event::Handle<bool> h_passed_gen_pre, h_passed_rec_pre, h_passed_gen, h_passed_rec;
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
    // calculator_gen(ctx, "gentopjets");
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_gen_pre = ctx.get_handle<bool>("h_passed_gen_pre");
    h_passed_rec_pre = ctx.get_handle<bool>("h_passed_rec_pre");
    h_puppi          = ctx.get_handle<std::vector<TopJet>>(puppi_jet_col);
    h_gentopjet      = ctx.get_handle<std::vector<GenTopJet>>(gentopjet_col);
    h_gen_weight_kin = ctx.declare_event_output<double>("h_gen_weight_kin");
    h_rec_weight_kin = ctx.declare_event_output<double>("h_rec_weight_kin");
    h_passed_rec     = ctx.declare_event_output<bool>("h_passed_rec");
    h_passed_gen     = ctx.declare_event_output<bool>("h_passed_gen");

    h_gen_tau2_start = ctx.declare_event_output<double>("h_gen_tau2_start");
    h_gen_tau2_lepcleaner = ctx.declare_event_output<double>("h_gen_tau2_lepcleaner");
    h_gen_tau2_cleaner = ctx.declare_event_output<double>("h_gen_tau2_cleaner");
    h_gen_tau3_start = ctx.declare_event_output<double>("h_gen_tau3_start");
    h_gen_tau3_lepcleaner = ctx.declare_event_output<double>("h_gen_tau3_lepcleaner");
    h_gen_tau3_cleaner = ctx.declare_event_output<double>("h_gen_tau3_cleaner");

    h_pf_tau2_start = ctx.declare_event_output<double>("h_pf_tau2_start");
    h_pf_tau2_start_puppi = ctx.declare_event_output<double>("h_pf_tau2_start_puppi");
    h_pf_tau2_common = ctx.declare_event_output<double>("h_pf_tau2_common");
    h_pf_tau2_common_puppi = ctx.declare_event_output<double>("h_pf_tau2_common_puppi");
    h_pf_tau2_corrector = ctx.declare_event_output<double>("h_pf_tau2_corrector");
    h_pf_tau2_corrector_puppi = ctx.declare_event_output<double>("h_pf_tau2_corrector_puppi");
    h_pf_tau2_lepcleaner = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner");
    h_pf_tau2_lepcleaner_puppi = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner_puppi");
    h_pf_tau2_cleaner = ctx.declare_event_output<double>("h_pf_tau2_cleaner");
    h_pf_tau2_cleaner_puppi = ctx.declare_event_output<double>("h_pf_tau2_cleaner_puppi");

    h_pf_tau3_start = ctx.declare_event_output<double>("h_pf_tau3_start");
    h_pf_tau3_start_puppi = ctx.declare_event_output<double>("h_pf_tau3_start_puppi");
    h_pf_tau3_common = ctx.declare_event_output<double>("h_pf_tau3_common");
    h_pf_tau3_common_puppi = ctx.declare_event_output<double>("h_pf_tau3_common_puppi");
    h_pf_tau3_corrector = ctx.declare_event_output<double>("h_pf_tau3_corrector");
    h_pf_tau3_corrector_puppi = ctx.declare_event_output<double>("h_pf_tau3_corrector_puppi");
    h_pf_tau3_lepcleaner = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner");
    h_pf_tau3_lepcleaner_puppi = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner_puppi");
    h_pf_tau3_cleaner = ctx.declare_event_output<double>("h_pf_tau3_cleaner");
    h_pf_tau3_cleaner_puppi = ctx.declare_event_output<double>("h_pf_tau3_cleaner_puppi");


    common.reset(new CommonModules());
    // lumiweight.reset(new MCLumiWeight(ctx));
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    topjetCorr.reset(new TopJetCorrections());
    topjetCorr->init(ctx);

    puppijetCorr.reset(new PuppiJetCorrections());
    puppijetCorr->init(ctx);

    common->disable_mclumiweight();
    common->switch_jetlepcleaner();
    common->disable_mcpileupreweight();
    common->init(ctx); // always last!

    MuonId muid       = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(55., 2.4));
    ElectronId eleid  = AndId<Electron>(ElectronID_Summer16_medium_noIso, PtEtaCut(55., 2.4));
    Btag_medium       = CSVBTag(CSVBTag::WP_MEDIUM);

    cleaner.reset(new GenTopJetLeptonCleaner(ctx, "genjetsAk8Substructure"));
    cleaner_sd.reset(new GenTopJetLeptonCleaner(ctx));
    gentopjetcleaner.reset(new GenTopJetCleaner(ctx, "genjetsAk8Substructure"));
    gentopjetcleaner_sd.reset(new GenTopJetCleaner(ctx));
    muoSR_cleaner.reset(new MuonCleaner(muid));
    eleSR_cleaner.reset(new ElectronCleaner(eleid));
    jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4));
    rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner());
    recpuppijetleptoncleaner.reset(new RecPuppiJetLeptonCleaner(ctx));
    topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5)));
    puppijetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5), puppi_jet_col));


    trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
    trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");

    isMC    = (ctx.get("dataset_type") == "MC");
    isTTbar = (ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft" || ctx.get("dataset_version") == "TTbar_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph");

    // 2. set up selections

    calculator_chs.reset(new Calculator(ctx, "topjets"));
    calculator_puppi.reset(new Calculator(ctx, puppi_jet_col));
    if(isMC) calculator_gen.reset(new Calculator(ctx, "gentopjets"));
    //setup generator selections
    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
      pt_mu_gen.reset(new GenMuonPtSelection(ctx, 55));
    }
    rm_particle.reset(new ParticleRemover(isMC));
    //setup reconstruction selections
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));

    nmu_sel.reset(new NMuonSelection(1,1));
    met_sel.reset(new METSelection(50,-1));
    pt_mu_sel.reset(new MuonptSelection(55));
    nele_sel.reset(new NElectronSelection(0, 0));
    twodcut_sel.reset(new TwoDCut(0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));


    // 3. Set up Hists classes:
    h_gen_pt_mu.reset(new GenHists(ctx, "gen_pt_mu"));

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
    h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
    h_nmu_puppi.reset(new TopSubstructureRecoHists(ctx, "nmu_puppi", puppi_jet_col));
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
    h_pt_mu.reset(new TopSubstructureRecoHists(ctx, "pt_mu"));
    h_pt_mu_puppi.reset(new TopSubstructureRecoHists(ctx, "pt_mu_puppi", puppi_jet_col));
    h_nele.reset(new TopSubstructureRecoHists(ctx, "nele"));
    h_nele_puppi.reset(new TopSubstructureRecoHists(ctx, "nele_puppi", puppi_jet_col));
    h_twodcut.reset(new TopSubstructureRecoHists(ctx, "twodcut"));
    h_twodcut_puppi.reset(new TopSubstructureRecoHists(ctx, "twodcut_puppi", puppi_jet_col));
    hists_btag_eff.reset(new BTagMCEfficiencyHists(ctx,"BTagMedium", Btag_medium));
    h_nbtag_medium.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium"));
    h_nbtag_medium_puppi.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium_puppi", puppi_jet_col));
  }


  bool KinCutModule::process(Event & event) {
    cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    vector<TopJet> topjets_puppi, topjets_chs;
    vector<GenTopJet> gentopjet;

    if(event.is_valid(h_puppi)){
      topjets_puppi.clear();
      topjets_puppi = event.get(h_puppi);
    }
    for(unsigned int i = 0; i < event.topjets->size(); i++){
      topjets_chs.clear();
      topjets_chs.push_back(event.topjets->at(i));
    }

    if(event.is_valid(h_gentopjet)){
      gentopjet.clear();
      gentopjet = event.get(h_gentopjet);
    }

    event.set(h_gen_weight_kin, event.weight);

    event.set(h_pf_tau2_start, calculator_chs->PF_tau2(event));
    event.set(h_pf_tau2_start_puppi, calculator_puppi->PF_tau2(event));
    event.set(h_pf_tau3_start, calculator_chs->PF_tau3(event));
    event.set(h_pf_tau3_start_puppi, calculator_puppi->PF_tau3(event));

    if(isMC){
      event.set(h_gen_tau2_start, calculator_gen->Gen_tau2(event));
      event.set(h_gen_tau3_start, calculator_gen->Gen_tau3(event));
    }
    else{
      event.set(h_gen_tau2_start, -100);
      event.set(h_gen_tau3_start, -100);
    }
    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen_pre)) passed_gen_pre = event.get(h_passed_gen_pre);
    else passed_gen_pre = false;
    passed_pt_mu_gen    = false;  // passed pt muon gen selection
    passed_gen          = false;  // passed full gen selection
    bool do_tau_lepcleaner = false;
    bool do_tau_cleaner = false;

    if(isTTbar){
      ttgenprod->process(event);
      if(passed_gen_pre){
        cleaner->process(event); // Do this always!
        cleaner_sd->process(event); // Do this always!
        do_tau_lepcleaner = true;
        gentopjetcleaner->process(event);
        gentopjetcleaner_sd->process(event);
        do_tau_cleaner = true;
      }
      passed_pt_mu_gen = pt_mu_gen->passes(event);
      if(passed_gen_pre && passed_pt_mu_gen){
        h_gen_pt_mu->fill(event);
        passed_gen = true;
      }
    }

    if(do_tau_lepcleaner){
      event.set(h_gen_tau2_lepcleaner, calculator_gen->Gen_tau2(event));
      event.set(h_gen_tau3_lepcleaner, calculator_gen->Gen_tau3(event));
    }
    else{
      event.set(h_gen_tau2_lepcleaner, -100);
      event.set(h_gen_tau3_lepcleaner, -100);
    }

    if(do_tau_cleaner){
      event.set(h_gen_tau2_cleaner, calculator_gen->Gen_tau2(event));
      event.set(h_gen_tau3_cleaner, calculator_gen->Gen_tau3(event));
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

    if(event.is_valid(h_passed_rec_pre)) passed_rec_pre = event.get(h_passed_rec_pre);
    else passed_rec_pre = false;
    passed_trigger      = false;
    passed_pv           = false;
    passed_nmu          = false;
    passed_met          = false;
    passed_pt_mu        = false;
    passed_nele         = false;
    passed_twodcut      = false;
    passed_nbtag_medium = false;
    passed_rec          = false;  // passed full rec selection

    bool common_passed = common->process(event);
    h_test->fill(event);
    h_test_puppi->fill(event);
    if(!common_passed) return false;
    for(unsigned int i = 0; i < event.topjets->size(); i++){
      topjets_chs.clear();
      topjets_chs.push_back(event.topjets->at(i));
    }
    event.set(h_pf_tau2_common, calculator_chs->PF_tau2(event));
    event.set(h_pf_tau3_common, calculator_chs->PF_tau3(event));
    if(event.is_valid(h_puppi)){
      topjets_puppi.clear();
      topjets_puppi = event.get(h_puppi);
    }
    event.set(h_pf_tau2_common_puppi, calculator_puppi->PF_tau2(event));
    event.set(h_pf_tau3_common_puppi, calculator_puppi->PF_tau3(event));

    h_common->fill(event);
    h_common_puppi->fill(event);


    topjetCorr->process(event);
    for(unsigned int i = 0; i < event.topjets->size(); i++){
      topjets_chs.clear();
      topjets_chs.push_back(event.topjets->at(i));
    }
    event.set(h_pf_tau2_corrector, calculator_chs->PF_tau2(event));
    event.set(h_pf_tau3_corrector, calculator_chs->PF_tau3(event));

    h_topjetcorrections->fill(event);
    h_topjetcorrections_puppi->fill(event);

    puppijetCorr->process(event);
    if(event.is_valid(h_puppi)){
      topjets_puppi.clear();
      topjets_puppi = event.get(h_puppi);
    }
    event.set(h_pf_tau2_corrector_puppi, calculator_puppi->PF_tau2(event));
    event.set(h_pf_tau3_corrector_puppi, calculator_puppi->PF_tau3(event));

    h_puppijetcorrections->fill(event);
    h_puppijetcorrections_puppi->fill(event);

    jetcleaner->process(event);
    h_jetcleaner->fill(event);
    h_jetcleaner_puppi->fill(event);

    muoSR_cleaner->process(event);
    h_muoncleaner->fill(event);
    h_muoncleaner_puppi->fill(event);

    eleSR_cleaner->process(event);
    h_elecleaner->fill(event);
    h_elecleaner_puppi->fill(event);

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    if(!isMC && event.run < 274954) {
      passed_trigger = trigger_sel_A->passes(event);
    }else{
      passed_trigger = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
    }
    passed_pv = pv_sel->passes(event); //at least 1 good primary vertex
    passed_nmu = nmu_sel->passes(event); // exactly 1 muon

    rectopjetleptoncleaner->process(event); // remove muon four-vector from topjet four-vector
    for(unsigned int i = 0; i < event.topjets->size(); i++){
      topjets_chs.clear();
      topjets_chs.push_back(event.topjets->at(i));
    }
    event.set(h_pf_tau2_lepcleaner, calculator_chs->PF_tau2(event));
    event.set(h_pf_tau3_lepcleaner, calculator_chs->PF_tau3(event));
    h_tjlc->fill(event);
    h_tjlc_puppi->fill(event);


    recpuppijetleptoncleaner->process(event); // remove muon four-vector from topjet four-vector
    if(event.is_valid(h_puppi)){
      topjets_puppi.clear();
      topjets_puppi = event.get(h_puppi);
    }
    event.set(h_pf_tau2_lepcleaner_puppi, calculator_puppi->PF_tau2(event));
    event.set(h_pf_tau3_lepcleaner_puppi, calculator_puppi->PF_tau3(event));

    h_pjlc->fill(event);
    h_pjlc_puppi->fill(event);
    topjetcleaner->process(event);


    for(unsigned int i = 0; i < event.topjets->size(); i++){
      topjets_chs.clear();
      topjets_chs.push_back(event.topjets->at(i));
    }
    event.set(h_pf_tau2_cleaner, calculator_chs->PF_tau2(event));
    event.set(h_pf_tau3_cleaner, calculator_chs->PF_tau3(event));

    h_tjc->fill(event);
    h_tjc_puppi->fill(event);


    puppijetcleaner->process(event);
    if(event.is_valid(h_puppi)){
      topjets_puppi.clear();
      topjets_puppi = event.get(h_puppi);
    }
    event.set(h_pf_tau2_cleaner_puppi, calculator_puppi->PF_tau2(event));
    event.set(h_pf_tau3_cleaner_puppi, calculator_puppi->PF_tau3(event));
    h_pjc->fill(event);
    h_pjc_puppi->fill(event);

    event.set(h_rec_weight_kin, event.weight);

    passed_met = met_sel->passes(event); // MET
    passed_pt_mu = pt_mu_sel->passes(event); // pT(muon)
    passed_nele = nele_sel->passes(event); // exactly 0 electron
    passed_twodcut = twodcut_sel->passes(event); // TwoDCut
    passed_nbtag_medium = nbtag_medium_sel->passes(event); // at least one medium b-tag

    if(passed_rec_pre && passed_trigger){
      h_trigger->fill(event);
      h_trigger_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv){
      h_pv->fill(event);
      h_pv_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu){
      h_nmu->fill(event);
      h_nmu_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met){
      h_met->fill(event);
      h_met_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu){
      h_pt_mu->fill(event);
      h_pt_mu_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele){
      h_nele->fill(event);
      h_nele_puppi->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele && passed_twodcut){
      h_twodcut->fill(event);
      h_twodcut_puppi->fill(event);
      hists_btag_eff->fill(event);
    }
    if(passed_rec_pre && passed_trigger && passed_pv && passed_nmu && passed_met && passed_pt_mu && passed_nele && passed_twodcut && passed_nbtag_medium){
      h_nbtag_medium->fill(event);
      h_nbtag_medium_puppi->fill(event);
      passed_rec = true;
    }
    if(!passed_rec && !passed_gen) return false;

    event.set(h_passed_rec, passed_rec);
    event.set(h_passed_gen, passed_gen);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
