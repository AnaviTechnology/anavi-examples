(function(i) {
	try {
		data = JSON.parse(i);
		humidity = data.humidity.toFixed(0);
		return humidity + "%";
	}
	catch (err) {
		return "-";
	}
})(input)
