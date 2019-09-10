#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>
#include <functional>

using namespace std;
bool compare(double i, std::vector<double> gen_binning);

std::vector<double> bin_range(TString file, TString name, TString hist, double n_events, std::vector<double> gen_binning){
  cout << "starting to calculate bin range." << '\n';
  //Load data
  TFile *TTbar = new TFile(file, "READ");
  TString histogram_path = name+hist;
  TH1D* histogram = (TH1D*) TTbar->Get(histogram_path);

  int start = 1;
  int size_gen = gen_binning.size();
  int n_histo_bins = histogram->GetNbinsX();
  double all_events = histogram->Integral();
  double nmax = n_events+2.;
  unsigned int n_rec_bins = std::floor(all_events/n_events);
  std::vector<double> bin_ranges = {0.};

  for(int i = 1; i < n_histo_bins; i++){
    double integral = histogram->Integral(start, i);
    if(integral >= n_events){
      if(!compare(i/1000, gen_binning)) i++;
      else{
        bin_ranges.push_back(i/1000.);
        start = i+1;
      }
    }
    if(bin_ranges.size()-1 == n_rec_bins){
      double check = histogram->Integral(i, 1000);
      if(check > n_events+2. || check < n_events-2.){
        n_events += check/(n_rec_bins-1);
        start = 1;
        i = 1;
        bin_ranges.clear();
        bin_ranges.push_back(0.);
      }
    }
  }

  bin_ranges.push_back(1.);
  TTbar->Close();
  cout << "Number of bins for " << name << ": " << bin_ranges.size()-1 << ". And the edges: " << '\n';
  for(unsigned int i = 0; i < bin_ranges.size(); i++) cout << bin_ranges.at(i) << '\n';
  cout << "Finished calculating bin range." << '\n';
  cout << '\n';
  return bin_ranges;
}

bool compare(double i, std::vector<double> gen_binning){
  bool ret = true;
  for(unsigned int j = 1; j < gen_binning.size(); j++){
    double diff_bin = fabs((i) - gen_binning.at(j));
    if((diff_bin < 0.04 && diff_bin >= 0)){
      ret = false;
    }
  }
  return ret;
}
