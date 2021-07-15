<?php
$myfile1 = fopen("Status1.txt", "r") or die("Unable to open file!");
$myfile2 = fopen("Status2.txt", "r") or die("Unable to open file!");
$myfile3 = fopen("Status3.txt", "r") or die("Unable to open file!");
echo fgets($myfile1);
echo fgets($myfile2);
echo fgets($myfile3);
fclose($myfile1);
fclose($myfile2);
fclose($myfile3);

$myfile = fopen("Status1.txt", "w") or die("Unable to open file!");
fwrite($myfile,'{"feeds":[');
$data = array("Status1"=>'0');

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);

fclose($myfile);


$myfile = fopen("Status2.txt", "w") or die("Unable to open file!");
fwrite($myfile,',');
$data = array("Status2"=>'0');

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);
fclose($myfile);

$myfile = fopen("Status3.txt", "w") or die("Unable to open file!");
fwrite($myfile,',');
$data = array("Status3"=>'0');

$Jdata = json_encode($data);

fwrite($myfile, $Jdata);
fwrite($myfile,'] }');
fclose($myfile);

?>