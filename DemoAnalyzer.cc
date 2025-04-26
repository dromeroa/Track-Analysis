// -*- C++ -*-
//
// Package:    Tracks/DemoAnalyzer
// Class:      DemoAnalyzer
// 
/**\class DemoAnalyzer DemoAnalyzer.cc Tracks/DemoAnalyzer/plugins/DemoAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  
//         Created:  Fri, 25 Apr 2025 12:07:03 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TH1.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <iostream>
using namespace std;

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

using reco::TrackCollection;

class DemoAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit DemoAnalyzer(const edm::ParameterSet&);
      ~DemoAnalyzer();
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;






      virtual void endJob() override;

      // ----------member data ---------------------------
  edm::EDGetTokenT<reco::TrackCollection> tracksToken_;
  double trackPtMin_;
  double trackEtaMin_;
  double trackEtaMax_;
  TH1D *demohisto_ntracks; 

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DemoAnalyzer::DemoAnalyzer(const edm::ParameterSet& iConfig): 
tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
trackPtMin_(iConfig.getParameter<double>("trackPtMin")),
trackEtaMin_(iConfig.getParameter<double>("trackEtaMin")),
trackEtaMax_(iConfig.getParameter<double>("trackEtaMax")){

	//now do what ever initialization is needed
   edm::Service<TFileService> fs;
   demohisto_ntracks = fs->make<TH1D>("nTracks" , "nTracks" , 100 , 0 , 500);

}


DemoAnalyzer::~DemoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DemoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  using namespace edm;
  using namespace std;
  
  edm::Handle<std::vector<reco::Track>> tracks;

  // Get the tracks from the event using the token
  iEvent.getByToken(tracksToken_, tracks);


  int nTrack = 0;
 
  for (const auto& track : *tracks) {
    // do something with track parameters, e.g, plot the charge.
    // int charge = track.charge();
    if(track.pt() < trackPtMin_) continue;
    if(track.eta() < trackEtaMin_ || track.eta() > trackEtaMax_) continue;
    nTrack++;
  }
  cout<<"nTrack = "<<nTrack<<endl;
  demohisto_ntracks->Fill(nTrack);



 if (!tracks.isValid()) {
    edm::LogWarning("DemoAnalyzer") << "Track collection is invalid!";
} else {
    edm::LogInfo("DemoAnalyzer") << "Number of tracks: " << tracks->size();
} 

 


#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
DemoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DemoAnalyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DemoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DemoAnalyzer);
