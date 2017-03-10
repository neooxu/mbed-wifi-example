/**
 ******************************************************************************
 * @file    iperf_cli.h
 * @author  Libo
 * @version V1.0.0
 * @date    5-Jan-2018
 * @brief   This file provide the iperf command line functions.
 ******************************************************************************
 *
 *  UNPUBLISHED PROPRIETARY SOURCE CODE
 *  Copyright (c) 2016 MXCHIP Inc.
 *
 *  The contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of MXCHIP Corporation.
 ******************************************************************************
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *               Function Declarations
 ******************************************************/

/**
  * @brief  Add iperf command lines to MiCO CLI.
  * @param  none.
  * @retval kNoErr is returned on success, otherwise, kXXXErr is returned.
  */
OSStatus iperf_cli_register( void );

#ifdef __cplusplus
} /*extern "C" */
#endif
