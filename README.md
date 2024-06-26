# Netpresso: Your Smart Coffee Solution
Netpresso simplifies coffee brewing by integrating the ESP8266 microcontroller with a web interface. 
With Netpresso, users can remotely control their coffee machines via a smartphone or computer. 
It offers intuitive network configuration so a local smart home hub can easily interact with the machine.

# Disclaimer
This project, Netpresso, is provided for educational and informational purposes only. While every effort has been made to ensure the accuracy and safety of the information provided, the creators of Netpresso make no representations or warranties of any kind, express or implied, about the completeness, accuracy, reliability, suitability, or availability of the project for any purpose.

By undertaking this project, you acknowledge that you do so at your own risk. The creators of Netpresso shall not be liable for any loss, damage, injury, or harm, including but not limited to damage to property or personal injury, that may result from the construction, modification, or use of the project.

It is your responsibility to ensure that you have the necessary knowledge, skills, and resources to safely undertake this project. Before attempting any modifications to electrical devices or appliances, consult with a qualified professional and adhere to all relevant safety guidelines and regulations.


# Preparations
## Components
| Part       | Quantity | Description    | 
|------------|----------|----------------|
| Transistor | 3 | 2N2222 (NPN)          |
| Resistor   | 3 | 43 kΩ                 |
| D1 Mini    | 1 | An ESP8266 board      | 

In addition to the components above you will need tools like a soldering iron and a voltmeter. 

## Coffee Machine
The project uses a DeLonghi Magnifica S, but many other similar machines will probably work.

> [!WARNING]
> This project will break any warranty on your coffee machine, do this at your own risk!

# Procedure
First, brew yourself a coffe, it will be the last from this particular machine for a while. 
Turn off, unplug and dismantle your coffee machine to the point that you reach the breadboard with buttons controlling the machine. Probably behind the front panel.

There are often several buttons on the panel, identify what buttons you want to automate. 
This project automates the power button, in addition to the one for making espresso and americano.
Feel free to adjust the number of buttons to your own needs.

> [!TIP]
> A transistor can function as a switch, this project utilizes this property to simulate the push of a button.
> 
> A simplified diagram illustrates the concept, the resistor will be connected to one of the GPIO's on the ESP8266 board.
> ![image](https://github.com/sindrebilden/netpresso/assets/43905215/58dedd62-53e7-43e5-a602-413b00e35fb7)
>
> A transistor will have some leakage current leaking into the circuit of the button, we add a resistor with high resistance in order to minimize the leakage.
> Since we don't know the full resistance in the circuit of the button we should make it as high as possible but still allow the transistor to respond to the signal sent from the ESP8266 board.
> I went with 43 kΩ but feel free to adjust.

## Programming
It is easiest to program the ESP8266 board before soldering, [an example is found here](/netpresso.ino).

On first boot Netpresso will set up a fallback access point.
Connect to the network and navigate to `netpresso.local`, at that site it is possible to provide SSID and password for your home network.


| Address    | Description               | GPIO on ESP8226 |
|------------|---------------------------|-----------------|
| /          | Dashboard                 |                 |
| /power     | Activate power button     | D5              |
| /espresso  | Activate espresso button  | D6              |
| /americano | Activate americano button | D7              |

After connecting to a network it can be reset with by accessing root at it's IP address on the local network.
<img width="462" alt="image" src="https://github.com/sindrebilden/netpresso/assets/43905215/eb397f04-a58d-4bc4-b450-1f5a7f93cbaa">


## Soldering

First the base of the transistors (the middle leg) should be wired to the resistor. It may be useful to connect some wire in between to add flexibility.
![image](https://github.com/sindrebilden/netpresso/assets/43905215/f05c1eaf-5fe3-445e-8d6e-a1f7d2e10d13)

Then each transistor can be soldered to their corresponding button on the front panel. 
> [!IMPORTANT]
> The transistor must be placed in the correct direction of the voltage drop, the collector (left leg when facing the flat side of the transistor) should be at the highest potential.
> Use a voltmeter to determine the direction by placing one probe at each leg of the button. This must be done while the machine is turned on and the front panel is exposed.

![image](https://github.com/sindrebilden/netpresso/assets/43905215/7b502b2f-9c7e-442a-9dba-33e8d9ead764)

The Magnifica S has some unused fields on it's breadboard wich both provides 5V and ground. To power the ESP8266 board we can solder some wires to the corresponding fields.

<img width="600" alt="image" src="https://github.com/sindrebilden/netpresso/assets/43905215/dc2550b8-2d0d-4aeb-8c1a-4cc62694c225" />
<img width="300" alt="image" src="https://github.com/sindrebilden/netpresso/assets/43905215/1acf1ee6-bb80-4b86-90b7-80e93b801919" />

The transistors wired on the front panel can be connected to the GPIOS on the ESP8266 board, the table below presents the how it's done in this project:

| GPIO | Wire Color | Button on panel |
|------|------------|-----------------|
| D5   | Blue       | Power           |
| D6   | Purple     | Espresso        |
| D7   | Yellow     | Americano       |

# References
[Pinout ESP8266](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)

## Similar projects
Not exactly what you had in mind? Take a look at theese projects:
- [Raspberry pi on DeLonghi Magnifica](https://misteu.github.io/RPiCoffee/)
- [MQTT on a DeLonghi Dinamica](https://github.com/tobygray/coffeesp32-link/tree/master)
