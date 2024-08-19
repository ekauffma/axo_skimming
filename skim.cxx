/*
 * Skimming NanoAOD for AXOL1TL
 */

#include "ROOT/RDataFrame.hxx"

#include "TStopwatch.h"

#include <string>
#include <vector>
#include <iostream>
#include <cmath>

/*
 * Select events which pass any AXO seed
 */
template <typename T>
auto AXOSelection(T &df) {
    return df.Filter("(DST_PFScouting_AXOVLoose == true)"
                     "|| (DST_PFScouting_AXOLoose == true) || (DST_PFScouting_AXONominal == true) || (DST_PFScouting_AXOTight)"
                     "|| (DST_PFScouting_AXOVTight == true) || (HLT_L1AXOVTight == true) || (DST_PFScouting_ZeroBias == true)",
		     "passes AXO trigger seed or ZeroBias");
}


/*
 * Main function for skimming (reduce content to interesting events and write to new files)
 */
int main(int argc, char* argv[]) {
    ROOT::EnableImplicitMT();

    TStopwatch time;
    time.Start();

    ROOT::RDataFrame df("Events", argv[0]);
    std::cout << "Number of events before skimming: " << *df.Count() << std::endl;

    auto dfFinal = AXOSelection(df);
    std::cout << "Number of events after skimming: " << *dfFinal.Count() << std::endl;

    auto report = dfFinal.Report();
    dfFinal.Snapshot("Events", argv[1]);
    time.Stop();

    report->Print();
    time.Print();
}
