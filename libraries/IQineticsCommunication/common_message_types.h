/// This header defines type values kTypeXXX and struct structures MsgXXX for
/// common message types. This common_message_types.h header is typically
/// included by a project's unique project_message_types.h header, which may
/// then append new message types.  Common library functions that need to know
/// about message types will include project_message_types.h.
///
/// Metadata is typically handled by wrapping a message type+data inside
/// another message with the appropriate metadate fields.  Examples of metadata
/// include "send this message to X on channel Y and have them reply to me at Z."
///
/// To avoid conflicts, common_message_types.h types range from 1-127 and
/// project_message_types.h types range from 128-255.
///
/// Typically a function component (eg. the imu) will have one base command type
/// used to get/set/save/reply single data members in the standard way.  It may
/// have additional convenience message types for efficient bulk reporting, ect.

#ifndef COMMON_MESSAGE_TYPES_H
#define COMMON_MESSAGE_TYPES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { GET_CMD = 0, SET_CMD = 1, SAVE_CMD = 2, RESPONSE_CMD = 3 } GetSetEnum;
typedef enum {kGet=0, kSet=1, kSave=2, kReply=3} AccessEnum;

////////////////////////////////////////////////////////////////////////////////
// Communications Management

const uint8_t kTypeWarn = 0;

/// Radio metadata message wraper for sending messages over the air.
/// * Don't declare a struct with this, just use it to pack into and extract from
///   byte arrays, in which case the data field length won't matter.
const uint8_t kTypeRadio = 1;
typedef struct __attribute__ ((__packed__)) {
  uint8_t receiver_id;      /// node ID of recipient
  uint8_t sender_id;        /// node ID of sender
  uint8_t type;               /// sent message type
  uint8_t data;               /// first byte of message data
} MsgRadio;

/// Radio send command (typical USB packet to base radio).
/// "Send a message of this type on this_channel with this data."
/// The message might typically be of type MsgRadio
/// * Don't delare a struct with this, just use it to pack into and extract from
///   byte arrays, in which case the data field length won't matter.
const uint8_t kTypeRelaySend = 2;
typedef struct __attribute__ ((__packed__)) {
  uint8_t receiver_channel; /// send on this radio channel
  uint8_t type;                 /// sent message type
  uint8_t data;                 /// first byte of message data
} MsgRelaySend;

/// Request A Message Response
const uint8_t kTypeRequestMessage = 3;
typedef struct __attribute__ ((__packed__)) {
  uint8_t msg_type;
} MsgRequestMessage;

/// Take No Action
const uint8_t kTypeKeepAlive = 4;

////////////////////////////////////////////////////////////////////////////////
// State Estimation

/// State estimate, rad/s rates and quaternion orientation
const uint8_t kTypeEstQuat = 25;
typedef struct __attribute__ ((__packed__)) {
  float t;
  float wx;
  float wy;
  float wz;
  float qw;
  float qx;
  float qy;
  float qz;
} MsgEstQuat;

/// State estimate, rad/s rates and roll, pitch, yaw (ZYX) orientation
const uint8_t kTypeEstEuler = 26;
typedef struct __attribute__ ((__packed__)) {
  float t;
  float wx;
  float wy;
  float wz;
  float roll;
  float pitch;
  float yaw;
} MsgEstEuler;

/// UKF Estimator Commands
const uint8_t kTypeUkf6dof = 27;
typedef struct __attribute__ ((__packed__)) {
  uint8_t cmd;
} MsgUkf6dof;
/*
27 | 0 | // reinitialize estimator
*/

////////////////////////////////////////////////////////////////////////////////
// Components

// used by "system_control.cpp"
const uint8_t kTypeSystemControl        = 5;

// used by "debugger.hpp"
const uint8_t kTypeDebugger             = 6;

// used by "mpu_60XX_imu_msgs.h"
#define TYPE_IMU_VAL                        7
#define TYPE_IMU_COMMAND_VAL                24
#define TYPE_IMU_RAW_VAL                    20
#define TYPE_IMU_METRIC_VAL                 21
#include "mpu_60XX_imu_msgs.h"

// used by "itg_3701_gyro_msgs.h"
#define TYPE_GYRO_VAL                        8
#define TYPE_GYRO_COMMAND_VAL                31
#define TYPE_GYRO_RAW_VAL                    32
#define TYPE_GYRO_METRIC_VAL                 33
#include "itg_3701_gyro_msgs.h"

// used by "brushless_controller_msgs.h"
#define TYPE_BRUSHLESS_CONTROLLER_VAL       10
#define TYPE_BRUSHLESS_CONTROLLER_STATE_VAL 30
#include "brushless_controller_msgs.h"

// used by "pulsing_controller"
#define TYPE_PULSING_CONTROLLER_VAL         28

// used by "time_pulsing_controller"
#define TYPE_TIME_PULSING_CONTROLLER_VAL         34

// used by "battery_monitor_msgs.h"
#define TYPE_BATTERY_MONITOR_VAL            40
#define TYPE_BATTERY_REPORT_VAL             41

// used by "supply_monitor_msgs.h"
#define TYPE_SUPPLY_MONITOR_VAL            40 //Supply is replacement for Battery, so same number
#define TYPE_SUPPLY_REPORT_VAL             41 //Supply is replacement for Battery, so same number

const uint8_t kTypeVoltageMonitor       = 42; // used by "buffered_voltage_monitor.cpp"

// used by "zigbee_interface_msgs.h"
#define TYPE_ZIGBEE_INTERFACE_VAL           12

// used by "usb_radio_msgs.h"
#define TYPE_USB_RADIO_VAL                  13

// used by "can_interface_msgs.h"
#define TYPE_CAN_INTERFACE_VAL              15

const uint8_t kTypeBrushlessDrive       = 50; // used by "brushless_drive.hpp"

const uint8_t kTypeComplexMotorControl  = 51; // used by "complex_motor_control.hpp"

const uint8_t kTypeEncoder              = 53;
const uint8_t kTypeBasicEncoder         = 53; // used by "basic_encoder.cpp"
const uint8_t kTypeBufferedEncoder      = 53; // used by "buffered_encoder.cpp"

const uint8_t kTypeCameraStrobe         = 54; // used by "camera_strobe.cpp"
const uint8_t kTypeMetronome            = 55; // used by "metronome.cpp"
const uint8_t kTypeRevIndicator         = 56; // used by "rev_indicator.cpp"

const uint8_t kTypeAs5047               = 56; // used by "as5047.cpp"

/// PersistentMemory Commands
const uint8_t kTypePersistentMemory = 11;
typedef struct __attribute__ ((__packed__)) {
  uint8_t command; // 0 = format
} MsgPersistentMemory;
// type | sub |
// 11   | 0   | // format memory

// Poll Device for Any Responses
// A contentless message which may be used to signal that communications are
// still alive, to give the device an opportunity to respond, or to illicit
// some generic device response response.
const uint8_t kTypePoll = 14;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COMMON_MESSAGE_TYPES_H