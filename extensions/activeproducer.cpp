#include "prodconsumer.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/random-variable-stream.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/double.h"


NS_LOG_COMPONENT_DEFINE ("ProdConsumer");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ProdConsumer);


TypeId
ProdConsumer::GetTypeId(){
    static TypeId tid = TypeId ("ProdConsumer")
            .AddAttribute("RecieverPrefix", "Prefix of the passive consumer application",
                          StringValue ("/localhop/LinkReciever"),
                          MakeStringAccessor (&ProdConsumer::m_reciever),
                          MakeStringChecker())
            .AddAttribute("DataName","The Name of the ContentObject to be consumed",
                          StringValue ("/node/Link"),
                          MakeStringAccessor(&ProdConsumer::m_dataName),
                          MakeStringChecker())
            .AddAttribute("RequestInterval", "Time interval in which ProdConsumer sends interests.",
                          DoubleValue (1.0),
                          MakeDoubleAccessor (&ProdConsumer::m_requestInterval),
                          MakeDoubleChecker<double> (0.0, 120.0))
            ;
    return tid;

}

void
ProdConsumer::StartApplication()
{
    //initialize ndn::App
    ndn::App::StartApplication();
    
    //Add entry to FIB for m_reciever
    ndn::FibHelper::AddRoute(GetNode(), ProdConsumer::m_reciever, ProdConsumer::m_face,0);
    
    //Schedule send of first interest
    Simulator::Schedule(Seconds(0.0), &ProdConsumer::SendInterest, this);
}

void
ProdConsumer::StopApplication()
{
    //cleanup ndn::App
    ndn::App::StopApplication();
}

void
ProdConsumer::SendInterest()
{
    /////////////////////////////////////
    // Sending one Interest packet out //
    /////////////////////////////////////
    
    //Create and Configure ndn::Interest
    auto interest = std::make_shared <ndn::Interest> (ProdConsumer::m_reciever);
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    interest->setNonce(rand->GetValue(0,std::numeric_limits<uint32_t>::max()));
    interest->setInterestLifetime(ndn::time::seconds(1));
    
    NS_LOG_DEBUG("Sending Active Interest Pkt for "<< *interest);
    
    //Call trace (for logging purposes)
    m_transmittedInterests(interest, this, m_face);
    
    m_face->onReceiveInterest(*interest);
}

//Callback that will be called when Interest arrives
void
ProdConsumer::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{

}

//Callback that will be called when Data arrives
/*ProdConsumer::OnData(std::shared_ptr<const ndn::Data> contentObject)
{

    //Nothing is supposed to happen
}*/

    
    
    
}// namespace ns3

