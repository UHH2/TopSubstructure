#pragma once

#include "UHH2/core/include/fwd.h"
#include "UHH2/core/include/AnalysisModule.h"
#include "UHH2/core/include/Event.h"
#include "UHH2/core/include/GenTopJet.h"

#include "UHH2/common/include/Utils.h"
#include "UHH2/common/include/CommonModules.h"
#include <UHH2/common/include/TTbarGen.h>
#include "fastjet/PseudoJet.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/SoftDrop.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
// #include "UHH2/core/plugins/NtupleWriterJets.h"

#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace fastjet;
using namespace contrib;

class Calculator{
 public:
  explicit Calculator(uhh2::Context &, std::string const & label_);

  double PF_tau1(uhh2::Event & event);
  double PF_tau2(uhh2::Event & event);
  double PF_tau3(uhh2::Event & event);
  double Gen_tau1(uhh2::Event & event);
  double Gen_tau2(uhh2::Event & event);
  double Gen_tau3(uhh2::Event & event);

private:
  uhh2::Event::Handle<std::vector<GenTopJet>> hndl_gen;
  uhh2::Event::Handle<std::vector<TopJet>> hndl;
  string label;
};
