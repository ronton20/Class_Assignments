# Disney Memory Game

### Author: Ron Ariel

This is a web-based Memory Game that uses the Disney API to get a list of random characters. The game is designed for users to match the pairs of characters displayed on the cards.

## Table of Contents

-   [Getting Started](#getting-started)
-   [Game Overview](#game-overview)
-   [How to Play](#how-to-play)
-   [How it Works](#how-it-works)
-   [Configuration](#configuration)
-   [HTTP Requests](#http-requests)

## Getting Started

Open [Ron's Memory Game](http://ronam.mysoft.jce.ac.il/ex1/) (Currntly only available in Israel).

## Game Overview

The Memory Game consists of a grid of cards. Players need to match pairs of cards with the same character. The game starts with a click on the `New Game` button, which sends an Ajax call to a `.php` file that provides the current time and a call to the Disney Characters API that fills the `rows * columns` cards with pairs of random characters.

## How to Play

1. Click on the `New Game` button to start the game.
2. Flip over two cards by clicking on them.
3. If the cards match, a popup window with the character's details will appear.
4. If the cards do not match, a `Try Again` banner will appear at the top of the screen and the cards will unflip after a short period of time.
5. Repeat steps 2-4 until all pairs have been found.
6. Once all pairs have been found, an alert will pop up indicating that you have won and how much time has passed.

## How it Works

-   When the player clicks on `New Game`, an AJAX call is sent to a `.php` file that generates the current time and calls the Disney Characters API to fill the `rows * columns` cards with pairs of random characters.
-   The player can flip over two cards at a time. If they match, a popup window with the character's details will appear. If they do not match, a `Try Again` banner will appear at the top of the screen and the cards will unflip after a short period of time.
-   Once the player finds all the pairs, an alert will pop up indicating that they have won and how much time has passed. This is done by sending another AJAX call to the `.php` file and calculating the time passed since the start.
-   The game uses `JavaScript` to flip and unflip cards, check for matches, and handle popups and alerts.
-   The game uses `jQuery` to manipulate the DOM and handle events.

## Configuration

-   `maxInput` and `minInput` can be used to set the maximum and minimum number of columns and rows that can be chosen in the game.

## HTTP Requests

1. Initialization:
    - The browser sends a request to load the HTML file containing the JavaScript code.
    - The server responds with the HTML file, which is rendered by the browser.
    - The browser sends additional requests for the CSS and JavaScript files used by the HTML file.
    - The server responds with the CSS and JavaScript files, which are rendered by the browser.
2. New Game:
    - The browser sends a `GET` request to the Disney API to retrieve data about Disney characters.
    - The server responds with the data in JSON format.
    - The browser sends a `GET` request to the `.php` file to get the current time.
    - The server responds with the current time.
3. Game Completed:
    - The browser sends a `GET` request to the `.php` file to get the current time.
    - The server responds with the current time.
