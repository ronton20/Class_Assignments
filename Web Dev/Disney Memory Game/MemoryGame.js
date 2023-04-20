const maxInput = 8;
const minInput = 2;
let columns = 5;
let rows = 4;

const firstPageUrl = "https://api.disneyapi.dev/characters";
let nextPageUrl = firstPageUrl;
let curretPage = 0;
let cardCounter = 0;

let cards = []; // Array of cards

let turns = 0;
let matches = 0;

let startTime;
let endTime;

const grid = $(".grid");
const turnsElement = $("#turns");
const matchesElement = $("#matches");
const flippedCards = [];

class Card {
	constructor(character) {
		this.character = character;
		this.element = null;
		this.flipped = false;
		this.matched = false;
	}

	setElement(element) {
		this.element = element;
		this.element.on("click", () => {
			// Check if the card is already flipped or matched and if there are already 2 flipped cards
			if (flippedCards.length < 2 && !this.flipped && !this.matched) {
				this.flip();
				if (flippedCards.length === 2) {
					checkForMatch();
				}
			}
		});
	}

	// Flip the card
	flip() {
		this.flipped = true;
		this.element.addClass("flipped");
		flippedCards.push(this);
	}

	// Unflip the card
	unflip() {
		this.flipped = false;
		this.element.removeClass("flipped");
	}
}

// Check if the player has won
function checkForWin() {
	if ($(".flipped").length === rows * columns) {
		endTime = getTime();
		setTimeout(() => {
			alert(`You win! Total time: ${getTimeDiff()}`);
		}, 500);
	}
}

// Check if the flipped cards match
function checkForMatch() {
	turns++;
	turnsElement.val(turns);
	if (flippedCards[0].character === flippedCards[1].character) {
		flippedCards[0].element.addClass("matched");
		flippedCards[0].matched = true;
		flippedCards[1].element.addClass("matched");
		flippedCards[1].matched = true;
		setTimeout(() => {
			openPopup(flippedCards[0]);
			// Clear the flipped cards array
			flippedCards.length = 0;
		}, 700);
		matches++;
		matchesElement.val(matches);
	} else {
		$("#no-match-popup").addClass("show");
		setTimeout(() => {
			flippedCards[0].unflip();
			flippedCards[1].unflip();
			flippedCards.length = 0;
			$("#no-match-popup").removeClass("show");
		}, 1000);
	}
}

// Open the popup with the character info
function openPopup(card) {
	// Set the popup data

	// Set the character image
	$("#char-img").css("background-image", `url(${card.character.imageUrl})`);

	// Set the character name
	$("#char-name").text(card.character.name);

	// Set the character films
	let films = "";
	card.character.films.forEach((film) => {
		films += film + "<br>";
	});
	// remove last '<br>'
	films = films.substring(0, films.length - 4);
	$("#char-movies").html(films);

	// Set the character TV shows
	let tvShows = "";
	card.character.tvShows.forEach((tvShow) => {
		tvShows += tvShow + "<br>";
	});
	// remove last '<br>'
	tvShows = tvShows.substring(0, tvShows.length - 4);
	$("#char-tv-shows").html(tvShows);

	// Set the character games
	let games = "";
	card.character.videoGames.forEach((game) => {
		games += game + "<br>";
	});
	// remove last '<br>'
	games = games.substring(0, games.length - 4);
	$("#char-games").html(games);

	// Set the character attractions
	let attractions = "";
	card.character.parkAttractions.forEach((attraction) => {
		attractions += attraction + "<br>";
	});
	// remove last '<br>'
	attractions = attractions.substring(0, attractions.length - 4);
	$("#char-attractions").html(attractions);

	$("#popup-content").addClass("show");
	$("#overlay").addClass("show");
}

// Close the popup
function closePopup() {
	$("#popup-content").removeClass("show");
	$("#overlay").removeClass("show");
	checkForWin();
}

// Generate an array of [a] random non repeating indexes from 0 to [b]
function getRandonIndexes(a, b) {
	let arr = [];
	while (arr.length < a) {
		let r = Math.floor(Math.random() * b);
		if (arr.indexOf(r) === -1) arr.push(r);
	}
	return arr;
}

// Create the cards
const CreateCards = (arr) => {
	cards = [];
	let cardsToCreate = rows * columns;
	let cardsCreated = 0;
	let indexes = getRandonIndexes(cardsToCreate / 2, arr.length);
	for (let i = 0; i < cardsToCreate; i++) {
		cards.push(new Card(arr[indexes[cardCounter]]));
		cardsCreated++;
		if (cardsCreated == 2) {
			cardCounter = (cardCounter + 1) % arr.length;
			cardsCreated = 0;
		}
	}
	shuffleCards();
};

// Shuffle the cards
function shuffleCards() {
	for (let i = cards.length - 1; i > 0; i--) {
		const j = Math.floor(Math.random() * (i + 1));
		[cards[i], cards[j]] = [cards[j], cards[i]];
	}
	addCards();
}

// Add the cards to the card container
function addCards() {
	const cardContainer = $("#card-container");
	// Clear the card container
	cardContainer.empty();

	// Create the cards
	cards.forEach((card) => {
		const cardDiv = $(`<div class="card"></div>`);
		const front = $('<div class="front"></div>');
		const back = $('<div class="back"></div>');
		const name = $(`<p>${card.character.name}</p>`);
		front.append(name);
		cardDiv.append(front);
		cardDiv.append(back);
		const image = $(`<img src="${card.character.imageUrl}">`);
		front.append(image);
		cardContainer.append(cardDiv);
		card.setElement(cardDiv);
	});
}

// Get the characters from the API
function getCharacters() {
	$.ajax({
		url: nextPageUrl,
		dataType: "json",
		success: function (response) {
			curretPage++;
			if (curretPage < response.totalPages) nextPageUrl = response.nextPage;
			else nextPageUrl = firstPageUrl;
			CreateCards(response.data);
		},
	});
}

// Get the current time from php file
function getTime() {
	var time;
	$.ajax({
		url: "get_current_time.php",
		type: "GET",
		async: false,
		success: function (response) {
			time = response;
		},
	});
	return time;
}

// Get the time difference between the start and end time
function getTimeDiff() {
	let diff = endTime - startTime;
	let seconds = diff % 60;
	let minutes = Math.floor(diff / 60) % 60;
	let hours = Math.floor(diff / 3600) % 60;
	// make it so there are always 2 digits
	let res = [hours, minutes, seconds].map((x) => {
		if (x < 10) return "0" + x;
		return x;
	});
	return res.join(":");
}

// Add selector inputs for rows and columns
function addSelectInputs() {
	$("#sizes").empty();
	// Create the select inputs
	var $rowSelect = $("<select>", { id: "row-select" });
	var $colSelect = $("<select>", { id: "col-select" });

	// Add options to the select inputs
	for (var i = 2; i <= 8; i++) {
		if (i % 2 == 0) $rowSelect.append($("<option>", { value: i, text: i }));
		$colSelect.append($("<option>", { value: i, text: i }));
	}

	// Create the labels
	var $rowLabel = $("<label>", { for: "row-select", text: "Rows:" });
	var $colLabel = $("<label>", { for: "col-select", text: "Columns:" });

	// Create the containing divs
	var $rowDiv = $("<div>", { id: "row-select-div" });
	var $colDiv = $("<div>", { id: "col-select-div" });

	// Append the select inputs, labels, and divs to the page
	$rowDiv.append($rowLabel, $rowSelect);
	$colDiv.append($colLabel, $colSelect);
	$("#sizes").append($rowDiv, $colDiv);

	// Set the default values
	$rowSelect.val(rows);
	$colSelect.val(columns);
}

// Start a new game
const newGame = () => {
	cardCounter = 0;
	rows = $("#row-select").val();
	columns = $("#col-select").val();
	$("#card-container").css("grid-template-columns", `repeat(${columns}, 1fr)`);
	turns = 0;
	matches = 0;
	flippedCards.length = 0;
	turnsElement.val(turns);
	matchesElement.val(matches);
	getCharacters();
	startTime = getTime();
};

const LoadPage = () => {
	addSelectInputs();
	$("#card-container").css("grid-template-columns", `repeat(${columns}, 1fr)`);
	flippedCards.length = 0;
	turnsElement.val(turns);
	matchesElement.val(matches);
	// Set the card images
	// getCharacters();
	$("#new-game-button").click(newGame);
	$("#popup-button").click(closePopup);
};

$(LoadPage);
