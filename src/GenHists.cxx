#include "UHH2/TopSubstructure/include/GenHists.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

GenHists::GenHists(Context & ctx, const string & dirname, std::string const & label): Hists(ctx, dirname), hndl(ctx.get_handle<vector<GenTopJet>>(label)){
  // book all histograms here
  // jets
  book<TH1D>("N_jets", "N_{GenJets}", 21, -0.5, 20.5);
  book<TH1D>("eta_jet1", "#eta^{GenJet 1}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet2", "#eta^{GenJet 2}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet3", "#eta^{GenJet 3}", 40, -2.5, 2.5);
  book<TH1D>("eta_jet4", "#eta^{GenJet 4}", 40, -2.5, 2.5);
  book<TH1D>("pt_jet1", "p_{T}^{GenJet 1}", 120, 0, 1200);
  book<TH1D>("pt_jet2", "p_{T}^{GenJet 2}", 120, 0, 1200);
  book<TH1D>("pt_jet3", "p_{T}^{GenJet 3}", 120, 0, 1200);
  book<TH1D>("pt_jet4", "p_{T}^{GenJet 4}", 120, 0, 1200);
  book<TH1D>("N_topjets", "N_{GenTopJets}", 10, -0.5, 9.5);

  // TopJets
  book<TH1D>("M_diff1", "M_{first GenTopJet} - M_{second GenTopJet} [GeV^{2}]", 80, -400, 400);
  book<TH1D>("M_diff2", "M_{first GenTopJet} - M_{second GenTopJet + Muon} [GeV^{2}]", 80, -400, 400);
  book<TH1D>("M_diff2_ele", "M_{first GenTopJet} - M_{second GenTopJet + Electron} [GeV^{2}]", 80, -400, 400);

  book<TH1D>("top_pt", "p_{T} top-quark [GeV]", 120, 0, 1200);
  book<TH1D>("top1_pt", "p_{T} top-quark (first) [GeV]", 120, 0, 1200);
  book<TH1D>("top2_pt", "p_{T} top-quark (second) [GeV]", 120, 0, 1200);
  book<TH1D>("M_ttbar", "M_{tt} [GeV^{2}]", 200, 0, 2000);


  // first candidate
  book<TH1D>("M_gtj1", "M_{first GenTopJet} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_gtj1_rebin", "M_{first GenTopJet} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_gtj1", "p_{T} first GenTopJet [GeV]", 120, 0, 1200);
  book<TH1D>("eta_gtj1", "#eta first GenTopJet", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_all", "N_{subjets} all GenTopJets", 4, -0.5, 3.5);
  book<TH1D>("Nsubjet_gtj1", "N_{subjets} first GenTopJet", 4, -0.5, 3.5);
  book<TH1D>("pt_gtj1_subjet1", "p_{T}^{subjet1} first GenTopJet [GeV]", 120, 0, 1200);
  book<TH1D>("pt_gtj1_subjet2", "p_{T}^{subjet2} first GenTopJet [GeV]", 80, 0, 800);
  book<TH1D>("m_gtj1_subjet1", "M_{subjet1} first GenTopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("m_gtj1_subjet2", "M_{subjet2} first GenTopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("tau1_gtj1", "#tau_{1} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau1_gtj1_rebin", "#tau_{1} first GenTopJet", 1000, 0, 1.);

  book<TH1D>("tau2_gtj1", "#tau_{2} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau2_gtj1_rebin", "#tau_{2} first GenTopJet", 1000, 0, 1.);

  book<TH1D>("tau2_gtj1_calc_start", "#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau2_gtj1_calc_lepcleaner", "#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau2_gtj1_calc_cleaner", "#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);

  book<TH1D>("tau3_gtj1", "#tau_{3} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau3_gtj1_rebin", "#tau_{3} first GenTopJet", 1000, 0, 1.);

  book<TH1D>("tau3_gtj1_calc_start", "#tau_{3} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau3_gtj1_calc_lepcleaner", "#tau_{3} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau3_gtj1_calc_cleaner", "#tau_{3} first GenTopJet (calculated)", 40, 0, 1.);

  book<TH1D>("tau4_gtj1", "#tau_{4} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau4_gtj1_rebin", "#tau_{4} first GenTopJet", 1000, 0, 1.);

  book<TH1D>("tau32_gtj1", "#tau_{3}/#tau_{2} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau32_gtj1_rebin1", "#tau_{3}/#tau_{2} first GenTopJet", 100, 0, 1.);
  book<TH1D>("tau32_gtj1_rebin2", "#tau_{3}/#tau_{2} first GenTopJet", 1000, 0, 1.);


  book<TH1D>("tau32_gtj1_calc_start", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_start_rebin1", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 100, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_start_rebin2", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 1000, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_lepcleaner", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_lepcleaner_rebin1", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 100, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_lepcleaner_rebin2", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 1000, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_cleaner", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 40, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_cleaner_rebin1", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 100, 0, 1.);
  book<TH1D>("tau32_gtj1_calc_cleaner_rebin2", "#tau_{3}/#tau_{2} first GenTopJet (calculated)", 1000, 0, 1.);


  book<TH1D>("tau21_gtj1", "#tau_{2}/#tau_{1} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau21_gtj1_rebin", "#tau_{2}/#tau_{1} first GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau31_gtj1", "#tau_{3}/#tau_{1} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau31_gtj1_rebin", "#tau_{3}/#tau_{1} first GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau41_gtj1", "#tau_{4}/#tau_{1} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau41_gtj1_rebin", "#tau_{4}/#tau_{1} first GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau42_gtj1", "#tau_{4}/#tau_{2} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau42_gtj1_rebin", "#tau_{4}/#tau_{2} first GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau43_gtj1", "#tau_{4}/#tau_{3} first GenTopJet", 40, 0, 1.);
  book<TH1D>("tau43_gtj1_rebin", "#tau_{4}/#tau_{3} first GenTopJet", 1000, 0, 1.);



  book<TH1D>("ecfN2_beta1_gtj1", "ecfN2_beta1 first GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta1_gtj1_rebin", "ecfN2_beta1 first GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN2_beta2_gtj1", "ecfN2_beta2 first GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta2_gtj1_rebin", "ecfN2_beta2 first GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta1_gtj1", "ecfN3_beta1 first GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta1_gtj1_rebin", "ecfN3_beta1 first GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta2_gtj1", "ecfN3_beta2 first GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta2_gtj1_rebin", "ecfN3_beta2 first GenTopJet", 1000, 0, 1.);



  book<TH1D>("dR_q1_gtj1", "#Delta R(quark1, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_q2_gtj1", "#Delta R(quark2, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_bqhad_gtj1", "#Delta R(b-quark (had), first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_whad_gtj1", "#Delta R(Whad, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dPhi_q1_gtj1", "#Delta #Phi (quark1, first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_q2_gtj1", "#Delta #Phi (quark2, first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_bqhad_gtj1", "#Delta #Phi (b-quark (had), first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_whad_gtj1", "#Delta #Phi (Whad, first GenTopJet)", 40, 0, 4.);

  book<TH1D>("dR_mu_gtj1", "#Delta R(#mu, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_nu_gtj1", "#Delta R(#nu_{#mu}, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_bqlep_gtj1", "#Delta R(b-quark (lep), first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_wlep_gtj1", "#Delta R(Wlep, first GenTopJet)", 50, 0, 5.);
  book<TH1D>("dPhi_mu_gtj1", "#Delta #Phi (#mu, first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_nu_gtj1", "#Delta #Phi (#nu_{#mu}, first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_gtj1", "#Delta #Phi (b-quark (lep), first GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_wlep_gtj1", "#Delta #Phi (Wlep, first GenTopJet)", 40, 0, 4.);


  //  second candidate
  book<TH1D>("M_gtj2", "M_{second GenTopJet} [GeV^{2}]", 40, 0, 400);
  book<TH1D>("M_gtj2_rebin", "M_{second GenTopJet} [GeV^{2}]", 80, 0, 400);
  book<TH1D>("pt_gtj2", "p_{T} second GenTopJet [GeV]", 120, 0, 1200);
  book<TH1D>("eta_gtj2", "#eta second GenTopJet", 40, -2.5, 2.5);
  book<TH1D>("Nsubjet_gtj2", "N_{subjets} second GenTopJet", 4, -0.5, 3.5);
  book<TH1D>("pt_gtj2_subjet1", "p_{T}^{subjet1} second GenTopJet [GeV]", 120, 0, 1200);
  book<TH1D>("pt_gtj2_subjet2", "p_{T}^{subjet2} second GenTopJet [GeV]", 80, 0, 800);
  book<TH1D>("m_gtj2_subjet1", "M_{subjet1} second GenTopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("m_gtj2_subjet2", "M_{subjet2} second GenTopJet [GeV^{2}]", 30, 0, 300);
  book<TH1D>("tau1_gtj2", "#tau_{1} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau1_gtj2_rebin", "#tau_{1} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau2_gtj2", "#tau_{2} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau2_gtj2_rebin", "#tau_{2} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau3_gtj2", "#tau_{3} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau3_gtj2_rebin", "#tau_{3} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau4_gtj2", "#tau_{4} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau4_gtj2_rebin", "#tau_{4} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau32_gtj2", "#tau_{3}/#tau_{2} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau32_gtj2_rebin1", "#tau_{3}/#tau_{2} second GenTopJet", 100, 0, 1.);
  book<TH1D>("tau32_gtj2_rebin2", "#tau_{3}/#tau_{2} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau21_gtj2", "#tau_{2}/#tau_{1} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau21_gtj2_rebin", "#tau_{2}/#tau_{1} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau31_gtj2", "#tau_{3}/#tau_{1} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau31_gtj2_rebin", "#tau_{3}/#tau_{1} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau41_gtj2", "#tau_{4}/#tau_{1} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau41_gtj2_rebin", "#tau_{4}/#tau_{1} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau42_gtj2", "#tau_{4}/#tau_{2} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau42_gtj2_rebin", "#tau_{4}/#tau_{2} second GenTopJet", 1000, 0, 1.);
  book<TH1D>("tau43_gtj2", "#tau_{4}/#tau_{3} second GenTopJet", 40, 0, 1.);
  book<TH1D>("tau43_gtj2_rebin", "#tau_{4}/#tau_{3} second GenTopJet", 1000, 0, 1.);

  book<TH1D>("ecfN2_beta1_gtj2", "ecfN2_beta1 second GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta1_gtj2_rebin", "ecfN2_beta1 second GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN2_beta2_gtj2", "ecfN2_beta2 second GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN2_beta2_gtj2_rebin", "ecfN2_beta2 second GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta1_gtj2", "ecfN3_beta1 second GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta1_gtj2_rebin", "ecfN3_beta1 second GenTopJet", 1000, 0, 1.);
  book<TH1D>("ecfN3_beta2_gtj2", "ecfN3_beta2 second GenTopJet", 40, 0, 1.);
  book<TH1D>("ecfN3_beta2_gtj2_rebin", "ecfN3_beta2 second GenTopJet", 1000, 0, 1.);

  book<TH1D>("dR_q1_gtj2", "#Delta R(quark1, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_q2_gtj2", "#Delta R(quark2, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_bqhad_gtj2", "#Delta  R(b-quark (had), second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_whad_gtj2", "#Delta  R(Whad, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dPhi_q1_gtj2", "#Delta #Phi (quark1, second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_q2_gtj2", "#Delta #Phi (quark2, second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_bqhad_gtj2", "#Delta #Phi (b-quark (had), second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_whad_gtj2", "#Delta #Phi (Whad, second GenTopJet)", 40, 0, 4.);

  book<TH1D>("dR_mu_gtj2", "#Delta R(#mu, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_nu_gtj2", "#Delta R(#nu_{#mu}, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_bqlep_gtj2", "#Delta R(b-quark (lep), second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dR_wlep_gtj2", "#Delta  R(Wlep, second GenTopJet)", 50, 0, 5.);
  book<TH1D>("dPhi_mu_gtj2", "#Delta #Phi (#mu, second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_nu_gtj2", "#Delta #Phi (#nu_{#mu}, second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_gtj2", "#Delta #Phi (b-quark (lep), second GenTopJet)", 40, 0, 4.);
  book<TH1D>("dPhi_wlep_gtj2", "#Delta #Phi (Wlep, second GenTopJet)", 40, 0, 4.);


  book<TH1D>("dR8_top_gtj1", "#Delta R(top, first GenTopJet)", 50, 0, 5.);

  book<TH1D>("dR_q1_q2", "#Delta R(quark1, quark2)", 50, 0, 5.);
  book<TH1D>("dR_bqhad_q1", "#Delta R(b-quark, quark1)", 50, 0, 5.);
  book<TH1D>("dR_bqhad_q2", "#Delta R(b-quark, quark2)", 50, 0, 5.);
  book<TH1D>("dPhi_q1_q2", "#Delta #Phi (quark1, quark2)", 40, 0, 4.);
  book<TH1D>("dPhi_bqhad_q1", "#Delta #Phi (b-quark, quark1)", 40, 0, 4.);
  book<TH1D>("dPhi_bqhad_q2", "#Delta #Phi (b-quark, quark2)", 40, 0, 4.);

  book<TH1D>("dR_mu_nu", "#Delta R(#mu, #nu_{#mu})", 50, 0, 5.);
  book<TH1D>("dR_bqlep_mu", "#Delta R(b-quark (lep), #mu)", 50, 0, 5.);
  book<TH1D>("dR_bqlep_nu", "#Delta R(b-quark (lep), #nu_{#mu})", 50, 0, 5.);
  book<TH1D>("dPhi_mu_nu", "#Delta #Phi (#mu, #nu_{#mu})", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_mu", "#Delta #Phi (b-quark (lep), #mu)", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_nu", "#Delta #Phi (b-quark (lep), #nu_{#mu})", 40, 0, 4.);


  book<TH1D>("dR_whad_bqhad", "#Delta R(Whad, b-quark had)", 50, 0, 5.);
  book<TH1D>("dR_whad_bqlep", "#Delta R(Whad, b-quark lep)", 50, 0, 5.);
  book<TH1D>("dR_whad_wlep", "#Delta R(Whad, Wlep)", 50, 0, 5.);
  book<TH1D>("dR_wlep_bqhad", "#Delta R(Wlep, b-quark had)", 50, 0, 5.);
  book<TH1D>("dR_wlep_bqlep", "#Delta R(Wlep, b-quark lep)", 50, 0, 5.);
  book<TH1D>("dPhi_whad_bqhad", "#Delta #Phi (Whad, b-quark had)", 40, 0, 4.);
  book<TH1D>("dPhi_whad_bqlep", "#Delta #Phi (Whad, b-quark lep)", 40, 0, 4.);
  book<TH1D>("dPhi_whad_wlep", "#Delta #Phi (Whad, Wlep)", 40, 0, 4.);
  book<TH1D>("dPhi_wlep_bqhad", "#Delta #Phi (Wlep, b-quark had)", 40, 0, 4.);
  book<TH1D>("dPhi_wlep_bqlep", "#Delta #Phi (Wlep, b-quark lep)", 40, 0, 4.);

  book<TH1D>("dR_electron_gtj1_ele", "#Delta R(electron, first GenTopJet) (ele)", 50, 0, 5.);
  book<TH1D>("dR_nu_gtj1_ele", "#Delta R(#nu_{electron}, first GenTopJet) (ele)", 50, 0, 5.);
  book<TH1D>("dPhi_electron_gtj1_ele", "#Delta #Phi (electron, first GenTopJet) (ele)", 40, 0, 4.);
  book<TH1D>("dPhi_nu_gtj1_ele", "#Delta #Phi (#nu_{electron}, first GenTopJet) (ele)", 40, 0, 4.);

  book<TH1D>("dR_electron_gtj2_ele", "#Delta R(electron, second GenTopJet) (ele)", 50, 0, 5.);
  book<TH1D>("dR_nu_gtj2_ele", "#Delta R(#nu_{electron}, second GenTopJet) (ele)", 50, 0, 5.);
  book<TH1D>("dPhi_electron_gtj2_ele", "#Delta #Phi (electron, second GenTopJet) (ele)", 40, 0, 4.);
  book<TH1D>("dPhi_nu_gtj2_ele", "#Delta #Phi (#nu_{electron}, second GenTopJet) (ele)", 40, 0, 4.);

  book<TH1D>("dR_electron_nu_ele", "#Delta R(electron, #nu_{electron}) (ele)", 50, 0, 5.);
  book<TH1D>("dR_bqlep_electron_ele", "#Delta R(b-quark (lep), electron) (ele)", 50, 0, 5.);
  book<TH1D>("dR_bqlep_nu_ele", "#Delta R(b-quark (lep), #nu_{electron}) (ele)", 50, 0, 5.);
  book<TH1D>("dPhi_electron_nu_ele", "#Delta #Phi (electron, #nu_{electron}) (ele)", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_electron_ele", "#Delta #Phi (b-quark (lep), electron) (ele)", 40, 0, 4.);
  book<TH1D>("dPhi_bqlep_nu_ele", "#Delta #Phi (b-quark (lep), #nu_{electron}) (ele)", 40, 0, 4.);




  // leptons
  book<TH1D>("pt_mu", "p_{T}^{#mu} [GeV]", 100, 0, 1000);
  book<TH1D>("eta_mu", "#eta^{#mu}", 40, -2.5, 2.5);
  book<TH1D>("mass_mu", "M^{#mu} [GeV]", 50, 0, 0.5);

  book<TH1D>("pt_ele", "p_{T}^{electron} [GeV]", 100, 0, 1000);
  book<TH1D>("eta_ele", "#eta^{electron}", 40, -2.5, 2.5);
  book<TH1D>("mass_ele", "M^{electron} [GeV]", 50, 0, 0.5);


  //general
  book<TH1D>("E_Tmiss", "missing E_{T} [GeV]", 75, 0, 1500);
  book<TH1D>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);
  book<TH1D>("empty_gen_cand", "BinContent = sum(eventweights)", 1, 0.5, 1.5);


  // get handles
  h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
  h_weight = ctx.get_handle<double>("h_gen_weight");
  h_weight_kin = ctx.get_handle<double>("h_gen_weight_kin");

  if (label.find("sd") != std::string::npos) {
    h_tau2_start = ctx.get_handle<double>("h_gen_tau2_start_sd");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_gen_tau2_lepcleaner_sd");
    h_tau2_cleaner = ctx.get_handle<double>("h_gen_tau2_cleaner_sd");
    h_tau3_start = ctx.get_handle<double>("h_gen_tau3_start_sd");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_gen_tau3_lepcleaner_sd");
    h_tau3_cleaner = ctx.get_handle<double>("h_gen_tau3_cleaner_sd");
  }
  else{
    h_tau2_start = ctx.get_handle<double>("h_gen_tau2_start");
    h_tau2_lepcleaner = ctx.get_handle<double>("h_gen_tau2_lepcleaner");
    h_tau2_cleaner = ctx.get_handle<double>("h_gen_tau2_cleaner");
    h_tau3_start = ctx.get_handle<double>("h_gen_tau3_start");
    h_tau3_lepcleaner = ctx.get_handle<double>("h_gen_tau3_lepcleaner");
    h_tau3_cleaner = ctx.get_handle<double>("h_gen_tau3_cleaner");
  }
}


void GenHists::fill(const Event & event){
  // Don't forget to always use the weight when filling.
  double weight;
  if(event.is_valid(h_weight))  weight = event.get(h_weight);
  else if(event.is_valid(h_weight_kin))  weight = event.get(h_weight_kin);
  else weight = event.weight;

  double tau3_start = -100;
  double tau3_lepcleaner = -100;
  double tau3_cleaner = -100;
  if(event.is_valid(h_tau3_start))      tau3_start      = event.get(h_tau3_start);
  if(event.is_valid(h_tau3_lepcleaner)) tau3_lepcleaner = event.get(h_tau3_lepcleaner);
  if(event.is_valid(h_tau3_cleaner))    tau3_cleaner    = event.get(h_tau3_cleaner);

  double tau2_start = -100;
  double tau2_lepcleaner = -100;
  double tau2_cleaner = -100;
  if(event.is_valid(h_tau2_start))      tau2_start      = event.get(h_tau2_start);
  if(event.is_valid(h_tau2_lepcleaner)) tau2_lepcleaner = event.get(h_tau2_lepcleaner);
  if(event.is_valid(h_tau2_cleaner))    tau2_cleaner    = event.get(h_tau2_cleaner);



  if (!event.is_valid(hndl)) {
    std::cerr << "In TopSubstructureRecoHists: Handle not valid!\n";
    assert(false);
  }

  vector<GenTopJet> gentopjet = event.get(hndl);

  //general
  hist("sum_event_weights")->Fill(1, weight);

  // Jets
  std::vector<GenJet>* jets = event.genjets;
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
  hist("N_topjets")->Fill(gentopjet.size(), weight);


  // GenTopJet
  if(gentopjet.size() > 0){
    for(unsigned int i=0; i < gentopjet.size(); i++){
      hist("Nsubjet_all")->Fill(gentopjet.at(i).subjets().size(), weight);
    }

    hist("M_gtj1")->Fill(gentopjet.at(0).v4().M(), weight);
    hist("M_gtj1_rebin")->Fill(gentopjet.at(0).v4().M(), weight);
    hist("pt_gtj1")->Fill(gentopjet.at(0).pt(), weight);
    hist("eta_gtj1")->Fill(gentopjet.at(0).eta(), weight);
    hist("Nsubjet_gtj1")->Fill(gentopjet.at(0).subjets().size(), weight);
    if(gentopjet.at(0).subjets().size() > 0) hist("pt_gtj1_subjet1")->Fill(gentopjet.at(0).subjets().at(0).v4().pt(), weight);
    if(gentopjet.at(0).subjets().size() >= 2) hist("pt_gtj1_subjet2")->Fill(gentopjet.at(0).subjets().at(1).v4().pt(), weight);
    if(gentopjet.at(0).subjets().size() > 0) hist("m_gtj1_subjet1")->Fill(gentopjet.at(0).subjets().at(0).v4().M(), weight);
    if(gentopjet.at(0).subjets().size() >= 2) hist("m_gtj1_subjet2")->Fill(gentopjet.at(0).subjets().at(1).v4().M(), weight);
    hist("tau1_gtj1")->Fill(gentopjet.at(0).tau1(), weight);
    hist("tau1_gtj1_rebin")->Fill(gentopjet.at(0).tau1(), weight);
    hist("tau2_gtj1")->Fill(gentopjet.at(0).tau2(), weight);
    hist("tau2_gtj1_rebin")->Fill(gentopjet.at(0).tau2(), weight);
    hist("tau3_gtj1")->Fill(gentopjet.at(0).tau3(), weight);
    hist("tau3_gtj1_rebin")->Fill(gentopjet.at(0).tau3(), weight);
    hist("tau4_gtj1")->Fill(gentopjet.at(0).tau4(), weight);
    hist("tau4_gtj1_rebin")->Fill(gentopjet.at(0).tau4(), weight);

    if(tau2_start >= 0) hist("tau2_gtj1_calc_start")->Fill(tau2_start, weight);
    if(tau2_lepcleaner >= 0) hist("tau2_gtj1_calc_lepcleaner")->Fill(tau2_lepcleaner, weight);
    if(tau2_cleaner >= 0) hist("tau2_gtj1_calc_cleaner")->Fill(tau2_cleaner, weight);

    if(tau3_start >= 0) hist("tau3_gtj1_calc_start")->Fill(tau3_start, weight);
    if(tau3_lepcleaner >= 0) hist("tau3_gtj1_calc_lepcleaner")->Fill(tau3_lepcleaner, weight);
    if(tau3_cleaner >= 0) hist("tau3_gtj1_calc_cleaner")->Fill(tau3_cleaner, weight);

    hist("tau32_gtj1")->Fill(gentopjet.at(0).tau3()/gentopjet.at(0).tau2(), weight);
    hist("tau32_gtj1_rebin1")->Fill(gentopjet.at(0).tau3()/gentopjet.at(0).tau2(), weight);
    hist("tau32_gtj1_rebin2")->Fill(gentopjet.at(0).tau3()/gentopjet.at(0).tau2(), weight);

    if(tau3_start >= 0 && tau2_start >= 0 ) hist("tau32_gtj1_calc_start")->Fill(tau3_start/tau2_start, weight);
    if(tau3_start >= 0 && tau2_start >= 0 ) hist("tau32_gtj1_calc_start_rebin1")->Fill(tau3_start/tau2_start, weight);
    if(tau3_start >= 0 && tau2_start >= 0 ) hist("tau32_gtj1_calc_start_rebin2")->Fill(tau3_start/tau2_start, weight);
    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ) hist("tau32_gtj1_calc_lepcleaner")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);
    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ) hist("tau32_gtj1_calc_lepcleaner_rebin1")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);
    if(tau3_lepcleaner >= 0 && tau2_lepcleaner >= 0 ) hist("tau32_gtj1_calc_lepcleaner_rebin2")->Fill(tau3_lepcleaner/tau2_lepcleaner, weight);
    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ) hist("tau32_gtj1_calc_cleaner")->Fill(tau3_cleaner/tau2_cleaner, weight);
    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ) hist("tau32_gtj1_calc_cleaner_rebin1")->Fill(tau3_cleaner/tau2_cleaner, weight);
    if(tau3_cleaner >= 0 && tau2_cleaner >= 0 ) hist("tau32_gtj1_calc_cleaner_rebin2")->Fill(tau3_cleaner/tau2_cleaner, weight);

    if(tau3_start == -50 || tau3_lepcleaner == -50 || tau3_cleaner == -50 || tau2_start == -50 || tau2_lepcleaner == -50 || tau2_cleaner == -50) hist("empty_gen_cand")->Fill(1, weight);

    hist("tau21_gtj1")->Fill(gentopjet.at(0).tau2()/gentopjet.at(0).tau1(), weight);
    hist("tau21_gtj1_rebin")->Fill(gentopjet.at(0).tau2()/gentopjet.at(0).tau1(), weight);
    hist("tau31_gtj1")->Fill(gentopjet.at(0).tau3()/gentopjet.at(0).tau1(), weight);
    hist("tau31_gtj1_rebin")->Fill(gentopjet.at(0).tau3()/gentopjet.at(0).tau1(), weight);
    hist("tau41_gtj1")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau1(), weight);
    hist("tau41_gtj1_rebin")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau1(), weight);
    hist("tau42_gtj1")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau2(), weight);
    hist("tau42_gtj1_rebin")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau2(), weight);
    hist("tau43_gtj1")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau3(), weight);
    hist("tau43_gtj1_rebin")->Fill(gentopjet.at(0).tau4()/gentopjet.at(0).tau3(), weight);

    hist("ecfN2_beta1_gtj1")->Fill(gentopjet.at(0).ecfN2_beta1(), weight);
    hist("ecfN2_beta1_gtj1_rebin")->Fill(gentopjet.at(0).ecfN2_beta1(), weight);
    hist("ecfN2_beta2_gtj1")->Fill(gentopjet.at(0).ecfN2_beta2(), weight);
    hist("ecfN2_beta2_gtj1_rebin")->Fill(gentopjet.at(0).ecfN2_beta2(), weight);
    hist("ecfN3_beta1_gtj1")->Fill(gentopjet.at(0).ecfN3_beta1(), weight);
    hist("ecfN3_beta1_gtj1_rebin")->Fill(gentopjet.at(0).ecfN3_beta1(), weight);
    hist("ecfN3_beta2_gtj1")->Fill(gentopjet.at(0).ecfN3_beta2(), weight);
    hist("ecfN3_beta2_gtj1_rebin")->Fill(gentopjet.at(0).ecfN3_beta2(), weight);

    if(event.is_valid(h_ttbargen)){
      const auto & ttbargen = event.get(h_ttbargen);
      bool isMu  = ttbargen.DecayChannel() == TTbarGen::e_muhad;
      bool isEle = ttbargen.DecayChannel() == TTbarGen::e_ehad;

      if(isMu || isEle){
        GenParticle tophad, toplep, q1, q2, bhad, blep, lep, nu, whad, wlep;

        tophad = ttbargen.TopHad();
        toplep = ttbargen.TopLep();
        q1 = ttbargen.Q1();
        q2 = ttbargen.Q2();
        bhad = ttbargen.BHad();
        blep = ttbargen.BLep();
        lep = ttbargen.ChargedLepton();
        nu = ttbargen.Neutrino();
        whad = ttbargen.WHad();
        wlep = ttbargen.WLep();


        std::vector<GenParticle> top;
        if(tophad.pt() > toplep.pt()){
          top.push_back(tophad);
          top.push_back(toplep);
        }
        else{
          top.push_back(toplep);
          top.push_back(tophad);
        }
        for(unsigned int i=0; i < top.size(); i++) hist("top_pt")->Fill(top.at(i).pt(), weight);
        hist("top1_pt")->Fill(top.at(0).pt(), weight);
        hist("top2_pt")->Fill(top.at(1).pt(), weight);
        hist("M_ttbar")->Fill((top.at(0).v4()+top.at(1).v4()).M(), weight);

        if(isMu){
                hist("pt_mu")->Fill(lep.pt(), weight);
                hist("eta_mu")->Fill(lep.eta(), weight);
                hist("mass_mu")->Fill(lep.v4().M(), weight);
        }
        else if(isEle){
                hist("pt_ele")->Fill(lep.pt(), weight);
                hist("eta_ele")->Fill(lep.eta(), weight);
                hist("mass_ele")->Fill(lep.v4().M(), weight);

        }
        if(gentopjet.size() > 1){
          hist("M_diff1")->Fill(gentopjet.at(0).v4().M() - gentopjet.at(1).v4().M(), weight);

          const auto dummy_mass = gentopjet.at(1).v4() + lep.v4();
          if(isMu) hist("M_diff2")->Fill(gentopjet.at(0).v4().M() - dummy_mass.M(), weight);
          else if(isEle) hist("M_diff2_ele")->Fill(gentopjet.at(0).v4().M() - dummy_mass.M(), weight);
        }

        hist("E_Tmiss")->Fill(nu.pt(), weight);



        hist("dR8_top_gtj1")->Fill(deltaR(tophad, gentopjet.at(0)), weight);

        hist("dR_q1_gtj1")->Fill(deltaR(q1, gentopjet.at(0)), weight);
        hist("dR_q2_gtj1")->Fill(deltaR(q2, gentopjet.at(0)), weight);
        hist("dR_bqhad_gtj1")->Fill(deltaR(bhad, gentopjet.at(0)), weight);
        hist("dR_whad_gtj1")->Fill(deltaR(whad, gentopjet.at(0)), weight);
        hist("dPhi_q1_gtj1")->Fill(deltaPhi(q1, gentopjet.at(0)), weight);
        hist("dPhi_q2_gtj1")->Fill(deltaPhi(q2, gentopjet.at(0)), weight);
        hist("dPhi_bqhad_gtj1")->Fill(deltaPhi(bhad, gentopjet.at(0)), weight);
        hist("dPhi_whad_gtj1")->Fill(deltaPhi(whad, gentopjet.at(0)), weight);

        hist("dR_mu_gtj1")->Fill(deltaR(lep, gentopjet.at(0)), weight);
        if(isMu) hist("dR_nu_gtj1")->Fill(deltaR(nu, gentopjet.at(0)), weight);
        else if(isEle) hist("dR_nu_gtj1_ele")->Fill(deltaR(nu, gentopjet.at(0)), weight);
        hist("dR_bqlep_gtj1")->Fill(deltaR(blep, gentopjet.at(0)), weight);
        hist("dR_wlep_gtj1")->Fill(deltaR(wlep, gentopjet.at(0)), weight);
        if(isMu) hist("dPhi_mu_gtj1")->Fill(deltaPhi(lep, gentopjet.at(0)), weight);
        else if(isEle) hist("dPhi_electron_gtj1_ele")->Fill(deltaPhi(lep, gentopjet.at(0)), weight);
        if(isMu) hist("dPhi_nu_gtj1")->Fill(deltaPhi(nu, gentopjet.at(0)), weight);
        else if(isEle) hist("dPhi_nu_gtj1_ele")->Fill(deltaPhi(nu, gentopjet.at(0)), weight);
        hist("dPhi_bqlep_gtj1")->Fill(deltaPhi(blep, gentopjet.at(0)), weight);
        hist("dPhi_wlep_gtj1")->Fill(deltaPhi(wlep, gentopjet.at(0)), weight);

        if(gentopjet.size() > 1){
          hist("dR_q1_gtj2")->Fill(deltaR(q1, gentopjet.at(1)), weight);
          hist("dR_q2_gtj2")->Fill(deltaR(q2, gentopjet.at(1)), weight);
          hist("dR_bqhad_gtj2")->Fill(deltaR(bhad, gentopjet.at(1)), weight);
          hist("dR_whad_gtj2")->Fill(deltaR(whad, gentopjet.at(1)), weight);
          hist("dPhi_q1_gtj2")->Fill(deltaPhi(q1, gentopjet.at(1)), weight);
          hist("dPhi_q2_gtj2")->Fill(deltaPhi(q2, gentopjet.at(1)), weight);
          hist("dPhi_bqhad_gtj2")->Fill(deltaPhi(bhad, gentopjet.at(1)), weight);
          hist("dPhi_whad_gtj2")->Fill(deltaPhi(whad, gentopjet.at(1)), weight);

          if(isMu) hist("dR_mu_gtj2")->Fill(deltaR(lep, gentopjet.at(1)), weight);
          else if(isEle) hist("dR_electron_gtj2_ele")->Fill(deltaR(lep, gentopjet.at(1)), weight);
          if(isMu) hist("dR_nu_gtj2")->Fill(deltaR(nu, gentopjet.at(1)), weight);
          else if(isEle) hist("dR_nu_gtj2_ele")->Fill(deltaR(nu, gentopjet.at(1)), weight);
          hist("dR_bqlep_gtj2")->Fill(deltaR(blep, gentopjet.at(1)), weight);
          hist("dR_wlep_gtj2")->Fill(deltaR(wlep, gentopjet.at(1)), weight);
          if(isMu) hist("dPhi_mu_gtj2")->Fill(deltaPhi(lep, gentopjet.at(1)), weight);
          else if(isEle) hist("dPhi_electron_gtj2_ele")->Fill(deltaPhi(lep, gentopjet.at(1)), weight);
          if(isMu) hist("dPhi_nu_gtj2")->Fill(deltaPhi(nu, gentopjet.at(1)), weight);
          else if(isEle) hist("dPhi_nu_gtj2_ele")->Fill(deltaPhi(nu, gentopjet.at(1)), weight);
          hist("dPhi_bqlep_gtj2")->Fill(deltaPhi(blep, gentopjet.at(1)), weight);
          hist("dPhi_wlep_gtj2")->Fill(deltaPhi(wlep, gentopjet.at(1)), weight);
        }

        hist("dR_q1_q2")->Fill(deltaR(q1, q2), weight);
        hist("dR_bqhad_q1")->Fill(deltaR(bhad, q1), weight);
        hist("dR_bqhad_q2")->Fill(deltaR(bhad, q2), weight);
        hist("dR_whad_bqhad")->Fill(deltaR(whad, bhad), weight);
        hist("dPhi_q1_q2")->Fill(deltaPhi(q1, q2), weight);
        hist("dPhi_bqhad_q1")->Fill(deltaPhi(bhad, q1), weight);
        hist("dPhi_bqhad_q2")->Fill(deltaPhi(bhad, q2), weight);
        hist("dPhi_whad_bqhad")->Fill(deltaPhi(whad, bhad), weight);

        if(isMu) hist("dR_mu_nu")->Fill(deltaR(lep, nu), weight);
        else if(isEle) hist("dR_electron_nu_ele")->Fill(deltaR(lep, nu), weight);
        if(isMu) hist("dR_bqlep_mu")->Fill(deltaR(blep, lep), weight);
        else if(isEle) hist("dR_bqlep_electron_ele")->Fill(deltaR(blep, lep), weight);
        if(isMu) hist("dR_bqlep_nu")->Fill(deltaR(blep, nu), weight);
        else if(isEle) hist("dR_bqlep_nu_ele")->Fill(deltaR(blep, nu), weight);
        hist("dR_wlep_bqlep")->Fill(deltaR(wlep, blep), weight);
        if(isMu) hist("dPhi_mu_nu")->Fill(deltaPhi(lep, nu), weight);
        else if(isEle) hist("dPhi_electron_nu_ele")->Fill(deltaPhi(lep, nu), weight);
        if(isMu) hist("dPhi_bqlep_mu")->Fill(deltaPhi(blep, lep), weight);
        else if(isEle) hist("dPhi_bqlep_electron_ele")->Fill(deltaPhi(blep, lep), weight);
        if(isMu) hist("dPhi_bqlep_nu")->Fill(deltaPhi(blep, nu), weight);
        else if(isEle) hist("dPhi_bqlep_nu_ele")->Fill(deltaPhi(blep, nu), weight);
        hist("dPhi_wlep_bqlep")->Fill(deltaPhi(wlep, blep), weight);

        hist("dR_whad_bqlep")->Fill(deltaR(whad, blep), weight);
        hist("dR_whad_wlep")->Fill(deltaR(whad, wlep), weight);
        hist("dR_wlep_bqhad")->Fill(deltaR(wlep, blep), weight);
        hist("dPhi_whad_bqlep")->Fill(deltaPhi(whad, blep), weight);
        hist("dPhi_whad_wlep")->Fill(deltaPhi(whad, wlep), weight);
        hist("dPhi_wlep_bqhad")->Fill(deltaPhi(wlep, blep), weight);
      }
    }
    // closing brackets of gentopjet.size
    if(gentopjet.size() > 1){
      hist("M_gtj2")->Fill(gentopjet.at(1).v4().M(), weight);
      hist("M_gtj2_rebin")->Fill(gentopjet.at(1).v4().M(), weight);
      hist("pt_gtj2")->Fill(gentopjet.at(1).pt(), weight);
      hist("eta_gtj2")->Fill(gentopjet.at(1).eta(), weight);
      hist("Nsubjet_gtj2")->Fill(gentopjet.at(1).subjets().size(), weight);
      if(gentopjet.at(1).subjets().size() > 0) hist("pt_gtj2_subjet1")->Fill(gentopjet.at(1).subjets().at(0).v4().pt(), weight);
      if(gentopjet.at(1).subjets().size() >= 2) hist("pt_gtj2_subjet2")->Fill(gentopjet.at(1).subjets().at(1).v4().pt(), weight);
      if(gentopjet.at(1).subjets().size() > 0) hist("m_gtj2_subjet1")->Fill(gentopjet.at(1).subjets().at(0).v4().M(), weight);
      if(gentopjet.at(1).subjets().size() >= 2)  hist("m_gtj2_subjet2")->Fill(gentopjet.at(1).subjets().at(1).v4().M(), weight);
      hist("tau1_gtj2")->Fill(gentopjet.at(1).tau1(), weight);
      hist("tau1_gtj2_rebin")->Fill(gentopjet.at(1).tau1(), weight);
      hist("tau2_gtj2")->Fill(gentopjet.at(1).tau2(), weight);
      hist("tau2_gtj2_rebin")->Fill(gentopjet.at(1).tau2(), weight);
      hist("tau3_gtj2")->Fill(gentopjet.at(1).tau3(), weight);
      hist("tau3_gtj2_rebin")->Fill(gentopjet.at(1).tau3(), weight);
      hist("tau4_gtj2")->Fill(gentopjet.at(1).tau4(), weight);
      hist("tau4_gtj2_rebin")->Fill(gentopjet.at(1).tau4(), weight);
      hist("tau32_gtj2")->Fill(gentopjet.at(1).tau3()/gentopjet.at(1).tau2(), weight);
      hist("tau32_gtj2_rebin1")->Fill(gentopjet.at(1).tau3()/gentopjet.at(1).tau2(), weight);
      hist("tau32_gtj2_rebin2")->Fill(gentopjet.at(1).tau3()/gentopjet.at(1).tau2(), weight);
      hist("tau21_gtj2")->Fill(gentopjet.at(1).tau2()/gentopjet.at(1).tau1(), weight);
      hist("tau21_gtj2_rebin")->Fill(gentopjet.at(1).tau2()/gentopjet.at(1).tau1(), weight);
      hist("tau31_gtj2")->Fill(gentopjet.at(1).tau3()/gentopjet.at(1).tau1(), weight);
      hist("tau31_gtj2_rebin")->Fill(gentopjet.at(1).tau3()/gentopjet.at(1).tau1(), weight);
      hist("tau41_gtj2")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau1(), weight);
      hist("tau41_gtj2_rebin")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau1(), weight);
      hist("tau42_gtj2")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau2(), weight);
      hist("tau42_gtj2_rebin")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau2(), weight);
      hist("tau43_gtj2")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau3(), weight);
      hist("tau43_gtj2_rebin")->Fill(gentopjet.at(1).tau4()/gentopjet.at(1).tau3(), weight);

      hist("ecfN2_beta1_gtj2")->Fill(gentopjet.at(1).ecfN2_beta1(), weight);
      hist("ecfN2_beta1_gtj2_rebin")->Fill(gentopjet.at(1).ecfN2_beta1(), weight);
      hist("ecfN2_beta2_gtj2")->Fill(gentopjet.at(1).ecfN2_beta2(), weight);
      hist("ecfN2_beta2_gtj2_rebin")->Fill(gentopjet.at(1).ecfN2_beta2(), weight);
      hist("ecfN3_beta1_gtj2")->Fill(gentopjet.at(1).ecfN3_beta1(), weight);
      hist("ecfN3_beta1_gtj2_rebin")->Fill(gentopjet.at(1).ecfN3_beta1(), weight);
      hist("ecfN3_beta2_gtj2")->Fill(gentopjet.at(1).ecfN3_beta2(), weight);
      hist("ecfN3_beta2_gtj2_rebin")->Fill(gentopjet.at(1).ecfN3_beta2(), weight);


    }
  }
}
