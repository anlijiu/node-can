#ifndef CAN_STORE_HOUSE_H
#define CAN_STORE_HOUSE_H

#include <iostream>
#include <memory>
#include <utility>
#include <typeindex>
#include <type_traits>
#include <map>
#include <vector>
#include <algorithm> 

namespace can {
template<typename Key, typename Object>
class StoreHouse {
private:
  using FakeCreatorFunc = std::add_pointer_t<Object *()>;
  template<typename... Args>
  using RealCreatorFunc = std::add_pointer_t<Object *(Args...)>;

  using RealKey    = std::pair<Key, std::type_index>;
  using CreatorMap = std::map<RealKey, FakeCreatorFunc>;

  template<typename T, typename... Args>
  static Object *CreateFunc(Args... args) {
    return new T{std::forward<Args>(args)...};
  }

  static CreatorMap &getMap() {
    static CreatorMap creators;
    return creators;
  }

public:
  static size_t Size() {
      return getMap().size();
  }

  static std::vector<Key> Keys() {
    std::vector<Key> keys;

    std::transform(
        getMap().begin(),
        getMap().end(),
        std::back_inserter(keys),
        [](const auto &pair){return pair.first.first;}
        );
    return keys;
  }



// Registers a factory function for creating a type derived from base
  template<typename Child, typename... Args>
  struct StoreHouseRegister {
    StoreHouseRegister(Key const &name) {
      StoreHouse<Key, Object>::RegisterItem<Child, Args...>(name);
    }
  };

  using ResultType = std::unique_ptr<Object>;

  template<typename T, typename... Args>
  static typename std::enable_if<std::is_base_of<Object, T>::value && 
  std::is_constructible<T, Args...>::value, bool>::type RegisterItem(Key key) {
    RealKey realKey{key, std::type_index(typeid(RealCreatorFunc<Args...>))};
    auto it = getMap().find(realKey);
    if (it != getMap().end())
      return false;
    getMap()[realKey] = reinterpret_cast<FakeCreatorFunc>( CreateFunc<T, Args...> );
//    std::cout << "11111111111111111111 key : " << key << " typeid: " << typeid( RealCreatorFunc<Args...>).name() << " size is " << sizeof...(Args) <<'\n';
//    std::cout << "11111111111111111111 key : " << key << " typeid: " << typeid( CreateFunc<T, Args...>).name() <<'\n';

    return true;
  }

  template<typename... Args>
  static ResultType Create(Key key, Args... args) {
    RealKey realKey{key, std::type_index(typeid(RealCreatorFunc<Args...>))};
    auto it = getMap().find(realKey);
//
//    std::cout << "22222222222 key: " << key << " typeid: " << typeid( RealCreatorFunc<Args...>).name() << " size is " << sizeof...(args) << '\n';
//    std::cout << std::forward<Key>(key);
//    using expander = int[];
//    (void)expander{0, (void(std::cout << ',' << std::forward<Args>(args)),0)...};
//    std::cout << "\n333333\n";
                        
    if (it == getMap().end()) {

//        std::cout << "\n4444\n";
      return {};
    }

//    std::cout << "\n5555\n";
    auto creator = reinterpret_cast<RealCreatorFunc<Args...>>( it->second );
//    std::cout << "\n66666\n";
    return std::unique_ptr<Object> {creator(std::forward<Args>(args)...)};
  }
};


} //namespace can
#endif // CAN_STORE_HOUSE_H
