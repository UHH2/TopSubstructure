#include "UHH2/TopSubstructure/include/GenHists.h"

#include <iostream>

using namespace std;
using namespace uhh2;
using namespace uhh2examples;

GenHists::GenHists(Context & ctx, const string & dirname): Hists(ctx, dirname){
  // book all histograms here
  // jets
  book<TH1F>("N_jets", "N_{GenJets}", 21, -0.5, 20.5);
  book<TH1F>("eta_jet1", "#eta^{GenJet 1}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet2", "#eta^{GenJet 2}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet3", "#eta^{GenJet 3}", 40, -2.5, 2.5);
  book<TH1F>("eta_jet4", "#eta^{GenJet 4}", 40, -2.5, 2.5);
  book<TH1F>("pt_jet1", "p_{T}^{GenJet 1}", 120, 0, 1200);
  book<TH1F>("pt_jet2", "p_{T}^{GenJet 2}", 120, 0, 1200);
  book<TH1F>("pt_jet3", "p_{T}^{GenJet 3}", 120, 0, 1200);
  book<TH1F>("pt_jet4", "p_{T}^{GenJet 4}", 120, 0, 1200);
  book<TH1F>("N_topjets", "N_{GenTopJets}", 10, -0.5, 9.5);

  // TopJets
  book<TH1F>("Ntopjet_cand", "number GenTopJet candidates", 6, -0.5, 5.5);

  book<TH1F>("M_diff1", "M_{had. GenTopJet candidate} - M_{lep. GenTopJet candidate} [GeV^{2}]", 80, -400, 400);
  book<TH1F>("M_diff2", "M_{had. GenTopJet candidate} - M_{lep. GenTopJet candidate + Muon} [GeV^{2}]", 80, -400, 400);

  // first candidate
  book<TH1F>("M_cand1", "M_{had. GenTopJet candidate} [GeV^{2}]", 40, 0, 400);
  book<TH1F>("pt_topjet_cand1", "p_{T} had. GenTopJet candidate [GeV]", 120, 0, 1200);
  book<TH1F>("eta_topjet_cand1", "#eta had. GenTopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand", "N_{subjets} all candidates", 4, -0.5, 3.5);
  book<TH1F>("Nsubjet_cand1", "N_{subjets} had. GenTopJet candidate", 4, -0.5, 3.5);
  book<TH1F>("pt_subjet1_cand1", "p_{T}^{subjet1} had. GenTopJet candidate [GeV]", 120, 0, 1200);
  book<TH1F>("pt_subjet2_cand1", "p_{T}^{subjet2} had. GenTopJet candidate [GeV]", 80, 0, 800);
  book<TH1F>("m_subjet1_cand1", "M_{subjet1} had. GenTopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1F>("m_subjet2_cand1", "M_{subjet2} had. GenTopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1F>("tau1_topjet_cand1", "#tau_{1} had. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand1", "#tau_{2} had. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand1", "#tau_{3} had. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand1", "#tau_{3}/#tau_{2} had. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau21_cand1", "#tau_{2}/#tau_{1} had. GenTopJet candidate", 40, 0, 1.);

  book<TH1F>("dR_q1_jet_cand1", "#Delta R(quark1, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_q2_jet_cand1", "#Delta R(quark2, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_bqhad_jet_cand1", "#Delta R(b-quark (had), had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_whad_jet_cand1", "#Delta R(Whad, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dPhi_q1_jet_cand1", "#Delta #Phi (quark1, had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_q2_jet_cand1", "#Delta #Phi (quark2, had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_bqhad_jet_cand1", "#Delta #Phi (b-quark (had), had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_whad_jet_cand1", "#Delta #Phi (Whad, had. GenTopJet candidate)", 40, 0, 4.);

  book<TH1F>("dR_mu_jet_cand1", "#Delta R(#mu, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_nu_jet_cand1", "#Delta R(#nu_{#mu}, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_bqlep_jet_cand1", "#Delta R(b-quark (lep), had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_wlep_jet_cand1", "#Delta R(Wlep, had. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dPhi_mu_jet_cand1", "#Delta #Phi (#mu, had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_nu_jet_cand1", "#Delta #Phi (#nu_{#mu}, had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_bqlep_jet_cand1", "#Delta #Phi (b-quark (lep), had. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_wlep_jet_cand1", "#Delta #Phi (Wlep, had. GenTopJet candidate)", 40, 0, 4.);


  //  second candidate
  book<TH1F>("M_cand2", "M_{lep GenTopJet candidate} [GeV^{2}]", 40, 0, 400);
  book<TH1F>("pt_topjet_cand2", "p_{T} lep. GenTopJet candidate [GeV]", 120, 0, 1200);
  book<TH1F>("eta_topjet_cand2", "#eta lep. GenTopJet candidate", 40, -2.5, 2.5);
  book<TH1F>("Nsubjet_cand2", "N_{subjets} lep. GenTopJet candidate", 4, -0.5, 3.5);
  book<TH1F>("pt_subjet1_cand2", "p_{T}^{subjet1} lep. GenTopJet candidate [GeV]", 120, 0, 1200);
  book<TH1F>("pt_subjet2_cand2", "p_{T}^{subjet2} lep. GenTopJet candidate [GeV]", 80, 0, 800);
  book<TH1F>("m_subjet1_cand2", "M_{subjet1} lep. GenTopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1F>("m_subjet2_cand2", "M_{subjet2} lep. GenTopJet candidate [GeV^{2}]", 30, 0, 300);
  book<TH1F>("tau1_topjet_cand2", "#tau_{1} lep. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau2_topjet_cand2", "#tau_{2} lep. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau3_topjet_cand2", "#tau_{3} lep. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau32_cand2", "#tau_{3}/#tau_{2} lep. GenTopJet candidate", 40, 0, 1.);
  book<TH1F>("tau21_cand2", "#tau_{2}/#tau_{1} lep. GenTopJet candidate", 40, 0, 1.);

  book<TH1F>("dR_q1_jet_cand2", "#Delta R(quark1, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_q2_jet_cand2", "#Delta R(quark2, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_bqhad_jet_cand2", "#Delta  R(b-quark (had), lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_whad_jet_cand2", "#Delta  R(Whad, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dPhi_q1_jet_cand2", "#Delta #Phi (quark1, lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_q2_jet_cand2", "#Delta #Phi (quark2, lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_bqhad_jet_cand2", "#Delta #Phi (b-quark (had), lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_whad_jet_cand2", "#Delta #Phi (Whad, lep. GenTopJet candidate)", 40, 0, 4.);

  book<TH1F>("dR_mu_jet_cand2", "#Delta R(#mu, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_nu_jet_cand2", "#Delta R(#nu_{#mu}, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_bqlep_jet_cand2", "#Delta R(b-quark (lep), lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dR_wlep_jet_cand2", "#Delta  R(Wlep, lep. GenTopJet candidate)", 50, 0, 5.);
  book<TH1F>("dPhi_mu_jet_cand2", "#Delta #Phi (#mu, lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_nu_jet_cand2", "#Delta #Phi (#nu_{#mu}, lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_bqlep_jet_cand2", "#Delta #Phi (b-quark (lep), lep. GenTopJet candidate)", 40, 0, 4.);
  book<TH1F>("dPhi_wlep_jet_cand2", "#Delta #Phi (Wlep, lep. GenTopJet candidate)", 40, 0, 4.);

  book<TH1F>("dR8_top_jet", "#Delta R(top, had. GenTopJet)", 50, 0, 5.);



  book<TH1F>("dR_q1_q2", "#Delta R(quark1, quark2)", 50, 0, 5.);
  book<TH1F>("dR_bqhad_q1", "#Delta R(b-quark, quark1)", 50, 0, 5.);
  book<TH1F>("dR_bqhad_q2", "#Delta R(b-quark, quark2)", 50, 0, 5.);
  book<TH1F>("dPhi_q1_q2", "#Delta #Phi (quark1, quark2)", 40, 0, 4.);
  book<TH1F>("dPhi_bqhad_q1", "#Delta #Phi (b-quark, quark1)", 40, 0, 4.);
  book<TH1F>("dPhi_bqhad_q2", "#Delta #Phi (b-quark, quark2)", 40, 0, 4.);

  book<TH1F>("dR_mu_nu", "#Delta R(#mu, #nu_{#mu})", 50, 0, 5.);
  book<TH1F>("dR_bqlep_mu", "#Delta R(b-quark (lep), #mu)", 50, 0, 5.);
  book<TH1F>("dR_bqlep_nu", "#Delta R(b-quark (lep), #nu_{#mu})", 50, 0, 5.);
  book<TH1F>("dPhi_mu_nu", "#Delta #Phi (#mu, #nu_{#mu})", 40, 0, 4.);
  book<TH1F>("dPhi_bqlep_mu", "#Delta #Phi (b-quark (lep), #mu)", 40, 0, 4.);
  book<TH1F>("dPhi_bqlep_nu", "#Delta #Phi (b-quark (lep), #nu_{#mu})", 40, 0, 4.);



  book<TH1F>("dR_whad_bqhad", "#Delta R(Whad, b-quark had)", 50, 0, 5.);
  book<TH1F>("dR_whad_bqlep", "#Delta R(Whad, b-quark lep)", 50, 0, 5.);
  book<TH1F>("dR_whad_wlep", "#Delta R(Whad, Wlep)", 50, 0, 5.);
  book<TH1F>("dR_wlep_bqhad", "#Delta R(Wlep, b-quark had)", 50, 0, 5.);
  book<TH1F>("dR_wlep_bqlep", "#Delta R(Wlep, b-quark lep)", 50, 0, 5.);
  book<TH1F>("dPhi_whad_bqhad", "#Delta #Phi (Whad, b-quark had)", 40, 0, 4.);
  book<TH1F>("dPhi_whad_bqlep", "#Delta #Phi (Whad, b-quark lep)", 40, 0, 4.);
  book<TH1F>("dPhi_whad_wlep", "#Delta #Phi (Whad, Wlep)", 40, 0, 4.);
  book<TH1F>("dPhi_wlep_bqhad", "#Delta #Phi (Wlep, b-quark had)", 40, 0, 4.);
  book<TH1F>("dPhi_wlep_bqlep", "#Delta #Phi (Wlep, b-quark lep)", 40, 0, 4.);


  book<TH1F>("N_jets_clean", "N_{jets} (additional)", 15, 0, 15);


  // leptons
  book<TH1F>("pt_mu", "p_{T}^{#mu} [GeV]", 100, 0, 1000);
  book<TH1F>("eta_mu", "#eta^{#mu}", 40, -2.5, 2.5);
  book<TH1F>("mass_mu", "M^{#mu} [GeV]", 50, 0, 0.5);

  //general
  book<TH1F>("E_Tmiss", "missing E_{T} [GeV]", 75, 0, 1500);
  book<TH1F>("sum_event_weights", "BinContent = sum(eventweights)", 1, 0.5, 1.5);


  // get handles
  h_ttbargen = ctx.get_handle<TTbarGen>("ttbargen");
  h_weight = ctx.get_handle<double>("h_gen_weight");
}


void GenHists::fill(const Event & event){
  // Don't forget to always use the weight when filling.
  double weight = event.get(h_weight);

  //general
  hist("sum_event_weights")->Fill(1, weight);

  // Jets
  std::vector<Particle>* jets = event.genjets;
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
  hist("N_topjets")->Fill(event.gentopjets->size(), weight);


  // GenTopJet
    hist("Ntopjet_cand")->Fill(event.gentopjets->size(), weight);

    if(event.gentopjets->size() > 0){
      for(unsigned int i=0; i < event.gentopjets->size(); i++){
        hist("Nsubjet_cand")->Fill(event.gentopjets->at(i).subjets().size(), weight);
      }

      hist("M_cand1")->Fill(event.gentopjets->at(0).v4().M(), weight);
      hist("pt_topjet_cand1")->Fill(event.gentopjets->at(0).pt(), weight);
      hist("eta_topjet_cand1")->Fill(event.gentopjets->at(0).eta(), weight);
      hist("Nsubjet_cand1")->Fill(event.gentopjets->at(0).subjets().size(), weight);
      hist("pt_subjet1_cand1")->Fill(event.gentopjets->at(0).subjets().at(0).v4().pt(), weight);
      if(event.gentopjets->at(0).subjets().size() >= 2) hist("pt_subjet2_cand1")->Fill(event.gentopjets->at(0).subjets().at(1).v4().pt(), weight);
      hist("m_subjet1_cand1")->Fill(event.gentopjets->at(0).subjets().at(0).v4().M(), weight);
      if(event.gentopjets->at(0).subjets().size() >= 2) hist("m_subjet2_cand1")->Fill(event.gentopjets->at(0).subjets().at(1).v4().M(), weight);
      hist("tau1_topjet_cand1")->Fill(event.gentopjets->at(0).tau1(), weight);
      hist("tau2_topjet_cand1")->Fill(event.gentopjets->at(0).tau2(), weight);
      hist("tau3_topjet_cand1")->Fill(event.gentopjets->at(0).tau3(), weight);
      hist("tau32_cand1")->Fill(event.gentopjets->at(0).tau3()/event.gentopjets->at(0).tau2(), weight);
      hist("tau21_cand1")->Fill(event.gentopjets->at(0).tau2()/event.gentopjets->at(0).tau1(), weight);


      if(event.is_valid(h_ttbargen)){
        const auto & ttbargen = event.get(h_ttbargen);
        GenParticle tophad, q1, q2, bhad, blep, mu, nu, whad, wlep;

        tophad = ttbargen.TopHad();
        q1 = ttbargen.Q1();
        q2 = ttbargen.Q2();
        bhad = ttbargen.BHad();
        blep = ttbargen.BLep();
        mu = ttbargen.ChargedLepton();
        nu = ttbargen.Neutrino();
        whad = ttbargen.WHad();
        wlep = ttbargen.WLep();

        hist("pt_mu")->Fill(mu.pt(), weight);
        hist("eta_mu")->Fill(mu.eta(), weight);
        hist("mass_mu")->Fill(mu.v4().M(), weight);

        if(event.gentopjets->size() > 1){
          hist("M_diff1")->Fill(event.gentopjets->at(0).v4().M() - event.gentopjets->at(1).v4().M(), weight);

          const auto dummy_mass = event.gentopjets->at(1).v4() + mu.v4();
          hist("M_diff2")->Fill(event.gentopjets->at(0).v4().M() - dummy_mass.M(), weight);
        }

        hist("E_Tmiss")->Fill(nu.pt(), weight);

        hist("dPhi_mu_jet_cand1")->Fill(deltaPhi(event.gentopjets->at(0), mu), weight);
        if(event.gentopjets->size() > 1)hist("dPhi_mu_jet_cand2")->Fill(deltaPhi(event.gentopjets->at(1), mu), weight);
        if(event.gentopjets->size() > 1) hist("dPhi_mu_jet_cand2")->Fill(deltaPhi(event.gentopjets->at(1), mu), weight);


        hist("dR8_top_jet")->Fill(deltaR(tophad, event.gentopjets->at(0)), weight);

        hist("dR_q1_jet_cand1")->Fill(deltaR(q1, event.gentopjets->at(0)), weight);
        hist("dR_q2_jet_cand1")->Fill(deltaR(q2, event.gentopjets->at(0)), weight);
        hist("dR_bqhad_jet_cand1")->Fill(deltaR(bhad, event.gentopjets->at(0)), weight);
        hist("dR_whad_jet_cand1")->Fill(deltaR(whad, event.gentopjets->at(0)), weight);
        hist("dPhi_q1_jet_cand1")->Fill(deltaPhi(q1, event.gentopjets->at(0)), weight);
        hist("dPhi_q2_jet_cand1")->Fill(deltaPhi(q2, event.gentopjets->at(0)), weight);
        hist("dPhi_bqhad_jet_cand1")->Fill(deltaPhi(bhad, event.gentopjets->at(0)), weight);
        hist("dPhi_whad_jet_cand1")->Fill(deltaPhi(whad, event.gentopjets->at(0)), weight);

        hist("dR_mu_jet_cand1")->Fill(deltaR(mu, event.gentopjets->at(0)), weight);
        hist("dR_nu_jet_cand1")->Fill(deltaR(nu, event.gentopjets->at(0)), weight);
        hist("dR_bqlep_jet_cand1")->Fill(deltaR(blep, event.gentopjets->at(0)), weight);
        hist("dR_wlep_jet_cand1")->Fill(deltaR(wlep, event.gentopjets->at(0)), weight);
        hist("dPhi_mu_jet_cand1")->Fill(deltaPhi(mu, event.gentopjets->at(0)), weight);
        hist("dPhi_nu_jet_cand1")->Fill(deltaPhi(nu, event.gentopjets->at(0)), weight);
        hist("dPhi_bqlep_jet_cand1")->Fill(deltaPhi(blep, event.gentopjets->at(0)), weight);
        hist("dPhi_wlep_jet_cand1")->Fill(deltaPhi(wlep, event.gentopjets->at(0)), weight);

        if(event.gentopjets->size() > 1){
          hist("dR_q1_jet_cand2")->Fill(deltaR(q1, event.gentopjets->at(1)), weight);
          hist("dR_q2_jet_cand2")->Fill(deltaR(q2, event.gentopjets->at(1)), weight);
          hist("dR_bqhad_jet_cand2")->Fill(deltaR(bhad, event.gentopjets->at(1)), weight);
          hist("dR_whad_jet_cand2")->Fill(deltaR(whad, event.gentopjets->at(1)), weight);
          hist("dPhi_q1_jet_cand2")->Fill(deltaPhi(q1, event.gentopjets->at(1)), weight);
          hist("dPhi_q2_jet_cand2")->Fill(deltaPhi(q2, event.gentopjets->at(1)), weight);
          hist("dPhi_bqhad_jet_cand2")->Fill(deltaPhi(bhad, event.gentopjets->at(1)), weight);
          hist("dPhi_whad_jet_cand2")->Fill(deltaPhi(whad, event.gentopjets->at(1)), weight);

          hist("dR_mu_jet_cand2")->Fill(deltaR(mu, event.gentopjets->at(1)), weight);
          hist("dR_nu_jet_cand2")->Fill(deltaR(nu, event.gentopjets->at(1)), weight);
          hist("dR_bqlep_jet_cand2")->Fill(deltaR(blep, event.gentopjets->at(1)), weight);
          hist("dR_wlep_jet_cand2")->Fill(deltaR(wlep, event.gentopjets->at(1)), weight);
          hist("dPhi_mu_jet_cand2")->Fill(deltaPhi(mu, event.gentopjets->at(1)), weight);
          hist("dPhi_nu_jet_cand2")->Fill(deltaPhi(nu, event.gentopjets->at(1)), weight);
          hist("dPhi_bqlep_jet_cand2")->Fill(deltaPhi(blep, event.gentopjets->at(1)), weight);
          hist("dPhi_wlep_jet_cand2")->Fill(deltaPhi(wlep, event.gentopjets->at(1)), weight);
        }

        hist("dR_q1_q2")->Fill(deltaR(q1, q2), weight);
        hist("dR_bqhad_q1")->Fill(deltaR(bhad, q1), weight);
        hist("dR_bqhad_q2")->Fill(deltaR(bhad, q2), weight);
        hist("dR_whad_bqhad")->Fill(deltaR(whad, bhad), weight);
        hist("dPhi_q1_q2")->Fill(deltaPhi(q1, q2), weight);
        hist("dPhi_bqhad_q1")->Fill(deltaPhi(bhad, q1), weight);
        hist("dPhi_bqhad_q2")->Fill(deltaPhi(bhad, q2), weight);
        hist("dPhi_whad_bqhad")->Fill(deltaPhi(whad, bhad), weight);

        hist("dR_mu_nu")->Fill(deltaR(mu, nu), weight);
        hist("dR_bqlep_mu")->Fill(deltaR(blep, mu), weight);
        hist("dR_bqlep_nu")->Fill(deltaR(blep, nu), weight);
        hist("dR_wlep_bqlep")->Fill(deltaR(wlep, blep), weight);
        hist("dPhi_mu_nu")->Fill(deltaPhi(mu, nu), weight);
        hist("dPhi_bqlep_mu")->Fill(deltaPhi(blep, mu), weight);
        hist("dPhi_bqlep_nu")->Fill(deltaPhi(blep, nu), weight);
        hist("dPhi_wlep_bqlep")->Fill(deltaPhi(wlep, blep), weight);

        hist("dR_whad_bqlep")->Fill(deltaR(whad, blep), weight);
        hist("dR_whad_wlep")->Fill(deltaR(whad, wlep), weight);
        hist("dR_wlep_bqhad")->Fill(deltaR(wlep, blep), weight);
        hist("dPhi_whad_bqlep")->Fill(deltaPhi(whad, blep), weight);
        hist("dPhi_whad_wlep")->Fill(deltaPhi(whad, wlep), weight);
        hist("dPhi_wlep_bqhad")->Fill(deltaPhi(wlep, blep), weight);

      }
       // closing brackets of event.gentopjets->size
      if(event.gentopjets->size() > 1){
        hist("M_cand2")->Fill(event.gentopjets->at(1).v4().M(), weight);
        hist("pt_topjet_cand2")->Fill(event.gentopjets->at(1).pt(), weight);
        hist("eta_topjet_cand2")->Fill(event.gentopjets->at(1).eta(), weight);
        hist("Nsubjet_cand2")->Fill(event.gentopjets->at(1).subjets().size(), weight);
        hist("pt_subjet1_cand2")->Fill(event.gentopjets->at(1).subjets().at(0).v4().pt(), weight);
        if(event.gentopjets->at(1).subjets().size() >= 2) hist("pt_subjet2_cand2")->Fill(event.gentopjets->at(1).subjets().at(1).v4().pt(), weight);
        hist("m_subjet1_cand2")->Fill(event.gentopjets->at(1).subjets().at(0).v4().M(), weight);
        if(event.gentopjets->at(1).subjets().size() >= 2)  hist("m_subjet2_cand2")->Fill(event.gentopjets->at(1).subjets().at(1).v4().M(), weight);
        hist("tau1_topjet_cand2")->Fill(event.gentopjets->at(1).tau1(), weight);
        hist("tau2_topjet_cand2")->Fill(event.gentopjets->at(1).tau2(), weight);
        hist("tau3_topjet_cand2")->Fill(event.gentopjets->at(1).tau3(), weight);
        hist("tau32_cand2")->Fill(event.gentopjets->at(1).tau3()/event.gentopjets->at(1).tau2(), weight);
        hist("tau21_cand2")->Fill(event.gentopjets->at(1).tau2()/event.gentopjets->at(1).tau1(), weight);
      }
    }
}
