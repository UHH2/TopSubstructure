// #include "TestPlotter.h"
#include "iostream"
#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TCanvas.h"
#include <TLegend.h>
#include <TStyle.h>
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

int FindLargestVariation(vector<TH1*> variations);
TH1* ConvertToRelative(TH1* sys, TH1* central);
TH1* SetSysError(TH1* data_unfolded, TH2* CovTotal);
TH1* CreateDeltaFromCov(TH2* Cov);
TH1* AddSys(vector<TH1*> sys);

void TestPlotter(int argc, char* argv[]){
  TFile *pseudo1 = new TFile("Unfolding_pseudo1.root", "READ");

  TGraph* asd = (TGraph*)pseudo1->Get("Graph");
  TSpline3* unfolded_pseudo1 = (TSpline3*)pseudo1->Get("Spline3 logTauX");
  TSpline3* unfolded_pseudo12 = (TSpline3*)pseudo1->Get("Spline3 logTauY");

  TCanvas* c1 = new TCanvas("test", "test1", 800, 600);
  c1->cd();
  asd->Draw();

  TCanvas* c2 = new TCanvas("asd", "asd1", 800, 600);
  c2->cd();
  unfolded_pseudo1->Draw();
  unfolded_pseudo12->Draw("same");
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
    cout << "Entry: " << entry[i] << '\n';
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
