import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:FEB13047-2F94-E511-9BB7-02163E0142A0.root'
#        'file:301A497D-70B0-E511-9630-002590D0AFA8.root'
#        'file:90F217E7-60AE-E511-9974-0025905C3D6C.root'
    )
)

process.demo = cms.EDAnalyzer('DemoAnalyzer',
    tracks    = cms.untracked.InputTag('generalTracks'),
    trackPtMin = cms.double(0.3),
    trackEtaMin = cms.double(-2.4),
    trackEtaMax = cms.double(2.4)
                              )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('output.root')
                                  )

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('logfile'),
    logfile = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    )
)


process.p = cms.Path(process.demo)
