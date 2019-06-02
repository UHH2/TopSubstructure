#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>
#include <functional>

using namespace std;
bool compare(double i, std::vector<double> gen_binning);

std::vector<double> bin_range(TString file, TString name, double n_events, std::vector<double> gen_binning){
  cout << "starting to calculate bin range." << '\n';
  //Load data
  TFile *TTbar = new TFile(file, "READ");
  TString histogram_path = name+"/tau32_tj1_rebin";
  TH1D* histogram = (TH1D*) TTbar->Get(histogram_path);

  int start = 1;
  int size_gen = gen_binning.size();
  int n_histo_bins = histogram->GetNbinsX();
  double all_events = histogram->Integral();
  double nmax = n_events+2.;
  int n_rec_bins = std::floor(all_events/n_events);
  std::vector<double> bin_range = {0.};

  for(int i = 1; i < n_histo_bins; i++){
    double integral = histogram->Integral(start, i);
    if(integral >= n_events){
      bin_range.push_back(i/10000.);
      start = i+1;
    }
    if(bin_range.size()-1 == n_rec_bins){
      double check = histogram->Integral(i, 10000);
      if(check >= n_events+2. || check <= n_events-2.){
        n_events += check/(n_rec_bins-1);
        start = 1;
        i = 1;
        bin_range.clear();
        bin_range.push_back(0.);
      }
    }
  }

  for(int i = 1; i < bin_range.size(); i++){
    for(int j = 1; j < size_gen; j++){
      double diff = bin_range.at(i) - gen_binning.at(j);
      if(diff < 0.01 && diff >= 0){
        bin_range.at(i) += (diff+0.001);
        // cout << "test2" << '\n';
        // if(i < bin_range.size()-1) bin_range.at(i+1) += diff+0.001;
        // cout << "test3" << '\n';
        // if(i < bin_range.size()-2) bin_range.at(i+2) += diff+0.001;
        // cout << "test4" << '\n';
        // i = 1;
        // j = 1;
      }
      else if(diff > -0.01 && diff <= 0){
        // cout << "test5" << '\n';
        // for(int k = i; k > 0; i--){
        //   bin_range.at(k) += (diff-0.001);
        // }
          bin_range.at(i) += (diff-0.001);
        // bin_range.at(i) -= diff-0.001;
        // cout << "test6" << '\n';
        // if(i >= 2) bin_range.at(i-1) -= diff-0.001;
        // cout << "test7" << '\n';
        // if(i >= 3) bin_range.at(i-2) -= diff-0.001;
        // cout << "test8" << '\n';
        // i = 1;
        // j = 1;
      }
    }
  }

  // for(int i = 1; i < n_histo_bins; i++){
  //   double integral = histogram->Integral(start, i);
  //   if(compare(i, gen_binning)) continue;
  //   if((integral >= nmax) && (histogram->Integral(bin_range.at(bin_range.size()-1), start) <= nmax)){
  //     // cout << "integral: " << integral << '\n';
  //     // cout << "start: " << start << ". And i: " << i << '\n';
  //     for(int k = 1; k < bin_range.size(); k++){
  //       // cout << "bin_range.at(" << k << ") before: " << bin_range.at(k) << '\n';
  //       bin_range.at(k) = bin_range.at(k)+0.001;
  //       // cout << "bin_range.at(" << k << ") after: " << bin_range.at(k) << '\n';
  //     }
  //     // bin_range.clear();
  //     // bin_range.push_back(0.);
  //     start = bin_range.back()*10000;
  //     i--;
  //   }
  //
  //   else if(integral >= n_events){
  //     start = i+1;
  //     bin_range.push_back(i/10000.);
  //   }
  // }


  // for(int i = 1; i <= n_histo_bins; i++){
  //   double integral = histogram->Integral(start, i);
  //
  //   if(integral >= n_events){
  //     for(int j = 0; j < size_gen-1; j++){
  //       double diff_gen = i/10000. - gen_binning.at(j);
  //       if(integral >= (n_events+2.)){
  //         bin_range.clear();
  //         bin_range.push_back(0.);
  //         n_events += 0.01;
  //         i = 1;
  //         start = 1;
  //       }
  //       if((diff_gen < 0.02 && diff_gen >= 0.)){
  //         j--;
  //         i++;
  //       }
  //       else if((diff_gen > -0.02 && diff_gen < 0 && integral < (n_events+2))){
  //         j--;
  //         i--;
  //       }
  //     }
  //     start = i;
  //     bin_range.push_back(i/10000.);
  //   }
  //   if(bin_range.size()-1 == n_rec_bins){
  //     cout << "Integral: " << histogram->Integral(i, 10000) << '\n';
  //     if((histogram->Integral(i, 10000) < (n_events-2.)) || (histogram->Integral(i, 10000) > (n_events+2.))){
  //       bin_range.clear();
  //       bin_range.push_back(0.);
  //       // n_rec_bins++;
  //       cout << "old nevents: " << n_events << '\n';
  //       n_events -= (histogram->Integral(i, 10000)/n_rec_bins);
  //       cout << "new nevents: " << n_events << '\n';
  //       i = 1;
  //       start = 1;
  //     }
  //     else break;
  //   }
  // }

  bin_range.push_back(1.);
  TTbar->Close();
  cout << "Number of bins for " << name << ": " << bin_range.size()-1 << ". And the edges: " << '\n';
  for(unsigned int i = 0; i < bin_range.size(); i++) cout << bin_range.at(i) << '\n';
  cout << "Finished calculating bin range." << '\n';
  cout << '\n';
  return bin_range;
}

bool compare(double i, std::vector<double> gen_binning){
  bool ret = false;
  for(int j = 1; j < gen_binning.size(); j++){
    double diff_bin = fabs((i/10000.) - gen_binning.at(j));
    if((diff_bin < 0.01 && diff_bin >= 0)){
      // cout << "diff: " << diff_bin   << '\n';
      ret = true;
    }
  }
  return ret;
}
