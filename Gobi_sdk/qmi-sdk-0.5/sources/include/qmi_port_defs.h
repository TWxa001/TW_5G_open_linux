/******************************************************************************
  @file    qmi_port_defs.h
  @brief   Common QMI connection ID definitions

  DESCRIPTION
  This file contains common QMI connection ID header file definitions.

  INITIALIZATION AND SEQUENCING REQUIREMENTS
  N/A

  ---------------------------------------------------------------------------
  Copyright (c) 2012 QUALCOMM Technologies Incorporated.
  All Rights Reserved. QUALCOMM Proprietary and Confidential.
  ---------------------------------------------------------------------------
******************************************************************************/

#ifndef QMI_PORT_DEFS_H
#define QMI_PORT_DEFS_H

/* QMI port identifiers,  Do not change the first valid port enum value... must start at
** 0 and increase sequentially.  Note that this is needed in qmi_platform.h, thus we
** will define it here, before including qmi_platform.h
** Also note that if non-virtual connection ID's are added, then the QMI_MAX_CONNECTIONS #define
** in qmi.h must be adjusted accordingly
*/
typedef enum
{
  QMI_CONN_ID_INVALID = -1,
  QMI_CONN_ID_FIRST = 0,
  QMI_CONN_ID_RMNET_0 = QMI_CONN_ID_FIRST, /* Corresponds to SMD DATA_5 data port */
  QMI_CONN_ID_RMNET_1,                 /* Corresponds to SMD DATA_6 data port */
  QMI_CONN_ID_RMNET_2,                 /* Corresponds to SMD DATA_7 data port */
  QMI_CONN_ID_RMNET_3,
  QMI_CONN_ID_RMNET_4,
  QMI_CONN_ID_RMNET_5,
  QMI_CONN_ID_RMNET_6,
  QMI_CONN_ID_RMNET_7,
  QMI_CONN_ID_RMNET_8,                 /* Corresponds to Rmnet tethered port */
  QMI_CONN_ID_RMNET_MDM_0,
  QMI_CONN_ID_RMNET_MDM_1,
  QMI_CONN_ID_RMNET_MDM_2,
  QMI_CONN_ID_RMNET_MDM_3,
  QMI_CONN_ID_RMNET_MDM_4,
  QMI_CONN_ID_RMNET_MDM_5,
  QMI_CONN_ID_RMNET_MDM_6,
  QMI_CONN_ID_RMNET_MDM_7,
  QMI_CONN_ID_RMNET_SDIO_0 = QMI_CONN_ID_RMNET_MDM_0,
  QMI_CONN_ID_RMNET_SDIO_1 = QMI_CONN_ID_RMNET_MDM_1,
  QMI_CONN_ID_RMNET_SDIO_2 = QMI_CONN_ID_RMNET_MDM_2,
  QMI_CONN_ID_RMNET_SDIO_3 = QMI_CONN_ID_RMNET_MDM_3,
  QMI_CONN_ID_RMNET_SDIO_4 = QMI_CONN_ID_RMNET_MDM_4,
  QMI_CONN_ID_RMNET_SDIO_5 = QMI_CONN_ID_RMNET_MDM_5,
  QMI_CONN_ID_RMNET_SDIO_6 = QMI_CONN_ID_RMNET_MDM_6,
  QMI_CONN_ID_RMNET_SDIO_7 = QMI_CONN_ID_RMNET_MDM_7,
  QMI_CONN_ID_RMNET_USB_0 = QMI_CONN_ID_RMNET_MDM_0,
  QMI_CONN_ID_RMNET_USB_1 = QMI_CONN_ID_RMNET_MDM_1,
  QMI_CONN_ID_RMNET_USB_2 = QMI_CONN_ID_RMNET_MDM_2,
  QMI_CONN_ID_RMNET_USB_3 = QMI_CONN_ID_RMNET_MDM_3,
  QMI_CONN_ID_RMNET_USB_4 = QMI_CONN_ID_RMNET_MDM_4,
  QMI_CONN_ID_RMNET_USB_5 = QMI_CONN_ID_RMNET_MDM_5,
  QMI_CONN_ID_RMNET_USB_6 = QMI_CONN_ID_RMNET_MDM_6,
  QMI_CONN_ID_RMNET_USB_7 = QMI_CONN_ID_RMNET_MDM_7,

  QMI_CONN_ID_RMNET_SMUX_0 = QMI_CONN_ID_RMNET_MDM_0,

  /* Identifier used to discern non-broadcast ports from broadcast ports */
  QMI_CONN_ID_MAX_NON_BCAST = QMI_CONN_ID_RMNET_MDM_7,

  QMI_CONN_ID_RMNET_12,                 /* Corresponds to SMD DATA_8 data port */
  QMI_CONN_ID_RMNET_13,                 /* Corresponds to SMD DATA_9 data port */

  QMI_CONN_ID_PROXY,                    /* Corresponds to QMI Proxy virtual port */

  QMI_MAX_CONN_IDS

} qmi_connection_id_type;

#endif
