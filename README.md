# Transmission-RC
remote client for transmission daemon

Terminal UI
![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/terminal.png)
Dependencies:
<br />
-curl
<br />
-boost<br />
-ncursers
<br />
<br />

GTK UI<br />

![alt text](https://github.com/TylerOrcutt/Transmission-RC/blob/master/screens/gtk.png)
<br />
Dependencies:<br />
-curl<br />
-boost<br />
-GTK/GTKMM<br />
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
-t   outputs a list of torrents with the id, name, size, status, percent done
<br/>
-r \<torrentID\>- resume torrent
 <br />
-s \<torrentID\> - stop torrent
<br />
-add URL/magnet link - adds a torrent
<br />
url/magnet link -add a torrents 
  
