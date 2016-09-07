#ifndef CONFIG_H
#define CONFIG_H

const QString fontType = "font-family: \"Times New Roman\", Times, serif;";
const QString fontSize = "font-size: 20px;";
const QString border = "border: 1px solid black;";
const QString globalTextAttributes = fontType + fontSize + border;

const QString roomsXmlPath = "../Nav/dependencies/rooms.xml";
const QString edgesXmlPath = "../Nav/dependencies/edges.xml";
//const QString roomsXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/rooms.xml";
//const QString edgesXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/edges.xml";
const QString videoTest = "../Nav/dependencies/fromDanToLab.avi";
const QString picTest = "../Nav/dependencies/Capture.JPG";

const QString projectTitleLabelText = "<project title>";
const QString projectTitleLabelStyle  = "background-color: yellow;";
const QString windowTitleText  = "<window title>";
const QString currentLocationLabelText = "I am here: ";
const QString currentLocationLabelStyle = "background-color: green;";
const QString destinationLabelText = "Take me to: ";
const QString destinationLabelStyle = "background-color: red;";
const QString logLabelText = "directions";
const QString logLabelStyle = "background-color: blue;";
const QString logStyle = "background-color: grey;";
const QString groupBoxPrefStyle = "";
const QString groupBoxViewByStyle = "";
const QString resetButtonStyle = "";
const QString filterButtonStyle = "";
const QString goButtonStyle = "";
const QString locationCBsStyle = "";
const QString floorsNumbersWidgetStyle = "";
const QString floorsNumbersWidgetHeader = "see only floors number:";

const QString baseFloor = "3"; //used in the filter. come in use when user uncheck all floors

#endif // CONFIG_H

//const QString roomsXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/rooms.xml";
//const QString edgesXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/edges.xml";
