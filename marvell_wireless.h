/*
 * Copyright (C) 2018 The LineageOS Project
 *                     Nemirtingas <nanaki89@hotmail.fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __WIRELESS_INTERFACE_
#define __WIRELESS_INTERFACE_
int wifi_enable(void);
int wifi_disable(void);
int wifi_set_drv_arg(const char * wifi_drv_arg);
int uap_enable(void);
int uap_disable(void);
int bluetooth_enable(void);
int bluetooth_disable(void);
int bluetooth_poweron(void);
int bluetooth_poweroff(void);
int bt_set_drv_arg(const char * bt_drv_arg);
int fm_enable(void);
int fm_disable(void);
int mrvl_sd8xxx_force_poweroff();
int wifi_get_fwstate();
int wifi_get_card_type(char *reply, int reply_size);

/* Force marvell wireless chip power off
  * Return value:
  * 0 : The command has been implemented;
  * 1 : The command is not supported(The feadture is disabled).
  *----------------------------------------------------
  * To enable/disable the feature, please set the property:
  * persist.sys.mrvl_wl_recovery
  * 1: enable; 0: disable.
  */
int wifi_uap_force_poweroff();
int bt_fm_force_poweroff();
int wifi_get_fwstate();
#endif

