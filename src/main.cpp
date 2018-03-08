#include <iostream>
#include <memory>
#include <bitset>

#include "weapon/canalystii.h"
#include "weapon/weapon_controller.h"
#include "store_house.h"
#include "tools/utils.h"
#include "can/signal_build_const_strategy.h"
#include "can/signal_build_strategy.h"
#include "can/signal_meta.h"

using std::cout;
using std::endl;
using std::bitset;

using can::StoreHouse;
using can::WeaponController;
using can::SignalBuildConstStrategy;
using can::SignalBuildStrategy;
using can::ByteOrder;
using can::SignalMeta;

int main(int argc, char **argv) {
  auto weapon_controller = WeaponStoreHouse::Create( "canalystii", std::string("canalystii"), 500 );
  size_t size = StoreHouse< std::string, WeaponController >::Size();
  std::vector keys = StoreHouse< std::string, WeaponController >::Keys();
  cout << "  StoreHouse< std::string, WeaponController >::Size()  is " << size << " keys.at(0) is " << keys.at(0);
  auto speed_meta = std::make_shared<SignalMeta>(
          SignalMeta{ "speed", 6, 6, ByteOrder::kBigEndian, 0.056, 0.0, 0.0, 460, false, "Km/h"});
//  auto weapon_controller = WeaponStoreHouse::Create( "canalystii", std::string("canalystii"), 500 );
  std::shared_ptr<SignalBuildStrategy> signal_build_strategy = std::make_shared<SignalBuildConstStrategy>(speed_meta, 5);
  std::bitset<64> s = can::reverse<64>(signal_build_strategy->generate());
  cout << "\n bitset<64> s  is " << s;
//  std::unique_ptr<WeaponController> weapon_controller = std::move(
//          StoreHouse<std::string, WeaponController>::Create(std::string("canalystii"), std::string("canalystii"),
//                                                            (uint8_t) 0x1C));
  if (weapon_controller != nullptr) {
    cout << weapon_controller->Name() << '\n';
    weapon_controller->Initialize();
  }

  const uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
  enum ByteOrder order = ByteOrder::kBigEndian;
  // enum can::ByteOrder order = can::ByteOrder::kLittleEndian;
  long ii = extractBits(data, 6, 7, order);
  cout << " \nii is " << ii;

  // ros::init(argc, argv, "canalystii_node");
#if 0
  CANalystii can_node;

  if(!can_node.start_device()){
      // ROS_WARN("device starts error");
      cout << "device starts error";

      return -1;
  }

  VCI_INIT_CONFIG vci_conf;
  vci_conf.AccCode = 0x80000008;
  vci_conf.AccMask = 0xFFFFFFFF;
  vci_conf.Filter = 1;//receive all frames
  vci_conf.Timing0 = 0x00;
  vci_conf.Timing1 = 0x1C;//baudrate 500kbps
  vci_conf.Mode = 0;//normal mode
  unsigned int can_idx = 0;
  if(!can_node.init_can_interface(can_idx,vci_conf)){
      // ROS_WARN("device port init error");

      cout << "device port init error";
      return -1;
  }

  // ROS_INFO("listening to can bus");
  VCI_CAN_OBJ can_obj;
  while(true){
      unsigned int recv_len = 1;

      //int len = can_node.receive_can_frame(can_idx,can_obj,recv_len,0);
      if(can_node.receive_can_frame(can_idx,can_obj,recv_len,20)){
          cout << "receive can  " << can_obj.ID << " " << can_obj.Data << endl;
          //ROS_INFO("received:%u",can_obj.ID);
          // canalystii_node_msg::can msg = CANalystii_node::can_obj2msg(can_obj);
          // can_node.can_msg_pub_.publish(msg);
      }
      //ros::spinOnce();
  }
  //ros::spin();
#endif
  return 0;
}
