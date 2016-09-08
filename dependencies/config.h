#ifndef CONFIG_H
#define CONFIG_H

const QString fontType = "font-family: \"Times New Roman\", Times, serif;";
const QString fontSize = "font-size: 20px;";
const QString border = "border: 1px solid black;";
const QString globalTextAttributes = fontType + fontSize + border;

const QString roomsXmlPath = "../Nav/dependencies/rooms.xml";
const QString projectLogo = "../Nav/dependencies/LOGO.JPG";

const QString projectTitleLabelText = "Navigation in Jacobs";
const QString projectTitleLabelStyle  = "background-color: yellow;";
const QString windowTitleText  = "Created By Adam Anan & Gilad Eini";
const QString currentLocationLabelText = "I am here: ";
const QString currentLocationLabelStyle = "background-color: green;";
const QString destinationLabelText = "Take me to: ";
const QString destinationLabelStyle = "background-color: red;";
const QString logLabelText = "Directions";
const QString logLabelStyle = "background-color: blue;";
const QString logStyle = "background-color: grey;";
const QString groupBoxPrefStyle = "";
const QString groupBoxViewByStyle = "";
const QString resetButtonStyle = "";
const QString filterButtonStyle = "";
const QString goButtonStyle = "";
const QString locationCBsStyle = "";
const QString floorsNumbersWidgetStyle = "";
const QString floorsNumbersWidgetHeader = "See only the following floors:";

const QString baseFloor = "3"; //used in the filter. come in use when user uncheck all floors

#endif // CONFIG_H


//const QString roomsXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/rooms.xml";
//const QString edgesXmlPath = "C:/Users/Adam/Studies/Robotics Lab/Nav/dependencies/edges.xml";
