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
  "<h3> DATA must be less than 10 characters"
  "<FORM action=\"/\" method=\"post\">"
  "<P>"

  "<label>Data 1 :&nbsp;</label><input maxlength=\"10\" name=\"drum\">"
  "<label>  Data 2 :&nbsp;</label><input maxlength=\"10\" name=\"O2\"><br>"

  "<label>Data 3 :&nbsp;</label><input maxlength=\"10\" name=\"shs_temp\">  <value var =\" document.getElementById('IP').value\">"
  "<label>  Data 4 :&nbsp;</label><input maxlength=\"10\" name=\"shs_prs\"><br>"


  "<label>Data 5:&nbsp;</label><input maxlength=\"10\" name=\"fr1_temp\">  <value var =\" document.getElementById('IP').value\">"
  "<label> Data 6 :&nbsp;</label><input maxlength=\"10\" name=\"fr2_temp\"><br>"


  "<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
  "</P>"
  "</FORM>"
  "</body>"
  "</html>";

