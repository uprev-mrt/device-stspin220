/**
  * @file stspin220.c
  * @author Jason Berger
  * @link https://www.st.com/resource/en/datasheet/stspin220.pdf
  * @brief Device driver for STSPIN220 device
  *
  */

#include "stspin220.h"


mrt_status_t stspn_init(stspin220_t* dev, stspn_hw_cfg_t* hw )
{

  /* Copy over hardware config*/
  memcpy(&dev->mHw, hw, sizeof(stspn_hw_cfg_t));

  /* Set default values */
  dev->mStepsPerMM = 0;
  dev->mMicroStepMode = SPN_STEP_FULL;
  dev->mTicksPerStep = 1;
  dev->mPos = 0;
  dev->mMaxPos = 0xFFFFFFFF;
  dev->mHold = false;
  dev->mReverse = false;


  MRT_GPIO_WRITE(dev->mHw.mMode2, MRT_LOW);
  MRT_GPIO_WRITE(dev->mHw.mMode2, MRT_LOW);
  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_LOW);


  return MRT_STATUS_OK;
}

mrt_status_t stspn_set_params(stspin220_t* dev, uint32_t currPos, uint32_t maxPos, uint32_t stepsPerMm)
{
  dev->mPos = currPos;
  dev->mMaxPos = maxPos;
  dev->mStepsPerMM = stepsPerMm;

  return MRT_STATUS_OK;
}


mrt_status_t stspn_set_mode(stspin220_t* dev, stspn_step_mode_e mode)
{
	dev->mMicroStepMode = mode;
	switch(mode)
	{
		case SPN_STEP_FULL:
			dev->mTicksPerStep = 1;
			break;
		case SPN_STEP_1_2:
			dev->mTicksPerStep = 2;
			break;
		case SPN_STEP_1_4:
			dev->mTicksPerStep = 3;
			break;
		case SPN_STEP_1_8:
			dev->mTicksPerStep = 4;
			break;
		case SPN_STEP_1_32:
			dev->mTicksPerStep = 32;
			break;
		case SPN_STEP_1_64:
			dev->mTicksPerStep = 64;
			break;
		case SPN_STEP_1_128:
			dev->mTicksPerStep = 128;
			break;
		case SPN_STEP_1_256:
			dev->mTicksPerStep = 256;
			break;
	}

  /* Set mode pins*/
  /*
   * https://www.st.com/resource/en/datasheet/stspin220.pdf
   * table on page 9
   */
  MRT_GPIO_WRITE( dev->mHw.mMode2, MRT_BIT(mode, 0 ));
  MRT_GPIO_WRITE( dev->mHw.mMode1, MRT_BIT(mode, 1 ));
  MRT_GPIO_WRITE( dev->mHw.mDir,  MRT_BIT(mode, 2 ));
  MRT_GPIO_WRITE( dev->mHw.mStck, MRT_BIT(mode, 3 ));
  /* Pulse Standby low*/
  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_LOW);
  MRT_DELAY_MS(1);
  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_HIGH);

  return MRT_STATUS_OK;
}


int stspn_move(stspin220_t* dev,  int ticks)
{
  int dir = 1;
  int tempPosition = dev->mPos;

  if(dev->mReverse)
  {
    ticks = -1 * ticks;
  }

  /* Reduce steps to stay in bounds */
  if((int32_t)(dev->mPos + ticks) > (int32_t)(dev->mMaxPos) )
  {
    ticks = dev->mMaxPos - dev->mPos;
  }
  else if((dev->mPos + ticks) < 0)
  {
    ticks = -1 * dev->mPos;
  }

  /* Set direction */
  if(ticks > 0)
  {
    MRT_GPIO_WRITE(dev->mHw.mDir, MRT_HIGH);
  }
  else 
  {
    MRT_GPIO_WRITE(dev->mHw.mDir, MRT_LOW);
    dir = -1;
  }


  ticks = MRT_ABS(ticks);
  for(int i=0 ; i < ticks; i++)
  {

    /* Pulse Clk 1 uS for each step. Use 1 ms on platforms that dont support uS delay*/
    MRT_GPIO_WRITE(dev->mHw.mStck, MRT_HIGH);
    #ifdef MRT_DELAY_US
        MRT_DELAY_US(1);
    #else 
        MRT_DELAY_MS(0);
    #endif
    MRT_GPIO_WRITE(dev->mHw.mStck, MRT_LOW);
    MRT_DELAY_MS(0);

    // Keep track of current position
	tempPosition += dir;
	if (tempPosition < 0)
	{
		tempPosition = 0;
	}

    //Check fault for over current
    if(MRT_GPIO_READ(dev->mHw.mFault) == MRT_LOW)
    {
        // Keep track of current position
   		dev->mPos = tempPosition;
   		return i * dir;
    }
  }
  
	// Keep track of current position
	dev->mPos = tempPosition;
	return ticks * dir;
}


int stspn_move_mm(stspin220_t* dev,  double mm)
{
  int ticks = dev->mStepsPerMM * mm * dev->mTicksPerStep;

  return stspn_move(dev, ticks);
}

int stspn_goto_mm(stspin220_t* dev,  double mm)
{
  double currPosMm = (dev->mPos * dev->mTicksPerStep) / dev->mStepsPerMM; //get current position in mm 
  
  return stspn_move_mm(dev, (mm - currPosMm));
}

int stspn_wake_goto_mm_sleep(stspin220_t* dev,  double mm)
{
	int returnValue;
	/* Set mode pins*/
	/*
	 * https://www.st.com/resource/en/datasheet/stspin220.pdf
	 * table on page 9
	*/
	  MRT_GPIO_WRITE( dev->mHw.mMode2, MRT_BIT(dev->mMicroStepMode, 0 ));
	  MRT_GPIO_WRITE( dev->mHw.mMode1, MRT_BIT(dev->mMicroStepMode, 1 ));
	  MRT_GPIO_WRITE( dev->mHw.mDir,  MRT_BIT(dev->mMicroStepMode, 2 ));
	  MRT_GPIO_WRITE( dev->mHw.mStck, MRT_BIT(dev->mMicroStepMode, 3 ));
	  /* Pulse Standby low*/
	  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_LOW);
	  MRT_DELAY_MS(1);
	  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_HIGH);
	  // Enable the motor (set the Fault/Enable pin high)
	  MRT_GPIO_WRITE(dev->mHw.mFault, MRT_HIGH);

	  stspn_goto_mm(dev, mm);

	  // Disable motor and enter standby mode
	  MRT_GPIO_WRITE(dev->mHw.mFault, MRT_LOW);
	  MRT_GPIO_WRITE(dev->mHw.mStandby, MRT_LOW);

}
