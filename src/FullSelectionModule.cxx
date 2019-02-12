#include <iostream>
#include <memory>

#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"

#include "UHH2/common/include/CommonModules.h"
#include "UHH2/common/include/CleaningModules.h"
#include "UHH2/common/include/ElectronHists.h"
#include "UHH2/common/include/MuonHists.h"
#include "UHH2/common/include/NSelections.h"
#include "UHH2/common/include/JetHists.h"
#include "UHH2/common/include/TriggerSelection.h"
#include "UHH2/common/include/MuonIds.h"
#include "UHH2/common/include/ElectronIds.h"
#include "UHH2/common/include/JetIds.h"
#include "UHH2/common/include/TopJetIds.h"
#include "UHH2/common/include/MCWeight.h"
#include "UHH2/common/include/TTbarGen.h"

#include "UHH2/TopSubstructure/include/GenHists.h"
#include "UHH2/TopSubstructure/include/TopSubstructureUtils.h"

using namespace std;
using namespace uhh2;

namespace uhh2examples {
  class FullSelectionModule: public AnalysisModule {
  public:
    explicit FullSelectionModule(Context & ctx);
    virtual bool process(Event & event) override;

  private:
    bool passed_1, passed_2;
    // declare the Selections to use. Use unique_ptr to ensure automatic call of delete in the destructor, to avoid memory leaks.

    std::unique_ptr<AnalysisModule> ttgenprod;
    std::unique_ptr<AnalysisModule> genparticle_rm;

    // store the Hists collection as member variables. Again, use unique_ptr to avoid memory leaks.
    std::unique_ptr<Hists> h_nsubjets_1, h_pt_top_quark_1, h_nsubjets_2, h_pt_top_quark_2;

    uhh2::Event::Handle<double> h_gen_weight;
  };


  FullSelectionModule::FullSelectionModule(Context & ctx){
    // 1. setup other modules. CommonModules and the JetCleaner:
    h_gen_weight = ctx.declare_event_output<double>("h_gen_weight");
    ctx.undeclare_event_output("GenParticles");
    ctx.undeclare_event_output("PFParticles");

    // 2. set up selections
    const std::string ttbar_gen_label("ttbargen");
    ttgenprod.reset(new TTbarGenProducer(ctx, ttbar_gen_label, false));
    genparticle_rm.reset(new GenParticleRemover(ctx));


    // 3. Set up Hists classes:
    h_nsubjets_1.reset(new GenHists(ctx, "nsubjets_1"));
    h_pt_top_quark_1.reset(new GenHists(ctx, "pt_top_quark_1"));
    h_nsubjets_2.reset(new GenHists(ctx, "nsubjets_2"));
    h_pt_top_quark_2.reset(new GenHists(ctx, "pt_top_quark_2"));
  }


  bool FullSelectionModule::process(Event & event) {
    cout << "FullSelectionModule: Starting to process event (runid, eventid) = (" << event.run << ", " << event.event << "); weight = " << event.weight << endl;
    passed_1 = false;
    passed_2 = false;
    event.set(h_gen_weight, 1);
    // 1. run all modules other modules.


    ttgenprod->process(event);

    sort_by_pt<GenTopJet>(*event.gentopjets);

    std::vector<GenParticle> genp;
    for(unsigned int i=0; i < event.genparticles->size(); i++){
      if(abs(event.genparticles->at(i).pdgId()) == 6){
        genp.push_back(event.genparticles->at(i));
      }
    }
    if(genp.size() == 2){
      std::vector<GenParticle> dummy;
      if(genp[0].v4().pt() > genp[1].v4().pt()){
        dummy.push_back(genp[0]);
        dummy.push_back(genp[1]);
      }
      else{
        dummy.push_back(genp[1]);
        dummy.push_back(genp[0]);
      }

      if(dummy.at(0).pt() > 130){
        h_pt_top_quark_1->fill(event);
        passed_1 = true;
        switch (event.gentopjets->size()){
          case 0:
            passed_1 = false;
            break;
          case 1:
            passed_1 = event.gentopjets->at(0).subjets().size() > 0;
            break;
          default:
            for(unsigned int i=0; i < event.gentopjets->size(); i++){
              if(event.gentopjets->at(i).subjets().size() > 0){
                passed_1 = true;
                break;
              }
            }
            break;
        }
        if(passed_1){
          h_nsubjets_1->fill(event);
        }
      }


      switch (event.gentopjets->size()){
        case 0:
          passed_2 = false;
          break;
        case 1:
          passed_2 = event.gentopjets->at(0).subjets().size() > 0;
          break;
        default:
          for(unsigned int i=0; i < event.gentopjets->size(); i++){
            if(event.gentopjets->at(i).subjets().size() > 0){
              passed_2 = true;
              break;
            }
          }
          break;
      }
      if(passed_2){
        h_nsubjets_2->fill(event);
        passed_2 = false;
        if(dummy.at(0).pt() > 130){
          h_pt_top_quark_2->fill(event);
          passed_2 = true;
        }
      }

    }
    if(!passed_1 && !passed_2) return false;
    // genparticle_rm->process(event);
    return true;
  }
  // as we want to run the ExampleCycleNew directly with AnalysisModuleRunner,
  // make sure the TopSubstructureModule is found by class name. This is ensured by this macro:
  UHH2_REGISTER_ANALYSIS_MODULE(FullSelectionModule)

}
