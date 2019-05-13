#include "Plots.h"

using namespace std;

Plotter::Plotter(TString directory){
  save_dir_ = directory;
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
  // gStyle->SetPadTopMargin(0.04);
  gStyle->SetLegendBorderSize(0);
}


void Plotter::Plot_projections(TH1D* projection, TH1* h_input, TString save){
  TH1D* projection_clone = (TH1D*) projection->Clone("projection_clone");
  TH1* h_input_clone = (TH1*) h_input->Clone("h_input_clone");
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  if(save.Contains("Projection_X")){
    if(save.Contains("pseudo1")) name = " of pseudo1";
    else if(save.Contains("pseudo2")) name = " of pseudo2";
    else if(save.Contains("pseudo3")) name = " of pseudo3";
    else if(save.Contains("data")) name = " of data";
    name += " X";
    x_axis_title = "gen_binning";
  }
  else if(save.Contains("Projection_Y")){
    if(save.Contains("pseudo1")) name = " of pseudo1";
    else if(save.Contains("pseudo2")) name = " of pseudo2";
    else if(save.Contains("pseudo3")) name = " of pseudo3";
    else if(save.Contains("data")) name = " of data";
    name += " Y";
    x_axis_title = "rec_binning";
    projection_clone->GetYaxis()->SetRangeUser(0, 120);
  }
  TCanvas* c_projection = new TCanvas("Projection" + name, "Projection", 1000, 800);
  c_projection->cd();
  projection_clone->GetYaxis()->SetTitleSize(0.06);
  projection_clone->GetYaxis()->SetTitleOffset(0.9);
  projection_clone->GetXaxis()->SetTitleSize(0.06);
  projection_clone->GetXaxis()->SetTitleOffset(0.7);
  projection_clone->GetYaxis()->SetTitle("events");
  projection_clone->GetXaxis()->SetTitle(x_axis_title);
  projection_clone->DrawCopy();
  h_input_clone->SetLineColor(kRed);
  h_input_clone->SetMarkerColor(kRed);
  h_input_clone->DrawCopy("same");
  c_projection->SaveAs(save_dir_ + save);

  c_projection->Write();
  delete c_projection;
}

void Plotter::Plot_output(TH1* h_unfolded, TH1* h_truth_, bool normalise, TString save){
  TH1* h_unfolded_clone = (TH1*) h_unfolded->Clone("h_unfolded_clone");
  TH1* h_truth_clone = (TH1*) h_truth_->Clone("h_truth_clone");

  if(save.Contains("pseudo1")){
    name = " of pseudo1";
    dataset = "pseudo1";
  }
  else if(save.Contains("pseudo2")){
    name = " of pseudo2";
    dataset = "pseudo2";
  }
  else if(save.Contains("pseudo3")){
    name = " of pseudo3";
    dataset = "pseudo3";
  }
  else if(save.Contains("data")){
    name = " of data";
    dataset = "data";
  }
  if(save.Contains("norm")) name += " nomalised";
  if(save.Contains("LCurve")) name += " (LCurve";
  else if(save.Contains("TauScan")) name += " (TauScan";
  else if(save.Contains("CustomTau")) name += " (CustomTau";
  if(save.Contains("crosscheck")) name += " check)";
  else name += ")";

  TCanvas* c_unfolding = new TCanvas("Unfolding" + name, "Unfolded data", 1000, 800);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_unfolding->cd();
  if(!normalise){
    h_unfolded_clone->SetLineColor(kRed);
    h_unfolded_clone->SetMarkerColor(kRed);
    h_unfolded_clone->GetYaxis()->SetTitleSize(0.06);
    h_unfolded_clone->GetYaxis()->SetTitleOffset(0.9);
    h_unfolded_clone->GetYaxis()->SetTitle("events");
    h_unfolded_clone->GetXaxis()->SetTitleSize(0.06);
    h_unfolded_clone->GetXaxis()->SetTitleOffset(0.7);
    h_unfolded_clone->SetTitle("");
    h_unfolded_clone->DrawCopy();
    h_truth_clone->DrawCopy("same hist");
    TLegend *legend1;
    if(save.Contains("all")) legend1 = new TLegend(0.35, 0.62, 0.75, 0.82, "");
    else legend1 = new TLegend(0.15, 0.22, 0.55, 0.42, "");
    legend1->SetBorderSize(0);
    legend1->SetFillStyle(0);
    if(save.Contains("crosscheck")) legend1->AddEntry(h_unfolded_clone, "unfolded mc","l");
    else legend1->AddEntry(h_unfolded_clone, "unfolded " + dataset,"l");
    legend1->AddEntry(h_truth_clone, "truth", "l");
    legend1->Draw();
  }
  else{
    TH1 *unfolded_data_norm = (TH1*) h_unfolded_clone->Clone("unfolded_data_norm");
    TH1 *h_truth_norm = (TH1*) h_truth_clone->Clone("h_truth_norm");

    for(int i = 1; i <= unfolded_data_norm->GetNbinsX(); i++){
      double content = unfolded_data_norm->GetBinContent(i);
      double error_bin = unfolded_data_norm->GetBinError(i);
      double bin_width = unfolded_data_norm->GetBinWidth(i);
      double new_content = content/bin_width;
      double new_error = error_bin/bin_width;

      double content_truth = h_truth_norm->GetBinContent(i);
      double error_truth = h_truth_norm->GetBinError(i);
      double bin_width_truth = h_truth_norm->GetBinWidth(i);
      double new_content_truth = content_truth/bin_width_truth;
      double new_error_truth = error_truth/bin_width_truth;

      h_truth_norm->SetBinContent(i, new_content_truth);
      h_truth_norm->SetBinError(i, new_error_truth);
      unfolded_data_norm->SetBinContent(i, new_content);
      unfolded_data_norm->SetBinError(i, new_error);
  }

    unfolded_data_norm->SetLineColor(kRed);
    unfolded_data_norm->SetMarkerColor(kRed);
    unfolded_data_norm->GetYaxis()->SetTitleSize(0.06);
    unfolded_data_norm->GetYaxis()->SetTitleOffset(0.95);
    unfolded_data_norm->GetXaxis()->SetTitleSize(0.06);
    unfolded_data_norm->GetXaxis()->SetTitleOffset(0.7);
    unfolded_data_norm->GetYaxis()->SetTitle("events/binwidth");
    unfolded_data_norm->SetTitle("");
    unfolded_data_norm->DrawCopy();
    h_truth_norm->DrawCopy("same hist");
    TLegend *legend_unfolding_norm;
    if(save.Contains("all")) legend_unfolding_norm = new TLegend(0.35, 0.62, 0.75, 0.82, "");
    else if(save.Contains("norm"))  legend_unfolding_norm = new TLegend(0.15, 0.72, 0.45, 0.88, "");
    else legend_unfolding_norm = new TLegend(0.15, 0.22, 0.65, 0.42, "");
    legend_unfolding_norm->SetBorderSize(0);
    legend_unfolding_norm->SetFillStyle(0);
    if(save.Contains("crosscheck")) legend_unfolding_norm->AddEntry(unfolded_data_norm, "unfolded mc","l");
    else legend_unfolding_norm->AddEntry(unfolded_data_norm, "unfolded " + dataset,"l");
    legend_unfolding_norm->AddEntry(h_truth_norm, "truth", "l");
    legend_unfolding_norm->Draw();
  }
  c_unfolding->SaveAs(save_dir_ + save);
  c_unfolding->Write();
  delete c_unfolding;
}

void Plotter::Plot_correlation_matrix(TH2* h_corr_matrix, TString save){
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.12);
  TH2* h_corr_matrix_clone = (TH2*) h_corr_matrix->Clone("h_corr_matrix_clone");

  if(save.Contains("LCurve")) name = " (LCurve)";
  else if(save.Contains("TauScan")) name = " (TauScan)";
  else if(save.Contains("CustomTau")) name = " (CustomTau)";
  TCanvas* c_corr_matrix = new TCanvas("Correlation Matrix" + name, "Correlation Matrix", 1000, 800);
  c_corr_matrix->cd();
  h_corr_matrix_clone->GetYaxis()->SetTitleSize(0.06);
  h_corr_matrix_clone->GetYaxis()->SetTitleOffset(0.7);
  h_corr_matrix_clone->GetXaxis()->SetTitleSize(0.06);
  h_corr_matrix_clone->GetXaxis()->SetTitleOffset(0.7);
  if(save.Contains("all"))h_corr_matrix_clone->GetYaxis()->SetTitle("binning_gen");
  else h_corr_matrix_clone->GetYaxis()->SetTitle("#tau_{3/2}");
  h_corr_matrix_clone->Draw("colz");
  c_corr_matrix->SaveAs(save_dir_ + save);
  c_corr_matrix->Write();
  delete c_corr_matrix;
}

void Plotter::Plot_LogTau(TSpline* logTau_, double tau, double coordinate, TString save){
  cout << "Plots.c: TString save: " << save << '\n';
  if(save.Contains("TauX")){
    name = "X";
    y_axis_title = "L_{X}";
  }
  else if(save.Contains("TauY")){
    name = "Y";
    y_axis_title = "L_{Y}";
  }
  if(save.Contains("pseudo1")){
    name += " of pseudo1";
    dataset = "pseudo1";
  }
  else if(save.Contains("pseudo2")){
    name += " of pseudo2";
    dataset = "pseudo2";
  }
  else if(save.Contains("pseudo3")){
    name += " of pseudo3";
    dataset = "pseudo3";
  }
  else if(save.Contains("data")){
    name += " of data";
    dataset = "data";
  }

  TCanvas* c_lcurve = new TCanvas("LogTau" + name, "LCurve of unfolding", 1000, 800);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_lcurve->cd();
  double logtau = TMath::Log10(tau);
  std::vector<double> Xpoint = {logtau};
  std::vector<double> Ypoint = {coordinate};
  TGraph * point = new TGraph(1, &Xpoint[0], &Ypoint[0]);
  point->SetTitle("");
  point->GetYaxis()->SetTitleSize(0.06);
  point->GetYaxis()->SetTitleOffset(0.7);
  point->GetXaxis()->SetTitleSize(0.06);
  point->GetXaxis()->SetTitleOffset(0.7);
  point->GetXaxis()->SetTitle("log #tau");
  point->GetYaxis()->SetTitle(y_axis_title);
  // point->GetYaxis()->SetTitleOffset(1.5);
  point->GetXaxis()->SetNdivisions(510);
  point->GetYaxis()->SetNdivisions(810);
  point->SetMarkerStyle(20);
  point->SetMarkerSize(1.5);
  point->SetLineColor(1);
  point->Draw("AP");
  point->GetXaxis()->SetLimits(-6.0, 0.0);
  if(save.Contains("TauY")) point->GetYaxis()->SetRangeUser(-3, 8);
  // double ymin = Ypoint[0]*0.99;
  // double ymax = Ypoint[0]*1.08;
  // point->GetYaxis()->SetRangeUser(ymin, ymax);
  point->DrawClone("AP");
  c_lcurve->Update();
  logTau_->SetLineColor(kRed);
  logTau_->DrawClone("same");
  point->DrawClone("psame");
  TLegend *legend_logtau = new TLegend(0.15, 0.42, 0.65, 0.62, "");
  legend_logtau->SetBorderSize(0);
  legend_logtau->SetFillStyle(0);
  legend_logtau->AddEntry(logTau_, "unfolded " + dataset, "l");
  legend_logtau->AddEntry(point, "log(#tau) value", "p");
  legend_logtau->Draw();
  c_lcurve->SaveAs(save_dir_ + save);
  c_lcurve->Write();
  delete c_lcurve;
}

void Plotter::Plot_LCurve(TGraph* lcurve, std::vector<double> coordinates, std::vector<double> coordinates_2, TString save){

  if(save.Contains("pseudo1")){
    name = " of pseudo1";
    dataset = "pseudo1";
  }
  else if(save.Contains("pseudo2")){
    name = " of pseudo2";
    dataset = "pseudo2";
  }
  else if(save.Contains("pseudo3")){
    name = " of pseudo3";
    dataset = "pseudo3";
  }
  else if(save.Contains("data")){
    name = " of data";
    dataset = "data";
  }
  TCanvas* c_lcurve = new TCanvas("LCurve "+ name, "LCurve of Unfolded data (TauScan)", 1000, 800);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_lcurve->cd();
  TMarker* LCurve_tau = new TMarker(coordinates.at(0), coordinates.at(1), 20);
  LCurve_tau->SetMarkerColor(kRed);
  TMarker* TauScan_tau = new TMarker(coordinates_2.at(0), coordinates_2.at(1), 20);
  TauScan_tau->SetMarkerColor(kBlue);
  lcurve->GetYaxis()->SetTitleSize(0.06);
  lcurve->GetYaxis()->SetTitleOffset(0.6);
  lcurve->GetYaxis()->SetTitle("L_{Y}");
  lcurve->GetXaxis()->SetTitleSize(0.06);
  lcurve->GetXaxis()->SetTitleOffset(0.7);
  lcurve->GetXaxis()->SetTitle("L_{X}");
  lcurve->DrawClone();
  LCurve_tau->DrawClone("same");
  TauScan_tau->DrawClone("same");
  TLegend *legend_lcurve = new TLegend(0.15, 0.42, 0.65, 0.62, "");
  legend_lcurve->SetBorderSize(0);
  legend_lcurve->SetFillStyle(0);
  legend_lcurve->AddEntry(lcurve, "LCurve", "l");
  legend_lcurve->AddEntry(LCurve_tau, "#tau value of LCurve", "p");
  legend_lcurve->AddEntry(TauScan_tau, "#tau value of ScanTau", "p");
  legend_lcurve->Draw();
  c_lcurve->SaveAs(save_dir_ + save);
  c_lcurve->Write();
  delete c_lcurve;
}

void Plotter::Plot_RhoLogTau(TSpline* rhologTau, double tau, TString save){

  TCanvas* c_rhologtau = new TCanvas("RhoLogTau ", "LCurve2 of Unfolded data (TauScan)", 1000, 800);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_rhologtau->cd();
  std::vector<double> Xpoint_rhologtau = {TMath::Log10(tau)};
  std::vector<double> Ypoint_rhologtau = {rhologTau->Eval(TMath::Log10(tau))};
  // cout << "YPoint: " << Ypoint_rhologtau[0] << '\n';
  TGraph * point_rhologtau = new TGraph(1, &Xpoint_rhologtau[0], &Ypoint_rhologtau[0]);
  point_rhologtau->SetTitle("");
  point_rhologtau->GetXaxis()->SetTitleSize(0.06);
  point_rhologtau->GetXaxis()->SetTitleOffset(0.7);
  point_rhologtau->GetYaxis()->SetTitleSize(0.06);
  point_rhologtau->GetYaxis()->SetTitleOffset(0.7);
  point_rhologtau->GetXaxis()->SetTitle("log #tau");
  point_rhologtau->GetYaxis()->SetTitle("#rho(log #tau)");
  // point_rhologtau->GetYaxis()->SetTitleOffset(1.5);
  point_rhologtau->GetXaxis()->SetNdivisions(510);
  point_rhologtau->GetYaxis()->SetNdivisions(810);
  point_rhologtau->SetMarkerStyle(20);
  point_rhologtau->SetMarkerSize(1.5);
  point_rhologtau->SetLineColor(1);
  point_rhologtau->Draw("AP");
  point_rhologtau->GetXaxis()->SetLimits(-6.0, 0.0);
  // double ymin_rhologtau = Ypoint_rhologtau[0]*0.95;
  // double ymax_rhologtau = 1.15;
  // point_rhologtau->GetYaxis()->SetRangeUser(ymin_rhologtau, ymax_rhologtau);
  point_rhologtau->DrawClone("AP");
  c_rhologtau->Update();
  rhologTau->SetLineColor(kRed);
  rhologTau->DrawClone("same");
  point_rhologtau->DrawClone("psame");
  TLegend *legend_rhologTau = new TLegend(0.15, 0.20, 0.55, 0.35, "");
  legend_rhologTau->SetBorderSize(0);
  legend_rhologTau->SetFillStyle(0);
  legend_rhologTau->AddEntry(rhologTau, "unfolded " + dataset, "l");
  legend_rhologTau->Draw();
  c_rhologtau->SaveAs(save_dir_ + save);
  c_rhologtau->Write();
  delete c_rhologtau;
}

void Plotter::Plot_ResponseMatrix(TH2* resp_matrix_, TString save){

  TCanvas* c_response = new TCanvas("Response Matrix", "", 1000, 800);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.12);
  c_response->cd();
  resp_matrix_->SetTitle("");
  resp_matrix_->GetYaxis()->SetTitleSize(0.06);
  resp_matrix_->GetYaxis()->SetTitleOffset(0.7);
  resp_matrix_->GetXaxis()->SetTitleSize(0.06);
  resp_matrix_->GetXaxis()->SetTitleOffset(0.7);
  resp_matrix_->DrawCopy("colz");
  c_response->SaveAs(save_dir_ + save);
  c_response->Write();
  delete c_response;
}
void Plotter::Plot_purity(TH1* purity_same, TH1* purity_all, TString save){
  TH1* purity_ratio = (TH1*) purity_same->Clone("purity_ratio");
  if(save.Contains("pseudo1")){
    name = " of pseudo1";
    dataset = "pseudo1";
  }
  else if(save.Contains("pseudo2")){
    name = " of pseudo2";
    dataset = "pseudo2";
  }
  else if(save.Contains("pseudo3")){
    name = " of pseudo3";
    dataset = "pseudo3";
  }
  else if(save.Contains("data")){
    name = " of data";
    dataset = "data";
  }
  TCanvas* c_purity = new TCanvas("Purity" + name, "Purity", 1000, 800);
  c_purity->cd();
  purity_ratio->Divide(purity_all);
  purity_ratio->DrawCopy();
  c_purity->SaveAs(save_dir_ + save);
  c_purity->Write();
  delete c_purity;
}
