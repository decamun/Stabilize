#ifndef BUFFERED_VOLTAGE_MONITOR_CLIENT_H
#define BUFFERED_VOLTAGE_MONITOR_CLIENT_H

#include "communication_interface.h"
#include "client_communication.hpp"

//TODO::Cleanup then include common_message_types and delete the below line
const uint8_t kTypeVoltageMonitor = 42;

class BufferedVoltageMonitorClient: public ClientAbstract{
  public:
    BufferedVoltageMonitorClient(uint8_t obj_idn):
      ClientAbstract(kTypeVoltageMonitor, obj_idn),
      VoltsRaw(kTypeVoltageMonitor,obj_idn,kSubVoltsRaw),
      Volts(kTypeVoltageMonitor,obj_idn,kSubVolts),
      VoltsGain(kTypeVoltageMonitor,obj_idn,kSubVoltsGain),
      FilterFs(kTypeVoltageMonitor,obj_idn,kSubFilterFs),
      FilterFc(kTypeVoltageMonitor,obj_idn,kSubFilterFc)
      {};
      
    // Client Entries
    ClientEntry<float> VoltsRaw;
    ClientEntry<float> Volts;
    ClientEntry<float> VoltsGain;
    ClientEntry<uint32_t> FilterFs;
    ClientEntry<uint32_t> FilterFc;

    void ReadMsg(CommunicationInterface& com, 
      uint8_t* rx_data, uint8_t rx_length)
    {
      static const uint8_t kEntryLength = kSubFilterFc+1;
      static ClientEntryAbstract* entry_array[kEntryLength] = {
        &VoltsRaw,  // 0
        nullptr,    // 1
        &Volts,     // 2
        nullptr,    // 3
        nullptr,    // 4
        nullptr,    // 5
        nullptr,    // 6
        nullptr,    // 7
        nullptr,    // 8
        nullptr,    // 9
        nullptr,    // 10
        &VoltsGain, // 11
        nullptr,    // 12
        nullptr,    // 13
        nullptr,    // 14
        nullptr,    // 15
        &FilterFs,  // 16
        &FilterFc   // 17
      };
      
      ParseMsg(rx_data, rx_length, entry_array, kEntryLength);
    }    
    
  private:
    static const uint8_t kSubVoltsRaw  = 0;
    static const uint8_t kSubVolts     = 2;
    static const uint8_t kSubVoltsGain = 11;
    static const uint8_t kSubFilterFs  = 16;
    static const uint8_t kSubFilterFc  = 17;
};

#endif // BUFFERED_VOLTAGE_MONITOR_CLIENT_H
