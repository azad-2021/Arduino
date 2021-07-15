const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta content=\"text/html; charset=ISO-8859-1\""
  " http-equiv=\"content-type\">"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>Wifi Configuration</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<h1>INPUT DATA </h1>"
  "<h3> DATA must be less than 4 characters"
  "<FORM action=\"/\" method=\"post\">"
  "<P>"

  "<label>Drum level :&nbsp;</label><input maxlength=\"4\" name=\"drum\">"
  "<label>  O2 :&nbsp;</label><input maxlength=\"4\" name=\"O2\"><br>"

  "<label>SHS Temp :&nbsp;</label><input maxlength=\"4\" name=\"shs_temp\">  <value var =\" document.getElementById('IP').value\">"
  "<label>  SHS Pressure :&nbsp;</label><input maxlength=\"4\" name=\"shs_prs\"><br>"


  "<label>Furnace 1 Temp :&nbsp;</label><input maxlength=\"4\" name=\"fr1_temp\">  <value var =\" document.getElementById('IP').value\">"
  "<label>  Furnace 2 Temp :&nbsp;</label><input maxlength=\"4\" name=\"fr2_temp\"><br>"


  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>"
  "</body>"
  "</html>";

