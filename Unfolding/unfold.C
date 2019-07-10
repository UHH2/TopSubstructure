#include "unfold.h"

using namespace std;

unfolding::unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response, TH1D* h_truth, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<std::vector<TH2*>> sys_matrix, std::vector<std::vector<TString>> sys_name, std::vector<TH1D*> background, std::vector<TString> background_names, int nscan, TString regmode_, TString density_flag, bool do_lcurve, bool subtract_background, double tau_value){
  response_matrix = response;
  hist_truth = h_truth;
  hist_mc = h_mc;

  // preserve the area
  TUnfold::EConstraint constraintMode = TUnfold::kEConstraintNone;
  // TUnfold::EConstraint constraintMode = TUnfold::kEConstraintArea;

  // basic choice of regularisation scheme:
  //    curvature (second derivative)
  if(regmode_ == "size")            regMode = TUnfold::kRegModeSize;
  else if(regmode_ == "derivative") regMode = TUnfold::kRegModeDerivative;
  else if(regmode_ == "curvature")  regMode = TUnfold::kRegModeCurvature; //Derivative testen
  else throw std::runtime_error("unfold.C: Use regmode: 'size', 'derivative' or 'curvature'!");

  // density flags
  if(density_flag == "none")                 densityFlags = TUnfoldDensity::kDensityModeNone;
  else if(density_flag == "binwidth")        densityFlags = TUnfoldDensity::kDensityModeBinWidth;
  else if(density_flag == "binwidthanduser") densityFlags = TUnfoldDensity::kDensityModeBinWidthAndUser;
  else if(density_flag == "user")            densityFlags = TUnfoldDensity::kDensityModeUser;
  else throw std::runtime_error("unfold.C: Use density_flag: 'none', 'binwidth', 'binwidthanduser' or 'user'!");

  // detailed steering for regularisation
  const char *REGULARISATION_DISTRIBUTION = 0;
  // const char *REGULARISATION_AXISSTEERING = "*[C]";
  const char *REGULARISATION_AXISSTEERING = 0;

  // set up matrix of migrations
  TUnfoldDensity unfold(response_matrix, TUnfold::kHistMapOutputHoriz, regMode, constraintMode, densityFlags, binning_gen, binning_rec, REGULARISATION_DISTRIBUTION, REGULARISATION_AXISSTEERING);
  TUnfoldDensity unfold_check(response_matrix, TUnfold::kHistMapOutputHoriz, regMode, constraintMode, densityFlags, binning_gen, binning_rec, REGULARISATION_DISTRIBUTION, REGULARISATION_AXISSTEERING);


  double background_integral = 0;
  if(subtract_background){
    for(unsigned int i = 0; i < background.size(); i++){
      background_integral += background.at(i)->Integral();
    }
  }
  double sf_ratio = (h_data->Integral()-background_integral)/hist_mc->Integral();
  cout << "ratio between data and mc (ttbar): " << sf_ratio << '\n';
  unfold.SetInput(h_data, sf_ratio);
  unfold_check.SetInput(hist_mc, 1);

  if(subtract_background){
    for(unsigned int i = 0; i < background.size(); i++){
      unfold.SubtractBackground(background.at(i), background_names.at(i));
      cout << "name of background: " << background_names.at(i) << '\n';
    }
  }

  TString unfolding_input = "Pseudodata Input ";
  TString unfolding_result = "Unfolded Pseudodata ";
  TString unfolding_corr = "Correlation Matrix ";
  TString unfolding_check = "Unfolded mc ";
  TString unfolding_covinput = "Covariance of mc ";
  TString unfolding_covmatrix = "Covariance of matrix ";
  TString unfolding_covtotal = "Covariance of total ";

  if(tau_value < 0){
    if(do_lcurve){
      unfolding_input += "LCurve";
      unfolding_result += "LCurve";
      unfolding_corr += "LCurve";
      unfolding_check += "LCurve";
      unfolding_covinput += "LCurve";
      unfolding_covmatrix += "LCurve";
      unfolding_covtotal += "LCurve";
    }
    else{
      unfolding_input += "TauScan";
      unfolding_result += "TauScan";
      unfolding_corr += "TauScan";
      unfolding_check += "TauScan";
      unfolding_covinput += "TauScan";
      unfolding_covmatrix += "TauScan";
      unfolding_covtotal += "TauScan";
    }
  }
  else{
    unfolding_input += "CustomTau";
    unfolding_result += "CustomTau";
    unfolding_corr += "CustomTau";
    unfolding_check += "CustomTau";
    unfolding_covinput += "CustomTau";
    unfolding_covmatrix += "CustomTau";
    unfolding_covtotal += "CustomTau";
  }

  l_curve = 0;
  l_curve_check = 0;
  logTauX = 0;
  logTauY = 0;

  if(tau_value < 0){
    if(do_lcurve){
      unfold.ScanLcurve(nscan, 0.000001, 0.9, &l_curve, &logTauX, &logTauY);
      unfold_check.ScanLcurve(1, 0.000001, 0.9, &l_curve_check);
      logTauX->SetName("Spline logTauX LCurve");
      logTauY->SetName("Spline logTauY LCurve");
      l_curve->SetName("LCurve of LCurve");
      l_curve_check->SetName("LCurve Check of LCurve");
    }
    else{
      rhoLogTau = 0;
      rhoLogTau_check = 0;
      const char *SCAN_DISTRIBUTION = 0;
      // const char *SCAN_AXISSTEERING = 0;
      // const char *SCAN_DISTRIBUTION = "measurement_gen";
      const char *SCAN_AXISSTEERING = 0;
      // TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoAvg;
      // TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoMax;
      TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoAvgSys;
      // TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoSquareAvg;
      // TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoSquareAvgSys;
      unfold.ScanTau(nscan, 0.000000000000000001, 0.9, &rhoLogTau, scanMode, SCAN_DISTRIBUTION, SCAN_AXISSTEERING, &l_curve, &logTauX, &logTauY);
      unfold_check.ScanTau(1, 0.000000000000000001, 0.9, &rhoLogTau_check, scanMode, SCAN_DISTRIBUTION, SCAN_AXISSTEERING);
      logTauX->SetName("Spline logTauX TauScan");
      logTauY->SetName("Spline logTauY TauScan");
      l_curve->SetName("LCurve of TauScan");
      rhoLogTau->SetName("RhoLogTau TauScan");
      rhoLogTau_check->SetName("RhoLogTau Check TauScan");
    }
  }
  else{
    unfold.DoUnfold(tau_value);
    unfold_check.DoUnfold(tau_value);
  }
  h_data_output     = unfold.GetOutput(unfolding_result, 0, "measurement_gen", "mass[C]", kTRUE);
  h_data_output_all = unfold.GetOutput(unfolding_result+"_all", 0, 0,0,kFALSE);
  h_data_rho        = unfold.GetRhoIJtotal(unfolding_corr, 0, "measurement_gen", "mass[C]", kTRUE);
  h_data_rho_all    = unfold.GetRhoIJtotal(unfolding_corr+"_all", 0, 0,0,kFALSE);
  h_check           = unfold_check.GetOutput(unfolding_check, 0, "measurement_gen", "mass[C]", kTRUE);
  h_check_all       = unfold_check.GetOutput(unfolding_check+"_all", 0, 0,0,kFALSE);

  // Statistical uncertainties of input distribution
  h_covarianceinputstat     = unfold.GetEmatrixInput(unfolding_covinput, 0, "measurement_gen", "mass[C]", kTRUE);
  h_covarianceinputstat_all = unfold.GetEmatrixInput(unfolding_covinput+"_all", 0, 0,0,kFALSE);

  // Statistical uncertainties of matrix
  h_covartiancematrixstat     = unfold.GetEmatrixSysUncorr(unfolding_covmatrix, 0, "measurement_gen", "mass[C]", kTRUE);
  h_covartiancematrixstat_all = unfold.GetEmatrixSysUncorr(unfolding_covmatrix+"_all", 0, 0,0,kFALSE);

  h_covariancetotal     = unfold.GetEmatrixTotal(unfolding_covtotal, 0, "measurement_gen", "mass[C]", kTRUE);
  h_covariancetotal_all = unfold.GetEmatrixTotal(unfolding_covtotal+"_all", 0, 0, 0, kFALSE);

  if(tau_value < 0){
    // logTau.clear();
    logTau.push_back(logTauX);
    logTau.push_back(logTauY);

    tau_result = unfold.GetTau();

    tau.SetUniqueID(tau_result);
    if(do_lcurve) tau.SetName("Tauvalue of LCurve");
    else          tau.SetName("Tauvalue of TauScan");

    double logTaud = TMath::Log10(tau_result);
    cout << "logtau = " << logTaud << '\n';

    x_value.SetUniqueID(logTauX->Eval(logTaud));
    y_value.SetUniqueID(logTauY->Eval(logTaud));
    if(do_lcurve){
      x_value.SetName("Coordinate X LCurve");
      y_value.SetName("Coordinate Y LCurve");
    }
    else{
      x_value.SetName("Coordinate X TauScan");
      y_value.SetName("Coordinate Y TauScan");
    }
  }


  if(subtract_background){
    for(unsigned int i=0; i<background_names.size(); i++){
      // Statistical uncertainties from Background
      CovBgrStat.push_back(unfold.GetEmatrixSysBackgroundUncorr(background_names.at(i), "", 0, "measurement_gen", "mass[C]", kTRUE));
      if(do_lcurve){
        TString temp_name = "Covariance of " + background_names.at(i) + " LCurve";
        CovBgrStat[i]->SetName(temp_name);
      }
      else{
        TString temp_name = "Covariance of " + background_names.at(i) + " TauScan";
        CovBgrStat[i]->SetName(temp_name);
      }
      // Sys uncertainties from background scale (delta)
      BgrDelta.push_back(unfold.GetDeltaSysBackgroundScale(background_names.at(i), "", 0, "measurement_gen", "mass[C]",kTRUE));
      if(do_lcurve){
        TString temp_name = "Delta of " + background_names.at(i) + " LCurve";
        BgrDelta[i]->SetName(temp_name);
      }
      else{
        TString temp_name = "Delta of " + background_names.at(i) + " TauScan";
        BgrDelta[i]->SetName(temp_name);
      }
    }
    for(unsigned int i=0; i<BgrDelta.size(); i++){
      // Sys uncertainties from background scale (cov matrix)
      CovBgrScale.push_back(CreateCovMatrixFromDelta(BgrDelta[i]));
      if(do_lcurve){
        TString temp_name = "Scale of " + background_names.at(i) + " LCurve";
        CovBgrScale[i]->SetName(temp_name);
      }
      else{
        TString temp_name = "Scale of " + background_names.at(i) + " TauScan";
        CovBgrScale[i]->SetName(temp_name);
      }
    }
  }

  // treat sys uncertainties
  for(unsigned int i = 0; i < sys_name.size(); i++){
    vector<TH1*> dummy;
    sys_delta.push_back(dummy);
    for(unsigned int j = 0; j < sys_name[i].size(); j++){
      unfold.AddSysError(sys_matrix[i][j], sys_name[i][j], TUnfold::kHistMapOutputHoriz, TUnfoldDensity::kSysErrModeMatrix);
      sys_delta[i].push_back(unfold.GetDeltaSysSource(sys_name[i][j], "",0,"measurement_gen","mass[C]",kTRUE));
      if(do_lcurve){
        TString temp_name = "Delta of " + sys_name[i][j] + " LCurve";
        sys_delta[i][j]->SetName(temp_name);
      }
      else{
        TString temp_name = "Delta of " + sys_name[i][j] + " TauScan";
        sys_delta[i][j]->SetName(temp_name);
      }
    }
  }
  for(unsigned int i=0; i<sys_delta.size(); i++){
    vector<TH2*> dummy2;
    sys_covariance.push_back(dummy2);
    for(unsigned int j=0; j<sys_delta[i].size(); j++){
      sys_covariance[i].push_back(CreateCovMatrixFromDelta(sys_delta[i][j]));
      TString temp_name = "Covariance of "+ sys_name[i][j] + "TauScan";
      sys_covariance[i][j]->SetName(temp_name);
    }
  }

  return;
}

void unfolding::get_output_check(){
  h_check->Write();
  h_check->Clear();
  return ;
}

void unfolding::get_output_check_all(){
  h_check_all->Write();
  h_check_all->Clear();
  return ;
}

void unfolding::get_output(){
  h_data_output->Write();
  h_data_output->Clear();
  return ;
}

void unfolding::get_output_all(){
  h_data_output_all->Write();
  h_data_output_all->Clear();
  return ;
}

void unfolding::get_input_statcov(){
  h_covarianceinputstat->Write();
  h_covarianceinputstat->Clear();
  return ;
}

void unfolding::get_input_statcov_all(){
  h_covarianceinputstat_all->Write();
  h_covarianceinputstat_all->Clear();
  return ;
}

void unfolding::get_matrix_statcov(){
  h_covartiancematrixstat->Write();
  h_covartiancematrixstat->Clear();
  return ;
}

void unfolding::get_matrix_statcov_all(){
  h_covartiancematrixstat_all->Write();
  h_covartiancematrixstat_all->Clear();
  return ;
}

void unfolding::get_total_statcov(){
  h_covariancetotal->Write();
  h_covariancetotal->Clear();
  return ;
}

void unfolding::get_total_statcov_all(){
  h_covariancetotal_all->Write();
  h_covariancetotal_all->Clear();
  return ;
}

void unfolding::get_lcurve(){
  l_curve->Write();
  l_curve->Clear();
  return ;
}

void unfolding::get_coordinates(){
  x_value.Write();
  y_value.Write();
  x_value.Clear();
  y_value.Clear();
  return ;
}

void unfolding::get_logtau(){
  logTau.at(0)->Write();
  logTau.at(1)->Write();
  logTau.clear();
  return ;
}

void unfolding::get_tau(){
  tau.Write();
  tau.Clear();
  return ;
}

void unfolding::get_rhologtau(){
  rhoLogTau->Write();
  rhoLogTau->Clear();
  return ;
}

void unfolding::get_correlation(){
  h_data_rho->Write();
  h_data_rho->Clear();
  return ;
}

void unfolding::get_correlation_all(){
  h_data_rho_all->Write();
  h_data_rho_all->Clear();
  return ;
}

void unfolding::check_projection(){

  std::cout << "starting to check projections \n";

  // TH1D* hist_data_rec = (TH1D*)hist_truth->Clone("hist_data_rec");
  // TRandom3 rnd;
  // for(int iRec=1; iRec <= hist_data_rec->GetSize(); iRec++) {
  //   double c0 = hist_data_rec->GetBinContent(iRec);
  //   double e0 = hist_data_rec->GetBinError(iRec);
  //   double n0 = c0*c0/(e0*e0);
  //   double a = e0*e0/c0;
  //   double ni = rnd.Poisson(n0);
  //   double ci = a*ni;
  //   double ei = a*TMath::Sqrt(ni);
  //   hist_data_rec->SetBinContent(iRec, ci);
  //   hist_data_rec->SetBinError(iRec, ei);
  // }
  TString title_proj_y = "Projection of rec distribution ";
  TString title_proj_x = "Projection of gen distribution ";
  title_proj_y += response_matrix->GetTitle();
  title_proj_x += response_matrix->GetTitle();

  TH1D* projection_y = new TH1D(title_proj_y, "", response_matrix->GetNbinsY(), 0.5, response_matrix->GetNbinsY()+0.5);
  TH1D* projection_x = new TH1D(title_proj_x, "", response_matrix->GetNbinsX(), 0.5, response_matrix->GetNbinsX()+0.5);

  for(int i = 0; i <= response_matrix->GetNbinsY() + 1; i++){
    double content_y = 0;
    for(int j = 0; j <= response_matrix->GetNbinsX() + 1; j++){
      content_y += response_matrix->GetBinContent(j, i);
    }
    projection_y->SetBinContent(i, content_y);
  }
  for(int i = 0; i <= response_matrix->GetNbinsX() + 1; i++){
    double content_x = 0;
    for(int j = 0; j <= response_matrix->GetNbinsY() + 1; j++){
      content_x += response_matrix->GetBinContent(i, j);
    }
    projection_x->SetBinContent(i, content_x);
  }
  projection_x->Write();
  projection_y->Write();
  projection_x->Clear();
  projection_y->Clear();
  std::cout << "Finished. \n";
  std::cout << '\n';
  return ;
}

void unfolding::get_sys_covariance(){
  for(unsigned int i = 0; i < sys_covariance.size(); i++){
    for(unsigned int j = 0; j < sys_covariance[i].size(); j++){
      sys_covariance[i][j]->Write();
      sys_covariance[i][j]->Clear();
    }
  }
  return ;
}

void unfolding::get_sys_delta(){
  for(unsigned int i = 0; i < sys_delta.size(); i++){
    for(unsigned int j = 0; j < sys_delta[i].size(); j++){
      sys_delta[i][j]->Write();
      sys_delta[i][j]->Clear();
    }
  }
  return ;
}

void unfolding::GetBgrStatCov(){
  for(unsigned int i = 0; i < CovBgrStat.size(); i++){
    CovBgrStat[i]->Write();
    CovBgrStat[i]->Clear();
  }
  return ;
}

void unfolding::GetBgrScaleCov(){
  for(unsigned int i = 0; i < CovBgrScale.size(); i++){
    CovBgrScale[i]->Write();
    CovBgrScale[i]->Clear();
  }
  return ;
}

void unfolding::get_bgr_delta(){
  for(unsigned int i = 0; i < BgrDelta.size(); i++){
    BgrDelta[i]->Write();
    BgrDelta[i]->Clear();
  }
  return ;
}

TH2* unfolding::CreateCovMatrixFromDelta(TH1* delta){
  TH2* cov = (TH2*) h_covarianceinputstat->Clone();
  cov->Reset();
  int nbins = delta->GetXaxis()->GetNbins();
  for(int i=1; i <= nbins; i++){
    for(int j=1; j <= nbins; j++){
      double covariance = delta->GetBinContent(i) * delta->GetBinContent(j);
      cov->SetBinContent(i, j, covariance);
    }
  }
  return cov;
}
