#include "unfold.h"

using namespace std;

unfolding::unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response, TH1D* h_truth, TH1D* h_test, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<TH1D*> background, std::vector<TString> background_names, int nscan, TString regmode_, TString density_flag, bool do_lcurve, bool subtract_background){
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
  double sf_ratio = (h_data->Integral()-background_integral)/h_test->Integral();
  cout << "ratio between data and mc (ttbar): " << sf_ratio << '\n';
  unfold.SetInput(h_data, sf_ratio);
  unfold_check.SetInput(h_mc, 1);

  if(subtract_background){
    for(unsigned int i = 0; i < background.size(); i++){
      unfold.SubtractBackground(background.at(i), background_names.at(i));
      cout << "name of background: " << background_names.at(i) << '\n';
    }
  }

  TString unfolding_result = "Unfolded Pseudodata ";
  TString unfolding_corr = "Correlation Matrix ";
  TString unfolding_check = "Unfolded mc ";
  if(do_lcurve){
    unfolding_result += "LCurve";
    unfolding_corr += "LCurve";
    unfolding_check += "LCurve";
  }
  else{
    unfolding_result += "TauScan";
    unfolding_corr += "TauScan";
    unfolding_check += "TauScan";
  }

  if(do_lcurve){
    TSpline *logTauX = 0, *logTauY = 0;
    TGraph *lcurve_check = 0;
    TSpline *logTauX_check = 0, *logTauY_check = 0;
    unfold.ScanLcurve(nscan, 0.000001, 0.9, &l_curve, &logTauX, &logTauY);
    unfold_check.ScanLcurve(1, 0.000001, 0.9, &lcurve_check, &logTauX_check, &logTauY_check);

    h_data_output = unfold.GetOutput(unfolding_result);
    h_data_rho = unfold.GetRhoIJtotal(unfolding_corr);
    h_check = unfold_check.GetOutput(unfolding_check);

    logTau.push_back(logTauX);
    logTau.push_back(logTauY);

    tau = unfold.GetTau();
    double logTaud = TMath::Log10(tau);
    // double lcurveX = logTauX->Eval(logTau);
    // double lcurveY = logTauY->Eval(logTau);
    coords.push_back(logTauX->Eval(logTaud));
    coords.push_back(logTauY->Eval(logTaud));
  }
  else{
    TSpline *rhoLogTau_test = 0;
    TGraph *lcurve_check = 0;
    TSpline *logTauX = 0, *logTauY = 0;
    const char *SCAN_DISTRIBUTION = 0;
    const char *SCAN_AXISSTEERING = 0;
    TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoAvgSys;
    unfold.ScanTau(nscan , 0.000001, 0.9, &rhoLogTau, scanMode, SCAN_DISTRIBUTION, SCAN_AXISSTEERING, &l_curve, &logTauX, &logTauY);
    int iBest_check = unfold_check.ScanTau(nscan , 0.000001, 0.9, &rhoLogTau_test, scanMode, SCAN_DISTRIBUTION, SCAN_AXISSTEERING, &lcurve_check);

    logTau.push_back(logTauX);
    logTau.push_back(logTauY);

    tau = unfold.GetTau();
    double logTaud = TMath::Log10(tau);
    coords.push_back(logTauX->Eval(logTaud));
    coords.push_back(logTauY->Eval(logTaud));

    h_data_output = unfold.GetOutput(unfolding_result);
    h_data_rho = unfold.GetRhoIJtotal(unfolding_corr);
    h_check = unfold_check.GetOutput(unfolding_check);
  }
  return;
}


TH1* unfolding::get_output_check(){
  return h_check;
}


TH1* unfolding::get_output(){
  return h_data_output;
}

TGraph* unfolding::get_lcurve(){
  return l_curve;
}

std::vector<double> unfolding::get_coordinates(){
  return coords;
}

std::vector<TSpline*> unfolding::get_logtau(){
  return logTau;
}

double unfolding::get_tau(){
  return tau;
}

TSpline* unfolding::get_rhologtau(){
  return rhoLogTau;
}

TH2* unfolding::get_correlation(){
  return h_data_rho;
}


std::vector<TH1D*> unfolding::check_projection(){
  std::cout << "starting to check projections \n";

  TH1D* hist_data_rec = (TH1D*)hist_truth->Clone("hist_data_rec");
  TRandom3 rnd;
  for(int iRec=1; iRec <= hist_data_rec->GetSize(); iRec++) {
    double c0 = hist_data_rec->GetBinContent(iRec);
    double e0 = hist_data_rec->GetBinError(iRec);
    double n0 = c0*c0/(e0*e0);
    double a = e0*e0/c0;
    double ni = rnd.Poisson(n0);
    double ci = a*ni;
    double ei = a*TMath::Sqrt(ni);
    hist_data_rec->SetBinContent(iRec, ci);
    hist_data_rec->SetBinError(iRec, ei);
  }
  TString title_proj_y = "Projection of rec distribution ";
  TString title_proj_x = "Projection of gen distribution ";
  title_proj_y += response_matrix->GetTitle();
  title_proj_x += response_matrix->GetTitle();

  m_1 = (TH1D*)gROOT->FindObject(title_proj_y);
  m_2 = (TH1D*)gROOT->FindObject(title_proj_x);
  if(m_1 || m_2){
    m_1->Delete();
    m_2->Delete();
  }

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

  vector<TH1D*> projections = {projection_x, projection_y};
  // projection_y->Delete();
  std::cout << "Finished. \n";
  return projections;
}
