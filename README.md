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
> This project will break any warrany on your coffee machine, do this at your own risk!

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
> <img width="206" alt="image" src="https://github.com/sindrebilden/netpresso/assets/43905215/58dedd62-53e7-43e5-a602-413b00e35fb7">
>
> A transistor will have some leakage current leaking into the circuit of the button, we add a resistor with high resistance in order to minimize the leakage.
> Since we don't know the full resistance in the circuit of the button we should make it as high as possible but still allow the transistor to respond to the signal sent from the ESP8266 board.
> I went with 43 kΩ but feel free to adjust.

## Soldering


# References
[https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/](Pinout ESP8266)
