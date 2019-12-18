#include <TH1D.h>

void Resolution(){

TFile *ttbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/PostSelection/Muon/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3_M.root", "READ");

TH1D *resolution_1 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_1");
TH1D *resolution_1_puppi = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_1");
TH1D *resolution_2 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_2");
TH1D *resolution_2_puppi = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_2");
TH1D *resolution_3 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_3");
TH1D *resolution_3_puppi = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_3");
TH1D *resolution_4 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_4");
TH1D *resolution_4_puppi = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_4");

TH1D *resolution_1_pu_20 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_1_pu_20");
TH1D *resolution_1_puppi_pu_20 = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_1_pu_20");
TH1D *resolution_2_pu_20 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_2_pu_20");
TH1D *resolution_2_puppi_pu_20 = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_2_pu_20");
TH1D *resolution_3_pu_20 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_3_pu_20");
TH1D *resolution_3_puppi_pu_20 = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_3_pu_20");
TH1D *resolution_4_pu_20 = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_4_pu_20");
TH1D *resolution_4_puppi_pu_20 = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_4_pu_20");

TH1D *resolution_1_pu_inft = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_1_pu_inft");
TH1D *resolution_1_puppi_pu_inft = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_1_pu_inft");
TH1D *resolution_2_pu_inft = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_2_pu_inft");
TH1D *resolution_2_puppi_pu_inft = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_2_pu_inft");
TH1D *resolution_3_pu_inft = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_3_pu_inft");
TH1D *resolution_3_puppi_pu_inft = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_3_pu_inft");
TH1D *resolution_4_pu_inft = (TH1D*) ttbar->Get("Resolution_Hist/Resolution_4_pu_inft");
TH1D *resolution_4_puppi_pu_inft = (TH1D*) ttbar->Get("ResolutionPuppi_Hist/Resolution_4_pu_inft");

gStyle->SetPadLeftMargin(0.13);
gStyle->SetPadRightMargin(0.05);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
gStyle->SetLegendBorderSize(0);

// TCanvas *c_reso = new TCanvas("res1","res_1",1000,1000);
// c_reso->cd();
resolution_1->SetTitle("");
resolution_1->GetYaxis()->SetTitle("Events");
resolution_1->GetXaxis()->SetTitle("#tau_{32}^{rec}/#tau_{32}^{gen}");
TFitResultPtr r1 = resolution_1->Fit("gaus", "SN", "", 0.7, 2.6);
double FWHM1 = 2*sqrt(2*log10(2))*r1->Parameter(2);
double sigma1 = r1->Parameter(2);
double sigma1_err = r1->ParError(2);

// TF1* fit_func = new TF1("fit_func", "gaus");
// fit_func->SetParameters(r1->Parameter(0), r1->Parameter(1), r1->Parameter(2));
// TF1* fit_func=(TF1*)resolution_1->GetFunction("gaus");
// double y1=fit_func->Eval(r1->Parameter(1)-sigma1);
// double y2=fit_func->Eval(r1->Parameter(1)+sigma1);

// TLine* line = new TLine(r1->Parameter(1)-sigma1,y1,r1->Parameter(1)+sigma1,y2);
// line->SetLineColor(kRed);
// line->Draw("same");
// c_reso->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/Example_reso.pdf");


TFitResultPtr r2 = resolution_2->Fit("gaus", "SN", "", 0.7, 1.8);
// double FWHM2 = 2*sqrt(2*log10(2))*r2->Parameter(2);
double sigma2 = r2->Parameter(2);
double sigma2_err = r2->ParError(2);

TFitResultPtr r3 = resolution_3->Fit("gaus", "SN", "", 0.7, 1.5);
// double FWHM3 = 2*sqrt(2*log10(2))*r3->Parameter(2);
double sigma3 = r3->Parameter(2);
double sigma3_err = r3->ParError(2);

TFitResultPtr r4 = resolution_4->Fit("gaus", "SN", "", 0.5, 1.4);
// double FWHM4 = 2*sqrt(2*log10(2))*r4->Parameter(2);
double sigma4 = r4->Parameter(2);
double sigma4_err = r4->ParError(2);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////                                        PUPPI                      //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TFitResultPtr r1_puppi = resolution_1_puppi->Fit("gaus", "SQN", "", 0.5, 2.6);
// double FWHM1_puppi = 2*sqrt(2*log10(2))*r1_puppi->Parameter(2);
double sigma1_puppi = r1_puppi->Parameter(2);
double sigma1_puppi_err = r1_puppi->ParError(2);

TCanvas *c_reso4 = new TCanvas("res4","res_4",1000,1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.11);
gStyle->SetPadLeftMargin(0.105);
gStyle->SetPadRightMargin(0.015);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
c_reso4->cd();
c_reso4->UseCurrentStyle();
resolution_2_puppi->SetTitle("");
resolution_2_puppi->GetYaxis()->SetTitle("Events");
resolution_2_puppi->GetXaxis()->SetTitle("#tau_{32}^{rec}/#tau_{32}^{gen}");
resolution_2_puppi->SetLineWidth(3);
resolution_2_puppi->SetTitle("");
resolution_2_puppi->GetYaxis()->SetLabelSize(0.04);
resolution_2_puppi->GetYaxis()->SetTitleSize(0.045);
resolution_2_puppi->GetYaxis()->SetTitleOffset(1.2);
resolution_2_puppi->GetXaxis()->SetLabelSize(0.04);
resolution_2_puppi->GetXaxis()->SetTitleSize(0.045);
resolution_2_puppi->GetXaxis()->SetTitleOffset(1.15);
resolution_2_puppi->GetYaxis()->SetRangeUser(0.0001, 120);
resolution_2_puppi->SetLineColor(kBlack);
resolution_2_puppi->SetMarkerColor(kBlack);
resolution_2_puppi->Draw();
TFitResultPtr r2_puppi = resolution_2_puppi->Fit("gaus", "SQN", "", 0.5, 2);
// double FWHM2_puppi = 2*sqrt(2*log10(2))*r2_puppi->Parameter(2);
double sigma2_puppi = r2_puppi->Parameter(2);
double sigma2_puppi_err = r2_puppi->ParError(2);

TF1* fit_func2 = new TF1("fit_func2", "gaus",0.5,2);
fit_func2->SetParameters(r2_puppi->Parameter(0), r2_puppi->Parameter(1), r2_puppi->Parameter(2));
// TF1* fit_func=(TF1*)resolution_1->GetFunction("gaus");
double y12=fit_func2->Eval(r2_puppi->Parameter(1)-sigma2_puppi);
double y22=fit_func2->Eval(r2_puppi->Parameter(1)+sigma2_puppi);
double y3=fit_func2->Eval(r2_puppi->Parameter(1));

TLine* line2 = new TLine(r2_puppi->Parameter(1)-sigma2_puppi,y12,r2_puppi->Parameter(1)+sigma2_puppi,y22);
line2->SetLineColor(kBlue);
line2->SetLineWidth(3);
line2->SetLineStyle(9);
line2->Draw("same");
fit_func2->SetLineWidth(3);
fit_func2->SetLineColor(kBlue);
fit_func2->SetMarkerColor(kBlue);
fit_func2->Draw("same");
TLine* line3 = new TLine(r2_puppi->Parameter(1),0,r2_puppi->Parameter(1),y3);
line3->SetLineColor(kBlue);
line3->SetLineWidth(3);
line3->SetLineStyle(9);
line3->Draw("same");
TArrow *ar5 = new TArrow(r2_puppi->Parameter(1)-sigma2_puppi+0.006,y12+1.5,r2_puppi->Parameter(1)-0.006,y22+1.5,0.005,"<|>");
ar5->SetAngle(60);
ar5->SetLineWidth(4);
ar5->SetLineColor(kBlue);
// ar5->SetFillStyle(3008);
ar5->SetFillColor(kBlue);
ar5->Draw();
TLatex *text2 = new TLatex(r2_puppi->Parameter(1)-(r2_puppi->Parameter(1)-sigma2_puppi/2),y12+1.5,"#sigma");
text2->SetNDC();
text2->SetTextAlign(13);
// text2->SetX(r2_puppi->Parameter(1)-sigma2_puppi); // 0.27 -> outside
text2->SetX(0.362); // 0.27 -> outside
text2->SetTextColor(kBlue);
text2->SetTextFont(52);
text2->SetTextSize(0.04);
text2->SetY(0.625); //0.982 -> outside
text2->Draw();
TLatex *text3 = new TLatex(0.393,0.09,"#mu");
text3->SetNDC();
text3->SetTextAlign(13);
text3->SetTextColor(kBlue);
// text2->SetX(r2_puppi->Parameter(1)-sigma2_puppi); // 0.27 -> outside
text3->SetTextFont(52);
text3->SetTextSize(0.04);
text3->Draw();

TLegend* legend_fit = new TLegend(0.62,0.72,0.87,0.845);
legend_fit->AddEntry(resolution_2_puppi, "t#bar{t}", "lep");
legend_fit->AddEntry(fit_func2, "Fit", "lp");
legend_fit->SetTextSize(0.038);
legend_fit->Draw();

c_reso4->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/Example_reso.pdf");




TFitResultPtr r3_puppi = resolution_3_puppi->Fit("gaus", "SQN", "", 0.5, 1.7);
// double FWHM3_puppi = 2*sqrt(2*log10(2))*r3_puppi->Parameter(2);
double sigma3_puppi = r3_puppi->Parameter(2);
double sigma3_puppi_err = r3_puppi->ParError(2);

TFitResultPtr r4_puppi = resolution_4_puppi->Fit("gaus", "SQN", "", 0.3, 1.4);
// double FWHM4_puppi = 2*sqrt(2*log10(2))*r4_puppi->Parameter(2);
double sigma4_puppi = r4_puppi->Parameter(2);
double sigma4_puppi_err = r4_puppi->ParError(2);


TCanvas* canvas2 = new TCanvas("Res","Res", 1000, 1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.11);
gStyle->SetPadLeftMargin(0.125);
gStyle->SetPadRightMargin(0.015);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
canvas2->cd();
canvas2->UseCurrentStyle();
TH1D* resolution = new TH1D("bin_width_chs","", 4,0.2, 1.0);
resolution->SetBinContent(1, 2*sigma1*0.3);
resolution->SetBinError(1, 2*sigma1_err*0.3);
resolution->SetBinContent(2, 2*sigma2*0.5);
resolution->SetBinError(2, 2*sigma2_err*0.5);
resolution->SetBinContent(3, 2*sigma3*0.7);
resolution->SetBinError(3, 2*sigma3_err*0.7);
resolution->SetBinContent(4, 2*sigma4*0.9);
resolution->SetBinError(4, 2*sigma4_err*0.9);
resolution->SetLineColor(kBlue);
resolution->SetMarkerColor(kBlue);

resolution->GetYaxis()->SetRangeUser(0.00001, 0.16);
resolution->GetYaxis()->SetTitle("bin width");
resolution->GetXaxis()->SetTitle("#tau_{32}^{gen}");
resolution->SetLineWidth(3);
resolution->SetLineWidth(3);
resolution->SetTitle("");
resolution->GetYaxis()->SetLabelSize(0.04);
resolution->GetYaxis()->SetTitleSize(0.045);
resolution->GetYaxis()->SetTitleOffset(1.5);
resolution->GetXaxis()->SetLabelSize(0.04);
resolution->GetXaxis()->SetTitleSize(0.05);
resolution->GetXaxis()->SetTitleOffset(1.0);
// resolution->GetYaxis()->SetRangeUser(0.0001, 120);
resolution->Draw();

TH1D* resolution_puppi = new TH1D("bin_width_puppi","", 4,0.2, 1.0);
resolution_puppi->SetBinContent(1, 2*sigma1_puppi*0.3);
resolution_puppi->SetBinError(1, 2*sigma1_puppi_err*0.3);
resolution_puppi->SetBinContent(2, 2*sigma2_puppi*0.5);
resolution_puppi->SetBinError(2, 2*sigma2_puppi_err*0.5);
resolution_puppi->SetBinContent(3, 2*sigma3_puppi*0.7);
resolution_puppi->SetBinError(3, 2*sigma3_puppi_err*0.7);
resolution_puppi->SetBinContent(4, 2*sigma4_puppi*0.9);
resolution_puppi->SetBinError(4, 2*sigma4_puppi_err*0.9);
resolution_puppi->SetLineColor(kRed);
resolution_puppi->SetMarkerColor(kRed);
resolution_puppi->SetLineWidth(3);
resolution_puppi->Draw("same");

TLegend *legend_bin = new TLegend(0.52,0.73,0.77,0.855);
legend_bin->AddEntry(resolution,"CHS","le");
legend_bin->AddEntry(resolution_puppi,"PUPPI","le");
legend_bin->Draw("same");
canvas2->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/Binwidth.pdf");

TCanvas* canvas4 = new TCanvas("Resolution_sum","Resolution_sum", 1000, 1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.11);
gStyle->SetPadLeftMargin(0.125);
gStyle->SetPadRightMargin(0.015);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
canvas4->cd();
canvas4->UseCurrentStyle();
TH1D* reso_puppi = new TH1D("Resolution_puppi2","", 4,0.2, 1.0);
reso_puppi->SetBinContent(1, 2*sigma1_puppi);
reso_puppi->SetBinError(1, 2*sigma1_puppi_err);
reso_puppi->SetBinContent(2, 2*sigma2_puppi);
reso_puppi->SetBinError(2, 2*sigma2_puppi_err);
reso_puppi->SetBinContent(3, 2*sigma3_puppi);
reso_puppi->SetBinError(3, 2*sigma3_puppi_err);
reso_puppi->SetBinContent(4, 2*sigma4_puppi);
reso_puppi->SetBinError(4, 2*sigma4_puppi_err);

reso_puppi->GetYaxis()->SetRangeUser(0.00001,0.35);
reso_puppi->GetYaxis()->SetTitle("resolution");
reso_puppi->GetXaxis()->SetTitle("#tau_{32}^{gen}");
reso_puppi->SetTitle("");
reso_puppi->GetYaxis()->SetLabelSize(0.04);
reso_puppi->GetYaxis()->SetTitleSize(0.045);
reso_puppi->GetYaxis()->SetTitleOffset(1.5);
reso_puppi->GetXaxis()->SetLabelSize(0.04);
reso_puppi->GetXaxis()->SetTitleSize(0.05);
reso_puppi->GetXaxis()->SetTitleOffset(1.0);
reso_puppi->SetLineColor(kRed);
reso_puppi->SetMarkerColor(kRed);
reso_puppi->SetLineWidth(3);
reso_puppi->Draw();
TH1D* reso = new TH1D("Reso","", 4,0.2, 1.0);
reso->SetBinContent(1, 2*sigma1);
reso->SetBinError(1, 2*sigma1_err);
reso->SetBinContent(2, 2*sigma2);
reso->SetBinError(2, 2*sigma2_err);
reso->SetBinContent(3, 2*sigma3);
reso->SetBinError(3, 2*sigma3_err);
reso->SetBinContent(4, 2*sigma4);
reso->SetBinError(4, 2*sigma4_err);
reso->SetLineWidth(3);
reso->SetLineColor(kBlue);
reso->SetMarkerColor(kBlue);
reso->Draw("same");

TLegend *legend_reso = new TLegend(0.7,0.65,0.95,0.775);
legend_reso->AddEntry(reso,"CHS","le");
legend_reso->AddEntry(reso_puppi,"PUPPI","le");
legend_reso->Draw();
canvas4->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/Resolution.pdf");



// //////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////                 splitted by PU             //////////////////////////////
TH1D* chs_low = new TH1D("chs_low","chs_low",4, 0,1);
TH1D *chs_high = new TH1D("chs_high","chs_high",4, 0,1);
TH1D* puppi_low = new TH1D("puppi_low","puppi_low",4, 0,1);
TH1D *puppi_high = new TH1D("puppi_high","puppi_high",4, 0,1);
TFitResultPtr r1_pu_20 = resolution_1_pu_20->Fit("gaus", "Sn", "", 0.7, 2.6);
// double FWHM1_pu_20 = 2*sqrt(2*log10(2))*r1_pu_20->Parameter(2);
double sigma1_pu_20 = r1_pu_20->Parameter(2);
chs_low->SetBinContent(1,r1_pu_20->Parameter(1));
chs_low->SetBinError(1,r1_pu_20->ParError(1));


TFitResultPtr r2_pu_20 = resolution_2_pu_20->Fit("gaus", "Sn", "", 0.7, 1.8);
double FWHM2_pu_20 = 2*sqrt(2*log10(2))*r2_pu_20->Parameter(2);
double sigma2_pu_20 = r2_pu_20->Parameter(2);
chs_low->SetBinContent(2,r2_pu_20->Parameter(1));
chs_low->SetBinError(2,r2_pu_20->ParError(1));


TFitResultPtr r3_pu_20 = resolution_3_pu_20->Fit("gaus", "Sn", "", 0.7, 1.5);
double FWHM3_pu_20 = 2*sqrt(2*log10(2))*r3_pu_20->Parameter(2);
double sigma3_pu_20 = r3_pu_20->Parameter(2);
chs_low->SetBinContent(3,r3_pu_20->Parameter(1));
chs_low->SetBinError(3,r3_pu_20->ParError(1));


TFitResultPtr r4_pu_20 = resolution_4_pu_20->Fit("gaus", "Sn", "", 0.5, 1.4);
double FWHM4_pu_20 = 2*sqrt(2*log10(2))*r4_pu_20->Parameter(2);
double sigma4_pu_20 = r4_pu_20->Parameter(2);
chs_low->SetBinContent(4,r4_pu_20->Parameter(1));
chs_low->SetBinError(4,r4_pu_20->ParError(1));


// TH1D* resolution_pu_20 = new TH1D("Resolution_pu_20_1","", 4,0.2, 1.0);
// resolution_pu_20->SetBinContent(1, 2*sigma1_pu_20*0.3);
// resolution_pu_20->SetBinContent(2, 2*sigma2_pu_20*0.5);
// resolution_pu_20->SetBinContent(3, 2*sigma3_pu_20*0.7);
// resolution_pu_20->SetBinContent(4, 2*sigma4_pu_20*0.9);


TFitResultPtr r1_pu_inft = resolution_1_pu_inft->Fit("gaus", "Sn", "", 0.7, 2.6);
// double FWHM1_pu_inft = 2*sqrt(2*log10(2))*r1_pu_inft->Parameter(2);
double sigma1_pu_inft = r1_pu_inft->Parameter(2);
chs_high->SetBinContent(1,r1_pu_inft->Parameter(1));
chs_high->SetBinError(1,r1_pu_inft->ParError(1));


TFitResultPtr r2_pu_inft = resolution_2_pu_inft->Fit("gaus", "Sn", "", 0.7, 1.8);
double FWHM2_pu_inft = 2*sqrt(2*log10(2))*r2_pu_inft->Parameter(2);
double sigma2_pu_inft = r2_pu_inft->Parameter(2);
chs_high->SetBinContent(2,r2_pu_inft->Parameter(1));
chs_high->SetBinError(2,r2_pu_inft->ParError(1));


TFitResultPtr r3_pu_inft = resolution_3_pu_inft->Fit("gaus", "Sn", "", 0.7, 1.5);
double FWHM3_pu_inft = 2*sqrt(2*log10(2))*r3_pu_inft->Parameter(2);
double sigma3_pu_inft = r3_pu_inft->Parameter(2);
chs_high->SetBinContent(3,r3_pu_inft->Parameter(1));
chs_high->SetBinError(3,r3_pu_inft->ParError(1));


TFitResultPtr r4_pu_inft = resolution_4_pu_inft->Fit("gaus", "Sn", "", 0.5, 1.4);
double FWHM4_pu_inft = 2*sqrt(2*log10(2))*r4_pu_inft->Parameter(2);
double sigma4_pu_inft = r4_pu_inft->Parameter(2);
chs_high->SetBinContent(4,r4_pu_inft->Parameter(1));
chs_high->SetBinError(4,r4_pu_inft->ParError(1));


// TH1D* resolution_pu_inft = new TH1D("Resolution_pu_inft_1","", 4,0.2, 1.0);
// resolution_pu_inft->SetBinContent(1, 2*sigma1_pu_inft*0.3);
// resolution_pu_inft->SetBinContent(2, 2*sigma2_pu_inft*0.5);
// resolution_pu_inft->SetBinContent(3, 2*sigma3_pu_inft*0.7);
// resolution_pu_inft->SetBinContent(4, 2*sigma4_pu_inft*0.9);

///////////////////////////////////////////////////////////////////////////////////////////////////////

TFitResultPtr r1_puppi_pu_20 = resolution_1_puppi_pu_20->Fit("gaus", "Sn", "", 0.7, 2.6);
// double FWHM1_puppi_pu_20 = 2*sqrt(2*log10(2))*r1_puppi_pu_20->Parameter(2);
double sigma1_puppi_pu_20 = r1_puppi_pu_20->Parameter(2);
puppi_low->SetBinContent(1,r1_puppi_pu_20->Parameter(1));
puppi_low->SetBinError(1,r1_puppi_pu_20->ParError(1));


TFitResultPtr r2_puppi_pu_20 = resolution_2_puppi_pu_20->Fit("gaus", "Sn", "", 0.7, 1.8);
double FWHM2_puppi_pu_20 = 2*sqrt(2*log10(2))*r2_puppi_pu_20->Parameter(2);
double sigma2_puppi_pu_20 = r2_puppi_pu_20->Parameter(2);
puppi_low->SetBinContent(2,r2_puppi_pu_20->Parameter(1));
puppi_low->SetBinError(2,r2_puppi_pu_20->ParError(1));


TFitResultPtr r3_puppi_pu_20 = resolution_3_puppi_pu_20->Fit("gaus", "Sn", "", 0.7, 1.5);
double FWHM3_puppi_pu_20 = 2*sqrt(2*log10(2))*r3_puppi_pu_20->Parameter(2);
double sigma3_puppi_pu_20 = r3_puppi_pu_20->Parameter(2);
puppi_low->SetBinContent(3,r3_puppi_pu_20->Parameter(1));
puppi_low->SetBinError(3,r3_puppi_pu_20->ParError(1));


TFitResultPtr r4_puppi_pu_20 = resolution_4_puppi_pu_20->Fit("gaus", "Sn", "", 0.5, 1.4);
double FWHM4_puppi_pu_20 = 2*sqrt(2*log10(2))*r4_puppi_pu_20->Parameter(2);
double sigma4_puppi_pu_20 = r4_puppi_pu_20->Parameter(2);
puppi_low->SetBinContent(4,r4_puppi_pu_20->Parameter(1));
puppi_low->SetBinError(4,r4_puppi_pu_20->ParError(1));

// TH1D* resolution_puppi_pu_20 = new TH1D("Resolution_puppi_pu_20_1","", 4,0.2, 1.0);
// resolution_puppi_pu_20->SetBinContent(1, 2*sigma1_puppi_pu_20*0.3);
// resolution_puppi_pu_20->SetBinContent(2, 2*sigma2_puppi_pu_20*0.5);
// resolution_puppi_pu_20->SetBinContent(3, 2*sigma3_puppi_pu_20*0.7);
// resolution_puppi_pu_20->SetBinContent(4, 2*sigma4_puppi_pu_20*0.9);


TFitResultPtr r1_puppi_pu_inft = resolution_1_puppi_pu_inft->Fit("gaus", "Sn", "", 0.7, 2.6);
// double FWHM1_puppi_pu_inft = 2*sqrt(2*log10(2))*r1_puppi_pu_inft->Parameter(2);
double sigma1_puppi_pu_inft = r1_puppi_pu_inft->Parameter(2);
puppi_high->SetBinContent(1,r1_puppi_pu_inft->Parameter(1));
puppi_high->SetBinError(1,r1_puppi_pu_inft->ParError(1));

TFitResultPtr r2_puppi_pu_inft = resolution_2_puppi_pu_inft->Fit("gaus", "Sn", "", 0.7, 1.8);
double FWHM2_puppi_pu_inft = 2*sqrt(2*log10(2))*r2_puppi_pu_inft->Parameter(2);
double sigma2_puppi_pu_inft = r2_puppi_pu_inft->Parameter(2);
puppi_high->SetBinContent(2,r2_puppi_pu_inft->Parameter(1));
puppi_high->SetBinError(2,r2_puppi_pu_inft->ParError(1));


TFitResultPtr r3_puppi_pu_inft = resolution_3_puppi_pu_inft->Fit("gaus", "Sn", "", 0.7, 1.5);
double FWHM3_puppi_pu_inft = 2*sqrt(2*log10(2))*r3_puppi_pu_inft->Parameter(2);
double sigma3_puppi_pu_inft = r3_puppi_pu_inft->Parameter(2);
puppi_high->SetBinContent(3,r3_puppi_pu_inft->Parameter(1));
puppi_high->SetBinError(3,r3_puppi_pu_inft->ParError(1));


TFitResultPtr r4_puppi_pu_inft = resolution_4_puppi_pu_inft->Fit("gaus", "Sn", "", 0.5, 1.4);
double FWHM4_puppi_pu_inft = 2*sqrt(2*log10(2))*r4_puppi_pu_inft->Parameter(2);
double sigma4_puppi_pu_inft = r4_puppi_pu_inft->Parameter(2);
puppi_high->SetBinContent(4,r4_puppi_pu_inft->Parameter(1));
puppi_high->SetBinError(4,r4_puppi_pu_inft->ParError(1));


// TH1D* resolution_puppi_pu_inft = new TH1D("Resolution_puppi_pu_inft_1","", 4,0.2, 1.0);
// resolution_puppi_pu_inft->SetBinContent(1, 2*sigma1_puppi_pu_inft*0.3);
// resolution_puppi_pu_inft->SetBinContent(2, 2*sigma2_puppi_pu_inft*0.5);
// resolution_puppi_pu_inft->SetBinContent(3, 2*sigma3_puppi_pu_inft*0.7);
// resolution_puppi_pu_inft->SetBinContent(4, 2*sigma4_puppi_pu_inft*0.9);

TCanvas* test1 = new TCanvas("test1","test1", 1000, 1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.11);
gStyle->SetPadLeftMargin(0.135);
gStyle->SetPadRightMargin(0.015);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
test1->cd();
test1->UseCurrentStyle();
puppi_high->GetYaxis()->SetTitle("<#tau_{32}^{rec}/#tau_{32}^{gen}>");
puppi_high->GetXaxis()->SetTitle("#tau_{32}^{gen}");
puppi_high->SetTitle("");
puppi_high->GetYaxis()->SetLabelSize(0.04);
puppi_high->GetYaxis()->SetTitleSize(0.05);
puppi_high->GetYaxis()->SetTitleOffset(1.35);
puppi_high->GetXaxis()->SetLabelSize(0.04);
puppi_high->GetXaxis()->SetTitleSize(0.05);
puppi_high->GetXaxis()->SetTitleOffset(1.0);
puppi_high->GetYaxis()->SetRangeUser(0.0001,1.5);
puppi_high->SetTitle("");
puppi_high->SetLineColor(kRed);
puppi_high->SetMarkerColor(kRed);
puppi_high->SetLineWidth(3);
puppi_high->Draw();
puppi_low->SetLineWidth(3);
puppi_low->SetLineColor(kBlue);
puppi_low->SetMarkerColor(kBlue);
puppi_low->Draw("same");

TLegend *legend_pu_puppi = new TLegend(0.67,0.77,0.92,0.895);
legend_pu_puppi->AddEntry(puppi_low,"N_{PV} #leq 20","leq");
legend_pu_puppi->AddEntry(puppi_high,"N_{PV} > 20","leq");
legend_pu_puppi->Draw();
test1->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/puppi_pu.pdf");


TCanvas* test2 = new TCanvas("test2","test2", 1000, 1000);
gStyle->SetLegendBorderSize(0);
gStyle->SetPadTopMargin(0.02);
gStyle->SetPadBottomMargin(0.11);
gStyle->SetPadLeftMargin(0.135);
gStyle->SetPadRightMargin(0.015);
gStyle->SetOptStat(0);
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);
test2->cd();
test2->UseCurrentStyle();
chs_high->GetYaxis()->SetTitle("<#tau_{32}^{rec}/#tau_{32}^{gen}>");
chs_high->GetXaxis()->SetTitle("#tau_{32}^{gen}");
chs_high->GetYaxis()->SetRangeUser(0.0001,1.5);
chs_high->GetYaxis()->SetLabelSize(0.04);
chs_high->GetYaxis()->SetTitleSize(0.05);
chs_high->GetYaxis()->SetTitleOffset(1.35);
chs_high->GetXaxis()->SetLabelSize(0.04);
chs_high->GetXaxis()->SetTitleSize(0.05);
chs_high->GetXaxis()->SetTitleOffset(1.0);
chs_high->SetTitle("");
chs_high->SetLineColor(kRed);
chs_high->SetMarkerColor(kRed);
chs_high->SetLineWidth(3);
chs_low->SetLineWidth(3);
chs_high->Draw();
chs_low->SetLineColor(kBlue);
chs_low->SetMarkerColor(kBlue);
chs_low->Draw("same");


TLegend *legend_pu_chs = new TLegend(0.69,0.79,0.94,0.915);
legend_pu_chs->AddEntry(chs_low,"N_{PV} #leq 20","leq");
legend_pu_chs->AddEntry(chs_high,"N_{PV} > 20","leq");
legend_pu_chs->Draw();
test2->SaveAs("/afs/desy.de/user/s/skottkej/masterthesis_v2/masterthesis/Plots/chs_pu.pdf");



}
