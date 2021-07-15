<?php
$myfile1 = fopen("Device1.txt", "r") or die("Unable to open file!");
$myfile2 = fopen("Device2.txt", "r") or die("Unable to open file!");
$myfile3 = fopen("Device3.txt", "r") or die("Unable to open file!");
echo fgets($myfile1);
echo fgets($myfile2);
echo fgets($myfile3);
fclose($myfile1);
fclose($myfile2);
fclose($myfile3);

?>