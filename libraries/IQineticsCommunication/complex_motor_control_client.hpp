#ifndef COMPLEX_MOTOR_CONTROL_CLIENT_H
#define COMPLEX_MOTOR_CONTROL_CLIENT_H

#include "communication_interface.h"
#include "client_communication.hpp"

//TODO::Cleanup then include common_message_types and delete the below line
const uint8_t kTypeComplexMotorControl = 51;

class ComplexMotorControlClient: public ClientAbstract{
  public:
    ComplexMotorControlClient(uint8_t obj_idn):
      ClientAbstract(     kTypeComplexMotorControl, obj_idn),
      CmdMode(            kTypeComplexMotorControl, obj_idn, kSubCmdMode),
      CmdPhasePwm(        kTypeComplexMotorControl, obj_idn, kSubCmdPhasePwm),
      CmdPhaseVolts(      kTypeComplexMotorControl, obj_idn, kSubCmdPhaseVolts),
      CmdSpinPwm(         kTypeComplexMotorControl, obj_idn, kSubCmdSpinPwm),
      CmdSpinVolts(       kTypeComplexMotorControl, obj_idn, kSubCmdSpinVolts),
      CmdBrake(           kTypeComplexMotorControl, obj_idn, kSubCmdBrake),
      CmdCoast(           kTypeComplexMotorControl, obj_idn, kSubCmdCoast),
      CmdCalibrate(       kTypeComplexMotorControl, obj_idn, kSubCmdCalibrate),
      CmdVelocity(        kTypeComplexMotorControl, obj_idn, kSubCmdVelocity),
      CmdAngle(           kTypeComplexMotorControl, obj_idn, kSubCmdAngle),
      DrivePwm(           kTypeComplexMotorControl, obj_idn, kSubDrivePwm),
      DriveVolts(         kTypeComplexMotorControl, obj_idn, kSubDriveVolts),
      MechLeadAngle(      kTypeComplexMotorControl, obj_idn, kSubMechLeadAngle),
      ObsSupplyVolts(     kTypeComplexMotorControl, obj_idn, kSubObsSupplyVolts),
      ObsSupplyAmps(      kTypeComplexMotorControl, obj_idn, kSubObsSupplyAmps),
      ObsAngle(           kTypeComplexMotorControl, obj_idn, kSubObsAngle),
      ObsAbsoluteAngle(   kTypeComplexMotorControl, obj_idn, kSubObsAbsoluteAngle),
      ObsVelocity(        kTypeComplexMotorControl, obj_idn, kSubObsVelocity),
      MotorPolePairs(     kTypeComplexMotorControl, obj_idn, kSubMotorPolePairs),
      MotorEmfShape(      kTypeComplexMotorControl, obj_idn, kSubMotorEmfShape),
      MotorKv(            kTypeComplexMotorControl, obj_idn, kSubMotorKv),
      MotorROhm(          kTypeComplexMotorControl, obj_idn, kSubMotorROhm),
      MotorIMax(          kTypeComplexMotorControl, obj_idn, kSubMotorIMax),
      PermuteWires(       kTypeComplexMotorControl, obj_idn, kSubPermuteWires),
      CalibrationAngle(   kTypeComplexMotorControl, obj_idn, kSubCalibrationAngle),
      LeadTime(           kTypeComplexMotorControl, obj_idn, kSubLeadTime),
      CommutationHz(      kTypeComplexMotorControl, obj_idn, kSubCommutationHz),
      ControlHz(          kTypeComplexMotorControl, obj_idn, kSubControlHz),
      PhaseAngle(         kTypeComplexMotorControl, obj_idn, kSubPhaseAngle),
      CalibrationTime(    kTypeComplexMotorControl, obj_idn, kSubCalibrationTime),
      VelocityFilterFc(   kTypeComplexMotorControl, obj_idn, kSubVelocityFilterFc),
      VelocityFilterValue(kTypeComplexMotorControl, obj_idn, kSubVelocityFilterValue),
      VelocityKp(         kTypeComplexMotorControl, obj_idn, kSubVelocityKp),
      VelocityKi(         kTypeComplexMotorControl, obj_idn, kSubVelocityKi),
      VelocityKd(         kTypeComplexMotorControl, obj_idn, kSubVelocityKd),
      VelocityFF0(        kTypeComplexMotorControl, obj_idn, kSubVelocityFF0),
      VelocityFF1(        kTypeComplexMotorControl, obj_idn, kSubVelocityFF1),
      VelocityFF2(        kTypeComplexMotorControl, obj_idn, kSubVelocityFF2),
      AngleKp(            kTypeComplexMotorControl, obj_idn, kSubAngleKp),
      AngleKi(            kTypeComplexMotorControl, obj_idn, kSubAngleKi),
      AngleKd(            kTypeComplexMotorControl, obj_idn, kSubAngleKd),
      EstMotorAmps(       kTypeComplexMotorControl, obj_idn, kSubEstMotorAmps),
      EstMotorTorque(     kTypeComplexMotorControl, obj_idn, kSubEstMotorTorque),
      ObsMotorAmps(       kTypeComplexMotorControl, obj_idn, kSubObsMotorAmps),
      CtrlSpinAmps(       kTypeComplexMotorControl, obj_idn, kSubCtrlSpinAmps),
      CtrlSpinTorque(     kTypeComplexMotorControl, obj_idn, kSubCtrlSpinTorque),
      AmpsKp(             kTypeComplexMotorControl, obj_idn, kSubAmpsKp),
      AmpsKi(             kTypeComplexMotorControl, obj_idn, kSubAmpsKi),
      AmpsKd(             kTypeComplexMotorControl, obj_idn, kSubAmpsKd)
      {};
      
    // Client Entries
    ClientEntry<uint8_t>  CmdMode;
    ClientEntry<float>    CmdPhasePwm;
    ClientEntry<float>    CmdPhaseVolts;
    ClientEntry<float>    CmdSpinPwm;
    ClientEntry<float>    CmdSpinVolts;
    ClientEntryVoid       CmdBrake;
    ClientEntryVoid       CmdCoast;
    ClientEntry<float>    CmdCalibrate;
    ClientEntry<float>    CmdVelocity;
    ClientEntry<float>    CmdAngle;
    ClientEntry<float>    DrivePwm;
    ClientEntry<float>    DriveVolts;
    ClientEntry<float>    MechLeadAngle;
    ClientEntry<float>    ObsSupplyVolts;
    ClientEntry<float>    ObsSupplyAmps;
    ClientEntry<float>    ObsAngle;
    ClientEntry<float>    ObsAbsoluteAngle;
    ClientEntry<float>    ObsVelocity;
    ClientEntry<uint16_t> MotorPolePairs;
    ClientEntry<uint8_t>  MotorEmfShape;
    ClientEntry<float>    MotorKv;
    ClientEntry<float>    MotorROhm;
    ClientEntry<float>    MotorIMax;
    ClientEntry<uint8_t>  PermuteWires;
    ClientEntry<float>    CalibrationAngle;
    ClientEntry<float>    LeadTime;
    ClientEntry<uint32_t> CommutationHz;
    ClientEntry<uint32_t> ControlHz;
    ClientEntry<float>    PhaseAngle;
    ClientEntry<float>    CalibrationTime;
    ClientEntry<uint32_t> VelocityFilterFc;
    ClientEntry<float>    VelocityFilterValue;
    ClientEntry<float>    VelocityKp;
    ClientEntry<float>    VelocityKi;
    ClientEntry<float>    VelocityKd;
    ClientEntry<float>    VelocityFF0;
    ClientEntry<float>    VelocityFF1;
    ClientEntry<float>    VelocityFF2;
    ClientEntry<float>    AngleKp;
    ClientEntry<float>    AngleKi;
    ClientEntry<float>    AngleKd;
    ClientEntry<float>    EstMotorAmps;
    ClientEntry<float>    EstMotorTorque;
    ClientEntry<float>    ObsMotorAmps;
    ClientEntry<float>    CtrlSpinAmps;
    ClientEntry<float>    CtrlSpinTorque;
    ClientEntry<float>    AmpsKp;
    ClientEntry<float>    AmpsKi;
    ClientEntry<float>    AmpsKd;

    void ReadMsg(CommunicationInterface& com, 
      uint8_t* rx_data, uint8_t rx_length)
    {
      static const uint8_t kEntryLength = kSubAmpsKd+1;
      static ClientEntryAbstract* entry_array[kEntryLength] = {
        &CmdMode,             // 0
        &CmdPhasePwm,         // 1
        &CmdPhaseVolts,       // 2
        &CmdSpinPwm,          // 3
        &CmdSpinVolts,        // 4
        &CmdBrake,            // 5
        &CmdCoast,            // 6
        &CmdCalibrate,        // 7
        &CmdVelocity,         // 8
        &CmdAngle,            // 9
        &DrivePwm,            // 10
        &DriveVolts,          // 11
        &MechLeadAngle,       // 12
        &ObsSupplyVolts,      // 13
        &ObsSupplyAmps,       // 14
        &ObsAngle,            // 15
        &ObsAbsoluteAngle,    // 16
        &ObsVelocity,         // 17
        &MotorPolePairs,      // 18
        &MotorEmfShape,       // 19
        &MotorKv,             // 20
        &MotorROhm,           // 21
        &MotorIMax,           // 22
        &PermuteWires,        // 23
        &CalibrationAngle,    // 24
        &LeadTime,            // 25
        &CommutationHz,       // 26
        &ControlHz,           // 27
        &PhaseAngle,          // 28
        &CalibrationTime,     // 29
        &VelocityFilterFc,    // 30
        &VelocityFilterValue, // 31
        &VelocityKp,          // 32
        &VelocityKi,          // 33
        &VelocityKd,          // 34
        &VelocityFF0,         // 35
        &VelocityFF1,         // 36
        &VelocityFF2,         // 37
        &AngleKp,             // 38
        &AngleKi,             // 39
        &AngleKd,             // 40
		nullptr,              // 41
        &EstMotorAmps,        // 42
        &EstMotorTorque,      // 43
        &ObsMotorAmps,        // 44
        &CtrlSpinAmps,        // 45
        &CtrlSpinTorque,      // 46
        &AmpsKp,              // 47
        &AmpsKi,              // 48
        &AmpsKd               // 49
      };
      
      ParseMsg(rx_data, rx_length, entry_array, kEntryLength);
    }    
    
  private:
    static const uint8_t kSubCmdMode             = 0;
    static const uint8_t kSubCmdPhasePwm         = 1;
    static const uint8_t kSubCmdPhaseVolts       = 2;
    static const uint8_t kSubCmdSpinPwm          = 3;
    static const uint8_t kSubCmdSpinVolts        = 4;
    static const uint8_t kSubCmdBrake            = 5;
    static const uint8_t kSubCmdCoast            = 6;
    static const uint8_t kSubCmdCalibrate        = 7;
    static const uint8_t kSubCmdVelocity         = 8;
    static const uint8_t kSubCmdAngle            = 9;
    static const uint8_t kSubDrivePwm            = 10;
    static const uint8_t kSubDriveVolts          = 11;
    static const uint8_t kSubMechLeadAngle       = 12;
    static const uint8_t kSubObsSupplyVolts      = 13;
    static const uint8_t kSubObsSupplyAmps       = 14;
    static const uint8_t kSubObsAngle            = 15;
    static const uint8_t kSubObsAbsoluteAngle    = 16;
    static const uint8_t kSubObsVelocity         = 17;
    static const uint8_t kSubMotorPolePairs      = 18;
    static const uint8_t kSubMotorEmfShape       = 19;
    static const uint8_t kSubMotorKv             = 20;
    static const uint8_t kSubMotorROhm           = 21;
    static const uint8_t kSubMotorIMax           = 22;
    static const uint8_t kSubPermuteWires        = 23;
    static const uint8_t kSubCalibrationAngle    = 24;
    static const uint8_t kSubLeadTime            = 25;
    static const uint8_t kSubCommutationHz       = 26;
    static const uint8_t kSubControlHz           = 27;
    static const uint8_t kSubPhaseAngle          = 28;
    static const uint8_t kSubCalibrationTime     = 29;
    static const uint8_t kSubVelocityFilterFc    = 30;
    static const uint8_t kSubVelocityFilterValue = 31;
    static const uint8_t kSubVelocityKp          = 32;
    static const uint8_t kSubVelocityKi          = 33;
    static const uint8_t kSubVelocityKd          = 34;
    static const uint8_t kSubVelocityFF0         = 35;
    static const uint8_t kSubVelocityFF1         = 36;
    static const uint8_t kSubVelocityFF2         = 37;
    static const uint8_t kSubAngleKp             = 38;
    static const uint8_t kSubAngleKi             = 39;
    static const uint8_t kSubAngleKd             = 40;
    // State deprecated                   = 41
    static const uint8_t kSubEstMotorAmps        = 42;
    static const uint8_t kSubEstMotorTorque      = 43;
    static const uint8_t kSubObsMotorAmps        = 44;
    static const uint8_t kSubCtrlSpinAmps        = 45;
    static const uint8_t kSubCtrlSpinTorque      = 46;
    static const uint8_t kSubAmpsKp              = 47;
    static const uint8_t kSubAmpsKi              = 48;
    static const uint8_t kSubAmpsKd              = 49;
};

#endif // COMPLEX_MOTOR_CONTROL_CLIENT_H
