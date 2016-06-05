#ifndef PASSIVECONSUMER_H
#define PASSIVECONSUMER_H

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/ndnSIM/ndn-cxx/name.hpp"

namespace ns3 {

class PassiveConsumer : public ndn::App {
public:
    // register NS-3 type "ProdConsumer"
    static TypeId
    GetTypeId();

    //
    virtual void
    StartApplication();

    //
    virtual void
    StopApplication();

    //
    virtual void
    OnInterest(std::shared_ptr<const ndn::Interest> interest);

    //virtual void
    //OnData(std::shared_ptr<const ndn::Data> contentObject);

private:
    void
    SendInterest();
    std::string m_ownPrefix;
    std::string m_producerPrefix;
    std::string m_dataName;
    double m_requestInterval;

};

} //namespace ns3

#endif // PASSIVECONSUMER_H
