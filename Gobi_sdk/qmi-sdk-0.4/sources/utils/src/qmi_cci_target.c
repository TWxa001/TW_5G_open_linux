/******************************************************************************
  @file    qmi_cci_target.c
  @brief   The QMI common client interface target specific module

  ---------------------------------------------------------------------------
  Copyright (c) 2012 QUALCOMM Technologies Incorporated. All Rights Reserved.
  QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
*******************************************************************************/
#include <string.h>
#include "qmi_client.h"
#include "qmi_cci_target.h"
#include "qmi_cci_common.h"

#ifdef QMI_FW_ANDROID
#include <cutils/properties.h>
#include <private/android_filesystem_config.h>

#define QMI_FW_PROPERTY_BASEBAND "ro.baseband"
#define QMI_FW_PROPERTY_BASEBAND_SIZE 10
#define QMI_FW_BASEBAND_VALUE_APQ "apq"
#define QMI_FW_BASEBAND_VALUE_UNDEFINED "undefined"
#endif

#ifdef QCCI_OVER_QMUX
#include "qmi_client_instance_defs.h"
extern qmi_cci_xport_ops_type qmuxd_ops;
#endif

static unsigned int use_qmuxd = 0;

#ifdef QCCI_OVER_QMUX
/*===========================================================================
  FUNCTION  qmi_cci_xport_register
===========================================================================*/
qmi_client_error_type qmi_cci_qmux_xport_register
(
 qmi_client_qmux_instance_type instance
)
{
  if ((instance < QMI_CLIENT_QMUX_BASE_INSTANCE) ||
      (instance > QMI_CLIENT_QMUX_MAX_INSTANCE_IDS))
    return QMI_INTERNAL_ERR;

  qmi_cci_xport_start(&qmuxd_ops, (void *)instance);
  return QMI_NO_ERR;
}

/*===========================================================================
  FUNCTION  qmi_cci_xport_unregister
===========================================================================*/
qmi_client_error_type qmi_cci_qmux_xport_unregister
(
 qmi_client_qmux_instance_type instance
)
{
  if ((instance < QMI_CLIENT_QMUX_BASE_INSTANCE) ||
      (instance > QMI_CLIENT_QMUX_MAX_INSTANCE_IDS))
    return QMI_INTERNAL_ERR;

  qmi_cci_xport_stop(&qmuxd_ops, (void *)instance);
  return QMI_NO_ERR;
}
#endif

/*===========================================================================
FUNCTION: qmi_cci_init

DESCRIPTION:
   Initialize the QCCI library.  This function is called when the QCCI
   shared library is loaded, before application's main() is started.

DEPENDENCIES:
   None

ARGUMENTS:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
===========================================================================*/
#ifdef __GNUC__
void __attribute__ ((constructor)) qmi_cci_init(void)
{
  smem_log_init();
#ifdef QCCI_OVER_QMUX
#ifdef QMI_FW_ANDROID
  int ret = 0;
  char args[QMI_FW_PROPERTY_BASEBAND_SIZE];
  char def[QMI_FW_PROPERTY_BASEBAND_SIZE];

  (void)strlcpy(def, QMI_FW_BASEBAND_VALUE_UNDEFINED,
                QMI_FW_PROPERTY_BASEBAND_SIZE);
  memset(args, 0, sizeof(args));
  ret = property_get(QMI_FW_PROPERTY_BASEBAND, args, def);
  if ((ret > 0) && (ret <= QMI_FW_PROPERTY_BASEBAND_SIZE))
  {
    /*In non APQ targets, use QMUXD*/
    if(strncmp(args, QMI_FW_BASEBAND_VALUE_APQ,
               sizeof(QMI_FW_BASEBAND_VALUE_APQ)))
    {
      use_qmuxd = 1;
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_0);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_1);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_2);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_3);
    }
  }
#else
  use_qmuxd = 1;
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_0);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_1);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_2);
      qmi_cci_xport_start(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_3);
#endif
#endif
}
#endif

/*===========================================================================
FUNCTION: qmi_cci_exit

DESCRIPTION:
   Cleans up the QCCI library.  This function is called after exit() or
   after application's main() completes.

DEPENDENCIES:
   None

ARGUMENTS:
   None

RETURN VALUE:
   None

SIDE EFFECTS:
===========================================================================*/
#ifdef __GNUC__
void __attribute__ ((destructor)) qmi_cci_exit(void)
{
#ifdef QCCI_OVER_QMUX
  if (use_qmuxd)
  {
    qmi_cci_xport_stop(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_0);
    qmi_cci_xport_stop(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_1);
    qmi_cci_xport_stop(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_2);
    qmi_cci_xport_stop(&qmuxd_ops, QMI_CLIENT_QMUX_RMNET_INSTANCE_3);
  }
#endif
  smem_log_exit();
}
#endif
