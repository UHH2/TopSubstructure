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
#include "normalise.h"

using namespace std;

int FindLargestVariation(vector<TH1D*> variations);
TH1D* ConvertToRelative(TH1D* sys, TH1D* central);
TH1D* SetSysError(TH1D* data_unfolded, TH2D* CovTotal);
TH1D* CreateDeltaFromCov(TH2D* Cov);
TH1D* AddSys(vector<TH1D*> sys);
TH1D* GetModelDelta(TH1D* unfolded, TH1D* truth);
TH2D* CreateCovFromDelta(TH1D* delta, TH2D* dummyCov);
int FindLargestVariationByMean(vector<TH1D*> truth, vector<TH1D*> variations, bool use_significance);
TH1D* ConstructAverage(vector<TH1D*> variations, int varfrom);
TH1D* ConvertToNewBinning_1D(TH1D*, vector<int>, const TUnfoldBinning*, TString);
TH2D* ConvertToNewBinning_2D(TH2D*, vector<int>, const TUnfoldBinning*, TString);
TH1D* ConvertToCrossSection(TH1D* hist);
TH2D* ConvertToCrossSection(TH2D* cov);
TH2D* CreateCorrelationMatrix(TH2D* cov);
vector<double> TotalCrossSection(TH1D* hist, TH2D* cov);

//  TODO: definitely needs an overhaul, because it gets rather complicated


int main(int argc, char* argv[]){

  // Setup which dataset will be plotted, where it will be saved, which channel is being used and the resulting systematic and model uncertainties
  if(argc < 2) throw runtime_error("How to use Plotter: ./Plotter <Filename>");
  TString directory = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
  TString File_name = argv[1];

  bool isdata = false;
  if(File_name.Contains("Data")) isdata = true;
  cout << "isData: " << isdata << '\n';

  TFile *file = new TFile(File_name, "READ");
  TString jetcol = "";
  if(File_name.Contains("puppi_sd")) jetcol = "_puppi_sd";
  else if(File_name.Contains("puppi")) jetcol = "_puppi";
  else if(File_name.Contains("sd")) jetcol = "_sd";
  cout << "jetcol: " << jetcol << '\n';

  string input(argv[1]);
  vector<string> result;
  boost::split(result, input, boost::is_any_of("_ . /"));
  TString dataset = "";
  int count = 0;
  TString data= "";
  TString sample_name = "";
  for(unsigned int i = 0; i < result.size(); i++){
    cout << "result: " << result[i] << '\n';
    if(result[i] != "mu" && result[i] != "ele" && result[i] != "comb" && i != 1){
      if(i > 0 && i < result.size()-1){
        directory += result[i];
        if(i==2){
          data = result[i];
          sample_name = result[i];
        }
        if(i==3){
          if(data.Contains("Pseudo")){
            data += "_"+result[i];
          }
        }
        if(result[i] != "sd" && result[i] != "puppi"){
          dataset += result[i];
        }
      }
      if(i < result.size() - 3 && i > 0){
        directory += "_";
        if(count == 0) dataset += "_";
      }
    }
  }
  dataset = data;
  directory += "/";
  TString jetcol2 = jetcol;
  TString channel = "";
  if(File_name.Contains("mu")){
    directory += "mu/";
    channel += "_mu";
  }
  else if(File_name.Contains("ele")){
    directory += "ele/";
    channel += "_ele";
  }
  else if(File_name.Contains("comb")){
    directory += "comb/";
    channel += "_comb";
  }
  jetcol += channel;
  cout << "directory: " << directory << '\n';
  cout << "channel: " << channel << '\n';
  cout << "jetcol: " << jetcol << '\n';
  cout << "jetcol2: " << jetcol2 << '\n';
  cout << "File_name: " << File_name << '\n';

  vector<TString> background_names = {"DYJets", "ST", "Diboson", "WJets_HT", "QCD"};
  vector<vector<TString>> sys_name;
  if(File_name.Contains("mu")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
  else if(File_name.Contains("ele")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
  else if(File_name.Contains("comb")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};

  vector<vector<TString>> model_name;
  if(isdata) model_name = {{"SCALEupup", "SCALEupnone", "SCALEnoneup", "SCALEnonedown", "SCALEdownnone", "SCALEdowndown"}, {"mtop1715", "mtop1735"}, {"isrup", "isrdown"}, {"fsrup", "fsrdown"}, {"hdampup", "hdampdown"}};

  vector<Int_t> binmap = {1,2,3,4,5};


  // Import all histogramms
  TUnfoldBinning* binning_rec = (TUnfoldBinning*) file->Get("binning_rec");
  TUnfoldBinning* binning_gen = (TUnfoldBinning*) file->Get("binning_gen");
  Plotter *plot = new Plotter(binning_gen, binning_rec);

  TH1D* projection_gen = (TH1D*) file->Get("Projection of gen distribution TauScan");
  TH1D* projection_rec = (TH1D*) file->Get("Projection of rec distribution TauScan");
  TH1D* h_input_gen = (TH1D*) file->Get("MC gen Distribution");
  TH1D* h_input_rec = (TH1D*) file->Get("MC rec input");
  plot->Plot_projections(projection_gen, h_input_gen, projection_rec, h_input_rec, directory);


  TH1D* h_unfolded_tauscan = (TH1D*) file->Get("Unfolded data (meas region) TauScan");
  TH1D* h_unfolded_tauscan_part = (TH1D*) file->Get("Unfolded data (meas region, m>155) TauScan");
  TH1D* h_data_truth, *h_data_truth_2;
  if(isdata){
    TFile* blub;
    blub = new TFile("Unfoldings/Unfolding_madgraph"+jetcol2+channel+".root", "READ");
    h_data_truth = (TH1D*) blub->Get("Data Truth");
    h_data_truth_2 = (TH1D*) file->Get("MC Truth 2017");
  }
  else h_data_truth = (TH1D*) file->Get("Data Truth");
  // if(File_name.Contains("data")) h_data_truth->SetName("POWHEG truth");
  // else h_data_truth->SetName(data+" truth");
  TH1D* h_mc_truth;
  if(isdata) h_mc_truth = (TH1D*) file->Get("MC Truth");
  else h_mc_truth = (TH1D*) file->Get("Bias Truth TauScan");
  h_mc_truth->SetName("POWHEG");
  // TH1D* check=(TH1D*) file->Get("Unfolded check (meas region) TauScan");
  TH1D* h_mc_truth_low = (TH1D*) file->Get("MC Truth low");
  TH1D* h_mc_truth_high = (TH1D*) file->Get("MC Truth high");


  TH1D* h_data_truth_all = (TH1D*) file->Get("Whole Data Truth");

  TH2D* h_covariance_input_tauscan = (TH2D*) file->Get("Covariance of input (meas region) TauScan");
  plot->Plot_covariance(h_covariance_input_tauscan, directory+"Test_");
  TH2D* h_covariance_covmatrix_tauscan = (TH2D*) file->Get("Covariance of CovMatrix (meas region) TauScan");
  // create the unfolding histogram with only statistical error
  TH1D* data_unfolded_stat = SetSysError(h_unfolded_tauscan, h_covariance_input_tauscan);

  TH2D* h_response = (TH2D*) file->Get("response matrix");
  plot->Plot_ResponseMatrix(h_response, directory+"Migration_Matrix");

  TH2D* h_probability = (TH2D*) file->Get("Probability Matrix TauScan");
  plot->Plot_ResponseMatrix(h_probability, directory+"Probability_Matrix");


  // add up all uncertainties step by step

  // add model uncertainties
  vector<vector<TH1D*>> model_output, model_delta, model_truth;
  if(isdata){
    for(unsigned int i=0; i<model_name.size(); i++){
      vector<TH1D*> dummy;
      model_output.push_back(dummy);
      model_delta.push_back(dummy);
      model_truth.push_back(dummy);
      vector<TH2D*> dummy2;

      for(unsigned int j=0; j<model_name[i].size(); j++){
        TFile *file_model = new TFile("Unfoldings/Unfolding_"+model_name[i][j]+jetcol+".root", "READ");
        TH1D* output = (TH1D*) file_model->Get("Unfolded data (meas region) TauScan");
        TH1D* truth = (TH1D*) file_model->Get("Data Truth");
        model_output[i].push_back(output);
        model_truth[i].push_back(truth);
        TH1D* delta = GetModelDelta(output, model_truth[i][j]);
        model_delta[i].push_back(delta);
      }
    }
  }

  // add up stat Covariance of input
  cout << "sum up stat cov matrices" << endl;
  vector<TH1D*> error, error_model;
  vector<TString> error_name, error_name_model;
  TH2D* CovStat = (TH2D*) h_covariance_input_tauscan->Clone("CovStat");
  TH2D* CovStat_model = (TH2D*) h_covariance_input_tauscan->Clone("covstat model clone");
  TH1D* stat_delta = CreateDeltaFromCov(CovStat);
  TH1D* stat_rel = ConvertToRelative(stat_delta, h_unfolded_tauscan);
  error.push_back(stat_rel);
  error_model.push_back(stat_rel);
  error_name.push_back("stat");
  error_name_model.push_back("stat");

  TH2D* CorrStat = CreateCorrelationMatrix(h_covariance_input_tauscan);
  plot->Plot_covariance(CorrStat, directory+"CorrStat");

  TH2D* h_covariance_input_tauscan_lcurve = (TH2D*) file->Get("Covariance of input (meas region) LCurve");
  plot->Plot_covariance(h_covariance_input_tauscan_lcurve, directory+"Test_lcurve");

  TH2D* CorrStat_lcurve = CreateCorrelationMatrix(h_covariance_input_tauscan_lcurve);
  plot->Plot_covariance(CorrStat_lcurve, directory+"CorrStat_lcurve");

  // add stat covariance of input matrix
  TH2D* mc_error = (TH2D*) h_covariance_covmatrix_tauscan->Clone("mc error clone");
  CovStat->Add(h_covariance_covmatrix_tauscan);
  CovStat_model->Add(h_covariance_covmatrix_tauscan);

  // add stat covariance of background -- if it is data
  if(isdata){
    vector<TH2D*> CovBgrStat;
    for(unsigned int i = 0; i < background_names.size(); i++){
      TH2D* temp_hist = (TH2D*) file->Get("Covariance of " + background_names.at(i) + " TauScan");
      CovBgrStat.push_back(temp_hist);
    }
    for(auto bgrcov: CovBgrStat){
      CovStat->Add(bgrcov);
      CovStat_model->Add(bgrcov);
      mc_error->Add(bgrcov);
    }
  }
  TH1D* MC_stat_delta = CreateDeltaFromCov(mc_error);
  TH1D* MC_stat_rel = ConvertToRelative(MC_stat_delta, h_unfolded_tauscan);

  TH2D* CovSys = (TH2D*) mc_error->Clone("CovSys");
  error.push_back(MC_stat_rel);
  error_model.push_back(MC_stat_rel);
  error_name.push_back("MC stat");
  error_name_model.push_back("MC stat");

  // then add sys cov from backgrounds
  cout << "sum up background sys cov matrices" << endl;
  TH2D* total_cov = (TH2D*) CovStat->Clone("CovStat Clone");
  vector<vector<TH1D*>> sys_del;
  TH2D* cov_model = (TH2D*) CovStat->Clone("cov_model");
  cov_model->Reset();
  if(isdata){
    vector<TH2D*> CovBgrScale;
    for(unsigned int i = 0; i < background_names.size(); i++){
      TH2D* temp_hist = (TH2D*) file->Get("Scale of " + background_names.at(i) + " TauScan");
      CovBgrScale.push_back(temp_hist);
    }

    TH2D* dummy_bgr_scale;
    dummy_bgr_scale = (TH2D*) CovBgrScale[0]->Clone("dummy clone");
    dummy_bgr_scale->Reset();
    for(auto bgrcov: CovBgrScale) dummy_bgr_scale->Add(bgrcov);
    TH1D* sys_bkg_delta;
    TH1D* sys_bkg_rel;
    sys_bkg_delta = CreateDeltaFromCov(dummy_bgr_scale);
    sys_bkg_rel = ConvertToRelative(sys_bkg_delta, h_unfolded_tauscan);
    error.push_back(sys_bkg_rel);
    error_name.push_back("background sys");
    for(auto bgrcov: CovBgrScale){
      total_cov->Add(bgrcov);
      CovSys->Add(bgrcov);
    }

    // then add sys cov (and convert used uncertainty to relative hist)
    cout << "sum up experimental sys cov matrices" << endl;

    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH1D*> dummy;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        TH1D* temp_hist = (TH1D*) file->Get("Delta of "+sys_name[i][j]+" TauScan");
        dummy.push_back(temp_hist);
      }
      sys_del.push_back(dummy);
    }


    vector<vector<TH2D*>> sys_cov;
    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH2D*> dummy;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        TH2D* temp_hist = (TH2D*) file->Get("Covariance of "+sys_name[i][j]+"TauScan");
        dummy.push_back(temp_hist);
      }
      sys_cov.push_back(dummy);
    }

    TH2D* cov_sys = (TH2D*) CovStat->Clone("covsys clone");
    cov_sys->Reset();
    vector<TH1D*> sys_rel;
    for(unsigned int i=0; i<sys_del.size(); i++){
      int j = FindLargestVariation(sys_del.at(i));
      total_cov->Add(sys_cov[i][j]);
      cov_sys->Add(sys_cov[i][j]);
      sys_rel.push_back(ConvertToRelative(sys_del[i][j], h_unfolded_tauscan));
      error.push_back(sys_rel[i]);
      error_name.push_back(sys_name[i][j]);
      CovSys->Add(sys_cov[i][j]);
    }

    // model sys: use average of both variations
    cout << "sum up model sys cov matrices" << endl;
    vector<TH1D*> model_delta_average;
    vector<TH1D*> model_rel;
    for(unsigned int i=0; i<model_delta.size(); i++){
      int j = FindLargestVariationByMean(model_truth[i], model_output[i], false);
      TH1D* delta_average = ConstructAverage(model_delta[i], j);
      model_delta_average.push_back(delta_average);
      TH2D* cov_average = CreateCovFromDelta(delta_average, h_covariance_input_tauscan);
      TH1D* chosen_delta;
      TH2D* chosen_cov;
      chosen_cov = cov_average;
      chosen_delta = delta_average;
      model_rel.push_back(ConvertToRelative(chosen_delta, h_unfolded_tauscan));
      error_model.push_back(model_rel[i]);
      error_name_model.push_back(model_name[i][j]);
      CovStat_model->Add(chosen_cov);
      cov_model->Add(chosen_cov);
    }
    model_rel.push_back(stat_rel);            // put in stat to get total
  }

  TH2D* CovTheo = (TH2D*) h_covariance_input_tauscan->Clone();
  CovTheo->Reset();
  TH2D* CovTheo_mad = (TH2D*) h_covariance_input_tauscan->Clone();
  CovTheo_mad->Reset();
  TH2D* CovTheo_17 = (TH2D*) h_covariance_input_tauscan->Clone();
  CovTheo_17->Reset();
  vector<vector<TH1D*>> THEO_DELTA;
  // get Delta between nominal truth and scale variation truth (for theo. uncert)
  for(unsigned int i=0; i<model_truth.size(); i++){
    vector<TH1D*> delta_temp, delta_temp_mad, delta_temp_17;
    vector<TH1D*> truth_temp, truth_temp_mad, truth_temp_17;
    vector<TH1D*> variation_temp;
    for(unsigned int j=0; j<model_truth[i].size(); j++){
      delta_temp.push_back(GetModelDelta(model_truth[i][j], h_mc_truth));
      delta_temp_mad.push_back(GetModelDelta(model_truth[i][j], h_data_truth));
      delta_temp_17.push_back(GetModelDelta(model_truth[i][j], h_data_truth_2));
      variation_temp.push_back((TH1D*)model_truth[i][j]);
      truth_temp.push_back((TH1D*) h_mc_truth); // always use central truth
      truth_temp_mad.push_back((TH1D*) h_data_truth); // always use central truth
      truth_temp_17.push_back((TH1D*) h_data_truth_2); // always use central truth
    }
    THEO_DELTA.push_back(delta_temp);
    int index = FindLargestVariationByMean(truth_temp, variation_temp, false);
    TH2* COV_temp = CreateCovFromDelta(delta_temp[index], h_covariance_input_tauscan);
    int index_mad = FindLargestVariationByMean(truth_temp_mad, variation_temp, false);
    TH2* COV_temp_mad = CreateCovFromDelta(delta_temp_mad[index_mad], h_covariance_input_tauscan);
    int index_17 = FindLargestVariationByMean(truth_temp_17, variation_temp, false);
    TH2* COV_temp_17 = CreateCovFromDelta(delta_temp_17[index_17], h_covariance_input_tauscan);
    CovTheo->Add(COV_temp);
    CovTheo_mad->Add(COV_temp_mad);
    CovTheo_17->Add(COV_temp_17);
  }

  TH2D* CovTotal = (TH2D*) total_cov->Clone("Total");
  CovTotal->Add(cov_model);
  TH1D* data_unfolded_sys = SetSysError(h_unfolded_tauscan, CovTotal);
  data_unfolded_sys->SetName("Unfolded Data with sys errors");
  // TH1D* check_sys = SetSysError(check, CovTotal);
  // check_sys->SetName("Unfolded Data with sys errors check");
  plot->Plot_covariance(CovTotal, directory+"Total_Covariance");
  TH1D* sys_tot_delta = CreateDeltaFromCov(total_cov);

  TH1D* sys_tot_rel = ConvertToRelative(sys_tot_delta, h_unfolded_tauscan);


  TH2D *CorrTotal = CreateCorrelationMatrix(CovTotal);
  plot->Plot_covariance(CorrTotal, directory+"CorrTotal");
  error.push_back(sys_tot_rel);
  error_name.push_back("stat #oplus exp sys");



  TH1D* model_tot_delta = CreateDeltaFromCov(CovStat_model);
  TH1D* model_tot_rel = ConvertToRelative(model_tot_delta, h_unfolded_tauscan);
  error_model.push_back(model_tot_rel);
  error_name_model.push_back("stat #oplus model sys");

  plot->Plot_delta(model_delta, model_name, directory);

  if(isdata){
    sys_del[sys_del.size()-1].push_back(MC_stat_delta);
    sys_name[sys_del.size()-1].push_back("Background");
  }
  plot->Plot_delta(sys_del, sys_name, directory);
  plot->Plot_uncertainty(error, error_name, directory+"EXP_");

  if(isdata) plot->Plot_result_with_uncertainty(data_unfolded_stat, data_unfolded_sys, h_data_truth, h_data_truth_2, h_mc_truth, false, sample_name, directory+"Event_");
  else plot->Plot_result_with_uncertainty(data_unfolded_stat, data_unfolded_sys, h_data_truth, h_mc_truth, false, sample_name, directory+"Event_");

  TH1D* data_unfolded_stat_cs = ConvertToCrossSection(data_unfolded_stat);
  TH2D* CovStat_cs = ConvertToCrossSection(h_covariance_input_tauscan);
  TH2D* CovModel_cs = ConvertToCrossSection(cov_model);
  TH2D* CovSys_cs = ConvertToCrossSection(CovSys);
  TH2D* CovTotal_cs = ConvertToCrossSection(CovTotal);
  TH1D* data_unfolded_sys_cs = ConvertToCrossSection(data_unfolded_sys);
  TH1D* h_data_truth_cs = ConvertToCrossSection(h_data_truth);
  TH1D* h_mc_truth_low_cs = ConvertToCrossSection(h_mc_truth_low);
  TH1D* h_mc_truth_high_cs = ConvertToCrossSection(h_mc_truth_high);
  TH1D* h_data_truth_2_cs;
  if(isdata) h_data_truth_2_cs = ConvertToCrossSection(h_data_truth_2);
  TH1D* h_mc_truth_cs = ConvertToCrossSection(h_mc_truth);
  // TH1D* check_cs = ConvertToCrossSection(check);
  // TH1D* check_sys_cs = ConvertToCrossSection(check_sys);

  plot->Plot_uncertainty(error_model, error_name_model, directory+"Model_");
  if(isdata) plot->Plot_result_with_uncertainty(data_unfolded_stat_cs, data_unfolded_sys_cs, h_data_truth_cs, h_data_truth_2_cs, h_mc_truth_cs, false, sample_name, directory+"CS_");
  else{
    plot->Plot_result_with_uncertainty(data_unfolded_stat_cs, data_unfolded_sys_cs, h_data_truth_cs, h_mc_truth_cs, false, sample_name, directory+"CS_");
    // plot->Plot_result_with_uncertainty(check_cs, check_sys_cs, h_mc_truth_cs, h_mc_truth_cs, false, sample_name, directory+"CS_Check_");
  }
  plot->Plot_result_with_uncertainty(data_unfolded_stat_cs, data_unfolded_sys_cs, h_mc_truth_low_cs, h_mc_truth_cs, false, sample_name, directory+"CS_POWHEG_");


  vector<double> xsdataStat = TotalCrossSection(data_unfolded_sys_cs, CovStat_cs);
  vector<double> xsdataModel = TotalCrossSection(data_unfolded_sys_cs, CovModel_cs);
  vector<double> xsdataSys = TotalCrossSection(data_unfolded_sys_cs, CovSys_cs);
  vector<double> xsdataTotal = TotalCrossSection(data_unfolded_sys_cs, CovTotal_cs);


  cout << '\n';
  cout << "DATA XS = " << xsdataStat[0] << " +- " << xsdataStat[1] << " (stat)";
  cout <<                                  " +- " << xsdataSys[1] << " (sys)";
  cout <<                                  " +- " << xsdataModel[1] << " (model)";
  cout <<                                  " +- " << xsdataTotal[1] << " (tot)" << endl;
  cout << '\n';



  vector<vector<TString>> model_name2;
  if(isdata) model_name2 = {{"SCALEupup", "SCALEupnone", "SCALEnoneup", "SCALEnonedown", "SCALEdownnone", "SCALEdowndown"}, {"isrup", "isrdown"}, {"fsrup", "fsrdown"}, {"hdampup", "hdampdown"}};

  // TH1D* h_mc_truth_error = (TH1D*) SetSysError(h_mc_truth, CovTheo);
  TH2D* CovTheo_cs = ConvertToCrossSection(CovTheo);
  vector<double> xsmc1 = TotalCrossSection(h_mc_truth_cs, CovTheo_cs);
  cout << "MC POWHEG XS = " << xsmc1[0] << " +- " << xsmc1[1] << endl;

  TH2D* CovTheo_mad_cs = ConvertToCrossSection(CovTheo_mad);
  vector<double> xsmcmad1 = TotalCrossSection(h_data_truth_cs, CovTheo_mad_cs);
  cout << "MC MADGRAP XS = " << xsmcmad1[0] << " +- " << xsmcmad1[1] << endl;

  TH2D* CovTheo_17_cs = ConvertToCrossSection(CovTheo_17);
  vector<double> xsmc171 = TotalCrossSection(h_data_truth_2_cs, CovTheo_17_cs);
  cout << "MC POWHEG 17 XS = " << xsmc171[0] << " +- " << xsmc171[1] << endl;






  Normalise *norm_cs_stat = new Normalise(h_unfolded_tauscan, h_covariance_input_tauscan, 0, 1, false);
  TH1D* data_cs_stat_norm = norm_cs_stat->GetHist();
  TH2D* CovMatrix_cs_stat_norm = norm_cs_stat->GetMatrix();
  TH1D* normed_stat = SetSysError(data_cs_stat_norm, CovMatrix_cs_stat_norm);

  Normalise *norm_cs_tot = new Normalise(h_unfolded_tauscan, CovTotal, 0, 1, false);
  TH1D* data_cs_tot_norm = norm_cs_tot->GetHist();
  TH2D* CovMatrix_cs_tot_norm = norm_cs_tot->GetMatrix();
  TH1D* normed_tot = SetSysError(data_cs_tot_norm, CovMatrix_cs_tot_norm);

  Normalise *normdata_truth = new Normalise(h_data_truth, 0, 1, false);
  TH1D* data_truth_norm = normdata_truth->GetHist();

  Normalise *normdata_truth_2;
  if(isdata) normdata_truth_2 = new Normalise(h_data_truth_2, 0, 1, false);
  TH1D* data_truth_norm_2;
  if(isdata) data_truth_norm_2 = normdata_truth_2->GetHist();

  Normalise *normmc_truth = new Normalise(h_mc_truth, 0, 1, false);
  TH1D* mc_truth_norm = normmc_truth->GetHist();
  if(isdata) plot->Plot_result_with_uncertainty(normed_stat, normed_tot, data_truth_norm, data_truth_norm_2, mc_truth_norm, true, sample_name, directory+"CS_normed_");
  else plot->Plot_result_with_uncertainty(normed_stat, normed_tot, data_truth_norm, mc_truth_norm, true, sample_name, directory+"CS_normed_");

  Normalise *norm_cs_stat_bin = new Normalise(h_unfolded_tauscan, h_covariance_input_tauscan, 0, 1, true);
  TH1D* data_cs_stat_norm_bin = norm_cs_stat_bin->GetHist();
  TH2D* CovMatrix_cs_stat_norm_bin = norm_cs_stat_bin->GetMatrix();
  TH1D* normed_stat_bin = SetSysError(data_cs_stat_norm_bin, CovMatrix_cs_stat_norm_bin);

  Normalise *norm_cs_tot_bin = new Normalise(h_unfolded_tauscan, CovTotal, 0, 1, true);
  TH1D* data_cs_tot_norm_bin = norm_cs_tot_bin->GetHist();
  TH2D* CovMatrix_cs_tot_norm_bin = norm_cs_tot_bin->GetMatrix();
  TH1D* normed_tot_bin = SetSysError(data_cs_tot_norm_bin, CovMatrix_cs_tot_norm_bin);

  Normalise *normdata_truth_bin = new Normalise(h_data_truth, 0, 1, true);
  TH1D* data_truth_norm_bin = normdata_truth_bin->GetHist();

  Normalise *normdata_truth_bin_2;
  if(isdata) normdata_truth_bin_2 = new Normalise(h_data_truth_2, 0, 1, true);
  TH1D* data_truth_norm_bin_2;
  if(isdata) data_truth_norm_bin_2 = normdata_truth_bin_2->GetHist();

  Normalise *normmc_truth_bin = new Normalise(h_mc_truth, 0, 1, true);
  TH1D* mc_truth_norm_bin = normmc_truth_bin->GetHist();
  if(isdata) plot->Plot_result_with_uncertainty(normed_stat_bin, normed_tot_bin, data_truth_norm_bin, data_truth_norm_bin_2, mc_truth_norm_bin, true, sample_name, directory+"CS_normed_bin_");
  else plot->Plot_result_with_uncertainty(normed_stat_bin, normed_tot_bin, data_truth_norm_bin, mc_truth_norm_bin, true, sample_name, directory+"CS_normed_bin_");


  if(isdata){
    TFile* blub2, *blub3;
    blub2 = new TFile("Unfoldings/Unfolding_fsrup"+jetcol2+channel+".root", "READ");
    TH1D* h_data_truth_3 = (TH1D*) blub2->Get("Data Truth");
    blub3 = new TFile("Unfoldings/Unfolding_fsrdown"+jetcol2+channel+".root", "READ");
    TH1D* h_data_truth_4 = (TH1D*) blub3->Get("Data Truth");

    TH1D* h_data_truth_3_cs = ConvertToCrossSection(h_data_truth_3);
    TH1D* h_data_truth_4_cs = ConvertToCrossSection(h_data_truth_4);

    Normalise *normdata_truth_3 = new Normalise(h_data_truth_3, 0, 1, false);
    TH1D* data_truth_norm_3 = normdata_truth_3->GetHist();
    Normalise *normdata_truth_4 = new Normalise(h_data_truth_4, 0, 1, false);
    TH1D* data_truth_norm_4 = normdata_truth_4->GetHist();

  Normalise *normdata_truth_bin_3 = new Normalise(h_data_truth_3, 0, 1, true);
  TH1D* data_truth_norm_bin_3 = normdata_truth_bin_3->GetHist();
  Normalise *normdata_truth_bin_4 = new Normalise(h_data_truth_4, 0, 1, true);
  TH1D* data_truth_norm_bin_4 = normdata_truth_bin_4->GetHist();

    plot->Plot_result_with_uncertainty(data_unfolded_stat, data_unfolded_sys, h_data_truth_3, h_data_truth_4, h_mc_truth, false, sample_name, directory+"Event_FSR_");
    plot->Plot_result_with_uncertainty(data_unfolded_stat_cs, data_unfolded_sys_cs, h_data_truth_3_cs, h_data_truth_4_cs, h_mc_truth_cs, false, sample_name, directory+"CS_FSR_");
    plot->Plot_result_with_uncertainty(normed_stat, normed_tot, data_truth_norm_3, data_truth_norm_4, mc_truth_norm, true, sample_name, directory+"CS_FSR_normed_");
    plot->Plot_result_with_uncertainty(normed_stat_bin, normed_tot_bin, data_truth_norm_bin_3, data_truth_norm_bin_4, mc_truth_norm_bin, true, sample_name, directory+"CS_FSR_normed_bin_");
  }





  TH1D* h_purity_all = (TH1D*) file->Get("Purity total");
  TH1D* h_stability_all = (TH1D*) file->Get("Stability total");
  TH1D* h_purity_samebin = (TH1D*) file->Get("Purity samebin");
  TH1D* h_stability_samebin = (TH1D*) file->Get("Stability samebin");
  plot->Plot_purity(h_purity_samebin, h_purity_all, directory);
  plot->Plot_purity(h_stability_samebin, h_stability_all, directory+"Stability");

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


  TH1D* input_data_dist = (TH1D*) file->Get("Data Distribution");
  TH1D* input_mc_dist = (TH1D*) file->Get("MC rec Distribution");
  TH1D* input_data = (TH1D*) file->Get("Data input");
  TH1D* input_mc = (TH1D*) file->Get("MC rec input");
  if(isdata){
    TH1D* input_background_dist = (TH1D*) file->Get("Background rec Distribution");
    TH1D* input_background = (TH1D*) file->Get("Background rec input");
    plot->Plot_input(input_data_dist, input_mc_dist, input_background_dist, directory+"dist");
    plot->Plot_input(input_data, input_mc, input_background, directory);
  }
  else{
    plot->Plot_input(input_data_dist, input_mc_dist, directory+"dist");
    plot->Plot_input(input_data, input_mc, directory);
  }


  TH2D* correlation_matrix = (TH2D*) file->Get("Correlation Matrix (meas region) TauScan");
  plot->Plot_covariance(correlation_matrix, directory+"Correlation");

  TH2D* cov_matrix_input = (TH2D*) file->Get("Covariance of input (meas region) TauScan");
  plot->Plot_covariance(cov_matrix_input, directory+"Covariance_input");

  TH2D* cov_matrix_input_all = (TH2D*) file->Get("Covariance of input (all) TauScan");
  plot->Plot_covariance(cov_matrix_input_all, directory+"Covariance_input_all");

  if(directory.Contains("Pseudo")){
    TFile *file_pseudo1 = new TFile("Unfoldings/Unfolding_Pseudo_1"+jetcol+".root", "READ");
    TFile *file_pseudo2 = new TFile("Unfoldings/Unfolding_Pseudo_2"+jetcol+".root", "READ");
    TFile *file_pseudo3 = new TFile("Unfoldings/Unfolding_Pseudo_3"+jetcol+".root", "READ");

    cout << "file: " << "Unfoldings/Unfolding_Pseudo_3"<<jetcol<<".root" << '\n';
    TH1D* h_pseudo1 = (TH1D*) file_pseudo1->Get("Unfolded data (meas region) TauScan");
    TH1D* pseudo1_unfolded_stat = SetSysError(h_pseudo1, h_covariance_input_tauscan);
    TH1D* pseudo1_unfolded_sys = SetSysError(h_pseudo1, CovTotal);
    pseudo1_unfolded_sys->SetName("Pseudo1 test");
    TH1D* h_pseudo1_truth = (TH1D*) file_pseudo1->Get("Data Truth");

    TH1D* h_pseudo2 = (TH1D*) file_pseudo2->Get("Unfolded data (meas region) TauScan");
    TH1D* pseudo2_unfolded_stat = SetSysError(h_pseudo2, h_covariance_input_tauscan);
    TH1D* pseudo2_unfolded_sys = SetSysError(h_pseudo2, CovTotal);
    pseudo2_unfolded_sys->SetName("Pseudo2 test");
    TH1D* h_pseudo2_truth = (TH1D*) file_pseudo2->Get("Data Truth");

    TH1D* h_pseudo3 = (TH1D*) file_pseudo3->Get("Unfolded data (meas region) TauScan");
    TH1D* pseudo3_unfolded_stat = SetSysError(h_pseudo3, h_covariance_input_tauscan);
    TH1D* pseudo3_unfolded_sys = SetSysError(h_pseudo3, CovTotal);
    pseudo3_unfolded_sys->SetName("Pseudo3 test");
    TH1D* h_pseudo3_truth = (TH1D*) file_pseudo3->Get("Data Truth");
    TH1D* h_pseudo1_cs = ConvertToCrossSection(pseudo1_unfolded_sys);
    TH1D* h_pseudo1_truth_cs = ConvertToCrossSection(h_pseudo1_truth);
    TH1D* h_pseudo2_cs = ConvertToCrossSection(pseudo2_unfolded_sys);
    TH1D* h_pseudo2_truth_cs = ConvertToCrossSection(h_pseudo2_truth);
    TH1D* h_pseudo3_cs = ConvertToCrossSection(pseudo3_unfolded_sys);
    TH1D* h_pseudo3_truth_cs = ConvertToCrossSection(h_pseudo3_truth);


    plot->Plot_all_pseudo(pseudo1_unfolded_sys, h_pseudo1_truth, pseudo2_unfolded_sys, h_pseudo2_truth, pseudo3_unfolded_sys, h_pseudo3_truth, false, directory+"All_uncer_");
    plot->Plot_all_pseudo(h_pseudo1_cs, h_pseudo1_truth_cs, h_pseudo2_cs, h_pseudo2_truth_cs, h_pseudo3_cs, h_pseudo3_truth_cs, true, directory+"CS_All_uncer_");
    file_pseudo1->Close();
    file_pseudo2->Close();
    file_pseudo3->Close();

  }

  if(directory.Contains("comb")){
    TFile *file_mu = new TFile("Unfoldings/Unfolding_"+dataset+jetcol2+"_mu.root", "READ");
    TFile *file_ele = new TFile("Unfoldings/Unfolding_"+dataset+jetcol2+"_ele.root", "READ");
    cout << "Unfoldings/Unfolding_"<<dataset<<jetcol2<<"_ele.root" << '\n';
    vector<vector<TString>> sys_name_mu, sys_name_ele;
    sys_name_mu = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
    sys_name_ele = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
    TH1D* h_unfolded_tauscan_mu = (TH1D*) file_mu->Get("Unfolded data (meas region) TauScan");
    TH2D* h_covariance_input_tauscan_mu = (TH2D*) file_mu->Get("Covariance of input (meas region) TauScan");
    TH2D* h_covariance_covmatrix_tauscan_mu = (TH2D*) file_mu->Get("Covariance of CovMatrix (meas region) TauScan");
    TH2D* CovStat_mu = (TH2D*) h_covariance_input_tauscan_mu->Clone("covstat mu clone");
    TH1D* data_unfolded_stat_mu = SetSysError(h_unfolded_tauscan_mu, h_covariance_input_tauscan_mu);

    CovStat_mu->Add(h_covariance_covmatrix_tauscan_mu);
    if(isdata){
      vector<TH2D*> CovBgrStat_mu;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_mu->Get("Covariance of " + background_names.at(i) + " TauScan");
        CovBgrStat_mu.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrStat_mu){
        CovStat_mu->Add(bgrcov);
      }
    }
    TH2D* total_cov_mu = (TH2D*) CovStat_mu->Clone("total cov mu clone");
    vector<vector<TH1D*>> sys_del_mu;
    if(isdata){
      vector<TH2D*> CovBgrScale_mu;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_mu->Get("Scale of " + background_names.at(i) + " TauScan");
        CovBgrScale_mu.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrScale_mu){
        total_cov_mu->Add(bgrcov);
      }
      for(unsigned int i = 0; i < sys_name_mu.size(); i++){
        vector<TH1D*> dummy;
        for(unsigned int j = 0; j < sys_name_mu[i].size(); j++){
          TH1D* temp_hist = (TH1D*) file_mu->Get("Delta of "+sys_name_mu[i][j]+" TauScan");
          // cout << "sysName: " << sys_name_mu[i][j] << '\n';
          // cout << "nbins: " << temp_hist->GetNbinsX() << '\n';
          dummy.push_back(temp_hist);
        }
        sys_del_mu.push_back(dummy);
      }
      vector<vector<TH2D*>> sys_cov_mu;
      for(unsigned int i = 0; i < sys_name_mu.size(); i++){
        vector<TH2D*> dummy;
        for(unsigned int j = 0; j < sys_name_mu[i].size(); j++){
          TH2D* temp_hist = (TH2D*) file->Get("Covariance of "+sys_name_mu[i][j]+"TauScan");
          dummy.push_back(temp_hist);
        }
        sys_cov_mu.push_back(dummy);
      }
      for(unsigned int i=0; i<sys_del_mu.size(); i++){
        int j = FindLargestVariation(sys_del_mu.at(i));
        total_cov_mu->Add(sys_cov_mu[i][j]);
      }
    }



    TH1D* h_unfolded_tauscan_ele = (TH1D*) file_ele->Get("Unfolded data (meas region) TauScan");

    TH2D* h_covariance_input_tauscan_ele = (TH2D*) file_ele->Get("Covariance of input (meas region) TauScan");
    TH2D* h_covariance_covmatrix_tauscan_ele = (TH2D*) file_ele->Get("Covariance of CovMatrix (meas region) TauScan");
    TH2D* CovStat_ele = (TH2D*) h_covariance_input_tauscan_ele->Clone("covstat ele clone");

    TH1D* data_unfolded_stat_ele = SetSysError(h_unfolded_tauscan_ele, h_covariance_input_tauscan_ele);
    CovStat_ele->Add(h_covariance_covmatrix_tauscan_ele);
    if(isdata){
      vector<TH2D*> CovBgrStat_ele;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_ele->Get("Covariance of " + background_names.at(i) + " TauScan");
        CovBgrStat_ele.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrStat_ele){
        CovStat_ele->Add(bgrcov);
      }
    }
    TH2D* total_cov_ele = (TH2D*) CovStat_ele->Clone("totalcov ele clone");
    vector<vector<TH1D*>> sys_del_ele;
    if(isdata){
      vector<TH2D*> CovBgrScale_ele;
      for(unsigned int i = 0; i < background_names.size(); i++){
        TH2D* temp_hist = (TH2D*) file_ele->Get("Scale of " + background_names.at(i) + " TauScan");
        CovBgrScale_ele.push_back(temp_hist);
      }
      for(auto bgrcov: CovBgrScale_ele){
        total_cov_ele->Add(bgrcov);
      }
      for(unsigned int i = 0; i < sys_name_ele.size(); i++){
        vector<TH1D*> dummy;
        for(unsigned int j = 0; j < sys_name_ele[i].size(); j++){
          TH1D* temp_hist = (TH1D*) file_ele->Get("Delta of "+sys_name_ele[i][j]+" TauScan");
          dummy.push_back(temp_hist);
        }
        sys_del_ele.push_back(dummy);
      }
      vector<vector<TH2D*>> sys_cov_ele;
      for(unsigned int i = 0; i < sys_name_ele.size(); i++){
        vector<TH2D*> dummy;
        for(unsigned int j = 0; j < sys_name_ele[i].size(); j++){
          TH2D* temp_hist = (TH2D*) file->Get("Covariance of "+sys_name_ele[i][j]+"TauScan");
          dummy.push_back(temp_hist);
        }
        sys_cov_ele.push_back(dummy);
      }
      for(unsigned int i=0; i<sys_del_ele.size(); i++){
        int j = FindLargestVariation(sys_del_ele.at(i));
        total_cov_ele->Add(sys_cov_ele[i][j]);
      }
    }



    TH1D* data_unfolded_sys_mu = SetSysError(h_unfolded_tauscan_mu, total_cov_mu);
    data_unfolded_sys_mu->SetName("Unfolded Data with sys errors_mu");

    TH1D* data_unfolded_sys_ele = SetSysError(h_unfolded_tauscan_ele, total_cov_ele);
    data_unfolded_sys_ele->SetName("Unfolded Data with sys errors_ele");

    TH1D* data_unfolded_stat_mu_cs = ConvertToCrossSection(data_unfolded_stat_mu);
    TH1D* data_unfolded_sys_mu_cs = ConvertToCrossSection(data_unfolded_sys_mu);
    TH1D* data_unfolded_stat_ele_cs = ConvertToCrossSection(data_unfolded_stat_ele);
    TH1D* data_unfolded_sys_ele_cs = ConvertToCrossSection(data_unfolded_sys_ele);

    plot->Plot_compatibility(data_unfolded_stat_mu, data_unfolded_sys_mu, data_unfolded_stat_ele, data_unfolded_sys_ele, directory);
    plot->Plot_compatibility(data_unfolded_stat_mu_cs, data_unfolded_sys_mu_cs, data_unfolded_stat_ele_cs, data_unfolded_sys_ele_cs, directory+"CS_");
    file_mu->Close();
    file_ele->Close();
  }
  file->Close();
  return 0;
}



int FindLargestVariation(vector<TH1D*> variations){
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

TH1D* ConvertToRelative(TH1D* sys, TH1D* central){
  int nbins = central->GetXaxis()->GetNbins();
  TString name = sys->GetName();
  name += " Clone";
  TH1D* hist = (TH1D*)sys->Clone(name);
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

TH1D* AddSys(vector<TH1D*> sys){
  int nbins = sys[0]->GetXaxis()->GetNbins();
  TH1D* hist = (TH1D*)sys[0]->Clone("hist");
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
TH1D* SetSysError(TH1D* data_unfolded, TH2D* CovTotal){
  TH1D* hist = (TH1D*) data_unfolded->Clone("just a clone");
  int nbins = hist->GetXaxis()->GetNbins();
  for(int i=1; i<=nbins; i++){
    double error = sqrt(CovTotal->GetBinContent(i,i));
    hist->SetBinError(i, error);
  }
  return hist;
}

// create delta distribution from COV
TH1D* CreateDeltaFromCov(TH2D* Cov){
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
void ScaleErrorToData(TH1D* hist){
  int nbins = hist->GetXaxis()->GetNbins();
  for(int bin=1; bin<=nbins; bin++){
    double N = hist->GetBinContent(bin);
    hist->SetBinError(bin, sqrt(N));
  }
  return;
}

TH1D* GetModelDelta(TH1D* unfolded, TH1D* truth){
  TH1D* delta = (TH1D*) unfolded->Clone();
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


TH2D* CreateCovFromDelta(TH1D* delta, TH2D* dummyCov){
  int nbins = delta->GetXaxis()->GetNbins();
  TH2D* cov = (TH2D*) dummyCov->Clone();
  cov->Reset();
  for(int i=1; i<=nbins; i++){
    for(int j=1; j<=nbins; j++){
      double entry = delta->GetBinContent(i) * delta->GetBinContent(j);
      cov->SetBinContent(i,j,entry);
    }
  }
  return cov;
}

int FindLargestVariationByMean(vector<TH1D*> truth, vector<TH1D*> variations, bool use_significance){
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


TH1D* ConstructAverage(vector<TH1D*> variations, int varfrom){
  if(variations.size() == 0) cout << "AVERAGE CAN NOT BE CONSTRUCTED" << endl;
  TH1D* average = (TH1D*) variations[0]->Clone("your average clone");
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

TH1D* ConvertToNewBinning_1D(TH1D* oldhist, vector<int> binmap, const TUnfoldBinning *binning, TString name){
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

  TH1D* newhist = (TH1D*) meas->CreateHistogram("", kTRUE, 0, 0, "mass[C]");
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
  TH1D* newnewhist = new TH1D(name+" new binning", "Interesting Title", dummy_bins.size()-1, bins);
  for(int i = 1; i <= newhist->GetNbinsX(); i++){
    newnewhist->SetBinContent(i, newhist->GetBinContent(i));
    newnewhist->SetBinError(i, newhist->GetBinError(i));
  }

  return newnewhist;
}

TH2D* ConvertToNewBinning_2D(TH2D* oldcov, vector<int> binmap, const TUnfoldBinning *binning, TString name){
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

  TH2D* newcov = meas->CreateErrorMatrixHistogram(name, kTRUE, 0, 0, "mass[C]");
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
  TH2D* newnewcov = new TH2D(name+" new binning", "Interesting Title", dummy_bins.size()-1, bins, dummy_bins.size()-1, bins);
  for(int i = 1; i <= newcov->GetNbinsX(); i++){
    for(int j = 1; j <= newcov->GetNbinsY(); j++){
      newnewcov->SetBinContent(i, j, newcov->GetBinContent(i, j));
    }
  }
  return newnewcov;
}

TH1D* ConvertToCrossSection(TH1D* hist){
  TH1D* newhist = (TH1D*) hist->Clone();
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

TH2D* ConvertToCrossSection(TH2D* cov){
  TH2D* newcov = (TH2D*) cov->Clone();
  newcov->Reset();
  int nbins = cov->GetXaxis()->GetNbins();
  for(int i=1; i<=nbins; i++){
    for(int j=1; j<=nbins; j++){
      double binwidth_i = cov->GetXaxis()->GetBinWidth(i);
      double binwidth_j = cov->GetYaxis()->GetBinWidth(j);
      double old_entry = cov->GetBinContent(i,j);
      double new_entry = old_entry/(binwidth_i*binwidth_j*35.9*35.9);
      newcov->SetBinContent(i,j, new_entry);
    }
  }
  return newcov;
}

vector<double> TotalCrossSection(TH1D* hist, TH2D* cov){
  vector<double> total_xs;
  double xs_content = 0;
  double xs_error2 = 0;
  int nbins = hist->GetXaxis()->GetNbins();
  for(int i=1; i<=nbins; i++){
    double xs_bin = hist->GetBinContent(i) * hist->GetBinWidth(i);
    xs_content += xs_bin;
    for(int j=1; j<=nbins; j++){
      double binwidth_i = hist->GetBinWidth(i);
      double binwidth_j = hist->GetBinWidth(j);
      double cov_bin = cov->GetBinContent(i,j) * binwidth_i * binwidth_j;
      xs_error2 += cov_bin;
    }
  }
  double xs_error = sqrt(xs_error2);
  total_xs.push_back(xs_content);
  total_xs.push_back(xs_error);
  return total_xs;
}

TH2D* CreateCorrelationMatrix(TH2D* cov){
  TH2D *cov_clone = (TH2D*) cov->Clone();
  TH2D *corr = (TH2D*) cov->Clone();
  corr->Reset();
  int entries_x = cov_clone->GetNbinsX();
  int entries_y = cov_clone->GetNbinsY();

  for(int i=1; i <= entries_x; i++){
    for(int j=1; j <= entries_y; j++){
      double cov_value = cov_clone->GetBinContent(i,j);
      double cov_ii = cov_clone->GetBinContent(i,i);
      double cov_jj = cov_clone->GetBinContent(j,j);
      double entry = cov_value/(sqrt(cov_ii)*sqrt(cov_jj));
      corr->SetBinContent(i,j,entry);
    }
  }
  return corr;
}
