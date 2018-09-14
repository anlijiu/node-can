#include <unistd.h>
#include <vector>
#include <cstring>
#include <cstdint>

#include "tools/utils.h"
#include "can/message.h"
#include "weapon/ammo/ammo.h"

#include "canalystii_controller.h"
#include "store_house.h"
#include "tools/enumerate.h"

namespace can {

// StoreHouseRegister<std::string, WeaponController> CanalystiiController::reg(std::string("canalystii"));
//StoreHouseRegister<std::string, WeaponController, CanalystiiController, std::string, uint8_t> CanalystiiController::reg(std::string("canalystii"));

CanalystiiController::CanalystiiController()
        : WeaponController("Canalystii"), rate_(500)
{
  this->Initialize();
}

CanalystiiController::CanalystiiController(std::string const &name, int rate)
        : WeaponController(name), rate_(rate)
{
  this->Initialize();
}


int CanalystiiController::Initialize() {

  if (!can_node_.start_device()) {
    cout << "canalystii device starts error";
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
  if (!can_node_.init_can_interface(can_idx, vci_conf)) {
    cout << "device port init error";
    return -1;
  }
  cout << "device port init success!!!!!";
}


void
CanalystiiController::onStartReceive()
{
  while(IsReceiving()) {
    VCI_CAN_OBJ can_obj[100];
    unsigned int recv_len = 100;
    unsigned int can_idx = 0;
    //int len = can_node.receive_can_frame(can_idx,can_obj,recv_len,0);
    unsigned int receive_len = 0;
    if(receive_len = can_node_.receive_can_frame(can_idx,can_obj,recv_len,20) > 0){
      cout << "CanalystiiController::onStartReceive  received " << receive_len << " entries message." << std::endl;
      if(!IsReceiving()) return;
      std::vector<Message> messages;
      for(int i = 0; i < receive_len; ++i) {
        if(can_obj[i].ID == 0) continue;
        Message message {
                (uint32_t)can_obj[i].ID,
                (uint8_t)can_obj[i].DataLen,
        };
        std::memcpy(message.raw, can_obj[i].Data, 8);
        messages.push_back(message);
//        printf(" id is %d, hex value: %x, data is ", can_obj[i].ID, can_obj[i].ID);
//        printBits(8, can_obj[i].Data);
//        printf("\n");
      }
      receive_mtx.lock();
      OnReceiveMessages(messages);
      receive_mtx.unlock();

//      OnReceiveMessage(can_obj);
      //ROS_INFO("received:%u",can_obj.ID);
      // canalystii_node_msg::can msg = CANalystii_node::can_obj2msg(can_obj);
      // can_node.can_msg_pub_.publish(msg);
    }
    usleep(10000);
    //ros::spinOnce();
  }
}

void
CanalystiiController::onStopReceive()
{

}

std::tuple<bool, int>
CanalystiiController::Shoot(Ammo const& ammo)
{
  std::tuple<bool, int> result;

  VCI_CAN_OBJ canObj;
  canObj.ID = ammo.id;
  canObj.ExternFlag = 0;   /*CAN_INIT_TYPE_ST*/
  canObj.RemoteFlag = 0;
  canObj.DataLen = ammo.dlc;

  std::memcpy(canObj.Data, &ammo.data, ammo.dlc);

  bool status = can_node_.send_can_frame(0, &canObj, 1);
  result = std::make_tuple(status, 0);
  std::cout << "shoot id:"<<ammo.id << " result is " << status << " data is ";
  printBits(ammo.dlc, canObj.Data);
  return result;
}

std::tuple<bool, int>
CanalystiiController::Shoot(std::vector<Ammo> const& ammos)
{
  std::tuple<bool, int> result;

  VCI_CAN_OBJ canObjs[ammos.size()];
  for(auto e : enumerate(ammos))
  {
    canObjs[e.first].ID = e.second.id;

    canObjs[e.first].ExternFlag = 0;   /*CAN_INIT_TYPE_ST*/
    canObjs[e.first].RemoteFlag = 0;
    canObjs[e.first].DataLen = e.second.dlc;

    std::memcpy(canObjs[e.first].Data, &e.second.data, e.second.dlc);
  }

  bool status = can_node_.send_can_frame(0, canObjs, ammos.size());
  result = std::make_tuple(status, 0);

  return result;

};
//
//bool CanalystiiController::Fire(const can_frame& frame) {
//    VCI_CAN_OBJ canObj;
//    canObj.ID = frame.can_id;
//    canObj.ExternFlag = 0;   /*CAN_INIT_TYPE_ST*/
//    canObj.RemoteFlag = frame.rtr;
//    canObj.DataLen = frame.can_dlc;
//    /* count of data bytes (0..8) */
//    for(int i = 0 ; i <  frame.can_dlc; ++i)
//        canObj.Data[i] = frame.data[i];
//
//    bool status = can_node_.send_can_frame(0, canObj, 1);
//    return status;
//}

bool CanalystiiController::Release() {
  can_node_.close_device();
  return true;
}

} //namespace can
