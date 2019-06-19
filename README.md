# Transmission-RC
remote client for transmission daemon

Terminal UI
![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/terminal.png)
![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/terminal2.png)
Dependencies:
<br />
-curl
<br />
-boost<br />
-ncursers
<br />
<br />
Keys:<br />
Navigate down/up- arrow down/j , arrow up/k <br />
i - show torrent info<br/>
s - toggle start/stop <br>
d - delete torrent<br />
q - quit<br />
: - show command prompt
<br />
<br />
Commands:<br />
update blocklist - request transmission updates it's blocklist file
<br />

GTK UI<br />

![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/gtk.png)
<br />
Dependencies:<br />
-curl<br />
-boost<br />
-GTK/GTKMM<br />

GTK i3 Dropdown prototype<br />
![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/i3blocks_dropdown.png)

<br />
<br />  
Config example: ~/.config/trc.conf<br />
 username=transmission<br />
 password=rpc<br />
 host=192.169.1.11<br />
 port=9091<br />

<br />
<br />  
Arguments<br />
-t   outputs a list of torrents with the id, name, size, status, percent done<br/>
-r [ torrentID ] - resume torrent <br />
-s [ torrentID ] - stop torrent <br />
-blocklist-update -requests blocklist update<br />
-add URL/magnet link - adds a torrent<br />
url/magnet link -add a torrents <br />
  
