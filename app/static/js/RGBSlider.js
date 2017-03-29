$(function() {
	var RGBChange = function() {
	$('#RGB').css('background', 'rgb('+r.getValue()+','+g.getValue()+','+b.getValue()+')');	
};

var Red = $('#R').slider();
var Green = $('#G').slider();
var Blue = $('#B').slider();
$(function() {
	Red.on('slide', function() {
	$(red_slider_text).text(Red.slider('getValue'));
});
});
$(function() {
	Green.on('slide', function() {
	$(green_slider_text).text(Green.slider('getValue'));
});
});
$(function() {
	Blue.on('slide', function() {
	$(blue_slider_text).text(Blue.slider('getValue'));
});
});
var r = $('#R').slider()
		.on('slide', RGBChange)
		.data('slider');
var g = Green
		.on('slide', RGBChange)
		.data('slider');
var b = Blue
		.on('slide', RGBChange)
		.data('slider');

var sendColors1 = 
function() {
	$.ajax({
	  type: "POST",
	  contentType: "application/json; charset=utf-8",
	  url: "/setColorsJSON",
	  data: sendData(),
	  success: function (data) {
	  },
	  dataType: "json"
	})
};

Red.on("slideStop", sendColors1);

Green.on("slideStop", sendColors1);

Blue.on("slideStop", sendColors1);

var sendData = function() {
				return JSON.stringify({ Red:Red.slider('getValue')
								, Green:Green.slider('getValue'),
								Blue: Blue.slider('getValue') });
};

$(function() {
    //hang on event of form with id=myform
    $("#light_timer_form").submit(function(e) {
    	var mdata = {};
        //prevent Default functionality
        e.preventDefault();

		$("#light_timer_form").serializeArray().map(function(x){mdata[x.name] = x.value;}); 
        //get the action-url of the form
        var actionurl = '/setLightPeriod';
        console.log(mdata);
        //do your own request an handle the results
        $.ajax({
                url: actionurl,
                type: "POST",
                contentType: "application/json; charset=utf-8",
                dataType: 'json',
                data: JSON.stringify({light_time: mdata.light_time}),
                success: function(data) {
                   alert(data.light_time);
                }
        });

    });

});


$("#Submit_button").on("click", function(e) {
	e.preventDefault();

	$.ajax({
	  type: "POST",
	  contentType: "application/json; charset=utf-8",
	  url: "/setColorsJSON",
	  data: sendData(),
	  success: function (data) {
	    alert(data.Red+","+data.Green + "," + data.Blue);
	  },
	  dataType: "json"
	});
});


});
///////////////LOAD THE CHART NEEDS REFACTORING /////
window.onload = function(){
		var plotChart = function(data){
			var lineChartData = {
				labels : ["January","February","March","April","May","June","July"],
				datasets : [
					{
						label: "My First dataset",
						fillColor : "rgba(220,220,220,0.2)",
						strokeColor : "rgba(220,220,220,1)",
						pointColor : "rgba(220,220,220,1)",
						pointStrokeColor : "#fff",
						pointHighlightFill : "#fff",
						pointHighlightStroke : "rgba(220,220,220,1)",
						//data : [randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor()]
						data : data.humidity
					},
			
				]

			}
			var chart1 = document.getElementById("line-chart").getContext("2d");
		window.myLine = new Chart(chart1).Line(lineChartData, {
			responsive: true
		});

	}
	
	$.ajax({
	type: "GET",
	contentType: "application/json; charset=utf-8",
	url: "/getEnvironmentData",
	success: plotChart,
	dataType: "json",
	error: function(XMLHttpRequest, textStatus, errorThrown) { 
        alert("Status: " + textStatus); alert("Error: " + errorThrown); 
    }   
	});
};


