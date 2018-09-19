
#include "ammo_generator.h"
#include "tools/utils.h"
#include <3rdparty/endian/endianness_config.h>

namespace can {

AmmoGenerator::AmmoGenerator(std::shared_ptr<MessageMeta> message_meta,
                             std::vector<AmmoPartGenerator> const& generators)
        : message_meta_(message_meta)
{
  std::vector<std::string> signal_names = message_meta_->signal_names;
//  for (auto t = generators.begin(); t != generators.end(); ++t) {
//    generators_.push_back(std::move(*t));
//  }

  generators_.assign(generators.begin(),
            generators.end());
}

uint32_t
AmmoGenerator::MsgId()
{
  return message_meta_->id;
}

Ammo
AmmoGenerator::Generate() {
  uint64_t bitset = 0;
  Ammo ammo {
    message_meta_->id,
    message_meta_->dlc,
    {0}
  };

  for(auto t : generators_) {
    t.generate(ammo.data);
  }

  return ammo;
}

void
AmmoGenerator::SetStrategy(std::string signal_name, std::shared_ptr<PartBuildStrategy> strategy)
{
  std::vector<AmmoPartGenerator>::iterator it =
          std::find_if(generators_.begin(), generators_.end(),
                       [&](AmmoPartGenerator & obj){ return obj.Name() == signal_name;});
  if(it != generators_.end()) {
    (*it).SetStrategy(strategy);
  }
}
}
