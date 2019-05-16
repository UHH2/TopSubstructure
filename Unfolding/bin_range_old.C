#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>

using namespace std;

void bin_range(){
  gStyle->SetOptStat(0);
  //Load data
  TFile *TTbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root", "READ");

  // TH1D* tau32 = (TH1D*)TTbar->Get("mass/tau32_tj1_rebin");
  // TH1D* tau32 = (TH1D*)TTbar->Get("rec_dr_sideband/tau32_tj1_rebin");
  TH1D* tau32 = (TH1D*)TTbar->Get("rec_mass_sideband/tau32_tj1_rebin");

  int start = 0;
  for(int i = 0; i < tau32->GetNbinsX(); i++){
    if(tau32->Integral(start, i) >= 107){
      cout << "bin range from: " << start << " to: " << i << '\n';
      cout << tau32->Integral(start, i) << '\n';
      start = i;
    }
  }
  // cout << ""  << tau32->Integral(0, 7015) << '\n';
  cout << "last bin: "  << tau32->Integral(start, 10000) << '\n';
  //
  // cout << "1. range:  "  << tau32->Integral(0   , 3624) << '\n';
  // cout << "2. range:  "  << tau32->Integral(3624, 4053) << '\n';
  // cout << "3. range:  "  << tau32->Integral(4053, 4369) << '\n';
  // cout << "4. range:  "  << tau32->Integral(4369, 4641) << '\n';
  // cout << "5. range:  "  << tau32->Integral(4641, 4904) << '\n';
  // cout << "6. range:  "  << tau32->Integral(4904, 5150) << '\n';
  // cout << "7. range:  "  << tau32->Integral(5150, 5383) << '\n';
  // cout << "8. range:  "  << tau32->Integral(5383, 5625) << '\n';
  // cout << "9. range:  "  << tau32->Integral(5625, 5853) << '\n';
  // cout << "10. range: "  << tau32->Integral(5853, 6081) << '\n';
  // cout << "11. range: "  << tau32->Integral(6081, 6317) << '\n';
  // cout << "12. range: "  << tau32->Integral(6317, 6559) << '\n';
  // cout << "13. range: "  << tau32->Integral(6559, 6790) << '\n';
  // cout << "14. range: "  << tau32->Integral(6790, 7028) << '\n';
  // cout << "15. range: "  << tau32->Integral(7028, 7260) << '\n';
  // cout << "16. range: "  << tau32->Integral(7260, 7508) << '\n';
  // cout << "17. range: "  << tau32->Integral(7508, 7758) << '\n';
  // cout << "18. range: "  << tau32->Integral(7758, 7998) << '\n';
  // cout << "19. range: "  << tau32->Integral(7998, 8224) << '\n';
  // cout << "20. range: "  << tau32->Integral(8224, 8456) << '\n';
  // cout << "21. range: "  << tau32->Integral(8456, 8698) << '\n';
  // cout << "22. range: "  << tau32->Integral(8698, 8989) << '\n';
  // cout << "23. range: "  << tau32->Integral(8989, 10000) << '\n';
  // cout << "24. range: "  << tau32->Integral(, ) << '\n';
  // cout << "25. range: "  << tau32->Integral(5952, 6038) << '\n';
  // cout << "26. range: "  << tau32->Integral(6038, 6129) << '\n';
  // cout << "27. range: "  << tau32->Integral(6129, 6220) << '\n';
  // cout << "27. range: "  << tau32->Integral(6220, 6310) << '\n';
  // cout << "28. range: "  << tau32->Integral(6310, 6398) << '\n';
  // cout << "23. range: "  << tau32->Integral(6398, 6487) << '\n';
  // cout << "24. range: "  << tau32->Integral(6487, 6580) << '\n';
  // cout << "25. range: "  << tau32->Integral(6580, 6671) << '\n';
  // cout << "26. range: "  << tau32->Integral(6671, 6757) << '\n';
  // cout << "27. range: "  << tau32->Integral(6757, 6849) << '\n';
  // cout << "27. range: "  << tau32->Integral(6849, 6939) << '\n';
  // cout << "28. range: "  << tau32->Integral(6939, 7030) << '\n';
  // cout << "23. range: "  << tau32->Integral(7030, 7121) << '\n';
  // cout << "24. range: "  << tau32->Integral(7121, 7212) << '\n';
  // cout << "25. range: "  << tau32->Integral(7212, 7303) << '\n';
  // cout << "26. range: "  << tau32->Integral(7303, 7393) << '\n';
  // cout << "27. range: "  << tau32->Integral(7393, 7479) << '\n';
  // cout << "27. range: "  << tau32->Integral(7479, 7569) << '\n';
  // cout << "28. range: "  << tau32->Integral(7569, 7657) << '\n';
  // cout << "23. range: "  << tau32->Integral(7657, 7746) << '\n';
  // cout << "23. range: "  << tau32->Integral(7746, 7836) << '\n';
  // cout << "24. range: "  << tau32->Integral(7836, 7927) << '\n';
  // cout << "25. range: "  << tau32->Integral(7927, 8013) << '\n';
  // cout << "26. range: "  << tau32->Integral(8013, 8095) << '\n';
  // cout << "27. range: "  << tau32->Integral(8095, 8175) << '\n';
  // cout << "27. range: "  << tau32->Integral(8175, 8257) << '\n';
  // cout << "28. range: "  << tau32->Integral(8257, 8343) << '\n';
  // cout << "24. range: "  << tau32->Integral(8343, 8431) << '\n';
  // cout << "25. range: "  << tau32->Integral(8431, 8517) << '\n';
  // cout << "26. range: "  << tau32->Integral(8517, 8597) << '\n';
  // cout << "27. range: "  << tau32->Integral(8597, 8682) << '\n';
  // cout << "27. range: "  << tau32->Integral(8682, 8769) << '\n';
  // cout << "27. range: "  << tau32->Integral(8769, 8868) << '\n';
  // cout << "27. range: "  << tau32->Integral(8868, 8975) << '\n';
  // cout << "27. range: "  << tau32->Integral(8975, 9101) << '\n';
  // cout << "27. range: "  << tau32->Integral(9101, 9259) << '\n';
  // cout << "27. range: "  << tau32->Integral(9259, 10000) << '\n';
  // cout << "27. range: "  << tau32->Integral(9593, 10000) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "27. range: "  << tau32->Integral(, ) << '\n';
  // cout << "28. range: "  << tau32->Integral(, ) << '\n';
}
