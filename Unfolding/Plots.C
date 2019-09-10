#include "Plots.h"
#include "TVectorD.h"

using namespace std;
void text(TCanvas* c, bool over_canvas);

Plotter::Plotter(TUnfoldBinning* binning_gen, TUnfoldBinning* binning_rec){
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
  dim_measurement_rec = measurement_rec->GetDistributionDimension();
  n_measurement_gen = measurement_gen->GetTH1xNumberOfBins();
  dim_measurement_gen = measurement_gen->GetDistributionDimension();

  n_mass_split_rec = mass_split_rec->GetNoElements() - 1;
  n_mass_split_gen = mass_split_gen->GetNoElements() - 1;

  n_rec_pt_topjet = rec_pt_topjet_sideband->GetTH1xNumberOfBins();
  dim_rec_pt_topjet = rec_pt_topjet_sideband->GetDistributionDimension();
  n_gen_pt_topjet = gen_pt_topjet_sideband->GetTH1xNumberOfBins();
  dim_gen_pt_topjet = gen_pt_topjet_sideband->GetDistributionDimension();

  n_mass_rec = rec_mass_sideband->GetTH1xNumberOfBins();
  dim_mass_rec = rec_mass_sideband->GetDistributionDimension();
  n_mass_gen = gen_mass_sideband->GetTH1xNumberOfBins();
  dim_mass_gen = gen_mass_sideband->GetDistributionDimension();

  gStyle->SetOptStat(kFALSE);
  gStyle->SetLegendBorderSize(0);

  // gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle -> SetPadTickX(1);
  gStyle -> SetPadTickY(1);
  // gStyle->SetEndErrorSize(5);

  // gStyle->SetPadBorderMode(0);
  // gStyle->SetPadColor(kWhite);
  // gStyle->SetPadGridX(false);
  // gStyle->SetPadGridY(false);
  // gStyle->SetGridColor(0);
  // gStyle->SetGridStyle(3);
  // gStyle->SetGridWidth(1);
  //
  // gStyle->SetFrameBorderMode(0);
  // gStyle->SetFrameBorderSize(1);
  // gStyle->SetFrameFillColor(0);
  // gStyle->SetFrameFillStyle(0);
  // gStyle->SetFrameLineColor(1);
  // gStyle->SetFrameLineStyle(1);
  // gStyle->SetFrameLineWidth(1);
  //
  // gStyle->SetTitleFont(42, "XYZ");
  // gStyle->SetLabelFont(42, "XYZ");
  //
  // gStyle->SetAxisColor(1, "XYZ");
  // gStyle->SetStripDecimals(kTRUE);
  // gStyle->SetTickLength(0.03, "XYZ");
  // gStyle->SetNdivisions(510, "XYZ");
}


void Plotter::Plot_projections(TH1D* projection_gen, TH1* h_input_gen, TH1D* projection_rec, TH1* h_input_rec, TString directory){
  TH1D* projection_gen_clone = (TH1D*) projection_gen->Clone("projection_gen_clone");
  TH1D* projection_rec_clone = (TH1D*) projection_rec->Clone("projection_rec_clone");
  TH1* h_input_gen_clone = (TH1*) h_input_gen->Clone("h_input_gen_clone");
  TH1* h_input_rec_clone = (TH1*) h_input_rec->Clone("h_input_rec_clone");
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);

  TCanvas* c_projection_x = new TCanvas("Projection X", "Projection X", 400, 400);
  c_projection_x->cd();
  c_projection_x->UseCurrentStyle();
  int max_bin = projection_gen_clone->GetMaximumBin();
  double max_value = projection_gen_clone->GetBinContent(max_bin)*1.1;
  double min_value = 0.;
  projection_gen_clone->GetYaxis()->SetRangeUser(min_value, max_value);
  projection_gen_clone->GetYaxis()->SetTitleSize(0.05);
  projection_gen_clone->GetYaxis()->SetTitleOffset(1.6);
  projection_gen_clone->GetXaxis()->SetTitleSize(0.05);
  projection_gen_clone->GetXaxis()->SetTitleOffset(0.9);
  projection_gen_clone->GetYaxis()->SetTitle("Events");
  projection_gen_clone->GetXaxis()->SetTitle("gen binning");
  projection_gen_clone->DrawCopy();
  h_input_gen_clone->SetLineColor(kRed);
  h_input_gen_clone->SetMarkerColor(kRed);
  h_input_gen_clone->DrawCopy("same");

  TLine *line = new TLine(n_measurement_gen+0.5, min_value, n_measurement_gen+0.5, max_value);
  line->SetLineColor(kRed);
  line->Draw("same");

  TLine *line2 = new TLine(n_measurement_gen/n_mass_split_gen+0.5, min_value, n_measurement_gen/n_mass_split_gen+0.5, max_value);
  line2->SetLineColor(kRed);
  line2->SetLineStyle(4);
  line2->Draw("same");
  TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, min_value, n_measurement_gen+n_gen_pt_topjet+0.5, max_value);
  line3->SetLineColor(kRed);
  line3->Draw("same");

  c_projection_x->SaveAs(directory+"projection_x.eps");
  delete c_projection_x;

  TCanvas* c_projection_Y = new TCanvas("Projection Y", "Projection Y", 400, 400);
  c_projection_Y->cd();
  c_projection_Y->UseCurrentStyle();
  max_bin = projection_rec_clone->GetMaximumBin();
  max_value = projection_rec_clone->GetBinContent(max_bin)*1.1;
  projection_rec_clone->GetYaxis()->SetRangeUser(min_value, max_value);
  projection_rec_clone->GetYaxis()->SetTitleSize(0.05);
  projection_rec_clone->GetYaxis()->SetTitleOffset(1.6);
  projection_rec_clone->GetXaxis()->SetTitleSize(0.05);
  projection_rec_clone->GetXaxis()->SetTitleOffset(0.9);
  projection_rec_clone->GetYaxis()->SetTitle("Events");
  projection_rec_clone->GetXaxis()->SetTitle("rec binning");
  projection_rec_clone->DrawCopy();
  h_input_rec_clone->SetLineColor(kRed);
  h_input_rec_clone->SetMarkerColor(kRed);
  h_input_rec_clone->DrawCopy("same");

  TLine *line4 = new TLine(n_measurement_rec+0.5, min_value, n_measurement_rec+0.5, max_value);
  line4->SetLineColor(kRed);
  line4->Draw("same");
  TLine *line5 = new TLine(n_measurement_rec/n_mass_split_rec+0.5, min_value, n_measurement_rec/n_mass_split_rec+0.5, max_value);
  line5->SetLineColor(kRed);
  line5->SetLineStyle(4);
  line5->Draw("same");
  TLine *line6 = new TLine(n_measurement_rec+n_rec_pt_topjet+0.5, min_value, n_measurement_rec+n_rec_pt_topjet+0.5, max_value);
  line6->SetLineColor(kRed);
  line6->Draw("same");

  c_projection_Y->SaveAs(directory+"projection_y.eps");
  delete c_projection_Y;
}




void Plotter::Plot_output(TH1* h_unfolded, TH1* h_truth_, bool normalise, TString directory, TString save){
  TH1* h_unfolded_clone = (TH1*) h_unfolded->Clone("h_unfolded_clone");
  TH1* h_truth_clone = (TH1*) h_truth_->Clone("h_truth_clone");

  TCanvas* c_unfolding = new TCanvas(save , "Unfolding result", 400, 400);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_unfolding->cd();
  c_unfolding->UseCurrentStyle();
  int max_bin = 0;
  double max_value = 0.;
  double min_value = 0.;
  if(!normalise){
    if(h_unfolded_clone->GetMaximum() > h_truth_clone->GetMaximum()){
      max_value = h_unfolded_clone->GetMaximum();
      max_value *= 1.1;
    }
    else{
      max_value = h_truth_clone->GetMaximum();
      max_value *= 1.1;
    }

    h_unfolded_clone->GetYaxis()->SetRangeUser(min_value, max_value);
    h_unfolded_clone->SetMarkerStyle(20);
    h_unfolded_clone->SetLineColor(kBlack);
    h_unfolded_clone->SetMarkerColor(kBlack);
    h_unfolded_clone->GetYaxis()->SetTitleSize(0.05);
    h_unfolded_clone->GetYaxis()->SetTitleOffset(1.6);
    h_unfolded_clone->GetYaxis()->SetTitle("Events");
    if(save.Contains("all")) h_unfolded_clone->GetXaxis()->SetTitle("generator binning");
    else h_unfolded_clone->GetXaxis()->SetTitle("#tau_{3/2}");
    h_unfolded_clone->GetXaxis()->SetTitleSize(0.05);
    h_unfolded_clone->GetXaxis()->SetTitleOffset(0.9);
    h_unfolded_clone->SetTitle("");
    h_unfolded_clone->DrawCopy("E1");
    h_truth_clone->DrawCopy("same hist");

    if(save.Contains("all")){
      TLine *line = new TLine(n_measurement_gen+0.5, min_value, n_measurement_gen+0.5, max_value*0.85);
      line->SetLineColor(kRed);
      line->Draw("same");

      TLine *line2 = new TLine(n_measurement_gen/n_mass_split_gen+0.5, min_value, n_measurement_gen/n_mass_split_gen+0.5, max_value*0.85);
      line2->SetLineColor(kRed);
      line2->SetLineStyle(4);
      line2->Draw("same");
      TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, min_value, n_measurement_gen+n_gen_pt_topjet+0.5, max_value*0.85);
      line3->SetLineColor(kRed);
      line3->Draw("same");
    }

    TLegend *legend1;
    if(save.Contains("all")) legend1 = new TLegend(0.40, 0.62, 0.80, 0.82, "");
    else legend1 = new TLegend(0.15, 0.22, 0.55, 0.42, "");
    legend1->SetFillStyle(0);
    legend1->AddEntry(h_unfolded_clone, "unfolded data","ple");
    legend1->AddEntry(h_truth_clone, "truth", "l");
    legend1->Draw();
  }
  else{
    TH1 *unfolded_data_norm = (TH1*) h_unfolded_clone->Clone("unfolded_data_norm");
    TH1 *h_truth_norm = (TH1*) h_truth_clone->Clone("h_truth_norm");
    TH1D* h_relative = new TH1D("h_"+save, "", unfolded_data_norm->GetNbinsX(), 0., 1.);

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
    if(unfolded_data_norm->GetMaximum() > h_truth_norm->GetMaximum()){
      max_value = unfolded_data_norm->GetMaximum()*1.1;
    }
    else{
      max_value = h_truth_norm->GetMaximum()*1.1;
    }

    unfolded_data_norm->GetYaxis()->SetRangeUser(min_value, max_value);
    unfolded_data_norm->SetMarkerStyle(20);
    unfolded_data_norm->SetLineColor(kBlack);
    unfolded_data_norm->SetMarkerColor(kBlack);
    unfolded_data_norm->GetYaxis()->SetTitleSize(0.05);
    unfolded_data_norm->GetYaxis()->SetTitleOffset(1.65);
    unfolded_data_norm->GetXaxis()->SetTitleSize(0.05);
    unfolded_data_norm->GetXaxis()->SetTitleOffset(0.9);
    unfolded_data_norm->GetYaxis()->SetTitle("Events/Bin");
    if(save.Contains("all")) unfolded_data_norm->GetXaxis()->SetTitle("generator binning");
    else unfolded_data_norm->GetXaxis()->SetTitle("#tau_{3/2}");

    unfolded_data_norm->SetTitle("");
    unfolded_data_norm->DrawCopy();
    h_truth_norm->DrawCopy("same hist");

    if(save.Contains("all")){
      TLine *line = new TLine(n_measurement_gen+0.5, min_value, n_measurement_gen+0.5, max_value*0.85);
      line->SetLineColor(kRed);
      line->Draw("same");

      TLine *line2 = new TLine(n_measurement_gen/n_mass_split_gen+0.5, min_value, n_measurement_gen/n_mass_split_gen+0.5, max_value*0.85);
      line2->SetLineColor(kRed);
      line2->SetLineStyle(4);
      line2->Draw("same");
      TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, min_value, n_measurement_gen+n_gen_pt_topjet+0.5, max_value*0.85);
      line3->SetLineColor(kRed);
      line3->Draw("same");
    }

    TLegend *legend_unfolding_norm;
    if(save.Contains("all")) legend_unfolding_norm = new TLegend(0.40, 0.62, 0.80, 0.82, "");
    else if(save.Contains("normalised")) legend_unfolding_norm = new TLegend(0.15, 0.72, 0.45, 0.88, "");
    else legend_unfolding_norm = new TLegend(0.15, 0.22, 0.65, 0.42, "");
    legend_unfolding_norm->SetFillStyle(0);
    legend_unfolding_norm->AddEntry(unfolded_data_norm, "unfolded data","ple");
    legend_unfolding_norm->AddEntry(h_truth_norm, "truth", "l");
    legend_unfolding_norm->Draw();
  }

  c_unfolding->SaveAs(directory + save + ".eps");
  delete c_unfolding;
}



void Plotter::Plot_result_with_uncertainty(TH1* h_unfolded, TH1* h_unfolded_uncertainty, TH1* h_data_truth, TH1* h_mc_truth, bool normalise, TString directory){

  TCanvas* c = new TCanvas("Unfolding result with uncertainty", "Unfolding Result", 400, 400);
  c->cd();
  gStyle->SetPadLeftMargin(0.19);
  gStyle->SetPadRightMargin(0.01);
  c->UseCurrentStyle();
  double min_value = 0;
  double max_bin = h_unfolded_uncertainty->GetMaximumBin();
  double max_value = (h_unfolded_uncertainty->GetBinContent(max_bin)+h_unfolded_uncertainty->GetBinError(max_bin))*1.1;

  if(h_unfolded_uncertainty->GetBinContent(h_unfolded_uncertainty->GetMaximumBin()) > h_data_truth->GetBinContent(h_data_truth->GetMaximumBin())){
    max_bin = h_unfolded_uncertainty->GetMaximumBin();
    max_value = (h_unfolded_uncertainty->GetBinContent(max_bin)+h_unfolded_uncertainty->GetBinError(max_bin))*1.15;
  }
  else{
    max_bin = h_data_truth->GetMaximumBin();
    max_value = (h_data_truth->GetBinContent(max_bin)+h_data_truth->GetBinError(max_bin))*1.15;
  }
  max_value = 400;
  if(normalise){
    TH1 *unfolded_norm = (TH1*) h_unfolded->Clone("unfolded_norm");
    TH1 *unfolded_uncer_norm = (TH1*) h_unfolded_uncertainty->Clone("unfolded_uncer_norm");
    TH1 *h_data_truth_norm = (TH1*) h_data_truth->Clone("h_data_truth_norm");
    TH1 *h_mc_truth_norm = (TH1*) h_data_truth->Clone("h_mc_truth_norm");
    TH1D* h_relative = new TH1D("h_norm", "", unfolded_norm->GetNbinsX(), 0., 1.);
    TH1D* h_relative_uncer = new TH1D("h_norm_uncer", "", unfolded_uncer_norm->GetNbinsX(), 0., 1.);

    for(int i = 1; i <= unfolded_norm->GetNbinsX(); i++){
      double content = unfolded_norm->GetBinContent(i);
      double error_bin = unfolded_norm->GetBinError(i);
      double bin_width = unfolded_norm->GetBinWidth(i);
      double new_content = content/bin_width;
      double new_error = error_bin/bin_width;

      double content_uncer = unfolded_uncer_norm->GetBinContent(i);
      double error_bin_uncer = unfolded_uncer_norm->GetBinError(i);
      double bin_width_uncer = unfolded_uncer_norm->GetBinWidth(i);
      double new_content_uncer = content_uncer/bin_width_uncer;
      double new_error_uncer = error_bin_uncer/bin_width_uncer;

      double content_truth = h_mc_truth->GetBinContent(i);
      double bin_width_truth = h_mc_truth->GetBinWidth(i);
      double new_content_truth = content_truth/bin_width_truth;

      double content_truth_data = h_data_truth_norm->GetBinContent(i);
      double bin_width_truth_data = h_data_truth_norm->GetBinWidth(i);
      double new_content_truth_data = content_truth_data/bin_width_truth_data;

      h_mc_truth_norm->SetBinContent(i, new_content_truth);
      h_mc_truth_norm->SetBinError(i, 0.0000001);
      h_data_truth_norm->SetBinContent(i, new_content_truth_data);
      h_data_truth_norm->SetBinError(i, 0.0000001);

      unfolded_norm->SetBinContent(i, new_content);
      unfolded_norm->SetBinError(i, new_error);
      unfolded_uncer_norm->SetBinContent(i, new_content_uncer);
      unfolded_uncer_norm->SetBinError(i, new_error_uncer);

      double relative_error = unfolded_norm->GetBinError(i)/unfolded_norm->GetBinContent(i)*100;
      h_relative->SetBinContent(i, relative_error);
      double relative_error_uncer = unfolded_uncer_norm->GetBinError(i)/unfolded_uncer_norm->GetBinContent(i)*100;
      h_relative_uncer->SetBinContent(i, relative_error_uncer);
    }

    if(unfolded_norm->GetBinContent(unfolded_norm->GetMaximumBin()) > h_data_truth_norm->GetBinContent(h_data_truth_norm->GetMaximumBin())){
      max_bin = unfolded_norm->GetMaximumBin();
      max_value = (unfolded_norm->GetBinContent(max_bin)+unfolded_norm->GetBinError(max_bin))*1.15;
    }
    else{
      max_bin = h_data_truth_norm->GetMaximumBin();
      max_value = (h_data_truth_norm->GetBinContent(max_bin)+h_data_truth_norm->GetBinError(max_bin))*1.15;
    }

    unfolded_norm->GetYaxis()->SetRangeUser(0, 400);
    unfolded_norm->SetMarkerStyle(20);
    unfolded_norm->SetLineColor(kBlack);
    unfolded_norm->SetMarkerColor(kBlack);
    unfolded_norm->GetYaxis()->SetLabelSize(0.04);
    unfolded_norm->GetYaxis()->SetTitleSize(0.07);
    unfolded_norm->GetYaxis()->SetTitleOffset(1.1);
    unfolded_norm->GetXaxis()->SetLabelSize(0.04);
    unfolded_norm->GetXaxis()->SetTitleSize(0.07);
    unfolded_norm->GetXaxis()->SetTitleOffset(0.9);
    unfolded_norm->GetYaxis()->SetTitle("Events/Bin");
    if(directory.Contains("all")) unfolded_norm->GetXaxis()->SetTitle("generator binning");
    else unfolded_norm->GetXaxis()->SetTitle("#tau_{3/2}");

    unfolded_norm->SetTitle("");
    unfolded_norm->Draw("e1");
    unfolded_uncer_norm->SetLineColor(kBlack);
    unfolded_uncer_norm->SetMarkerColor(kBlack);
    unfolded_uncer_norm->Draw("same e1");
    c->Modified();
    c->Update();
    // if(!directory.Contains("data")){
    //   h_data_truth_norm->SetLineColor(kRed);
    //   h_data_truth_norm->SetMarkerColor(kRed);
    //   h_data_truth_norm->Draw("same hist");
    //   c->Modified();
    //   c->Update();
    // }
    h_mc_truth_norm->SetLineColor(kBlue+2);
    h_mc_truth_norm->SetMarkerColor(kBlue+2);
    h_mc_truth_norm->Draw("same hist");
    c->Modified();
    c->Update();

    if(directory.Contains("_all_")){
      TLine *line = new TLine(n_measurement_gen+0.5, min_value, n_measurement_gen+0.5, max_value*0.85);
      line->SetLineColor(kRed);
      line->Draw("same");

      TLine *line2 = new TLine(n_measurement_gen/n_mass_split_gen+0.5, min_value, n_measurement_gen/n_mass_split_gen+0.5, max_value*0.85);
      line2->SetLineColor(kRed);
      line2->SetLineStyle(4);
      line2->Draw("same");
      TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, min_value, n_measurement_gen+n_gen_pt_topjet+0.5, max_value*0.85);
      line3->SetLineColor(kRed);
      line3->Draw("same");
    }

    TLegend *legend_unfolding_norm;
    if(normalise) legend_unfolding_norm = new TLegend(0.2, 0.7, 0.5, 0.85, "");
    else if(directory.Contains("all")) legend_unfolding_norm = new TLegend(0.40, 0.62, 0.80, 0.82, "");
    else legend_unfolding_norm = new TLegend(0.15, 0.22, 0.65, 0.42, "");
    legend_unfolding_norm->SetFillStyle(0);
    legend_unfolding_norm->AddEntry(unfolded_norm, "unfolded data","ple");
    // if(!directory.Contains("data")) legend_unfolding_norm->AddEntry(h_data_truth_norm, "Data truth", "l");
    legend_unfolding_norm->AddEntry(h_mc_truth_norm, "POWHEG Truth", "l");
    legend_unfolding_norm->Draw();

  }
  else{

    h_mc_truth->GetYaxis()->SetRangeUser(0, max_value);
    if(directory.Contains("CS")) h_mc_truth->GetYaxis()->SetTitle("#frac{d#sigma}{d#tau_{32}} [fb]");
    else h_mc_truth->GetYaxis()->SetTitle("Events");
    h_mc_truth->SetTitle("");
    h_mc_truth->GetYaxis()->SetLabelSize(0.04);
    h_mc_truth->GetYaxis()->SetTitleSize(0.05);
    h_mc_truth->GetYaxis()->SetTitleOffset(1.6);
    h_mc_truth->GetXaxis()->SetLabelSize(0.04);
    h_mc_truth->GetXaxis()->SetTitleSize(0.05);
    h_mc_truth->GetXaxis()->SetTitleOffset(0.9);
    h_mc_truth->SetLineWidth(3);
    if(directory.Contains("data")){
      h_data_truth->SetLineColor(kBlue+2);
      h_data_truth->SetMarkerColor(kBlue+2);
      h_data_truth->SetLineStyle(7);
      h_mc_truth->SetLineColor(kRed);
      h_mc_truth->SetMarkerColor(kRed);
    }
    else{
      // h_mc_truth->SetLineStyle(7);
      // h_data_truth->SetLineColor(kRed);
      // h_data_truth->SetMarkerColor(kRed);
        h_data_truth->SetLineColor(kBlue+2);
        h_data_truth->SetMarkerColor(kBlue+2);
        h_data_truth->SetLineStyle(7);
        h_mc_truth->SetLineColor(kRed);
        h_mc_truth->SetMarkerColor(kRed);
    }
    h_mc_truth->Draw("hist");
    h_data_truth->SetLineWidth(3);
    for(int i = 1; i <= h_data_truth->GetNbinsX(); i++) h_data_truth->SetBinError(i, 0.000001);
    h_data_truth->Draw("same hist");
    h_unfolded_uncertainty->SetLineColor(kBlack);
    h_unfolded_uncertainty->SetMarkerColor(kBlack);
    h_unfolded_uncertainty->SetMarkerStyle(9);
    h_unfolded_uncertainty->SetMarkerSize(1);
    h_unfolded_uncertainty->Draw("same E1");
    h_unfolded->SetLineColor(kBlack);
    h_unfolded->SetMarkerColor(kBlack);
    h_unfolded->SetMarkerStyle(9);
    h_unfolded->SetMarkerSize(1);
    h_unfolded->Draw("same E1");

    TLegend *legend;
    if(directory.Contains("CS")) legend = new TLegend(0.53,0.77,0.83,0.92);
    else if(!directory.Contains("data")) legend = new TLegend(0.15, 0.32, 0.45, 0.52);
    else legend = new TLegend(0.15, 0.18, 0.45, 0.38);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);
    legend->AddEntry(h_unfolded_uncertainty, "Unfolded MADGRAPH", "lpe");
    if(directory.Contains("data")){
      legend->AddEntry(h_mc_truth, "POWHEG", "l");
      legend->AddEntry(h_data_truth, "MADGRAPH", "l");

    }
    else{
      legend->AddEntry(h_mc_truth, "POWHEG (Mig. Ma.)", "l");
      legend->AddEntry(h_data_truth, "MADGRAPH", "l");
    }
    legend->SetTextSize(0.04);


    legend->Draw();
  }
  text(c,false);
  c->SaveAs(directory + "Result_w_uncertainty.eps");
  delete c;
}



void Plotter::Plot_uncertainty(std::vector<TH1*> error, std::vector<TString> error_name, TString directory){
  std::vector<TH1*> h_rel;
  for(unsigned int i = 0; i < error.size(); i++){
    TH1* h_temp = (TH1*) error[i]->Clone();
    // cout << "error_name[i]: " << error_name[i] << '\n';
    for(int j = 1; j <= h_temp->GetNbinsX(); j++){
      double relative_error = 0.;
      relative_error = error[i]->GetBinContent(j);
      h_temp->SetBinContent(j, relative_error);
    }
    h_rel.push_back(h_temp);
  }
  int last_element = h_rel.size()-1;

  TCanvas* c_relative = new TCanvas("relative_", "relative", 400, 400);
  c_relative->cd();
  gStyle->SetPadLeftMargin(0.19);
  gStyle->SetPadRightMargin(0.01);
  gStyle->SetPadTopMargin(0.045);
  // gStyle->SetLegendTextSize(0.1);
  c_relative->UseCurrentStyle();
  double max = 0;

  for(unsigned int i = 0; i < h_rel.size(); i++){
    if(h_rel.at(i)->GetMaximum() > max) max = h_rel.at(i)->GetMaximum();
  }

  h_rel[last_element]->GetYaxis()->SetRangeUser(0., 100.);
  h_rel[last_element]->GetYaxis()->SetTitle("relative uncertainty [%]");
  h_rel[last_element]->GetXaxis()->SetTitle("#tau_{3/2}");
  h_rel[last_element]->SetLineColor(kGray);
  h_rel[last_element]->SetFillColor(kGray);
  h_rel[last_element]->GetYaxis()->SetTitleSize(0.05);
  h_rel[last_element]->GetYaxis()->SetLabelSize(0.04);
  h_rel[last_element]->GetYaxis()->SetTitleOffset(1.6);
  h_rel[last_element]->GetXaxis()->SetTitleSize(0.05);
  h_rel[last_element]->GetXaxis()->SetLabelSize(0.04);
  h_rel[last_element]->GetXaxis()->SetTitleOffset(0.9);
  h_rel[last_element]->Draw("hist");



  TLegend* legend_rel;
  // cout << "last_element: " << last_element << '\n';
  // cout << "error_name size: " << error_name.size() << '\n';
  // cout << "error_name[last_element]: " << error_name[last_element] << '\n';

  if(!error_name[last_element].Contains("model")) legend_rel = new TLegend(0.42,0.6,0.88,0.88); //  0.2, 0.57, 0.8, 0.9
  else legend_rel = new TLegend(0.25, 0.57, 0.55, 0.9);
  if(!error_name[last_element].Contains("model")) legend_rel->SetNColumns(2);
  legend_rel->AddEntry(h_rel[last_element], error_name[last_element], "f");

  for(int i = 0; i < last_element; i++){
    h_rel[i]->SetLineWidth(3);
    if(error_name[i]=="stat"){
      h_rel[i]->SetLineColor(kBlack);
      h_rel[i]->SetMarkerColor(kBlack);
    }
    else if(error_name[i]=="MC stat"){
      h_rel[i]->SetLineColor(kBlack);
      h_rel[i]->SetLineStyle(5);
      h_rel[i]->SetMarkerColor(kBlack);
    }
    else if(error_name[i].Contains("background")){
      h_rel[i]->SetLineColor(kRed);
      h_rel[i]->SetMarkerColor(kRed);
    }
    else if(error_name[i].Contains("JEC")){
      h_rel[i]->SetLineColor(kBlue);
      h_rel[i]->SetMarkerColor(kBlue);
      error_name[i] = "jet energy scale";
    }
    else if(error_name[i].Contains("JER")){
      h_rel[i]->SetLineColor(kBlue);
      h_rel[i]->SetLineStyle(5);
      h_rel[i]->SetMarkerColor(kBlue);
      error_name[i] = "jet energy resolution";
    }
    else if(error_name[i].Contains("BTag")){
      h_rel[i]->SetLineColor(kGreen+2);
      h_rel[i]->SetMarkerColor(kGreen+2);
      error_name[i] = "b tagging";
    }
    else if(error_name[i].Contains("PU")){
      h_rel[i]->SetLineColor(kOrange-3);
      h_rel[i]->SetMarkerColor(kOrange-3);
      error_name[i] = "pileup";
    }
    else if(error_name[i].Contains("MUID")){
      h_rel[i]->SetLineColor(kOrange);
      h_rel[i]->SetMarkerColor(kOrange);
      error_name[i] = "muon ID";
    }
    else if(error_name[i].Contains("MUTrigger")){
      h_rel[i]->SetLineColor(kViolet);
      h_rel[i]->SetMarkerColor(kViolet);
      error_name[i] = "muon trigger";
    }
    else if(error_name[i].Contains("scale")){
      h_rel[i]->SetLineColor(kOrange-3);
      h_rel[i]->SetMarkerColor(kOrange-3);
      error_name[i] = "scale";
    }
    else if(error_name[i].Contains("mtop")){
      h_rel[i]->SetLineColor(kGreen+2);
      h_rel[i]->SetMarkerColor(kGreen+2);
      error_name[i] = "choice of m_{t}";
    }
    else if(error_name[i].Contains("ELEID")){
      h_rel[i]->SetLineColor(kOrange);
      h_rel[i]->SetLineStyle(5);
      h_rel[i]->SetMarkerColor(kOrange);
      error_name[i] = "electron ID";
    }
    else if(error_name[i].Contains("ELETrigger")){
      h_rel[i]->SetLineColor(kViolet);
      h_rel[i]->SetLineStyle(5);
      h_rel[i]->SetMarkerColor(kViolet);
      error_name[i] = "electron trigger";
    }
    else if(error_name[i].Contains("ELEReco")){
      h_rel[i]->SetLineColor(kCyan+2);
      h_rel[i]->SetLineStyle(5);
      h_rel[i]->SetMarkerColor(kCyan+2);
      error_name[i] = "electron reconstruction";
    }
    else if(error_name[i].Contains("isr")){
      h_rel[i]->SetLineColor(kRed);
      h_rel[i]->SetMarkerColor(kRed);
      error_name[i] = "ISR";
    }
    else if(error_name[i].Contains("fsr")){
      h_rel[i]->SetLineColor(kMagenta+2);
      h_rel[i]->SetMarkerColor(kMagenta+2);
      error_name[i] = "FSR";
    }
    else if(error_name[i].Contains("hdamp")){
      h_rel[i]->SetLineColor(kCyan+2);
      h_rel[i]->SetMarkerColor(kCyan+2);
      error_name[i] = "h_{damp}";
    }
    h_rel[i]->Draw("same e1");
    legend_rel->AddEntry(h_rel[i], error_name[i], "l");
  }
  legend_rel->SetBorderSize(0);
  legend_rel->SetFillStyle(0);
  legend_rel->SetTextSize(0.03);
  legend_rel->Draw();
  // text(c_relative);
  c_relative->SaveAs(directory + "rel_uncertainty.eps");
  delete c_relative;
}






void Plotter::Plot_correlation_matrix(TH2* h_corr_matrix, TString save){
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.12);
  TH2* h_corr_matrix_clone = (TH2*) h_corr_matrix->Clone("h_corr_matrix_clone");
  TString name = "";

  if(save.Contains("LCurve")) name = " (LCurve)";
  else if(save.Contains("TauScan")) name = " (TauScan)";
  else if(save.Contains("CustomTau")) name = " (CustomTau)";
  TCanvas* c_corr_matrix = new TCanvas("Correlation Matrix" + name, "Correlation Matrix", 400, 400);
  c_corr_matrix->cd();
  h_corr_matrix_clone->SetTitle("");
  if(save.Contains("all"))h_corr_matrix_clone->GetXaxis()->SetTitle("generator binning");
  else h_corr_matrix_clone->GetXaxis()->SetTitle("#tau_{3/2}");
  h_corr_matrix_clone->GetYaxis()->SetTitleSize(0.07);
  h_corr_matrix_clone->GetYaxis()->SetTitleOffset(0.7);
  h_corr_matrix_clone->GetXaxis()->SetTitleSize(0.07);
  h_corr_matrix_clone->GetXaxis()->SetTitleOffset(0.9);
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
  c_corr_matrix->SaveAs(save + ".eps");
  delete c_corr_matrix;
}





void Plotter::Plot_LogTau(TSpline* logTau_, double tau, double coordinate, TString directory){
  TString y_axis_title = "";
  if(directory.Contains("X")) y_axis_title = "L_{X}";
  else if(directory.Contains("Y")) y_axis_title = "L_{Y}";

  TCanvas* c_lcurve = new TCanvas("LogTau", "LCurve of unfolding", 400, 400);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_lcurve->cd();
  c_lcurve->UseCurrentStyle();
  double logtau = TMath::Log10(tau);
  std::vector<double> Xpoint = {logtau};
  std::vector<double> Ypoint = {coordinate};
  TGraph * point = new TGraph(1, &Xpoint[0], &Ypoint[0]);
  point->SetTitle("");
  point->GetYaxis()->SetTitleSize(0.07);
  point->GetYaxis()->SetTitleOffset(0.7);
  point->GetXaxis()->SetTitleSize(0.07);
  point->GetXaxis()->SetTitleOffset(0.9);
  point->GetXaxis()->SetTitle("log #tau");
  point->GetYaxis()->SetTitle(y_axis_title);
  point->GetXaxis()->SetNdivisions(510);
  point->GetYaxis()->SetNdivisions(810);
  point->SetMarkerStyle(20);
  point->SetMarkerSize(1.5);
  point->SetMarkerColor(kRed);
  point->SetLineColor(kRed);
  point->Draw("AP");
  point->GetXaxis()->SetLimits(-6.0, 0.0);
  // point->GetYaxis()->SetRangeUser(-6.0, 3.0);
  if(directory.Contains("Y")) point->GetYaxis()->SetRangeUser(-3, 8);
  point->Draw("AP");
  c_lcurve->Update();
  logTau_->SetLineColor(kBlack);
  logTau_->Draw("same");
  point->Draw("psame");
  TLegend *legend_logtau = new TLegend(0.15, 0.42, 0.65, 0.62, "");
  legend_logtau->SetFillStyle(0);
  legend_logtau->AddEntry(logTau_, "unfolded data", "l");
  legend_logtau->AddEntry(point, "log(#tau) value", "p");
  legend_logtau->Draw();
  c_lcurve->SaveAs(directory+".eps");
  delete c_lcurve;
}




void Plotter::Plot_LCurve(TGraph* lcurve, std::vector<double> coordinates, std::vector<double> coordinates_2, TString directory){

  TCanvas* c_lcurve = new TCanvas("LCurve", "LCurve of Unfolded data (TauScan)", 400, 400);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_lcurve->cd();
  c_lcurve->UseCurrentStyle();
  TMarker* LCurve_tau = new TMarker(coordinates.at(0), coordinates.at(1), 20);
  LCurve_tau->SetMarkerColor(kRed);
  TMarker* TauScan_tau = new TMarker(coordinates_2.at(0), coordinates_2.at(1), 20);
  TauScan_tau->SetMarkerColor(kBlue);
  lcurve->GetYaxis()->SetTitleSize(0.07);
  lcurve->GetYaxis()->SetTitleOffset(0.6);
  lcurve->GetYaxis()->SetTitle("L_{Y}");
  lcurve->GetXaxis()->SetTitleSize(0.07);
  lcurve->GetXaxis()->SetTitleOffset(0.9);
  lcurve->GetXaxis()->SetTitle("L_{X}");
  lcurve->SetTitle("");
  lcurve->DrawClone();
  LCurve_tau->DrawClone("same");
  TauScan_tau->DrawClone("same");
  TLegend *legend_lcurve = new TLegend(0.15, 0.42, 0.65, 0.62, "");
  legend_lcurve->SetFillStyle(0);
  legend_lcurve->AddEntry(lcurve, "LCurve", "l");
  legend_lcurve->AddEntry(LCurve_tau, "#tau value of LCurve", "p");
  legend_lcurve->AddEntry(TauScan_tau, "#tau value of ScanTau", "p");
  legend_lcurve->Draw();
  c_lcurve->SaveAs(directory+"LCurve.eps");
  delete c_lcurve;
}






void Plotter::Plot_RhoLogTau(TSpline* rhologTau, double tau, TString directory){

  TCanvas* c = new TCanvas("RhoLogTau ", "LCurve2 of Unfolded data (TauScan)", 400, 400);
  gStyle->SetPadLeftMargin(0.19);
  gStyle->SetPadRightMargin(0.01);
  c->cd();
  c->UseCurrentStyle();
  std::vector<double> Xpoint_rhologtau = {TMath::Log10(tau)};
  std::vector<double> Ypoint_rhologtau = {rhologTau->Eval(TMath::Log10(tau))};
  TGraph * point_rhologtau = new TGraph(1, &Xpoint_rhologtau[0], &Ypoint_rhologtau[0]);
  point_rhologtau->SetTitle("");
  point_rhologtau->GetYaxis()->SetLabelSize(0.04);
  point_rhologtau->GetYaxis()->SetTitleSize(0.05);
  point_rhologtau->GetYaxis()->SetTitleOffset(1.6);
  point_rhologtau->GetXaxis()->SetLabelSize(0.04);
  point_rhologtau->GetXaxis()->SetTitleSize(0.05);
  point_rhologtau->GetXaxis()->SetTitleOffset(0.9);



  point_rhologtau->GetXaxis()->SetTitle("log #tau");
  point_rhologtau->GetYaxis()->SetTitle("#rho(log #tau)");
  // point_rhologtau->GetXaxis()->SetNdivisions(510);
  // point_rhologtau->GetYaxis()->SetNdivisions(810);

  point_rhologtau->SetMarkerStyle(20);
  point_rhologtau->SetMarkerSize(1.5);
  point_rhologtau->SetLineColor(1);
  // point_rhologtau->Draw("AP");
  point_rhologtau->GetXaxis()->SetLimits(-6.0, 0.0);
  point_rhologtau->GetYaxis()->SetRangeUser(0.6, 1.2);
  point_rhologtau->DrawClone("AP");
  c->Update();
  rhologTau->SetLineColor(kRed);
  rhologTau->SetLineWidth(3);
  rhologTau->DrawClone("samel");
  point_rhologtau->DrawClone("psame");
  TLegend *legend_rhologTau = new TLegend(0.25, 0.7, 0.65, 0.85, "");
  legend_rhologTau->SetFillStyle(0);
  legend_rhologTau->SetBorderSize(0);
  legend_rhologTau->AddEntry(point_rhologtau,"final #tau value","pl");
  legend_rhologTau->AddEntry(rhologTau,"#tau scan","l");
  legend_rhologTau->Draw();
  c->SaveAs(directory+"RhoLogTau.eps");
  delete c;
}



void Plotter::Plot_ResponseMatrix(TH2* resp_matrix_, TString directory){

  TCanvas* c_response = new TCanvas("Response Matrix", "", 400, 400);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.11);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.12);
  c_response->cd();
  c_response->UseCurrentStyle();
  if(directory.Contains("Migration"))c_response->SetLogz();
  resp_matrix_->SetTitle("");
  resp_matrix_->GetYaxis()->SetLabelSize(0);
  resp_matrix_->GetXaxis()->SetLabelSize(0);
  resp_matrix_->GetYaxis()->SetTitle("detector binning");
  resp_matrix_->GetYaxis()->SetTitleSize(0.05);
  resp_matrix_->GetYaxis()->SetTitleOffset(1.3);
  resp_matrix_->GetXaxis()->SetTitle("generator binning");
  resp_matrix_->GetXaxis()->SetTitleSize(0.05);
  resp_matrix_->GetXaxis()->SetTitleOffset(1.1);
  if(!directory.Contains("Migration")) resp_matrix_->GetZaxis()->SetRangeUser(0, 0.3);
  resp_matrix_->DrawCopy("colz");

  Double_t ymax = resp_matrix_->GetNbinsY();
  Double_t xmax = resp_matrix_->GetNbinsX();
  if(xmax/2 > 1){
    TLine *line = new TLine(n_measurement_gen+0.5, 0.5, n_measurement_gen+0.5, ymax+0.5);
    line->SetLineColor(kRed);
    line->Draw("same");

    TLatex text1;
    text1.SetTextFont(43);
    text1.SetTextSize(11);
    text1.SetTextAlign(12);
    double position1 = n_measurement_gen+n_gen_pt_topjet/2-1.5;
    text1.DrawLatex(position1, -2.5, "p_{T}^{} < 400 GeV");

    if(dim_measurement_gen > 1){
      double sub_region_gen = (n_measurement_gen/dim_measurement_gen);
      TLine *line_dashed = new TLine(sub_region_gen+0.5, 0.5, sub_region_gen+0.5, ymax+0.5);
      line_dashed->SetLineColor(kRed);
      line_dashed->SetLineStyle(4);
      line_dashed->Draw("same");

      TLatex text;
      text.SetTextFont(43);
      text.SetTextSize(11);
      text.SetTextAlign(12);
      double position = sub_region_gen/2-1.5;
      text.DrawLatex(position, -2.5, "m_{jet}^{} < 155 GeV");

      TLatex text2;
      text2.SetTextFont(43);
      text2.SetTextSize(11);
      text2.SetTextAlign(12);
      double position2 = n_measurement_gen-sub_region_gen+1;
      text2.DrawLatex(position2, -2.5, "155 GeV < m_{jet}");
    }

    TLine *line2 = new TLine(0.5, n_measurement_rec+0.5, xmax+0.5, n_measurement_rec+0.5);
    line2->SetLineColor(kRed);
    line2->Draw("same");

    TLatex text11;
    text11.SetTextFont(43);
    text11.SetTextSize(11);
    text11.SetTextAngle(105);
    text11.SetTextAlign(12);
    double position11 = n_measurement_rec+n_rec_pt_topjet/2-2.5;
    text11.DrawLatex(0.1, position11, "p_{T}^{} < 400 GeV");

    if(dim_measurement_rec > 1){

      double sub_region_rec = (n_measurement_rec/dim_measurement_rec);
      TLine *line_dashed = new TLine(0.5, sub_region_rec+0.5, xmax+0.5, sub_region_rec+0.5);
      line_dashed->SetLineColor(kRed);
      line_dashed->SetLineStyle(4);
      line_dashed->Draw("same");

      TLatex text;
      text.SetTextFont(43);
      text.SetTextSize(11);
      text.SetTextAngle(90);
      text.SetTextAlign(12);
      double position = sub_region_rec/2-sub_region_rec/2+2;
      text.DrawLatex(0.1, position, "m_{jet}^{} < 152 GeV");

      TLatex text2;
      text2.SetTextFont(43);
      text2.SetTextSize(11);
      text2.SetTextAngle(90);
      text2.SetTextAlign(12);
      double position2 = n_measurement_rec-sub_region_rec+2;
      text2.DrawLatex(0.1, position2, "152 GeV < m_{jet}");
    }

    if(xmax/3 >= 2){
      TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, 0.5, n_measurement_gen+n_gen_pt_topjet+0.5, ymax+0.5);
      line3->SetLineColor(kRed);
      line3->Draw("same");
      TLine *line4 = new TLine(0.5, n_measurement_rec+n_rec_pt_topjet+0.5, xmax+0.5, n_measurement_rec+n_rec_pt_topjet+0.5);
      line4->SetLineColor(kRed);
      line4->Draw("same");

      TLatex text4;
      text4.SetTextFont(43);
      text4.SetTextSize(11);
      text4.SetTextAlign(12);
      double position4 = n_measurement_gen+n_gen_pt_topjet+1+0.5;
      text4.DrawLatex(position4, -2.5, "M_{jet1}^{} < M_{jet2+lep}^{}");

      TLatex text44;
      text44.SetTextFont(43);
      text44.SetTextSize(11);
      text44.SetTextAngle(90);
      text44.SetTextAlign(12);
      double position44 = n_measurement_rec+n_rec_pt_topjet+2.5;
      text44.DrawLatex(0.1, position44, "M_{jet1}^{} < M_{jet2+lep}^{}");
    }
  }
  text(c_response, true);
  c_response->SaveAs(directory+".eps");
  delete c_response;
}





void Plotter::Plot_purity(TH1* purity_same, TH1* purity_all, TString directory){
  TH1* purity_ratio = (TH1*) purity_same->Clone("purity_ratio");

  TCanvas* c_purity = new TCanvas("Purity", "Purity", 400, 400);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  c_purity->cd();
  purity_ratio->SetTitle("");
  purity_ratio->GetYaxis()->SetTitleSize(0.07);
  purity_ratio->GetYaxis()->SetTitleOffset(0.7);
  purity_ratio->GetXaxis()->SetTitleSize(0.07);
  purity_ratio->GetXaxis()->SetTitleOffset(0.9);
  purity_ratio->GetYaxis()->SetTitle("Purity [%]");
  purity_ratio->Scale(100);
  purity_ratio->Divide(purity_all);
  purity_ratio->DrawCopy();
  c_purity->SaveAs(directory + "Purity.eps");
  delete c_purity;
}






void Plotter::Plot_input(TH1* data, TH1* mc, TString directory){
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.06);
  TCanvas* c_input = new TCanvas("Input"  , "Input", 400, 400);
  c_input->cd();
  c_input->UseCurrentStyle();
  // TLine *l_input, *l_input_2;
  // if(!directory.Contains("dist")){
  //   l_input = new TLine(27.5,0,27.5,130);
  //   l_input->SetLineColor(kBlue);
  //   l_input_2 = new TLine(37.5,0,37.5,130);
  //   l_input_2->SetLineColor(kBlue);
  // }
  mc->SetLineColor(633);
  mc->SetFillColor(633);
  mc->SetMarkerColor(633);
  double max_bin = data->GetMaximumBin();
  double max_value = data->GetBinContent(max_bin)*1.1;
  mc->GetYaxis()->SetRangeUser(0, max_value);
  mc->SetTitle("");
  mc->GetYaxis()->SetTitle("Events");
  mc->GetXaxis()->SetTitle("detector binning");
  mc->GetYaxis()->SetTitleSize(0.07);
  mc->GetYaxis()->SetTitleOffset(0.7);
  mc->GetXaxis()->SetTitleSize(0.07);
  mc->GetXaxis()->SetTitleOffset(0.9);
  mc->Draw("hist");
  data->SetLineColor(kBlack);
  data->SetMarkerColor(kBlack);
  data->SetMarkerStyle(20);
  data->Draw("same");
  // if(!directory.Contains("dist")) l_input->Draw("same");
  // if(!directory.Contains("dist")) l_input_2->Draw("same");
  TLegend *legend_input = new TLegend(0.39, 0.70, 0.59, 0.85, "");
  legend_input->SetFillStyle(0);
  legend_input->AddEntry(data, "Data", "lp");
  legend_input->AddEntry(mc, "MC", "f");
  legend_input->Draw();
  c_input->SaveAs(directory+"Input.eps");
  delete c_input;
}








void Plotter::Plot_covariance(TH2* matrix, TString directory){
  TCanvas* c_cov = new TCanvas("Covariance", "Covariance", 400, 400);
  c_cov->cd();
  c_cov->SetLogz();
  gStyle->SetPadLeftMargin(0.1);
  if(directory.Contains("Cov")) gStyle->SetPadRightMargin(0.15);
  else gStyle->SetPadRightMargin(0.11);
  c_cov->UseCurrentStyle();

  if(directory.Contains("all")) matrix->GetYaxis()->SetTitle("generator binning");
  else matrix->GetYaxis()->SetTitle("#tau_{3/2}");
  matrix->GetYaxis()->SetTitleSize(0.07);
  matrix->GetYaxis()->SetTitleOffset(0.7);

  if(directory.Contains("all")) matrix->GetXaxis()->SetTitle("generator binning");
  else matrix->GetYaxis()->SetTitle("#tau_{3/2}");
  matrix->GetXaxis()->SetTitleSize(0.07);
  matrix->GetXaxis()->SetTitleOffset(0.9);

  matrix->SetTitle("");
  if(directory.Contains("all")) matrix->GetYaxis()->SetLabelSize(0);
  if(directory.Contains("all")) matrix->GetXaxis()->SetLabelSize(0);
  if(directory.Contains("Cov")) matrix->GetZaxis()->SetRangeUser(0, 30000);
  else matrix->GetZaxis()->SetRangeUser(-1, 1);
  matrix->DrawCopy("colz");

  if(directory.Contains("all")) {
    Double_t ymax = matrix->GetNbinsY();
    Double_t xmax = matrix->GetNbinsX();
    if(xmax/2 > 1){
      TLine *line = new TLine(n_measurement_gen+0.5, 0.5, n_measurement_gen+0.5, ymax+0.5);
      line->SetLineColor(kRed);
      line->Draw("same");

      TLatex text1;
      text1.SetTextFont(43);
      text1.SetTextSize(18);
      text1.SetTextAlign(12);
      double position1 = n_measurement_gen+n_gen_pt_topjet/2-2;
      text1.DrawLatex(position1, -0.25, "300 GeV #leq p_{T}^{} < 400 GeV");

      if(dim_measurement_gen > 1){
        double sub_region_gen = (n_measurement_gen/dim_measurement_gen);
        TLine *line_dashed = new TLine(sub_region_gen+0.5, 0.5, sub_region_gen+0.5, ymax+0.5);
        line_dashed->SetLineColor(kRed);
        line_dashed->SetLineStyle(4);
        line_dashed->Draw("same");

        TLatex text;
        text.SetTextFont(43);
        text.SetTextSize(20);
        text.SetTextAlign(12);
        double position = sub_region_gen/2-1.5;
        text.DrawLatex(position, -0.25, "m_{jet}^{} < 155 GeV");

        TLatex text2;
        text2.SetTextFont(43);
        text2.SetTextSize(20);
        text2.SetTextAlign(12);
        double position2 = n_measurement_gen-sub_region_gen+1;
        text2.DrawLatex(position2, -0.25, "155 GeV < m_{jet}");
      }

      TLine *line2 = new TLine(0.5, n_measurement_gen+0.5, xmax+0.5, n_measurement_gen+0.5);
      line2->SetLineColor(kRed);
      line2->Draw("same");

      TLatex text11;
      text11.SetTextFont(43);
      text11.SetTextSize(18);
      text11.SetTextAngle(90);
      text11.SetTextAlign(12);
      double position11 = n_measurement_gen+n_gen_pt_topjet/2-2;
      text11.DrawLatex(0.1, position11, "300 GeV #leq p_{T}^{} < 400 GeV");

      if(dim_measurement_gen > 1){

        double sub_region_gen = (n_measurement_gen/dim_measurement_gen);
        TLine *line_dashed = new TLine(0.5, sub_region_gen+0.5, xmax+0.5, sub_region_gen+0.5);
        line_dashed->SetLineColor(kRed);
        line_dashed->SetLineStyle(4);
        line_dashed->Draw("same");

        TLatex text;
        text.SetTextFont(43);
        text.SetTextSize(20);
        text.SetTextAngle(90);
        text.SetTextAlign(12);
        double position = sub_region_gen/2-sub_region_gen/2+0.75;
        text.DrawLatex(0.1, position, "m_{jet}^{} < 155 GeV");

        TLatex text2;
        text2.SetTextFont(43);
        text2.SetTextSize(20);
        text2.SetTextAngle(90);
        text2.SetTextAlign(12);
        double position2 = n_measurement_gen-sub_region_gen+0.75;
        text2.DrawLatex(0.1, position2, "155 GeV < m_{jet}");
      }

      if(xmax/3 >= 2){
        TLine *line3 = new TLine(n_measurement_gen+n_gen_pt_topjet+0.5, 0.5, n_measurement_gen+n_gen_pt_topjet+0.5, ymax+0.5);
        line3->SetLineColor(kRed);
        line3->Draw("same");
        TLine *line4 = new TLine(0.5, n_measurement_gen+n_gen_pt_topjet+0.5, xmax+0.5, n_measurement_gen+n_gen_pt_topjet+0.5);
        line4->SetLineColor(kRed);
        line4->Draw("same");

        TLatex text4;
        text4.SetTextFont(43);
        text4.SetTextSize(18);
        text4.SetTextAlign(12);
        double position4 = n_measurement_gen+n_gen_pt_topjet+1;
        text4.DrawLatex(position4, -0.25, "M_{jet1}^{} < M_{jet2 + #mu}^{}");

        TLatex text44;
        text44.SetTextFont(43);
        text44.SetTextSize(18);
        text44.SetTextAngle(90);
        text44.SetTextAlign(12);
        double position44 = n_measurement_gen+n_gen_pt_topjet+0.75;
        text44.DrawLatex(0.1, position44, "M_{jet1}^{} < M_{jet2 + #mu}^{}");
      }
    }
  }
  c_cov->SaveAs(directory+".eps");
  delete c_cov;
}




void Plotter::Plot_delta(std::vector<std::vector<TH1*>> delta, std::vector<std::vector<TString>> delta_name, TString directory){
  for(unsigned int i = 0; i < delta.size(); i++){
    for(unsigned int j = 0; j < delta[i].size(); j++){
      TH1* dummy = (TH1*) delta[i][j]->Clone();
      dummy->Reset();
      for(int k = 1; k <= delta[i][j]->GetNbinsX(); k++){
        dummy->SetBinContent(k, delta[i][j]->GetBinContent(k));
        dummy->SetBinError(k, 0.01);
      }
      TLine *line = new TLine(0,0,1,0);
      line->SetLineColor(kBlack);
      TCanvas *c = new TCanvas("", "", 400, 400);
      c->cd();
      gStyle->SetPadTopMargin(0.1);
      c->UseCurrentStyle();
      double min_bin = dummy->GetMinimumBin();
      double min_value = dummy->GetBinContent(min_bin);
      double max_bin = dummy->GetMaximumBin();
      double max_value = dummy->GetBinContent(max_bin);
      if(min_value < 0) min_value *= 1.1;
      else min_value *= 0.9;
      if(max_value < 0) max_value = 0;
      else max_value *= 1.1;

      if(min_value < 0.) dummy->GetYaxis()->SetRangeUser(min_value, max_value);
      else dummy->GetYaxis()->SetRangeUser(0., max_value);
      dummy->SetMarkerStyle(10);
      dummy->SetLineColor(kRed);
      dummy->SetMarkerColor(kRed);
      dummy->SetTitle(delta_name[i][j]);
      dummy->GetYaxis()->SetTitle("#Delta Events");
      dummy->GetXaxis()->SetTitle("#tau_{3/2}");
      dummy->GetYaxis()->SetTitleSize(0.07);
      dummy->GetYaxis()->SetTitleOffset(0.7);
      dummy->GetXaxis()->SetTitleSize(0.07);
      dummy->GetXaxis()->SetTitleOffset(0.9);
      dummy->Draw("p E1");
      line->Draw("same");

      // TLegend *legend = new TLegend(0.2, 0.75, 0.35, 0.9, "");
      // legend->SetBorderSize(0);
      // legend->SetFillStyle(0);
      // legend->AddEntry(dummy, delta_name[i][j], "lp");
      // legend->Draw("same");
      c->SaveAs(directory+"Delta_"+delta_name[i][j]+".eps");
      delete c;
    }
  }
}

void Plotter::Plot_all_pseudo(TH1* pseudo1, TH1* pseudo1_truth_1, TH1* pseudo2, TH1* pseudo2_truth_1, TH1* pseudo3, TH1* pseudo3_truth_1, bool cs, TString directory){

  TH1* pseudo1_truth = (TH1*) pseudo1_truth_1->Clone("pseudo1_truth");
  TH1* pseudo2_truth = (TH1*) pseudo2_truth_1->Clone("pseudo2_truth");
  TH1* pseudo3_truth = (TH1*) pseudo3_truth_1->Clone("pseudo3_truth");

  if(!cs){
    for(int i = 1; i <= pseudo3_truth->GetNbinsX(); i++){
      double content_1 = pseudo1->GetBinContent(i);
      double error_bin_1 = pseudo1->GetBinError(i);
      double bin_width_1 = pseudo1->GetBinWidth(i);
      double bin_width_2 = pseudo2->GetBinWidth(i);
      double new_content_1 = content_1/bin_width_1;
      double new_error_1 = error_bin_1/bin_width_1;

      double content_2 = pseudo2->GetBinContent(i);
      double error_bin_2 = pseudo2->GetBinError(i);

      double new_content_2 = content_2/bin_width_2;
      double new_error_2 = error_bin_2/bin_width_2;


      double content_3 = pseudo3->GetBinContent(i);
      double error_bin_3 = pseudo3->GetBinError(i);
      double bin_width_3 = pseudo3->GetBinWidth(i);
      double new_content_3 = content_3/bin_width_3;
      double new_error_3 = error_bin_3/bin_width_3;

      double content_truth_1 = pseudo1_truth->GetBinContent(i);
      double error_truth_1 = pseudo1_truth->GetBinError(i);
      double bin_width_truth_1 = pseudo1_truth->GetBinWidth(i);
      double new_content_truth_1 = content_truth_1/bin_width_truth_1;
      double new_error_truth_1 = error_truth_1/bin_width_truth_1;

      double content_truth_2 = pseudo2_truth->GetBinContent(i);
      double error_truth_2 = pseudo2_truth->GetBinError(i);
      double bin_width_truth_2 = pseudo2_truth->GetBinWidth(i);
      double new_content_truth_2 = content_truth_2/bin_width_truth_2;
      double new_error_truth_2 = error_truth_2/bin_width_truth_2;

      double content_truth_3 = pseudo3_truth->GetBinContent(i);
      double error_truth_3 = pseudo3_truth->GetBinError(i);
      double bin_width_truth_3 = pseudo3_truth->GetBinWidth(i);
      double new_content_truth_3 = content_truth_3/bin_width_truth_3;
      double new_error_truth_3 = error_truth_3/bin_width_truth_3;

      pseudo1_truth->SetBinContent(i, new_content_truth_1);
      pseudo1_truth->SetBinError(i, new_error_truth_1);
      pseudo2_truth->SetBinContent(i, new_content_truth_2);
      pseudo2_truth->SetBinError(i, new_error_truth_2);
      pseudo3_truth->SetBinContent(i, new_content_truth_3);
      pseudo3_truth->SetBinError(i, new_error_truth_3);
      pseudo1->SetBinContent(i, new_content_1);
      pseudo1->SetBinError(i, new_error_1);
      pseudo2->SetBinContent(i, new_content_2);
      pseudo2->SetBinError(i, new_error_2);
      pseudo3->SetBinContent(i, new_content_3);
      pseudo3->SetBinError(i, new_error_3);
    }
  }
  TH1D* dummy = (TH1D*) pseudo1_truth->Clone("dummy");
  dummy->Add(pseudo2_truth);
  dummy->Add(pseudo3_truth);
  dummy->Scale(1./3);

  const Int_t n = dummy->GetNbinsX();
  Double_t x[n];
  Double_t y[n];
  for(int i = 0; i < n; i++){
    x[i] = dummy->GetBinCenter(i+1);
    y[i] = dummy->GetBinContent(i+1);
  }
  TGraphAsymmErrors* gr = new TGraphAsymmErrors(n, x, y);
  for(int i = 1; i <= n; i++){
    double t1 = pseudo1_truth->GetBinContent(i);
    double t2 = pseudo2_truth->GetBinContent(i);
    double t3 = pseudo3_truth->GetBinContent(i);
    double error_high = 0;
    double error_low = 0;

    if(t1 > t2 && t1 > t3) error_high = t1 - dummy->GetBinContent(i);
    else if(t2 > t1 && t2 > t3) error_high = t2 - dummy->GetBinContent(i);
    else if(t3 > t1 && t3 > t2) error_high = t3 - dummy->GetBinContent(i);

    if(t1 < t2 && t1 < t3) error_low = dummy->GetBinContent(i) - t1;
    else if(t2 < t1 && t2 < t3) error_low = dummy->GetBinContent(i) - t2;
    else if(t3 < t1 && t3 < t2) error_low = dummy->GetBinContent(i) - t3;

    gr->SetPointEXlow(i-1, dummy->GetBinCenter(i) - dummy->GetXaxis()->GetBinLowEdge(i));
    gr->SetPointEXhigh(i-1, dummy->GetXaxis()->GetBinUpEdge(i) - dummy->GetBinCenter(i));
    gr->SetPointEYlow(i-1, error_low);
    gr->SetPointEYhigh(i-1, error_high);
  }

  int nbins_new = n*3+1;
  Double_t check[n];
  Double_t bin_width[nbins_new];
  for(int i = 0; i <= n; i++){
    if(i == 0) check[i] = 0;
    else {
      check[i] = check[i-1] + dummy->GetBinWidth(i);
    }
  }
  for(int i = 1; i <= n; i++){
    for(int j = (3*i-2); j <= 3*i; j++){
      if(j == 1) bin_width[j-1] = 0;
      bin_width[j] = bin_width[j-1] + dummy->GetBinWidth(i)/3;
    }
  }

  TH1D* unfold1 = new TH1D("pseudo1", "pseudo1", nbins_new-1, bin_width);
  TH1D* unfold2 = new TH1D("pseudo2", "pseudo2", nbins_new-1, bin_width);
  TH1D* unfold3 = new TH1D("pseudo3", "pseudo3", nbins_new-1, bin_width);

  for(int i = 1; i <= n; i++){
    unfold1->SetBinContent((3*i-2), pseudo1->GetBinContent(i));
    unfold1->SetBinError((3*i-2), pseudo1->GetBinError(i));
    unfold2->SetBinContent((3*i-1), pseudo2->GetBinContent(i));
    unfold2->SetBinError((3*i-1), pseudo2->GetBinError(i));
    unfold3->SetBinContent((3*i), pseudo3->GetBinContent(i));
    unfold3->SetBinError((3*i), pseudo3->GetBinError(i));
  }

  int max_bin1 = unfold1->GetMaximumBin();
  int max_bin2 = unfold2->GetMaximumBin();
  int max_bin3 = unfold3->GetMaximumBin();
  double max_count1 = unfold1->GetBinContent(max_bin1);
  double max_count2 = unfold2->GetBinContent(max_bin2);
  double max_count3 = unfold3->GetBinContent(max_bin3);
  double max_range = 0;
  if(max_count1 > max_count2 && max_count1 > max_count3) max_range = max_count1 *1.1;
  else if(max_count2 > max_count1 && max_count2 > max_count3) max_range = max_count2 *1.1;
  else if(max_count3 > max_count1 && max_count3 > max_count2) max_range = max_count3 *1.1;

  TCanvas* c1 = new TCanvas("test", "tests", 600, 600);
  c1->cd();
  gStyle->SetPadLeftMargin(0.19);
  gStyle->SetPadRightMargin(0.01);
  gStyle->SetPadTopMargin(0.045);
  // gStyle->SetPadBottomMargin(0.1);

  // gStyle->UseCurrentStyle();
  c1->UseCurrentStyle();

  gr->SetTitle("");
  // gr->GetXaxis()->SetLabelSize(0.04);
  // gr->GetYaxis()->SetLabelSize(0.04);
  gr->GetXaxis()->SetRangeUser(0, 1);
  if(cs) gr->GetYaxis()->SetRangeUser(0, 400);
  else gr->GetYaxis()->SetRangeUser(0, max_range);
  gr->GetXaxis()->SetTitle("#tau_{3/2}");
  // gr->GetYaxis()->SetTitle("#frac{Events}{Bin}");
  gr->GetYaxis()->SetTitle("#frac{d#sigma}{d#tau_{32}} [fb]");
  gr->GetYaxis()->SetTitleSize(0.05);
  gr->GetYaxis()->SetLabelSize(0.04);
  gr->GetYaxis()->SetTitleOffset(1.6);
  gr->GetXaxis()->SetTitleSize(0.05);
  gr->GetXaxis()->SetLabelSize(0.04);
  gr->GetXaxis()->SetTitleOffset(0.9);
  gr->SetFillColor(kRed);
  gr->SetLineColor(kRed);
  gr->SetMarkerColor(kRed);


  gr->Draw("AP e2");
  unfold1->SetLineColor(kBlack);
  unfold1->SetMarkerColor(kBlack);
  unfold1->SetMarkerStyle(20);
  unfold1->SetMarkerSize(1.5);
  unfold1->Draw("same");
  unfold2->SetLineColor(kBlack);
  unfold2->SetMarkerColor(kBlack);
  unfold2->SetMarkerStyle(21);
  unfold2->SetMarkerSize(1.5);
  unfold2->Draw("same");
  unfold3->SetLineColor(kBlack);
  unfold3->SetMarkerColor(kBlack);
  unfold3->SetMarkerStyle(22);
  unfold3->SetMarkerSize(1.5);
  unfold3->Draw("same");

  TLegend *t1 = new TLegend(0.6,0.72,0.9,0.92);
  t1->SetBorderSize(0);
  t1->SetFillStyle(0);
  t1->AddEntry(gr, "Pseudo data Truth", "lp");
  t1->AddEntry(unfold1, "Pseudo data 1", "lp");
  t1->AddEntry(unfold2, "Pseudo data 2", "lp");
  t1->AddEntry(unfold3, "Pseudo data 3", "lp");
  t1->SetTextSize(0.04);
  t1->Draw();
  text(c1, false);
  c1->SaveAs(directory+"all_pseudo.eps");
  delete c1;
}




void Plotter::Plot_compatibility(TH1* mu_dummy, TH1* mu_sys_dummy, TH1* ele_dummy, TH1* ele_sys_dummy, TString directory){
  TH1* mu = (TH1*) mu_dummy->Clone();
  TH1* mu_sys = (TH1*) mu_sys_dummy->Clone();
  TH1* ele = (TH1*) ele_dummy->Clone();
  TH1* ele_sys = (TH1*) ele_sys_dummy->Clone();

  TCanvas* c = new TCanvas("Compatibility", "" , 600, 830);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadLeftMargin(0.11);
  c->UseCurrentStyle();
  c->cd();

  mu->SetMarkerColor(kRed);
  mu->SetLineColor(kRed);
  mu_sys->SetMarkerColor(kRed);
  mu_sys->SetLineColor(kRed);


  ele->SetFillColor(kGray+1);
  ele->SetMarkerStyle(10);
  ele_sys->SetFillColor(kGray);
  ele_sys->SetTitle("");
  ele_sys->GetYaxis()->SetTitle("Events");
  ele_sys->GetYaxis()->SetTitleSize(0.05);
  ele_sys->GetYaxis()->SetTitleOffset(1.6);
  ele_sys->GetXaxis()->SetTitleSize(0.06);
  ele_sys->GetXaxis()->SetTitleOffset(0.9);
  ele_sys->Draw("same e2");
  ele->Draw("same e2");
  mu->Draw("same e1");
  mu_sys->Draw("same e1");
  gPad->RedrawAxis();

  TLegend *l = new TLegend(0.2, 0.62, 0.5, 0.82, "");
  l->SetBorderSize(0);
  l->SetFillStyle(0);
  l->AddEntry(mu_sys,  "muon channel", "lp");
  l->AddEntry(ele_sys, "ele channel", "lp");
  l->Draw();

  c->SaveAs(directory+"Compatibility.eps");
  delete c;
}

void text(TCanvas* c, bool over_canvas){
  if(!over_canvas){

    double lumi = 35.9;
    TString infotext = TString::Format("%3.1f fb^{-1} (13 TeV)", lumi);
    TLatex *text1 = new TLatex(3.5, 24, infotext);
    text1->SetNDC();
    text1->SetTextAlign(33);
    text1->SetTextFont(42);
    text1->SetTextSize(0.6*c->GetTopMargin());
    text1->SetX(0.99);
    text1->SetY(0.987);
    text1->Draw();

    TString cmstext = "CMS";
    TLatex *text = new TLatex(3.5, 24, cmstext);
    text->SetNDC();
    text->SetTextAlign(13);
    text->SetX(0.23);      //0.2 -> outside
    text->SetTextFont(61);
    text->SetTextSize(0.75*1.5*c->GetTopMargin());
    text->SetY(0.92); //0.99 -> outside
    text->Draw();

    TString cmstext2 = "Simulation";
    TLatex *text2 = new TLatex(3.5, 24, cmstext2);
    text2->SetNDC();
    text2->SetTextAlign(13);
    text2->SetX(0.225); // 0.27 -> outside
    text2->SetTextFont(52);
    text2->SetTextSize(0.76*1.5*0.75*c->GetTopMargin());
    text2->SetY(0.865); //0.982 -> outside
    text2->Draw();

    TString cmstext3 = "work in progress";
    TLatex *text3 = new TLatex(3.5, 24, cmstext3);
    text3->SetNDC();
    text3->SetTextAlign(13);
    text3->SetX(0.225); // 0.27 -> outside
    text3->SetTextFont(52);
    text3->SetTextSize(0.76*1.5*0.75*c->GetTopMargin());
    text3->SetY(0.82); //0.982 -> outside
    text3->Draw();
  }
  else{

    double lumi = 35.9;
    TString infotext = TString::Format("%3.1f fb^{-1} (13 TeV)", lumi);
    TLatex *text1 = new TLatex(3.5, 24, infotext);
    text1->SetNDC();
    text1->SetTextAlign(33);
    text1->SetTextFont(42);
    text1->SetTextSize(0.6*c->GetTopMargin());
    text1->SetX(0.89);
    text1->SetY(0.982);
    text1->Draw();

    TString cmstext = "CMS";
    TLatex *text = new TLatex(3.5, 24, cmstext);
    text->SetNDC();
    text->SetTextAlign(13);
    text->SetX(0.13);      //0.2 -> outside
    text->SetTextFont(61);
    text->SetTextSize(0.75*c->GetTopMargin());
    text->SetY(0.99); //0.99 -> outside
    text->Draw();

    TString cmstext2 = "Simulation work in progress";
    TLatex *text2 = new TLatex(3.5, 24, cmstext2);
    text2->SetNDC();
    text2->SetTextAlign(13);
    text2->SetX(0.21); // 0.27 -> outside
    text2->SetTextFont(52);
    text2->SetTextSize(0.76*0.75*c->GetTopMargin());
    text2->SetY(0.981); //0.982 -> outside
    text2->Draw();
  }
}
