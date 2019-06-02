#include "Plots.h"
#include "TVectorD.h"

using namespace std;

Plotter::Plotter(TString file, TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec){
  save_dir_ = file;
  measurement_rec = binning_rec->FindNode("measurement_rec");
  measurement_gen = binning_gen->FindNode("measurement_gen");
  rec_pt_topjet_sideband = binning_rec->FindNode("rec_pt_topjet_sideband");
  gen_pt_topjet_sideband = binning_gen->FindNode("gen_pt_topjet_sideband");
  rec_mass_sideband = binning_rec->FindNode("rec_mass_sideband");
  gen_mass_sideband = binning_gen->FindNode("gen_mass_sideband");

  n_regions = 3;

  const TVectorD *mass_split_rec = measurement_rec->GetDistributionBinning(1);
  const TVectorD *mass_split_gen = measurement_gen->GetDistributionBinning(1);

  n_measurement_rec = measurement_rec->GetTH1xNumberOfBins();
  n_measurement_gen = measurement_gen->GetTH1xNumberOfBins();

  n_mass_split_rec = mass_split_rec->GetNoElements() - 1;
  n_mass_split_gen = mass_split_gen->GetNoElements() - 1;

  n_rec_pt_topjet = rec_pt_topjet_sideband->GetTH1xNumberOfBins();
  n_gen_pt_topjet = gen_pt_topjet_sideband->GetTH1xNumberOfBins();

  n_mass_rec = rec_mass_sideband->GetTH1xNumberOfBins();
  n_mass_gen = gen_mass_sideband->GetTH1xNumberOfBins();

  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
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
  projection_clone->GetYaxis()->SetTitle("Events");
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
  if(save.Contains("norm")) name += " normalised";
  if(save.Contains("LCurve")) name += " (LCurve";
  else if(save.Contains("TauScan")) name += " (TauScan";
  else if(save.Contains("CustomTau")) name += " (CustomTau";
  if(save.Contains("crosscheck")) name += " check)";
  else name += ")";
  if(save.Contains("all")) name += "_all";

  TCanvas* c_unfolding = new TCanvas("Unfolding" + name, "Unfolded data", 1000, 800);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_unfolding->cd();
  if(!normalise){
    h_unfolded_clone->SetMarkerStyle(20);
    h_unfolded_clone->SetLineColor(kRed);
    h_unfolded_clone->SetMarkerColor(kRed);
    h_unfolded_clone->GetYaxis()->SetTitleSize(0.06);
    h_unfolded_clone->GetYaxis()->SetTitleOffset(0.9);
    h_unfolded_clone->GetYaxis()->SetTitle("Events");
    h_unfolded_clone->GetXaxis()->SetTitle("generator binning");
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
    if(save.Contains("crosscheck")) legend1->AddEntry(h_unfolded_clone, "unfolded mc","pl");
    else legend1->AddEntry(h_unfolded_clone, "unfolded pseudodata","pl"); // + dataset
    legend1->AddEntry(h_truth_clone, "truth", "l");
    legend1->Draw();
  }
  else{
    TH1 *unfolded_data_norm = (TH1*) h_unfolded_clone->Clone("unfolded_data_norm");
    TH1 *h_truth_norm = (TH1*) h_truth_clone->Clone("h_truth_norm");
    TH1D* h_relative = new TH1D("h_relative"+name, "", unfolded_data_norm->GetNbinsX(), 0., 1.);

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


      double relative_error = unfolded_data_norm->GetBinError(i)/unfolded_data_norm->GetBinContent(i)*100;
      h_relative->SetBinContent(i, relative_error);
    }

    unfolded_data_norm->SetMarkerStyle(20);
    unfolded_data_norm->SetLineColor(kRed);
    unfolded_data_norm->SetMarkerColor(kRed);
    unfolded_data_norm->GetYaxis()->SetTitleSize(0.06);
    unfolded_data_norm->GetYaxis()->SetTitleOffset(0.95);
    unfolded_data_norm->GetXaxis()->SetTitleSize(0.06);
    unfolded_data_norm->GetXaxis()->SetTitleOffset(0.7);
    unfolded_data_norm->GetYaxis()->SetTitle("Events/Bin width");
    if(save.Contains("all")) unfolded_data_norm->GetXaxis()->SetTitle("generator binning");
    else unfolded_data_norm->GetXaxis()->SetTitle("#tau_{3/2}");

    unfolded_data_norm->SetTitle("");
    if(save.Contains("TauScan/Unfolding_norm_")) unfolded_data_norm->Write("Unfolded Pseudodata TauScan");
    unfolded_data_norm->DrawCopy();
    h_truth_norm->DrawCopy("same hist");
    TLegend *legend_unfolding_norm;
    if(save.Contains("all")) legend_unfolding_norm = new TLegend(0.35, 0.62, 0.75, 0.82, "");
    else if(save.Contains("norm"))  legend_unfolding_norm = new TLegend(0.15, 0.72, 0.45, 0.88, "");
    else legend_unfolding_norm = new TLegend(0.15, 0.22, 0.65, 0.42, "");
    legend_unfolding_norm->SetBorderSize(0);
    legend_unfolding_norm->SetFillStyle(0);
    if(save.Contains("crosscheck")) legend_unfolding_norm->AddEntry(unfolded_data_norm, "unfolded mc","pl");
    else legend_unfolding_norm->AddEntry(unfolded_data_norm, "unfolded pseudodata","pl"); //  + dataset
    legend_unfolding_norm->AddEntry(h_truth_norm, "truth", "l");
    legend_unfolding_norm->Draw();

    TCanvas* c_relative = new TCanvas("relative"+name, "relative", 1000, 800);
    c_relative->cd();
    h_relative->GetYaxis()->SetTitleSize(0.05);
    h_relative->GetYaxis()->SetTitleOffset(0.95);
    h_relative->GetXaxis()->SetTitleSize(0.05);
    h_relative->GetXaxis()->SetTitleOffset(0.7);
    h_relative->GetYaxis()->SetTitle("relative uncertainty [%]");
    h_relative->GetYaxis()->SetRangeUser(0, 45);
    h_relative->GetXaxis()->SetTitle("#tau_{3/2}");
    h_relative->Draw("B");
    TLegend* legend_rel = new TLegend(0.15, 0.42, 0.45, 0.62, "");
    legend_rel->AddEntry(h_relative, "stat Pseudo data 1", "l");
    legend_rel->Draw();
    c_relative->SaveAs(save_dir_ + "TauScan/relative_error_"+name+".eps");
    c_relative->Write();
    delete c_relative;
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
  h_corr_matrix_clone->SetTitle("Correlation Matrix");
  if(save.Contains("all"))h_corr_matrix_clone->GetXaxis()->SetTitle("generator binning");
  else h_corr_matrix_clone->GetXaxis()->SetTitle("#tau_{3/2}");
  h_corr_matrix_clone->GetYaxis()->SetTitleSize(0.06);
  h_corr_matrix_clone->GetYaxis()->SetTitleOffset(0.7);
  h_corr_matrix_clone->GetXaxis()->SetTitleSize(0.06);
  h_corr_matrix_clone->GetXaxis()->SetTitleOffset(0.7);
  if(save.Contains("all"))h_corr_matrix_clone->GetYaxis()->SetTitle("generator binning");
  else h_corr_matrix_clone->GetYaxis()->SetTitle("#tau_{3/2}");
  h_corr_matrix_clone->Draw("colz");
  Double_t ymax = h_corr_matrix_clone->GetNbinsY();
  Double_t xmax = h_corr_matrix_clone->GetNbinsX();
  if(xmax/2 > 1){
    TLine *line = new TLine(n_measurement_gen+0.5,0.5,n_measurement_gen+0.5,ymax+0.5);
    line->SetLineColor(kRed);
    line->Draw("same");
    TLine *line2 = new TLine(0.5,n_gen_pt_topjet+0.5,xmax+0.5,n_gen_pt_topjet+0.5);
    line2->SetLineColor(kRed);
    line2->Draw("same");
    if(xmax/3 >= 2){
      TLine *line3 = new TLine(2*n_gen_pt_topjet+0.5,0.5,2*n_gen_pt_topjet+0.5,ymax+0.5);
      line3->SetLineColor(kRed);
      line3->Draw("same");
      TLine *line4 = new TLine(0.5,2*n_measurement_gen+0.5,xmax+0.5,2*n_measurement_gen+0.5);
      line4->SetLineColor(kRed);
      line4->Draw("same");
    }
  }
  c_corr_matrix->SaveAs(save_dir_ + save);
  c_corr_matrix->Write();
  delete c_corr_matrix;
}

void Plotter::Plot_LogTau(TSpline* logTau_, double tau, double coordinate, TString save){
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
  point->Draw("AP");
  c_lcurve->Update();
  logTau_->SetLineColor(kRed);
  logTau_->Draw("same");
  point->Draw("psame");
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
  legend_rhologTau->AddEntry(rhologTau, "unfolded Pseudo data 1", "l");
  legend_rhologTau->Draw();
  c_rhologtau->SaveAs(save_dir_ + save);
  c_rhologtau->Write();
  delete c_rhologtau;
}

void Plotter::Plot_ResponseMatrix(TH2* resp_matrix_, TString save){

  TCanvas* c_response = new TCanvas("Response Matrix", "", 1000, 800);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.12);
  c_response->SetLogz();
  c_response->cd();
  resp_matrix_->SetTitle("Migration Matrix");
  resp_matrix_->GetYaxis()->SetTitle("detector binning");
  resp_matrix_->GetYaxis()->SetTitleSize(0.045);
  resp_matrix_->GetYaxis()->SetTitleOffset(0.7);
  resp_matrix_->GetXaxis()->SetTitle("generator binning");
  resp_matrix_->GetXaxis()->SetTitleSize(0.045);
  resp_matrix_->GetXaxis()->SetTitleOffset(0.9);
  resp_matrix_->DrawCopy("colz");

  Double_t ymax = resp_matrix_->GetNbinsY();
  Double_t xmax = resp_matrix_->GetNbinsX();
  if(xmax/2 > 1){
    TLine *line = new TLine(n_measurement_gen+0.5, 0.5, n_measurement_gen+0.5, ymax+0.5);
    line->SetLineColor(kRed);
    line->Draw("same");
    TLine *line2 = new TLine(0.5, n_measurement_rec+0.5, xmax+0.5, n_measurement_rec+0.5);
    line2->SetLineColor(kRed);
    line2->Draw("same");
    if(xmax/3 >= 2){
      TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, 0.5, n_measurement_gen+n_gen_pt_topjet+0.5, ymax+0.5);
      line3->SetLineColor(kRed);
      line3->Draw("same");
      TLine *line4 = new TLine(0.5, n_measurement_rec+n_rec_pt_topjet+0.5, xmax+0.5, n_measurement_rec+n_rec_pt_topjet+0.5);
      line4->SetLineColor(kRed);
      line4->Draw("same");
    }
  }
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
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_purity->cd();
  purity_ratio->SetTitle("");
  purity_ratio->GetYaxis()->SetTitleSize(0.06);
  purity_ratio->GetYaxis()->SetTitleOffset(0.7);
  purity_ratio->GetXaxis()->SetTitleSize(0.06);
  purity_ratio->GetXaxis()->SetTitleOffset(0.7);
  purity_ratio->GetYaxis()->SetTitle("Purity [%]");
  purity_ratio->Scale(100);
  purity_ratio->Divide(purity_all);
  purity_ratio->DrawCopy();
  c_purity->SaveAs(save_dir_ + save);
  c_purity->Write();
  delete c_purity;
}

void Plotter::Plot_input(TH1* data, TH1* mc, TString save){
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  if(save.Contains("pseudo1")){
    name = " of pseudo1";
    dataset = "Pseudo data 1";
  }
  else if(save.Contains("pseudo2")){
    name = " of pseudo2";
    dataset = "Pseudo data 2";
  }
  else if(save.Contains("pseudo3")){
    name = " of pseudo3";
    dataset = "Pseudo data 3";
  }
  else if(save.Contains("data")){
    name = " of data";
    dataset = "Data";
  }
  TCanvas* c_input = new TCanvas("Input" + dataset, "Input", 1000, 800);
  c_input->cd();
  if(!save.Contains("dist")){
    l_input = new TLine(27.5,0,27.5,130);
    l_input->SetLineColor(kBlue);
    l_input_2 = new TLine(37.5,0,37.5,130);
    l_input_2->SetLineColor(kBlue);
  }
  mc->SetLineColor(633);
  mc->SetFillColor(633);
  mc->SetMarkerColor(633);
  if(save.Contains("dist")) mc->GetYaxis()->SetRangeUser(0, 500);
  else mc->GetYaxis()->SetRangeUser(0, 200);
  mc->SetTitle("");
  mc->GetYaxis()->SetTitle("Events");
  mc->GetXaxis()->SetTitle("detector binning");
  mc->GetYaxis()->SetTitleSize(0.06);
  mc->GetYaxis()->SetTitleOffset(0.7);
  mc->GetXaxis()->SetTitleSize(0.06);
  mc->GetXaxis()->SetTitleOffset(0.7);
  mc->Draw("hist");
  data->SetLineColor(kBlack);
  data->SetMarkerColor(kBlack);
  data->SetMarkerStyle(20);
  data->Draw("same");
  l_input->Draw("same");
  l_input_2->Draw("same");
  TLegend *legend_input = new TLegend(0.39, 0.70, 0.59, 0.85, "");
  legend_input->SetBorderSize(0);
  legend_input->SetFillStyle(0);
  legend_input->AddEntry(data, dataset, "lp");
  legend_input->AddEntry(mc, "MC", "f");
  legend_input->Draw();
  c_input->SaveAs(save_dir_ + save);
  c_input->Write();
  delete c_input;
}

void Plotter::Plot_covariance(TH2* matrix, TString save){
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
  if(save.Contains("total")) name += " total";
  else if(save.Contains("input")) name += " input";
  else if(save.Contains("matrix")) name += " matrix";

  TCanvas* c_cov = new TCanvas("Covariance"+name, "Covariance", 1000, 800);
  c_cov->cd();
  c_cov->SetLogz();
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.11);

  if(save.Contains("input")) matrix->SetTitle("Statistics of Pseudo data");
  else if(save.Contains("matrix")) matrix->SetTitle("Statistics of Migration Matrix");

  matrix->GetYaxis()->SetTitle("generator binning");
  matrix->GetYaxis()->SetTitleSize(0.06);
  matrix->GetYaxis()->SetTitleOffset(0.7);

  matrix->GetXaxis()->SetTitle("generator binning");
  matrix->GetXaxis()->SetTitleSize(0.06);
  matrix->GetXaxis()->SetTitleOffset(0.7);

  matrix->DrawCopy("colz");

  // Draw red lines to distinguish each region
  Double_t ymax = matrix->GetNbinsY();
  Double_t xmax = matrix->GetNbinsX();

  // draw vertical lines
  if(xmax/n_regions > 1){
    // TLine *line = new TLine(n_measurement_gen/n_mass_split_gen+0.5, 0.5, n_measurement_gen/n_mass_split_gen+0.5, ymax+0.5);
    // solid lines
    TLine *l_region_gen_vs1 = new TLine(n_measurement_gen + 0.5, 0.5, n_measurement_gen + 0.5, ymax + 0.5);
    l_region_gen_vs1->SetLineColor(kRed);
    l_region_gen_vs1->Draw("same");
    if(xmax/n_regions > 2){
      TLine *l_region_gen_vs2 = new TLine(n_measurement_gen + n_gen_pt_topjet + 0.5, 0.5, n_measurement_gen + n_gen_pt_topjet + 0.5, ymax + 0.5);
      l_region_gen_vs2->SetLineColor(kRed);
      l_region_gen_vs2->Draw("same");
    }
  }
  // dashed lines
  if(xmax/n_regions > 1){
    TLine *l_region_gen_vd1 = new TLine(n_measurement_gen/n_mass_split_gen+0.5, 0.5, n_measurement_gen/n_mass_split_gen+0.5, ymax+0.5);
    l_region_gen_vd1->SetLineColor(kRed);
    l_region_gen_vd1->SetLineStyle(5);
    l_region_gen_vd1->Draw("same");
    if(xmax/n_regions > 2){
      TLine *l_region_gen_vd2 = new TLine(2*n_measurement_gen/n_mass_split_gen+0.5, 0.5, 2*n_measurement_gen/n_mass_split_gen+0.5, ymax+0.5);
      l_region_gen_vd2->SetLineColor(kRed);
      l_region_gen_vd2->SetLineStyle(5);
      l_region_gen_vd2->Draw("same");
    }
  }

  //draw horizontal lines
  if(ymax/n_regions > 1){
    // TLine *line = new TLine(n_measurement_gen/n_mass_split_gen+0.5, 0.5, n_measurement_gen/n_mass_split_gen+0.5, ymax+0.5);
    TLine *l_region_gen_h1 = new TLine(0.5, n_measurement_gen + 0.5, xmax + 0.5, n_measurement_gen + 0.5);
    l_region_gen_h1->SetLineColor(kRed);
    l_region_gen_h1->Draw("same");
    if(ymax/n_regions > 2){
      TLine *l_region_gen_h2 = new TLine(0.5, n_measurement_gen + n_gen_pt_topjet + 0.5, xmax + 0.5, n_measurement_gen + n_gen_pt_topjet + 0.5);
      l_region_gen_h2->SetLineColor(kRed);
      l_region_gen_h2->Draw("same");
    }
  }



  c_cov->SaveAs(save_dir_+save);
  c_cov->Write();
  delete c_cov;
}
