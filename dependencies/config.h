#ifndef CONFIG_H
#define CONFIG_H

const QString fontType = "font-family: \"Times New Roman\", Times, serif;";
const QString fontSize = "font-size: 20px;";
const QString border = "border: 1px solid black;";
const QString globalTextAttributes = fontType + fontSize + border;

const QString roomsXmlPath = "../Nav/dependencies/rooms.xml";
const QString projectTitleLabelText = "<project title>";
const QString projectTitleLabelStyle  = "background-color: yellow;";
const QString windowTitleText  = "<window title>";
const QString comboKey = "name";
const QString currentLocationLabelText = "I am here: ";
const QString currentLocationLabelStyle = "background-color: green;";
const QString destinationLabelText = "Take me to: ";
const QString destinationLabelStyle = "background-color: red;";
const QString logLabelText = "directions";
const QString logLabelStyle = "background-color: blue;";
const QString logStyle = "background-color: grey;";

#endif // CONFIG_H
