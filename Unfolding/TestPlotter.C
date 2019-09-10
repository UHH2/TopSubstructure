// #include "TestPlotter.h"
#include "iostream"
#include "Plots.h"
#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TCanvas.h"
#include <TLegend.h>
#include <TStyle.h>
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TUnfoldBinningXML.h"
#include <TLine.h>
#include <TLatex.h>
#include <TVectorD.h>
#include <boost/algorithm/string.hpp>

using namespace std;

int FindLargestVariation(vector<TH1*> variations);
TH1* ConvertToRelative(TH1* sys, TH1* central);
TH1* SetSysError(TH1* data_unfolded, TH2* CovTotal);
TH1* CreateDeltaFromCov(TH2* Cov);
TH1* AddSys(vector<TH1*> sys);
TH1* GetModelDelta(TH1* unfolded, TH1* truth);
TH2* CreateCovFromDelta(TH1* delta, TH2* dummyCov);
int FindLargestVariationByMean(vector<TH1D*> truth, vector<TH1*> variations, bool use_significance);
TH1* ConstructAverage(vector<TH1*> variations, int varfrom);
TH1* ConvertToNewBinning_1D(TH1*, vector<int>, const TUnfoldBinning*, TString);
TH2* ConvertToNewBinning_2D(TH2*, vector<int>, const TUnfoldBinning*, TString);
TH1* ConvertToCrossSection(TH1* hist);

int main(int argc, char* argv[]){
  if(argc < 2) throw runtime_error("How to use Plotter: ./Plotter <Filename>");
  TString directory = "/afs/desy.de/user/s/skottkej/Plots/TEST/";
  TString File_name = argv[1];
  // vector<Int_t> binmap = {1,1,1,2,2,3,4,5,5};
  vector<Int_t> binmap = {1,2,3,4,5};

  bool isdata = false;
  bool ispseudodata = false;
  if(File_name.Contains("data")) isdata = true;
  if(File_name.Contains("madgraph")) ispseudodata = true;

  vector<TString> background_names = {"DYJets", "ST", "Diboson", "WJets"};
  vector<vector<TString>> sys_name;
  if(File_name.Contains("mu")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
  else if(File_name.Contains("ele")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
  else if(File_name.Contains("comb")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};

  vector<vector<TString>> model_name;
  if(isdata) model_name = {{"scale_uu", "scale_un", "scale_nu", "scale_nd", "scale_dn", "scale_dd"}, {"mtop1695", "mtop1715", "mtop1735", "mtop1755"}, {"isrup", "isrdown"}, {"fsrup", "fsrdown"}, {"hdampup", "hdampdown"}};
  if(ispseudodata) model_name = {{"scale_uu", "scale_un", "scale_nu", "scale_nd", "scale_dn", "scale_dd"}, {"mtop1695", "mtop1715", "mtop1735", "mtop1755"}, {"isrup", "isrdown"}, {"fsrup", "fsrdown"}, {"hdampup", "hdampdown"}};

  TFile *file = new TFile(File_name, "READ");
  TString jetcol = "";
  if(File_name.Contains("puppi_sd")) jetcol = "_puppi_sd";
  else if(File_name.Contains("puppi")) jetcol = "_puppi";
  else if(File_name.Contains("sd")) jetcol = "_sd";

  TUnfoldBinning* binning_rec = (TUnfoldBinning*) file->Get("binning_rec");
  TUnfoldBinning* binning_gen = (TUnfoldBinning*) file->Get("binning_gen");
  TH1D* projection_gen = (TH1D*) file->Get("Projection of gen distribution TauScan");
  TH1D* projection_rec = (TH1D*) file->Get("Projection of rec distribution TauScan");
  TH1* h_input_gen = (TH1*) file->Get("MC gen Distribution");
  TH1* h_input_rec = (TH1*) file->Get("MC rec input");

  Plotter *plot = new Plotter(binning_gen, binning_rec);

  string input(argv[1]);
  vector<string> result;
  boost::split(result, input, boost::is_any_of("_ ."));
  TString dataset = "";
  int count = 0;
  TString data= "";
  for(unsigned int i = 0; i < result.size(); i++){
    if(result[i] != "mu" && result[i] != "ele" && result[i] != "comb"){
      if(i > 0 && i < result.size()-1){
        directory += result[i];
        if(i==1) data = result[i];
        if(i==2){
          if(data.Contains("scale")){
            data += "_"+result[i];
          }
        }
        if(result[i] != "sd" && result[i] != "puppi"){
          dataset += result[i];
          if(result[i] != "scale") count++;
        }
      }
      if(i < result.size() - 3 && i > 0){
        directory += "_";
        if(count == 0) dataset += "_";
      }
    }
  }
  // cout << "data: " << data << '\n';
  // cout << "dataset: " << dataset << '\n';
  directory += "/";
  TString jetcol2 = jetcol;
  TString channel = "";
  if(File_name.Contains("mu"))        directory += "mu/";
  else if(File_name.Contains("ele"))  directory += "ele/";
  else if(File_name.Contains("comb")) directory += "comb/";
  if(File_name.Contains("mu"))        channel += "_mu";
  else if(File_name.Contains("ele"))  channel += "_ele";
  else if(File_name.Contains("comb")) channel += "_comb";
  jetcol += channel;

  plot->Plot_projections(projection_gen, h_input_gen, projection_rec, h_input_rec, directory);

  TH1D* h_unfolded_tauscan = (TH1D*) file->Get("Unfolded data (meas region) TauScan");
  // h_unfolded_tauscan->SetName("unfolded "+data);
  TH1* h_unfolded_tauscan_newbinning = ConvertToNewBinning_1D(h_unfolded_tauscan, binmap, binning_gen, "Unfolded data (meas region) TauScan new binning");
  TH1D* h_unfolded_tauscan_part = (TH1D*) file->Get("Unfolded data (meas region, m>155) TauScan");
  TH1D* h_data_truth;
  if(isdata){
    TFile* blub;
    if(File_name.Contains("puppi_sd")) blub = new TFile("Unfoldings/Unfolding_madgraph_puppi_sd"+channel+".root", "READ");
    if(File_name.Contains("puppi")) blub = new TFile("Unfoldings/Unfolding_madgraph_puppi"+channel+".root", "READ");
    if(File_name.Contains("sd")) blub = new TFile("Unfoldings/Unfolding_madgraph_sd"+channel+".root", "READ");
    else blub = new TFile("Unfoldings/Unfolding_madgraph"+channel+".root", "READ");
    h_data_truth = (TH1D*) blub->Get("Data Truth");
  }
  else h_data_truth = (TH1D*) file->Get("Data Truth");
  // if(File_name.Contains("data")) h_data_truth->SetName("POWHEG truth");
  // else h_data_truth->SetName(data+" truth");
  TH1* h_data_truth_newbinning = ConvertToNewBinning_1D(h_data_truth, binmap, binning_gen, "Data Truth new binning");
  TH1D* h_mc_truth;
  if(isdata) h_mc_truth = (TH1D*) file->Get("MC Truth");
  else h_mc_truth = (TH1D*) file->Get("Bias Truth TauScan");
  // h_mc_truth->SetName("POWHEG");
  TH1* h_mc_truth_newbinning = ConvertToNewBinning_1D(h_mc_truth, binmap, binning_gen, "MC Truth new binning");

  plot->Plot_output(h_unfolded_tauscan, h_data_truth, false, directory, "Unfolding_result_Tauscan");
  plot->Plot_output(h_unfolded_tauscan, h_data_truth, true, directory, "Unfolding_result_normalised_Tauscan");
  plot->Plot_output(h_unfolded_tauscan_newbinning, h_data_truth_newbinning, false, directory+"New_binning/", "Unfolding_result_Tauscan");
  plot->Plot_output(h_unfolded_tauscan_newbinning, h_data_truth_newbinning, true, directory+"New_binning/", "Unfolding_result_normalised_Tauscan");
  plot->Plot_output(h_unfolded_tauscan_part, h_data_truth, false, directory, "Unfolding_result_Tauscan_part");
  plot->Plot_output(h_unfolded_tauscan_part, h_data_truth, true, directory, "Unfolding_result_normalised_Tauscan_part");

  TH1D* h_data_truth_all = (TH1D*) file->Get("Whole Data Truth");
  TH1D* h_unfolded_tauscan_all = (TH1D*) file->Get("Unfolded data (all) TauScan");
  plot->Plot_output(h_unfolded_tauscan_all, h_data_truth_all, false, directory, "Unfolding_result_all_Tauscan");

  TH2D* h_covariance_input_tauscan = (TH2D*) file->Get("Covariance of input (meas region) TauScan");
  TH2D* h_covariance_covmatrix_tauscan = (TH2D*) file->Get("Covariance of CovMatrix (meas region) TauScan");

  TH2D* h_response = (TH2D*) file->Get("response matrix");
  plot->Plot_ResponseMatrix(h_response, directory+"Migration_Matrix");

  TH2D* h_probability = (TH2D*) file->Get("Probability Matrix TauScan");
  plot->Plot_ResponseMatrix(h_probability, directory+"Probability_Matrix");



  vector<vector<TH1*>> model_output, model_delta, model_output_newbinning, model_delta_newbinning, model_truth_newbinning;
  vector<vector<TH1D*>> model_truth;
  vector<vector<TH2*>> CovModel, CovModel_newbinning;
  if(isdata){
    for(unsigned int i=0; i<model_name.size(); i++){
      vector<TH1*> dummy;
      model_output.push_back(dummy);
      model_delta.push_back(dummy);
      model_output_newbinning.push_back(dummy);
      model_delta_newbinning.push_back(dummy);
      vector<TH1D*> dummyd;
      vector<TH1*> dummyd2;
      model_truth.push_back(dummyd);
      model_truth_newbinning.push_back(dummyd2);

      vector<TH2*> dummy2;
      CovModel.push_back(dummy2);
      CovModel_newbinning.push_back(dummy2);
      for(unsigned int j=0; j<model_name[i].size(); j++){
        TFile *file_model = new TFile("Unfoldings/Unfolding_"+model_name[i][j]+jetcol+".root", "READ");
        TH1* output = (TH1*) file_model->Get("Unfolded data (meas region) TauScan");
        TH1D* truth = (TH1D*) file_model->Get("Data Truth");
        model_output[i].push_back(output);
        model_truth[i].push_back(truth);
        model_output_newbinning[i].push_back(ConvertToNewBinning_1D(output, binmap, binning_gen, "Unfolded data (meas region) TauScan new binning"));
        model_truth_newbinning[i].push_back(ConvertToNewBinning_1D(truth, binmap, binning_gen, "Data Truth new binning"));
        TH1* delta = GetModelDelta(output, model_truth[i][j]);
        TH2* cov = CreateCovFromDelta(delta, h_covariance_input_tauscan);
        model_delta[i].push_back(delta);
        CovModel[i].push_back(cov);
        model_delta_newbinning[i].push_back(ConvertToNewBinning_1D(delta, binmap, binning_gen, "Model Delta"));
        CovModel_newbinning[i].push_back(ConvertToNewBinning_2D(cov, binmap, binning_gen, "Cov Model"));
      }
    }
  }


  // add up stat Cov
  cout << "sum up stat cov matrices" << endl;
  vector<TH1*> error, error_model, error_newbinning, error_model_newbinning;
  vector<TString> error_name, error_name_model;
  TH2D* CovStat = (TH2D*) h_covariance_input_tauscan->Clone("covstat clone");
  CovStat->SetName("blub");
  TH2D* CovStat_model = (TH2D*) h_covariance_input_tauscan->Clone("covstat model clone");
  TH2* CovStat_newbinning = ConvertToNewBinning_2D(CovStat, binmap, binning_gen,"blub2");
  TH2* CovStat_model_newbinning = ConvertToNewBinning_2D(CovStat_model, binmap, binning_gen,"CovStat model");
  TH1* stat_delta = CreateDeltaFromCov(CovStat);
  TH1* stat_rel = ConvertToRelative(stat_delta, h_unfolded_tauscan);
  error.push_back(stat_rel);
  error_model.push_back(stat_rel);
  error_newbinning.push_back(ConvertToNewBinning_1D(stat_rel, binmap, binning_gen,"stat_rel"));
  error_model_newbinning.push_back(ConvertToNewBinning_1D(stat_rel, binmap, binning_gen,"stat_rel model"));
  error_name.push_back("stat");
  error_name_model.push_back("stat");

  TH2D* mc_error = (TH2D*) h_covariance_covmatrix_tauscan->Clone("mc error clone");
  CovStat->Add(h_covariance_covmatrix_tauscan);
  CovStat_model->Add(h_covariance_covmatrix_tauscan);

  CovStat_newbinning->Add(ConvertToNewBinning_2D(h_covariance_covmatrix_tauscan, binmap, binning_gen,"CovMatrix"));
  CovStat_model_newbinning->Add(ConvertToNewBinning_2D(h_covariance_covmatrix_tauscan, binmap, binning_gen,"CovMatrix Model"));

  int y = 0;
  if(isdata){
    vector<TH2*> CovBgrStat;
    vector<TH2*> CovBgrStat_newbinning;
    for(unsigned int i = 0; i < background_names.size(); i++){
      TH2D* temp_hist = (TH2D*) file->Get("Covariance of " + background_names.at(i) + " TauScan");
      CovBgrStat.push_back(temp_hist);
      CovBgrStat_newbinning.push_back(ConvertToNewBinning_2D(temp_hist, binmap, binning_gen,"CovBgrStat"+to_string(i)));
    }
    for(auto bgrcov: CovBgrStat){
      CovStat->Add(bgrcov);
      CovStat_model->Add(bgrcov);
      CovStat_newbinning->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,"CovStat"+to_string(y)));
      CovStat_model_newbinning->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,"CovStat Model"+to_string(y)));
      mc_error->Add(bgrcov);
      y++;
    }
  }
  y = 0;
  TH1* MC_stat_delta = CreateDeltaFromCov(mc_error);
  TH1* MC_stat_rel = ConvertToRelative(MC_stat_delta, h_unfolded_tauscan);

  error.push_back(MC_stat_rel);
  error_model.push_back(MC_stat_rel);
  error_newbinning.push_back(ConvertToNewBinning_1D(MC_stat_rel, binmap, binning_gen,"MC_stat_rel"));
  error_model_newbinning.push_back(ConvertToNewBinning_1D(MC_stat_rel, binmap, binning_gen,"MC_stat_rel model"));
  error_name.push_back("MC stat");
  error_name_model.push_back("MC stat");

  // // then add sys cov from backgrounds
  cout << "sum up background sys cov matrices" << endl;
  TH2* total_cov = (TH2*) CovStat->Clone("CovStat Clone");
  TH2* total_cov_newbinning = (TH2*) CovStat_newbinning->Clone("blub2new");
  vector<vector<TH1*>> sys_del, sys_del_newbinning;
  if(isdata){
    vector<TH2*> CovBgrScale;
    vector<TH2*> CovBgrScale_newbinning;
    for(unsigned int i = 0; i < background_names.size(); i++){
      TH2D* temp_hist = (TH2D*) file->Get("Scale of " + background_names.at(i) + " TauScan");
      CovBgrScale.push_back(temp_hist);
      CovBgrScale_newbinning.push_back(ConvertToNewBinning_2D(temp_hist, binmap, binning_gen,"ScaleBgr"+to_string(i)));
    }

    // cout << "size of CovBgrScale: " << CovBgrScale.size() << '\n';

    TH2* dummy_bgr_scale;
    dummy_bgr_scale = (TH2*) CovBgrScale[0]->Clone("dummy clone");
    dummy_bgr_scale->Reset();
    for(auto bgrcov: CovBgrScale) dummy_bgr_scale->Add(bgrcov);
    TH1* sys_bkg_delta;
    TH1* sys_bkg_rel;
    sys_bkg_delta = CreateDeltaFromCov(dummy_bgr_scale);
    sys_bkg_rel = ConvertToRelative(sys_bkg_delta, h_unfolded_tauscan);
    error.push_back(sys_bkg_rel);
    error_newbinning.push_back(ConvertToNewBinning_1D(sys_bkg_rel, binmap, binning_gen,"SysBkgRel"));
    error_name.push_back("background sys");
    for(auto bgrcov: CovBgrScale){
      total_cov->Add(bgrcov);
      total_cov_newbinning->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,"CovBgrScale"+to_string(y)));
      y++;
    }

    // then add sys cov (and convert used uncertainty to relative hist)
    cout << "sum up experimental sys cov matrices" << endl;

    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH1*> dummy;
      vector<TH1*> dummy_newbinning;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        TH1* temp_hist = (TH1*) file->Get("Delta of "+sys_name[i][j]+" TauScan");
        dummy.push_back(temp_hist);
        dummy_newbinning.push_back(ConvertToNewBinning_1D(temp_hist, binmap, binning_gen,"Delta Sys"+to_string(i)+to_string(j)));
      }
      sys_del.push_back(dummy);
      sys_del_newbinning.push_back(dummy_newbinning);
    }


    vector<vector<TH2*>> sys_cov;
    vector<vector<TH2*>> sys_cov_newbinning;
    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH2*> dummy;
      vector<TH2*> dummy_newbinning;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        TH2* temp_hist = (TH2*) file->Get("Covariance of "+sys_name[i][j]+"TauScan");
        dummy.push_back(temp_hist);
        dummy_newbinning.push_back(ConvertToNewBinning_2D(temp_hist, binmap, binning_gen,"Cov"+sys_name[i][j]+to_string(i)+to_string(j)));
      }
      sys_cov.push_back(dummy);
      sys_cov_newbinning.push_back(dummy_newbinning);
    }

    TH2* cov_sys = (TH2*) CovStat->Clone("covsys clone");
    TH2* cov_sys_newbinning = (TH2*) CovStat_newbinning->Clone("covsys new clone");
    cov_sys->Reset();
    cov_sys_newbinning->Reset();
    vector<TH1*> sys_rel, sys_rel_newbinning;
    for(unsigned int i=0; i<sys_del.size(); i++){
      int j = FindLargestVariation(sys_del.at(i));
      total_cov->Add(sys_cov[i][j]);
      total_cov_newbinning->Add(ConvertToNewBinning_2D(sys_cov[i][j], binmap, binning_gen,"TotalSysCov"+to_string(i)));
      cov_sys->Add(sys_cov[i][j]);
      cov_sys_newbinning->Add(ConvertToNewBinning_2D(sys_cov[i][j], binmap, binning_gen,"SysCov"+to_string(i)));
      sys_rel.push_back(ConvertToRelative(sys_del[i][j], h_unfolded_tauscan));
      sys_rel_newbinning.push_back(ConvertToNewBinning_1D(ConvertToRelative(sys_del[i][j], h_unfolded_tauscan), binmap, binning_gen,"SysRel"+to_string(i)));
      error.push_back(sys_rel[i]);
      error_newbinning.push_back(ConvertToNewBinning_1D(sys_rel[i], binmap, binning_gen,"ErrorSysRel"+to_string(i)));
      error_name.push_back(sys_name[i][j]);
    }

    cout << "sum up model sys cov matrices" << endl;
    vector<TH1*> model_delta_average, model_delta_average_newbinning;
    vector<TH1*> model_rel, model_rel_newbinning;
    for(unsigned int i=0; i<model_delta.size(); i++){
      int j = FindLargestVariationByMean(model_truth[i], model_output[i], false);
      TH1* delta_average = ConstructAverage(model_delta[i], j);
      model_delta_average.push_back(delta_average);
      model_delta_average_newbinning.push_back(ConvertToNewBinning_1D(delta_average, binmap, binning_gen,"Delta Model"+to_string(i)));
      TH2* cov_average = CreateCovFromDelta(delta_average, h_covariance_input_tauscan);
      TH1* chosen_delta;
      TH2* chosen_cov;
      chosen_cov = cov_average;
      chosen_delta = delta_average;
      model_rel.push_back(ConvertToRelative(chosen_delta, h_unfolded_tauscan));
      model_rel_newbinning.push_back(ConvertToNewBinning_1D(ConvertToRelative(chosen_delta, h_unfolded_tauscan), binmap, binning_gen,"Model Rel"+to_string(i)));
      error_model.push_back(model_rel[i]);
      error_model_newbinning.push_back(ConvertToNewBinning_1D(model_rel_newbinning[i], binmap, binning_gen,"Error Model Rel"+to_string(i)));
      error_name_model.push_back(model_name[i][j]);
      CovStat_model->Add(chosen_cov);
      CovStat_model_newbinning->Add(ConvertToNewBinning_2D(chosen_cov, binmap, binning_gen,"CovStat Model"+to_string(i)+to_string(j)));
    }
    model_rel.push_back(stat_rel);            // put in stat to get total
    model_rel_newbinning.push_back(ConvertToNewBinning_1D(stat_rel, binmap, binning_gen,"Model stat rel"));            // put in stat to get total
  }
  TH1* data_unfolded_sys = SetSysError(h_unfolded_tauscan, total_cov);
  data_unfolded_sys->SetName("Unfolded Data with sys errors");
  TH1* data_unfolded_sys_newbinning = ConvertToNewBinning_1D(SetSysError(h_unfolded_tauscan, total_cov), binmap, binning_gen,"Unfolded result");
  data_unfolded_sys_newbinning->SetName("Unfolded Data with sys errors (new binning)");

  TH1* sys_tot_delta = CreateDeltaFromCov(total_cov);

  TH1* sys_tot_rel = ConvertToRelative(sys_tot_delta, h_unfolded_tauscan);
  TH1* sys_tot_rel_newbinning = ConvertToNewBinning_1D(ConvertToRelative(sys_tot_delta, h_unfolded_tauscan), binmap, binning_gen,"Total Sys Delta");

  error.push_back(sys_tot_rel);
  error_newbinning.push_back(sys_tot_rel_newbinning);
  error_name.push_back("stat #oplus exp sys");



  TH1* model_tot_delta = CreateDeltaFromCov(CovStat_model);
  TH1* model_tot_rel = ConvertToRelative(model_tot_delta, h_unfolded_tauscan);
  TH1* model_tot_rel_newbinning = ConvertToNewBinning_1D(ConvertToRelative(model_tot_delta, h_unfolded_tauscan), binmap, binning_gen,"Totel Model Delta");
  error_model.push_back(model_tot_rel);
  error_model_newbinning.push_back(model_tot_rel_newbinning);
  error_name_model.push_back("stat #oplus model sys");

  plot->Plot_delta(model_delta, model_name, directory);
  plot->Plot_delta(model_delta_newbinning, model_name, directory+"New_binning/");

  if(isdata){
    sys_del[sys_del.size()-1].push_back(MC_stat_delta);
    sys_del_newbinning[sys_del.size()-1].push_back(ConvertToNewBinning_1D(MC_stat_delta, binmap, binning_gen,"MC Stat Delta"));
    sys_name[sys_del.size()-1].push_back("Background");
  }
  plot->Plot_delta(sys_del, sys_name, directory);
  plot->Plot_uncertainty(error, error_name, directory+"EXP_");
  plot->Plot_uncertainty(error_newbinning, error_name, directory+"New_binning/EXP_");


  // for(unsigned int i = 0; i<error_model.size()-1; i++){
  //   cout << "error Model name: " << error_name_model.at(i) << '\n';
  //   cout << "error MOdel nbins: " << error_model.at(i)->GetNbinsX() << '\n';
  // }
  TH1* h_unfolded_tauscan_cs = ConvertToCrossSection(h_unfolded_tauscan);
  TH1* data_unfolded_sys_cs = ConvertToCrossSection(data_unfolded_sys);
  TH1* h_data_truth_cs = ConvertToCrossSection(h_data_truth);
  TH1* h_mc_truth_cs = ConvertToCrossSection(h_mc_truth);



  plot->Plot_uncertainty(error_model, error_name_model, directory+"Model_");
  plot->Plot_uncertainty(error_model_newbinning, error_name_model, directory+"New_binning/Model_");
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan, data_unfolded_sys, h_data_truth, h_mc_truth, false, directory);
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan_newbinning, data_unfolded_sys_newbinning, h_data_truth_newbinning, h_mc_truth_newbinning, false, directory+"New_binning/");
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan, data_unfolded_sys, h_data_truth, h_mc_truth, true, directory+"Normalised_");
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan_cs, data_unfolded_sys_cs, h_data_truth_cs, h_mc_truth_cs, false, directory+"CS_");
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan_newbinning, data_unfolded_sys_newbinning, h_data_truth_newbinning, h_mc_truth_newbinning, true, directory+"New_binning/Normalised_");

  TH1D* h_purity_all = (TH1D*) file->Get("Purity total");
  TH1D* h_stability_all = (TH1D*) file->Get("Stability total");
  TH1* h_purity_all_newbinning = ConvertToNewBinning_1D(h_purity_all, binmap, binning_gen,"Purity all");
  TH1* h_stability_all_newbinning = ConvertToNewBinning_1D(h_stability_all, binmap, binning_gen,"Stability all");
  TH1D* h_purity_samebin = (TH1D*) file->Get("Purity samebin");
  TH1D* h_stability_samebin = (TH1D*) file->Get("Stability samebin");
  TH1* h_purity_samebin_newbinning = ConvertToNewBinning_1D(h_purity_samebin, binmap, binning_gen,"Purity samebin");
  TH1* h_stability_samebin_newbinning = ConvertToNewBinning_1D(h_stability_samebin, binmap, binning_gen,"Stability samebin");
  plot->Plot_purity(h_purity_samebin, h_purity_all, directory);
  plot->Plot_purity(h_purity_samebin_newbinning, h_purity_all_newbinning, directory+"New_binning/");
  plot->Plot_purity(h_stability_samebin, h_stability_all, directory+"Stability_");
  plot->Plot_purity(h_stability_samebin_newbinning, h_stability_all_newbinning, directory+"New_binning/Stability_");

  TSpline* logtau_x = (TSpline*) file->Get("Spline logTauX TauScan");
  TSpline* logtau_y = (TSpline*) file->Get("Spline logTauY TauScan");
  TVectorD *tau_value = (TVectorD*) file->Get("Tauvalue of TauScan");
  TVectorD *coordinate_x = (TVectorD*) file->Get("Coordinate X TauScan");
  TVectorD *coordinate_y = (TVectorD*) file->Get("Coordinate Y TauScan");
  TVectorD *coordinate_x_lcurve = (TVectorD*) file->Get("Coordinate X LCurve");
  TVectorD *coordinate_y_lcurve = (TVectorD*) file->Get("Coordinate Y LCurve");
  TGraph *lcurve = (TGraph*) file->Get("LCurve of TauScan");

  plot->Plot_LogTau(logtau_x, tau_value[0][0], coordinate_x[0][0], directory+"LogTauX");
  plot->Plot_LogTau(logtau_y, tau_value[0][0], coordinate_y[0][0], directory+"LogTauY");

  std::vector<double> coords;
  std::vector<double> coords_lcurve;

  coords.push_back(coordinate_x[0][0]);
  coords.push_back(coordinate_y[0][0]);
  coords_lcurve.push_back(coordinate_x_lcurve[0][0]);
  coords_lcurve.push_back(coordinate_y_lcurve[0][0]);
  plot->Plot_LCurve(lcurve, coords_lcurve, coords, directory);

  TSpline* rhologtau = (TSpline*) file->Get("RhoLogTau TauScan");
  plot->Plot_RhoLogTau(rhologtau, tau_value[0][0], directory);


  TH1* input_data_dist = (TH1*) file->Get("Data Distribution");
  TH1* input_mc_dist = (TH1*) file->Get("MC rec Distribution");
  TH1* input_data = (TH1*) file->Get("Data input");
  TH1* input_mc = (TH1*) file->Get("MC rec input");
  plot->Plot_input(input_data_dist, input_mc_dist, directory+"dist");
  plot->Plot_input(input_data, input_mc, directory);



  TH2* correlation_matrix = (TH2*) file->Get("Correlation Matrix (meas region) TauScan");
  plot->Plot_covariance(correlation_matrix, directory+"Correlation");

  TH2* correlation_matrix_new_binning = ConvertToNewBinning_2D(correlation_matrix, binmap, binning_gen, "Correlation Matrix (meas region) TauScan new binning");
  plot->Plot_covariance(correlation_matrix_new_binning, directory+"New_binning/Correlation");

  TH2* cov_matrix_input = (TH2*) file->Get("Covariance of input (meas region) TauScan");
  plot->Plot_covariance(cov_matrix_input, directory+"Covariance_input");

  TH2* cov_matrix_input_new_binning = ConvertToNewBinning_2D(cov_matrix_input, binmap, binning_gen, "Covariance of input (meas region) TauScan new binning");
  plot->Plot_covariance(cov_matrix_input_new_binning, directory+"New_binning/Covariance_input");

  TH2* cov_matrix_input_all = (TH2*) file->Get("Covariance of input (all) TauScan");
  plot->Plot_covariance(cov_matrix_input_all, directory+"Covariance_input_all");

  if(directory.Contains("pseudo")){
    TFile *file_pseudo1 = new TFile("Unfoldings/Unfolding_Pseudo_1"+jetcol+".root", "READ");
    TFile *file_pseudo2 = new TFile("Unfoldings/Unfolding_Pseudo_2"+jetcol+".root", "READ");
    TFile *file_pseudo3 = new TFile("Unfoldings/Unfolding_Pseudo_3"+jetcol+".root", "READ");

    TH1* h_pseudo1 = (TH1*) file_pseudo1->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo1_new = ConvertToNewBinning_1D(h_pseudo1, binmap, binning_gen,"Unfolded data (meas region) TauScan new binning");
    TH1* pseudo1_unfolded_sys = SetSysError(h_pseudo1, total_cov);
    TH1* pseudo1_unfolded_sys_new = SetSysError(h_pseudo1_new, total_cov_newbinning);
    pseudo1_unfolded_sys->SetName("Pseudo1 test");
    pseudo1_unfolded_sys_new->SetName("Pseudo1 test new");
    TH1* h_pseudo1_truth = (TH1*) file_pseudo1->Get("Data Truth");
    TH1* h_pseudo1_truth_new = ConvertToNewBinning_1D(h_pseudo1_truth, binmap, binning_gen,"Data Truth new binning");

    TH1* h_pseudo2 = (TH1*) file_pseudo2->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo2_new = ConvertToNewBinning_1D(h_pseudo2, binmap, binning_gen,"Unfolded data (meas region) TauScan new binning");
    TH1* pseudo2_unfolded_sys = SetSysError(h_pseudo2, total_cov);
    TH1* pseudo2_unfolded_sys_new = SetSysError(h_pseudo2_new, total_cov_newbinning);
    pseudo2_unfolded_sys->SetName("Pseudo2 test");
    pseudo2_unfolded_sys_new->SetName("Pseudo2 test new");
    TH1* h_pseudo2_truth = (TH1*) file_pseudo2->Get("Data Truth");
    TH1* h_pseudo2_truth_new = ConvertToNewBinning_1D(h_pseudo2_truth, binmap, binning_gen,"Data Truth new binning");

    TH1* h_pseudo3 = (TH1*) file_pseudo3->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo3_new = ConvertToNewBinning_1D(h_pseudo3, binmap, binning_gen,"Unfolded data (meas region) TauScan new binning");
    TH1* pseudo3_unfolded_sys = SetSysError(h_pseudo3, total_cov);
    TH1* pseudo3_unfolded_sys_new = SetSysError(h_pseudo3_new, total_cov_newbinning);
    pseudo3_unfolded_sys->SetName("Pseudo3 test");
    pseudo3_unfolded_sys_new->SetName("Pseudo3 test new");
    TH1* h_pseudo3_truth = (TH1*) file_pseudo3->Get("Data Truth");
    TH1* h_pseudo3_truth_new = ConvertToNewBinning_1D(h_pseudo3_truth, binmap, binning_gen,"Data Truth new binning");
    TH1* h_pseudo1_cs = ConvertToCrossSection(pseudo1_unfolded_sys);
    TH1* h_pseudo1_truth_cs = ConvertToCrossSection(h_pseudo1_truth);
    TH1* h_pseudo2_cs = ConvertToCrossSection(pseudo2_unfolded_sys);
    TH1* h_pseudo2_truth_cs = ConvertToCrossSection(h_pseudo2_truth);
    TH1* h_pseudo3_cs = ConvertToCrossSection(pseudo3_unfolded_sys);
    TH1* h_pseudo3_truth_cs = ConvertToCrossSection(h_pseudo3_truth);


    plot->Plot_all_pseudo(pseudo1_unfolded_sys, h_pseudo1_truth, pseudo2_unfolded_sys, h_pseudo2_truth, pseudo3_unfolded_sys, h_pseudo3_truth, false, directory+"All_uncer_");
    plot->Plot_all_pseudo(h_pseudo1, h_pseudo1_truth, h_pseudo2, h_pseudo2_truth, h_pseudo3, h_pseudo3_truth, false, directory);

    plot->Plot_all_pseudo(h_pseudo1_cs, h_pseudo1_truth_cs, h_pseudo2_cs, h_pseudo2_truth_cs, h_pseudo3_cs, h_pseudo3_truth_cs, true, directory+"CS_All_uncer_");

    plot->Plot_all_pseudo(pseudo1_unfolded_sys_new, h_pseudo1_truth_new, pseudo2_unfolded_sys_new, h_pseudo2_truth_new, pseudo3_unfolded_sys_new, h_pseudo3_truth_new, false, directory+"New_binning/All_uncer_");
    plot->Plot_all_pseudo(h_pseudo1_new, h_pseudo1_truth_new, h_pseudo2_new, h_pseudo2_truth_new, h_pseudo3_new, h_pseudo3_truth_new, false, directory+"New_binning/");
  }

  if(directory.Contains("comb")){
    TFile *file_mu = new TFile("Unfoldings/Unfolding_"+dataset+jetcol2+"_mu"+".root", "READ");
    TFile *file_ele = new TFile("Unfoldings/Unfolding_"+dataset+jetcol2+"_ele"+".root", "READ");

    vector<vector<TString>> sys_name_mu, sys_name_ele;
    sys_name_mu = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
    sys_name_ele = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
    TH1D* h_unfolded_tauscan_mu = (TH1D*) file_mu->Get("Unfolded data (meas region) TauScan");
    TH1* h_unfolded_tauscan_newbinning_mu = ConvertToNewBinning_1D(h_unfolded_tauscan_mu, binmap, binning_gen, "Unfolded data (meas region) TauScan new binning mu");
    TH2D* h_covariance_input_tauscan_mu = (TH2D*) file_mu->Get("Covariance of input (meas region) TauScan");
    TH2D* h_covariance_covmatrix_tauscan_mu = (TH2D*) file_mu->Get("Covariance of CovMatrix (meas region) TauScan");
    TH2D* CovStat_mu = (TH2D*) h_covariance_input_tauscan_mu->Clone("covstat mu clone");
    TH2* CovStat_newbinning_mu = ConvertToNewBinning_2D(CovStat_mu, binmap, binning_gen,"");

    CovStat_mu->Add(h_covariance_covmatrix_tauscan_mu);
    CovStat_newbinning_mu->Add(ConvertToNewBinning_2D(h_covariance_covmatrix_tauscan_mu, binmap, binning_gen,""));
    if(isdata){
      vector<TH2*> CovBgrStat_mu;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_mu->Get("Covariance of " + background_names.at(i) + " TauScan");
        CovBgrStat_mu.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrStat_mu){
        CovStat_mu->Add(bgrcov);
        CovStat_newbinning_mu->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,""));
      }
    }
    TH2* total_cov_mu = (TH2*) CovStat_mu->Clone("total cov mu clone");
    TH2* total_cov_newbinning_mu = (TH2*) CovStat_newbinning_mu->Clone("total cov new mu clone");
    vector<vector<TH1*>> sys_del_mu;
    if(isdata){
      vector<TH2*> CovBgrScale_mu;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_mu->Get("Scale of " + background_names.at(i) + " TauScan");
        CovBgrScale_mu.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrScale_mu){
        total_cov_mu->Add(bgrcov);
        total_cov_newbinning_mu->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,""));
      }
      for(unsigned int i = 0; i < sys_name_mu.size(); i++){
        vector<TH1*> dummy;
        for(unsigned int j = 0; j < sys_name_mu[i].size(); j++){
          TH1* temp_hist = (TH1*) file_mu->Get("Delta of "+sys_name_mu[i][j]+" TauScan");
          // cout << "sysName: " << sys_name_mu[i][j] << '\n';
          // cout << "nbins: " << temp_hist->GetNbinsX() << '\n';
          dummy.push_back(temp_hist);
        }
        sys_del_mu.push_back(dummy);
      }
      vector<vector<TH2*>> sys_cov_mu;
      for(unsigned int i = 0; i < sys_name_mu.size(); i++){
        vector<TH2*> dummy;
        for(unsigned int j = 0; j < sys_name_mu[i].size(); j++){
          TH2* temp_hist = (TH2*) file->Get("Covariance of "+sys_name_mu[i][j]+"TauScan");
          dummy.push_back(temp_hist);
        }
        sys_cov_mu.push_back(dummy);
      }
      for(unsigned int i=0; i<sys_del_mu.size(); i++){
        int j = FindLargestVariation(sys_del_mu.at(i));
        total_cov_mu->Add(sys_cov_mu[i][j]);
        total_cov_newbinning_mu->Add(ConvertToNewBinning_2D(sys_cov_mu[i][j], binmap, binning_gen,""));
      }
    }



    TH1D* h_unfolded_tauscan_ele = (TH1D*) file_ele->Get("Unfolded data (meas region) TauScan");
    TH1* h_unfolded_tauscan_newbinning_ele = ConvertToNewBinning_1D(h_unfolded_tauscan_ele, binmap, binning_gen, "Unfolded data (meas region) TauScan new binning ele");

    TH2D* h_covariance_input_tauscan_ele = (TH2D*) file_ele->Get("Covariance of input (meas region) TauScan");
    TH2D* h_covariance_covmatrix_tauscan_ele = (TH2D*) file_ele->Get("Covariance of CovMatrix (meas region) TauScan");
    TH2D* CovStat_ele = (TH2D*) h_covariance_input_tauscan_ele->Clone("covstat ele clone");
    TH2* CovStat_newbinning_ele = ConvertToNewBinning_2D(CovStat_ele, binmap, binning_gen,"");

    CovStat_ele->Add(h_covariance_covmatrix_tauscan_ele);
    CovStat_newbinning_ele->Add(ConvertToNewBinning_2D(h_covariance_covmatrix_tauscan_ele, binmap, binning_gen,""));
    if(isdata){
      vector<TH2*> CovBgrStat_ele;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_ele->Get("Covariance of " + background_names.at(i) + " TauScan");
        CovBgrStat_ele.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrStat_ele){
        CovStat_ele->Add(bgrcov);
        CovStat_newbinning_ele->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,""));
      }
    }
    TH2* total_cov_ele = (TH2*) CovStat_ele->Clone("totalcov ele clone");
    TH2* total_cov_newbinning_ele = (TH2*) CovStat_newbinning_ele->Clone("total cov new ele clone");
    vector<vector<TH1*>> sys_del_ele;
    if(isdata){
      vector<TH2*> CovBgrScale_ele;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_ele->Get("Scale of " + background_names.at(i) + " TauScan");
        CovBgrScale_ele.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrScale_ele){
        total_cov_ele->Add(bgrcov);
        total_cov_newbinning_ele->Add(ConvertToNewBinning_2D(bgrcov, binmap, binning_gen,""));
      }
      for(unsigned int i = 0; i < sys_name_ele.size(); i++){
        vector<TH1*> dummy;
        for(unsigned int j = 0; j < sys_name_ele[i].size(); j++){
          TH1* temp_hist = (TH1*) file_ele->Get("Delta of "+sys_name_ele[i][j]+" TauScan");
          dummy.push_back(temp_hist);
        }
        sys_del_ele.push_back(dummy);
      }
      vector<vector<TH2*>> sys_cov_ele;
      for(unsigned int i = 0; i < sys_name_ele.size(); i++){
        vector<TH2*> dummy;
        for(unsigned int j = 0; j < sys_name_ele[i].size(); j++){
          TH2* temp_hist = (TH2*) file->Get("Covariance of "+sys_name_ele[i][j]+"TauScan");
          dummy.push_back(temp_hist);
        }
        sys_cov_ele.push_back(dummy);
      }
      for(unsigned int i=0; i<sys_del_ele.size(); i++){
        int j = FindLargestVariation(sys_del_ele.at(i));
        total_cov_ele->Add(sys_cov_ele[i][j]);
        total_cov_newbinning_ele->Add(ConvertToNewBinning_2D(sys_cov_ele[i][j], binmap, binning_gen,""));
      }
    }



    TH1* data_unfolded_sys_mu = SetSysError(h_unfolded_tauscan_mu, total_cov_mu);
    data_unfolded_sys_mu->SetName("Unfolded Data with sys errors_mu");
    TH1* data_unfolded_sys_newbinning_mu = ConvertToNewBinning_1D(SetSysError(h_unfolded_tauscan_mu, total_cov_mu), binmap, binning_gen,"");
    data_unfolded_sys_newbinning_mu->SetName("Unfolded Data with sys errors (new binning)_mu");

    TH1* data_unfolded_sys_ele = SetSysError(h_unfolded_tauscan_ele, total_cov_ele);
    data_unfolded_sys_ele->SetName("Unfolded Data with sys errors_ele");
    TH1* data_unfolded_sys_newbinning_ele = ConvertToNewBinning_1D(SetSysError(h_unfolded_tauscan_ele, total_cov_ele), binmap, binning_gen,"");
    data_unfolded_sys_newbinning_ele->SetName("Unfolded Data with sys errors (new binning)_ele");

    plot->Plot_compatibility(h_unfolded_tauscan_mu, data_unfolded_sys_mu, h_unfolded_tauscan_ele, data_unfolded_sys_ele, directory);
    plot->Plot_compatibility(h_unfolded_tauscan_newbinning_mu, data_unfolded_sys_newbinning_mu, h_unfolded_tauscan_newbinning_ele, data_unfolded_sys_newbinning_ele, directory+"New_binning/");
  }

  return 0;
}



int FindLargestVariation(vector<TH1*> variations){
  if(variations.size() < 1) cout << "Vector of variations has size 0" << endl;
  int nbins = variations[0]->GetXaxis()->GetNbins();
  vector<double> entry;
  for(auto i:variations) entry.push_back(0);
  for(unsigned int j=0; j<variations.size(); j++){
    for(int i=1; i<=nbins; i++){
      entry[j] += abs(variations[j]->GetBinContent(i));
    }
  }
  double max_value = 0;
  int position = -1;
  for(unsigned int i=0; i<entry.size(); i++){
    // cout << "Entry: " << entry[i] << '\n';
    if(entry[i] >= max_value){
      max_value = entry[i];
      position = i;
    }
  }
  if(position == -1) cout << "NO MAX VALUE FOUND FOR VARIATION" << endl;
  return position;
}

TH1* ConvertToRelative(TH1* sys, TH1* central){
  int nbins = central->GetXaxis()->GetNbins();
  TString name = sys->GetName();
  name += " Clone";
  TH1* hist = (TH1*)sys->Clone(name);
  hist->Reset();
  for(int bin=1; bin<=nbins; bin++){
    double cont_central = central->GetBinContent(bin);
    double cont_sys = abs(sys->GetBinContent(bin));
    double percent;
    if(cont_central == 0) percent = 0;
    else percent = 100*cont_sys/cont_central;
    hist->SetBinContent(bin, percent);
    hist->SetBinError(bin, 0.000000001); // just for plotting reasons
  }
  return hist;
}

TH1* AddSys(vector<TH1*> sys){
  int nbins = sys[0]->GetXaxis()->GetNbins();
  TH1* hist = (TH1*)sys[0]->Clone("hist");
  hist->Reset();
  for(int bin=1; bin<=nbins; bin++){
    double sum = 0;
    for(unsigned int j=0; j<sys.size(); j++){
      double cont_sys = abs(sys[j]->GetBinContent(bin));
      sum += cont_sys * cont_sys;
    }
    double entry = sqrt(sum);
    hist->SetBinContent(bin, entry);
  }
  return hist;
}

// set uncert from cov to 1d hist
TH1* SetSysError(TH1* data_unfolded, TH2* CovTotal){
  TH1* hist = (TH1*) data_unfolded->Clone("just a clone");
  int nbins = hist->GetXaxis()->GetNbins();
  for(int i=1; i<=nbins; i++){
    double error = sqrt(CovTotal->GetBinContent(i,i));
    hist->SetBinError(i, error);
  }
  return hist;
}

// create delta distribution from COV
TH1* CreateDeltaFromCov(TH2* Cov){
  const int nbins = Cov->GetXaxis()->GetNbins();
  TString name = Cov->GetName();
  name += " Delta";
  vector<double> bins;
  vector<double> values;
  bins.push_back(Cov->GetXaxis()->GetBinLowEdge(1));
  for(int i=1; i<=nbins; i++){
    bins.push_back(Cov->GetXaxis()->GetBinUpEdge(i));
    values.push_back( sqrt(Cov->GetBinContent(i,i)) );
  }
  TH1D* delta = new TH1D(name, " ", nbins, &bins[0]);
  for(int i=1; i<=nbins; i++) delta->SetBinContent(i, values[i-1]);
  return delta;
}

// scale stat error of input distribution to data (sqrt(N))
void ScaleErrorToData(TH1* hist){
  int nbins = hist->GetXaxis()->GetNbins();
  for(int bin=1; bin<=nbins; bin++){
    double N = hist->GetBinContent(bin);
    hist->SetBinError(bin, sqrt(N));
  }
  return;
}

TH1* GetModelDelta(TH1* unfolded, TH1* truth){
  TH1* delta = (TH1*) unfolded->Clone("just a clone 2");
  delta->Reset();
  int nbins = delta->GetXaxis()->GetNbins();
  for(int i=1; i<=nbins; i++){
    double diff = unfolded->GetBinContent(i) - truth->GetBinContent(i);
    double err = unfolded->GetBinError(i) - truth->GetBinError(i);
    delta->SetBinContent(i, diff);
    delta->SetBinError(i, err);
  }
  return delta;
}


TH2* CreateCovFromDelta(TH1* delta, TH2* dummyCov){
  int nbins = delta->GetXaxis()->GetNbins();
  TH2* cov = (TH2*) dummyCov->Clone("just a clone 3");
  cov->Reset();
  for(int i=1; i<=nbins; i++){
    for(int j=1; j<=nbins; j++){
      double entry = delta->GetBinContent(i) * delta->GetBinContent(j);
      cov->SetBinContent(i,j,entry);
    }
  }
  return cov;
}

int FindLargestVariationByMean(vector<TH1D*> truth, vector<TH1*> variations, bool use_significance){
  double max_significance = 0;
  double max_shift = 0;
  double error_truth = 0;
  int index_sig = -1;
  int index_shift = -1;
  if(truth.size() != variations.size()) cout << "Not same number of Truth and Variations" << endl;
  for(unsigned int i=0; i<variations.size(); i++){
    double mean_truth = truth[i]->GetMean();
    error_truth = truth[i]->GetMeanError();
    double mean = variations[i]->GetMean();
    double error = variations[i]->GetMeanError();
    double shift = fabs(mean_truth - mean);
    double significance = shift/(sqrt(error_truth*error_truth + error*error));
    if(significance > max_significance){
      max_significance = significance;
      index_sig = i;
    }
    if(shift > max_shift){
      max_shift = shift;
      index_shift = i;
    }
  }
  cout << "  -- largest significance: "<< max_significance << " (Variation "<< index_sig << ")" << endl;
  cout << "  -- largest shift: "<< max_shift << " (Variation "<< index_shift << ")" << endl;

  int index;
  if(use_significance) index = index_sig;
  else                 index = index_shift;

  if(index == -1)  cout << "NO MAX VALUE FOUND FOR VARIATION" << endl;
  return index;
}


TH1* ConstructAverage(vector<TH1*> variations, int varfrom){
  if(variations.size() == 0) cout << "AVERGAE CAN NOT BE CONSTRUCTED" << endl;
  TH1* average = (TH1*) variations[0]->Clone("your average clone");
  average->Reset();
  int nbins = average->GetXaxis()->GetNbins();
  for(int bin=1; bin<=nbins; bin++){
    int nvars = variations.size();
    double sum = 0.0;
    for(auto var: variations) sum += fabs(var->GetBinContent(bin));
    double ave = sum/nvars;
    if(variations[varfrom]->GetBinContent(bin) < 0) ave *= -1.0;
    average->SetBinContent(bin, ave);
  }
  return average;
}

TH1* ConvertToNewBinning_1D(TH1* oldhist, vector<int> binmap, const TUnfoldBinning *binning, TString name){
  const TUnfoldBinning *meas = binning->FindNode("measurement_gen");
  const TVectorD *bin_edges = meas->GetDistributionBinning(0);

  vector<double> dummy_bins = {0.};
  for(unsigned int i = 0; i < binmap.size()-1; i++){
    if(binmap.at(i) == binmap.at(i+1)) continue;
    else{
        dummy_bins.push_back(i+1);
    }
  }
  dummy_bins.push_back(1.);

  double bins[dummy_bins.size()];
  for(unsigned int i = 0; i < dummy_bins.size(); i++){
    if(i == 0 || i == dummy_bins.size()-1) bins[i] = dummy_bins.at(i);
    else bins[i] = bin_edges[0][dummy_bins.at(i)];
    // cout << "dummy_bins: " << bins[i] << '\n';
  }

  TH1* newhist = meas->CreateHistogram("", kTRUE, 0, 0, "mass[C]");
  int nbins = newhist->GetXaxis()->GetNbins();
  unsigned int nbinsold = oldhist->GetXaxis()->GetNbins();
  if(binmap.size() != nbinsold) cout << "not correct amount of entries in binmap" << endl;
  for(int newbin=1; newbin<=nbins; newbin++){
    double sum=0, err=0, err2=0;
    for(unsigned int i=0; i<binmap.size(); i++){
      int oldbin = i+1;
      if(newbin == binmap[i]){
        sum += oldhist->GetBinContent(oldbin);
        err2 += pow(oldhist->GetBinError(oldbin),2);
      }
    }
    err = sqrt(err2);
    newhist->SetBinContent(newbin, sum);
    newhist->SetBinError(newbin, err);
  }
  TH1* newnewhist = new TH1D(name+" new binning", "Interesting Title", dummy_bins.size()-1, bins);
  for(int i = 1; i <= newhist->GetNbinsX(); i++){
    newnewhist->SetBinContent(i, newhist->GetBinContent(i));
    newnewhist->SetBinError(i, newhist->GetBinError(i));
  }

  return newnewhist;
}

TH2* ConvertToNewBinning_2D(TH2* oldcov, vector<int> binmap, const TUnfoldBinning *binning, TString name){
  const TUnfoldBinning *meas = binning->FindNode("measurement_gen");
  const TVectorD *bin_edges = meas->GetDistributionBinning(0);

  vector<double> dummy_bins = {0.};
  for(unsigned int i = 0; i < binmap.size()-1; i++){
    if(binmap.at(i) == binmap.at(i+1)) continue;
    else{
      dummy_bins.push_back(i+1);
    }
  }
  dummy_bins.push_back(1.);

  double bins[dummy_bins.size()];
  for(unsigned int i = 0; i < dummy_bins.size(); i++){
    if(i == 0 || i == dummy_bins.size()-1) bins[i] = dummy_bins.at(i);
    else bins[i] = bin_edges[0][dummy_bins.at(i)];
  }

  TH2* newcov = meas->CreateErrorMatrixHistogram(name, kTRUE, 0, 0, "mass[C]");
  int nbins = newcov->GetXaxis()->GetNbins();
  unsigned int nbinsold = oldcov->GetXaxis()->GetNbins();
  if(binmap.size() != nbinsold) cout << "not correct amount of entries in binmap" << endl;
  for(int xbin=1; xbin<=nbins; xbin++){
    for(int ybin=1; ybin<=nbins; ybin++){
      double new_entry = 0;
      for(unsigned int i=0; i<binmap.size(); i++){
        if(xbin == binmap[i]){
          int oldxbin = i+1;
          for(unsigned int j=0; j<binmap.size(); j++){
            if(ybin == binmap[j]){
              int oldybin = j+1;
              new_entry += oldcov->GetBinContent(oldxbin,   oldybin);
            }
          }
        }
      }
      newcov->SetBinContent(xbin, ybin, new_entry);
    }
  }
  TH2* newnewcov = new TH2D(name+" new binning", "Interesting Title", dummy_bins.size()-1, bins, dummy_bins.size()-1, bins);
  for(int i = 1; i <= newcov->GetNbinsX(); i++){
    for(int j = 1; j <= newcov->GetNbinsY(); j++){
      newnewcov->SetBinContent(i, j, newcov->GetBinContent(i, j));
    }
  }
  return newnewcov;
}

TH1* ConvertToCrossSection(TH1* hist){
  TH1* newhist = (TH1*) hist->Clone();
  int nbins = hist->GetXaxis()->GetNbins();
  for(int bin=1; bin<=nbins; bin++){
    double events = hist->GetBinContent(bin);
    double error = hist->GetBinError(bin);
    double binwidth = hist->GetBinWidth(bin);
    double cs = events/(binwidth*35.9);
    double er = error/(binwidth*35.9);
    newhist->SetBinContent(bin, cs);
    newhist->SetBinError(bin, er);
  }
  return newhist;
}
