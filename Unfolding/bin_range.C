#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>
#include <functional>

using namespace std;

std::vector<double> bin_range(TString file, TString name, double n_events){
  cout << "starting to calculate bin range." << '\n';
  //Load data
  TFile *TTbar = new TFile(file, "READ");
  TString histogram_path = name+"/tau32_tj1_rebin";
  TH1D* histogram = (TH1D*) TTbar->Get(histogram_path);

  int start = 1;
  std::vector<double> bin_range = {0.};

  for(int i = 1; i <= histogram->GetNbinsX(); i++){
    double integral = histogram->Integral(start, i);
    if(integral >= n_events){
      start = i;
      bin_range.push_back(i/10000.);
    }
    if(i == histogram->GetNbinsX()){
      double diff = fabs(n_events - integral);
      if(diff >= 2.){
        n_events+=0.1;
        i = 1;
        start = 1;
        bin_range.clear();
        bin_range.push_back(0);
      }
    }
  }
  bin_range.push_back(1.);
  TTbar->Close();
  cout << "Number of bins for " << name << ": " << bin_range.size()<< '\n';
  cout << "Finished calculating bin range." << '\n';
  return bin_range;
}
