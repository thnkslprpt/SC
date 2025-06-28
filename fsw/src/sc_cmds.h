/************************************************************************
 * NASA Docket No. GSC-18,924-1, and identified as “Core Flight
 * System (cFS) Stored Command Application version 3.1.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *   This file contains functions to handle processing an RTS or ATS
 *   command as well as the generic ground commands
 */
#ifndef SC_CMDS_H
#define SC_CMDS_H

#include "common_types.h"
#include "sc_msg.h"


/**
 * \brief Sends out an Event message
 *
 *  \par Description
 *       Command for testing aliveness of SC
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 *
 *  \param[in] Cmd Pointer to Software Bus buffer
 *
 *  \sa #SC_NOOP_CC
 */
void SC_NoopCmd(const SC_NoopCmd_t *Cmd);

/**
 * \brief Reset Counters Command
 *
 *  \par Description
 *       Clears the command counters and error counters for SC
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 *
 *  \param[in] Cmd Pointer to Software Bus buffer
 *
 *  \sa #SC_RESET_COUNTERS_CC
 */
void SC_ResetCountersCmd(const SC_ResetCountersCmd_t *Cmd);

/**
 * \brief Send Hk Packet to the ground
 *
 *  \par Description
 *       This routine collects the housekeeping status information,
 *       formats the packet and sends the packet over the software bus
 *       to health and safety.
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 */
void SC_SendHkPacket(void);

void SC_SendHkCmd(const SC_SendHkCmd_t *Cmd);
void SC_WakeupCmd(const SC_WakeupCmd_t *Cmd);

/**
 * \brief Process an ATS Command
 *
 *  \par Description
 *       SC_ProcessAtpCmd takes ONE command from the current
 *       ATS buffer and executes it. It then figures out when the
 *       next command needs to execute and it returns. If for some
 *       reason the next ATS command cannot be found, then the
 *       ATS is stopped. If the command happens to be a Switch ATS command
 *       the command is executed locally instead of sending it out on the
 *       Software Bus.
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 */
void SC_ProcessAtpCmd(void);

/**
 * \brief Process an RTS Command
 *
 *  \par Description
 *        This routine processes ONE command from ONE active Relative
 *        Time Sequence. The command that has to be executed is already
 *        set up in the RTPs control block, It simply has to fetch the
 *        command, execute it, and get the next command.
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 */
void SC_ProcessRtpCommand(void);

#endif
