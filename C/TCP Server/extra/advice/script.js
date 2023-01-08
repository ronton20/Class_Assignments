let displayAdvice = function (responseText) {
    let advice = JSON.parse(responseText);
    $("#advice").text(advice.slip.advice);
}

$(document).ready(function() {
    $("#advice-btn").click(function() {
        $.ajax({url: "https://api.adviceslip.com/advice", success: displayAdvice});
    });
});