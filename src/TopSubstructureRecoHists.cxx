#include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

TopSubstructureRecoHists::TopSubstructureRecoHists(Context & ctx, const string & dirname, std::string const & label): Hists(ctx, dirname), hndl(ctx.get_handle<vector<TopJet>>(label)){
  // book all histograms here

  const std::string& channel = ctx.get("channel", ""); //define Channel
  if(channel != ""){
    if     (channel == "muon") channel_ = muon;
    else if(channel == "ele") channel_ = ele;
    else {
      std::string log("TopSubstructureRecoHists::TopSubstructureRecoHists -- ");
      log += "invalid argument for 'channel' key in xml file (must be 'muon' or 'ele'): \""+channel+"\"";
      throw std::runtime_error(log);
    }
  }
  else channel_ = none;
  // jets
  book<TH1D>("N_jets", "N_{Jets}", 17, -0.5, 16.5);
  book<TH1D>("eta_jet1", "#eta^{Jet 1}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet2", "#eta^{Jet 2}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet3", "#eta^{Jet 3}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet4", "#eta^{Jet 4}", 40, -2.5, 2.5);
  book<TH1D>("pt_jet1", "p_{T}^{Jet 1}", 120, 0, 1200);
  book<TH1D>("pt_jet2", "p_{T}^{Jet 2}", 120, 0, 1200);
  book<TH1D>("pt_jet3", "p_{T}^{Jet 3}", 120, 0, 1200);
  book<TH1D>("pt_jet4", "p_{T}^{Jet 4}", 90, 0, 900);
  book<TH1D>("N_topjets", "N_{TopJets}", 8, -0.5, 7.5);

  // bJets
  book<TH1D>("N_bJets_loose", "N_{Bjets}^{loose}", 16, -0.5, 15.5);
  book<TH1D>("N_bJets_med", "N_{Bjets}^{medium}", 16, -0.5, 15.5);
  book<TH1D>("N_bJets_tight", "N_{Bjets}^{tight}", 16, -0.5, 15.5);

  // Mass difference between first and second topjet
  book<TH1D>("M_diff1", "M_{first TopJet} - M_{second TopJet} [GeV^{2}]", 80, -400, 400);
  if(channel_ == muon) book<TH1D>("M_diff2", "M_{first TopJet} - M_{second TopJet + Muon} [GeV^{2}]", 80, -400, 400);
  else if(channel_ == ele) book<TH1D>("M_diff2", "M_{first TopJet} - M_{second TopJet + Electron} [GeV^{2}]", 80, -400, 400);
  else if(channel_ == none){
    book<TH1D>("M_diff2_mu", "M_{first TopJet} - M_{second TopJet + Muon} [GeV^{2}]", 80, -400, 400);
    book<TH1D>("M_diff2_ele", "M_{first TopJet} - M_{second TopJet + Electron} [GeV^{2}]", 80, -400, 400);

  }
  book<TH1D>("M_diff1_groomed", "M_{first TopJet, groomed} - M_{second TopJet, groomed} [GeV^{2}]", 80, -400, 400);
  if(channel_ == muon) book<TH1D>("M_diff2_groomed", "M_{first TopJet, groomed} - M_{second TopJet + Muon, groomed} [GeV^{2}]", 80, -400, 400);
  else if(channel_ == ele) book<TH1D>("M_diff2_groomed", "M_{first TopJet, groomed} - M_{second TopJet + Electron, groomed} [GeV^{2}]", 80, -400, 400);
  else if(channel_ == none){
    book<TH1D>("M_diff2_groomed_mu", "M_{first TopJet, groomed} - M_{second TopJet + Muon, groomed} [GeV^{2}]", 80, -400, 400);
    book<TH1D>("M_diff2_groomed_ele", "M_{first TopJet, groomed} - M_{second TopJet + Electron, groomed} [GeV^{2}]", 80, -400, 400);

  }

  // TopJets
  // first topjet, ungroomed
  book<TH1D>("M_tj1", "M_{first TopJet} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_tj1_rebin", "M_{first TopJet} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_tj1", "p_{T} first TopJet [GeV]", 120, 0, 1200);
  book<TH1D>("pt_tj1_rebin1", "p_{T} first TopJet [GeV]", 80, 0, 1200);
  book<TH1D>("pt_tj1_rebin2", "p_{T} first TopJet [GeV]", 60, 0, 1200);
  book<TH1D>("eta_tj1", "#eta first TopJet", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_tjs", "N_{subjets} all TopJets", 4, -0.5, 3.5);
  book<TH1D>("Nsubjet_tj1", "N_{subjets} first TopJet", 4, -0.5, 3.5);
  book<TH1D>("pt_subjet1_tj1", "p_{T}^{subjet1} first TopJet [GeV]", 120, 0, 1200);
  book<TH1D>("pt_subjet2_tj1", "p_{T}^{subjet2} first TopJet [GeV]", 80, 0, 800);
  book<TH1D>("m_subjet1_tj1", "M_{subjet1} first TopJet [GeV^{2}]", 60, 0, 300);
  book<TH1D>("m_subjet2_tj1", "M_{subjet2} first TopJet [GeV^{2}]", 60, 0, 300);
  book<TH1D>("tau1_tj1", "#tau_{1} first TopJet", 40, 0, 1.);
  book<TH1D>("tau1_tj1_rebin", "#tau_{1} first TopJet", 1000, 0, 1.);
  book<TH1D>("tau2_tj1", "#tau_{2} first TopJet", 40, 0, 1.);
  book<TH1D>("tau2_tj1_rebin", "#tau_{2} first TopJet", 1000, 0, 1.);

  book<TH1D>("tau2_tj1_calc_start", "#tau_{2} first TopJet (calculated start)", 40, 0, 1.);
  book<TH1D>("tau2_tj1_start_diff", "(#tau_{2}-#tau_{2, calc})/#tau_{2} first TopJet (start)", 40, -1., 1.);
  book<TH1D>("tau2_tj1_calc_common", "#tau_{2} first TopJet (calculated common)", 40, 0, 1.);
  book<TH1D>("tau2_tj1_common_diff", "(#tau_{2}-#tau_{2, calc})/#tau_{2} first TopJet (common)", 40, -1., 1.);
  book<TH1D>("tau2_tj1_calc_corrector", "#tau_{2} first TopJet (calculated corrector)", 40, 0, 1.);
  book<TH1D>("tau2_tj1_corrector_diff", "(#tau_{2}-#tau_{2, calc})/#tau_{2} first TopJet (corrector)", 40, -1., 1.);
  book<TH1D>("tau2_tj1_calc_lepcleaner", "#tau_{2} first TopJet (calculated lepcleaner)", 40, 0, 1.);
  book<TH1D>("tau2_tj1_lepcleaner_diff", "(#tau_{2}-#tau_{2, calc})/#tau_{2} first TopJet (lepcleaner)", 40, -1., 1.);
  book<TH1D>("tau2_tj1_calc_cleaner", "#tau_{2} first TopJet (calculated cleaner)", 40, 0, 1.);
  book<TH1D>("tau2_tj1_cleaner_diff", "(#tau_{2}-#tau_{2, calc})/#tau_{2} first TopJet (cleaner)", 40, -1., 1.);

  book<TH1D>("tau3_tj1", "#tau_{3} first TopJet", 40, 0, 1.);
  book<TH1D>("tau3_tj1_rebin", "#tau_{3} first TopJet", 1000, 0, 1.);
  book<TH1D>("tau4_tj1", "#tau_{4} first TopJet", 40, 0, 1.);
  book<TH1D>("tau4_tj1_rebin", "#tau_{4} first TopJet", 1000, 0, 1.);

  book<TH1D>("tau3_tj1_calc_start", "#tau_{3} first TopJet (calculated start)", 40, 0, 1.);
  book<TH1D>("tau3_tj1_start_diff", "(#tau_{3}-#tau_{3, calc})/#tau_{3} first TopJet (start)", 40, -1., 1.);
  book<TH1D>("tau3_tj1_calc_common", "#tau_{3} first TopJet (calculated common)", 40, 0, 1.);
  book<TH1D>("tau3_tj1_common_diff", "(#tau_{3}-#tau_{3, calc})/#tau_{3} first TopJet (common)", 40, -1., 1.);
  book<TH1D>("tau3_tj1_calc_corrector", "#tau_{3} first TopJet (calculated corrector)", 40, 0, 1.);
  book<TH1D>("tau3_tj1_corrector_diff", "(#tau_{3}-#tau_{3, calc})/#tau_{3} first TopJet (corrector)", 40, -1., 1.);
  book<TH1D>("tau3_tj1_calc_lepcleaner", "#tau_{3} first TopJet (calculated lepcleaner)", 40, 0, 1.);
  book<TH1D>("tau3_tj1_lepcleaner_diff", "(#tau_{3}-#tau_{3, calc})/#tau_{3} first TopJet (lepcleaner)", 40, -1., 1.);
  book<TH1D>("tau3_tj1_calc_cleaner", "#tau_{3} first TopJet (calculated cleaner)", 40, 0, 1.);
  book<TH1D>("tau3_tj1_cleaner_diff", "(#tau_{3}-#tau_{3, calc})/#tau_{3} first TopJet (cleaner)", 40, -1., 1.);

  book<TH1D>("tau32_tj1", "#tau_{3}/#tau_{2} first TopJet", 40, 0, 1.);
  book<TH1D>("tau32_tj1_rebin1", "#tau_{3}/#tau_{2} first TopJet", 100, 0, 1.);
  book<TH1D>("tau32_tj1_rebin2", "#tau_{3}/#tau_{2} first TopJet", 1000, 0, 1.);

  book<TH1D>("tau31_tj1", "#tau_{3}/#tau_{1} first TopJet", 40, 0, 1.);
  book<TH1D>("tau31_tj1_rebin1", "#tau_{3}/#tau_{1} first TopJet", 100, 0, 1.);
  book<TH1D>("tau31_tj1_rebin2", "#tau_{3}/#tau_{1} first TopJet", 1000, 0, 1.);
  book<TH1D>("tau41_tj1", "#tau_{4}/#tau_{1} first TopJet", 40, 0, 1.);
  book<TH1D>("tau41_tj1_rebin1", "#tau_{4}/#tau_{1} first TopJet", 100, 0, 1.);
  book<TH1D>("tau41_tj1_rebin2", "#tau_{1}/#tau_{4} first TopJet", 1000, 0, 1.);
  book<TH1D>("tau42_tj1", "#tau_{4}/#tau_{2} first TopJet", 40, 0, 1.);
  book<TH1D>("tau42_tj1_rebin1", "#tau_{4}/#tau_{2} first TopJet", 100, 0, 1.);
  book<TH1D>("tau42_tj1_rebin2", "#tau_{4}/#tau_{2} first TopJet", 1000, 0, 1.);
  book<TH1D>("tau43_tj1", "#tau_{4}/#tau_{3} first TopJet", 40, 0, 1.);
  book<TH1D>("tau43_tj1_rebin1", "#tau_{4}/#tau_{3} first TopJet", 100, 0, 1.);
  book<TH1D>("tau43_tj1_rebin2", "#tau_{4}/#tau_{3} first TopJet", 1000, 0, 1.);

  book<TH1D>("tau32_tj1_calc_start", "#tau_{3}/#tau_{2} first TopJet (calculated start)", 40, 0, 1.);
  book<TH1D>("tau32_tj1_start_diff", "(#tau_{32}-#tau_{32, calc})/#tau_{32} first TopJet (start)", 40, -1., 1.);
  book<TH1D>("tau32_tj1_calc_start_rebin1", "#tau_{3}/#tau_{2} first TopJet (calculated start)", 100, 0, 1.);
  book<TH1D>("tau32_tj1_calc_start_rebin2", "#tau_{3}/#tau_{2} first TopJet (calculated start)", 1000, 0, 1.);
  book<TH1D>("tau32_tj1_calc_common", "#tau_{3}/#tau_{2} first TopJet (calculated common)", 40, 0, 1.);
  book<TH1D>("tau32_tj1_common_diff", "(#tau_{32}-#tau_{32, calc})/#tau_{32} first TopJet (common)", 40, -1., 1.);
  book<TH1D>("tau32_tj1_calc_common_rebin1", "#tau_{3}/#tau_{2} first TopJet (calculated common)", 100, 0, 1.);
  book<TH1D>("tau32_tj1_calc_common_rebin2", "#tau_{3}/#tau_{2} first TopJet (calculated common)", 1000, 0, 1.);
  book<TH1D>("tau32_tj1_calc_corrector", "#tau_{3}/#tau_{2} first TopJet (calculated corrector)", 40, 0, 1.);
  book<TH1D>("tau32_tj1_corrector_diff", "(#tau_{32}-#tau_{32, calc})/#tau_{32} first TopJet (corrector)", 40, -1., 1.);
  book<TH1D>("tau32_tj1_calc_corrector_rebin1", "#tau_{3}/#tau_{2} first TopJet (calculated corrector)", 100, 0, 1.);
  book<TH1D>("tau32_tj1_calc_corrector_rebin2", "#tau_{3}/#tau_{2} first TopJet (calculated corrector)", 1000, 0, 1.);
  book<TH1D>("tau32_tj1_calc_lepcleaner", "#tau_{3}/#tau_{2} first TopJet (calculated lepcleaner)", 40, 0, 1.);
  book<TH1D>("tau32_tj1_lepcleaner_diff", "(#tau_{32}-#tau_{32, calc})/#tau_{32} first TopJet (lepcleaner)", 40, -1., 1.);
  book<TH1D>("tau32_tj1_calc_lepcleaner_rebin1", "#tau_{3}/#tau_{2} first TopJet (calculated lepcleaner)", 100, 0, 1.);
  book<TH1D>("tau32_tj1_calc_lepcleaner_rebin2", "#tau_{3}/#tau_{2} first TopJet (calculated lepcleaner)", 1000, 0, 1.);
  book<TH1D>("tau32_tj1_calc_cleaner", "#tau_{3}/#tau_{2} first TopJet (calculated cleaner)", 40, 0, 1.);
  book<TH1D>("tau32_tj1_cleaner_diff", "(#tau_{32}-#tau_{32, calc})/#tau_{32} first TopJet (cleaner)", 40, -1., 1.);
  book<TH1D>("tau32_tj1_calc_cleaner_rebin1", "#tau_{3}/#tau_{2} first TopJet (calculated cleaner)", 100, 0, 1.);
  book<TH1D>("tau32_tj1_calc_cleaner_rebin2", "#tau_{3}/#tau_{2} first TopJet (calculated cleaner)", 1000, 0, 1.);

  book<TH1D>("tau21_tj1", "#tau_{2}/#tau_{1} first TopJet", 40, 0, 1.);
  book<TH1D>("tau21_tj1_rebin", "#tau_{2}/#tau_{1} first TopJet", 1000, 0, 1.);

  if(channel_ == muon)     book<TH1D>("dR_lep_tj1", "#Delta R(#mu, first TopJet)", 50, 0, 5.);
  else if(channel_ == ele) book<TH1D>("dR_lep_tj1", "#Delta R(electron, first TopJet)", 50, 0, 5.);
  else{
    book<TH1D>("dR_mu_tj1", "#Delta R(#mu, first TopJet)", 50, 0, 5.);
    book<TH1D>("dR_ele_tj1", "#Delta R(electron, first TopJet)", 50, 0, 5.);
  }
  if(channel_ == muon) book<TH1D>("dPhi_lep_tj1", "#Delta #Phi (#mu, first TopJet)", 32, 0, 3.2);
  else if(channel_ == ele)                 book<TH1D>("dPhi_lep_tj1", "#Delta #Phi (electron, first TopJet)", 32, 0, 3.2);
  else{
    book<TH1D>("dPhi_mu_tj1", "#Delta #Phi (#mu, first TopJet)", 32, 0, 3.2);
    book<TH1D>("dPhi_ele_tj1", "#Delta #Phi (electron, first TopJet)", 32, 0, 3.2);
  }

  // first topjet, groomed
  book<TH1D>("M_tj1_groomed", "M_{first TopJet, groomed} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("tau1_tj1_groomed", "#tau_{1, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau1_tj1_groomed_diff", "(#tau_{1, ungroomed}-#tau_{1, groomed})/#tau_{1,ungroomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau2_tj1_groomed", "#tau_{2,groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau2_tj1_groomed_diff", "(#tau_{2, ungroomed}-#tau_{2, groomed})/#tau_{2,ungroomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau3_tj1_groomed", "#tau_{3, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau3_tj1_groomed_diff", "(#tau_{3, ungroomed}-#tau_{3, groomed})/#tau_{3,ungroomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau4_tj1_groomed", "#tau_{4, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau4_tj1_groomed_diff", "(#tau_{4, ungroomed}-#tau_{4, groomed})/#tau_{4,ungroomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau21_tj1_groomed", "#tau_{2, groomed}/#tau_{1, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau21_tj1_groomed_diff", "(#tau_{21, ungroomed}-#tau_{21, groomed})/#tau_{21, ungroomed} first TopJet", 40, -1., 1.);
  book<TH1D>("tau32_tj1_groomed", "#tau_{3, groomed}/#tau_{2, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau32_tj1_groomed_diff", "(#tau_{32, ungroomed}-#tau_{32, groomed})/#tau_{32, ungroomed} first TopJet", 40, -1., 1.);
  book<TH1D>("tau31_tj1_groomed", "#tau_{3, groomed}/#tau_{1, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau31_tj1_groomed_diff", "(#tau_{31, ungroomed}-#tau_{31, groomed})/#tau_{31, ungroomed} first TopJet", 40, -1., 1.);
  book<TH1D>("tau41_tj1_groomed", "#tau_{4, groomed}/#tau_{1, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau41_tj1_groomed_diff", "(#tau_{41, ungroomed}-#tau_{41, groomed})/#tau_{41, ungroomed} first TopJet", 40, -1., 1.);
  book<TH1D>("tau42_tj1_groomed", "#tau_{4, groomed}/#tau_{2, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau42_tj1_groomed_diff", "(#tau_{42, ungroomed}-#tau_{42, groomed})/#tau_{42, ungroomed} first TopJet", 40, -1., 1.);
  book<TH1D>("tau43_tj1_groomed", "#tau_{4, groomed}/#tau_{3, groomed} first TopJet", 40, 0, 1.);
  book<TH1D>("tau43_tj1_groomed_diff", "(#tau_{43, ungroomed}-#tau_{43, groomed})/#tau_{43, ungroomed} first TopJet", 40, -1., 1.);

  book<TH1D>("ecfN2_beta1_tj1", "ecfN2_beta1 first TopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta1_tj1_rebin", "ecfN2_beta1 first TopJet", 1000, 0, 1.);
  book<TH1D>("ecfN2_beta2_tj1", "ecfN2_beta2 first TopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta2_tj1_rebin", "ecfN2_beta2 first TopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta1_tj1", "ecfN3_beta1 first TopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta1_tj1_rebin", "ecfN3_beta1 first TopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta2_tj1", "ecfN3_beta2 first TopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta2_tj1_rebin", "ecfN3_beta2 first TopJet", 1000, 0, 1.);


  //  second topjet, ungroomed
  book<TH1D>("M_tj2", "M_{second TopJet} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_tj2_rebin", "M_{second TopJet} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_tj2", "p_{T} second TopJet [GeV]", 120, 0, 1200);
  book<TH1D>("eta_tj2", "#eta second TopJet", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_tj2", "N_{subjets} second TopJet", 4, -0.5, 3.5);
  book<TH1D>("pt_subjet1_tj2", "p_{T}^{subjet1} second TopJet [GeV]", 120, 0, 1200);
  book<TH1D>("pt_subjet2_tj2", "p_{T}^{subjet2} second TopJet [GeV]", 80, 0, 800);
  book<TH1D>("m_subjet1_tj2", "M_{subjet1} second TopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("m_subjet2_tj2", "M_{subjet2} second TopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("tau1_tj2", "#tau_{1} second TopJet", 40, 0, 1.);
  book<TH1D>("tau1_tj2_rebin", "#tau_{1} second TopJet", 1000, 0, 1.);
  book<TH1D>("tau2_tj2", "#tau_{2} second TopJet", 40, 0, 1.);
  book<TH1D>("tau2_tj2_rebin", "#tau_{2} second TopJet", 1000, 0, 1.);
  book<TH1D>("tau3_tj2", "#tau_{3} second TopJet", 40, 0, 1.);
  book<TH1D>("tau3_tj2_rebin", "#tau_{3} second TopJet", 1000, 0, 1.);
  book<TH1D>("tau32_tj2", "#tau_{3}/#tau_{2} second TopJet", 40, 0, 1.);
  book<TH1D>("tau32_tj2_rebin1", "#tau_{3}/#tau_{2} second TopJet", 100, 0, 1.);
  book<TH1D>("tau32_tj2_rebin2", "#tau_{3}/#tau_{2} second TopJet", 1000, 0, 1.);
  book<TH1D>("tau21_tj2", "#tau_{2}/#tau_{1} second TopJet", 40, 0, 1.);
  book<TH1D>("tau21_tj2_rebin", "#tau_{2}/#tau_{1} second TopJet", 1000, 0, 1.);

  if(channel_ == muon)     book<TH1D>("dR_lep_tj2", "#Delta R(#mu, second TopJet)", 50, 0, 5.);
  else if(channel_ == ele) book<TH1D>("dR_lep_tj2", "#Delta R(electron, second TopJet)", 50, 0, 5.);
  else{
    book<TH1D>("dR_mu_tj2", "#Delta R(#mu, second TopJet)", 50, 0, 5.);
    book<TH1D>("dR_ele_tj2", "#Delta R(electron, second TopJet)", 50, 0, 5.);
  }
  if(channel_ == muon)     book<TH1D>("dPhi_lep_tj2", "#Delta #Phi (#mu, second TopJet)", 32, 0, 3.2);
  else if(channel_ == ele) book<TH1D>("dPhi_lep_tj2", "#Delta #Phi (electron, second TopJet)", 32, 0, 3.2);
  else{
    book<TH1D>("dPhi_mu_tj2", "#Delta #Phi (#mu, second TopJet)", 32, 0, 3.2);
    book<TH1D>("dPhi_ele_tj2", "#Delta #Phi (electron, second TopJet)", 32, 0, 3.2);
  }

  // second topjet, groomed
  book<TH1D>("M_tj2_groomed", "M_{second TopJet, groomed} [GeV^{2}]", 40, 0, 400);

  // leptons
  book<TH1D>("N_ele", "N_{electrons}", 8, -0.5, 7.5);
  book<TH1D>("N_mu", "N_{#mu}", 8, -0.5, 7.5);
  if(channel_ == muon) book<TH1D>("pt_lep", "p_{T}^{#mu} [GeV]", 100, 0, 1000);
  else if(channel_ == ele) book<TH1D>("pt_lep", "p_{T}^{electron} [GeV]", 100, 0, 1000);
  else{
    book<TH1D>("pt_mu", "p_{T}^{#mu} [GeV]", 100, 0, 1000);
    book<TH1D>("pt_ele", "p_{T}^{electron} [GeV]", 100, 0, 1000);
  }
  if(channel_ == muon) book<TH1D>("eta_lep", "#eta^{#mu}", 40, -2.5, 2.5);
  else if(channel_ == ele) book<TH1D>("eta_lep", "#eta^{electron}", 40, -2.5, 2.5);
  else{
    book<TH1D>("eta_mu", "#eta^{#mu}", 40, -2.5, 2.5);
    book<TH1D>("eta_ele", "#eta^{electron}", 40, -2.5, 2.5);
  }
  if(channel_ == muon) book<TH1D>("mass_lep", "M^{#mu} [GeV]", 50, 0, 0.5);
  else if(channel_ == ele) book<TH1D>("mass_lep", "M^{electron} [GeV]", 50, 0, 0.5);
  else{
    book<TH1D>("mass_mu", "M^{#mu} [GeV]", 50, 0, 0.5);
    book<TH1D>("mass_ele", "M^{electron} [GeV]", 50, 0, 0.5);
  }

  //general
  book<TH1D>("E_Tmiss", "missing E_{T} [GeV]", 75, 0, 1500);
  book<TH1D>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);
  book<TH1D>("empty_pf_cand", "BinContent = sum(eventweights)", 1, 0.5, 1.5);


  // 2-D plot
  TwoDCut = book<TH2F>("TwoDCut", "x=#Delta R y=p_{T}^{rel}", 50, 0, 2, 50, 0, 200);
  if(channel_ == none){
    TwoDCut_mu = book<TH2F>("TwoDCut_mu", "x=#Delta R y=p_{T}^{rel} (#mu)", 50, 0, 2, 50, 0, 200);
    TwoDCut_ele = book<TH2F>("TwoDCut_ele", "x=#Delta R y=p_{T}^{rel} (electron)", 50, 0, 2, 50, 0, 200);
  }

  // get handles
  h_weight = ctx.get_handle<double>("h_rec_weight");
  h_weight_kin = ctx.get_handle<double>("h_rec_weight_kin");
  isTTbar = (ctx.get("dataset_version") == "TTbar_2016v3_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_2016v3_Mtt1000toInft" || ctx.get("dataset_version") == "TTbar_2016v3");

  if (label.find("puppi_sd") != std::string::npos) {
    std::cout << "found!" << '\n';
    h_tau2_start = ctx.get_handle<double>("h_pf_tau2_start_puppi_sd");
    h_tau2_common = ctx.get_handle<double>("h_pf_tau2_common_puppi_sd");
    h_tau2_corrector = ctx.get_handle<double>("h_pf_tau2_corrector_puppi_sd");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_pf_tau2_lepcleaner_puppi_sd");
    h_tau2_cleaner = ctx.get_handle<double>("h_pf_tau2_cleaner_puppi_sd");

    h_tau3_start = ctx.get_handle<double>("h_pf_tau3_start_puppi_sd");
    h_tau3_common = ctx.get_handle<double>("h_pf_tau3_common_puppi_sd");
    h_tau3_corrector = ctx.get_handle<double>("h_pf_tau3_corrector_puppi_sd");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_pf_tau3_lepcleaner_puppi_sd");
    h_tau3_cleaner = ctx.get_handle<double>("h_pf_tau3_cleaner_puppi_sd");
  }
  else if(label.find("puppi") != std::string::npos){
    h_tau2_start = ctx.get_handle<double>("h_pf_tau2_start_puppi");
    h_tau2_common = ctx.get_handle<double>("h_pf_tau2_common_puppi");
    h_tau2_corrector = ctx.get_handle<double>("h_pf_tau2_corrector_puppi");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_pf_tau2_lepcleaner_puppi");
    h_tau2_cleaner = ctx.get_handle<double>("h_pf_tau2_cleaner_puppi");

    h_tau3_start = ctx.get_handle<double>("h_pf_tau3_start_puppi");
    h_tau3_common = ctx.get_handle<double>("h_pf_tau3_common_puppi");
    h_tau3_corrector = ctx.get_handle<double>("h_pf_tau3_corrector_puppi");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_pf_tau3_lepcleaner_puppi");
    h_tau3_cleaner = ctx.get_handle<double>("h_pf_tau3_cleaner_puppi");
  }
  else if(label.find("sd") != std::string::npos){
    h_tau2_start = ctx.get_handle<double>("h_pf_tau2_start_sd");
    h_tau2_common = ctx.get_handle<double>("h_pf_tau2_common_sd");
    h_tau2_corrector = ctx.get_handle<double>("h_pf_tau2_corrector_sd");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_pf_tau2_lepcleaner_sd");
    h_tau2_cleaner = ctx.get_handle<double>("h_pf_tau2_cleaner_sd");

    h_tau3_start = ctx.get_handle<double>("h_pf_tau3_start_sd");
    h_tau3_common = ctx.get_handle<double>("h_pf_tau3_common_sd");
    h_tau3_corrector = ctx.get_handle<double>("h_pf_tau3_corrector_sd");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_pf_tau3_lepcleaner_sd");
    h_tau3_cleaner = ctx.get_handle<double>("h_pf_tau3_cleaner_sd");
  }
  else{
    h_tau2_start = ctx.get_handle<double>("h_pf_tau2_start");
    h_tau2_common = ctx.get_handle<double>("h_pf_tau2_common");
    h_tau2_corrector = ctx.get_handle<double>("h_pf_tau2_corrector");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_pf_tau2_lepcleaner");
    h_tau2_cleaner = ctx.get_handle<double>("h_pf_tau2_cleaner");

    h_tau3_start = ctx.get_handle<double>("h_pf_tau3_start");
    h_tau3_common = ctx.get_handle<double>("h_pf_tau3_common");
    h_tau3_corrector = ctx.get_handle<double>("h_pf_tau3_corrector");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_pf_tau3_lepcleaner");
    h_tau3_cleaner = ctx.get_handle<double>("h_pf_tau3_cleaner");
  }
}

void TopSubstructureRecoHists::fill(const Event & event){
  // Don't forget to always use the weight when filling.
  double weight;
  if(event.is_valid(h_weight))  weight = event.get(h_weight);
  else if(event.is_valid(h_weight_kin))  weight = event.get(h_weight_kin);
  else weight = event.weight;

  double tau3_start = -100;
  double tau3_common = -100;
  double tau3_corrector = -100;
  double tau3_lepcleaner = -100;
  double tau3_cleaner = -100;
  // if(event.is_valid(h_tau3_before)) tau3_before = event.get(h_tau3_before);
  if(event.is_valid(h_tau3_start))      tau3_start      = event.get(h_tau3_start);
  if(event.is_valid(h_tau3_common))     tau3_common     = event.get(h_tau3_common);
  if(event.is_valid(h_tau3_corrector))  tau3_corrector  = event.get(h_tau3_corrector);
  if(event.is_valid(h_tau3_lepcleaner)) tau3_lepcleaner = event.get(h_tau3_lepcleaner);
  if(event.is_valid(h_tau3_cleaner))    tau3_cleaner    = event.get(h_tau3_cleaner);

  double tau2_start = -100;
  double tau2_common = -100;
  double tau2_corrector = -100;
  double tau2_lepcleaner = -100;
  double tau2_cleaner = -100;
  // if(event.is_valid(h_tau2_before)) tau2_before = event.get(h_tau2_before);
  if(event.is_valid(h_tau2_start))      tau2_start      = event.get(h_tau2_start);
  if(event.is_valid(h_tau2_common))     tau2_common     = event.get(h_tau2_common);
  if(event.is_valid(h_tau2_corrector))  tau2_corrector  = event.get(h_tau2_corrector);
  if(event.is_valid(h_tau2_lepcleaner)) tau2_lepcleaner = event.get(h_tau2_lepcleaner);
  if(event.is_valid(h_tau2_cleaner))    tau2_cleaner    = event.get(h_tau2_cleaner);


  if (!event.is_valid(hndl)) {
    std::cerr << "In TopSubstructureRecoHists: Handle not valid!\n";
    assert(false);
  }

  vector<TopJet> topjet = event.get(hndl);

  // std::vector<TopJet>* topjet = event.topjets;

  //general
  hist("sum_event_weights")->Fill(1, weight);

  double drmin, ptrel;
  if(channel_ == muon && event.muons->size() > 0){
    std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
    TwoDCut->Fill(drmin, ptrel, weight);
  }
  else if(channel_ == ele && event.electrons->size() > 0){
    std::tie(drmin, ptrel) = drmin_pTrel(event.electrons->at(0), *event.jets);
    TwoDCut->Fill(drmin, ptrel, weight);
  }
  if(channel_ == none && event.muons->size() > 0){
    std::tie(drmin, ptrel) = drmin_pTrel(event.muons->at(0), *event.jets);
    TwoDCut_mu->Fill(drmin, ptrel, weight);
  }
  if(channel_ == none && event.electrons->size() > 0){
    std::tie(drmin, ptrel) = drmin_pTrel(event.electrons->at(0), *event.jets);
    TwoDCut_ele->Fill(drmin, ptrel, weight);
  }




  // Jets
  std::vector<Jet>* jets = event.jets;
  int Njets = jets->size();

  hist("N_jets")->Fill(Njets, weight);
  if(Njets>=1) hist("eta_jet1")->Fill(jets->at(0).eta(), weight);
  if(Njets>=2) hist("eta_jet2")->Fill(jets->at(1).eta(), weight);
  if(Njets>=3) hist("eta_jet3")->Fill(jets->at(2).eta(), weight);
  if(Njets>=4) hist("eta_jet4")->Fill(jets->at(3).eta(), weight);
  if(Njets>=1) hist("pt_jet1")->Fill(jets->at(0).pt(), weight);
  if(Njets>=2) hist("pt_jet2")->Fill(jets->at(1).pt(), weight);
  if(Njets>=3) hist("pt_jet3")->Fill(jets->at(2).pt(), weight);
  if(Njets>=4) hist("pt_jet4")->Fill(jets->at(3).pt(), weight);

  hist("N_topjets")->Fill(topjet.size(), weight);


  // bJets
  std::vector<Jet> bjets_loose, bjets_med, bjets_tight;
  CSVBTag Btag_loose = CSVBTag(CSVBTag::WP_LOOSE);
  CSVBTag Btag_medium = CSVBTag(CSVBTag::WP_MEDIUM);
  CSVBTag Btag_tight = CSVBTag(CSVBTag::WP_TIGHT);


  for (unsigned int i =0; i<jets->size(); ++i) {
    if(Btag_loose(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_loose.push_back(jets->at(i));
    }
    if(Btag_medium(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_med.push_back(jets->at(i));
    }
    if(Btag_tight(jets->at(i),event)) { //loose: >0.605, medium: >0.890, tight: >0.970
      bjets_tight.push_back(jets->at(i));
    }
  }

  int NbJets_loose = bjets_loose.size();
  hist("N_bJets_loose")->Fill(NbJets_loose,weight);
  int NbJets_med = bjets_med.size();
  hist("N_bJets_med")->Fill(NbJets_med,weight);
  int NbJets_tight = bjets_tight.size();
  hist("N_bJets_tight")->Fill(NbJets_tight,weight);


  // TopJet
  if(topjet.size() > 0){
    for(unsigned int i=0; i < topjet.size(); i++){
      hist("Nsubjet_tjs")->Fill(topjet.at(i).subjets().size(), weight);
    }

    hist("M_tj1")->Fill(topjet.at(0).v4().M(), weight);
    hist("M_tj1_rebin")->Fill(topjet.at(0).v4().M(), weight);


    LorentzVector subjet_sum1;
    for (const auto s : topjet.at(0).subjets()){
      subjet_sum1 += s.v4();
    }
    double mass1 = subjet_sum1.M();
    hist("M_tj1_groomed")->Fill(mass1, weight);


    hist("pt_tj1")->Fill(topjet.at(0).pt(), weight);
    hist("pt_tj1_rebin1")->Fill(topjet.at(0).pt(), weight);
    hist("pt_tj1_rebin2")->Fill(topjet.at(0).pt(), weight);
    hist("eta_tj1")->Fill(topjet.at(0).eta(), weight);
    hist("Nsubjet_tj1")->Fill(topjet.at(0).subjets().size(), weight);
    if(topjet.at(0).subjets().size() >= 1) hist("pt_subjet1_tj1")->Fill(topjet.at(0).subjets().at(0).v4().pt(), weight);
    if(topjet.at(0).subjets().size() >= 2) hist("pt_subjet2_tj1")->Fill(topjet.at(0).subjets().at(1).v4().pt(), weight);
    if(topjet.at(0).subjets().size() >= 1) hist("m_subjet1_tj1")->Fill(topjet.at(0).subjets().at(0).v4().M(), weight);
    if(topjet.at(0).subjets().size() >= 2) hist("m_subjet2_tj1")->Fill(topjet.at(0).subjets().at(1).v4().M(), weight);
    hist("tau1_tj1")->Fill(topjet.at(0).tau1(), weight);
    hist("tau1_tj1_groomed")->Fill(topjet.at(0).tau1_groomed(), weight);
    hist("tau1_tj1_groomed_diff")->Fill((topjet.at(0).tau1()-topjet.at(0).tau1_groomed())/topjet.at(0).tau1(), weight);
    hist("tau1_tj1_rebin")->Fill(topjet.at(0).tau1(), weight);
    hist("tau2_tj1")->Fill(topjet.at(0).tau2(), weight);
    hist("tau2_tj1_groomed")->Fill(topjet.at(0).tau2_groomed(), weight);
    hist("tau2_tj1_groomed_diff")->Fill((topjet.at(0).tau2()-topjet.at(0).tau2_groomed())/topjet.at(0).tau2(), weight);

    if(tau2_start >= 0){
      hist("tau2_tj1_calc_start")->Fill(tau2_start, weight);
      double ratio = (topjet.at(0).tau2()-tau2_start)/topjet.at(0).tau2();
      hist("tau2_tj1_start_diff")->Fill(ratio, weight);
    }
    else hist("tau2_tj1_start_diff")->Fill(1., weight);
    if(tau2_common >= 0){
      hist("tau2_tj1_calc_common")->Fill(tau2_common, weight);
      double ratio = (topjet.at(0).tau2()-tau2_common)/topjet.at(0).tau2();
      hist("tau2_tj1_common_diff")->Fill(ratio, weight);
    }
    else hist("tau2_tj1_common_diff")->Fill(1., weight);
    if(tau2_corrector >= 0){
      hist("tau2_tj1_calc_corrector")->Fill(tau2_corrector, weight);
      double ratio = (topjet.at(0).tau2()-tau2_corrector)/topjet.at(0).tau2();
      hist("tau2_tj1_corrector_diff")->Fill(ratio, weight);
    }
    else hist("tau2_tj1_corrector_diff")->Fill(1., weight);
    if(tau2_lepcleaner >= 0){
      hist("tau2_tj1_calc_lepcleaner")->Fill(tau2_lepcleaner, weight);
      double ratio = (topjet.at(0).tau2()-tau2_lepcleaner)/topjet.at(0).tau2();
      hist("tau2_tj1_lepcleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau2_tj1_lepcleaner_diff")->Fill(1., weight);
    if(tau2_cleaner >= 0){
      hist("tau2_tj1_calc_cleaner")->Fill(tau2_cleaner, weight);
      double ratio = (topjet.at(0).tau2()-tau2_cleaner)/topjet.at(0).tau2();
      hist("tau2_tj1_cleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau2_tj1_cleaner_diff")->Fill(1., weight);

    hist("tau2_tj1_rebin")->Fill(topjet.at(0).tau2(), weight);
    hist("tau3_tj1")->Fill(topjet.at(0).tau3(), weight);
    hist("tau4_tj1")->Fill(topjet.at(0).tau4(), weight);
    hist("tau3_tj1_groomed")->Fill(topjet.at(0).tau3_groomed(), weight);
    hist("tau3_tj1_groomed_diff")->Fill((topjet.at(0).tau3()-topjet.at(0).tau3_groomed())/topjet.at(0).tau3(), weight);
    hist("tau4_tj1_groomed")->Fill(topjet.at(0).tau4_groomed(), weight);
    hist("tau4_tj1_groomed_diff")->Fill((topjet.at(0).tau4()-topjet.at(0).tau4_groomed())/topjet.at(0).tau4(), weight);
    if(tau3_start >= 0){
      hist("tau3_tj1_calc_start")->Fill(tau3_start, weight);
      double ratio = (topjet.at(0).tau3()-tau3_start)/topjet.at(0).tau3();
      hist("tau3_tj1_start_diff")->Fill(ratio, weight);
    }
    else hist("tau3_tj1_start_diff")->Fill(1., weight);
    if(tau3_common >= 0){
      hist("tau3_tj1_calc_common")->Fill(tau3_common, weight);
      double ratio = (topjet.at(0).tau3()-tau3_common)/topjet.at(0).tau3();
      hist("tau3_tj1_common_diff")->Fill(ratio, weight);
    }
    else hist("tau3_tj1_common_diff")->Fill(1., weight);
    if(tau3_corrector >= 0){
      hist("tau3_tj1_calc_corrector")->Fill(tau3_corrector, weight);
      double ratio = (topjet.at(0).tau3()-tau3_corrector)/topjet.at(0).tau3();
      hist("tau3_tj1_corrector_diff")->Fill(ratio, weight);
    }
    else hist("tau3_tj1_corrector_diff")->Fill(1., weight);
    if(tau3_lepcleaner >= 0){
      hist("tau3_tj1_calc_lepcleaner")->Fill(tau3_lepcleaner, weight);
      double ratio = (topjet.at(0).tau3()-tau3_lepcleaner)/topjet.at(0).tau3();
      hist("tau3_tj1_lepcleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau3_tj1_lepcleaner_diff")->Fill(1., weight);
    if(tau3_cleaner >= 0){
      hist("tau3_tj1_calc_cleaner")->Fill(tau3_cleaner, weight);
      double ratio = (topjet.at(0).tau3()-tau3_cleaner)/topjet.at(0).tau3();
      hist("tau3_tj1_cleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau3_tj1_cleaner_diff")->Fill(1., weight);


    hist("tau3_tj1_rebin")->Fill(topjet.at(0).tau3(), weight);
    hist("tau4_tj1_rebin")->Fill(topjet.at(0).tau4(), weight);
    hist("tau32_tj1")->Fill(topjet.at(0).tau3()/topjet.at(0).tau2(), weight);
    hist("tau32_tj1_groomed")->Fill(topjet.at(0).tau3_groomed()/topjet.at(0).tau2_groomed(), weight);
    double tau32_ungroomed = topjet.at(0).tau3()/topjet.at(0).tau2();
    double tau32_groomed = topjet.at(0).tau3_groomed()/topjet.at(0).tau2_groomed();
    double ratio_32_ug = (tau32_ungroomed-tau32_groomed)/tau32_ungroomed;
    hist("tau32_tj1_groomed_diff")->Fill(ratio_32_ug, weight);
    hist("tau32_tj1_rebin1")->Fill(topjet.at(0).tau3()/topjet.at(0).tau2(), weight);
    hist("tau32_tj1_rebin2")->Fill(topjet.at(0).tau3()/topjet.at(0).tau2(), weight);


    if(tau3_start >= 0 && tau2_start >= 0 ){
      hist("tau32_tj1_calc_start")->Fill(tau3_start/tau2_start, weight);
      double tau32_saved = topjet.at(0).tau3()/topjet.at(0).tau2();
      double tau32_calculated = (tau3_start/tau2_start);
      double ratio = (tau32_saved-tau32_calculated)/tau32_saved;
      hist("tau32_tj1_cleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau32_tj1_cleaner_diff")->Fill(1., weight);
    if(tau3_start >= 0 && tau2_start >= 0 )hist("tau32_tj1_calc_start_rebin1")->Fill(tau3_start/tau2_start, weight);
    if(tau3_start >= 0 && tau2_start >= 0 ) hist("tau32_tj1_calc_start_rebin2")->Fill(tau3_start/tau2_start, weight);

    if(tau3_common >= 0 && tau2_common >= 0 ){
      hist("tau32_tj1_calc_common")->Fill(tau3_common/tau2_common, weight);
      double tau32_saved = topjet.at(0).tau3()/topjet.at(0).tau2();
      double tau32_calculated = (tau3_common/tau2_common);
      double ratio = (tau32_saved-tau32_calculated)/tau32_saved;
      hist("tau32_tj1_common_diff")->Fill(ratio, weight);
    }
    else hist("tau32_tj1_common_diff")->Fill(1., weight);
    if(tau3_common >= 0 && tau2_common >= 0 ) hist("tau32_tj1_calc_common_rebin1")->Fill(tau3_common/tau2_common, weight);
    if(tau3_common >= 0 && tau2_common >= 0 ) hist("tau32_tj1_calc_common_rebin2")->Fill(tau3_common/tau2_common, weight);

    if(tau3_corrector >= 0 && tau2_corrector >= 0 ){
      hist("tau32_tj1_calc_corrector")->Fill(tau3_corrector/tau2_corrector, weight);
      double tau32_saved = topjet.at(0).tau3()/topjet.at(0).tau2();
      double tau32_calculated = (tau3_corrector/tau2_corrector);
      double ratio = (tau32_saved-tau32_calculated)/tau32_saved;
      hist("tau32_tj1_corrector_diff")->Fill(ratio, weight);
    }
    else hist("tau32_tj1_corrector_diff")->Fill(1., weight);
    if(tau3_corrector >= 0 && tau2_corrector >= 0 ) hist("tau32_tj1_calc_corrector_rebin1")->Fill(tau3_corrector/tau2_corrector, weight);
    if(tau3_corrector >= 0 && tau2_corrector >= 0 ) hist("tau32_tj1_calc_corrector_rebin2")->Fill(tau3_corrector/tau2_corrector, weight);

    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ){
      hist("tau32_tj1_calc_lepcleaner")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);
      double tau32_saved = topjet.at(0).tau3()/topjet.at(0).tau2();
      double tau32_calculated = (tau3_lepcleaner/tau2_lepcleaner);
      double ratio = (tau32_saved-tau32_calculated)/tau32_saved;
      hist("tau32_tj1_lepcleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau32_tj1_lepcleaner_diff")->Fill(1., weight);
    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ) hist("tau32_tj1_calc_lepcleaner_rebin1")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);
    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ) hist("tau32_tj1_calc_lepcleaner_rebin2")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);

    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ){
      hist("tau32_tj1_calc_cleaner")->Fill(tau3_cleaner/tau2_cleaner, weight);
      double tau32_saved = topjet.at(0).tau3()/topjet.at(0).tau2();
      double tau32_calculated = (tau3_cleaner/tau2_cleaner);
      double ratio = (tau32_saved-tau32_calculated)/tau32_saved;
      hist("tau32_tj1_cleaner_diff")->Fill(ratio, weight);
    }
    else hist("tau32_tj1_cleaner_diff")->Fill(1., weight);
    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ) hist("tau32_tj1_calc_cleaner_rebin1")->Fill(tau3_cleaner/tau2_cleaner, weight);
    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ) hist("tau32_tj1_calc_cleaner_rebin2")->Fill(tau3_cleaner/tau2_cleaner, weight);

    if(tau3_start == -50 || tau3_common == -50 || tau3_corrector == -50 || tau3_lepcleaner == -50 || tau3_cleaner == -50 || tau2_start == -50 || tau2_common == -50 || tau2_corrector == -50 || tau2_lepcleaner == -50 || tau2_cleaner == -50) hist("empty_pf_cand")->Fill(1, weight);

    hist("tau21_tj1")->Fill(topjet.at(0).tau2()/topjet.at(0).tau1(), weight);
    hist("tau21_tj1_groomed")->Fill(topjet.at(0).tau2_groomed()/topjet.at(0).tau1_groomed(), weight);
    double tau21_ungroomed = topjet.at(0).tau2()/topjet.at(0).tau1();
    double tau21_groomed = topjet.at(0).tau2_groomed()/topjet.at(0).tau1_groomed();
    double ratio_21_ug = (tau21_ungroomed-tau21_groomed)/tau21_ungroomed;
    hist("tau21_tj1_groomed_diff")->Fill(ratio_21_ug, weight);
    hist("tau21_tj1_rebin")->Fill(topjet.at(0).tau2()/topjet.at(0).tau1(), weight);


    hist("tau31_tj1")->Fill(topjet.at(0).tau3()/topjet.at(0).tau1(), weight);
    hist("tau31_tj1_rebin1")->Fill(topjet.at(0).tau3()/topjet.at(0).tau1(), weight);
    hist("tau31_tj1_rebin2")->Fill(topjet.at(0).tau3()/topjet.at(0).tau1(), weight);
    hist("tau31_tj1_groomed")->Fill(topjet.at(0).tau3_groomed()/topjet.at(0).tau1_groomed(), weight);
    hist("tau41_tj1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau1(), weight);
    hist("tau41_tj1_rebin1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau1(), weight);
    hist("tau41_tj1_rebin2")->Fill(topjet.at(0).tau4()/topjet.at(0).tau1(), weight);
    hist("tau41_tj1_groomed")->Fill(topjet.at(0).tau4_groomed()/topjet.at(0).tau1_groomed(), weight);
    hist("tau42_tj1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau2(), weight);
    hist("tau42_tj1_rebin1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau2(), weight);
    hist("tau42_tj1_rebin2")->Fill(topjet.at(0).tau4()/topjet.at(0).tau2(), weight);
    hist("tau42_tj1_groomed")->Fill(topjet.at(0).tau4_groomed()/topjet.at(0).tau2_groomed(), weight);
    hist("tau43_tj1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau3(), weight);
    hist("tau43_tj1_rebin1")->Fill(topjet.at(0).tau4()/topjet.at(0).tau3(), weight);
    hist("tau43_tj1_rebin2")->Fill(topjet.at(0).tau4()/topjet.at(0).tau3(), weight);
    hist("tau43_tj1_groomed")->Fill(topjet.at(0).tau4_groomed()/topjet.at(0).tau3_groomed(), weight);


    hist("ecfN2_beta1_tj1")->Fill(topjet.at(0).ecfN2_beta1(), weight);
    hist("ecfN2_beta1_tj1_rebin")->Fill(topjet.at(0).ecfN2_beta1(), weight);
    hist("ecfN2_beta2_tj1")->Fill(topjet.at(0).ecfN2_beta2(), weight);
    hist("ecfN2_beta2_tj1_rebin")->Fill(topjet.at(0).ecfN2_beta2(), weight);
    hist("ecfN3_beta1_tj1")->Fill(topjet.at(0).ecfN3_beta1(), weight);
    hist("ecfN3_beta1_tj1_rebin")->Fill(topjet.at(0).ecfN3_beta1(), weight);
    hist("ecfN3_beta2_tj1")->Fill(topjet.at(0).ecfN3_beta2(), weight);
    hist("ecfN3_beta2_tj1_rebin")->Fill(topjet.at(0).ecfN3_beta2(), weight);

    if(channel_ == muon && event.muons->size() > 0)         hist("dR_lep_tj1")->Fill(deltaR(event.muons->at(0), topjet.at(0)), weight);
    else if(channel_ == ele && event.electrons->size() > 0) hist("dR_lep_tj1")->Fill(deltaR(event.electrons->at(0), topjet.at(0)), weight);
    if(channel_ == none && event.muons->size() > 0)         hist("dR_mu_tj1")->Fill(deltaR(event.muons->at(0), topjet.at(0)), weight);
    if(channel_ == none && event.electrons->size() > 0)hist("dR_ele_tj1")->Fill(deltaR(event.electrons->at(0), topjet.at(0)), weight);

    if(channel_ == muon && event.muons->size() > 0)         hist("dPhi_lep_tj1")->Fill(deltaPhi(event.muons->at(0), topjet.at(0)), weight);
    else if(channel_ == ele && event.electrons->size() > 0) hist("dPhi_lep_tj1")->Fill(deltaPhi(event.electrons->at(0), topjet.at(0)), weight);
    if(channel_ == none && event.muons->size() > 0)         hist("dPhi_mu_tj1")->Fill(deltaPhi(event.muons->at(0), topjet.at(0)), weight);
    if(channel_ == none && event.electrons->size() > 0)hist("dPhi_ele_tj1")->Fill(deltaPhi(event.electrons->at(0), topjet.at(0)), weight);

    if(topjet.size() > 1){
      if(channel_ == muon && event.muons->size() > 0)         hist("dR_lep_tj2")->Fill(deltaR(event.muons->at(0), topjet.at(1)), weight);
      else if(channel_ == ele && event.electrons->size() > 0) hist("dR_lep_tj2")->Fill(deltaR(event.electrons->at(0), topjet.at(1)), weight);
      if(channel_ == none && event.muons->size() > 0)         hist("dR_mu_tj2")->Fill(deltaR(event.muons->at(0), topjet.at(1)), weight);
      if(channel_ == none && event.electrons->size() > 0)hist("dR_ele_tj2")->Fill(deltaR(event.electrons->at(0), topjet.at(1)), weight);

      if(channel_ == muon && event.muons->size() > 0)         hist("dPhi_lep_tj2")->Fill(deltaPhi(event.muons->at(0), topjet.at(1)), weight);
      else if(channel_ == ele && event.electrons->size() > 0) hist("dPhi_lep_tj2")->Fill(deltaPhi(event.electrons->at(0), topjet.at(1)), weight);
      if(channel_ == none && event.muons->size() > 0)         hist("dPhi_mu_tj2")->Fill(deltaPhi(event.muons->at(0), topjet.at(1)), weight);
      if(channel_ == none && event.electrons->size() > 0)hist("dPhi_ele_tj2")->Fill(deltaPhi(event.electrons->at(0), topjet.at(1)), weight);

      hist("M_tj2")->Fill(topjet.at(1).v4().M(), weight);
      hist("M_tj2_rebin")->Fill(topjet.at(1).v4().M(), weight);
      hist("pt_tj2")->Fill(topjet.at(1).pt(), weight);
      hist("eta_tj2")->Fill(topjet.at(1).eta(), weight);
      hist("Nsubjet_tj2")->Fill(topjet.at(1).subjets().size(), weight);
      if(topjet.at(1).subjets().size() >= 1) hist("pt_subjet1_tj2")->Fill(topjet.at(1).subjets().at(0).v4().pt(), weight);
      if(topjet.at(1).subjets().size() >= 2) hist("pt_subjet2_tj2")->Fill(topjet.at(1).subjets().at(1).v4().pt(), weight);
      if(topjet.at(1).subjets().size() >= 1)  hist("m_subjet1_tj2")->Fill(topjet.at(1).subjets().at(0).v4().M(), weight);
      if(topjet.at(1).subjets().size() >= 2)  hist("m_subjet2_tj2")->Fill(topjet.at(1).subjets().at(1).v4().M(), weight);
      hist("tau1_tj2")->Fill(topjet.at(1).tau1(), weight);
      hist("tau1_tj2_rebin")->Fill(topjet.at(1).tau1(), weight);
      hist("tau2_tj2")->Fill(topjet.at(1).tau2(), weight);
      hist("tau2_tj2_rebin")->Fill(topjet.at(1).tau2(), weight);
      hist("tau3_tj2")->Fill(topjet.at(1).tau3(), weight);
      hist("tau3_tj2_rebin")->Fill(topjet.at(1).tau3(), weight);
      hist("tau32_tj2")->Fill(topjet.at(1).tau3()/topjet.at(1).tau2(), weight);
      hist("tau32_tj2_rebin1")->Fill(topjet.at(1).tau3()/topjet.at(1).tau2(), weight);
      hist("tau32_tj2_rebin2")->Fill(topjet.at(1).tau3()/topjet.at(1).tau2(), weight);
      hist("tau21_tj2")->Fill(topjet.at(1).tau2()/topjet.at(1).tau1(), weight);
      hist("tau21_tj2_rebin")->Fill(topjet.at(1).tau2()/topjet.at(1).tau1(), weight);


      hist("M_diff1")->Fill(topjet.at(0).v4().M() - topjet.at(1).v4().M(), weight);

      LorentzVector subjet_sum2;
      for (const auto s : topjet.at(1).subjets()) {
        subjet_sum2 += s.v4();
      }
      double mass2 = subjet_sum2.M();
      hist("M_tj2_groomed")->Fill(mass2, weight);

      hist("M_diff1_groomed")->Fill(mass1 - mass2, weight);

      if(channel_ == muon && event.muons->size() > 0){
        const auto dummy_mass = topjet.at(1).v4() + event.muons->at(0).v4();
        hist("M_diff2")->Fill(topjet.at(0).v4().M() - dummy_mass.M(), weight);
        subjet_sum2 += event.muons->at(0).v4();
        double mass2_lep = subjet_sum2.M();
        hist("M_diff2_groomed")->Fill(mass1 - mass2_lep, weight);
      }
      else if(channel_ == ele && event.electrons->size() > 0){
        const auto dummy_mass = topjet.at(1).v4() + event.electrons->at(0).v4();
        hist("M_diff2")->Fill(topjet.at(0).v4().M() - dummy_mass.M(), weight);
        subjet_sum2 += event.electrons->at(0).v4();
        double mass2_lep = subjet_sum2.M();
        hist("M_diff2_groomed")->Fill(mass1 - mass2_lep, weight);
      }

      if(channel_ == none && event.muons->size() > 0){
        const auto dummy_mass = topjet.at(1).v4() + event.muons->at(0).v4();
        hist("M_diff2_mu")->Fill(topjet.at(0).v4().M() - dummy_mass.M(), weight);
        subjet_sum2 += event.muons->at(0).v4();
        double mass2_lep = subjet_sum2.M();
        hist("M_diff2_groomed_mu")->Fill(mass1 - mass2_lep, weight);
      }
      if(channel_ == none && event.electrons->size() > 0){
        const auto dummy_mass = topjet.at(1).v4() + event.electrons->at(0).v4();
        hist("M_diff2_ele")->Fill(topjet.at(0).v4().M() - dummy_mass.M(), weight);
        subjet_sum2 += event.electrons->at(0).v4();
        double mass2_lep = subjet_sum2.M();
        hist("M_diff2_groomed_ele")->Fill(mass1 - mass2_lep, weight);
      }



    }
  } // closing brackets of TopJet
  hist("N_mu")->Fill(event.muons->size(), weight);
  hist("N_ele")->Fill(event.electrons->size(), weight);
  if(channel_ == muon && event.muons->size() > 0) hist("pt_lep")->Fill(event.muons->at(0).pt(), weight);
  else if(channel_ == ele && event.electrons->size() > 0) hist("pt_lep")->Fill(event.electrons->at(0).pt(), weight);
  if(channel_ == none && event.muons->size() > 0) hist("pt_mu")->Fill(event.muons->at(0).pt(), weight);
  if(channel_ == none && event.electrons->size() > 0) hist("pt_ele")->Fill(event.electrons->at(0).pt(), weight);

  if(channel_ == muon && event.muons->size() > 0) hist("eta_lep")->Fill(event.muons->at(0).eta(), weight);
  else if(channel_ == ele && event.electrons->size() > 0) hist("eta_lep")->Fill(event.electrons->at(0).eta(), weight);
  if(channel_ == none && event.muons->size() > 0) hist("eta_mu")->Fill(event.muons->at(0).eta(), weight);
  if(channel_ == none && event.electrons->size() > 0) hist("eta_ele")->Fill(event.electrons->at(0).eta(), weight);

  if(channel_ == muon && event.muons->size() > 0) hist("mass_lep")->Fill(event.muons->at(0).v4().M(), weight);
  if(channel_ == ele && event.electrons->size() > 0) hist("mass_lep")->Fill(event.electrons->at(0).v4().M(), weight);
  if(channel_ == none && event.muons->size() > 0) hist("mass_mu")->Fill(event.muons->at(0).v4().M(), weight);
  if(channel_ == none && event.electrons->size() > 0) hist("mass_ele")->Fill(event.electrons->at(0).v4().M(), weight);

  hist("E_Tmiss")->Fill(event.met->pt(), weight);
}
