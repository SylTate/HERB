var updateCharts = function(e) {
        var obj = JSON.parse(e.data);
        var newValue = obj.temp;
        var humidityvalue = obj.humidity;
        $('#easypiechart-orange').data('easyPieChart').update(newValue);
        $('span',("#easypiechart-orange")).text(newValue + "\xB0");
        $('#easypiechart-blue').data('easyPieChart').update(humidityvalue);
        $('span',("#easypiechart-blue")).text(humidityvalue + "\x25");
    };
if (!!window.EventSource) {
    var source = new EventSource('/getStream');
    source.onmessage = updateCharts
}


$(function() {
    $('#easypiechart-teal').easyPieChart({
        scaleColor: false,
        barColor: '#1ebfae'
    });
});

$(function() {
    $('#easypiechart-orange').easyPieChart({
        scaleColor: false,
        barColor: '#ffb53e'
    });
});

$(function() {
    $('#easypiechart-red').easyPieChart({
        scaleColor: false,
        barColor: '#f9243f'
    });
});

$(function() {
   $('#easypiechart-blue').easyPieChart({
       scaleColor: false,
       barColor: '#30a5ff'
   });
});

