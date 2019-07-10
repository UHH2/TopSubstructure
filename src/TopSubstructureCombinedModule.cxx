// #include <iostream>
// #include <memory>
//
// #include "UHH2/core/include/AnalysisModule.h"
// #include "UHH2/core/include/Event.h"
//
// #include "UHH2/common/include/CommonModules.h"
// #include "UHH2/common/include/CleaningModules.h"
// #include "UHH2/common/include/ElectronHists.h"
// #include "UHH2/common/include/MuonHists.h"
// #include "UHH2/common/include/NSelections.h"
// #include "UHH2/common/include/JetHists.h"
// #include "UHH2/common/include/TriggerSelection.h"
// #include "UHH2/common/include/MuonIds.h"
// #include "UHH2/common/include/ElectronIds.h"
// #include "UHH2/common/include/JetIds.h"
// #include "UHH2/common/include/TopJetIds.h"
// #include "UHH2/common/include/MCWeight.h"
// #include "UHH2/common/include/TTbarGen.h"
//
// #include "UHH2/TopSubstructure/include/TopSubstructureSelections.h"
// #include "UHH2/TopSubstructure/include/TopSubstructureCombinedSelections.h"
// #include "UHH2/TopSubstructure/include/TopSubstructureRecoHists.h"
// #include "UHH2/TopSubstructure/include/GenHists.h"
// #include "UHH2/TopSubstructure/include/TopSubstructureGenSelections.h"
// #include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"
//
// using namespace std;
// using namespace uhh2;
//
// namespace uhh2examples {
//   class TopSubstructureCombinedModule: public AnalysisModule {
//   public:
//     explicit TopSubstructureCombinedModule(Context & ctx);
//     virtual bool process(Event & event) override;
//
//   private:
//     std::unique_ptr<CommonModules>   common;
//     std::unique_ptr<MuonCleaner>     muoSR_cleaner;
//     std::unique_ptr<ElectronCleaner> eleSR_cleaner;
//
//     bool isMC, isTTbar;
//     bool passed_trig, passed_gen, passed_rec;
//     bool matched_gen, matched_rec;
//
//     bool passed_gen_2, passed_rec_2;
//     bool matched_gen_2, matched_rec_2;
//
//     std::unique_ptr<JetCleaner> jetcleaner;
//     std::unique_ptr<TopJetCleaner> topjetcleaner;
//     std::unique_ptr<GenTopJetCleaner> gentopjetcleaner;
//
//     // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.
//     std::unique_ptr<Selection> pv_sel, nmu_sel, met_sel, pt_mu_sel, nele_sel, twodcut_sel, nbtag_medium_sel;
//     std::unique_ptr<Selection> pt_topjet_sel1, ntopjet2_sel, dr_sel, mass_sel0, mass_sel1;
//     std::unique_ptr<Selection> trigger_sel_A, trigger_sel_B;
//     std::unique_ptr<Selection> recmatching;
//
//     std::unique_ptr<Selection> nmu_gen, pt_topjet_gen, ntopjet2_gen, dr_gen, mass_gen0, mass_gen;
//     std::unique_ptr<Selection> genmatching;
//
//     std::unique_ptr<AnalysisModule> PUreweight, lumiweight;
//     std::unique_ptr<AnalysisModule> cleaner, rectopjetleptoncleaner;
//
//     std::unique_ptr<AnalysisModule> ttgenprod;
//
//     // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
//     std::unique_ptr<Hists> h_start, h_gensel, h_lumi, h_common, h_jetcleaner, h_muoncleaner, h_elecleaner, h_pu, h_trigger;
//
//     std::unique_ptr<Hists> h_pv, h_nmu, h_tjlc, h_tjc, h_met, h_pt_mu, h_nele, h_twodcut, h_nbtag_medium;
//
//     std::unique_ptr<Hists> h_pt_topjet, h_pt_topjet_matched, h_pt_topjet_unmatched;
//     std::unique_ptr<Hists> h_ntopjet2, h_ntopjet2_matched, h_ntopjet2_unmatched;
//     std::unique_ptr<Hists> h_dr, h_dr_matched, h_dr_unmatched;
//     std::unique_ptr<Hists> h_mass0, h_mass0_matched, h_mass0_unmatched;
//     std::unique_ptr<Hists> h_mass1, h_mass1_matched, h_mass1_unmatched;
//
//
//
//     std::unique_ptr<Hists> h_gen_nmu, h_gen_nmu_matched, h_gen_nmu_unmatched;
//     std::unique_ptr<Hists> h_gen_pt_topjet, h_gen_pt_topjet_matched, h_gen_pt_topjet_unmatched;
//     std::unique_ptr<Hists> h_gen_ntopjet2, h_gen_ntopjet2_matched, h_gen_ntopjet2_unmatched;
//     std::unique_ptr<Hists> h_gen_dr, h_gen_dr_matched, h_gen_dr_unmatched;
//     std::unique_ptr<Hists> h_gen_mass0, h_gen_mass0_matched, h_gen_mass0_unmatched;
//     std::unique_ptr<Hists> h_gen_mass1, h_gen_mass1_matched, h_gen_mass1_unmatched;
//
//     std::unique_ptr<Hists> h_recgen_rec;
//     std::unique_ptr<Hists> h_recgen_gen, h_recgen_gen_matched, h_recgen_gen_unmatched;
//     std::unique_ptr<Hists> h_recgen_rec_2;
//     std::unique_ptr<Hists> h_recgen_gen_2, h_recgen_gen_matched_2, h_recgen_gen_unmatched_2;
//
//     JetId Btag_medium;
//
//     uhh2::Event::Handle<double> h_gen_weight;
//     uhh2::Event::Handle<double> h_rec_weight;
//   };
//
//
//   TopSubstructureCombinedModule::TopSubstructureCombinedModule(Context & ctx){
//     // 1. setup other modules. CommonModules and the JetCleaner:
//     h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");
//     h_rec_weight = ctx.declare_event_output<double>("h_rec_weight");
//
//     common.reset(new CommonModules());
//     lumiweight.reset(new MCLumiWeight(ctx));
//     // TODO: configure common here, e.g. by
//     // calling common->set_*_id or common->disable_*
//
//     common->disable_mclumiweight();
//     common->switch_jetlepcleaner();
//     common->init(ctx); // always last!
//
//
//     MuonId muid       = AndId<Muon>(MuonID(Muon::CutBasedIdTight), PtEtaCut(55., 2.4));
//     ElectronId eleid  = AndId<Electron>(ElectronID_Summer16_medium_noIso, PtEtaCut(55., 2.4));
//     Btag_medium        = CSVBTag(CSVBTag::WP_MEDIUM);
//
//     muoSR_cleaner.reset(new MuonCleaner(muid));
//     eleSR_cleaner.reset(new ElectronCleaner(eleid));
//     jetcleaner.reset(new JetCleaner(ctx, 30.0, 2.4));
//     topjetcleaner.reset(new TopJetCleaner(ctx, PtEtaCut(170., 2.5)));
//     gentopjetcleaner.reset(new GenTopJetCleaner(ctx));
//     rectopjetleptoncleaner.reset(new RecTopJetLeptonCleaner());
//
//
//     trigger_sel_A = uhh2::make_unique<TriggerSelection>("HLT_Mu50_v*");
//     trigger_sel_B = uhh2::make_unique<TriggerSelection>("HLT_TkMu50_v*");
//
//
//     isTTbar = (ctx.get("dataset_version") == "TTbar_Mtt0000to0700" || ctx.get("dataset_version") == "TTbar_Mtt0700to1000" || ctx.get("dataset_version") == "TTbar_Mtt1000toInft");
//     isMC = (ctx.get("dataset_type") == "MC");
//
//     // 2. set up selections
//     if(isTTbar){
//       const std::string ttbar_gen_label("ttbargen");
//       ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
//       cleaner.reset(new GenTopJetLeptonCleaner(ctx));
//       ntopjet2_gen.reset(new GenNTopJet(2,2));
//       nmu_gen.reset(new TTbarSemilep(ctx));
//       mass_gen0.reset(new GenMassCompare(ctx, 0));
//       mass_gen.reset(new GenMassCompare(ctx, 1));
//       dr_gen.reset(new dRSelection(ctx));
//       pt_topjet_gen.reset(new PtSelection(400, 200));
//       genmatching.reset(new GenMatching(ctx));
//     }
//     pv_sel.reset(new NPVSelection(1, -1, PrimaryVertexId(StandardPrimaryVertexId())));
//     PUreweight.reset(new MCPileupReweight(ctx, "central"));
//
//     ntopjet2_sel.reset(new NTopJetSelection(2,2));
//     nmu_sel.reset(new NMuonSelection(1,1));
//     met_sel.reset(new METSelection(50,-1));
//     pt_mu_sel.reset(new MuonptSelection(55));
//     nele_sel.reset(new NElectronSelection(0, 0));
//     twodcut_sel.reset(new TwoDCut(0.4, 40)); // use 30GeV for more statistics
//     nbtag_medium_sel.reset(new NJetSelection(1, -1, Btag_medium));
//     mass_sel0.reset(new RecMassCompare(0));   // 0: compare mass between first and second topjet
//     mass_sel1.reset(new RecMassCompare(1));   // 1: added 4-vector of lepton and second topjet, then compare masses
//     dr_sel.reset(new RecdRSelection());
//     pt_topjet_sel1.reset(new RecPtSelection(400, 200));
//     recmatching.reset(new RecMatching(ctx));
//
//
//     // 3. Set up Hists classes:
//
//     h_start.reset(new TopSubstructureRecoHists(ctx, "start"));
//     h_lumi.reset(new TopSubstructureRecoHists(ctx, "lumi"));
//     h_gensel.reset(new TopSubstructureRecoHists(ctx, "gensel"));
//     h_common.reset(new TopSubstructureRecoHists(ctx, "common"));
//     h_jetcleaner.reset(new TopSubstructureRecoHists(ctx, "jetcleaner"));
//     h_muoncleaner.reset(new TopSubstructureRecoHists(ctx, "muoncleaner"));
//     h_elecleaner.reset(new TopSubstructureRecoHists(ctx, "elecleaner"));
//     h_pu.reset(new TopSubstructureRecoHists(ctx, "pu"));
//     h_trigger.reset(new TopSubstructureRecoHists(ctx, "trigger"));
//
//     h_pv.reset(new TopSubstructureRecoHists(ctx, "pv"));
//     h_nmu.reset(new TopSubstructureRecoHists(ctx, "nmu"));
//     h_tjlc.reset(new TopSubstructureRecoHists(ctx, "tjlc"));
//     h_tjc.reset(new TopSubstructureRecoHists(ctx, "tjc"));
//     h_met.reset(new TopSubstructureRecoHists(ctx, "met"));
//     h_pt_mu.reset(new TopSubstructureRecoHists(ctx, "pt_mu"));
//     h_nele.reset(new TopSubstructureRecoHists(ctx, "nele"));
//     h_twodcut.reset(new TopSubstructureRecoHists(ctx, "twodcut"));
//     h_nbtag_medium.reset(new TopSubstructureRecoHists(ctx, "nbtag_medium"));
//
//     h_pt_topjet.reset(new TopSubstructureRecoHists(ctx, "pt_topjet"));
//     h_pt_topjet_matched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_matched"));
//     h_pt_topjet_unmatched.reset(new TopSubstructureRecoHists(ctx, "pt_topjet_unmatched"));
//
//     h_ntopjet2.reset(new TopSubstructureRecoHists(ctx, "ntopjet2"));
//     h_ntopjet2_matched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_matched"));
//     h_ntopjet2_unmatched.reset(new TopSubstructureRecoHists(ctx, "ntopjet2_unmatched"));
//
//     h_dr.reset(new TopSubstructureRecoHists(ctx, "dr"));
//     h_dr_matched.reset(new TopSubstructureRecoHists(ctx, "dr_matched"));
//     h_dr_unmatched.reset(new TopSubstructureRecoHists(ctx, "dr_unmatched"));
//
//     h_mass0.reset(new TopSubstructureRecoHists(ctx, "mass0"));
//     h_mass0_matched.reset(new TopSubstructureRecoHists(ctx, "mass0_matched"));
//     h_mass0_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass0_unmatched"));
//
//     h_mass1.reset(new TopSubstructureRecoHists(ctx, "mass1"));
//     h_mass1_matched.reset(new TopSubstructureRecoHists(ctx, "mass1_matched"));
//     h_mass1_unmatched.reset(new TopSubstructureRecoHists(ctx, "mass1_unmatched"));
//
//
//
//     h_gen_nmu.reset(new GenHists(ctx, "gen_nmu"));
//     h_gen_nmu_matched.reset(new GenHists(ctx, "gen_nmu_matched"));
//     h_gen_nmu_unmatched.reset(new GenHists(ctx, "gen_nmu_unmatched"));
//
//     h_gen_pt_topjet.reset(new GenHists(ctx, "gen_pt_topjet"));
//     h_gen_pt_topjet_matched.reset(new GenHists(ctx, "gen_pt_topjet_matched"));
//     h_gen_pt_topjet_unmatched.reset(new GenHists(ctx, "gen_pt_topjet_unmatched"));
//
//     h_gen_ntopjet2.reset(new GenHists(ctx, "gen_ntopjet2"));
//     h_gen_ntopjet2_matched.reset(new GenHists(ctx, "gen_ntopjet2_matched"));
//     h_gen_ntopjet2_unmatched.reset(new GenHists(ctx, "gen_ntopjet2_unmatched"));
//
//     h_gen_dr.reset(new GenHists(ctx, "gen_dr"));
//     h_gen_dr_matched.reset(new GenHists(ctx, "gen_dr_matched"));
//     h_gen_dr_unmatched.reset(new GenHists(ctx, "gen_dr_unmatched"));
//
//     h_gen_mass0.reset(new GenHists(ctx, "gen_mass0"));
//     h_gen_mass0_matched.reset(new GenHists(ctx, "gen_mass0_matched"));
//     h_gen_mass0_unmatched.reset(new GenHists(ctx, "gen_mass0_unmatched"));
//
//     h_gen_mass1.reset(new GenHists(ctx, "gen_mass1"));
//     h_gen_mass1_matched.reset(new GenHists(ctx, "gen_mass1_matched"));
//     h_gen_mass1_unmatched.reset(new GenHists(ctx, "gen_mass1_unmatched"));
//
//
//     h_recgen_rec.reset(new TopSubstructureRecoHists(ctx, "recgen_rec"));
//     h_recgen_gen.reset(new GenHists(ctx, "recgen_gen"));
//     h_recgen_gen_matched.reset(new GenHists(ctx, "recgen_gen_matched"));
//     h_recgen_gen_unmatched.reset(new GenHists(ctx, "recgen_gen_unmatched"));
//
//     h_recgen_rec_2.reset(new TopSubstructureRecoHists(ctx, "recgen_rec_2"));
//     h_recgen_gen_2.reset(new GenHists(ctx, "recgen_gen_2"));
//     h_recgen_gen_matched_2.reset(new GenHists(ctx, "recgen_gen_matched_2"));
//     h_recgen_gen_unmatched_2.reset(new GenHists(ctx, "recgen_gen_unmatched_2"));
//   }
//
//
//   bool TopSubstructureCombinedModule::process(Event & event) {
//     cout << "TopSubstructureCombinedModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
//     h_start->fill(event);
//     lumiweight->process(event);
//     h_lumi->fill(event);
//
//     event.set(h_gen_weight, event.weight);
//
//     passed_trig = false;
//     passed_gen = false;
//     passed_rec = false;
//     matched_gen = false;
//     passed_gen_2 = false;
//     passed_rec_2 = false;
//     matched_gen_2 = false;
//     matched_rec = false;
//     if(isTTbar){
//       ttgenprod->process(event);
//       passed_gen = nmu_gen->passes(event);
//       if(passed_gen){
//         h_gen_nmu->fill(event);
//         matched_gen = genmatching->passes(event);
//         if(matched_gen) h_gen_nmu_matched->fill(event);
//         else h_gen_nmu_unmatched->fill(event);
//
//         cleaner->process(event); // Do this always!
//         gentopjetcleaner->process(event);
//         passed_gen = pt_topjet_gen->passes(event);
//         if(passed_gen){
//           h_gen_pt_topjet->fill(event);
//           matched_gen = genmatching->passes(event);
//           if(matched_gen) h_gen_pt_topjet_matched->fill(event);
//           else h_gen_pt_topjet_unmatched->fill(event);
//
//           passed_gen = ntopjet2_gen->passes(event);
//           if(passed_gen){
//             h_gen_ntopjet2->fill(event);
//             matched_gen = genmatching->passes(event);
//             if(matched_gen) h_gen_ntopjet2_matched->fill(event);
//             else h_gen_ntopjet2_unmatched->fill(event);
//
//             passed_gen = dr_gen->passes(event);
//             if(passed_gen){
//               h_gen_dr->fill(event);
//               matched_gen = genmatching->passes(event);
//               if(matched_gen) h_gen_dr_matched->fill(event);
//               else h_gen_dr_unmatched->fill(event);
//
//               passed_gen = mass_gen0->passes(event);
//               if(passed_gen){
//                 h_gen_mass0->fill(event);
//                 matched_gen = genmatching->passes(event);
//                 if(matched_gen) h_gen_mass0_matched->fill(event);
//                 else h_gen_mass0_unmatched->fill(event);
//               }
//
//               passed_gen_2 = mass_gen->passes(event);
//               if(passed_gen_2){
//                 h_gen_mass1->fill(event);
//                 matched_gen_2 = genmatching->passes(event);
//                 if(matched_gen_2) h_gen_mass1_matched->fill(event);
//                 else h_gen_mass1_unmatched->fill(event);
//               }
//             }
//           }
//         }
//       }
//     }
//
//     // 1. run all modules other modules.
//     h_gensel->fill(event);
//
//     common->process(event);
//     h_common->fill(event);
//     jetcleaner->process(event);
//     h_jetcleaner->fill(event);
//     muoSR_cleaner->process(event);
//     h_muoncleaner->fill(event);
//     eleSR_cleaner->process(event);
//     h_elecleaner->fill(event);
//
//     /** PU Reweighting *********************/
//     PUreweight->process(event);
//     h_pu->fill(event);
//     /* *********** Trigger *********** */
//     // for DATA until run 274954 -> use only Trigger A
//     // for MC and DATA from 274954 -> use "A || B"
//     if(!isMC && event.run < 274954) {
//       passed_trig = trigger_sel_A->passes(event);
//     }else{
//       passed_trig = (trigger_sel_A->passes(event) || trigger_sel_B->passes(event));
//     }
//     if(!passed_trig) return false;
//     h_trigger->fill(event);
//
//     event.set(h_rec_weight, event.weight);
//
//
//     /*
//     ████████  ██████  ██████   ██████
//        ██    ██    ██ ██   ██ ██    ██
//        ██    ██    ██ ██   ██ ██    ██
//        ██    ██    ██ ██   ██ ██    ██
//        ██     ██████  ██████   ██████
//
//        Additional AK4 Jets: Don't save them in a handle. Either write a function which returns an integer of the number of AK4 jets or program it directly in the hist class. Check for both candidates! Hadronic and Leptonic!!
//     */
//
//     //at least 1 good primary vertex
//     passed_rec = pv_sel->passes(event);
//     if(passed_rec){
//       h_pv->fill(event);
//       // exactly 1 muon
//       passed_rec = nmu_sel->passes(event);
//       if(passed_rec){
//         h_nmu->fill(event);
//         rectopjetleptoncleaner->process(event); // remove muon four-vector from topjet four-vector
//         h_tjlc->fill(event);
//
//         topjetcleaner->process(event);  // remove topjets from event.topjets with pT < 170 & eta > 2.5
//         h_tjc->fill(event);
//         // MET
//         passed_rec = met_sel->passes(event);
//         if(passed_rec){
//           h_met->fill(event);
//           // pT(muon)
//           passed_rec = pt_mu_sel->passes(event);
//           if(passed_rec){
//             h_pt_mu->fill(event);
//             // exactly 0 electron
//             passed_rec = nele_sel->passes(event);
//             if(passed_rec){
//               h_nele->fill(event);
//               // TwoDCut
//               passed_rec = twodcut_sel->passes(event);
//               if(passed_rec){
//                 h_twodcut->fill(event);
//                 // at least one tight b-tag
//                 passed_rec = nbtag_medium_sel->passes(event);
//                 if(passed_rec){
//                   h_nbtag_medium->fill(event);
//                   // pT(first Topjet) > 400 && pT(second Topjet) > 200
//                   passed_rec = pt_topjet_sel1->passes(event);
//                   if(passed_rec){
//                     h_pt_topjet->fill(event);
//                     if(isTTbar){
//                       matched_rec = recmatching->passes(event);
//                       if(matched_rec) h_pt_topjet_matched->fill(event);
//                       else h_pt_topjet_unmatched->fill(event);
//                     }
//                     // exactly 2 TopJets
//                     passed_rec = ntopjet2_sel->passes(event);
//                     if(passed_rec){
//                       h_ntopjet2->fill(event);
//                       if(isTTbar){
//                         matched_rec = recmatching->passes(event);
//                         if(matched_rec) h_ntopjet2_matched->fill(event);
//                         else h_ntopjet2_unmatched->fill(event);
//                       }
//                       // dR(muon, second TopJet) < 0.8
//                       passed_rec = dr_sel->passes(event);
//                       if(passed_rec){
//                       h_dr->fill(event);
//                       if(isTTbar){
//                         matched_rec = recmatching->passes(event);
//                         if(matched_rec) h_dr_matched->fill(event);
//                         else h_dr_unmatched->fill(event);
//                       }
//                         // M(first TopJet) > M(second TopJet)
//                         passed_rec = mass_sel0->passes(event);
//                         if(passed_rec){
//                           h_mass0->fill(event);
//                           if(isTTbar){
//                             matched_rec = recmatching->passes(event);
//                             if(matched_rec) h_mass0_matched->fill(event);
//                             else h_mass0_unmatched->fill(event);
//                           }
//                         }
//                         // M(first TopJet) > M(second TopJet + Muon)
//                         passed_rec_2 = mass_sel1->passes(event);
//                         if(passed_rec_2){
//                           h_mass1->fill(event);
//                           if(isTTbar){
//                             matched_rec = recmatching->passes(event);
//                             if(matched_rec) h_mass1_matched->fill(event);
//                             else h_mass1_unmatched->fill(event);
//                           }
//                         }
//                       }
//                     }
//                   }
//                 }
//               }
//             }
//           }
//         }
//       }
//     }
//
//     if(passed_gen && passed_rec){
//       h_recgen_rec->fill(event);
//       h_recgen_gen->fill(event);
//       if(matched_gen) h_recgen_gen_matched->fill(event);
//       else h_recgen_gen_unmatched->fill(event);
//     }
//
//     if(passed_gen_2 && passed_rec_2){
//       h_recgen_rec_2->fill(event);
//       h_recgen_gen_2->fill(event);
//       if(matched_gen_2) h_recgen_gen_matched_2->fill(event);
//       else h_recgen_gen_unmatched_2->fill(event);
//     }
//
//     if((!passed_rec && !passed_gen) || (!passed_rec_2 && !passed_gen_2)) return false;
//
//     // 3. decide whether or not to keep the current event in the output:
//     return true;
//   }
//
//   // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
//   // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
//   UHH2_REGISTER_ANALYSIS_MODULE(TopSubstructureCombinedModule)
//
// }
