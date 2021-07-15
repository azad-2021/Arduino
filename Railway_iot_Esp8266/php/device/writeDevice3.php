<?php
$Device3 = htmlspecialchars($_GET["Value"]);
$myfile = fopen("Device3.txt", "w") or die("Unable to open file!");
fwrite($myfile,',');
$data = array("Device3"=>$Device3);

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);
fwrite($myfile,'] }');
fclose($myfile);
?>