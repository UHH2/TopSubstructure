#include "TFile.h"
#include "TTree.h"
#include <TH1F.h>
#include <cmath>

using namespace std;

void bin_range(){
  gStyle->SetOptStat(0);
  //Load data
  TFile *TTbar = new TFile("/nfs/dust/cms/user/skottkej/TopSubstructure/Selection/Post_kin_full_sel_cmssw10/uhh2.AnalysisModuleRunner.MC.TTbar_2016v3.root", "READ");

  TH1D* tau32 = (TH1D*)TTbar->Get("mass/tau32_tj1_rebin");

  int start = 0;
  for(int i = 0; i < tau32->GetNbinsX(); i++){
    if(tau32->Integral(start, i) >= 102.5){
      cout << "bin range from: " << start << " to: " << i << '\n';
      cout << tau32->Integral(start, i) << '\n';
      start = i;
    }
  }
  cout << "1. range: " << tau32->Integral(0, 3533) << '\n';
  cout << "2. range: "  << tau32->Integral(3533, 3940) << '\n';
  cout << "3. range: "  << tau32->Integral(3940, 4215) << '\n';
  cout << "4. range: "  << tau32->Integral(4215, 4456) << '\n';
  cout << "5. range: "  << tau32->Integral(4456, 4679) << '\n';
  cout << "6. range: "  << tau32->Integral(4679, 4898) << '\n';
  cout << "7. range: "  << tau32->Integral(4898, 5101) << '\n';
  cout << "8. range: "  << tau32->Integral(5101, 5294) << '\n';
  cout << "9. range: "  << tau32->Integral(5294, 5487) << '\n';
  cout << "10. range: "  << tau32->Integral(5487, 5680) << '\n';
  cout << "11. range: "  << tau32->Integral(5680, 5861) << '\n';
  cout << "12. range: "  << tau32->Integral(5861, 6045) << '\n';
  cout << "13. range: "  << tau32->Integral(6045, 6230) << '\n';
  cout << "14. range: "  << tau32->Integral(6230, 6418) << '\n';
  cout << "15. range: "  << tau32->Integral(6418, 6609) << '\n';
  cout << "16. range: "  << tau32->Integral(6609, 6795) << '\n';
  cout << "17. range: "  << tau32->Integral(6795, 6986) << '\n';
  cout << "18. range: "  << tau32->Integral(6986, 7169) << '\n';
  cout << "19. range: "  << tau32->Integral(7169, 7354) << '\n';
  cout << "20. range: "  << tau32->Integral(7354, 7539) << '\n';
  cout << "21. range: "  << tau32->Integral(7539, 7731) << '\n';
  cout << "22. range: "  << tau32->Integral(7731, 7923) << '\n';
  cout << "23. range: "  << tau32->Integral(7923, 8096) << '\n';
  cout << "24. range: "  << tau32->Integral(8096, 8271) << '\n';
  cout << "25. range: "  << tau32->Integral(8271, 8449) << '\n';
  cout << "26. range: "  << tau32->Integral(8449, 8621) << '\n';
  cout << "27. range: "  << tau32->Integral(8621, 8820) << '\n';
  cout << "28. range: "  << tau32->Integral(8820, 9075) << '\n';
  cout << "29. range: "  << tau32->Integral(9075, 10000) << '\n';
}
