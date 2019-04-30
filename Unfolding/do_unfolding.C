#include "do_unfolding.h"

using namespace std;
int counter = 1;
int main(int argc, char* argv[]){
  // switch on histogram errors
  TH1::SetDefaultSumw2();

  string filename;
  filename = "Unfolding.root";
  outputFile = new TFile(filename.c_str(),"recreate");

  binning_xml = "Binning.xml";

  Int_t error = parser.ParseFile(binning_xml);
  if(error) cout << "error = " << error << " from TDOMParser\n";
  TXMLDocument const *XMLdocument = parser.GetXMLDocument();
  binning_rec = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_rec");
  binning_gen = TUnfoldBinningXML::ImportXML(XMLdocument, "binning_gen");
  binning_rec->Write();
  binning_gen->Write();

  /*
  ███████ ██ ██      ██          ██   ██ ██ ███████ ████████  ██████   ██████  ██████   █████  ███    ███ ███████
  ██      ██ ██      ██          ██   ██ ██ ██         ██    ██    ██ ██       ██   ██ ██   ██ ████  ████ ██
  █████   ██ ██      ██          ███████ ██ ███████    ██    ██    ██ ██   ███ ██████  ███████ ██ ████ ██ ███████
  ██      ██ ██      ██          ██   ██ ██      ██    ██    ██    ██ ██    ██ ██   ██ ██   ██ ██  ██  ██      ██
  ██      ██ ███████ ███████     ██   ██ ██ ███████    ██     ██████   ██████  ██   ██ ██   ██ ██      ██ ███████
  */

  // define directory
  TString dir = "/nfs/dust/cms/user/skottkej/CMSSW_10_2_X_v1/CMSSW_10_2_10/src/UHH2/TopSubstructure/Unfolding/";
  TFile *data_File = new TFile(dir + "Histograms.root");

  // fill background hists -- add further or remove unnecassary backgrounds in background_names
  background_names = {"DYJets", "QCD", "ST", "WJets", "Diboson_WW", "Diboson_WZ", "Diboson_ZZ"};

  // Setup everything for unfolding!
  if(argc < 3){
    throw runtime_error("Use: ./do_unfolding <dataset> <number of scans>");
  }
  else if(argv[1] == std::string("pseudo1")){
    data_File->GetObject("ttbar_matrix_1", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_1", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_1", h_truth);
    data_File->GetObject("TTbar_gen_1", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_1", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_1", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_1", h_purity_samebin);
  }
  else if(argv[1] == std::string("pseudo2")){
    data_File->GetObject("ttbar_matrix_2", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_2", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_2", h_truth);
    data_File->GetObject("TTbar_gen_2", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_2", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_2", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_2", h_purity_samebin);
  }
  else if(argv[1] == std::string("pseudo3")){
    data_File->GetObject("ttbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("pseudodata_3", h_data);                     // fill histogram with data which should be unfolded
    data_File->GetObject("pseudodata_truth_3", h_truth);              // fill histogram with truth
    data_File->GetObject("TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_3", h_purity_samebin);
  }
  else if(argv[1] == std::string("data")){
    data_File->GetObject("ttbar_matrix_3", mat_response);             // fill response matrix
    data_File->GetObject("data", h_data);                             // fill histogram with data which should be unfolded
    data_File->GetObject("TTbar_gen_3", h_unfold);                    // fill histogram with what data should be unfolded
    data_File->GetObject("TTbar_truth_3", h_truth);                   // fill histogram with truth
    data_File->GetObject("TTbar_rec_3", h_mc);                        // fill histogram to check if ratio between data and mc is appropiate
    data_File->GetObject("TTbar_purity_all_3", h_purity_all);
    data_File->GetObject("TTbar_purity_samebin_3", h_purity_samebin);
  }
  else{
    throw runtime_error("use pseudo1, pseudo2 or pseudo3");
  }



  for(unsigned int i = 0; i < background_names.size(); i++){
    background.push_back((TH1D*) data_File->Get("background_rec_" + background_names.at(i)));
  }
  subtract_background = false;                                      // should the background be subtracted?
  TString save_dir = "/afs/desy.de/user/s/skottkej/Plots/Unfolding/";
  TString dataset = argv[1];
  TString format = ".eps";
  TString regmode = "size";
  TString density_flag = "none";
  TString nscan_word = argv[2];
  int nscan = atoi(argv[2]);
  bool do_lcurve = true;
  bool do_lcurve_2 = false;

  // TString save_projection_x_as = "LCurve_projection_x";
  // save_projection_x_as += regmode;
  // save_projection_x_as += density_flag;

  TString save_LCurve_projection_x = "LCurve_projection_X_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_projection_y = "LCurve_projection_Y_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_Unfolding = "LCurve_Unfolding_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_Unfolding_correlation = "LCurve_Unfolding_correlation_" + dataset + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_TauX = "LCurve_TauX_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_TauY = "LCurve_TauY_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_LCurve_TauScan = "LCurve_TauScan_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;
  TString save_TauScan_RhoLogTau = "TauScan_RhoLogTau_" + dataset + "_" + regmode + "_" + density_flag + "_" + nscan_word + format;

  unfolding unfold(h_data, h_mc, mat_response, h_truth, h_mc, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve, subtract_background);

  unfolded_data = unfold.get_output();
  correlation_matrix = unfold.get_correlation();
  unfolded_mc_test = unfold.get_output_check();
  lcurve = unfold.get_lcurve();
  coordinates = unfold.get_coordinates();
  tau = unfold.get_tau();
  logTau = unfold.get_logtau();
  rhologTau = unfold.get_rhologtau();
  std::vector<TH1D*> projections = unfold.check_projection();

  outputFile->cd();
  TCanvas* c_projection = new TCanvas("Projection of " + dataset, "Projections", 1000, 800);
  gStyle->SetOptStat(0);
  c_projection->Divide(1,2);
  c_projection->cd(1);
  projections.at(0)->GetYaxis()->SetTitle("events");
  projections.at(0)->GetXaxis()->SetTitle("gen_binning");
  projections.at(0)->DrawCopy();
  h_unfold->SetLineColor(kRed);
  h_unfold->DrawCopy("same");
  c_projection->cd(1)->SaveAs(save_dir + save_LCurve_projection_x);

  c_projection->cd(2);
  projections.at(1)->GetYaxis()->SetTitle("events");
  projections.at(1)->GetXaxis()->SetTitle("rec_binning");
  projections.at(1)->DrawCopy();
  h_mc->SetLineColor(kRed);
  h_mc->DrawCopy("same");
  c_projection->cd(2)->SaveAs(save_dir + save_LCurve_projection_y);
  c_projection->Write();
  delete c_projection;

  TCanvas* c_unfolding = new TCanvas("Unfolding of " + dataset, "Unfolded data", 1000, 800);
  gStyle->SetOptStat(0);
  c_unfolding->Divide(1,2);
  c_unfolding->cd(1);
  // unfolded_data->GetYaxis()->SetRangeUser(-2000, 2000);
  unfolded_data->SetLineColor(kRed);
  unfolded_data->GetYaxis()->SetTitle("events");
  unfolded_data->SetTitle("");
  unfolded_data->DrawCopy();
  h_truth->DrawCopy("same hist");
  TLegend *legend1 = new TLegend(0.15, 0.22, 0.65, 0.42, "");
  legend1->SetBorderSize(0);
  legend1->SetFillStyle(0);
  legend1->AddEntry(unfolded_data, "unfolded " + dataset,"l");
  legend1->AddEntry(h_truth, "truth", "l");
  legend1->Draw();
  c_unfolding->cd(1)->SaveAs(save_dir + save_LCurve_Unfolding);

  c_unfolding->cd(2);
  correlation_matrix->GetYaxis()->SetTitle("#tau_{3/2}");
  correlation_matrix->Draw("colz");
  c_unfolding->cd(2)->SaveAs(save_dir + save_LCurve_Unfolding_correlation);
  c_unfolding->Write();
  delete c_unfolding;

  TCanvas* c_lcurve = new TCanvas("LCurve ", "LCurve of unfolding", 600, 1200);
  c_lcurve->Divide(1, 3);
  gStyle->SetOptStat(0);

  c_lcurve->cd(1);
  p2=new TMarker(TMath::Log10(tau), coordinates.at(0), 20);

  double logtau = TMath::Log10(tau);
  std::vector<double> Xpoint_x = {logtau};
  std::vector<double> Ypoint_x = {coordinates.at(0)};
  TGraph * point_x = new TGraph(1, &Xpoint_x[0], &Ypoint_x[0]);
  point_x->SetTitle("");
  point_x->GetXaxis()->SetTitle("log #tau");
  point_x->GetYaxis()->SetTitle("L_{X}");
  point_x->GetYaxis()->SetTitleOffset(1.5);
  point_x->GetXaxis()->SetNdivisions(510);
  point_x->GetYaxis()->SetNdivisions(810);
  point_x->SetMarkerStyle(20);
  point_x->SetMarkerSize(1.5);
  point_x->SetLineColor(1);
  point_x->Draw("AP");
  point_x->GetXaxis()->SetLimits(-6.0, 0.0);
  double ymin_x = Ypoint_x[0]*0.99;
  double ymax_x = Ypoint_x[0]*1.08;
  point_x->GetYaxis()->SetRangeUser(ymin_x, ymax_x);
  point_x->DrawClone("AP");
  c_lcurve->cd(1)->Update();
  logTau.at(0)->SetLineColor(kRed);
  logTau.at(0)->DrawClone("same");
  point_x->DrawClone("psame");
  TLegend *legend_logtaux = new TLegend(0.15, 0.22, 0.65, 0.42, "");
  legend_logtaux->SetBorderSize(0);
  legend_logtaux->SetFillStyle(0);
  legend_logtaux->AddEntry(logTau.at(0), "unfolded " + dataset,"l");
  legend_logtaux->Draw();
  c_lcurve->cd(1)->SaveAs(save_dir + save_LCurve_TauX);

  c_lcurve->cd(2);
  std::vector<double> Xpoint_y = {logtau};
  std::vector<double> Ypoint_y = {coordinates.at(1)};
  TGraph * point_y = new TGraph(1, &Xpoint_y[0], &Ypoint_y[0]);
  point_y->SetTitle("");
  point_y->GetXaxis()->SetTitle("log #tau");
  point_y->GetYaxis()->SetTitle("L_{Y}");
  point_y->GetYaxis()->SetTitleOffset(1.5);
  point_y->GetXaxis()->SetNdivisions(510);
  point_y->GetYaxis()->SetNdivisions(810);
  point_y->SetMarkerStyle(20);
  point_y->SetMarkerSize(1.5);
  point_y->SetLineColor(1);
  point_y->Draw("AP");
  point_y->GetXaxis()->SetLimits(-6.0, 0.0);
  double ymin_y = Ypoint_y[0]*-0.7;
  double ymax_y = Ypoint_y[0]*1.08;
  point_y->GetYaxis()->SetRangeUser(ymin_y, ymax_y);
  point_y->DrawClone("AP");
  c_lcurve->cd(2)->Update();
  logTau.at(1)->SetLineColor(kRed);
  logTau.at(1)->DrawClone("same");
  point_y->DrawClone("psame");
  TLegend *legend_logtauy = new TLegend(0.15, 0.22, 0.65, 0.42, "");
  legend_logtauy->SetBorderSize(0);
  legend_logtauy->SetFillStyle(0);
  legend_logtauy->AddEntry(logTau.at(1), "unfolded " + dataset,"l");
  legend_logtauy->Draw();
  c_lcurve->cd(2)->SaveAs(save_dir + save_LCurve_TauY);

  c_lcurve->Write();
  delete c_lcurve;



  unfolding unfold2(h_data, h_mc, mat_response, h_truth, h_mc, binning_gen, binning_rec, background, background_names, nscan, regmode, density_flag, do_lcurve_2, subtract_background);

  coordinates_2 = unfold2.get_coordinates();
  tau_2 = unfold2.get_tau();
  logTau_2 = unfold2.get_logtau();
  rhologTau_2 = unfold2.get_rhologtau();

  TCanvas* c_unfolding_2 = new TCanvas("Unfolding2 ", "Unfolded data", 1000, 1000);
  gStyle->SetOptStat(0);
  c_unfolding_2->Divide(1,2);
  c_unfolding_2->cd(1);
  // unfolded_data_tau->GetYaxis()->SetRangeUser(-2000, 2000);
  LCurve_tau = new TMarker(coordinates.at(0), coordinates.at(1), 10);
  LCurve_tau->SetMarkerColor(kRed);
  TauScan_tau = new TMarker(coordinates_2.at(0), coordinates_2.at(1), 20);
  TauScan_tau->SetMarkerColor(kBlue);
  lcurve->GetYaxis()->SetRangeUser(0, 6);
  lcurve->GetYaxis()->SetTitle("L_{Y}");
  lcurve->GetXaxis()->SetTitle("L_{X}");
  lcurve->DrawClone();
  LCurve_tau->DrawClone("same");
  TauScan_tau->DrawClone("same");
  TLegend *legend_lcurve = new TLegend(0.15, 0.22, 0.65, 0.42, "");
  legend_lcurve->SetBorderSize(0);
  legend_lcurve->SetFillStyle(0);
  legend_lcurve->AddEntry(lcurve, "LCurve", "l");
  legend_lcurve->AddEntry(LCurve_tau, "#tau value of LCurve", "p");
  legend_lcurve->AddEntry(TauScan_tau, "#tau value of ScanTau", "p");
  legend_lcurve->Draw();
  c_unfolding_2->cd(1)->SaveAs(save_dir + save_LCurve_TauScan);

  c_unfolding_2->cd(2);
  // p4=new TMarker(TMath::Log10(tau_2), rhologTau_2->Eval(TMath::Log10(tau_2)), 20);
  // point->GetXaxis()->SetTitle("log #tau");
  // point->GetYaxis()->SetTitle("#rho(log #tau)");
  std::vector<double> Xpoint_rhologtau = {TMath::Log10(tau_2)};
  std::vector<double> Ypoint_rhologtau = {rhologTau_2->Eval(TMath::Log10(tau_2))};
  TGraph * point_rhologtau = new TGraph(1, &Xpoint_rhologtau[0], &Ypoint_rhologtau[0]);
  point_rhologtau->SetTitle("");
  point_rhologtau->GetXaxis()->SetTitle("log #tau");
  point_rhologtau->GetYaxis()->SetTitle("#rho(log #tau)");
  point_rhologtau->GetYaxis()->SetTitleOffset(1.5);
  point_rhologtau->GetXaxis()->SetNdivisions(510);
  point_rhologtau->GetYaxis()->SetNdivisions(810);
  point_rhologtau->SetMarkerStyle(20);
  point_rhologtau->SetMarkerSize(1.5);
  point_rhologtau->SetLineColor(1);
  point_rhologtau->Draw("AP");
  point_rhologtau->GetXaxis()->SetLimits(-6.0, 0.0);
  double ymin_rhologtau = Ypoint_rhologtau[0]*0.95;
  double ymax_rhologtau = 1.15;
  point_rhologtau->GetYaxis()->SetRangeUser(ymin_rhologtau, ymax_rhologtau);
  point_rhologtau->DrawClone("AP");
  c_unfolding_2->cd(2)->Update();
  rhologTau_2->SetLineColor(kRed);
  rhologTau_2->DrawClone("same");
  point_rhologtau->DrawClone("psame");
  TLegend *legend_rhologTau = new TLegend(0.15, 0.22, 0.65, 0.42, "");
  legend_rhologTau->SetBorderSize(0);
  legend_rhologTau->SetFillStyle(0);
  legend_rhologTau->AddEntry(rhologTau_2, "unfolded " + dataset, "l");
  legend_rhologTau->Draw();
  c_unfolding_2->cd(2)->SaveAs(save_dir + save_TauScan_RhoLogTau  );
  c_unfolding_2->Write();
  delete c_unfolding_2;

  outputFile->Close();
  return 0;
}
