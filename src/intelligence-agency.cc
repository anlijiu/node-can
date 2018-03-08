#include <napi.h>
#include <tools/enumerate.h>
#include <iostream>

#include "intelligence-agency.h"

namespace can {
std::vector<Agent*> IntelligenceAgency::agents;

void IntelligenceAgency::RegisterAgent(Agent* agent)
{
  agents.push_back(agent);
}
void IntelligenceAgency::UnRegisterAgent(Agent* agent)
{
  std::vector<Agent*>::iterator it = std::find( agents.begin( ), agents.end( ), agent );
  if(it != agents.end()) {
    agents.erase(it);
  }
}

bool
IntelligenceAgency::ReceiveUnknownMessages(std::vector<Message> const& messages)
{
  for(const auto agent: agents) {
//    agent->ReceiveUnknownMessages(messages);
  }
}

bool
IntelligenceAgency::ReceiveMessages(std::vector<Message> const& messages)
{
  for(const auto agent: agents) {
//    agent->ReceiveMessages(messages);
  }
}

} //namespace can
