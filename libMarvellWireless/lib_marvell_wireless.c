/*
* All Rights Reserved
*
* MARVELL CONFIDENTIAL
* Copyright 2008 ~ 2010 Marvell International Ltd All Rights Reserved.
* The source code contained or described herein and all documents related to
* the source code ("Material") are owned by Marvell International Ltd or its
* suppliers or licensors. Title to the Material remains with Marvell International Ltd
* or its suppliers and licensors. The Material contains trade secrets and
* proprietary and confidential information of Marvell or its suppliers and
* licensors. The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used, copied,
* reproduced, modified, published, uploaded, posted, transmitted, distributed,
* or disclosed in any way without Marvell's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery
* of the Materials, either expressly, by implication, inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be
* express and approved by Marvell in writing.
* 
*/
#define LOG_TAG "libMarvellWireless"
#include <stdio.h>
#include <utils/Log.h>
#include <stdlib.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "marvell_wireless.h"

#define SOCKERR_IO          -1
#define SOCKERR_CLOSED      -2
#define SOCKERR_INVARG      -3
#define SOCKERR_TIMEOUT     -4
#define SOCKERR_OK          0
#define MAX_BUFFER_SIZE    256
#define MAX_DRV_ARG_SIZE   128


static int wireless_send_command(const char *cmd, char *reply_buffer, int reply_buffer_size);
static int cli_conn (const char *name);

static const char *local_socket_dir = "/data/misc/wireless/socket_daemon";
/* interfaces implements for wifi,uAP,bluetooth, FMradio*/
int wifi_enable(void)
{
    return wireless_send_command("WIFI_ENABLE",0,0);
}
int wifi_disable(void)
{
    return wireless_send_command("WIFI_DISABLE",0,0);
}
int uap_enable(void)
{
    return wireless_send_command("UAP_ENABLE",0,0);
}
int uap_disable(void)
{
    return wireless_send_command("UAP_DISABLE",0,0);
}

int bluetooth_enable(void)
{
    return wireless_send_command("BT_ENABLE",0,0);
}
int bluetooth_disable(void)
{
    return wireless_send_command("BT_DISABLE",0,0);
}

int bluetooth_poweron(void)
{
    return wireless_send_command("BT_ON",0,0);
}

int bluetooth_poweroff(void)
{
    return wireless_send_command("BT_OFF",0,0);
}

int fm_enable(void)
{
    return wireless_send_command("FM_ENABLE",0,0);
}
int fm_disable(void)
{
    return wireless_send_command("FM_DISABLE",0,0);
}
int nfc_enable(void)
{
    return wireless_send_command("NFC_ENABLE",0,0);
}
int nfc_disable(void)
{
    return wireless_send_command("NFC_DISABLE",0,0);
}
int wifi_set_drv_arg(const char * wifi_drv_arg)
{
    char wifi_drvarg[MAX_DRV_ARG_SIZE] = "WIFI_DRV_ARG ";

    if (strlen(wifi_drv_arg) >= (MAX_DRV_ARG_SIZE - strlen(wifi_drvarg))) {
        ALOGE("The wifi driver arg[%s] is too long( >= %d )!", wifi_drv_arg,
                MAX_DRV_ARG_SIZE - strlen(wifi_drvarg));
        return -1;
    }

    return wireless_send_command(strcat(wifi_drvarg, wifi_drv_arg),0,0);
}
int bt_set_drv_arg(const char * bt_drv_arg)
{
    char bt_drvarg[MAX_DRV_ARG_SIZE] = "BT_DRV_ARG ";
    if (strlen(bt_drv_arg) >= (MAX_DRV_ARG_SIZE - strlen(bt_drvarg))) {
        ALOGE("The bt driver arg[%s] is too long( >= %d )!", bt_drv_arg,
                MAX_DRV_ARG_SIZE - strlen(bt_drvarg));
        return -1;
    }

    return wireless_send_command(strcat(bt_drvarg, bt_drv_arg),0,0);
}

int mrvl_sd8xxx_force_poweroff()
{
    return wireless_send_command("MRVL_SD8XXX_FORCE_POWER_OFF",0,0);
}

int wifi_get_fwstate() {
    return wireless_send_command("WIFI_GET_FWSTATE",0,0);
}

int wifi_get_card_type(char *reply, int reply_size)
{
    if( reply == NULL )
        return -1;
    if( wireless_send_command("GET_CARD_TYPE", reply, reply_size) )
        return -1;

    return 0;
}

/*send wireless command:wifi,uAP,Bluetooth,FM enable/disable commands to marvell wireless daemon*/
static int wireless_send_command(const char *cmd, char *reply_buffer, int reply_size)
{
    int conn_fd = -1;
    int n = 0;
    int i = 0;
    int data = 0;
    char buffer[256];
    int len = 0;
    int ret = 1;
    char *pos;

    conn_fd = cli_conn (local_socket_dir);
    if (conn_fd < 0) {
        ALOGE("cli_conn error.\n");
        ret = 1;
        goto out1;
    }
    len = strlen(cmd);
    strncpy(buffer, cmd, len);
    buffer[len++] = '\0';
    n = write(conn_fd, buffer, len);

    if (n == SOCKERR_IO) 
    {
        ALOGE("write error on fd %d\n", conn_fd);
        ret = 1;
        goto out;
    }
    else if (n == SOCKERR_CLOSED) 
    {
        ALOGE("fd %d has been closed.\n", conn_fd);
        ret = 1;
        goto out;
    }
    else 
        ALOGI("Wrote %s to server. \n", buffer);

    n = read(conn_fd, buffer, sizeof (buffer));
    if (n == SOCKERR_IO) 
    {
        ALOGE("read error on fd %d\n", conn_fd);
        ret = 1;
        goto out;
    }
    else if (n == SOCKERR_CLOSED) 
    {
        ALOGE("fd %d has been closed.\n", conn_fd);
        ret = 1;
        goto out;
    }
    else 
    {
        if(strncmp(buffer,"0,OK", strlen("0,OK")))
            ret = 1;
        else
        {
            if( reply_buffer )
            {
                pos = strchr(buffer, ' ');
                if( !pos )
                {
                    ret = -1;
                }
                else
                {
                    len = strlen(pos+1);
                    if( reply_size > len )
                    {
                        strncpy(reply_buffer, pos+1, len);
                        ret = 0;
                    }
                    else
                    {
                        ALOGE("reply length exceeds provided buffer size");
                    }
                }
            }
        }
    }
out:
    close(conn_fd);
out1:
    return ret; 
}

/* returns fd if all OK, -1 on error */
static int cli_conn (const char *name)
{
    int                fd, len;
    int ret = 0;
    struct sockaddr_un unix_addr;
    int value = 0x1;
    /* create a Unix domain stream socket */
    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        ALOGE("create socket failed, ret:%d, strerror:%s", fd, strerror(errno));
        return(-1);
    }

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&value, sizeof(value));
    /* fill socket address structure w/server's addr */
    memset(&unix_addr, 0, sizeof(unix_addr));
    unix_addr.sun_family = AF_UNIX;
    strcpy(unix_addr.sun_path, name);
    len = sizeof(unix_addr.sun_family) + strlen(unix_addr.sun_path);

    if ((ret = connect (fd, (struct sockaddr *) &unix_addr, len)) < 0)
    {
        ALOGE("connect failed, ret:%d, strerror:%s", ret, strerror(errno));
        goto error;
    }
    return (fd);

error:
    close (fd);
    return ret;
}

