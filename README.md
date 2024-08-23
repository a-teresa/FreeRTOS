<h1> Free RTOS Projects </h1>

<h4> Inside <b> workspace/RTOS_workspace</b> there are several projects that consist in exploring some of the basic freeRTOS features, such as: </h4>

<ul> . queues</ul>  
<ul>  . mutex </ul> 
<ul>  . binary semaphores  </ul> 

  <h4>The purpose is to understand how to implement a system that can share data between tasks and improve performance while using multiple peripherals (I2C, SPI, UART, etc.), as well as external interrups (e.g. hardware timer). </h4>
  <p>We usually avoid to share peripherals between tasks, but that reality is also explored.</p>
  

  <p>The project is not completed, and usb functionality will be added in the future (the targets stm32F4 and stm32F7)</p> 

 <p> This work is based on the book: <i><b>Hands on RTOS with Microcontrollers </b></i> (Second Edition), Brian Amos et al., Feb. 2024 </p> 

 <p>The HAL library is used in all the projects.</p>

 <p>Either converting stlink in JTAG (possible in windows) or using the JLINK (macOS, linux), SEGGER Ozone and SEGGER SystemView are valuable tools for a deeper understanding of how to improve performance and avoid unsuspected issues.</p>

 <p>SEGGER Ozone screenshot:</p>

 <img src="https://github.com/a-teresa/FreeRTOS/blob/master/workspace/RTOS_workspace/segger_ozone.png"></img>
 
 <p>SEGGER SystemView screenshot of one of the initial projects:</p>



 <img src="https://github.com/a-teresa/FreeRTOS/blob/master/workspace/RTOS_workspace/SEGGER_SYSTEMVIEW.png"></img>
