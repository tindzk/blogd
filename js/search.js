var searchbox = document.getElementById("searchbox");
var searchtxt = document.getElementById("searchtxt");
var tiptext = "Type text to search here...";

if(searchtxt.value == "" || searchtxt.value == tiptext) {
	searchtxt.className += " searchtip";
	searchtxt.value = tiptext;
}

searchtxt.onfocus = function(e) {
	if(searchtxt.value == tiptext) {
		searchtxt.value = "";
		searchtxt.className = searchtxt.className.replace(" searchtip", "");
	}
}

searchtxt.onblur = function(e) {
	if(searchtxt.value == "") {
		searchtxt.className += " searchtip";
		searchtxt.value = tiptext;
	}
}
