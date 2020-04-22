
<h1>STSPIN220 Device</h1>
<ul>
  <li> Generated with <a href="https://github.com/uprev-mrt/mrtutils/wiki/mrt-device">MrT Device Utility</a> </li>
  <li> Bus:  GPIO</li>
  <li> RegMap: <a href="Regmap.html">Register Map</a>
  <li>Datasheet: <a href="https://www.st.com/resource/en/datasheet/stspin220.pdf">https://www.st.com/resou...</a> </li>
  <li> DigiKey: <a href="https://www.digikey.com/products/en?KeyWords=497-16602-1-ND">497-16602-1-ND</a></li>
</ul>
<hr/>
<h2>Description: </h2>
<p>Low voltage stepper motor driver</p>


<!--*user-block-description-start*-->

<p >
  The STSPIN220 is a stepper motor driver which integrates, in a small
  VFQFPN 3 x 3 x 1.0 mm package, both control logic and a low R<sub>DS</sub>
  <sub>(on)</sub> power stage.
</p>

  <p >
    The integrated controller implements PWM current control with fixed OFF
    time and a microstepping resolution up to 1/256<sup>th</sup> of a
    step.<br />
    The device is designed to operate in battery-powered scenarios and can
    be forced into a zero-consumption state, allowing a significant increase
    in battery life.<br />
    The device offers a complete set of protection features including
    overcurrent, overtemperature and short-circuit protection.
  </p>
  
<p >Key Features </p>
    <ul>
      <li data-automation-text="feature-text">
        Operating voltage: from 1.8 to 10 V
      </li>
      <li data-automation-text="feature-text">
        Maximum output current: 1.3 A<sub>rms</sub>
      </li>
      <li data-automation-text="feature-text">
        R<sub>DS(ON)</sub> HS + LS = 0.4 Ω typ.
      </li>
      <li data-automation-text="feature-text">
        Microstepping up to 1/256<sup>th</sup> of a step
      </li>
      <li data-automation-text="feature-text">
        Current control with programmable off-time
      </li>
      <li data-automation-text="feature-text">
        Full protection set
        <ul class="st-list">
          <li>Non-dissipative overcurrent protection</li>
          <li>Short-circuit protection</li>
          <li>Thermal shutdown</li>
        </ul>
      </li>
      <li data-automation-text="feature-text">
        Energy saving and long battery life with standby consumption less
        than 80 nA
      </li>
    </ul>

<br>

Example Code:
```c
  stspin220_t stspn;
  stspn_hw_cfg_t hw;
  hw.mDir = gpio_pa0;
  hw.mStck = gpio_pa1;
  hw.mMode1 = gpio_pa2;
  hw.mMode2 = gpio_pa2;

  stspn_init(&stspn, &hw, 56);                    /* Init with 56 steps per mm*/
  stspn_set_mode(&stspn, SPN_STEP_1_4);           /* use 1/4 stepping */ 
  stspn_move_mm(&stpsn, SPN_DIR_FORWARD, 32.5);   /* Move by mm */
  
  stspn_move(&stpsn, SPN_DIR_FORWARD, 150 );      /* Move by steps */


```

<!--*user-block-description-end*-->
<br/>


<hr class="section">
<h2 class="right"> Register Map</h2>
<hr class="thick">

<table class="fields">
    <tr>
        <th>Name</th>
        <th>Address</th>
        <th>Type</th>
        <th>Access</th>
        <th>Default</th>
        <th>Description</th>
    </tr>

</table>



<hr class="section">
<h2 class="right"> Registers</h2>
<hr class="thick">


