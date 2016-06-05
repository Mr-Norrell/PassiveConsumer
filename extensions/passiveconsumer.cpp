#include "passiveconsumer.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp";
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp";
#include "ns3/ndnSIM/apps/ndn-app.hpp";
#include "ns3/random-variable-stream.h";
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/ndnSIM/ndn-cxx/name.hpp>
#include <ns3/ndnSIM/ndn-cxx/interest.hpp>

    /* This application awaits interests in which m_producerPrefix equals to a partial name of the received ones.
     * Then it sends out interests for m_dataName - which is retreived from received interest.
     */

NS_LOG_COMPONENT_DEFINE ("PassiveConsumer");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (PassiveConsumer);

TypeId
PassiveConsumer::GetTypeId(){
    static TypeId tid = TypeId ("PASSIVECONSUMER")
            .SetGroupName("Ndn")
            .SetParent<ndn::App>()
            .AddConstructor<PassiveConsumer>();
            .AddAttribute("WaitedPrefix", "Prefix of the passive consumer application",
                          StringValue ("/Node"),
                          MakeStringAccessor (&PassiveConsumer::m_producerPrefix),
                          MakeStringChecker())
            /*.AddAttribute("DataName","The Name of the ContentObject to be consumed",
                          StringValue ("/node/Link"),
                          MakeStringAccessor(&ProdConsumer::m_dataName),
                          MakeStringChecker()) */
            .AddAttribute("RequestInterval", "Time interval in which ProdConsumer sends interests.",
                          DoubleValue (1.0),
                          MakeDoubleAccessor (&PassiveConsumer::m_requestInterval),
                          MakeDoubleChecker<double> (0.0, 120.0))
            ;
    return tid;

}

void
PassiveConsumer::StartApplication()
{
    //initialize ndn::App
    ndn::App::StartApplication();

    //Add entry to FIB for m_reciever
    ndn::FibHelper::AddRoute(GetNode(),m_producerPrefix, m_face,0);


    //Schedule send of first interest
    //Update! This app is passive. Thus it is not supposed to be scheduled to send interest.
    //Simulator::Schedule(Seconds(0.0), &ProdConsumer::SendInterest, this);
}

void
PassiveConsumer::StopApplication()
{
    //cleanup ndn::App
    ndn::App::StopApplication();
}

void
PassiveConsumer::SendInterest()
{
    /////////////////////////////////////
    // Sending one Interest packet out //
    /////////////////////////////////////

    //Create and Configure ndn::Interest
    auto interest = std::make_shared <ndn::Interest> (PassiveConsumer::m_dataName);
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    interest->setNonce(rand->GetValue(0,std::numeric_limits<uint32_t>::max()));
    interest->setInterestLifetime(ndn::time::seconds(1));

    NS_LOG_DEBUG("Sending Real Interest Pkt for "<< *interest);

    //Call trace (for logging purposes)
    m_transmittedInterests(interest, this, m_face);

    m_face->onReceiveInterest(*interest);
}

//Callback that will be called when Interest arrives
void
PassiveConsumer::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
    ndn::App::OnInterest(interest);

    NS_LOG_DEBUG("Recieved Interest PKT for " << interest->getName());


    ndn::Name name = interest->getName();
    ndn::Name prefix = name.getSubName(2,1);
    std::string str_prefix = prefix.toUri();



    if (str_prefix == m_producerPrefix){
        name = name.getSubName(2);
        m_dataName = name.toUri();
        PassiveConsumer::SendInterest();
    }

}

//Callback that will be called when Data arrives
/*PassiveConsumer::OnData(std::shared_ptr<const ndn::Data> contentObject)
{
    //Nothing is supposed to happen
}*/




}// namespace ns3

