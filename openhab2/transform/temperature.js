var celsius = true;

(function(i) {
	try {
		data = JSON.parse(i);
		temperature = data.temperature.toFixed(1);
		if (false === celsius) {
			// Convert temperature in Fahrenheit
			temperature = temperature * 9 / 5 + 32;
			return temperature + "F";
		}
		else {
			return temperature + "C";
		}
	}
	catch (err) {
		return "-";
	}
})(input)
