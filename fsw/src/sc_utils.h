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
#ifndef SC_UTILS_H
#define SC_UTILS_H

#include "cfe.h"
#include "sc_app.h"

/**
 * \brief Gets the current time from CFE
 *
 *  \par Description
 *       Queries the CFE TIME services and retieves the Current time
 *
 *  \par Assumptions, External Events, and Notes:
 *        This routine stores the time in #SC_AppData
 */
void SC_GetCurrentTime(void);

/**
 * \brief Gets the absolute time from an ATS entry
 *
 *  \par Description
 *       This function returns the absolute time tag contained within
 *       the ATS entry passed into the function
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 *
 *  \param [in]        Entry         Pointer to ATS entry
 *
 *  \return The absolute time tag
 */
SC_AbsTimeTag_t SC_GetAtsEntryTime(SC_AtsEntryHeader_t *Entry);

/**
 * \brief Computes an absolute time from relative time
 *
 *  \par Description
 *       This function computes an absolute time from 'now' and the
 *       relative time passed into the function
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 *
 *  \param [in]        RelTime         The relative time to compute from
 *
 *  \return The absolute time tag
 */
SC_AbsTimeTag_t SC_ComputeAbsTime(SC_RelTimeTag_t RelTime);

/**
 * \brief Compares absolute time
 *
 *  \par Description
 *
 *       This function compares two absolutes time.
 *
 *  \par Assumptions, External Events, and Notes:
 *        None
 *
 *  \param [in]    AbsTime1            The first time to compare
 *  \param [in]    AbsTime2            The second time to compare
 *
 *  \return Boolean comparison result
 *  \retval true    AbsTime1 is greater than AbsTime2
 *  \retval false   AbsTime1 is less than AbsTime2
 */
bool SC_CompareAbsTime(SC_AbsTimeTag_t AbsTime1, SC_AbsTimeTag_t AbsTime2);

/**
 * \brief Verify command message length
 *
 *  \par Description
 *       This routine will check if the actual length of a software bus
 *       command message matches the expected length and send an
 *       error event message if a mismatch occurs
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \param [in]   Msg              Pointer to message
 *  \param [in]   ExpectedLength   The expected length of the message
 *                                 based upon the command code
 *
 *  \return Boolean length verification result
 *  \retval true  Length matches expected
 *  \retval false Length does not match expected
 *
 *  \sa #SC_CMD_LEN_ERR_EID
 */
bool SC_VerifyCmdLength(const CFE_MSG_Message_t *Msg, size_t ExpectedLength);

/**
 * \brief Toggles the ATS index
 *
 *  \par Description
 *       This function toggles the ATS index between 0 and 1.  This
 *       function does not modify global data, but rather returns a
 *       new ATS index that can be used and saved by the calling
 *       function.
 *
 *  \par Assumptions, External Events, and Notes:
 *       None
 *
 *  \return Toggled ATS index
 *  \retval 0 When current ATS index is 1
 *  \retval 1 When current ATS index is 0
 *
 *  \sa #SC_CMD_LEN_ERR_EID
 */
uint16 SC_ToggleAtsIndex(void);

#endif
