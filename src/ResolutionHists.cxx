#include "UHH2/TopSubstructure/include/ResolutionHists.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

ResolutionHists::ResolutionHists(Context & ctx, const string & dirname, std::string const & label1, std::string const & label2): Hists(ctx, dirname), hndl1(ctx.get_handle<vector<GenTopJet>>(label1)), hndl2(ctx.get_handle<vector<TopJet>>(label2)){
  book<TH1D>("Resolution_1", "#tau_{32, rec}/#tau_{32, gen}", 40, 0.6, 2.6);
  book<TH1D>("Resolution_1_rebin", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_2", "#tau_{32, rec}/#tau_{32, gen}", 28, 0.6, 2);
  book<TH1D>("Resolution_2_rebin", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_3", "#tau_{32, rec}/#tau_{32, gen}", 24, 0.6, 1.8);
  book<TH1D>("Resolution_3_rebin", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_4", "#tau_{32, rec}/#tau_{32, gen}", 24, 0.4, 1.6);
  book<TH1D>("Resolution_4_rebin", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_1_pu_20", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_1_pu_inft", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_2_pu_20", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_2_pu_inft", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_3_pu_20", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_3_pu_inft", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_4_pu_20", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  book<TH1D>("Resolution_4_pu_inft", "#tau_{32, rec}/#tau_{32, gen}", 30, 0, 3);
  h_rec_weight = ctx.get_handle<double>("h_rec_weight");
  h_gen_weight = ctx.get_handle<double>("h_gen_weight");
}

void ResolutionHists::fill(const Event & event){
  // Don't forget to always use the weight when filling.
  double rec_weight, gen_weight;
  if(event.is_valid(h_rec_weight))  rec_weight = event.get(h_rec_weight);
  else rec_weight = event.weight;
  // if(event.is_valid(h_gen_weight))  gen_weight = event.get(h_gen_weight);
  // else gen_weight = event.weight;

  if (!event.is_valid(hndl1) || !event.is_valid(hndl2) ) {
    std::cerr << "In ResolutionHists: Handle not valid!\n";
    assert(false);
  }

  vector<GenTopJet> gentopjet = event.get(hndl1);
  vector<TopJet> topjet = event.get(hndl2);

  double gen_tau32 = 0.;
  double rec_tau32 = 0.;

  if(deltaR(topjet.at(0), gentopjet.at(0)) < 0.4){
    gen_tau32 = gentopjet.at(0).tau3()/gentopjet.at(0).tau2();
    rec_tau32 = topjet.at(0).tau3()/topjet.at(0).tau2();
  }

  if(0.2 <= gen_tau32 && gen_tau32 <= 0.4){
    hist("Resolution_1")->Fill(rec_tau32/gen_tau32,rec_weight);
    hist("Resolution_1_rebin")->Fill(rec_tau32/gen_tau32,rec_weight);
  }
  else if(0.4 < gen_tau32 && gen_tau32 <= 0.6){
    hist("Resolution_2")->Fill(rec_tau32/gen_tau32,rec_weight);
    hist("Resolution_2_rebin")->Fill(rec_tau32/gen_tau32,rec_weight);
  }
  else if(0.6 < gen_tau32 && gen_tau32 <= 0.8){
    hist("Resolution_3")->Fill(rec_tau32/gen_tau32,rec_weight);
    hist("Resolution_3_rebin")->Fill(rec_tau32/gen_tau32,rec_weight);
  }
  else if(0.8 < gen_tau32 && gen_tau32 <= 1.0){
    hist("Resolution_4")->Fill(rec_tau32/gen_tau32,rec_weight);
    hist("Resolution_4_rebin")->Fill(rec_tau32/gen_tau32,rec_weight);
  }

  if(event.genInfo->pileup_TrueNumInteractions() <= 20){
    if(0.2 <= gen_tau32 && gen_tau32 <= 0.4){
      hist("Resolution_1_pu_20")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.4 < gen_tau32 && gen_tau32 <= 0.6){
      hist("Resolution_2_pu_20")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.6 < gen_tau32 && gen_tau32 <= 0.8){
      hist("Resolution_3_pu_20")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.8 < gen_tau32 && gen_tau32 <= 1.0){
      hist("Resolution_4_pu_20")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
  }
  else{
    if(0.2 <= gen_tau32 && gen_tau32 <= 0.4){
      hist("Resolution_1_pu_inft")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.4 < gen_tau32 && gen_tau32 <= 0.6){
      hist("Resolution_2_pu_inft")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.6 < gen_tau32 && gen_tau32 <= 0.8){
      hist("Resolution_3_pu_inft")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
    else if(0.8 < gen_tau32 && gen_tau32 <= 1.0){
      hist("Resolution_4_pu_inft")->Fill(rec_tau32/gen_tau32,rec_weight);
    }
  }
}
