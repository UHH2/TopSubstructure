#include "unfold.h"

using namespace std;

unfolding::unfolding(TH1D* h_data, TH1D* h_mc, TH2D* response , TH1D* h_truth, TH1D* h_test, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec, std::vector<TH1D*> background, std::vector<TString> background_names, bool subtract_background){
  response_matrix = response;
  hist_truth = h_truth;
 // preserve the area
  TUnfold::EConstraint constraintMode = TUnfold::kEConstraintNone;
  // basic choice of regularisation scheme:
  //    curvature (second derivative)
  TUnfold::ERegMode regMode = TUnfold::kRegModeCurvature; //Derivative testen
  // density flags
  TUnfoldDensity::EDensityMode densityFlags = TUnfoldDensity::kDensityModeNone;
  // detailed steering for regularisation
  const char *REGULARISATION_DISTRIBUTION = 0;
  // const char *REGULARISATION_AXISSTEERING = "*[C]";
  const char *REGULARISATION_AXISSTEERING = 0;
  // set up matrix of migrations
  TUnfoldDensity unfold(response_matrix, TUnfold::kHistMapOutputHoriz, regMode, constraintMode, densityFlags, binning_gen, binning_rec, REGULARISATION_DISTRIBUTION, REGULARISATION_AXISSTEERING);
  TUnfoldDensity unfold2(response_matrix, TUnfold::kHistMapOutputHoriz, regMode, constraintMode, densityFlags, binning_gen, binning_rec, REGULARISATION_DISTRIBUTION, REGULARISATION_AXISSTEERING);

  double background_integral = 0;
  if(subtract_background){
    for(unsigned int i = 0; i < background.size(); i++){
      background_integral += background.at(i)->Integral();
    }
  }
  double sf_ratio = (h_data->Integral()-background_integral)/h_test->Integral();
  cout << "ratio between data and mc (ttbar): " << sf_ratio << '\n';
  unfold.SetInput(h_data, sf_ratio);
  unfold2.SetInput(h_data, sf_ratio);

  // double nbin_gen = h_mc->GetSize() - 2;
  // double nbin_rec = h_data->GetSize() - 2;

  TString string = "Unfolding ";
  TGraph *lcurve = 0;
  TSpline *logTauX = 0, *logTauY = 0;
  // h_data_LCURVE   = binning_gen_test->CreateHistogram("h_data_LCURVE", kTRUE, 0, 0);
  // TH2 *h_data_LCURVErho = new TH2D("h_data_LCURVErho", ";r_{LCURVE};r_{LCURVE}", nbin_gen, 0, 1, nbin_gen, 0, 1 );

  if(subtract_background){
    for(unsigned int i = 0; i < background.size(); i++){
      unfold.SubtractBackground(background.at(i), background_names.at(i));
      unfold2.SubtractBackground(background.at(i), background_names.at(i));
      cout << "name of background: " << background_names.at(i) << '\n';
    }
  }
  unfold.ScanLcurve(100, 0., 0., &lcurve, &logTauX, &logTauY);

  h_data_LCURVE = unfold.GetOutput("Unfolded data");
  h_data_LCURVErho = unfold.GetRhoIJtotal("Correlation Matrix");

  // double tau = unfold.GetTau();
  // double logTau = TMath::Log10(tau);
  // double lcurveX = logTauX->Eval(logTau);
  // double lcurveY = logTauY->Eval(logTau);

  int nScan = 30;
  TSpline *rhoLogTau = 0;
  TGraph *lCurve = 0;
  const char *SCAN_DISTRIBUTION = 0;
  const char *SCAN_AXISSTEERING = 0;
  TUnfoldDensity::EScanTauMode scanMode = TUnfoldDensity::kEScanTauRhoAvgSys;
  int iBest = unfold2.ScanTau(nScan , 0.000001, 0.9, &rhoLogTau, scanMode, SCAN_DISTRIBUTION, SCAN_AXISSTEERING, &lCurve);

  h_data_tau = unfold2.GetOutput("Unfolded data tau");
  h_data_taurho = unfold2.GetRhoIJtotal("Correlation Matrix tau");

  return;
}


TH1* unfolding::get_output_Lcurve(){
  return h_data_LCURVE;
}


TH2* unfolding::get_correlation_Lcurve(){
  return h_data_LCURVErho;
}


TH1* unfolding::get_output(){
  return h_data_tau;
}


TH2* unfolding::get_correlation(){
  return h_data_taurho;
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

  TH1D* projection_y = new TH1D("Projection of rec distribution", "", response_matrix->GetNbinsY(), 0.5, response_matrix->GetNbinsY()+0.5);
  TH1D* projection_x = new TH1D("Projection of gen distribution", "", response_matrix->GetNbinsX(), 0.5, response_matrix->GetNbinsX()+0.5);

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
  std::cout << "Finished. \n";
  return projections;
}
