/**

\
  * @file stspin220.h
  * @author Jason Berger
  * @link https://www.st.com/resource/en/datasheet/stspin220.pdf
  * @brief Device driver for STSPIN220 device
  *
  */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Platforms/Common/mrt_platform.h"


/*******************************************************************************
  Sruct                                                                                
*******************************************************************************/
typedef enum{
  SPN_STEP_FULL   = 0b0000,
  SPN_STEP_1_2    = 0b1010,
  SPN_STEP_1_4    = 0b0101,
  SPN_STEP_1_8    = 0b1011,
  SPN_STEP_1_32   = 0b0001,
  SPN_STEP_1_64   = 0b1101,
  SPN_STEP_1_128  = 0b0010,
  SPN_STEP_1_256  = 0b0011
}stspn_step_mode_e;

typedef struct{
    mrt_gpio_t mDir;                    /* Direction      */
    mrt_gpio_t mStck;                   /* Step clock     */
    mrt_gpio_t mStandby;                /* Standby/Reset  */
    mrt_gpio_t mMode1;                  /* Standby/Reset  */
    mrt_gpio_t mMode2;                  /* Standby/Reset  */
    mrt_gpio_t mFault;                  /* Fault pin input/enable output */
}stspn_hw_cfg_t;


typedef struct{
    stspn_hw_cfg_t mHw;                 /* Hardware config*/
    uint32_t mStepsPerMM;               /* FULL Steps Per MM of actuator*/
    stspn_step_mode_e mMicroStepMode;   /* Micro Step setting */
    uint32_t mTicksPerStep;             /* We store this to needing to look it up for each movement*/
    uint32_t mPos;                      /* Current position in steps. when microstepping, each microstep is a step*/
    uint32_t mMaxPos;                   /* Max position limit*/
    bool mHold;                         /* Hold/Brake after each movement*/
    bool mReverse;                      /* Reverse Direction */
}stspin220_t;


/**
  *@brief Initialize spn220 device
  *@param dev ptr to STSPIN220 device
  *@param hw ptr to STSPIN220 hardware config struct device
  *@return MRT_STATUS_OK if succesful 
  *@return MRT_STATUS_ERROR if there is a problem
  */
mrt_status_t stspn_init(stspin220_t* dev, stspn_hw_cfg_t* hw );

/**
 * @brief set mechanical parameters 
 * 
 * @param dev ptr to device 
 * @param currPos current position (if not 0)
 * @param maxPos max position in microsteps/steps 
 * @param stepsPerMm Full steps per mm
 * @return mrt_status_t 
 */
mrt_status_t stspn_set_params(stspin220_t* dev, uint32_t currPos, uint32_t maxPos, uint32_t stepsPerMm);

/**
 * @brief set mechanical parameters in mm
 *
 * @param dev ptr to device
 * @param currPos current position in mm
 * @param maxPos max position in microsteps/steps
 * @param stepsPerMm Full steps per mm
 * @return mrt_status_t
 */
mrt_status_t stspn_set_params_in_mm(stspin220_t* dev, double currPosMm, double maxPos, uint32_t stepsPerMm);

/**
 * @brief Set microstepping mode
 * @param dev ptr to stpn device
 * @param mode micro stepping mode
 * @return mrt_status_t 
 */
mrt_status_t stspn_set_mode(stspin220_t* dev, stspn_step_mode_e mode);

/**
 * @brief Move motor n steps 
 * @param dev ptr to stspn device
 * @param ticks number of ticks to move +/- for direction
 * @return number of steps moved 
 */
int stspn_move(stspin220_t* dev,  int ticks);

/**
 * @brief Move motor n millimeters  
 * @param dev ptr to stspn device
 * @param mm number of mm to move +/- for direction
 * @return number of steps moved 
 */
int stspn_move_mm(stspin220_t* dev,  double mm);

/**
 * @brief Move motor to position by mm 
 * @param dev ptr to stspn device
 * @param mm number of mm to move +/- for direction
 * @return number of steps moved 
 */
int stspn_goto_mm(stspin220_t* dev,  double mm);

/**
 * @brief Move motor to position by mm.  Wake the device, set the step mode, and enable.
 *        When movment is finished, disable the device and set to standby.
 * @param dev ptr to stspn device
 * @param mm number of mm to move +/- for direction
 * @return number of steps moved
 */
int stspn_wake_goto_mm_sleep(stspin220_t* dev,  double mm);



#ifdef __cplusplus
}
#endif
