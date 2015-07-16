// window.setInterval(get_device_status, 1000);

window.onload = get_device_status;

function get_device_status() {
	$.ajaxSetup({
	  cache: false
	});

	$.get("curl.php", {url: "192.168.2.79", command: "dim", station: "r", state: ""}, function(data) {
        	json_data = jQuery.parseJSON(data);

		if (json_data.ack == "ok") {
		  $("#lightStatus").html("Online");
		  $("#lightStatus").css("color", "green");
		}
		else {
		  $("#lightStatus").html("Offline");
		  $("#lightStatus").css("color", "red");
		}

		/* use this switch as an example for the rest  */
		switch(json_data.Dim0) {
		  case "1":
                    $("#L1 .button").removeClass("active");
                    $("#2").addClass("active");
		    break;
		  case "5":
                    $("#L1 .button").removeClass("active");
                    $("#3").addClass("active");		  
		    break;
		  case "on":
                    $("#L1 .button").removeClass("active");
                    $("#4").addClass("active");		  
		    break;
                  case "off":
                    $("#L1 .button").removeClass("active");
                    $("#1").addClass("active");		 
		    break;
		}
		
		
        switch(json_data.Dim1) {
		  case "3":
                    $("#5").removeClass("off-button-active");
                    $("#5").addClass("off-button-inactive");
                    $("#6").removeClass("on-button-inactive");
                    $("#6").addClass("on-button-active");
                    $("#7").removeClass("on-button-active");
                    $("#7").addClass("on-button-inactive");
                    $("#8").removeClass("on-button-active");
                    $("#8").addClass("on-button-inactive");
                    break;
                  case "5":
                    $("#5").removeClass("off-button-active");
                    $("#5").addClass("off-button-inactive");
                    $("#6").removeClass("on-button-active");
                    $("#6").addClass("on-button-inactive");
                    $("#7").removeClass("on-button-inactive");
                    $("#7").addClass("on-button-active");
                    $("#8").removeClass("on-button-active");
                    $("#8").addClass("on-button-inactive");
		    break;
                  case "on":
                    $("#5").removeClass("off-button-active");
                    $("#5").addClass("off-button-inactive");
                    $("#6").removeClass("on-button-active");
                    $("#6").addClass("on-button-inactive");
                    $("#7").removeClass("on-button-active");
                    $("#7").addClass("on-button-inactive");
                    $("#8").removeClass("on-button-inactive");
                    $("#8").addClass("on-button-active");
		    break;
                  case "off":
                    $("#5").removeClass("off-button-inactive");
                    $("#5").addClass("off-button-active");
                    $("#6").removeClass("on-button-active");
                    $("#6").addClass("on-button-inactive");
                    $("#7").removeClass("on-button-active");
                    $("#7").addClass("on-button-inactive");
                    $("#8").removeClass("on-button-active");
                    $("#8").addClass("on-button-inactive");
		    break;
		}
		switch(json_data.Dim2) {
                  case "3":
                    $("#9").removeClass("off-button-active");
                    $("#9").addClass("off-button-inactive");
                    $("#10").removeClass("on-button-inactive");
                    $("#10").addClass("on-button-active");
                    $("#11").removeClass("on-button-active");
                    $("#11").addClass("on-button-inactive");
                    $("#12").removeClass("on-button-active");
                    $("#12").addClass("on-button-inactive");
		    break;
		  case "5":
                    $("#9").removeClass("off-button-active");
                    $("#9").addClass("off-button-inactive");
                    $("#10").removeClass("on-button-active");
                    $("#10").addClass("on-button-inactive");
                    $("#11").removeClass("on-button-inactive");
                    $("#11").addClass("on-button-active");
                    $("#12").removeClass("on-button-active");
                    $("#12").addClass("on-button-inactive");
		    break;
		  case "on":
                    $("#9").removeClass("off-button-active");
                    $("#9").addClass("off-button-inactive");
                    $("#10").removeClass("on-button-active");
                    $("#10").addClass("on-button-inactive");
                    $("#11").removeClass("on-button-active");
                    $("#11").addClass("on-button-inactive");
                    $("#12").removeClass("on-button-inactive");
                    $("#12").addClass("on-button-active");
		    break;
		  case "off":
                    $("#9").removeClass("off-button-inactive");
                    $("#9").addClass("off-button-active");
                    $("#10").removeClass("on-button-active");
                    $("#10").addClass("on-button-inactive");
                    $("#11").removeClass("on-button-active");
                    $("#11").addClass("on-button-inactive");
                    $("#12").removeClass("on-button-active");
                    $("#12").addClass("on-button-inactive");
		    break;
		}
                switch(json_data.Dim3) {
                  case "3":
                    $("#13").removeClass("off-button-active");
                    $("#13").addClass("off-button-inactive");
                    $("#14").removeClass("on-button-inactive");
                    $("#14").addClass("on-button-active");
                    $("#15").removeClass("on-button-active");
                    $("#15").addClass("on-button-inactive");
                    $("#16").removeClass("on-button-active");
                    $("#16").addClass("on-button-inactive");
                    break;
                  case "5":
                    $("#13").removeClass("off-button-active");
                    $("#13").addClass("off-button-inactive");
                    $("#14").removeClass("on-button-active");
                    $("#14").addClass("on-button-inactive");
                    $("#15").removeClass("on-button-inactive");
                    $("#15").addClass("on-button-active");
                    $("#16").removeClass("on-button-active");
                    $("#16").addClass("on-button-inactive");
                    break;
                  case "on":
                    $("#13").removeClass("off-button-active");
                    $("#13").addClass("off-button-inactive");
                    $("#14").removeClass("on-button-active");
                    $("#14").addClass("on-button-inactive");
                    $("#15").removeClass("on-button-active");
                    $("#15").addClass("on-button-inactive");
                    $("#16").removeClass("on-button-inactive");
                    $("#16").addClass("on-button-active");
                    break;
                  case "off":
                    $("#13").removeClass("off-button-inactive");
                    $("#13").addClass("off-button-active");
                    $("#14").removeClass("on-button-active");
                    $("#14").addClass("on-button-inactive");
                    $("#15").removeClass("on-button-active");
                    $("#15").addClass("on-button-inactive");
                    $("#16").removeClass("on-button-active");
                    $("#16").addClass("on-button-inactive");
                    break;
                }
                switch(json_data.Dim4) {
                  case "3":
                    $("#17").removeClass("off-button-active");
                    $("#17").addClass("off-button-inactive");
                    $("#18").removeClass("on-button-inactive");
                    $("#18").addClass("on-button-active");
                    $("#19").removeClass("on-button-active");
                    $("#19").addClass("on-button-inactive");
                    $("#20").removeClass("on-button-active");
                    $("#20").addClass("on-button-inactive");
                    break;
                  case "5":
                    $("#17").removeClass("off-button-active");
                    $("#17").addClass("off-button-inactive");
                    $("#18").removeClass("on-button-active");
                    $("#18").addClass("on-button-inactive");
                    $("#19").removeClass("on-button-inactive");
                    $("#19").addClass("on-button-active");
                    $("#20").removeClass("on-button-active");
                    $("#20").addClass("on-button-inactive");
                    break;
                  case "on":
                    $("#17").removeClass("off-button-active");
                    $("#17").addClass("off-button-inactive");
                    $("#18").removeClass("on-button-active");
                    $("#18").addClass("on-button-inactive");
                    $("#19").removeClass("on-button-active");
                    $("#19").addClass("on-button-inactive");
                    $("#20").removeClass("on-button-inactive");
                    $("#20").addClass("on-button-active");
                    break;
                  case "off":
                    $("#17").removeClass("off-button-inactive");
                    $("#17").addClass("off-button-active");
                    $("#18").removeClass("on-button-active");
                    $("#18").addClass("on-button-inactive");
                    $("#19").removeClass("on-button-active");
                    $("#19").addClass("on-button-inactive");
                    $("#20").removeClass("on-button-active");
                    $("#20").addClass("on-button-inactive");
                    break;
                }
                switch(json_data.Dim5) {
                  case "3":
                    $("#21").removeClass("off-button-active");
                    $("#21").addClass("off-button-inactive");
                    $("#22").removeClass("on-button-inactive");
                    $("#22").addClass("on-button-active");
                    $("#23").removeClass("on-button-active");
                    $("#23").addClass("on-button-inactive");
                    $("#24").removeClass("on-button-active");
                    $("#24").addClass("on-button-inactive");
                    break;
                  case "5":
                    $("#21").removeClass("off-button-active");
                    $("#21").addClass("off-button-inactive");
                    $("#22").removeClass("on-button-active");
                    $("#22").addClass("on-button-inactive");
                    $("#23").removeClass("on-button-inactive");
                    $("#23").addClass("on-button-active");
                    $("#24").removeClass("on-button-active");
                    $("#24").addClass("on-button-inactive");
                    break;
                  case "on":
                    $("#21").removeClass("off-button-active");
                    $("#21").addClass("off-button-inactive");
                    $("#22").removeClass("on-button-active");
                    $("#22").addClass("on-button-inactive");
                    $("#23").removeClass("on-button-active");
                    $("#23").addClass("on-button-inactive");
                    $("#24").removeClass("on-button-inactive");
                    $("#24").addClass("on-button-active");
                    break;
                  case "off":
                    $("#21").removeClass("off-button-inactive");
                    $("#21").addClass("off-button-active");
                    $("#22").removeClass("on-button-active");
                    $("#22").addClass("on-button-inactive");
                    $("#23").removeClass("on-button-active");
                    $("#23").addClass("on-button-inactive");
                    $("#24").removeClass("on-button-active");
                    $("#24").addClass("on-button-inactive");
                    break;
                }

  	});

        $.get("curl.php", {url: "192.168.2.162", command: "d", station: "r", state: ""}, function(data) {
                json_data = jQuery.parseJSON(data);

                if (json_data.ack == "ok") {
                        $("#garageStatus").html("Online");
                        $("#garageStatus").css("color", "green");
                }
                else {
                        $("#garageStatus").html("Offline");
                        $("#garageStatus").css("color", "red");
                }

                if (json_data.D9 == 0) {
                        $("#25").removeClass("off-button-inactive");
                        $("#25").addClass("off-button-active");
                        $("#26").removeClass("on-button-active");
                        $("#26").addClass("on-button-inactive");
                }
                else if (json_data.D9 == 1) {
                        $("#25").removeClass("off-button-active");
                        $("#25").addClass("off-button-inactive");
                        $("#26").removeClass("on-button-inactive");
                        $("#26").addClass("on-button-active");
                }
                if (json_data.D8 == 0) {
                        $("#27").removeClass("off-button-inactive");
                        $("#27").addClass("off-button-active");
                        $("#28").removeClass("on-button-active");
                        $("#28").addClass("on-button-inactive");
                }
                else if (json_data.D8 == 1) {
                        $("#27").removeClass("off-button-active");
                        $("#27").addClass("off-button-inactive");
                        $("#28").removeClass("on-button-inactive");
                        $("#28").addClass("on-button-active");
                }
                if (json_data.D7 == 0) {
                        $("#29").removeClass("off-button-inactive");
                        $("#29").addClass("off-button-active");
                        $("#30").removeClass("on-button-active");
                        $("#30").addClass("on-button-inactive");
                }
                else if (json_data.D7 == 1) {
                        $("#29").removeClass("off-button-active");
                        $("#29").addClass("off-button-inactive");
                        $("#30").removeClass("on-button-inactive");
                        $("#30").addClass("on-button-active");
                }
                if (json_data.D4 == 0) {
                        $("#31").removeClass("off-button-inactive");
                        $("#31").addClass("off-button-active");
                        $("#32").removeClass("on-button-active");
                        $("#32").addClass("on-button-inactive");
                }
                else if (json_data.D4 == 1) {
                        $("#31").removeClass("off-button-active");
                        $("#31").addClass("off-button-inactive");
                        $("#32").removeClass("on-button-inactive");
                        $("#32").addClass("on-button-active");
                }

	}).done(setTimeout(get_device_status, 5000)); 

}

function buttonClick(clicked_id){
	switch(clicked_id) {
                case "1": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "0", state: "0"} ); break;
                case "2": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "0", state: "1"} ); break;
                case "3": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "0", state: "5"} ); break;
                case "4": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "0", state: "10"} ); break;
                case "5": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "1", state: "0"} ); break;
                case "6": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "1", state: "3"} ); break;
                case "7": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "1", state: "5"} ); break;
                case "8": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "1", state: "10"} ); break;
                case "9": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "2", state: "0"} ); break;
                case "10": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "2", state: "3"} ); break;
                case "11": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "2", state: "5"} ); break;
                case "12": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "2", state: "10"} ); break;
                case "13": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "3", state: "0"} ); break;
                case "14": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "3", state: "3"} ); break;
                case "15": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "3", state: "5"} ); break;
                case "16": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "3", state: "10"} ); break;
                case "17": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "4", state: "0"} ); break;
                case "18": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "4", state: "3"} ); break;
                case "19": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "4", state: "5"} ); break;
                case "20": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "4", state: "10"} ); break;
                case "21": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "5", state: "0"} ); break;
                case "22": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "5", state: "3"} ); break;
                case "23": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "5", state: "5"} ); break;
                case "24": $.get( "curl.php", {url: "192.168.2.79", command: "dim", station: "5", state: "10"} ); break;

                case "25": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "9", state: "0"} ); break;
                case "26": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "9", state: "1"} ); break;
                case "27": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "8", state: "0"} ); break;
                case "28": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "8", state: "1"} ); break;
                case "29": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "7", state: "0"} ); break;
                case "30": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "7", state: "1"} ); break;
                case "31": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "4", state: "0"} ); break;
                case "32": $.get( "curl.php", {url: "192.168.2.162", command: "d", station: "4", state: "1"} ); break;

                case "33": $.get( "curl.php", {url: "192.168.2.162", command: "momentary", station: "10", state: "250"} ); break;
                case "34": $.get( "curl.php", {url: "192.168.2.162", command: "momentary", station: "11", state: "250"} ); break;
	}
}
