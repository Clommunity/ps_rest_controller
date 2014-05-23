//https://www.cvent.com/d/ncqhyw?lang=en&sms=7&cn=GOfH4kFCjUSy3RV-aH-Xig
var channel_url="channels.json";
var controller_host="127.0.0.1"; //Set this variable to rest_controller server IP address
var controller_port="1234";
var vlc_plugin_states = new Array("Idle","Loading..","Buffering..","Playing..","Paused","Stopped","Ended","Error");

function update_controller_status()
{
	var info_div = document.getElementById("information");
	var old_storage = document.getElementById("storage_element");

	var storage = document.createElement("object");
	storage.id = "storage_element";
	storage.style = "overflow:hidden; ";
	storage.data = "http://"+controller_host+":"+controller_port+"/about";	

	if ( old_storage != null )
		info_div.removeChild(old_storage);
	info_div.appendChild(storage);
}

function getVLCinstance()
{
	return document.getElementById("vlc");
}
function updateChannels()
{
    var curr = $("#ChannelsList option:selected").text();
    console.log("Uploading channels list...");
    console.log("current channel is "+curr);
    $("#ChannelsList").html("");
    $.ajaxSetup({ cache: false }); // force cache disabling
    jQuery.getJSON(channel_url,  
     function(jsonResult){
     console.log("Channels list json file downloaded...");
    })
    .done(function(data){
     $.each(data, function(key, val) {
      //console.log("key=" + key + " val=" + data.channels);
      $.each(val.channels, function(index, val) {
       var value = "http://"+controller_host+":"+controller_port+"/channelPort?name="+val.title+"&source_ip="+val.source+"&source_port="+val.port;
       var title = val.title + " @ " + key;
       $("#ChannelsList").append($("<option"+((title==curr)?" selected":"")+"></option>").attr("value",value).text(title));
       //console.log("id=\"" + index + "\" " + "value=\"" + value + "\" title=\""+title+"\"");
      });
     });
    })
//    .fail(function(jsonObj, textStatus, error) {
//     console.log( "Request Failed, error: " + error);
//     document.getElementById("ChannelsLog").innerHTML = "Error \"" + textStatus + "," + error + "\"";
//    })
    .always(function() { console.log( "complete" ); })
    .complete(function() { 
     console.log("Channels update completed!"); 
     document.getElementById("ChannelsLog").innerHTML = "Completed";
    });
};


function doPlay(value)
{
    // Currently, the playlist is not used since the
    // vlc element reads information from only one port.
    console.log("Getting... \""+value+"\"");
    $.ajaxSetup({ cache: false }); // force cache disabling
    jQuery.getJSON(value,
     function(jsonResult) {
      console.log("success");}
     )
    .done(function(data) {
     console.log("done"); 
     var log = "Done";
     $.each(data, function(key, val) {
      console.log("key=" + key + " val=" + val + " ele="+val.name);
      console.log("name=" + val.name + " source_ip=" + val.source_ip + " source_port=" + val.source_port + " local_udp_port=" + val.local_udp_port);
      //var target = "udp://@:"+val.local_udp_port;
      //console.log("fuuuuu target= \""+$("#vlc").attr("target")+"\"");
     });
     document.getElementById("ChannelsLog").innerHTML = log;
    })
    .fail(function(jsonResult, textStatus, err) { 
 //    console.log("Error: "+jsonResult+ " status="+textStatus+" err="+err); 
//     document.getElementById("ChannelsLog").innerHTML = "Error \"" + textStatus + "," + err + "\"";
    })
    .always(function(data) { 
     console.log("finished");
     $.each(data, function(key, val) {
//      console.log("key=" + key + " val=" + val);
     });
    });
};
function init()
{
		console.log("in init");
    var vlc = getVLCinstance(); 
    $(document).ready(function() {
      $.ajaxSetup({ cache: false });
    });
    vlc.audio.volume=50;    
    updateChannels();
    GetVersion();
		updateVolume(50);
    window.setInterval(function(){doState();},500);
//	window.setInterval(function() {update_controller_status();},3000);
		window.setInterval(function() {update_controller_status();},3000);
		};
function updateVolume(deltaVol)
{
    var vlc = getVLCinstance(); 
    vlc.audio.volume += deltaVol;
};
function fullscreen()
{
    var vlc = getVLCinstance(); 
    vlc.video.toggleFullscreen();
};
function PlayPause()
{
    var vlc = getVLCinstance(); 
    if (vlc.playlist.isPlaying)
    {
      document.getElementById("Play").value = " Play  ";
      pause();
    }
    else
    {
      pause();
      document.getElementById("Play").value = " Pause ";
    }
};
function bugAudio()
{
    var vlc = getVLCinstance(); 
    if(vlc.playlist.isPlaying) 
    {
    // Bug: VLC v-1.1.4: no audio after resume from pause
    // Trick: Force vlc.audio.track to change and restore the previous value.     
    var track = vlc.audio.track;
    setTimeout(function(){doAudioTrack(vlc.audio.count - 1);},50);
    setTimeout(function(){doAudioTrack(track);},100);
    }
}
function play()
{
    var vlc = getVLCinstance(); 
    vlc.playlist.play();
};
function stop()
{
    var vlc = getVLCinstance(); 
    vlc.playlist.stop();
    document.getElementById("Play").value = " Play  "
};
function pause()
{
    var vlc = getVLCinstance(); 
    vlc.playlist.togglePause();
    if (vlc.playlist.isPlaying)
       bugAudio();
};
function mute(){
    var vlc = getVLCinstance(); 
    vlc.audio.toggleMute();
    document.getElementById("Mute").value = (vlc.audio.mute?" Unmute ":" Mute ");
};
function GetVersion()
{
    var vlc = getVLCinstance(); 
    document.getElementById("version").innerHTML = "vlc " + vlc.VersionInfo;
};
function doAspectRatio(value)
{
   var vlc = getVLCinstance(); 
   vlc.video.aspectRatio = value;
   document.getElementById("state").innerHTML = "Aspect " + value;
};
function doAudioChannel(value)
{
    var vlc = getVLCinstance(); 
    vlc.audio.channel = parseInt(value);
};
function doAudioTrack(value)
{
    var vlc = getVLCinstance(); 
    vlc.audio.track = vlc.audio.track + value;
    document.getElementById("trackTextField").innerHTML = vlc.audio.track + "/" + (vlc.audio.count-1);
};
function doState()
{
    var vlc = getVLCinstance(); 
    var newState = vlc.input.state;
    
		if (newState >= 0 && newState <= 7)
			document.getElementById("state").innerHTML = vlc_plugin_states[newState];
		else
			document.getElementById("state").innerHTML = "Unrecognized plugin state D:";

};
