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
    return df.Filter("(DST_PFScouting_AXOLoose == true) || (DST_PFScouting_AXONominal == true) || (DST_PFScouting_AXOTight)"
                     "|| (DST_PFScouting_AXOVTight == true) || (DST_PFScouting_ZeroBias == true)",
		     "|| (DST_PFScouting_CICADALoose == true) || (DST_PFScouting_CICADAMedium == true) || (DST_PFScouting_CICADATight == true)"
		     "|| (DST_PFScouting_CICADAVLoose == true) || (DST_PFScouting_CICADAVTight == true)",
		     "passes AXO trigger seed, CICADA trigger seed, or ZeroBias");
}

/* Get rid of branches we have no use for */
std::vector<std::string> DropColumns(std::vector<std::string> &&good_cols)
{
    const std::vector<std::string> blacklist = {
        /* "nScoutingFatJet",
        "ScoutingFatJet_nConstituents",
        "ScoutingFatJet_nCh",
        "ScoutingFatJet_nElectrons",
        "ScoutingFatJet_nMuons",
        "ScoutingFatJet_nNh",
        "ScoutingFatJet_nPhotons",
        "ScoutingFatJet_area",
        "ScoutingFatJet_chEmEF",
        "ScoutingFatJet_chHEF",
        "ScoutingFatJet_eta",
        "ScoutingFatJet_mass",
        "ScoutingFatJet_muEF",
        "ScoutingFatJet_neEmEF",
        "ScoutingFatJet_neHEF",
        "ScoutingFatJet_phi",
        "ScoutingFatJet_pt",
        "ScoutingFatJet_msoftdrop",
        "ScoutingFatJet_n2b1",
        "ScoutingFatJet_n3b1",
        "ScoutingFatJet_particleNet_mass",
        "ScoutingFatJet_particleNet_prob_Hbb",
        "ScoutingFatJet_particleNet_prob_Hcc",
        "ScoutingFatJet_particleNet_prob_Hqq",
        "ScoutingFatJet_particleNet_prob_QCD",
        "ScoutingFatJet_tau1",
        "ScoutingFatJet_tau2",
        "ScoutingFatJet_tau3",
        "ScoutingFatJet_tau4", */
        "nScoutingParticle",
        "ScoutingParticle_charge",
        "ScoutingParticle_pdgId",
        "ScoutingParticle_trkLostInnerHits",
        "ScoutingParticle_trkQuality",
        "ScoutingParticle_vertexIndex",
        "ScoutingParticle_eta",
        "ScoutingParticle_mass",
        "ScoutingParticle_phi",
        "ScoutingParticle_pt",
        "ScoutingParticle_trkDxy",
        "ScoutingParticle_trkDxysig",
        "ScoutingParticle_trkDz",
        "ScoutingParticle_trkDzsig",
        "ScoutingParticle_trkEta",
        "ScoutingParticle_trkNormchi2",
        "ScoutingParticle_trkPhi",
        "ScoutingParticle_trkPt",
        "ScoutingPrimaryVertex_isValidVtx",
        "ScoutingPrimaryVertex_ndof",
        "ScoutingPrimaryVertex_tracksSize",
        "ScoutingPrimaryVertex_chi2",
        "ScoutingPrimaryVertex_x",
        "ScoutingPrimaryVertex_xError",
        "ScoutingPrimaryVertex_y",
        "ScoutingPrimaryVertex_yError",
        "ScoutingPrimaryVertex_z",
        "ScoutingPrimaryVertex_zError",
        "ScoutingTrack_charge",
        "ScoutingTrack_nTrackerLayersWithMeasurement",
        "ScoutingTrack_nValidPixelHits",
        "ScoutingTrack_nValidStripHits",
        "ScoutingTrack_vtxInd",
        "ScoutingTrack__lambda",
        "ScoutingTrack_chi",
        "ScoutingTrack_dsz",
        "ScoutingTrack_dszError",
        "ScoutingTrack_dxy",
        "ScoutingTrack_dxyError",
        "ScoutingTrack_dxy_dsz_cov",
        "ScoutingTrack_dz",
        "ScoutingTrack_dzError",
        "ScoutingTrack_eta",
        "ScoutingTrack_lambdaError",
        "ScoutingTrack_lambda_dsz_cov",
        "ScoutingTrack_lambda_dxy_cov",
        "ScoutingTrack_lambda_phi_cov",
        "ScoutingTrack_ndof",
        "ScoutingTrack_phi",
        "ScoutingTrack_phiError",
        "ScoutingTrack_phi_dsz_cov",
        "ScoutingTrack_phi_dxy_cov",
        "ScoutingTrack_pt",
        "ScoutingTrack_qoverp",
        "ScoutingTrack_qoverpError",
        "ScoutingTrack_qoverp_dsz_cov",
        "ScoutingTrack_qoverp_dxy_cov",
        "ScoutingTrack_qoverp_lambda_cov",
        "ScoutingTrack_qoverp_phi_cov",
        "ScoutingTrack_vx",
        "ScoutingTrack_vy",
        "ScoutingTrack_vz"
    };
    
    // a lambda that checks if `s` is in the blacklist
    auto is_blacklisted = [&blacklist](const std::string &s)  { return std::find(blacklist.begin(), blacklist.end(), s) != blacklist.end(); };
    
    good_cols.erase(std::remove_if(good_cols.begin(), good_cols.end(), is_blacklisted), good_cols.end());
       
    return good_cols;
}


/*
 * Main function for skimming (reduce content to interesting events and write to new files)
 */
int main(int argc, char* argv[]) {
    ROOT::EnableImplicitMT();

    TStopwatch time;
    time.Start();

    ROOT::RDataFrame df("Events", argv[1]);
    std::cout << "Number of events before skimming: " << *df.Count() << std::endl;

    auto dfFinal = AXOSelection(df);
    std::cout << "Number of events after skimming: " << *dfFinal.Count() << std::endl;

    auto report = dfFinal.Report();
    dfFinal.Snapshot("Events", argv[2], DropColumns(dfFinal.GetColumnNames()));
    time.Stop();

    report->Print();
    time.Print();
}
