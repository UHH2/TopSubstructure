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

class calc_Nsubjettiness{
 public:
  explicit calc_Nsubjettiness();
  double tau_one(uhh2::Event & event);
  double tau_two(uhh2::Event & event);
  double tau_three(uhh2::Event & event);
};
