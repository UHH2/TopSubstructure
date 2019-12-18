#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/LumiSelection.h"
#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/EventHists.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/ElectronIds.h"

#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"
#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopJetCorrections.h"
#include "UHH2/TopSubstructure/include/PuppiJetCorrections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"
#include "UHH2/TopSubstructure/include/PuppiJetUtils.h"
#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"

// #include "UHH2/TopSubstructure/include/VariablesCalculator.h"     // need this someday, maybe :) (probably needs still some attention)

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class KinCutModule: public AnalysisModule {
  public:
    explicit KinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    //set enum for muon or electron channel
    enum lepton { muon, ele };
    lepton channel_;

    //declare general stuff
    TString dataset_version_string;
    std::unique_ptr<CommonModules>   common;
    std::unique_ptr<LumiSelection>   lumisel;
    std::unique_ptr<MuonCleaner>     muoSR_cleaner;
    std::unique_ptr<ElectronCleaner> eleSR_cleaner;

    bool isMC, isTTbar;
    std::unique_ptr<TopJetCorrections> topjetCorr;
    std::unique_ptr<PuppiJetCorrections> puppijetCorr;
    bool error_test;

    bool passed_mu_gen_pre, passed_ele_gen_pre,  passed_mu_gen, passed_ele_gen;
    bool do_tau_lepcleaner, do_tau_cleaner, do_cleaner, passed_pt_lep_gen;
    bool passed_trigger, passed_pv, passed_met, passed_nlep0, passed_nlep1, passed_pt_lep, passed_twodcut, passed_nbtag_medium;
    bool passed_mu_rec_pre, passed_ele_rec_pre, passed_mu_rec, passed_ele_rec;

    JetId Btag_medium;
    std::unique_ptr<JetCleaner> jetcleaner_1, jetcleaner_2;
    std::unique_ptr<TopJetCleaner> topjetcleaner, puppijetcleaner;
    std::unique_ptr<GenTopJetCleaner> gentopjetcleaner, gentopjetcleaner_sd;

    std::unique_ptr<AnalysisModule> lumiweight, cleaner, cleaner_sd, rectopjetleptoncleaner, recpuppijetleptoncleaner, ttgenprod;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    //declare generator selections
    std::unique_ptr<Selection> pt_lep_gen;

    //declare reconstruction selections
    std::unique_ptr<Selection> elec_sel_triggerA, trigger_el_A, trigger_el_B, trigger_mu_A, trigger_mu_B, elec_sel_120;  //trigger selections
    std::unique_ptr<Selection> pv_sel, nbtag_medium_sel, twodcut_sel, met_sel, nlep0_sel, nlep1_sel, pt_lep_sel;

    //store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    //declare generator histograms
    std::unique_ptr<Hists> h_gen_pt_lep;

    //declare reconstruction histograms
    std::unique_ptr<Hists> h_common, h_common_puppi;
    std::unique_ptr<Hists> h_pv, h_pv_puppi, h_nlep_1, h_nlep_1_puppi, h_tjc, h_met, h_met_puppi, h_pt_lep, h_pt_lep_puppi, h_nlep_0, h_nlep_0_puppi, h_twodcut, h_twodcut_puppi, h_nbtag_medium, h_nbtag_medium_puppi,  h_trigger, h_trigger_puppi, h_event, h_electron, h_muon, h_jets, h_lumi;

    // declare btag hist
    std::unique_ptr<BTagMCEfficiencyHists> hists_btag_eff;

    //declare handles
    uhh2::Event::Handle<double> h_rec_weight_kin;
    uhh2::Event::Handle<double> h_gen_weight_kin;
    uhh2::Event::Handle<bool> h_passed_mu_gen_pre, h_passed_mu_rec_pre, h_passed_ele_gen_pre, h_passed_ele_rec_pre;
    uhh2::Event::Handle<bool> h_passed_mu_gen, h_passed_mu_rec, h_passed_ele_gen, h_passed_ele_rec;

    // name of puppi jetcollection
    string puppi_jet_col = "jetsAk8PuppiSubstructure_SoftDropPuppi";

    // 4 calculated taus (for PUPPI and CHS):
    // 1. at the start
    // 2. after top-/puppijetcorrector
    // 3. after top-/puppijetleptoncleaner
    // 4. after top-/puppijetcleaner
    // uhh2::Event::Handle<double> h_gen_tau2_start, h_gen_tau2_lepcleaner, h_gen_tau2_cleaner;
    // uhh2::Event::Handle<double> h_gen_tau3_start, h_gen_tau3_lepcleaner, h_gen_tau3_cleaner;

    // uhh2::Event::Handle<double> h_pf_tau2_start, h_pf_tau2_common, h_pf_tau2_corrector, h_pf_tau2_lepcleaner, h_pf_tau2_cleaner;
    // uhh2::Event::Handle<double> h_pf_tau2_start_puppi, h_pf_tau2_common_puppi, h_pf_tau2_corrector_puppi, h_pf_tau2_lepcleaner_puppi, h_pf_tau2_cleaner_puppi;
    // uhh2::Event::Handle<double> h_pf_tau3_start, h_pf_tau3_common, h_pf_tau3_corrector, h_pf_tau3_lepcleaner, h_pf_tau3_cleaner;
    // uhh2::Event::Handle<double> h_pf_tau3_start_puppi, h_pf_tau3_common_puppi, h_pf_tau3_corrector_puppi, h_pf_tau3_lepcleaner_puppi, h_pf_tau3_cleaner_puppi;1


    // std::unique_ptr<Calculator> calculator_chs;
    // std::unique_ptr<Calculator> calculator_puppi;
    // std::unique_ptr<Calculator> calculator_gen;

  };


  KinCutModule::KinCutModule(Context & ctx){
    error_test = false;
    isMC    = (ctx.get("dataset_type") == "MC");

    const std::string& channel = ctx.get("channel", ""); //define Channel
    if     (channel == "muon") channel_ = muon;
    else if(channel == "ele")  channel_ = ele;
    else {
      std::string log("KinCutModule::KinCutModule -- ");
      log += "invalid argument for 'channel' key in xml file (must be 'muon' or 'ele'): \""+channel+"\"";
      throw std::runtime_error(log);
    }

    dataset_version_string = ctx.get("dataset_version");
    isTTbar = dataset_version_string.Contains("TTbar");

    // 1. setup other modules. CommonModules and the JetCleaner:
    h_passed_mu_gen_pre   = ctx.get_handle<bool>("h_passed_mu_gen_pre");
    h_passed_ele_gen_pre  = ctx.get_handle<bool>("h_passed_ele_gen_pre");
    h_passed_mu_rec_pre   = ctx.get_handle<bool>("h_passed_mu_rec_pre");
    h_passed_ele_rec_pre  = ctx.get_handle<bool>("h_passed_ele_rec_pre");

    h_gen_weight_kin = ctx.declare_event_output<double>("h_gen_weight_kin");
    h_rec_weight_kin = ctx.declare_event_output<double>("h_rec_weight_kin");
    h_passed_mu_gen  = ctx.declare_event_output<bool>("h_passed_mu_gen");
    h_passed_mu_rec  = ctx.declare_event_output<bool>("h_passed_mu_rec");
    h_passed_ele_gen = ctx.declare_event_output<bool>("h_passed_ele_gen");
    h_passed_ele_rec = ctx.declare_event_output<bool>("h_passed_ele_rec");

    // h_gen_tau2_start      = ctx.declare_event_output<double>("h_gen_tau2_start");
    // h_gen_tau2_lepcleaner = ctx.declare_event_output<double>("h_gen_tau2_lepcleaner");
    // h_gen_tau2_cleaner    = ctx.declare_event_output<double>("h_gen_tau2_cleaner");
    // h_gen_tau3_start      = ctx.declare_event_output<double>("h_gen_tau3_start");
    // h_gen_tau3_lepcleaner = ctx.declare_event_output<double>("h_gen_tau3_lepcleaner");
    // h_gen_tau3_cleaner    = ctx.declare_event_output<double>("h_gen_tau3_cleaner");

    // h_pf_tau2_start             = ctx.declare_event_output<double>("h_pf_tau2_start");
    // h_pf_tau2_start_puppi       = ctx.declare_event_output<double>("h_pf_tau2_start_puppi");
    // h_pf_tau2_common            = ctx.declare_event_output<double>("h_pf_tau2_common");
    // h_pf_tau2_common_puppi      = ctx.declare_event_output<double>("h_pf_tau2_common_puppi");
    // h_pf_tau2_corrector         = ctx.declare_event_output<double>("h_pf_tau2_corrector");
    // h_pf_tau2_corrector_puppi   = ctx.declare_event_output<double>("h_pf_tau2_corrector_puppi");
    // h_pf_tau2_lepcleaner        = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner");
    // h_pf_tau2_lepcleaner_puppi  = ctx.declare_event_output<double>("h_pf_tau2_lepcleaner_puppi");
    // h_pf_tau2_cleaner           = ctx.declare_event_output<double>("h_pf_tau2_cleaner");
    // h_pf_tau2_cleaner_puppi     = ctx.declare_event_output<double>("h_pf_tau2_cleaner_puppi");
    //
    // h_pf_tau3_start             = ctx.declare_event_output<double>("h_pf_tau3_start");
    // h_pf_tau3_start_puppi       = ctx.declare_event_output<double>("h_pf_tau3_start_puppi");
    // h_pf_tau3_common            = ctx.declare_event_output<double>("h_pf_tau3_common");
    // h_pf_tau3_common_puppi      = ctx.declare_event_output<double>("h_pf_tau3_common_puppi");
    // h_pf_tau3_corrector         = ctx.declare_event_output<double>("h_pf_tau3_corrector");
    // h_pf_tau3_corrector_puppi   = ctx.declare_event_output<double>("h_pf_tau3_corrector_puppi");
    // h_pf_tau3_lepcleaner        = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner");
    // h_pf_tau3_lepcleaner_puppi  = ctx.declare_event_output<double>("h_pf_tau3_lepcleaner_puppi");
    // h_pf_tau3_cleaner           = ctx.declare_event_output<double>("h_pf_tau3_cleaner");
    // h_pf_tau3_cleaner_puppi     = ctx.declare_event_output<double>("h_pf_tau3_cleaner_puppi");

    common.reset(new CommonModules());
    // TODO: configure common here, e.g. by
    // calling common->set_*_id or common->disable_*

    topjetCorr.reset(new TopJetCorrections());
    topjetCorr->init(ctx);

    puppijetCorr.reset(new PuppiJetCorrections());
    puppijetCorr->init(ctx);

    common->disable_mclumiweight();
    lumiweight.reset(new MCLumiWeight(ctx));
    common->switch_jetlepcleaner();
    common->disable_mcpileupreweight();
    lumisel.reset(new LumiSelection(ctx));
    common->init(ctx); // always last!

    // set muon id
    MuonId muid  = AndId<Muon>(MuonID(Muon::Tight), PtEtaCut(55., 2.4));

    // this is only used for cleaner and electron veto
    ElectronId eleid_noiso55  = AndId<Electron>(ElectronID_Summer16_tight_noIso, PtEtaSCCut(55., 2.4));
    // this is used to decide which ele trigger is used
    ElectronId eleid_noiso120 = AndId<Electron>(PtEtaSCCut(120., 2.4), ElectronID_Summer16_tight_noIso);
    // this is used in combination with iso trigger
    ElectronId eleid_iso55    = AndId<Electron>(PtEtaSCCut(55., 2.4), ElectronID_Summer16_tight);

    Btag_medium = CSVBTag(CSVBTag::WP_MEDIUM);

    muoSR_cleaner.reset(new MuonCleaner(muid));
    eleSR_cleaner.reset(new ElectronCleaner(eleid_noiso55));

    jetcleaner_1.reset(new JetCleaner(ctx, 15., 3.0));
    jetcleaner_2.reset(new JetCleaner(ctx, 30., 2.4));

    if(channel_ == muon){
      rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner(0));
      recpuppijetleptoncleaner.reset(new RecPuppiJetLeptonCleaner(ctx, 0));
    }
    else if(channel_ == ele){
      rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner(1));
      recpuppijetleptoncleaner.reset(new RecPuppiJetLeptonCleaner(ctx, 1));
    }
    topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.4)));
    puppijetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.4), puppi_jet_col));

    // 2. set up selections
    // calculator_chs.reset(new Calculator(ctx, "topjets"));
    // calculator_puppi.reset(new Calculator(ctx, puppi_jet_col));
    // calculator_gen.reset(new Calculator(ctx, "gentopjets"));

    //setup generator selections
    if(isTTbar){
      cleaner.reset(new GenTopJetLeptonCleaner(ctx, "genjetsAk8Substructure"));
      cleaner_sd.reset(new GenTopJetLeptonCleaner(ctx));
      gentopjetcleaner.reset(new GenTopJetCleaner(ctx, "genjetsAk8Substructure"));
      gentopjetcleaner_sd.reset(new GenTopJetCleaner(ctx));
      ttgenprod.reset(new TTbarGenProducer(ctx, "ttbargen", false));

      if(channel_ == muon) pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 0, 55));
      if(channel_ == ele)  pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 1, 55));
    }

    //setup reconstruction selections
    pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));

    if(channel_ == ele){
      nlep0_sel.reset(new NMuonSelection(0, 0, muid));
      nlep1_sel.reset(new NElectronSelection(1, 1, eleid_noiso55));
      pt_lep_sel.reset(new LeptonPtSelection(1, 55));
    }
    else if (channel_ == muon){
      nlep1_sel.reset(new NMuonSelection(1, 1, muid));
      nlep0_sel.reset(new NElectronSelection(0, 0, eleid_noiso55));
      pt_lep_sel.reset(new LeptonPtSelection(0, 55));
    }

    met_sel.reset(new METSelection(50,-1));

    trigger_mu_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
    trigger_mu_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");
    trigger_el_A = uhh2::make_unique<TriggerSelection>("HLT_Ele27_WPTight_Gsf_v*");
    trigger_el_B = uhh2::make_unique<TriggerSelection>("HLT_Ele115_CaloIdVT_GsfTrkIdT_v*");
    elec_sel_triggerA.reset(new NElectronSelection(1, 1, eleid_iso55));
    elec_sel_120.reset(new NElectronSelection(1, 1, eleid_noiso120));

    if(channel_ == muon)     twodcut_sel.reset(new TwoDCut(0, 0.4, 40));
    else if(channel_ == ele) twodcut_sel.reset(new TwoDCut(1, 0.4, 40));
    nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));


    // 3. Set up Hists classes:
    h_gen_pt_lep.reset(new GenHists(ctx, "gen_pt_lep"));

    h_common.reset(new TopSubstructureRecoHists(ctx, "common"));
    h_common_puppi.reset(new TopSubstructureRecoHists(ctx, "common_puppi", puppi_jet_col));

    h_trigger.reset(new TopSubstructureRecoHists(ctx, "trigger"));
    h_trigger_puppi.reset(new TopSubstructureRecoHists(ctx, "trigger_puppi", puppi_jet_col));
    h_pv.reset(new TopSubstructureRecoHists(ctx, "pv"));
    h_pv_puppi.reset(new TopSubstructureRecoHists(ctx, "pv_puppi", puppi_jet_col));
    h_nlep_1.reset(new TopSubstructureRecoHists(ctx, "nlep_1"));
    h_nlep_1_puppi.reset(new TopSubstructureRecoHists(ctx, "nlep_1_puppi"));
    h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
    h_met_puppi.reset(new TopSubstructureRecoHists(ctx, "met_puppi", puppi_jet_col));
    h_pt_lep.reset(new TopSubstructureRecoHists(ctx, "pt_lep"));
    h_pt_lep_puppi.reset(new TopSubstructureRecoHists(ctx, "pt_elp_puppi", puppi_jet_col));
    h_nlep_0.reset(new TopSubstructureRecoHists(ctx, "nlep_0"));
    h_nlep_0_puppi.reset(new TopSubstructureRecoHists(ctx, "nlep_0_puppi", puppi_jet_col));
    h_twodcut.reset(new TopSubstructureRecoHists(ctx, "twodcut"));
    h_twodcut_puppi.reset(new TopSubstructureRecoHists(ctx, "twodcut_puppi", puppi_jet_col));
    hists_btag_eff.reset(new BTagMCEfficiencyHists(ctx,"BTagMedium", Btag_medium));
    h_nbtag_medium.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium"));
    h_nbtag_medium_puppi.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium_puppi", puppi_jet_col));
    h_muon.reset(new MuonHists(ctx, "Muon_Hist",false));
    h_electron.reset(new ElectronHists(ctx, "Electron_Hist",false));
    h_jets.reset(new JetHists(ctx, "Jet_Hist"));
    h_lumi.reset(new LuminosityHists(ctx, "Lumi_Hist"));
    h_event.reset(new EventHists(ctx, "Event_Hist"));
  }


  bool KinCutModule::process(Event & event) {
    if(error_test) cout << "KinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    lumiweight->process(event);
    event.set(h_gen_weight_kin, event.weight);

    // event.set(h_pf_tau2_start, calculator_chs->PF_tau2(event));
    // event.set(h_pf_tau2_start_puppi, calculator_puppi->PF_tau2(event));
    // event.set(h_pf_tau3_start, calculator_chs->PF_tau3(event));
    // event.set(h_pf_tau3_start_puppi, calculator_puppi->PF_tau3(event));

    // if(isMC){
    //   event.set(h_gen_tau2_start, calculator_gen->Gen_tau2(event));
    //   event.set(h_gen_tau3_start, calculator_gen->Gen_tau3(event));
    // }
    // else{
    //   event.set(h_gen_tau2_start, -100);
    //   event.set(h_gen_tau3_start, -100);
    // }

    // set default values for all generator bools to false (or get bools from preselection)
    if(event.is_valid(h_passed_mu_gen_pre) && channel_ == muon) passed_mu_gen_pre = event.get(h_passed_mu_gen_pre);
    else passed_mu_gen_pre  = false;
    if(event.is_valid(h_passed_ele_gen_pre) && channel_ == ele) passed_ele_gen_pre = event.get(h_passed_ele_gen_pre);
    else passed_ele_gen_pre = false;
    passed_pt_lep_gen       = false;
    passed_mu_gen           = false;
    passed_ele_gen          = false;
    do_tau_lepcleaner       = false;
    do_tau_cleaner          = false;
    do_cleaner              = false;

    if(isTTbar){
      ttgenprod->process(event);
      if((channel_ == muon && passed_mu_gen_pre) || (channel_ == ele && passed_ele_gen_pre)){
        cleaner->process(event);    // Always need to do it! (no softdrop)
        cleaner_sd->process(event); // Always need to do it! (softdrop)
        do_tau_lepcleaner = true;

        // if(do_tau_lepcleaner){
        //   event.set(h_gen_tau2_lepcleaner, calculator_gen->Gen_tau2(event));
        //   event.set(h_gen_tau3_lepcleaner, calculator_gen->Gen_tau3(event));
        // }
        gentopjetcleaner->process(event);
        gentopjetcleaner_sd->process(event);
        do_tau_cleaner = true;
      }
      if(do_tau_lepcleaner && do_tau_cleaner) do_cleaner = true;

      passed_pt_lep_gen = pt_lep_gen->passes(event);
      if((passed_mu_gen_pre || passed_ele_gen_pre) && do_cleaner && passed_pt_lep_gen){
        h_gen_pt_lep->fill(event);
        if(channel_ == muon)     passed_mu_gen = true;
        else if(channel_ == ele) passed_ele_gen = true;
      }
    }

    // if(!do_tau_lepcleaner){
    //   event.set(h_gen_tau2_lepcleaner, -100);
    //   event.set(h_gen_tau3_lepcleaner, -100);
    // }
    // if(do_tau_cleaner){
    //   event.set(h_gen_tau2_cleaner, calculator_gen->Gen_tau2(event));
    //   event.set(h_gen_tau3_cleaner, calculator_gen->Gen_tau3(event));
    // }
    // else{
    //   event.set(h_gen_tau2_cleaner, -100);
    //   event.set(h_gen_tau3_cleaner, -100);
    // }

    /*
    ██████  ███████  ██████  ██████
    ██   ██ ██      ██      ██    ██
    ██████  █████   ██      ██    ██
    ██   ██ ██      ██      ██    ██
    ██   ██ ███████  ██████  ██████
    */
    // set default values of all reconstruction bools to false (or get bools from the preselection)
    bool passed_trigger_1 = false;
    bool passed_trigger_2 = false;

    if(event.is_valid(h_passed_mu_rec_pre) && channel_ == muon) passed_mu_rec_pre = event.get(h_passed_mu_rec_pre);
    else passed_mu_rec_pre  = false;
    if(event.is_valid(h_passed_ele_rec_pre) && channel_ == ele) passed_ele_rec_pre = event.get(h_passed_ele_rec_pre);
    else passed_ele_rec_pre = false;
    passed_trigger          = false;
    passed_pv               = false;
    passed_nlep1            = false;
    passed_met              = false;
    passed_pt_lep           = false;
    passed_nlep0            = false;
    passed_twodcut          = false;
    passed_nbtag_medium     = false;
    passed_mu_rec           = false;
    passed_ele_rec          = false;

    muoSR_cleaner->process(event);
    if(event.muons->size() > 0) sort_by_pt<Muon>(*event.muons);

    eleSR_cleaner->process(event);
    if(event.electrons->size() > 0) sort_by_pt<Electron>(*event.electrons);


    if(!isMC && !lumisel->passes(event)) return false;
    bool common_passed = common->process(event);
    if(!common_passed) return false;
    if(passed_mu_rec_pre || passed_ele_rec_pre) h_common->fill(event);
    if(passed_mu_rec_pre || passed_ele_rec_pre) h_common_puppi->fill(event);

    jetcleaner_1->process(event);
    sort_by_pt<Jet>(*event.jets);
    // event.set(h_pf_tau2_common, calculator_chs->PF_tau2(event));
    // event.set(h_pf_tau3_common, calculator_chs->PF_tau3(event));
    // event.set(h_pf_tau2_common_puppi, calculator_puppi->PF_tau2(event));
    // event.set(h_pf_tau3_common_puppi, calculator_puppi->PF_tau3(event));

    topjetCorr->process(event);
    // event.set(h_pf_tau2_corrector, calculator_chs->PF_tau2(event));
    // event.set(h_pf_tau3_corrector, calculator_chs->PF_tau3(event));

    puppijetCorr->process(event);
    // event.set(h_pf_tau2_corrector_puppi, calculator_puppi->PF_tau2(event));
    // event.set(h_pf_tau3_corrector_puppi, calculator_puppi->PF_tau3(event));

    /* *********** Trigger *********** */
    // for DATA until run 274954 -> use only Trigger A
    // for MC and DATA from 274954 -> use "A || B"
    bool elec_is_isolated = false;
    if(channel_ == muon){
      if(!isMC && event.run < 274954) {
        if(!trigger_mu_A->passes(event)) passed_trigger = false;
        else passed_trigger = true;
      }
      else{
        if( !(trigger_mu_A->passes(event) || trigger_mu_B->passes(event)) ) passed_trigger = false;
        else passed_trigger = true;
      }
    }
    else if(channel_ == ele){
      // only use triggerA and isolation if elec pt < 120
      // for pt > 120 use triggerB || triggerC; use triggerC only for photon stream
      if(!elec_sel_120->passes(event)){
        passed_trigger_1 = trigger_el_A->passes(event);
        passed_trigger_2 = elec_sel_triggerA->passes(event);
        if(passed_trigger_1 && passed_trigger_2){
          elec_is_isolated = true;
          passed_trigger = true;
        }
      }
      else{
        if( !(trigger_el_B->passes(event)) ) passed_trigger = false;
        else passed_trigger = true;
      }
    }

    passed_pv = pv_sel->passes(event); //at least 1 good primary vertex

    passed_nlep1 = nlep1_sel->passes(event); // exactly 1 electron/muon
    rectopjetleptoncleaner->process(event); // remove lepton four-vector from topjet four-vector
    // event.set(h_pf_tau2_lepcleaner, calculator_chs->PF_tau2(event));
    // event.set(h_pf_tau3_lepcleaner, calculator_chs->PF_tau3(event));

    recpuppijetleptoncleaner->process(event);       // remove lepton four-vector from puppi four-vector
    // event.set(h_pf_tau2_lepcleaner_puppi, calculator_puppi->PF_tau2(event));
    // event.set(h_pf_tau3_lepcleaner_puppi, calculator_puppi->PF_tau3(event));

    topjetcleaner->process(event);
    // event.set(h_pf_tau2_cleaner, calculator_chs->PF_tau2(event));
    // event.set(h_pf_tau3_cleaner, calculator_chs->PF_tau3(event));

    puppijetcleaner->process(event);
    // event.set(h_pf_tau2_cleaner_puppi, calculator_puppi->PF_tau2(event));
    // event.set(h_pf_tau3_cleaner_puppi, calculator_puppi->PF_tau3(event));

    event.set(h_rec_weight_kin, event.weight);
    passed_nlep0 = nlep0_sel->passes(event); // exactly 0 muon/electron
    passed_met    = met_sel->passes(event); // MET
    passed_pt_lep = pt_lep_sel->passes(event); // pT(ele/muon)

    if(channel_ == ele && elec_is_isolated)  passed_twodcut = true; // do not do 2D cut for isolated electrons
    else passed_twodcut = twodcut_sel->passes(event); // TwoDCut

    jetcleaner_2->process(event);
    sort_by_pt<Jet>(*event.jets);

    passed_nbtag_medium = nbtag_medium_sel->passes(event); // at least one medium b-tag

    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger){
      h_trigger->fill(event);
      h_trigger_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv){
      h_pv->fill(event);
      h_pv_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1){
      h_nlep_1->fill(event);
      h_nlep_1_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met){
      h_met->fill(event);
      h_met_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep){
      h_pt_lep->fill(event);
      h_pt_lep_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0){
      h_nlep_0->fill(event);
      h_nlep_0_puppi->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0 && passed_twodcut){
      h_twodcut->fill(event);
      h_twodcut_puppi->fill(event);
      hists_btag_eff->fill(event);
    }
    if((passed_mu_rec_pre || passed_ele_rec_pre) && passed_trigger && passed_pv && passed_nlep1 && passed_met && passed_pt_lep && passed_nlep0 && passed_twodcut && passed_nbtag_medium){
      h_nbtag_medium->fill(event);
      h_nbtag_medium_puppi->fill(event);
      if(channel_ == muon) passed_mu_rec = true;
      else if(channel_ == ele) passed_ele_rec = true;
      h_muon->fill(event);
      h_electron->fill(event);
      h_jets->fill(event);
      h_lumi->fill(event);
      h_event->fill(event);
    }

    if((channel_ == muon && !passed_mu_rec && !passed_mu_gen) || (channel_ == ele && !passed_ele_rec && !passed_ele_gen)) return false;

    event.set(h_passed_mu_rec, passed_mu_rec);
    event.set(h_passed_mu_gen, passed_mu_gen);
    event.set(h_passed_ele_rec, passed_ele_rec);
    event.set(h_passed_ele_gen, passed_ele_gen);

    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(KinCutModule)

}
