<?php
$Device1 = htmlspecialchars($_GET["Value"]);
$myfile = fopen("Status1.txt", "w") or die("Unable to open file!");
fwrite($myfile,'{"feeds":[');
$data = array("Status1"=>$Device1);

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);

fclose($myfile);


$Device1 = htmlspecialchars($_GET["Value"]);
$myfile = fopen("Device1.json", "w") or die("Unable to open file!");
fwrite($myfile,'{"feeds":[');
$data = array("Device1"=>$Device1);

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);

fclose($myfile);


?>