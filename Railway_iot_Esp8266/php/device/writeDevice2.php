<?php
$Device2 = htmlspecialchars($_GET["Value"]);
$myfile = fopen("Device2.txt", "w") or die("Unable to open file!");
fwrite($myfile,',');
$data = array("Device2"=>$Device2);

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);
fclose($myfile);
?>