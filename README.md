# Programmable Incubator for Protein Expression



[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com) <a href="https://www.repostatus.org/#wip"><img src="https://www.repostatus.org/badges/latest/wip.svg" alt="Project Status: WIP – Initial development is in progress, but there has not yet been a stable, usable release suitable for the public." align="left" /></a> [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT) 

Note [25 Feb 2024]: This is a draft repository - several parts are currently incomplete.

## Summary

The Programmable Incubator for Protein Expression project is developing a control module that can be added to a commercially available incubator that allows programming of temperature profiles over time, currently using a WiFi interface that allows user to connect via a phone or computer without the need for internet access.

The primary use case is controlling temperature during overnight autoinduction of recombinant proteins in _Escherichia coli_.

## Background

<img src="./img/incubator-photo.jpg" width=30% height=30% align="right" >

Open Bioeconomy Lab developed a protocol for expressing recombinant proteins in _E. coli_ on autoinduction agar in petri dishes. This solid state bioreactor approach removes the need for shaking incubators and high-volume centrifuges from the protein production process.

Low-cost peltier controlled incubator (often sold commercially as an egg incubator or a beer fridge) offer an affordable solution for incubation of protein expression cultures at common growth temperatures e.g. 37 C. 

They can also be integrated with DIY orbital shakers to provide a small-footprint, adaptable package for protein expression. However, often protein expression is maximised when temperatures are dropped following induction (e.g. to 18 C or even 11 C), in order to reduce growth and allow for slower folding and maturation of the protein. If the user is inducing the culture then they can manually adjust the temperature but there are contexts in which it would be convenient to have programmable control. These include:

 - Autoinduction, where induction occurs as a result of metabolism or bacterial growth, usually overnight because this is a convenient time to incubate the culture.
 - Where automated induction using heat, light or chemical addition to the culture is possible (this is especially useful when coupled with measurement of cell growth and protein expression using sensors)

It is therefore advantageous to be able to programme temperature profiles over time and leave the incubator to implement these autonomously (or in response to sensor input).

The Programmable Incubator for Protein Expression project is developing a control module that can be added to the commercially available incubator that allows programming of temperature profiles, currently using a WiFi interface that allows user to connect via a phone or computer without the need for internet access.

## Technical Description
The incubator is a Lucky Reptile Herp Nursery II – Incubator (model HN-2) with a temperature range of 5 to 60 degC. The incubator uses a Peltier device to heat or cool running at 12 V. 
The control system is based on an ESP32-S3 board from Adafruit, interfaced to the relay control circuit of the incubator.
The control system running on the ESP32 used a PID algorithm. Temperature monitoring is done using a SHT31 temperature sensor. 
Remote communication with the system is via a web interface, with the ESP32 acting a webserver. 
The system can be programmed with a constant temperature setpoint or schedule of temperature points with dwell times. 

## Incubator wiring
![image](https://github.com/openbioeconomy/programmable-incubator/assets/50485336/2d474b24-3f68-4b22-a6c5-431ea4754904)

## ESP32 pins 
Pin 17 – Heat <br>
Pin 16 – Cool 

## Sensor pins 
Green – SDA <br>
White – SCL <br>
Red - 3.3 V <br>
Black - GND

## Dependencies
Visual Studio
Platform IO

## The Team

 - Richard Hopper
 - Jenny Molloy

This is a collaboration of [Open Bioeconomy Lab](https://openbioeconomy.org) with the [Biomaker Meetup](https://www.meetup.com/makespace/events/pbhdjtygcdbcc/) at [Cambridge Makespace](https://web.makespace.org/)

