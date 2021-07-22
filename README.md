# RV8803Tiny
A RV8803 RTC i2c based interafcing library for the new 0-series and 1-series ATtiny and ATmega based uCs. 

## Inspiration:
Tested in ATTINY1607 running on [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore) compiled in Arduino IDE 1.8.13.
A lot of inspiration and methods were borrowed from [Sparkfun's RV8803 Libray](https://github.com/sparkfun/SparkFun_RV-8803_Arduino_Library).

## Background:
I was working on a project where I needed to use a RTC. I needed to use very low power uC (So I stumbled upon the new ATTINY series uCs) in a very small physical footprint. Usual RTC clocks are rather big for my purpose and current consumption metrices, so I stumbled upon [RV-8803 RTC](https://www.microcrystal.com/fileadmin/Media/Products/RTC/App.Manual/RV-8803-C7_App-Manual.pdf), from microcrystal. Upon looking further on the internet, above mentioned, sparkfun library came up in focus. But the library uses `wire` library under-neath for it's i2C communication for reading and writing, to & from the RTC. The library is okay to be used in typical arduino projects but it is very memory heavy, bulky and not so efficient for my purpos. 

So I set to rewrite some of the useful methods from the sparkfun library, replacing `wire` library parts with custom, more efficient for new ATTINY's I2C needs, the [TinyMegaI2C](https://github.com/dattasaurabh82/TinyMegaI2C).

_**NOTE**_: 
1. I have not implemented all the methods as I do not need them and they will save me memory and thus current.
But feel free to extend on it. I will definitely extend the funstions as per needed, in next releases. 🤗
2. I have tested this library for ATTINY1607, compiled using Arduino IDE 1.8.3. But should work for others in the same series. 😕
3. Do not forget the PULLUP Resistors on the I2C Bus. They are there for a [reason](https://github.com/SpenceKonde/megaTinyCore/discussions/465) 🤓


## Installation: 
Prerequisite: [TinyMegaI2C](https://github.com/dattasaurabh82/TinyMegaI2C)

Then it is Standard Arduino Library Installation method. 
<p align="center">
   <img width="920" src="https://raw.githubusercontent.com/dattasaurabh82/RV8803Tiny/main/assets/install.svg">
</p>

Take a look in the examples to get the ideas about the methods implemented. 

## HW Connection: 
<p align="center">
   <img width="920" src="https://user-images.githubusercontent.com/4619862/126473329-91724017-c11b-4ed3-bd86-b67e2438738a.png">
</p>

## ToDo: 
- [ ] Set Hundredths
- [ ] Timestamp
- [ ] Alarm interrupts
- [ ] Countdown interrupts
- [ ] Periodic Interrupt
- [ ] Fine Tuning RTC. 


