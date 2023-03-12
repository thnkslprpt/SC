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
 *   This file contains the utilty functions for Stored Command
 */

/**************************************************************************
 **
 ** Include section
 **
 **************************************************************************/

#include "cfe.h"
#include "sc_utils.h"
#include "sc_events.h"
#include "sc_msgids.h"
#include <string.h>

/**************************************************************************
 **
 ** Functions
 **
 **************************************************************************/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Get the Current time from CFE TIME                              */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void SC_GetCurrentTime(void)
{
    CFE_TIME_SysTime_t TempTime;

/* Use SC defined time */
#if (SC_TIME_TO_USE == SC_USE_UTC)
    TempTime = CFE_TIME_GetUTC();
#elif (SC_TIME_TO_USE == SC_USE_TAI)
    TempTime = CFE_TIME_GetTAI();
#else
    /* Use cFE configured time */
    TempTime = CFE_TIME_GetTime();
#endif

    /* We don't care about subseconds */
    SC_AppData.CurrentTime = TempTime.Seconds;
}

SC_AbsTimeTag_t SC_GetAtsEntryTime(SC_AtsEntryHeader_t *Entry)
{
    /*
    ** ATS Entry Header looks like this...
    **
    **    uint16 Pad;
    **    uint16 CmdNumber;
    **
    **    uint16 TimeTag_MS;
    **    uint16 TimeTag_LS;
    **
    **    CFE_SB_Buffer_t Buffer;
    **
    ** The command packet data is variable length,
    **    only the command packet header is shown here.
    */

    return ((Entry->TimeTag_MS << 16) + Entry->TimeTag_LS);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Compute Absolute time from relative time                       */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
SC_AbsTimeTag_t SC_ComputeAbsTime(SC_RelTimeTag_t RelTime)
{
    CFE_TIME_SysTime_t AbsoluteTimeWSubs;
    CFE_TIME_SysTime_t RelTimeWSubs;
    CFE_TIME_SysTime_t ResultTimeWSubs;
    /*
     ** get the current time
     */
    AbsoluteTimeWSubs.Seconds    = SC_AppData.CurrentTime;
    AbsoluteTimeWSubs.Subseconds = 0;

    RelTimeWSubs.Seconds    = RelTime;
    RelTimeWSubs.Subseconds = 0;
    /*
     ** add the relative time the current time
     */
    ResultTimeWSubs = CFE_TIME_Add(AbsoluteTimeWSubs, RelTimeWSubs);

    /* We don't need subseconds */
    return (ResultTimeWSubs.Seconds);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/*  Compare absolute times                                         */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool SC_CompareAbsTime(SC_AbsTimeTag_t AbsTime1, SC_AbsTimeTag_t AbsTime2)
{
    bool               Status;
    CFE_TIME_SysTime_t Time1WSubs;
    CFE_TIME_SysTime_t Time2WSubs;
    CFE_TIME_Compare_t Result;

    Time1WSubs.Seconds    = AbsTime1;
    Time1WSubs.Subseconds = 0;

    Time2WSubs.Seconds    = AbsTime2;
    Time2WSubs.Subseconds = 0;

    Result = CFE_TIME_Compare(Time1WSubs, Time2WSubs);

    if (Result == CFE_TIME_A_GT_B)
    {
        Status = true;
    }
    else
    {
        Status = false;
    }

    return Status;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* SC Verify the length of the command                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool SC_VerifyCmdLength(const CFE_MSG_Message_t *Msg, size_t ExpectedLength)
{
    CFE_SB_MsgId_t    MessageID    = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t CommandCode  = 0;
    bool              Result       = true;
    size_t            ActualLength = 0;

    CFE_MSG_GetSize(Msg, &ActualLength);

    /* Verify the command packet length */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(Msg, &MessageID);
        CFE_MSG_GetFcnCode(Msg, &CommandCode);

        CFE_EVS_SendEvent(SC_CMD_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid msg length: ID = 0x%08lX, CC = %d, Len = %d, Expected = %d",
                          (unsigned long)CFE_SB_MsgIdToValue(MessageID), CommandCode, (int)ActualLength,
                          (int)ExpectedLength);
        Result = false;
        if (CFE_SB_MsgIdToValue(MessageID) == SC_CMD_MID)
        {
            SC_OperData.HkPacket.CmdErrCtr++;
        }
    }
    return (Result);
}

uint16 SC_ToggleAtsIndex(void)
{
    uint16 CurrAtsIndex = SC_ATS_NUM_TO_INDEX(SC_OperData.AtsCtrlBlckAddr->AtsNumber);

    return (1 - CurrAtsIndex);
}
