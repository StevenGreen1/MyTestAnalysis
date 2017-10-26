////////////////////////////////////////////////////////////////////////
// Class:       MyTestAnalyzer
// Plugin Type: analyzer (art v2_08_03)
// File:        MyTestAnalyzer_module.cc
//
// Generated at Fri Oct 20 15:42:43 2017 by Lorena Escudero Sanchez using cetskelgen
// from cetlib version v3_01_01.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

//to save our tree
#include "TTree.h"                                       
#include "TFile.h"                                                                                                             
#include "art/Framework/Services/Optional/TFileService.h"

#include "lardataobj/RecoBase/PFParticle.h"
#include "canvas/Persistency/Common/FindManyP.h"

namespace test {
  class MyTestAnalyzer;
}


class test::MyTestAnalyzer : public art::EDAnalyzer {
public:
  explicit MyTestAnalyzer(fhicl::ParameterSet const & p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.
  virtual ~MyTestAnalyzer();

  // Plugins should not be copied or assigned.
  MyTestAnalyzer(MyTestAnalyzer const &) = delete;
  MyTestAnalyzer(MyTestAnalyzer &&) = delete;
  MyTestAnalyzer & operator = (MyTestAnalyzer const &) = delete;
  MyTestAnalyzer & operator = (MyTestAnalyzer &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;

  // Selected optional functions.
  void reconfigure(fhicl::ParameterSet const & p) override;

private:
    typedef std::vector< art::Ptr<recob::Hit>> HitVector;
    typedef std::map< art::Ptr<recob::Cluster>, HitVector> ClustersToHits;

  // Declare member data here.
  TFile * m_file;
  TTree * m_tree;
  unsigned int nEvent;
    std::string m_particleLabel;
    int m_nPFParticles;
    unsigned int m_nTracks;
    unsigned int m_nShowers;
};


test::MyTestAnalyzer::MyTestAnalyzer(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{
    nEvent = 0;
    m_nPFParticles = -1;
    m_nTracks = 0;
    m_nShowers = 0;

  //create output tree                                                                                                             
  art::ServiceHandle<art::TFileService> tfs;                                                                                       
  m_file = new TFile("MyTestAnalyzer.root", "RECREATE");                                                                 
  m_tree = tfs->make<TTree>("tree","Tree");
  //add branches
    m_tree->Branch("nEvent",&nEvent,"nEvent/I");  
    m_tree->Branch("nPFParticles",&m_nPFParticles,"nPFParticles/I");  

  this->reconfigure(p);

}

test::MyTestAnalyzer::~MyTestAnalyzer()
{                                                                                                                                 
  //store output tree                                                                                                         
  m_file->cd();                                                                                                               
  m_tree->Write("tree");                                                                                            
  m_file->Close();                                                                                                        
  std::cout << "End!" << std::endl;                                                                                     
}

void test::MyTestAnalyzer::analyze(art::Event const & e)
{
  // Implementation of required member function here.
    art::Handle< std::vector<recob::Hit>

    art::Handle< std::vector<recob::PFParticle> > theParticles;
    e.getByLabel(m_particleLabel, theParticles);
    m_nPFParticles = -1;
    m_nPFParticles = theParticles->size();

    nEvent++;

    m_nTracks = 0;
    m_nShowers = 0;

    for (unsigned int i =0; i < theParticles->size(); i++)
    {
        const art::Ptr<recob::PFParticle> particle(theParticles, i);
        const int pdg(particle->PdgCode());
        if (pdg == 13)
        {
            m_nTracks++;
        }
        else if (pdg == 11)
        {
            m_nShowers++;
        }
        else
        {
            std::cout << "Found PFParticle with PDG code: " << pdg << std::endl;
        }
    }

    m_tree->Fill();
}

void test::MyTestAnalyzer::reconfigure(fhicl::ParameterSet const & p)
{
  // Implementation of optional member function here.
  //here you add an external fcl file to change configuration
    m_particleLabel = p.get<std::string>("PFParticleModule", "pandoraNu");
}

DEFINE_ART_MODULE(test::MyTestAnalyzer)
