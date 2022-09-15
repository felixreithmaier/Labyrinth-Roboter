/*
    Code fur den zweiten Roboter, der das Labyrinth mit den Daten vom ersten Roboter durchfahrt.
*/

//Anschlusse
#define MOTOREN OUT_BC
//Settings
#define POWER 40
#define DISTANCE 360*2.2
#define INBOX 5
//Defines fur Lesbarkeit.
#define OBEN 1
#define RECHTS 2
#define UNTEN 3
#define LINKS 4
#define EndOfLine 5
#define RECHTS_DREHEN 100
#define LINKS_DREHEN -100
#define CONVERT_ASCII(x) (x-48)

//Vars
string befehlskette = "";
int richtung = OBEN; //Startrichtung

//Funktion Headers.
void Drehen(int drehwert);
void DrehenMitFeld(int drehwert);
int PfeilDrehen(int nachRechts, int anzahlDerUmdrehungen);
void Richtunganpassen(int sollRichtung);
void Fahren();
void BluetoothCheck(int connection);
void ErgbenisEmpfangen();


void Drehen(int drehwert) {
    //90 Grad Drehung nach rechts durchfuhren.

    //Drehen
    RotateMotorEx(MOTOREN, POWER, 175, drehwert, true, true);
}

void DrehenMitFeld(int drehwert) {
    //Drehung ausfuhren und Richtung anpassen.

    Drehen(drehwert);
    Wait(500);

    //Richtung anpassen.
    richtung = PfeilDrehen((drehwert==RECHTS_DREHEN), 1);
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

void Richtunganpassen(int sollRichtung) {
    //Passt die Richtung an die angegebene Richtung an.

    //Richtungen durchchecken mit virtellerPfeil und dann dementsprechend Drehen.
    int virtuellerPfeil;
    if (richtung == sollRichtung) {
        //Gerade aus.
        return;
    }

    if(PfeilDrehen(1, 1) == sollRichtung) {
        //Nach rechts.
        DrehenMitFeld(RECHTS_DREHEN);
        return;
    }


    if(PfeilDrehen(1, 2) == sollRichtung) {
        //Nach hinten.
        DrehenMitFeld(RECHTS_DREHEN);
        DrehenMitFeld(RECHTS_DREHEN);
        return;
    }

    //Nach links.
    DrehenMitFeld(LINKS_DREHEN);
}

void Fahren() {
    //Ein Tile nach vorne fahren.

    //Vorwartsfahren
    RotateMotorEx(MOTOREN, POWER, DISTANCE, 0, true, true);
}

void BluetoothCheck(int connection) {
    //Testet die Bluetoothverbindung.
    if (!BluetoothStatus(connection) == NO_ERR) {
        TextOut(5, LCD_LINE2, "Error");
        Wait(1000);
        Stop(true);
    }
}

void ErgbenisEmpfangen() {
    //Empfangt Befehlskette.
    while(!strcmp(befehlskette, "")){
        BluetoothCheck(0);
        ReceiveRemoteString(INBOX, true, befehlskette);
    }
    ResetScreen();
    TextOut(5, LCD_LINE2, befehlskette);
    Wait(400);
    PlayTone(880, 1000);
    Wait(1100);
}

void Displayausgabe(int i) {
    ResetScreen();

    TextOut(10, LCD_LINE1 , "i:");
    NumOut(10, LCD_LINE2 , i);

    TextOut(10, LCD_LINE3 , "befehlskette[i]:");
    NumOut(10, LCD_LINE4, CONVERT_ASCII(befehlskette[i]));

    TextOut(10, LCD_LINE7 , "richtung:");
    NumOut(10, LCD_LINE8 , richtung);

}

task main() {
    ErgbenisEmpfangen();



    //Befehlskette abfahren.
    int i = 0;
    while(CONVERT_ASCII(befehlskette[i]) != EndOfLine) {
        Displayausgabe(i);

        Richtunganpassen(CONVERT_ASCII(befehlskette[i]));
        Fahren();
        i++;
    }
    PlayTone(880, 1000);
    Wait(1100);
}