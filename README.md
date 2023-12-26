## Youtube Link:
https://youtu.be/yoha_CzNoKA

## Purpose
This project is based on the classic video game “Pong”. There are two paddles on either side of the screen, one for each player, and they use it to try and get the ball past their opponent's paddle and earn a point. To win the game, the player has to reach five points. It can be played with either one or two players. If the user plays alone, they will go against a bot as their opponent.

## Functionality
There are three main aspects of this project, the Arduino, the Raspberry Pi, and the website. Here is a picture of the components together:
<img width="543" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/beeb0696-8e88-4adc-8ea3-21535992e416">

The Arduino is the core of the project. I’m using an Arduino Mega due to it having more GPIO pins to use. The Arduino is used to control the screen, joysticks, and LEDs. Starting with the screen, it’s a TFT screen built with the Arduino in mind. As a result, the screen requires a large portion of the GPIO pins, and if I were using a regular Arduino board, then this project would be unfeasible. To control the screen I used the Adafruit GFX and MCUFRIEND_kbv libraries, which allowed me to draw the menu and the game itself. Here is how the game looks:

<img width="530" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/ef8d227d-4777-4932-91d0-48c0d25de14a">
The game itself

<img width="512" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/87ced80e-78d2-4d9d-9383-8d9b1f32926a">
The menu, displaying “Player 2 Wins” to show the result of the previous game

The joysticks are used for menu control, where the player can select if they want to play with two players or with one player. There are also 10 LEDs, which are used to display the player's score. Since the goal of the game is to get five points, there are five LEDs that are allocated for the two players, with five blue LEDs for player one and five green for player two.

Another core concept the Arduino does is making all of the calculations for the single-player computer opponent. The computer opponent works by checking if the ball is moving in its direction, and if it is then it will calculate where the ball will end up and move there. It does this by taking the x and y coordinates of the ball, and since it knows the direction it’s going in, otherwise known as the slope, we can use y=mx+b to calculate where the ball will eventually go.

With this method, it would be impossible to beat the bot, since it can perfectly calculate where it needs to go to reflect the ball. I added a 13/20 chance for the bot to reflect the ball, where a random number is picked between 1 and 20 and that number will determine if the computer opponent will hit the ball or not. However this brings up the issue of randomization if the random seed was a set value, then the computer would be too predicatable. So the seed for the computer opponent is the amount of time it takes for the player to start the game from the menu, and this is done through the millis() function. This random seed is also used for determining the direction the ball will go at the beginning of the game.

The second part of the project is the Raspberry Pi. The Pi is running Raspbian, an optimized version of Debian Linux built for the Pi. The purpose of the Pi is to act as a messenger between the Arduino and the website. To do this, I use the Flask API, which allows me to read messages from the website, translate the message into a method the Arduino can read, and send it to the Arduino using the Serial library. The code for the Pi is written using Python. In terms of physical connections, the Pi is connected to the Arduino via USB, as seen here:
<img width="590" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/9303f876-0462-4337-8d10-e69b0dd3fcc4">


The website sends string data to the Pi and states if Player 1 or Player 2 wants their paddle to go left or right. This is done through the strings, “left1”, “right1”, “left2”, and “right2”. Once the Pi receives this data, I add or subtract 5 to the y-coordinates that the respective paddles are in at that time. The Pi then sends the y-coordinates value to the Arduino, so that the Arduino can immediately translate the player paddle upon arrival of the message.

In terms of power, the whole system is powered through a portable battery bank, which is connected via micro-USB to the Pi. The Arduino gets its power through the USB connection from the Pi, which in turn powers all of the components that are connected to the Arduino. For network connectivity, the Pi uses WiFi, although a cellular module or mobile hotspot can be used in situations where WiFi is not available.

The final aspect of the project is the website itself. The website was made using HTML for the site itself, JavaScript for the code regarding communication between the website and the Pi, and is hosted on my laptop using Apache. Here is what the website looks like:
<img width="404" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/ded2753e-25c4-4c46-8955-89cf60c4c3f2">


As you can see from the picture above, there are four buttons, with two buttons being allocated for each player. The players press the left and right buttons to move their respective players on the Arduino. Players can use multiple devices to control their players, so player 1 can use their device, and player 2 can use their device to control their respective paddles. Here is a picture of the same site on both a tablet and laptop simultaneously:
<img width="687" alt="image" src="https://github.com/Tejasarus/pong/assets/50884660/5bdfca6d-a624-42ca-815c-87e68cc67990">


To send the data to the Raspberry Pi, ‘POST’ requests are used, which sends the string data when a button is pressed.

## How Tested
Initial testing was done through the two joysticks which acted as the controls for the two paddles to test the functionality of the game. Once the game as a whole worked, I moved on to the implementation of the Pi. To test the Pi and its connection with the Arduino, I wrote the code so that I could manually send y-coordinate values as inputs (using Python input()) and see if the Arduino would react to the change at all, in terms of if it moved the paddle to the inputted y-coordinate. Finally, to test the communication between the Pi and the website, I first checked to see if the Pi would receive any data from the website. Once I managed to obtain a connection, I moved on to receive strings from the website and finally integrated those string-sending messages with the buttons.

## Challenges/Lessons Learned
Since I have never worked with communication between a website and another device, there was a steep learning curve to learn how to communicate between the two devices. I also learned that there are static and dynamic pages, and that the website I built was a dynamic page, so hosting it off of GitHub pages or google pages wasn’t viable since they only support static pages. My solution was to host my website by myself, however, I had the issue of not being able to port-forward on my WiFi connection since I do not have that type of control and access (UCR-SECURE). My solution was to host it on my local network, with my local network being my phone hotspot, which proved to be a viable solution.

