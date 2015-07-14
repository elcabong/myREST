setInterval(function() {

	$.get("curl.php", {station: "r", state: ""}, function(data) { 
        	json_data = jQuery.parseJSON(data);

		if (json_data.ack == "ok") {
			$("#deviceStatus").html("Online");
			$("#deviceStatus").css("color", "green");
		}
		else {
			$("#deviceStatus").html("Offline");
			$("#deviceStatus").css("color", "red");
		}

                if (json_data.D9 == 1) {		
                        $("#1").removeClass("off-button-active");
                        $("#1").addClass("off-button-inactive");
                        $("#2").removeClass("on-button-inactive");
                        $("#2").addClass("on-button-active");
                }
                else {
	                $("#1").removeClass("off-button-inactive");
	                $("#1").addClass("off-button-active");
        	        $("#2").removeClass("on-button-active");
                	$("#2").addClass("on-button-inactive");
		}

  	});

}, 2000);



function buttonClick(clicked_id){

    if (clicked_id == "1"){
        $.get( "curl.php", {
        station: "9", state: "0"} );
    }

    if (clicked_id == "2"){
        $.get( "curl.php", {
        station: "9", state: "1"} );
    }

}
