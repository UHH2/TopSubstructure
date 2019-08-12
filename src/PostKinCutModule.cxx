#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/EventHists.h"
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

#include "UHH2/TopSubstructure/include/ElecTriggerSF.h"
#include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"
#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"
#include "UHH2/TopSubstructure/include/PuppiJetUtils.h"
#include "UHH2/TopSubstructure/include/PuppiJetCorrections.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class PostKinCutModule: public AnalysisModule {
  public:
    explicit PostKinCutModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    enum lepton { muon, ele };
    lepton channel_;
    // declare MC bools
    bool isTTbar, isMC;

    // declare bools for KinCut results
    bool passed_rec_kin, passed_gen_kin;

    // declare bool for generator level matching
    bool matched_gen;
    bool matched_gen_sd;

    // declare bools for final selection
    bool passed_lep_gen_final,       passed_gen_final_sd;
    bool passed_rec_final,       passed_rec_final_sd;
    bool passed_rec_final_puppi, passed_rec_final_puppi_sd;
    bool passed_gen_final;

    // declare bools for each Selection step:
    // for generator level
    bool passed_gen_pt_lep, passed_gen_ntopjet, passed_gen_pt_topjet, passed_gen_dr, passed_gen_mass;
    bool passed_gen_ntopjet_sd, passed_gen_pt_topjet_sd, passed_gen_dr_sd, passed_gen_mass_sd;

    bool passed_gen_pt_lep_sideband,    passed_gen_pt_topjet_sideband,    passed_gen_dr_sideband,    passed_gen_mass_sideband;
    bool passed_gen_pt_lep_sideband_sd, passed_gen_pt_topjet_sideband_sd, passed_gen_dr_sideband_sd, passed_gen_mass_sideband_sd;

    // for CHS Jets w/ and w/o SoftDrop
    bool passed_rec_pt_lep, passed_rec_ntopjet, passed_rec_pt_topjet, passed_rec_dr, passed_rec_mass;
    bool passed_rec_pt_topjet_sd, passed_rec_dr_sd, passed_rec_mass_sd;

    bool passed_rec_pt_lep_sideband,    passed_rec_pt_topjet_sideband,    passed_rec_dr_sideband,    passed_rec_mass_sideband;
    bool passed_rec_pt_lep_sideband_sd, passed_rec_pt_topjet_sideband_sd, passed_rec_dr_sideband_sd, passed_rec_mass_sideband_sd;

    // for PUPPI Jets with and without SoftDrop
    bool passed_rec_ntopjet_puppi, passed_rec_pt_topjet_puppi, passed_rec_dr_puppi, passed_rec_mass_puppi;
    bool passed_rec_pt_topjet_puppi_sd, passed_rec_dr_puppi_sd, passed_rec_mass_puppi_sd;

    bool passed_rec_pt_lep_sideband_puppi,    passed_rec_pt_topjet_sideband_puppi,    passed_rec_dr_sideband_puppi,    passed_rec_mass_sideband_puppi;
    bool passed_rec_pt_lep_sideband_puppi_sd, passed_rec_pt_topjet_sideband_puppi_sd, passed_rec_dr_sideband_puppi_sd, passed_rec_mass_sideband_puppi_sd;

    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
    // Selections on generator level
    std::unique_ptr<Selection> pt_lep_gen, ntopjet2_gen, pt_topjet_gen, dr_gen, mass_gen, genmatching;
    std::unique_ptr<Selection> ntopjet2_gen_sd, pt_topjet_gen_sd, dr_gen_sd, mass_gen_sd, genmatching_sd;

    std::unique_ptr<Selection> dr_gen_sideband, dr_gen_sideband_sd, pt_topjet_gen_sideband, pt_topjet_gen_sideband_sd, pt_lep_gen_sideband;

    std::unique_ptr<Selection> gen_mass_lower, gen_mass_higher;
    std::unique_ptr<Selection> gen_mass_lower_sd, gen_mass_higher_sd;

    // Selections for CHS Jets w/ and w/o SoftDrop
    std::unique_ptr<Selection> pt_lep_sel, ntopjet2_sel, pt_topjet_sel, dr_sel, mass_sel;
    std::unique_ptr<Selection> pt_topjet_sel_sd, dr_sel_sd, mass_sel_sd;

    std::unique_ptr<Selection> dr_rec_sideband, pt_topjet_rec_sideband, pt_lep_rec_sideband;
    std::unique_ptr<Selection> dr_rec_sideband_sd, pt_topjet_rec_sideband_sd, pt_lep_rec_sideband_sd;

    std::unique_ptr<Selection> mass_lower, mass_higher;
    std::unique_ptr<Selection> mass_lower_sd, mass_higher_sd;

    // Selections for PUPPI Jets w/ and w/o SoftDrop
    std::unique_ptr<Selection> ntopjet2_sel_puppi, pt_topjet_sel_puppi, dr_sel_puppi, mass_sel_puppi;
    std::unique_ptr<Selection> pt_topjet_sel_puppi_sd, dr_sel_puppi_sd, mass_sel_puppi_sd;

    std::unique_ptr<Selection> dr_rec_sideband_puppi, pt_topjet_rec_sideband_puppi;
    std::unique_ptr<Selection> dr_rec_sideband_puppi_sd, pt_topjet_rec_sideband_puppi_sd;

    std::unique_ptr<Selection> mass_lower_puppi, mass_higher_puppi;
    std::unique_ptr<Selection> mass_lower_puppi_sd, mass_higher_puppi_sd;

    // declare Lumiweight, ScaleVariations & PileUp reweighting
    std::unique_ptr<AnalysisModule> scale_variation;
    std::unique_ptr<AnalysisModule> PUreweight, lumiweight, sf_btag, muo_tight_noniso_SF, muo_trigger_SF, ele_trigger_SF, ele_reco_SF, ele_id_SF;

    // declare TTbar Producer
    std::unique_ptr<AnalysisModule> ttgenprod;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    // declare Generator level Hists
    std::unique_ptr<Hists> h_gen_pt_lep, h_gen_pt_lep_matched, h_gen_pt_lep_unmatched;
    std::unique_ptr<Hists> h_gen_ntopjet2, h_gen_ntopjet2_matched, h_gen_ntopjet2_unmatched;
    std::unique_ptr<Hists> h_gen_ntopjet2_sd, h_gen_ntopjet2_sd_matched, h_gen_ntopjet2_sd_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet, h_gen_pt_topjet_matched, h_gen_pt_topjet_unmatched;
    std::unique_ptr<Hists> h_gen_pt_topjet_sd, h_gen_pt_topjet_sd_matched, h_gen_pt_topjet_sd_unmatched;
    std::unique_ptr<Hists> h_gen_dr, h_gen_dr_matched, h_gen_dr_unmatched;
    std::unique_ptr<Hists> h_gen_dr_sd, h_gen_dr_sd_matched, h_gen_dr_sd_unmatched;
    std::unique_ptr<Hists> h_gen_mass, h_gen_mass_matched, h_gen_mass_unmatched;
    std::unique_ptr<Hists> h_gen_mass_sd, h_gen_mass_sd_matched, h_gen_mass_sd_unmatched;

    std::unique_ptr<Hists> h_gen_mass_lower, h_gen_mass_lower_matched, h_gen_mass_lower_unmatched;
    std::unique_ptr<Hists> h_gen_mass_higher, h_gen_mass_higher_matched, h_gen_mass_higher_unmatched;

    std::unique_ptr<Hists> h_gen_mass_lower_sd, h_gen_mass_lower_sd_matched, h_gen_mass_lower_sd_unmatched;
    std::unique_ptr<Hists> h_gen_mass_higher_sd, h_gen_mass_higher_sd_matched, h_gen_mass_higher_sd_unmatched;

    std::unique_ptr<Hists> h_gen_pt_lep_sideband,    h_gen_pt_topjet_sideband,    h_gen_dr_sideband,    h_gen_mass_sideband;
    std::unique_ptr<Hists> h_gen_pt_lep_sideband_sd, h_gen_pt_topjet_sideband_sd, h_gen_dr_sideband_sd, h_gen_mass_sideband_sd;
    std::unique_ptr<Hists> h_passedgen_rec, h_passedgen_rec_sd;

    // declare CHS Jets Hists w/ and w/o SoftDroph_start
    std::unique_ptr<Hists> h_start, h_pureweight, h_sfbtag, h_leptontrigger, h_leptontight, h_elereco_sf;
    std::unique_ptr<Hists> h_pt_lep, h_ntopjet2, h_pt_topjet, h_dr, h_mass;
    std::unique_ptr<Hists> h_ntopjet2_sd, h_pt_topjet_sd, h_dr_sd, h_mass_sd;

    std::unique_ptr<Hists> h_mass_lower,    h_mass_higher;
    std::unique_ptr<Hists> h_mass_lower_sd, h_mass_higher_sd;

    std::unique_ptr<Hists> h_passedrec_gen,    h_passedrec_gen_puppi;
    std::unique_ptr<Hists> h_passedrec_gen_sd, h_passedrec_gen_puppi_sd;

    std::unique_ptr<Hists> h_rec_pt_topjet_sideband, h_rec_dr_sideband, h_rec_mass_sideband, h_rec_pt_lep_sideband;
    std::unique_ptr<Hists> h_rec_pt_lep_sideband_sd, h_rec_pt_topjet_sideband_sd, h_rec_dr_sideband_sd, h_rec_mass_sideband_sd;

    // declare PUPPI Jets Hists w/ and w/o SoftDrop
    std::unique_ptr<Hists> h_ntopjet2_puppi, h_pt_topjet_puppi, h_dr_puppi, h_mass_puppi;
    std::unique_ptr<Hists> h_ntopjet2_puppi_sd, h_pt_topjet_puppi_sd, h_dr_puppi_sd, h_mass_puppi_sd;

    std::unique_ptr<Hists> h_mass_lower_puppi,    h_mass_higher_puppi;
    std::unique_ptr<Hists> h_mass_lower_puppi_sd, h_mass_higher_puppi_sd;

    std::unique_ptr<Hists> h_rec_pt_lep_sideband_puppi,    h_rec_pt_topjet_sideband_puppi,    h_rec_dr_sideband_puppi,    h_rec_mass_sideband_puppi;
    std::unique_ptr<Hists> h_rec_pt_lep_sideband_puppi_sd, h_rec_pt_topjet_sideband_puppi_sd, h_rec_dr_sideband_puppi_sd, h_rec_mass_sideband_puppi_sd;
    std::unique_ptr<Hists> h_event;

    // declare Handles for KinCut weights
    uhh2::Event::Handle<double> h_rec_weight_kin;
    uhh2::Event::Handle<double> h_gen_weight_kin;

    // declare Handles for final weights
    uhh2::Event::Handle<double> h_rec_weight;
    uhh2::Event::Handle<double> h_gen_weight;
    uhh2::Event::Handle<bool> h_passed_gen_kin, h_passed_rec_kin;

    // declare handles for KinCut results
    uhh2::Event::Handle<bool> h_passed_lep_gen, h_passed_lep_rec, h_passed_ele_gen, h_passed_ele_rec;

    // declare Handles for Final Selection (generator level)
    uhh2::Event::Handle<bool> h_passed_gen_final, h_passed_gen_final_sd;
    uhh2::Event::Handle<bool> h_passed_gen_pt_lep_sideband, h_passed_gen_pt_topjet_sideband, h_passed_gen_dr_sideband, h_passed_gen_mass_sideband;
    uhh2::Event::Handle<bool> h_passed_gen_pt_lep_sideband_sd, h_passed_gen_dr_sideband_sd, h_passed_gen_mass_sideband_sd, h_passed_gen_pt_topjet_sideband_sd;

    // declare Handles for Final Selection (CHS Jets w/ and w/o SoftDrop)
    uhh2::Event::Handle<bool> h_passed_rec_final, h_passed_rec_final_sd;
    uhh2::Event::Handle<bool> h_passed_rec_pt_lep_sideband,    h_passed_rec_pt_topjet_sideband,    h_passed_rec_dr_sideband,    h_passed_rec_mass_sideband;
    uhh2::Event::Handle<bool> h_passed_rec_pt_lep_sideband_sd, h_passed_rec_pt_topjet_sideband_sd, h_passed_rec_dr_sideband_sd, h_passed_rec_mass_sideband_sd;

    // declare Handles for Final Selection (PUPPI Jets w/ and w/o SoftDrop)
    uhh2::Event::Handle<bool> h_passed_rec_final_puppi, h_passed_rec_final_puppi_sd;
    uhh2::Event::Handle<bool> h_passed_rec_pt_lep_sideband_puppi,    h_passed_rec_pt_topjet_sideband_puppi,    h_passed_rec_dr_sideband_puppi, h_passed_rec_mass_sideband_puppi;
    uhh2::Event::Handle<bool> h_passed_rec_pt_lep_sideband_puppi_sd, h_passed_rec_pt_topjet_sideband_puppi_sd, h_passed_rec_dr_sideband_puppi_sd, h_passed_rec_mass_sideband_puppi_sd;


    uhh2::Event::Handle<double> h_pt_gen, h_pt_gen_sd, h_pt_rec, h_pt_rec_puppi;
    uhh2::Event::Handle<double> h_tau32_gen, h_tau32_gen_sd;
    uhh2::Event::Handle<double> h_tau32_rec, h_tau32_rec_sd, h_tau32_rec_puppi, h_tau32_rec_puppi_sd;
    uhh2::Event::Handle<double> h_mass_gen, h_mass_gen_sd;
    uhh2::Event::Handle<double> h_mass_rec, h_mass_rec_sd;
    uhh2::Event::Handle<double> h_mass_rec_puppi, h_mass_rec_puppi_sd;

    uhh2::Event::Handle<std::vector<TopJet>> h_puppi_jets;
    uhh2::Event::Handle<std::vector<GenTopJet>> h_gentopjet;
    string puppi_jet_col = "jetsAk8PuppiSubstructure_SoftDropPuppi";
    string gentopjet_col = "genjetsAk8Substructure";

    string BTag_variation       = "none";
    string MuTrigger_variation  = "none";
    string MuScale_variation    = "none";
    string EleTrigger_variation = "none";
    string EleID_variation      = "none";
    string EleReco_variation    = "none";
    string PU_variation         = "none";

    bool derror;
  };


  PostKinCutModule::PostKinCutModule(Context & ctx){
    ctx.undeclare_all_event_output();

    const std::string& channel = ctx.get("channel", ""); //define Channel
    if     (channel == "muon") channel_ = muon;
    else if(channel == "ele") channel_ = ele;
    else {
      std::string log("PostKinCutModule::PostKinCutModule -- ");
      log += "invalid argument for 'channel' key in xml file (must be 'muon' or 'ele'): \""+channel+"\"";
      throw std::runtime_error(log);
    }

    derror = false;
    h_puppi_jets = ctx.get_handle<std::vector<TopJet>>(puppi_jet_col);
    h_gentopjet  = ctx.get_handle<std::vector<GenTopJet>>(gentopjet_col);

    // Get Handles from KinCut
    if(channel_ == muon)      h_passed_gen_kin = ctx.get_handle<bool>("h_passed_mu_gen");
    else if(channel_ == ele)  h_passed_gen_kin = ctx.get_handle<bool>("h_passed_ele_gen");
    if(channel_ == muon)      h_passed_rec_kin = ctx.get_handle<bool>("h_passed_mu_rec");
    else if(channel_ == ele)  h_passed_rec_kin = ctx.get_handle<bool>("h_passed_ele_rec");

    h_rec_weight_kin                      = ctx.get_handle<double>("h_rec_weight_kin");
    h_gen_weight_kin                      = ctx.get_handle<double>("h_gen_weight_kin");

    h_pt_rec                              = ctx.declare_event_output<double>("h_pt_rec");
    h_pt_rec_puppi                        = ctx.declare_event_output<double>("h_pt_rec_puppi");
    h_mass_rec                            = ctx.declare_event_output<double>("h_mass_rec");
    h_mass_rec_sd                         = ctx.declare_event_output<double>("h_mass_rec_sd");
    h_mass_rec_puppi                      = ctx.declare_event_output<double>("h_mass_rec_puppi");
    h_mass_rec_puppi_sd                   = ctx.declare_event_output<double>("h_mass_rec_puppi_sd");
    h_pt_gen                              = ctx.declare_event_output<double>("h_pt_gen");
    h_pt_gen_sd                           = ctx.declare_event_output<double>("h_pt_gen_sd");
    h_mass_gen                            = ctx.declare_event_output<double>("h_mass_gen");
    h_mass_gen_sd                         = ctx.declare_event_output<double>("h_mass_gen_sd");
    h_gen_weight                          = ctx.declare_event_output<double>("h_gen_weight");
    h_rec_weight                          = ctx.declare_event_output<double>("h_rec_weight");

    h_passed_gen_pt_lep_sideband          = ctx.declare_event_output<bool>("h_passed_gen_pt_lep_sideband");
    h_passed_gen_dr_sideband              = ctx.declare_event_output<bool>("h_passed_gen_dr_sideband");
    h_passed_gen_mass_sideband            = ctx.declare_event_output<bool>("h_passed_gen_mass_sideband");
    h_passed_gen_pt_topjet_sideband       = ctx.declare_event_output<bool>("h_passed_gen_pt_topjet_sideband");
    h_passed_gen_pt_lep_sideband_sd       = ctx.declare_event_output<bool>("h_passed_gen_pt_lep_sideband_sd");
    h_passed_gen_dr_sideband_sd           = ctx.declare_event_output<bool>("h_passed_gen_dr_sideband_sd");
    h_passed_gen_mass_sideband_sd         = ctx.declare_event_output<bool>("h_passed_gen_mass_sideband_sd");
    h_passed_gen_pt_topjet_sideband_sd    = ctx.declare_event_output<bool>("h_passed_gen_pt_topjet_sideband_sd");


    h_passed_rec_final                       = ctx.declare_event_output<bool>("h_passed_rec_final");
    h_passed_rec_final_sd                    = ctx.declare_event_output<bool>("h_passed_rec_final_sd");
    h_passed_rec_final_puppi                 = ctx.declare_event_output<bool>("h_passed_rec_final_puppi");
    h_passed_rec_final_puppi_sd              = ctx.declare_event_output<bool>("h_passed_rec_final_puppi_sd");
    h_passed_gen_final                       = ctx.declare_event_output<bool>("h_passed_gen_final");
    h_passed_gen_final_sd                    = ctx.declare_event_output<bool>("h_passed_gen_final_sd");
    h_passed_rec_pt_lep_sideband             = ctx.declare_event_output<bool>("h_passed_rec_pt_lep_sideband");
    h_passed_rec_pt_lep_sideband_sd          = ctx.declare_event_output<bool>("h_passed_rec_pt_lep_sideband_sd");
    h_passed_rec_pt_lep_sideband_puppi       = ctx.declare_event_output<bool>("h_passed_rec_pt_lep_sideband_puppi");
    h_passed_rec_pt_lep_sideband_puppi_sd    = ctx.declare_event_output<bool>("h_passed_rec_pt_lep_sideband_puppi_sd");
    h_passed_rec_dr_sideband                 = ctx.declare_event_output<bool>("h_passed_rec_dr_sideband");
    h_passed_rec_dr_sideband_sd              = ctx.declare_event_output<bool>("h_passed_rec_dr_sideband_sd");
    h_passed_rec_dr_sideband_puppi           = ctx.declare_event_output<bool>("h_passed_rec_dr_sideband_puppi");
    h_passed_rec_dr_sideband_puppi_sd        = ctx.declare_event_output<bool>("h_passed_rec_dr_sideband_puppi_sd");
    h_passed_rec_mass_sideband               = ctx.declare_event_output<bool>("h_passed_rec_mass_sideband");
    h_passed_rec_mass_sideband_sd            = ctx.declare_event_output<bool>("h_passed_rec_mass_sideband_sd");
    h_passed_rec_mass_sideband_puppi         = ctx.declare_event_output<bool>("h_passed_rec_mass_sideband_puppi");
    h_passed_rec_mass_sideband_puppi_sd      = ctx.declare_event_output<bool>("h_passed_rec_mass_sideband_puppi_sd");
    h_passed_rec_pt_topjet_sideband          = ctx.declare_event_output<bool>("h_passed_rec_pt_topjet_sideband");
    h_passed_rec_pt_topjet_sideband_sd       = ctx.declare_event_output<bool>("h_passed_rec_pt_topjet_sideband_sd");
    h_passed_rec_pt_topjet_sideband_puppi    = ctx.declare_event_output<bool>("h_passed_rec_pt_topjet_sideband_puppi");
    h_passed_rec_pt_topjet_sideband_puppi_sd = ctx.declare_event_output<bool>("h_passed_rec_pt_topjet_sideband_puppi_sd");

    h_tau32_rec                           = ctx.declare_event_output<double>("h_tau32_rec");
    h_tau32_rec_sd                        = ctx.declare_event_output<double>("h_tau32_rec_sd");
    h_tau32_rec_puppi                     = ctx.declare_event_output<double>("h_tau32_rec_puppi");
    h_tau32_rec_puppi_sd                  = ctx.declare_event_output<double>("h_tau32_rec_puppi_sd");
    h_tau32_gen                           = ctx.declare_event_output<double>("h_tau32_gen");
    h_tau32_gen_sd                        = ctx.declare_event_output<double>("h_tau32_gen_sd");

    BTag::algo btag_algo = BTag::DEEPCSV;
    BTag::wp btag_wp = BTag::WP_MEDIUM;

    PU_variation = ctx.get("PU_variation");
    BTag_variation = ctx.get("BTag_variation");
    MuScale_variation = ctx.get("MuScale_variation");
    MuTrigger_variation = ctx.get("MuTrigger_variation");
    EleID_variation = ctx.get("EleID_variation");
    EleTrigger_variation = ctx.get("EleTrigger_variation");
    EleReco_variation = ctx.get("EleReco_variation");

    //scale variation
    scale_variation.reset(new MCScaleVariation(ctx));
    lumiweight.reset(new MCLumiWeight(ctx));

    PUreweight.reset(new MCPileupReweight(ctx, PU_variation));

    sf_btag.reset(new MCBTagScaleFactor(ctx, btag_algo, btag_wp, "jets", BTag_variation));
    if(channel_ == muon){
      muo_tight_noniso_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonID_EfficienciesAndSF_average_RunBtoH.root","MC_NUM_TightID_DEN_genTracks_PAR_pt_eta",1, "tightID", false, MuScale_variation));
      muo_trigger_SF.reset(new MCMuonScaleFactor(ctx,"/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/MuonTrigger_EfficienciesAndSF_average_RunBtoH.root","IsoMu50_OR_IsoTkMu50_PtEtaBins",1, "muonTrigger", false, MuTrigger_variation));
      if(derror) std::cout << "TEST!!: " << MuTrigger_variation << '\n';
    }
    if(channel_ == ele){
      ele_trigger_SF.reset(new ElecTriggerSF(ctx, EleTrigger_variation, "eta_ptbins"));
      ele_reco_SF.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/egammaEffi.txt_EGM2D_RecEff_Moriond17.root", 1, "", EleReco_variation));
      ele_id_SF.reset(new MCElecScaleFactor(ctx, "/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/common/data/2016/egammaEffi.txt_EGM2D_CutBased_Tight_ID.root", 1, "", EleID_variation));
    }

    isTTbar = (ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft" || ctx.get("dataset_version") == "TTbar_2016v3" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700_part1" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part1" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000_part2" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft_part0" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft_part1" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1695_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1715_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1735_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755_part0" || ctx.get("dataset_version") == "TTbar_2016v3_mtop1755_part1" || ctx.get("dataset_version") == "TTbar_2016v3_madgraph_part0");

    isMC = (ctx.get("dataset_type") == "MC");

    // 2. set up selections

    if(isTTbar){
      const std::string ttbar_gen_label("ttbargen");
      ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));

      if(channel_ == muon) pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 0, 60));
      else if(channel_ == ele) pt_lep_gen.reset(new GenLeptonPtSelection(ctx, 1, 60));
      ntopjet2_gen.reset(new GenNTopJet(ctx, 2,2,gentopjet_col));
      ntopjet2_gen_sd.reset(new GenNTopJet(ctx, 2,2));

      if(channel_ == muon)     dr_gen.reset(new dRSelection(ctx, 0.8, 0, 0, gentopjet_col));
      else if(channel_ == ele) dr_gen.reset(new dRSelection(ctx, 0.8, 0, 1, gentopjet_col));
      if(channel_ == muon)     dr_gen_sd.reset(new dRSelection(ctx, 0.8, 1, 0));
      else if(channel_ == ele) dr_gen_sd.reset(new dRSelection(ctx, 0.8, 1, 1));

      if(channel_ == muon)     mass_gen.reset(new GenMassCompare(ctx, 0, 0, gentopjet_col));
      else if(channel_ == ele) mass_gen.reset(new GenMassCompare(ctx, 0, 1, gentopjet_col));
      if(channel_ == muon)     mass_gen_sd.reset(new GenMassCompare(ctx, 1, 0));
      else if(channel_ == ele) mass_gen_sd.reset(new GenMassCompare(ctx, 1, 1));

      pt_topjet_gen.reset(new PtSelection(ctx, 400, 200, 0, gentopjet_col));
      pt_topjet_gen_sd.reset(new PtSelection(ctx, 400, 200, 1));

      if(channel_ == muon)     pt_lep_gen_sideband.reset(new GenLeptonPtSelection(ctx, 0, 55));
      else if(channel_ == ele) pt_lep_gen_sideband.reset(new GenLeptonPtSelection(ctx, 1, 55));


      if(channel_ == muon)     dr_gen_sideband.reset(new dRSelection(ctx, 1.0, 0, 0, gentopjet_col));
      else if(channel_ == ele) dr_gen_sideband.reset(new dRSelection(ctx, 1.0, 0, 1, gentopjet_col));
      if(channel_ == muon)     dr_gen_sideband_sd.reset(new dRSelection(ctx, 1.0, 1, 0));
      else if(channel_ == ele) dr_gen_sideband_sd.reset(new dRSelection(ctx, 1.0, 1, 1));


      pt_topjet_gen_sideband.reset(new PtSelection(ctx, 300, 200, 0, gentopjet_col));
      pt_topjet_gen_sideband_sd.reset(new PtSelection(ctx, 300, 200, 1));

      gen_mass_lower.reset(new GenMassTopJet(ctx, 0., 155., 0, gentopjet_col));
      gen_mass_higher.reset(new GenMassTopJet(ctx, 155., -1, 0, gentopjet_col));

      gen_mass_lower_sd.reset(new GenMassTopJet(ctx, 0., 155., 1));
      gen_mass_higher_sd.reset(new GenMassTopJet(ctx, 155., -1, 1));

      genmatching.reset(new GenMatching(ctx, gentopjet_col));
      genmatching_sd.reset(new GenMatching(ctx));
    }
    // calculator_tau.reset(new calc_Nsubjettiness());
    ntopjet2_sel.reset(new NTopJetSelection(2,2));
    ntopjet2_sel_puppi.reset(new NPuppiJet(ctx, 2,2));

    if(channel_ == muon)     mass_sel.reset(new RecMassCompare(0, 0));         // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    else if(channel_ == ele) mass_sel.reset(new RecMassCompare(0, 1));      // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    if(channel_ == muon)     mass_sel_sd.reset(new RecMassCompare(1, 0));         // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    else if(channel_ == ele) mass_sel_sd.reset(new RecMassCompare(1, 1));             // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    if(channel_ == muon)     mass_sel_puppi.reset(new PuppiMassCompare(ctx, 0, 0));    // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    else if(channel_ == ele) mass_sel_puppi.reset(new PuppiMassCompare(ctx, 0, 1));   // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    if(channel_ == muon)     mass_sel_puppi_sd.reset(new PuppiMassCompare(ctx, 1, 0));  // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    else if(channel_ == ele) mass_sel_puppi_sd.reset(new PuppiMassCompare(ctx, 1, 1));  // 0: added 4-vector of lepton and second topjet, then compare masses; 1: same but SD
    if(channel_ == muon)     dr_sel.reset(new RecdRSelection(0.8, 0, 0));
    else if(channel_ == ele) dr_sel.reset(new RecdRSelection(0.8, 0, 1));
    if(channel_ == muon)     dr_sel_sd.reset(new RecdRSelection(0.8, 1, 0));
    else if(channel_ == ele) dr_sel_sd.reset(new RecdRSelection(0.8, 1, 1));
    if(channel_ == muon)     dr_sel_puppi.reset(new PuppidRSelection(ctx, 0, 0, 0.8));
    else if(channel_ == ele) dr_sel_puppi.reset(new PuppidRSelection(ctx, 0, 1, 0.8));
    if(channel_ == muon)     dr_sel_puppi_sd.reset(new PuppidRSelection(ctx, 1, 0, 0.8));
    else if(channel_ == ele) dr_sel_puppi_sd.reset(new PuppidRSelection(ctx, 1, 1, 0.8));

    pt_topjet_sel.reset(new RecPtSelection(400, 200));
    pt_topjet_sel_sd.reset(new RecPtSelection(400, 200, 1));
    pt_topjet_sel_puppi.reset(new PuppiPtSelection(ctx, 400, 200));
    pt_topjet_sel_puppi_sd.reset(new PuppiPtSelection(ctx, 400, 200, 1));

    if(channel_ == muon) pt_lep_sel.reset(new LeptonPtSelection(0, 60));
    else if(channel_ == ele) pt_lep_sel.reset(new LeptonPtSelection(1, 60));

    mass_lower.reset(new MassTopJetSelection(0, 0., 152.));
    mass_higher.reset(new MassTopJetSelection(0, 152.));
    mass_lower_sd.reset(new MassTopJetSelection(1, 0., 152.));
    mass_higher_sd.reset(new MassTopJetSelection(1, 152.));
    mass_lower_puppi.reset(new MassPuppiJetSelection(ctx, 0, 0., 152.));
    mass_higher_puppi.reset(new MassPuppiJetSelection(ctx, 0, 152.));
    mass_lower_puppi_sd.reset(new MassPuppiJetSelection(ctx, 1, 0., 152.));
    mass_higher_puppi_sd.reset(new MassPuppiJetSelection(ctx, 1, 152.));

    if(channel_ == muon)     pt_lep_rec_sideband.reset(new LeptonPtSelection(0, 55));
    else if(channel_ == ele) pt_lep_rec_sideband.reset(new LeptonPtSelection(1, 55));
    if(channel_ == muon)     dr_rec_sideband.reset(new RecdRSelection(1.0, 0, 0));
    else if(channel_ == ele) dr_rec_sideband.reset(new RecdRSelection(1.0, 0, 1));
    if(channel_ == muon)     dr_rec_sideband_sd.reset(new RecdRSelection(1.0, 1, 0));
    else if(channel_ == ele) dr_rec_sideband_sd.reset(new RecdRSelection(1.0, 1, 1));
    if(channel_ == muon)     dr_rec_sideband_puppi.reset(new PuppidRSelection(ctx, 0, 0, 1.0));
    else if(channel_ == ele) dr_rec_sideband_puppi.reset(new PuppidRSelection(ctx, 0, 1, 1.0));
    if(channel_ == muon)     dr_rec_sideband_puppi_sd.reset(new PuppidRSelection(ctx, 1, 0, 1.0));
    else if(channel_ == ele) dr_rec_sideband_puppi_sd.reset(new PuppidRSelection(ctx, 1, 1, 1.0));

    pt_topjet_rec_sideband.reset(new RecPtSelection(300, 200));
    pt_topjet_rec_sideband_sd.reset(new RecPtSelection(300, 200, 1));
    pt_topjet_rec_sideband_puppi.reset(new PuppiPtSelection(ctx, 300, 200));
    pt_topjet_rec_sideband_puppi_sd.reset(new PuppiPtSelection(ctx, 300, 200, 1));


    // 3. Set up Hists classes:
    h_gen_pt_lep.reset(new GenHists(ctx, "gen_pt_lep"));
    h_gen_pt_lep_matched.reset(new GenHists(ctx, "gen_pt_lep_matched"));
    h_gen_pt_lep_unmatched.reset(new GenHists(ctx, "gen_pt_lep_unmatched"));

    h_gen_pt_topjet.reset(new GenHists(ctx, "gen_pt_topjet"));
    h_gen_pt_topjet_matched.reset(new GenHists(ctx, "gen_pt_topjet_matched"));
    h_gen_pt_topjet_unmatched.reset(new GenHists(ctx, "gen_pt_topjet_unmatched"));

    h_gen_pt_topjet_sd.reset(new GenHists(ctx, "gen_pt_topjet_sd"));
    h_gen_pt_topjet_sd_matched.reset(new GenHists(ctx, "gen_pt_topjet_sd_matched"));
    h_gen_pt_topjet_sd_unmatched.reset(new GenHists(ctx, "gen_pt_topjet_sd_unmatched"));

    h_gen_ntopjet2.reset(new GenHists(ctx, "gen_ntopjet2"));
    h_gen_ntopjet2_matched.reset(new GenHists(ctx, "gen_ntopjet2_matched"));
    h_gen_ntopjet2_unmatched.reset(new GenHists(ctx, "gen_ntopjet2_unmatched"));

    h_gen_ntopjet2_sd.reset(new GenHists(ctx, "gen_ntopjet2_sd"));
    h_gen_ntopjet2_sd_matched.reset(new GenHists(ctx, "gen_ntopjet2_sd_matched"));
    h_gen_ntopjet2_sd_unmatched.reset(new GenHists(ctx, "gen_ntopjet2_sd_unmatched"));

    h_gen_dr.reset(new GenHists(ctx, "gen_dr"));
    h_gen_dr_matched.reset(new GenHists(ctx, "gen_dr_matched"));
    h_gen_dr_unmatched.reset(new GenHists(ctx, "gen_dr_unmatched"));

    h_gen_dr_sd.reset(new GenHists(ctx, "gen_dr_sd"));
    h_gen_dr_sd_matched.reset(new GenHists(ctx, "gen_dr_sd_matched"));
    h_gen_dr_sd_unmatched.reset(new GenHists(ctx, "gen_dr_sd_unmatched"));

    h_gen_mass.reset(new GenHists(ctx, "gen_mass"));
    h_gen_mass_matched.reset(new GenHists(ctx, "gen_mass_matched"));
    h_gen_mass_unmatched.reset(new GenHists(ctx, "gen_mass_unmatched"));

    h_gen_mass_sd.reset(new GenHists(ctx, "gen_mass_sd"));
    h_gen_mass_sd_matched.reset(new GenHists(ctx, "gen_mass_sd_matched"));
    h_gen_mass_sd_unmatched.reset(new GenHists(ctx, "gen_mass_sd_unmatched"));

    h_gen_pt_topjet_sideband.reset(new GenHists(ctx, "gen_pt_topjet_sideband"));
    h_gen_pt_topjet_sideband_sd.reset(new GenHists(ctx, "gen_pt_topjet_sideband_sd"));
    h_gen_pt_lep_sideband.reset(new GenHists(ctx, "gen_pt_lep_sideband"));
    h_gen_pt_lep_sideband_sd.reset(new GenHists(ctx, "gen_pt_lep_sideband_sd"));
    h_gen_dr_sideband.reset(new GenHists(ctx, "gen_dr_sideband"));
    h_gen_dr_sideband_sd.reset(new GenHists(ctx, "gen_dr_sideband_sd"));
    h_gen_mass_sideband.reset(new GenHists(ctx, "gen_mass_sideband"));
    h_gen_mass_sideband_sd.reset(new GenHists(ctx, "gen_mass_sideband_sd"));

    h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
    h_pureweight.reset(new TopSubstructureRecoHists(ctx, "pu_reweight"));
    h_sfbtag.reset(new TopSubstructureRecoHists(ctx, "sf_btag"));
    h_leptontrigger.reset(new TopSubstructureRecoHists(ctx, "lepton_trigger"));
    h_leptontight.reset(new TopSubstructureRecoHists(ctx, "lepton_tight"));
    h_elereco_sf.reset(new TopSubstructureRecoHists(ctx, "ele_reco_sf"));
    h_pt_lep.reset(new TopSubstructureRecoHists(ctx, "pt_lep"));
    h_pt_topjet.reset(new TopSubstructureRecoHists(ctx, "pt_topjet"));
    h_pt_topjet_sd.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_sd"));
    h_pt_topjet_puppi.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_puppi"));
    h_pt_topjet_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_puppi_sd"));
    h_ntopjet2.reset(new TopSubstructureRecoHists(ctx, "ntopjet2"));
    h_ntopjet2_sd.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_sd"));
    h_ntopjet2_puppi.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_puppi"));
    h_ntopjet2_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_puppi_sd"));
    h_dr.reset(new TopSubstructureRecoHists(ctx, "dr"));
    h_dr_sd.reset(new TopSubstructureRecoHists(ctx, "dr_sd"));
    h_dr_puppi.reset(new TopSubstructureRecoHists(ctx, "dr_puppi"));
    h_dr_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "dr_puppi_sd"));
    h_mass.reset(new TopSubstructureRecoHists(ctx, "mass"));
    h_mass_sd.reset(new TopSubstructureRecoHists(ctx, "mass_sd"));
    h_mass_puppi.reset(new TopSubstructureRecoHists(ctx, "mass_puppi"));
    h_mass_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "mass_puppi_sd"));

    h_passedgen_rec.reset(new TopSubstructureRecoHists(ctx, "passedgen_rec"));
    h_passedgen_rec_sd.reset(new TopSubstructureRecoHists(ctx, "passedgen_rec_sd"));
    h_passedrec_gen.reset(new GenHists(ctx, "passedrec_gen"));
    h_passedrec_gen_sd.reset(new GenHists(ctx, "passedrec_gen_sd"));
    h_passedrec_gen_puppi.reset(new GenHists(ctx, "passedrec_gen_puppi"));
    h_passedrec_gen_puppi_sd.reset(new GenHists(ctx, "passedrec_gen_puppi_sd"));

    h_mass_lower.reset(new TopSubstructureRecoHists(ctx, "mass_lower"));
    h_mass_higher.reset(new TopSubstructureRecoHists(ctx, "mass_higher"));
    h_mass_lower_sd.reset(new TopSubstructureRecoHists(ctx, "mass_lower_sd"));
    h_mass_higher_sd.reset(new TopSubstructureRecoHists(ctx, "mass_higher_sd"));
    h_mass_lower_puppi.reset(new TopSubstructureRecoHists(ctx, "mass_lower_puppi"));
    h_mass_higher_puppi.reset(new TopSubstructureRecoHists(ctx, "mass_higher_puppi"));
    h_mass_lower_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "mass_lower_puppi_sd"));
    h_mass_higher_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "mass_higher_puppi_sd"));

    h_gen_mass_lower.reset(new GenHists(ctx, "gen_mass_lower"));
    h_gen_mass_lower_matched.reset(new GenHists(ctx, "gen_mass_lower_matched"));
    h_gen_mass_lower_unmatched.reset(new GenHists(ctx, "gen_mass_lower_unmatched"));
    h_gen_mass_lower_sd.reset(new GenHists(ctx, "gen_mass_lower_sd"));
    h_gen_mass_lower_sd_matched.reset(new GenHists(ctx, "gen_mass_lower_sd_matched"));
    h_gen_mass_lower_sd_unmatched.reset(new GenHists(ctx, "gen_mass_lower_sd_unmatched"));

    h_gen_mass_higher.reset(new GenHists(ctx, "gen_mass_higher"));
    h_gen_mass_higher_matched.reset(new GenHists(ctx, "gen_mass_higher_matched"));
    h_gen_mass_higher_unmatched.reset(new GenHists(ctx, "gen_mass_higher_unmatched"));
    h_gen_mass_higher_sd.reset(new GenHists(ctx, "gen_mass_higher_sd"));
    h_gen_mass_higher_sd_matched.reset(new GenHists(ctx, "gen_mass_higher_sd_matched"));
    h_gen_mass_higher_sd_unmatched.reset(new GenHists(ctx, "gen_mass_higher_sd_unmatched"));

    h_rec_pt_lep_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_lep_sideband"));
    h_rec_pt_lep_sideband_sd.reset(new TopSubstructureRecoHists(ctx, "rec_pt_lep_sideband_sd"));
    h_rec_pt_lep_sideband_puppi.reset(new TopSubstructureRecoHists(ctx, "rec_pt_lep_sideband_puppi"));
    h_rec_pt_lep_sideband_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "rec_pt_lep_sideband_puppi_sd"));
    h_rec_pt_topjet_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband"));
    h_rec_pt_topjet_sideband_sd.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband_sd"));
    h_rec_pt_topjet_sideband_puppi.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband_puppi"));
    h_rec_pt_topjet_sideband_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "rec_pt_topjet_sideband_puppi_sd"));
    h_rec_dr_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband"));
    h_rec_dr_sideband_sd.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband_sd"));
    h_rec_dr_sideband_puppi.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband_puppi"));
    h_rec_dr_sideband_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "rec_dr_sideband_puppi_sd"));
    h_rec_mass_sideband.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband"));
    h_rec_mass_sideband_sd.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband_sd"));
    h_rec_mass_sideband_puppi.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband_puppi"));
    h_rec_mass_sideband_puppi_sd.reset(new TopSubstructureRecoHists(ctx, "rec_mass_sideband_puppi_sd"));


    h_event.reset(new EventHists(ctx, "Event Hist"));


    // h_ttbar_hist.reset(new TTbarGenHists(ctx, "ttbar_hist"));
    // h_ttbar_hist_sd.reset(new TTbarGenHists(ctx, "ttbar_hist_sd"));
  }


  bool PostKinCutModule::process(Event & event) {
    if(derror) cout << "PostKinCutModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;

    vector<TopJet> h_puppi;
    if(event.is_valid(h_puppi_jets)) h_puppi = event.get(h_puppi_jets);

    vector<GenTopJet> gentopjet;
    if(event.is_valid(h_gentopjet)){
      gentopjet.clear();
      gentopjet = event.get(h_gentopjet);
    }

    if(event.is_valid(h_gen_weight_kin)) event.weight = event.get(h_gen_weight_kin);
    lumiweight->process(event);
    scale_variation->process(event); // here, it is only executed to be filled into the gen weight is has to be done again to appear in the event.weight


    event.set(h_gen_weight, event.weight);
    if(event.is_valid(h_rec_weight_kin)) event.weight *= event.get(h_rec_weight_kin);

    // 1. run all modules other modules.
    if(event.is_valid(h_passed_gen_kin)) passed_gen_kin = event.get(h_passed_gen_kin);
    else passed_gen_kin                    = false;

    passed_gen_pt_lep                  = false;
    passed_gen_ntopjet                 = false;
    passed_gen_ntopjet_sd              = false;
    passed_gen_dr                      = false;
    passed_gen_dr_sd                   = false;
    passed_gen_mass                    = false;
    passed_gen_mass_sd                 = false;
    passed_gen_pt_topjet               = false;
    passed_gen_pt_topjet_sd            = false;
    passed_gen_pt_lep_sideband         = false;
    passed_gen_pt_lep_sideband_sd      = false;
    passed_gen_dr_sideband             = false;
    passed_gen_dr_sideband_sd          = false;
    passed_gen_mass_sideband           = false;
    passed_gen_mass_sideband_sd        = false;
    passed_gen_pt_topjet_sideband      = false;
    passed_gen_pt_topjet_sideband_sd   = false;
    passed_gen_final                   = false;
    passed_gen_final_sd                = false;
    matched_gen                        = false;
    matched_gen_sd                     = false;

    if(event.is_valid(h_passed_rec_kin)) passed_rec_kin = event.get(h_passed_rec_kin);
    else passed_rec_kin                       = false;

    passed_rec_pt_lep                     = false;
    passed_rec_ntopjet                    = false;
    passed_rec_ntopjet_puppi              = false;
    passed_rec_dr                         = false;
    passed_rec_dr_sd                      = false;
    passed_rec_dr_puppi                   = false;
    passed_rec_dr_puppi_sd                = false;
    passed_rec_mass                       = false;
    passed_rec_mass_sd                    = false;
    passed_rec_mass_puppi                 = false;
    passed_rec_mass_puppi_sd              = false;
    passed_rec_pt_topjet                  = false;
    passed_rec_pt_topjet_sd               = false;
    passed_rec_pt_topjet_puppi            = false;
    passed_rec_pt_topjet_puppi_sd         = false;
    passed_rec_dr_sideband                = false;
    passed_rec_dr_sideband_sd             = false;
    passed_rec_dr_sideband_puppi          = false;
    passed_rec_dr_sideband_puppi_sd       = false;
    passed_rec_mass_sideband              = false;
    passed_rec_mass_sideband_sd           = false;
    passed_rec_mass_sideband_puppi        = false;
    passed_rec_mass_sideband_puppi_sd     = false;
    passed_rec_pt_lep_sideband            = false;
    passed_rec_pt_lep_sideband_sd         = false;
    passed_rec_pt_lep_sideband_puppi      = false;
    passed_rec_pt_lep_sideband_puppi_sd   = false;
    passed_rec_pt_topjet_sideband         = false;
    passed_rec_pt_topjet_sideband_sd      = false;
    passed_rec_pt_topjet_sideband_puppi   = false;
    passed_rec_pt_topjet_sideband_puppi_sd= false;
    passed_rec_final                      = false;
    passed_rec_final_sd                   = false;
    passed_rec_final_puppi                = false;
    passed_rec_final_puppi_sd             = false;

    /** PU Reweighting *********************/
    if(derror) cout << "test1" << '\n';
    h_start->fill(event);
    PUreweight->process(event);
    if(derror) cout << "test2" << '\n';
    h_pureweight->fill(event);
    sf_btag->process(event);
    if(derror) cout << "test3" << '\n';
    h_sfbtag->fill(event);
    if(channel_ == muon){
      muo_trigger_SF->process(event);
      if(derror) cout << "test4" << '\n';
      h_leptontrigger->fill(event);
      muo_tight_noniso_SF->process(event);
      if(derror) cout << "test5" << '\n';
      h_leptontight->fill(event);
    }
    else if(channel_ == ele){
      ele_trigger_SF->process(event);
      if(derror) cout << "test4" << '\n';
      h_leptontrigger->fill(event);
      ele_id_SF->process(event);
      if(derror) cout << "test5" << '\n';
      h_leptontight->fill(event);
      if(derror) cout << "test5.1" << '\n';
      ele_reco_SF->process(event);
      if(derror) cout << "test5.2" << '\n';
      h_elereco_sf->fill(event);
      if(derror) cout << "test5.3" << '\n';
    }

    event.set(h_rec_weight, event.weight);

    if(isTTbar){
      if(derror) cout << "test6" << '\n';
      ttgenprod->process(event);
      if(derror) cout << "test7" << '\n';
      passed_gen_pt_lep = pt_lep_gen->passes(event);
      if(derror) cout << "test8" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep){
        if(derror) cout << "test9" << '\n';
        h_gen_pt_lep->fill(event);
        if(derror) cout << "test10" << '\n';
        matched_gen = genmatching->passes(event);
        if(derror) cout << "test11" << '\n';
        if(matched_gen) h_gen_pt_lep_matched->fill(event);
        else h_gen_pt_lep_unmatched->fill(event);
        if(derror) cout << "test12" << '\n';
      }

      if(derror) cout << "test13" << '\n';
      passed_gen_pt_topjet = pt_topjet_gen->passes(event);
      if(derror) cout << "test14" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet){
        if(derror) cout << "test15" << '\n';
        h_gen_pt_topjet->fill(event);
        if(derror) cout << "test16" << '\n';
        matched_gen = genmatching->passes(event);
        if(derror) cout << "test17" << '\n';
        if(matched_gen) h_gen_pt_topjet_matched->fill(event);
        else h_gen_pt_topjet_unmatched->fill(event);
        if(derror) cout << "test18" << '\n';
      }

      if(derror) cout << "test19" << '\n';
      passed_gen_pt_topjet_sd = pt_topjet_gen_sd->passes(event);
      if(derror) cout << "test20" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet_sd){
        if(derror) cout << "test21" << '\n';
        h_gen_pt_topjet_sd->fill(event);
        if(derror) cout << "test22" << '\n';
        matched_gen_sd = genmatching_sd->passes(event);
        if(derror) cout << "test23" << '\n';
        if(matched_gen_sd) h_gen_pt_topjet_sd_matched->fill(event);
        else h_gen_pt_topjet_sd_unmatched->fill(event);
        if(derror) cout << "test24" << '\n';
      }

      if(derror) cout << "test25" << '\n';
      passed_gen_ntopjet = ntopjet2_gen->passes(event);
      if(derror) cout << "test26" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet){
        if(derror) cout << "test27" << '\n';
        h_gen_ntopjet2->fill(event);
        if(derror) cout << "test28" << '\n';
        matched_gen = genmatching->passes(event);
        if(derror) cout << "test29" << '\n';
        if(matched_gen) h_gen_ntopjet2_matched->fill(event);
        else h_gen_ntopjet2_unmatched->fill(event);
        if(derror) cout << "test30" << '\n';
      }

      if(derror) cout << "test31" << '\n';
      passed_gen_ntopjet_sd = ntopjet2_gen_sd->passes(event);
      if(derror) cout << "test32" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd){
        if(derror) cout << "test33" << '\n';
        h_gen_ntopjet2_sd->fill(event);
        if(derror) cout << "test34" << '\n';
        matched_gen_sd = genmatching_sd->passes(event);
        if(derror) cout << "test35" << '\n';
        if(matched_gen_sd) h_gen_ntopjet2_sd_matched->fill(event);
        else h_gen_ntopjet2_sd_unmatched->fill(event);
        if(derror) cout << "test36" << '\n';
      }

      if(derror) cout << "test37" << '\n';
      passed_gen_dr = dr_gen->passes(event);
      if(derror) cout << "test38" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr){
        if(derror) cout << "test39" << '\n';
        h_gen_dr->fill(event);
        if(derror) cout << "test40" << '\n';
        matched_gen = genmatching->passes(event);
        if(derror) cout << "test41" << '\n';
        if(matched_gen) h_gen_dr_matched->fill(event);
        else h_gen_dr_unmatched->fill(event);
        if(derror) cout << "test42" << '\n';
      }

      if(derror) cout << "test43" << '\n';
      passed_gen_dr_sd = dr_gen_sd->passes(event);
      if(derror) cout << "test44" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && passed_gen_dr_sd){
        if(derror) cout << "test45" << '\n';
        h_gen_dr_sd->fill(event);
        if(derror) cout << "test46" << '\n';
        matched_gen_sd = genmatching_sd->passes(event);
        if(derror) cout << "test47" << '\n';
        if(matched_gen_sd) h_gen_dr_sd_matched->fill(event);
        else h_gen_dr_sd_unmatched->fill(event);
        if(derror) cout << "test48" << '\n';
      }

      if(derror) cout << "test49" << '\n';
      passed_gen_mass = mass_gen->passes(event);
      if(derror) cout << "test50" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass){
        if(derror) cout << "test51" << '\n';
        h_gen_mass->fill(event);
        if(derror) cout << "test52" << '\n';
        h_passedgen_rec->fill(event);
        if(derror) cout << "test53" << '\n';
        passed_gen_final = true;
        if(derror) cout << "test54" << '\n';
        matched_gen = genmatching->passes(event);
        if(derror) cout << "test55" << '\n';
        if(matched_gen) h_gen_mass_matched->fill(event);
        else h_gen_mass_unmatched->fill(event);
        if(derror) cout << "test56" << '\n';
      }

      if(derror) cout << "test57" << '\n';
      passed_gen_mass_sd = mass_gen_sd->passes(event);
      if(derror) cout << "test58" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && passed_gen_dr_sd && passed_gen_mass_sd){
        if(derror) cout << "test59" << '\n';
        h_gen_mass_sd->fill(event);
        if(derror) cout << "test60" << '\n';
        h_passedgen_rec_sd->fill(event);
        if(derror) cout << "test61" << '\n';
        passed_gen_final_sd = true;
        if(derror) cout << "test62" << '\n';
        matched_gen_sd = genmatching_sd->passes(event);
        if(derror) cout << "test63" << '\n';
        if(matched_gen_sd) h_gen_mass_sd_matched->fill(event);
        else h_gen_mass_sd_unmatched->fill(event);
        if(derror) cout << "test64" << '\n';
      }

      if(derror) cout << "test65" << '\n';
      if(passed_gen_kin && !passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && pt_lep_gen_sideband->passes(event)){
        if(derror) cout << "test66" << '\n';
        h_gen_pt_lep_sideband->fill(event);
        if(derror) cout << "test67" << '\n';
        passed_gen_pt_lep_sideband = true;
      }

      if(derror) cout << "test68" << '\n';
      if(passed_gen_kin && !passed_gen_pt_lep && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && passed_gen_dr_sd && passed_gen_mass_sd && pt_lep_gen_sideband->passes(event)){
        if(derror) cout << "test69" << '\n';
        h_gen_pt_lep_sideband_sd->fill(event);
        if(derror) cout << "test70" << '\n';
        passed_gen_pt_lep_sideband_sd = true;
      }

      if(derror) cout << "test71" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && !passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && passed_gen_mass && pt_topjet_gen_sideband->passes(event)){
        if(derror) cout << "test72" << '\n';
        h_gen_pt_topjet_sideband->fill(event);
        if(derror) cout << "test73" << '\n';
        passed_gen_pt_topjet_sideband = true;
      }

      if(derror) cout << "test74" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && !passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && passed_gen_dr_sd && passed_gen_mass_sd && pt_topjet_gen_sideband_sd->passes(event)){
        if(derror) cout << "test75" << '\n';
        h_gen_pt_topjet_sideband_sd->fill(event);
        if(derror) cout << "test76" << '\n';
        passed_gen_pt_topjet_sideband_sd = true;
      }

      if(derror) cout << "test77" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet && !passed_gen_dr && passed_gen_mass && dr_gen_sideband->passes(event)){
        if(derror) cout << "test78" << '\n';
        h_gen_dr_sideband->fill(event);
        if(derror) cout << "test79" << '\n';
        passed_gen_dr_sideband = true;
      }

      if(derror) cout << "test80" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && !passed_gen_dr_sd && passed_gen_mass_sd && dr_gen_sideband_sd->passes(event)){

        if(derror) cout << "test81" << '\n';
        h_gen_dr_sideband_sd->fill(event);
        if(derror) cout << "test82" << '\n';
        passed_gen_dr_sideband_sd = true;
      }

      if(derror) cout << "test83" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep && passed_gen_pt_topjet && passed_gen_ntopjet && passed_gen_dr && !passed_gen_mass){
        if(derror) cout << "test84" << '\n';
        h_gen_mass_sideband->fill(event);
        if(derror) cout << "test85" << '\n';
        passed_gen_mass_sideband = true;
      }

      if(derror) cout << "test86" << '\n';
      if(passed_gen_kin && passed_gen_pt_lep  && passed_gen_pt_topjet_sd && passed_gen_ntopjet_sd && passed_gen_dr_sd && !passed_gen_mass_sd){
        if(derror) cout << "test87" << '\n';
        h_gen_mass_sideband_sd->fill(event);
        if(derror) cout << "test88" << '\n';
        passed_gen_mass_sideband_sd = true;
      }

      if(derror) cout << "test89" << '\n';
      if(passed_gen_final && gen_mass_lower->passes(event)){
        if(derror) cout << "test90" << '\n';
        h_gen_mass_lower->fill(event);
        if(derror) cout << "test91" << '\n';
        if(isTTbar){
          if(derror) cout << "test92" << '\n';
          matched_gen = genmatching->passes(event);
          if(derror) cout << "test93" << '\n';
          if(matched_gen) h_gen_mass_lower_matched->fill(event);
          else h_gen_mass_lower_unmatched->fill(event);
          if(derror) cout << "test94" << '\n';
        }
      }
      if(derror) cout << "test95" << '\n';
      if(passed_gen_final && gen_mass_higher->passes(event)){
        if(derror) cout << "test96" << '\n';
        h_gen_mass_higher->fill(event);
        if(derror) cout << "test97" << '\n';
        if(isTTbar){
          if(derror) cout << "test98" << '\n';
          matched_gen = genmatching->passes(event);
          if(derror) cout << "test99" << '\n';
          if(matched_gen) h_gen_mass_higher_matched->fill(event);
          else h_gen_mass_higher_unmatched->fill(event);
          if(derror) cout << "test100" << '\n';
        }
      }

      if(derror) cout << "test101" << '\n';
      if(passed_gen_final_sd && gen_mass_lower_sd->passes(event)){
        if(derror) cout << "test102" << '\n';
        h_gen_mass_lower_sd->fill(event);
        if(derror) cout << "test103" << '\n';
        if(isTTbar){
          if(derror) cout << "test104" << '\n';
          matched_gen_sd = genmatching_sd->passes(event);
          if(derror) cout << "test105" << '\n';
          if(matched_gen_sd) h_gen_mass_lower_sd_matched->fill(event);
          else h_gen_mass_lower_sd_unmatched->fill(event);
          if(derror) cout << "test106" << '\n';
        }
      }
      if(derror) cout << "test107" << '\n';
      if(passed_gen_final_sd && gen_mass_higher_sd->passes(event)){
        if(derror) cout << "test108" << '\n';
        h_gen_mass_higher_sd->fill(event);
        if(derror) cout << "test109" << '\n';
        if(isTTbar){
          if(derror) cout << "test110" << '\n';
          matched_gen_sd = genmatching_sd->passes(event);
          if(derror) cout << "test111" << '\n';
          if(matched_gen_sd) h_gen_mass_higher_sd_matched->fill(event);
          else h_gen_mass_higher_sd_unmatched->fill(event);
          if(derror) cout << "test112" << '\n';
        }
      }
    }

    if(derror) cout << "test113" << '\n';
    /*
    ██████  ███████  ██████  ██████
    ██   ██ ██      ██      ██    ██
    ██████  █████   ██      ██    ██
    ██   ██ ██      ██      ██    ██
    ██   ██ ███████  ██████  ██████
    */

    // calculator_tau->tau_one(event);
    passed_rec_pt_lep = pt_lep_sel->passes(event);
    if(derror) cout << "test114" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep) h_pt_lep->fill(event);
    if(derror) cout << "test115" << '\n';

    // pT(first Topjet) > 400
    if(derror) cout << "test116" << '\n';
    passed_rec_pt_topjet = pt_topjet_sel->passes(event);
    if(derror) cout << "test117" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet) h_pt_topjet->fill(event);
    if(derror) cout << "test118" << '\n';

    passed_rec_pt_topjet_sd = pt_topjet_sel_sd->passes(event);
    if(derror) cout << "test119" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd) h_pt_topjet_sd->fill(event);
    if(derror) cout << "test120" << '\n';

    passed_rec_pt_topjet_puppi = pt_topjet_sel_puppi->passes(event);
    if(derror) cout << "test121" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi) h_pt_topjet_puppi->fill(event);
    if(derror) cout << "test122" << '\n';

    passed_rec_pt_topjet_puppi_sd = pt_topjet_sel_puppi_sd->passes(event);
    if(derror) cout << "test123" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd) h_pt_topjet_puppi_sd->fill(event);
    if(derror) cout << "test124" << '\n';

    // >= 2 TopJets
    passed_rec_ntopjet = ntopjet2_sel->passes(event);
    if(derror) cout << "test125" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet) h_ntopjet2->fill(event);
    if(derror) cout << "test126" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet) h_ntopjet2_sd->fill(event);
    if(derror) cout << "test127" << '\n';

    passed_rec_ntopjet_puppi = ntopjet2_sel_puppi->passes(event);
    if(derror) cout << "test128" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi) h_ntopjet2_puppi->fill(event);
    if(derror) cout << "test129" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi) h_ntopjet2_puppi_sd->fill(event);
    if(derror) cout << "test130" << '\n';

    // dR(muon, second TopJet) < 0.8
    passed_rec_dr = dr_sel->passes(event);
    if(derror) cout << "test131" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr) h_dr->fill(event);
    if(derror) cout << "test132" << '\n';
    passed_rec_dr_sd = dr_sel_sd->passes(event);
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet && passed_rec_dr_sd) h_dr_sd->fill(event);
    if(derror) cout << "test133" << '\n';

    passed_rec_dr_puppi = dr_sel_puppi->passes(event);
    if(derror) cout << "test134" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && passed_rec_dr_puppi) h_dr_puppi->fill(event);
    if(derror) cout << "test135" << '\n';
    passed_rec_dr_puppi_sd = dr_sel_puppi_sd->passes(event);
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && passed_rec_dr_puppi_sd) h_dr_puppi_sd->fill(event);
    if(derror) cout << "test136" << '\n';

    // M(first TopJet) > M(second TopJet + Muon)
    passed_rec_mass = mass_sel->passes(event);
    if(derror) cout << "test137" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass){
      if(derror) cout << "test138" << '\n';
      h_mass->fill(event);
      if(derror) cout << "test139" << '\n';
      passed_rec_final = true;
      if(isMC) h_passedrec_gen->fill(event);
      if(derror) cout << "test140" << '\n';
      // h_ttbar_hist->fill(event);
    }
    if(derror) cout << "test141" << '\n';
    passed_rec_mass_sd = mass_sel_sd->passes(event);
    if(derror) cout << "test142" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet && passed_rec_dr_sd && passed_rec_mass_sd){
      if(derror) cout << "test143" << '\n';
      h_mass_sd->fill(event);
      if(derror) cout << "test144" << '\n';
      passed_rec_final_sd = true;
      if(isMC) h_passedrec_gen_sd->fill(event);
      if(derror) cout << "test145" << '\n';
      // h_ttbar_hist_sd->fill(event);
    }

    if(derror) cout << "test146" << '\n';
    passed_rec_mass_puppi = mass_sel_puppi->passes(event);
    if(derror) cout << "test147" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && passed_rec_dr_puppi && passed_rec_mass_puppi){
      if(derror) cout << "test148" << '\n';
      h_mass_puppi->fill(event);
      if(derror) cout << "test149" << '\n';
      passed_rec_final_puppi = true;
      if(isMC) h_passedrec_gen_puppi->fill(event);
      if(derror) cout << "test150" << '\n';
    }

    if(derror) cout << "test151" << '\n';
    passed_rec_mass_puppi_sd = mass_sel_puppi_sd->passes(event);
    if(derror) cout << "test152" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && passed_rec_dr_puppi_sd && passed_rec_mass_puppi_sd){
      if(derror) cout << "test153" << '\n';
      h_mass_puppi_sd->fill(event);
      if(derror) cout << "test154" << '\n';
      passed_rec_final_puppi_sd = true;
      if(isMC) h_passedrec_gen_puppi_sd->fill(event);
      if(derror) cout << "test155" << '\n';
    }

    /*
    ███████ ██ ██████  ███████ ██████   █████  ███    ██ ██████  ███████
    ██      ██ ██   ██ ██      ██   ██ ██   ██ ████   ██ ██   ██ ██
    ███████ ██ ██   ██ █████   ██████  ███████ ██ ██  ██ ██   ██ ███████
         ██ ██ ██   ██ ██      ██   ██ ██   ██ ██  ██ ██ ██   ██      ██
    ███████ ██ ██████  ███████ ██████  ██   ██ ██   ████ ██████  ███████
    */
    if(derror) cout << "test156" << '\n';
    if(passed_rec_kin && !passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && pt_lep_rec_sideband->passes(event)){
      if(derror) cout << "test157" << '\n';
      h_rec_pt_lep_sideband->fill(event);
      if(derror) cout << "test158" << '\n';
      passed_rec_pt_lep_sideband = true;
    }

    if(derror) cout << "test159" << '\n';
    if(passed_rec_kin && !passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet && passed_rec_dr_sd && passed_rec_mass_sd && pt_lep_rec_sideband->passes(event)){
      if(derror) cout << "test160" << '\n';
      h_rec_pt_lep_sideband_sd->fill(event);
      if(derror) cout << "test161" << '\n';
      passed_rec_pt_lep_sideband_sd = true;
    }

    if(derror) cout << "test162" << '\n';
    if(passed_rec_kin && !passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && passed_rec_dr_puppi && passed_rec_mass_puppi && pt_lep_rec_sideband->passes(event)){
      if(derror) cout << "test163" << '\n';
      h_rec_pt_lep_sideband_puppi->fill(event);
      if(derror) cout << "test164" << '\n';
      passed_rec_pt_lep_sideband_puppi = true;
    }

    if(derror) cout << "test165" << '\n';
    if(passed_rec_kin && !passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && passed_rec_dr_puppi_sd && passed_rec_mass_puppi_sd && pt_lep_rec_sideband->passes(event)){
      if(derror) cout << "test166" << '\n';
      h_rec_pt_lep_sideband_puppi_sd->fill(event);
      if(derror) cout << "test167" << '\n';
      passed_rec_pt_lep_sideband_puppi_sd = true;
    }

    if(derror) cout << "test168" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && !passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && passed_rec_mass && pt_topjet_rec_sideband->passes(event)){
      if(derror) cout << "test169" << '\n';
      h_rec_pt_topjet_sideband->fill(event);
      if(derror) cout << "test170" << '\n';
      passed_rec_pt_topjet_sideband = true;
    }

    if(derror) cout << "test171" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && !passed_rec_pt_topjet_sd && passed_rec_ntopjet && passed_rec_dr_sd && passed_rec_mass_sd && pt_topjet_rec_sideband_sd->passes(event)){
      if(derror) cout << "test172" << '\n';
      h_rec_pt_topjet_sideband_sd->fill(event);
      if(derror) cout << "test173" << '\n';
      passed_rec_pt_topjet_sideband_sd= true;
    }

    if(derror) cout << "test174" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && !passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && passed_rec_dr_puppi && passed_rec_mass_puppi && pt_topjet_rec_sideband_puppi->passes(event)){
      if(derror) cout << "test175" << '\n';
      h_rec_pt_topjet_sideband_puppi->fill(event);
      if(derror) cout << "test176" << '\n';
      passed_rec_pt_topjet_sideband_puppi = true;
    }

    if(derror) cout << "test177" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && !passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && passed_rec_dr_puppi_sd && passed_rec_mass_puppi_sd && pt_topjet_rec_sideband_puppi_sd->passes(event)){
      if(derror) cout << "test178" << '\n';
      h_rec_pt_topjet_sideband_puppi_sd->fill(event);
      if(derror) cout << "test179" << '\n';
      passed_rec_pt_topjet_sideband_puppi_sd = true;
    }

    if(derror) cout << "test180" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet && !passed_rec_dr && passed_rec_mass && dr_rec_sideband->passes(event)){
      if(derror) cout << "test181" << '\n';
      h_rec_dr_sideband->fill(event);
      if(derror) cout << "test182" << '\n';
      passed_rec_dr_sideband = true;
    }

    if(derror) cout << "test183" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet && !passed_rec_dr_sd && passed_rec_mass_sd && dr_rec_sideband_sd->passes(event)){
      if(derror) cout << "test184" << '\n';
      h_rec_dr_sideband_sd->fill(event);
      if(derror) cout << "test185" << '\n';
      passed_rec_dr_sideband_sd = true;
    }

    if(derror) cout << "test186" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && !passed_rec_dr_puppi && passed_rec_mass_puppi && dr_rec_sideband_puppi->passes(event)){
      if(derror) cout << "test187" << '\n';
      h_rec_dr_sideband_puppi->fill(event);
      if(derror) cout << "test188" << '\n';
      passed_rec_dr_sideband_puppi = true;
    }

    if(derror) cout << "test189" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && !passed_rec_dr_puppi_sd && passed_rec_mass_puppi_sd && dr_rec_sideband_puppi_sd->passes(event)){
      if(derror) cout << "test190" << '\n';
      h_rec_dr_sideband_puppi_sd->fill(event);
      if(derror) cout << "test191" << '\n';
      passed_rec_dr_sideband_puppi_sd = true;
    }

    if(derror) cout << "test192" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet && passed_rec_ntopjet && passed_rec_dr && !passed_rec_mass){
      if(derror) cout << "test193" << '\n';
      h_rec_mass_sideband->fill(event);
      if(derror) cout << "test194" << '\n';
      passed_rec_mass_sideband = true;
    }

    if(derror) cout << "test195" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_sd && passed_rec_ntopjet && passed_rec_dr_sd && !passed_rec_mass_sd){
      if(derror) cout << "test196" << '\n';
      h_rec_mass_sideband_sd->fill(event);
      if(derror) cout << "test197" << '\n';
      passed_rec_mass_sideband_sd = true;

    }

    if(derror) cout << "test198" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi && passed_rec_ntopjet_puppi && passed_rec_dr_puppi && !passed_rec_mass_puppi){
      if(derror) cout << "test199" << '\n';
      h_rec_mass_sideband_puppi->fill(event);
      if(derror) cout << "test200" << '\n';
      passed_rec_mass_sideband_puppi = true;
    }

    if(derror) cout << "test201" << '\n';
    if(passed_rec_kin && passed_rec_pt_lep && passed_rec_pt_topjet_puppi_sd && passed_rec_ntopjet_puppi && passed_rec_dr_puppi_sd && !passed_rec_mass_puppi_sd){
      if(derror) cout << "test202" << '\n';
      h_rec_mass_sideband_puppi_sd->fill(event);
      if(derror) cout << "test203" << '\n';
      passed_rec_mass_sideband_puppi_sd = true;
    }

    if(derror) cout << "test204" << '\n';
    if(passed_rec_final && mass_lower->passes(event))  h_mass_lower->fill(event);
    if(derror) cout << "test205" << '\n';
    if(passed_rec_final_sd && mass_lower_sd->passes(event))  h_mass_lower_sd->fill(event);
    if(derror) cout << "test206" << '\n';
    if(passed_rec_final && mass_higher->passes(event)) h_mass_higher->fill(event);
    if(derror) cout << "test207" << '\n';
    if(passed_rec_final_sd && mass_higher_sd->passes(event)) h_mass_higher_sd->fill(event);
    if(derror) cout << "test208" << '\n';

    if(passed_rec_final_puppi && mass_lower_puppi->passes(event))  h_mass_lower_puppi->fill(event);
    if(derror) cout << "test209" << '\n';
    if(passed_rec_final_puppi_sd && mass_lower_puppi_sd->passes(event))  h_mass_lower_puppi_sd->fill(event);
    if(derror) cout << "test200" << '\n';
    if(passed_rec_final_puppi && mass_higher_puppi->passes(event)) h_mass_higher_puppi->fill(event);
    if(derror) cout << "test201" << '\n';
    if(passed_rec_final_puppi_sd && mass_higher_puppi_sd->passes(event)) h_mass_higher_puppi_sd->fill(event);
    if(derror) cout << "test202" << '\n';

    event.set(h_passed_rec_final, passed_rec_final);
    event.set(h_passed_rec_final_sd, passed_rec_final_sd);
    event.set(h_passed_rec_final_puppi, passed_rec_final_puppi);
    event.set(h_passed_rec_final_puppi_sd, passed_rec_final_puppi_sd);
    event.set(h_passed_gen_final, passed_gen_final);
    event.set(h_passed_gen_final_sd, passed_gen_final_sd);
    event.set(h_passed_gen_pt_lep_sideband, passed_gen_pt_lep_sideband);
    event.set(h_passed_gen_pt_lep_sideband_sd, passed_gen_pt_lep_sideband_sd);
    event.set(h_passed_gen_dr_sideband, passed_gen_dr_sideband);
    event.set(h_passed_gen_dr_sideband_sd, passed_gen_dr_sideband_sd);
    event.set(h_passed_gen_mass_sideband, passed_gen_mass_sideband);
    event.set(h_passed_gen_mass_sideband_sd, passed_gen_mass_sideband_sd);
    event.set(h_passed_gen_pt_topjet_sideband, passed_gen_pt_topjet_sideband);
    event.set(h_passed_gen_pt_topjet_sideband_sd, passed_gen_pt_topjet_sideband_sd);
    event.set(h_passed_rec_dr_sideband, passed_rec_dr_sideband);
    event.set(h_passed_rec_dr_sideband_sd, passed_rec_dr_sideband_sd);
    event.set(h_passed_rec_dr_sideband_puppi, passed_rec_dr_sideband_puppi);
    event.set(h_passed_rec_dr_sideband_puppi_sd, passed_rec_dr_sideband_puppi_sd);
    event.set(h_passed_rec_mass_sideband, passed_rec_mass_sideband);
    event.set(h_passed_rec_mass_sideband_sd, passed_rec_mass_sideband_sd);
    event.set(h_passed_rec_mass_sideband_puppi, passed_rec_mass_sideband_puppi);
    event.set(h_passed_rec_mass_sideband_puppi_sd, passed_rec_mass_sideband_puppi_sd);
    event.set(h_passed_rec_pt_lep_sideband, passed_rec_pt_lep_sideband);
    event.set(h_passed_rec_pt_lep_sideband_sd, passed_rec_pt_lep_sideband_sd);
    event.set(h_passed_rec_pt_lep_sideband_puppi, passed_rec_pt_lep_sideband_puppi);
    event.set(h_passed_rec_pt_lep_sideband_puppi_sd, passed_rec_pt_lep_sideband_puppi_sd);
    event.set(h_passed_rec_pt_topjet_sideband, passed_rec_pt_topjet_sideband);
    event.set(h_passed_rec_pt_topjet_sideband_sd, passed_rec_pt_topjet_sideband_sd);
    event.set(h_passed_rec_pt_topjet_sideband_puppi, passed_rec_pt_topjet_sideband_puppi);
    event.set(h_passed_rec_pt_topjet_sideband_puppi_sd, passed_rec_pt_topjet_sideband_puppi_sd);

    if(derror) cout << "test203" << '\n';
    if(event.topjets->size() > 0){
      if(derror) cout << "test204" << '\n';
      event.set(h_pt_rec, event.topjets->at(0).pt());
      if(derror) cout << "test205" << '\n';
      double tau32_rec = event.topjets->at(0).tau3()/event.topjets->at(0).tau2();
      if(derror) cout << "test206" << '\n';
      double tau32_rec_sd = event.topjets->at(0).tau3_groomed()/event.topjets->at(0).tau2_groomed();
      if(derror) cout << "test207" << '\n';
      event.set(h_tau32_rec, tau32_rec);
      event.set(h_tau32_rec_sd, tau32_rec_sd);
      if(derror) cout << "test208" << '\n';
      event.set(h_mass_rec, event.topjets->at(0).v4().M());
      if(derror) cout << "test209" << '\n';

      LorentzVector subjet_sum1;
      for (const auto s : event.topjets->at(0).subjets()) {
        subjet_sum1 += s.v4();
      }
      if(derror) cout << "test210" << '\n';
      double mass_topjet = subjet_sum1.M();
      event.set(h_mass_rec_sd, mass_topjet);
      if(derror) cout << "test211" << '\n';
    }
    else{
      if(derror) cout << "test212" << '\n';
      event.set(h_pt_rec, -100);
      event.set(h_tau32_rec, -100);
      event.set(h_tau32_rec_sd, -100);
      event.set(h_mass_rec, -100);
      event.set(h_mass_rec_sd, -100);
      if(derror) cout << "test213" << '\n';
    }
    if(derror) cout << "test214" << '\n';

    if(h_puppi.size() > 0){
      if(derror) cout << "test215" << '\n';
      event.set(h_pt_rec_puppi, h_puppi.at(0).pt());
      if(derror) cout << "test216" << '\n';
      double tau32_rec = h_puppi.at(0).tau3()/h_puppi.at(0).tau2();
      if(derror) cout << "test217" << '\n';
      double tau32_rec_sd = h_puppi.at(0).tau3_groomed()/h_puppi.at(0).tau2_groomed();
      if(derror) cout << "test218" << '\n';
      event.set(h_tau32_rec_puppi, tau32_rec);
      event.set(h_tau32_rec_puppi_sd, tau32_rec_sd);

      if(derror) cout << "test219" << '\n';
      event.set(h_mass_rec_puppi, h_puppi.at(0).v4().M());

      if(derror) cout << "test220" << '\n';
      LorentzVector subjet_sum1;
      for (const auto s : h_puppi.at(0).subjets()) {
        subjet_sum1 += s.v4();
      }
      if(derror) cout << "test221" << '\n';
      double mass_topjet = subjet_sum1.M();
      if(derror) cout << "test222" << '\n';
      // cout << "mass_topjet: " << mass_topjet << '\n';
      // cout << ".softdropmass(): " << h_puppi.at(0).softdropmass() << '\n';
      event.set(h_mass_rec_puppi_sd, mass_topjet);
      if(derror) cout << "test223" << '\n';
    }
    else{
      if(derror) cout << "test224" << '\n';
      event.set(h_pt_rec_puppi, -100);
      event.set(h_tau32_rec_puppi, -100);
      event.set(h_tau32_rec_puppi_sd, -100);
      event.set(h_mass_rec_puppi, -100);
      event.set(h_mass_rec_puppi_sd, -100);
      if(derror) cout << "test225" << '\n';
    }

    if(derror) cout << "test226" << '\n';
    if(isMC && event.gentopjets->size() > 0){
      if(derror) cout << "test227" << '\n';
      // h_mass & h_mass_gen are the same! need to fix this later!
      event.set(h_pt_gen_sd, event.gentopjets->at(0).pt());
      if(derror) cout << "test228" << '\n';
      double tau32_gen = event.gentopjets->at(0).tau3()/event.gentopjets->at(0).tau2();
      if(derror) cout << "test229" << '\n';
      event.set(h_tau32_gen_sd, tau32_gen);
      event.set(h_mass_gen_sd, event.gentopjets->at(0).v4().M());
      if(derror) cout << "test230" << '\n';
    }
    else{
      if(derror) cout << "test235" << '\n';
      event.set(h_pt_gen_sd, -100);
      event.set(h_tau32_gen_sd, -100);
      event.set(h_mass_gen_sd, -100);
      if(derror) cout << "test236" << '\n';
    }
    if(isMC && gentopjet.size() > 0){
      if(derror) cout << "test231" << '\n';
      // h_mass & h_mass_gen are the same! need to fix this later!
      event.set(h_pt_gen, gentopjet.at(0).pt());
      if(derror) cout << "test232" << '\n';
      double tau32_gen = gentopjet.at(0).tau3()/gentopjet.at(0).tau2();
      if(derror) cout << "test233" << '\n';
      event.set(h_tau32_gen, tau32_gen);
      event.set(h_mass_gen, gentopjet.at(0).v4().M());
      if(derror) cout << "test234" << '\n';
    }
    else{
      if(derror) cout << "test235" << '\n';
      event.set(h_pt_gen, -100);
      event.set(h_tau32_gen, -100);
      event.set(h_mass_gen, -100);
      if(derror) cout << "test236" << '\n';
    }
    h_event->fill(event);
    if(derror) cout << "test237" << '\n';
    if(!passed_rec_final&& !passed_rec_final_sd &&
      !passed_rec_final_puppi && !passed_rec_final_puppi_sd &&
      !passed_gen_final && !passed_gen_final_sd &&
      !passed_gen_pt_lep_sideband && !passed_gen_pt_lep_sideband_sd &&
      !passed_gen_pt_topjet_sideband && !passed_gen_pt_topjet_sideband_sd &&
      !passed_gen_dr_sideband && !passed_gen_dr_sideband_sd &&
      !passed_gen_mass_sideband && !passed_gen_mass_sideband_sd &&
      !passed_rec_pt_lep_sideband && !passed_rec_pt_lep_sideband_sd &&
      !passed_rec_pt_lep_sideband_puppi && !passed_rec_pt_lep_sideband_puppi_sd &&
      !passed_rec_pt_topjet_sideband && !passed_rec_pt_topjet_sideband_sd &&
      !passed_rec_pt_topjet_sideband_puppi && !passed_rec_pt_topjet_sideband_puppi_sd &&
      !passed_rec_dr_sideband && !passed_rec_dr_sideband_sd &&
      !passed_rec_dr_sideband_puppi && !passed_rec_dr_sideband_puppi_sd &&
      !passed_rec_mass_sideband &&   !passed_rec_mass_sideband_sd &&
      !passed_rec_mass_sideband_puppi && !passed_rec_mass_sideband_puppi_sd) return false;

      if(derror) cout << "test238" << '\n';
      // 3. decide whether or not to keep the current event in the output:
      return true;
    }

    // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
    // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
    UHH2_REGISTER_ANALYSIS_MODULE(PostKinCutModule)

  }
