#ifndef INTELLIGENCE_AGENCY_H
#define INTELLIGENCE_AGENCY_H

#include <map>
#include <napi.h>
#include <vector>
#include "can/message.h"
#include "agent.h"

/**
 * javascript 可以这样使用:
 * IntelligenceAgency.onSignalChange((name, value) => {
 *   console.log(name, value)
 * })
 */
namespace can {

class IntelligenceAgency {
public:

  static void RegisterAgent(Agent* agent);
  static void UnRegisterAgent(Agent* agent);

  static bool ReceiveMessages(std::vector<Message> const& messages);
  static bool ReceiveUnknownMessages(std::vector<Message> const& messages);

private:
  static std::vector<Agent*> agents;
};


} //namespace can

#endif //INTELLIGENCE_AGENCY_H
