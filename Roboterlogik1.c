/*
    Code fur den ersten Roboter, der das Labyrinth erkundet und dann einen Weg uber Bluetooth an den zweiten Roboter schickt.
*/

//Anschlusse
#define MOTOREN OUT_BC

#define SENSOR_RECHTS IN_4
#define SENSOR_VORNE IN_1
#define SENSOR_LINKS IN_3

//Settings
#define MINDESTABSTAND 22
#define POWER 40
#define DISTANCE 360*2.2
#define BLUETOOTH_CONNECTION 1
#define OUTBOX 5
//Defines fur Lesbarkeit.
#define MAXIMAL_SIZE 9
#define NICHTS 0
#define OBEN 1
#define RECHTS 2
#define UNTEN 3
#define LINKS 4
#define EndOfLine 5
#define RECHTS_DREHEN 100
#define LINKS_DREHEN -100

//Vars
float sensorRechts;
float sensorVorne;
float sensorLinks;
string befehlskette;
int feld[MAXIMAL_SIZE*MAXIMAL_SIZE];
int richtung = OBEN; //Startrichtung
int posY = 0; //Momentane Y-Position vom Roboter.
int posX; //Momentane X-Position vom Roboter.
int startpos;
int SIZE_X;
int SIZE_Y;


//Fuktion Headers.
void printFeld();
int AbfrageNachStart();
void Displayausgabe();
void Setup();
void Drehen(int drehwert);
void DrehenMitFeld(int drehwert);
int PfeilDrehen(int nachRechts, int anzahlDerUmdrehungen);
void Fahren();
int FahrenMitFeld();
void VirtuellenPfeilAnwenden(int virtuellerPfeil);
void Zyklus();
void BefehlsketteErstellen();
void BluetoothCheck(int connection);
void ErgbenisUbertragen();


int AbfrageNachStart() {
    //Abfrage fuer Feld Groesse (SIZE_X, SIZE_Y)

    ResetScreen();
    TextOut(16, LCD_LINE1 , "Feld X Groesse", false);
    TextOut(16, LCD_LINE2 , "auswaehlen", false);

    SIZE_X = 5;
    while(true){
        NumOut(48, LCD_LINE5, SIZE_X);

        //INPUT-Eingabe
        if (ButtonPressed(BTNRIGHT,false) && SIZE_X < MAXIMAL_SIZE){
            SIZE_X += 1;
            Wait(400);
        }else if (ButtonPressed(BTNLEFT, false) && SIZE_X > 1){
            SIZE_X -= 1;
            Wait(400);
        }else if (ButtonPressed(BTNCENTER, false)){
            break;
        }
    }
    Wait(400);

    ResetScreen();
    TextOut(16, LCD_LINE1 , "Feld Y Groesse", false);
    TextOut(16, LCD_LINE2 , "auswaehlen", false);

    SIZE_Y = 4;
    while(true){
        NumOut(48, LCD_LINE5, SIZE_Y);

        //INPUT-Eingabe
        if (ButtonPressed(BTNRIGHT,false) && SIZE_Y < MAXIMAL_SIZE){
            SIZE_Y += 1;
            Wait(400);
        }else if (ButtonPressed(BTNLEFT, false) && SIZE_Y > 1){
            SIZE_Y -= 1;
            Wait(400);
        }else if (ButtonPressed(BTNCENTER, false)){
            break;
        }
    }
    Wait(400);

    //Abfrage fuer den Startpunkt (0-4)

    ResetScreen();
    int startfeld = 0;
    TextOut(16, LCD_LINE1 , "Startpunkt (X-Seite, Y=0)", false);
    TextOut(16, LCD_LINE2 , "auswaehlen", false);

    while(true){
        NumOut(48, LCD_LINE5, startfeld);

        //INPUT-Eingabe
        if (ButtonPressed(BTNRIGHT,false) && startfeld < SIZE_X-1){
            startfeld += 1;
            Wait(400);
        }else if (ButtonPressed(BTNLEFT, false) && startfeld > 0){
            startfeld -= 1;
            Wait(400);
        }else if (ButtonPressed(BTNCENTER, false)){
            break;
        }
    }

    //Ausgabe Bestatigung und CountDown.
    TextOut(16, LCD_LINE2, "Vielen Dank", true);
    TextOut(16, LCD_LINE3, "Starte in..", false);
    for(int i=3; i>=0; i--){
            NumOut(48, LCD_LINE5 , i);
            if(i > 0){
                PlayTone(440, 550);
                Wait(1000);
            }else{
                PlayTone(880, 1000);
                Wait(1100);
            }
    }

    Wait(1000);
    ResetScreen();
    return startfeld;
}

void Displayausgabe() {
    ResetScreen();

    TextOut(1, LCD_LINE1 , "Sensor Links:", false);
    TextOut(1, LCD_LINE3 , "Sensor Vorne:", false);
    TextOut(1, LCD_LINE5 , "Sensor Rechts:", false);
    TextOut(1, LCD_LINE7 , "Position:", false);

    NumOut(1, LCD_LINE2 , SensorUS(SENSOR_LINKS));
    NumOut(1, LCD_LINE4 , SensorUS(SENSOR_VORNE));
    NumOut(1, LCD_LINE6 , SensorUS(SENSOR_RECHTS));
    NumOut(1, LCD_LINE8 , posY);
    NumOut(10, LCD_LINE8 , posX);

    NumOut(40, LCD_LINE8 , richtung);

}

void Setup() {
    //Settup fur drehen und Kordinatennetz.

    //Sensoren setzen.
    SetSensorLowspeed(SENSOR_VORNE);
    SetSensorLowspeed(SENSOR_RECHTS);
    SetSensorLowspeed(SENSOR_LINKS);

    //innit Kordinatennetz, Abfrage nach Startposition.
    for (int x = 0; x < SIZE_X; x++){
        for (int y = 0; y < SIZE_Y; y++) {
            feld[x+SIZE_X*y] = NICHTS;
        }
    }
    posX = AbfrageNachStart();
    startpos = posX;
}

void Drehen(int drehwert) {
    //90 Grad Drehung nach rechts durchfuhren.

    //Drehen
    RotateMotorEx(MOTOREN, POWER, 175, drehwert, true, true);
}

void DrehenMitFeld(int drehwert) {
    //Drehung ausfuhren und Richtung anpassen.

    Drehen(drehwert);

    //Richtung anpassen.
    richtung = PfeilDrehen((drehwert==RECHTS_DREHEN), 1);
}



void Fahren() {
    //Ein Tile nach vorne fahren.

    //Vorwartsfahren
    RotateMotorEx(MOTOREN, POWER, DISTANCE, 0, true, true);
}

int FahrenMitFeld() {
    //Nach vorne Fahren, neue Position merken und in Feld eintragen.

    //In Feld eintragen.
    feld[posX+posY*SIZE_X] = richtung;

    Fahren();

    //Position Akualisieren.
    switch (richtung) {
        case OBEN:
            posY++;
        break;
        case RECHTS:
            posX++;
        break;
        case LINKS:
            posX--;
        break;
        case UNTEN:
            posY--;
        break;
        default:
        break;
    }
}

void VirtuellenPfeilAnwenden(int virtuellerPfeil) {
    //Looperkennung muss erreichbare felder Markieren, diese funktion ist fur die Markierung da.

    //In die entgegengesetze Richtung von virtueller Pfeil gehen und dann in dieser Zelle einzeichnen.
    int pfeilPosX = posX;
    int pfeilPosY = posY;

    switch (virtuellerPfeil) {
        case OBEN:
            pfeilPosY--;
        break;
        case LINKS:
             pfeilPosX++;
        break;
        case UNTEN:
             pfeilPosY++;
        break;
        case RECHTS:
             pfeilPosX--;
        break;
        default:
    }

    if(!(pfeilPosX > (SIZE_X-1) || pfeilPosX < 0 || pfeilPosY > (SIZE_Y-1) || pfeilPosY < 0)){
         feld[pfeilPosX+(pfeilPosY*SIZE_X)] = virtuellerPfeil;
    }
}


void Zyklus() {
    //Zyklus fur ein Tile(Logik).

    //Sensoren auslesen.
    float sensorRechts = SensorUS(SENSOR_RECHTS);
    float sensorVorne = SensorUS(SENSOR_VORNE);
    float sensorLinks = SensorUS(SENSOR_LINKS);



    //Freie benachbarte Zellen markieren,
    //Wenn frei, entgegengesetzten Peil erstellen und dann eintragen.
    if (sensorRechts > MINDESTABSTAND) {
        //Rechts markieren.
        VirtuellenPfeilAnwenden(PfeilDrehen(0, 1)); //PfeilDrehen(int nachRechts, int anzahlDerUmdrehungen)
    }
    if (sensorLinks > MINDESTABSTAND) {
        //Links markieren.
        VirtuellenPfeilAnwenden(PfeilDrehen(1, 1));
    }
    if (sensorVorne > MINDESTABSTAND) {
        //Vorne markieren.
        VirtuellenPfeilAnwenden(PfeilDrehen(1, 2));
    }





    printFeld();

    //Fahren und speichern.
    if (sensorRechts > MINDESTABSTAND) {
        //Rechts abbiegen.
        DrehenMitFeld(RECHTS_DREHEN);
        Wait(400);
        FahrenMitFeld();
    } else if (sensorVorne > MINDESTABSTAND) {
        //Gerade aus fahren.
        FahrenMitFeld();
    } else if (sensorLinks > MINDESTABSTAND) {
        //Links abbiegen.
        DrehenMitFeld(LINKS_DREHEN);
        Wait(400);
        FahrenMitFeld();
    } else {
        //Umdrehen
        DrehenMitFeld(RECHTS_DREHEN);
        DrehenMitFeld(RECHTS_DREHEN);
        Wait(400);
        FahrenMitFeld();
    }




    printFeld();
}

int PfeilDrehen(int nachRechts, int anzahlDerUmdrehungen) {
    //Richtung anpassen.
    int variable = richtung;
    for(int umdrehung = 0; umdrehung < anzahlDerUmdrehungen; umdrehung++) {
        if (nachRechts) {
            if (variable < LINKS) {
                variable++;
            } else {
                variable = OBEN;
            }
        } else {
            if (variable > OBEN) {
                variable--;
            } else {
                variable = LINKS;
            }
        }
    }

    return variable;
}

void BefehlsketteErstellen() {
    //Befehlskette aus Feld erstellen fur zweiten Roboter.

    //Tempurare Vars.
    int virtualPosX = startpos;
    int virtualPosY = 0;

    while (!(virtualPosX > (SIZE_X-1) || virtualPosX < 0 || virtualPosY > (SIZE_Y-1) || virtualPosY < 0)) {
        //Richtung auslesen.

        befehlskette += NumToStr(feld[virtualPosX+SIZE_X*virtualPosY]);

        ResetScreen();
        TextOut(1, LCD_LINE1, "Position x, y:");
        NumOut(1, LCD_LINE2, virtualPosX);
        NumOut(20, LCD_LINE2, virtualPosY);

        Wait(500);

        //Position Akualisieren.
        switch (feld[virtualPosX+SIZE_X*virtualPosY]) {
            case OBEN:
                virtualPosY++;
            break;
            case RECHTS:
                virtualPosX++;
            break;
            case LINKS:
                virtualPosX--;
            break;
            case UNTEN:
                virtualPosY--;
            break;
            default:
            break;
        }
    }

    befehlskette += NumToStr(EndOfLine);


}

void BluetoothCheck(int connection) {
    //Testet die Bluetoothverbingung.
    if (!BluetoothStatus(connection) == NO_ERR) {
        TextOut(5, LCD_LINE2, "Error");
        Wait(1000);
        Stop(true);
    }
}

void ErgbenisUbertragen() {
    //Erstellt die Befehlskette und ubertragt diese.


    printFeld();


    BefehlsketteErstellen();





    Wait(1000);

    ResetScreen();
    TextOut(5, LCD_LINE2, befehlskette);
    Wait(1000);

    BluetoothCheck(BLUETOOTH_CONNECTION);

    SendRemoteString(BLUETOOTH_CONNECTION, OUTBOX, befehlskette);
    PlayTone(440, 550);
    Wait(1000);
}

void printFeld(){


    ResetScreen();

    for (int x = 0; x < SIZE_X; x++){
        for (int y = 0; y < SIZE_Y; y++) {
            NumOut(10*x, 10*y ,feld[x+SIZE_X*y]);
        }
    }

    Wait(400);
}

task main() {
    //Setup
    Setup();

    //Weg finden.
    while(!(posX > (SIZE_X-1) || posX < 0 || posY > (SIZE_Y-1) || posY < 0)) {

        Zyklus();
    }

    //Ubertragen
    ErgbenisUbertragen();
}