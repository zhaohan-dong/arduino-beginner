/*
   This file is part of ArduinoIoTCloud.

   Copyright 2019 ARDUINO SA (http://www.arduino.cc/)

   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html

   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to license@arduino.cc.
*/

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <ArduinoIoTCloud.h>

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/

void ArduinoIoTCloudClass::addCallback(ArduinoIoTCloudEvent const event, OnCloudEventCallback callback)
{
  _cloud_event_callback[static_cast<size_t>(event)] = callback;
}

void ArduinoIoTCloudClass::addPropertyReal(ArduinoCloudProperty& property, String name, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  addPropertyReal(property, name, -1, permission_type, seconds, fn, minDelta, synFn);
}

void ArduinoIoTCloudClass::addPropertyReal(ArduinoCloudProperty& property, String name, int tag, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  Permission permission = Permission::ReadWrite;
  if (permission_type == READ) {
    permission = Permission::Read;
  } else if (permission_type == WRITE) {
    permission = Permission::Write;
  } else {
    permission = Permission::ReadWrite;
  }

  if (seconds == ON_CHANGE) {
    _thing.addPropertyReal(property, name, permission, tag).publishOnChange(minDelta, DEFAULT_MIN_TIME_BETWEEN_UPDATES_MILLIS).onUpdate(fn).onSync(synFn);
  } else {
    _thing.addPropertyReal(property, name, permission, tag).publishEvery(seconds).onUpdate(fn).onSync(synFn);
  }
}

void ArduinoIoTCloudClass::addPropertyReal(bool& property, String name, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  addPropertyReal(property, name, -1, permission_type, seconds, fn, minDelta, synFn);
}

void ArduinoIoTCloudClass::addPropertyReal(bool& property, String name, int tag, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  ArduinoCloudProperty* p = new CloudWrapperBool(property);
  addPropertyReal(*p, name, tag, permission_type, seconds, fn, minDelta, synFn);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(bool& property, String name, Permission const permission)
{
  return addPropertyReal(property, name, -1, permission);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(bool& property, String name, int tag, Permission const permission)
{
  ArduinoCloudProperty* p = new CloudWrapperBool(property);
  return _thing.addPropertyReal(*p, name, permission, tag);
}

void ArduinoIoTCloudClass::addPropertyReal(float& property, String name, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  addPropertyReal(property, name, -1, permission_type, seconds, fn, minDelta, synFn);
}

void ArduinoIoTCloudClass::addPropertyReal(float& property, String name, int tag, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  ArduinoCloudProperty* p = new CloudWrapperFloat(property);
  addPropertyReal(*p, name, tag, permission_type, seconds, fn, minDelta, synFn);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(float& property, String name, Permission const permission)
{
  return addPropertyReal(property, name, -1, permission);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(float& property, String name, int tag, Permission const permission)
{
  ArduinoCloudProperty* p = new CloudWrapperFloat(property);
  return _thing.addPropertyReal(*p, name, permission, tag);
}

void ArduinoIoTCloudClass::addPropertyReal(int& property, String name, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  addPropertyReal(property, name, -1, permission_type, seconds, fn, minDelta, synFn);
}

void ArduinoIoTCloudClass::addPropertyReal(int& property, String name, int tag, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  ArduinoCloudProperty* p = new CloudWrapperInt(property);
  addPropertyReal(*p, name, tag, permission_type, seconds, fn, minDelta, synFn);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(int& property, String name, Permission const permission)
{
  return addPropertyReal(property, name, -1, permission);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(int& property, String name, int tag, Permission const permission)
{
  ArduinoCloudProperty* p = new CloudWrapperInt(property);
  return _thing.addPropertyReal(*p, name, permission, tag);
}

void ArduinoIoTCloudClass::addPropertyReal(String& property, String name, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  addPropertyReal(property, name, -1, permission_type, seconds, fn, minDelta, synFn);
}

void ArduinoIoTCloudClass::addPropertyReal(String& property, String name, int tag, permissionType permission_type, long seconds, void(*fn)(void), float minDelta, void(*synFn)(ArduinoCloudProperty & property))
{
  ArduinoCloudProperty* p = new CloudWrapperString(property);
  addPropertyReal(*p, name, tag, permission_type, seconds, fn, minDelta, synFn);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(String& property, String name, Permission const permission)
{
  return addPropertyReal(property, name, -1, permission);
}

ArduinoCloudProperty& ArduinoIoTCloudClass::addPropertyReal(String& property, String name, int tag, Permission const permission)
{
  ArduinoCloudProperty* p = new CloudWrapperString(property);
  return _thing.addPropertyReal(*p, name, permission, tag);
}

/******************************************************************************
 * PROTECTED MEMBER FUNCTIONS
 ******************************************************************************/

NetworkConnectionState ArduinoIoTCloudClass::checkPhyConnection()
{
  NetworkConnectionState const connect_state = _connection->check();

  if (_connection->check() != NetworkConnectionState::CONNECTED)
  {
    if (_iot_status == ArduinoIoTConnectionStatus::CONNECTED)
    {
      disconnect();
    }
  }

  return connect_state;
}

void ArduinoIoTCloudClass::execCloudEventCallback(ArduinoIoTCloudEvent const event)
{
  OnCloudEventCallback callback = _cloud_event_callback[static_cast<size_t>(event)];
  if (callback) {
    (*callback)();
  }
}

void ArduinoIoTCloudClass::printConnectionStatus(ArduinoIoTConnectionStatus status)
{
  switch (status)
  {
    case ArduinoIoTConnectionStatus::IDLE:         Debug.print(DBG_INFO,  "Arduino IoT Cloud Connection status: IDLE");         break;
    case ArduinoIoTConnectionStatus::ERROR:        Debug.print(DBG_ERROR, "Arduino IoT Cloud Connection status: ERROR");        break;
    case ArduinoIoTConnectionStatus::CONNECTING:   Debug.print(DBG_INFO,  "Arduino IoT Cloud Connection status: CONNECTING");   break;
    case ArduinoIoTConnectionStatus::RECONNECTING: Debug.print(DBG_INFO,  "Arduino IoT Cloud Connection status: RECONNECTING"); break;
    case ArduinoIoTConnectionStatus::CONNECTED:    Debug.print(DBG_INFO,  "Arduino IoT Cloud Connection status: CONNECTED");    break;
    case ArduinoIoTConnectionStatus::DISCONNECTED: Debug.print(DBG_ERROR, "Arduino IoT Cloud Connection status: DISCONNECTED"); break;
  }
}
