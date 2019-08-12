#include <UHH2/TopSubstructure/include/PuppiJetCorrections.h>

using namespace uhh2;
using namespace std;

void PuppiJetCorrections::fail_if_init() const{
  if(init_done){
    throw invalid_argument("PuppiJetCorrections::init already called!");
  }
}

PuppiJetCorrections::PuppiJetCorrections(){
  tjec_tag_2016 = "Summer16_07Aug2017";
  tjec_ver_2016 = "11";

  tjec_tag_2017 = "Fall17_17Nov2017";
  tjec_ver_2017 = "32";

  tjec_tag_2018 = "Autumn18";
  tjec_ver_2018 = "7";

  tjec_pjet_coll = "dummy";
}

void PuppiJetCorrections::init(Context & ctx){
  if(init_done){
    throw invalid_argument("PuppiJetCorrections::init called twice!");
  }
  init_done = true;

  is_mc = ctx.get("dataset_type") == "MC";
  year = extract_year(ctx);

  // setup correction pjet type for JECs
  std::string userTopJetColl = string2lowercase("jetsAk8PuppiSubstructure_SoftDropPuppi");
  // h_puppi          = string2lowercase(ctx.get_handle<std::vector<TopJet>>("jetsAk8PuppiSubstructure_SoftDropPuppi"));

  std::string algo = "";
  // algo size
  if (userTopJetColl.find("ak4") != std::string::npos) {
    algo = "AK4";
  }
  else if(userTopJetColl.find("ak8") != std::string::npos){
    algo = "AK8";
  }
  else if (userTopJetColl.find("ak8") == std::string::npos) {
    std::cout << "PuppiJetCorrections.cxx: Cannot determine pjet cone + radius (neither AK4 nor AK8) - going to assume it is AK8 for JECs" << '\n';
    algo = "AK8";
  }

  std::string pus = "";
  // Pileup subtraction
  if (userTopJetColl.find("puppi") != std::string::npos) {
    pus = "PFPuppi";
  }
  else if (userTopJetColl.find("chs") != std::string::npos) {
    pus = "PFchs";
  }
  else if (userTopJetColl.find("puppi") == std::string::npos) {
    std::cout << "Cannot determine pileup subtraction (neither CHS nor PUPPI) - going to assume it is Puppi for JECs" << std::endl;
    pus = "PFPuppi";
  }
  tjec_pjet_coll = algo + pus;

  if(is_mc){
    pjet_corrector_MC.reset(new YearSwitcher(ctx));
    pjet_corrector_MC->setup2016(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(tjec_tag_2016, tjec_ver_2016, tjec_pjet_coll), "jetsAk8PuppiSubstructure_SoftDropPuppi"));
    pjet_corrector_MC->setup2017(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(tjec_tag_2017, tjec_ver_2017, tjec_pjet_coll), "jetsAk8PuppiSubstructure_SoftDropPuppi"));
    pjet_corrector_MC->setup2018(std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesMC(tjec_tag_2018, tjec_ver_2018, tjec_pjet_coll), "jetsAk8PuppiSubstructure_SoftDropPuppi"));

    JERSmearing::SFtype1 JER_sf;
    std::string resFilename = "";
    if (year == Year::is2016v2 || year == Year::is2016v3) {
      JER_sf = JERSmearing::SF_13TeV_Summer16_25nsV1;
      resFilename = "2016/Summer16_25nsV1_MC_PtResolution_"+algo+pus+".txt";
    } else if (year == Year::is2017v1 || year == Year::is2017v2) {
      JER_sf = JERSmearing::SF_13TeV_Fall17_V3;
      resFilename = "2017/Fall17_V3_MC_PtResolution_"+algo+pus+".txt";
    } else if (year == Year::is2018) {
      JER_sf = JERSmearing::SF_13TeV_Autumn18_RunABCD_V4;
      resFilename = "2018/Autumn18_V4_MC_PtResolution_"+algo+pus+".txt";
    } else {
      throw runtime_error("Cannot find suitable jet resolution file & scale factors for this year for JetResolutionSmearer");
    }

    pjet_resolution_smearer.reset(new GenericJetResolutionSmearer(ctx, "jetsAk8PuppiSubstructure_SoftDropPuppi", "gentopjets", JER_sf, resFilename));
  }
  else{
    tjec_switcher_16.reset(new RunSwitcher(ctx, "2016"));
    for (const auto & runItr : runPeriods2016) { // runPeriods defined in common/include/Utils.h
      tjec_switcher_16->setupRun(runItr, std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesDATA(tjec_tag_2016, tjec_ver_2016, tjec_pjet_coll, runItr), "jetsAk8PuppiSubstructure_SoftDropPuppi"));
    }

    tjec_switcher_17.reset(new RunSwitcher(ctx, "2017"));
    for (const auto & runItr : runPeriods2017) {
      tjec_switcher_17->setupRun(runItr, std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesDATA(tjec_tag_2017, tjec_ver_2017, tjec_pjet_coll, runItr), "jetsAk8PuppiSubstructure_SoftDropPuppi"));
    }

    tjec_switcher_18.reset(new RunSwitcher(ctx, "2018"));
    for (const auto & runItr : runPeriods2018) {
      tjec_switcher_18->setupRun(runItr, std::make_shared<GenericTopJetCorrector>(ctx, JERFiles::JECFilesDATA(tjec_tag_2018, tjec_ver_2018, tjec_pjet_coll, runItr), "jetsAk8PuppiSubstructure_SoftDropPuppi"));
    }

    pjet_corrector_data.reset(new YearSwitcher(ctx));
    pjet_corrector_data->setup2016(tjec_switcher_16);
    pjet_corrector_data->setup2017(tjec_switcher_17);
    pjet_corrector_data->setup2018(tjec_switcher_18);
  }
}

bool PuppiJetCorrections::process(uhh2::Event & event){
  if(!init_done){
    throw runtime_error("PuppiJetCorrections::init not called (has to be called in AnalysisModule constructor)");
  }
  if (is_mc) {
    pjet_corrector_MC->process(event);
  } else {
    pjet_corrector_data->process(event);
  }

  if(is_mc) pjet_resolution_smearer->process(event);
  return true;
}