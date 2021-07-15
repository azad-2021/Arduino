<?php
$Device2 = htmlspecialchars($_GET["Value"]);
$myfile = fopen("Status2.txt", "w") or die("Unable to open file!");
fwrite($myfile,',');
$data = array("Status2"=>$Device2);

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);
fclose($myfile);
?>