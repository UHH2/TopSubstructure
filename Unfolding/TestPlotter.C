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

int main(int argc, char* argv[]){
  if(argc < 2) throw runtime_error("How to use Plotter: ./Plotter <Filename>");
  TString directory = "/afs/desy.de/user/s/skottkej/Plots/TEST/";
  TString File_name = argv[1];

  bool isdata = false;
  bool ispseudodata = false;
  if(File_name.Contains("data")) isdata = true;
  if(File_name.Contains("madgraph")) ispseudodata = true;

  vector<TString> background_names = {"DYJets", "ST", "Diboson"};
  vector<vector<TString>> sys_name;
  if(File_name.Contains("mu")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}};
  else if(File_name.Contains("ele")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};
  else if(File_name.Contains("comb")) sys_name = {{"JECup", "JECdown"}, {"JERup", "JERdown"}, {"BTagup", "BTagdown"}, {"PUup", "PUdown"}, {"MUIDup", "MUIDdown"}, {"MUTriggerup", "MUTriggerdown"}, {"ELEIDup", "ELEIDdown"}, {"ELETriggerup", "ELETriggerdown"}, {"ELERecoup", "ELERecodown"}};

  vector<vector<TString>> model_name;
  if(isdata) model_name = {{"scale_uu", "scale_un", "scale_nu", "scale_nd", "scale_dn", "scale_dd"}, {"mtop1695", "mtop1715", "mtop1735", "mtop1755"}, {"madgraph"}};
  if(ispseudodata) model_name = {{"scale_uu", "scale_un", "scale_nu", "scale_nd", "scale_dn", "scale_dd"}, {"mtop1695", "mtop1715", "mtop1735", "mtop1755"}};

  TFile *file = new TFile(File_name, "READ");
  TString jetcol = "";
  if(File_name.Contains("puppi_sd")) jetcol = "_puppi_sd";
  else if(File_name.Contains("puppi")) jetcol = "_puppi";
  else if(File_name.Contains("sd")) jetcol = "_sd";

  TUnfoldBinning* binning_rec = (TUnfoldBinning*) file->Get("binning_rec");
  TUnfoldBinning* binning_gen = (TUnfoldBinning*) file->Get("binning_gen");
  TH1D* projection_gen = (TH1D*) file->Get("Projection of gen distribution");
  TH1D* projection_rec = (TH1D*) file->Get("Projection of rec distribution");
  TH1* h_input_gen = (TH1*) file->Get("MC gen Distribution");
  TH1* h_input_rec = (TH1*) file->Get("MC rec Distribution");

  Plotter *plot = new Plotter(binning_gen, binning_rec);

  string input(argv[1]);
  vector<string> result;
  boost::split(result, input, boost::is_any_of("_ ."));
  for(unsigned int i = 0; i < result.size(); i++){
    if(result[i] != "mu" && result[i] != "ele" && result[i] != "comb"){
      if(i > 0 && i < result.size()-1) directory += result[i];
      if(i < result.size() - 3 && i > 0) directory += "_";
    }
  }
  directory += "/";
  if(File_name.Contains("mu"))        directory += "mu/";
  else if(File_name.Contains("ele"))  directory += "ele/";
  else if(File_name.Contains("comb")) directory += "comb/";
  if(File_name.Contains("mu"))        jetcol += "_mu";
  else if(File_name.Contains("ele"))  jetcol += "_ele";
  else if(File_name.Contains("comb")) jetcol += "_comb";

  plot->Plot_projections(projection_gen, h_input_gen, projection_rec, h_input_rec, directory);

  TH1D* h_unfolded_tauscan = (TH1D*) file->Get("Unfolded data (meas region) TauScan");
  TH1D* h_unfolded_tauscan_part = (TH1D*) file->Get("Unfolded data (meas region, m>155) TauScan");
  TH1D* h_data_truth = (TH1D*) file->Get("Data Truth");
  TH1D* h_mc_truth = (TH1D*) file->Get("MC Truth");
  plot->Plot_output(h_unfolded_tauscan, h_data_truth, false, directory, "Unfolding_result_Tauscan");
  plot->Plot_output(h_unfolded_tauscan, h_data_truth, true, directory, "Unfolding_result_normalised_Tauscan");
  plot->Plot_output(h_unfolded_tauscan_part, h_data_truth, false, directory, "Unfolding_result_Tauscan_part");
  plot->Plot_output(h_unfolded_tauscan_part, h_data_truth, true, directory, "Unfolding_result_normalised_Tauscan_part");

  TH1D* h_data_truth_all = (TH1D*) file->Get("Whole Data Truth");
  TH1D* h_mc_truth_all = (TH1D*) file->Get("Whole MC Truth");
  TH1D* h_unfolded_tauscan_all = (TH1D*) file->Get("Unfolded data (all) TauScan");
  plot->Plot_output(h_unfolded_tauscan_all, h_data_truth_all, false, directory, "Unfolding_result_all_Tauscan");

  TH2D* h_covariance_input_tauscan = (TH2D*) file->Get("Covariance of input (meas region) TauScan");
  TH2D* h_covariance_covmatrix_tauscan = (TH2D*) file->Get("Covariance of CovMatrix (meas region) TauScan");

  TH2D* h_response = (TH2D*) file->Get("response matrix");
  plot->Plot_ResponseMatrix(h_response, directory+"Migration_Matrix");

  TH2D* h_probability = (TH2D*) file->Get("Probability Matrix TauScan");
  plot->Plot_ResponseMatrix(h_probability, directory+"Probability_Matrix");



  vector<vector<TH1*>> model_output, model_delta;
  vector<vector<TH1D*>> model_truth;
  vector<vector<TH2*>> CovModel;
  if(isdata){
    for(unsigned int i=0; i<model_name.size(); i++){
      vector<TH1*> dummy;
      model_output.push_back(dummy);
      model_delta.push_back(dummy);
      vector<TH1D*> dummyd;
      model_truth.push_back(dummyd);
      vector<TH2*> dummy2;
      CovModel.push_back(dummy2);
      for(unsigned int j=0; j<model_name[i].size(); j++){
        cout << "***********************" << endl;
        cout << " UNFOLDING OF " << model_name[i][j] << endl;
        TFile *file_model = new TFile("Unfoldings/Unfolding_"+model_name[i][j]+jetcol+".root", "READ");
        TH1* output = (TH1*) file_model->Get("Unfolded data (meas region) TauScan");
        TH1D* truth = (TH1D*) file_model->Get("Data Truth");
        model_output[i].push_back(output);
        model_truth[i].push_back(truth);
        TH1* delta = GetModelDelta(output, model_truth[i][j]);
        TH2* cov = CreateCovFromDelta(delta, h_covariance_input_tauscan);
        model_delta[i].push_back(delta);
        CovModel[i].push_back(cov);

        cout << "unfolding finished" << endl;
      }
    }
  }


  // add up stat Cov
  cout << "sum up stat cov matrices" << endl;
  vector<TH1*> error, error_model;
  vector<TString> error_name, error_name_model;
  TH2D* CovStat = (TH2D*) h_covariance_input_tauscan->Clone();
  TH2D* CovStat_model = (TH2D*) h_covariance_input_tauscan->Clone();
  TH1* stat_delta = CreateDeltaFromCov(CovStat);
  TH1* stat_rel = ConvertToRelative(stat_delta, h_unfolded_tauscan);
  error.push_back(stat_rel);
  error_model.push_back(stat_rel);
  error_name.push_back("stat");
  error_name_model.push_back("stat");

  TH2D* mc_error = (TH2D*) h_covariance_covmatrix_tauscan->Clone();
  CovStat->Add(h_covariance_covmatrix_tauscan);
  CovStat_model->Add(h_covariance_covmatrix_tauscan);

  if(isdata){
    vector<TH2*> CovBgrStat;
    for(unsigned int i = 0; i < background_names.size(); i++)
    CovBgrStat.push_back((TH2D*) file->Get("Covariance of " + background_names.at(i) + " TauScan"));
    for(auto bgrcov: CovBgrStat){
      CovStat->Add(bgrcov);
      CovStat_model->Add(bgrcov);
      mc_error->Add(bgrcov);
    }
  }
  TH1* MC_stat_delta = CreateDeltaFromCov(mc_error);
  TH1* MC_stat_rel = ConvertToRelative(MC_stat_delta, h_unfolded_tauscan);

  error.push_back(MC_stat_rel);
  error_model.push_back(MC_stat_rel);
  error_name.push_back("MC stat");
  error_name_model.push_back("MC stat");

  // // then add sys cov from backgrounds
  cout << "sum up background sys cov matrices" << endl;
  TH2* total_cov = (TH2*) CovStat->Clone();
  TH2* total_model_cov = (TH2*) CovStat->Clone();
  vector<vector<TH1*>> sys_del;
  if(isdata){
    vector<TH2*> CovBgrScale;
    for(unsigned int i = 0; i < background_names.size(); i++) CovBgrScale.push_back((TH2D*) file->Get("Scale of " + background_names.at(i) + " TauScan"));

    cout << "size of CovBgrScale: " << CovBgrScale.size() << '\n';

    TH2* dummy_bgr_scale;
    dummy_bgr_scale = (TH2*) CovBgrScale[0]->Clone();
    dummy_bgr_scale->Reset();
    for(auto bgrcov: CovBgrScale) dummy_bgr_scale->Add(bgrcov);
    TH1* sys_bkg_delta;
    TH1* sys_bkg_rel;
    sys_bkg_delta = CreateDeltaFromCov(dummy_bgr_scale);
    sys_bkg_rel = ConvertToRelative(sys_bkg_delta, h_unfolded_tauscan);
    error.push_back(sys_bkg_rel);
    error_name.push_back("background sys");
    for(auto bgrcov: CovBgrScale) total_cov->Add(bgrcov);

    // then add sys cov (and convert used uncertainty to relative hist)
    cout << "sum up experimental sys cov matrices" << endl;

    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH1*> dummy;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        dummy.push_back((TH1*) file->Get("Delta of "+sys_name[i][j]+" TauScan"));
      }
      sys_del.push_back(dummy);
    }


    vector<vector<TH2*>> sys_cov;
    for(unsigned int i = 0; i < sys_name.size(); i++){
      vector<TH2*> dummy;
      for(unsigned int j = 0; j < sys_name[i].size(); j++){
        dummy.push_back((TH2*) file->Get("Covariance of "+sys_name[i][j]+"TauScan"));
      }
      sys_cov.push_back(dummy);
    }

    TH2* cov_sys = (TH2*) CovStat->Clone();
    cov_sys->Reset();
    vector<TH1*> sys_rel;
    for(unsigned int i=0; i<sys_del.size(); i++){
      int j = FindLargestVariation(sys_del.at(i));
      total_cov->Add(sys_cov[i][j]);
      cov_sys->Add(sys_cov[i][j]);
      sys_rel.push_back(ConvertToRelative(sys_del[i][j], h_unfolded_tauscan));
      error.push_back(sys_rel[i]);
      error_name.push_back(sys_name[i][j]);
    }

    cout << "sum up model sys cov matrices" << endl;
    vector<TH1*> model_delta_average;
    vector<TH1*> model_rel;
    for(unsigned int i=0; i<model_delta.size(); i++){
      int j = FindLargestVariationByMean(model_truth[i], model_output[i], false);
      TH1* delta_average = ConstructAverage(model_delta[i], j);
      model_delta_average.push_back(delta_average);
      TH2* cov_average = CreateCovFromDelta(delta_average, h_covariance_input_tauscan);
      TH1* chosen_delta;
      TH2* chosen_cov;
      chosen_cov = cov_average;
      chosen_delta = delta_average;
      model_rel.push_back(ConvertToRelative(chosen_delta, h_unfolded_tauscan));
      error_model.push_back(model_rel[i]);
      error_name_model.push_back(model_name[i][j]);
      CovStat_model->Add(chosen_cov);
    }
    model_rel.push_back(stat_rel);            // put in stat to get total
  }
  TH1* data_unfolded_sys = SetSysError(h_unfolded_tauscan, total_cov);
  data_unfolded_sys->SetName("Unfolded Data with sys errors");

  TH1* sys_tot_delta = CreateDeltaFromCov(total_cov);
  TH1* sys_tot_rel = ConvertToRelative(sys_tot_delta, h_unfolded_tauscan);

  error.push_back(sys_tot_rel);
  error_name.push_back("stat #oplus exp sys");



  TH1* model_tot_delta = CreateDeltaFromCov(CovStat_model);
  TH1* model_tot_rel = ConvertToRelative(model_tot_delta, h_unfolded_tauscan);
  error_model.push_back(model_tot_rel);
  error_name_model.push_back("stat #oplus model sys");

  plot->Plot_delta(model_delta, model_name, directory);

  if(isdata){
    sys_del[sys_del.size()-1].push_back(MC_stat_delta);
    sys_name[sys_del.size()-1].push_back("Background");
  }
  plot->Plot_delta(sys_del, sys_name, directory);
  plot->Plot_uncertainty(error, error_name, directory+"EXP_");
  plot->Plot_uncertainty(error_model, error_name_model, directory+"Model_");
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan, data_unfolded_sys, h_data_truth, h_mc_truth, false, directory);
  plot->Plot_result_with_uncertainty(h_unfolded_tauscan, data_unfolded_sys, h_data_truth, h_mc_truth, true, directory+"Normalised_");

  TH1D* h_purity_all = (TH1D*) file->Get("Purity total");
  TH1D* h_purity_samebin = (TH1D*) file->Get("Purity samebin");
  plot->Plot_purity(h_purity_samebin, h_purity_all, directory);

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


  TH2* correlation_matrix = (TH2*) file->Get("Correlation Matrix TauScan");
  plot->Plot_covariance(correlation_matrix, directory+"Correlation");

  TH2* cov_matrix_input = (TH2*) file->Get("Covariance of input (meas region) TauScan");
  plot->Plot_covariance(cov_matrix_input, directory+"Covariance_input");

  TH2* cov_matrix_input_all = (TH2*) file->Get("Covariance of input (all) TauScan");
  plot->Plot_covariance(cov_matrix_input_all, directory+"Covariance_input_all");

  if(directory.Contains("pseudo")){
    TFile *file_pseudo1 = new TFile("Unfoldings/Unfolding_pseudo1"+jetcol+".root", "READ");
    TFile *file_pseudo2 = new TFile("Unfoldings/Unfolding_pseudo2"+jetcol+".root", "READ");
    TFile *file_pseudo3 = new TFile("Unfoldings/Unfolding_pseudo3"+jetcol+".root", "READ");
    TH1* h_pseudo1 = (TH1*) file_pseudo1->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo1_truth = (TH1*) file_pseudo1->Get("Data Truth");
    TH1* h_pseudo2 = (TH1*) file_pseudo2->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo2_truth = (TH1*) file_pseudo2->Get("Data Truth");
    TH1* h_pseudo3 = (TH1*) file_pseudo3->Get("Unfolded data (meas region) TauScan");
    TH1* h_pseudo3_truth = (TH1*) file_pseudo3->Get("Data Truth");
    plot->Plot_all_pseudo(h_pseudo1, h_pseudo1_truth, h_pseudo2, h_pseudo2_truth, h_pseudo3, h_pseudo3_truth, directory);
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
  TH1* hist = (TH1*)sys->Clone("hist");
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
  TH1* hist = (TH1*) data_unfolded->Clone();
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
  vector<double> bins;
  vector<double> values;
  bins.push_back(Cov->GetXaxis()->GetBinLowEdge(1));
  for(int i=1; i<=nbins; i++){
    bins.push_back(Cov->GetXaxis()->GetBinUpEdge(i));
    values.push_back( sqrt(Cov->GetBinContent(i,i)) );
  }
  TH1D* delta = new TH1D(" ", " ", nbins, &bins[0]);
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
  TH1* delta = (TH1*) unfolded->Clone();
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
  TH2* cov = (TH2*) dummyCov->Clone();
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
  TH1* average = (TH1*) variations[0]->Clone();
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
