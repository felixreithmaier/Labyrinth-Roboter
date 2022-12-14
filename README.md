# Labyrinth Roboter

## Einleitung

Im Rahmen des Studiums an der Technischen Hochschule Deggendorf müssen
wir im 4. Semester ein Projekt belegen, welches 4 Semesterwochenstunden
beansprucht. Wir haben uns hierbei für eine Aufgabe mit dem Lego
Mindstorms Roboter, welcher ein Labyrinth durchfahren muss, entschieden.

### Aufgabenstellung

Folgende Aufgabenstellung wurde von den Professoren vorgegeben:

> Programmierung zweier Legoroboter, bei denen einer ein Labyrinth
> erkundet, einen Weg durch das Labyrinth findet, die Strecke optimiert
> und die optimierte Strecken an einen zweiten Roboter per Bluetooth
> übermittelt. Der zweite Roboter fährt auf der optimierten Strecke
> durch das Labyrinth.

Ein Ansprechpartner des Projektes gab uns zu Beginn des Projekt eine Einführung zur NXC (Not
eXactly C) Software und wie diese zu installieren ist. Ebenso wurde auf
die Dokumentation von NXC  hingewiesen, welche uns jedoch im Verlauf des
Projekts noch einige Probleme bereiten wird. Uns wurde gezeigt, an
unterschiedlichen Stellen platziert werden.

### Eigene Interpretation der Aufgabenstellung

Die Aufgabenstellung lässt noch Interpretationsspielraum offen. Folgende
Punkte werden von der Anforderung noch nicht genau definiert:

  - Der Such-Algorithmus der verwendet werden soll

  - Startposition der Roboter

  - Mit welchen Sensoren soll der Roboter das Labyrinth erkunden

Als passenden Such-Algorithmus wählten wir die Rechte-Hand-Methode.
Dieser folgt der rechten Wand, bis der Ausgang erreicht wird.

Die Startposition ist bei uns frei wählbar, da der Eingang des
Labyrinthes veränderbar ist. Der Nutzer soll zu Beginn des Programms
eine Startposition eingeben können, damit der Roboter weiß, auf welcher
Position er sich befindet.

Bei der Wahl der Sensoren sind wir durch durch die vorhandenen vier
Sensor-Steckplätze des NXT eingeschränkt. Um Wände erkennen zu können,
nutzen wir Ultraschallsensoren.

## Methodik

Im Folgenden wird der Aufbau der beiden Roboter und die Vorgehensweiße
erklärt.

### Aufbau

Aufbau von Roboter 1

<img src="images/roboter1.jpg" width="400">

Aufbau von Roboter 2

<img src="images/roboter2.jpg" width="400">


Beide zu verwendende Roboter sind auf Grundlage der offiziellen Lego NXT
Anleitung  aufgebaut. Diese eignen sich optimal um das Fahren der
Roboter zu ermöglichen. Besonders hilfreich ist das kleine Stützrad, um
enge Kurvenradien zu ermöglichen, was im Labyrinth hilfreich ist.

Bei Roboter 1 wird der Standardaufbau leicht verändert, um mehrere
weitere Bauteile anbringen zu können. Hier befinden sich, wie in
gezeigt; links, rechts und vorne am Roboter Ultraschallsensoren. Diese
messen den Zentimeterabstand \(0\) bis \(255\) vom jeweiligen Sensor bis
zur Labyrinthwand.

Da Roboter 2 keine Sensoren benötigt, wird der Standardaufbau, wie in zu
sehen, nicht verändert.

### Logik

Im Weiteren wird die Logik genauer erklärt. Dabei wird gezeigt, wie ein
Weg im Labyrinth gefunden wird und wie der Algorithmus im Roboter
umgesetzt wird.

#### Lösungsalgorithmus

Rechte-Hand-Methode - Kreisproblem

<img src="images/rechtehand-circle.PNG" width="400">


Wie in der Einleitung bereits erwähnt, wird als Algorithmus die
Rechte-Hand-Methode verwendet. Diese setzt voraus, dass das Labyrinth
einen Eingang und einen Ausgang hat. Bei dieser Methode erkennt der
Roboter die Wände um ihn und folgt der Wand, die sich rechts aus Sicht
des Roboters befindet. Der Algorithmus kann jedoch versagen, wenn man,
wie in zu sehen, im inneren des Labyrinthes startet, und sich dort
logisch ein "Kreis" befindet.

Rechte-Hand-Methode - Startposition außen

<img src="images/rechtehand.PNG" width="400">


Um dieses Problem zu vermeiden startet unser Roboter an einer
Randposition, da die Außenwand zusammenhängend ist. So kann keine
Situation entstehen, bei der der Roboter in einer Endlosschleife
gefangen ist. Ein Beispiel wird in gezeigt.


ohne Optimierung

<img src="images/rechtehand-feld.PNG" width="400">


mit Optimierung

<img src="images/rechtehand-optimiert.PNG" width="400">


Auch wenn der Algorithmus bereits immer eine Lösung findet, kann dieser
noch optimiert werden. Wie in der rechts unten zu sehen, gibt es
Positionen, die mehrmals durchfahren werden. Um dies zu vermeiden
speichert der Roboter Pfeile im Feld, die die Richtung des letzten
Durchquerens darstellen. (siehe )

Zusätzlich werden bei unserem Algorithmus manche Wege optimiert. Hierzu
prüft der Roboter bei offenen Wänden, ob ein Pfeil im Nachbarfeld zu ihm
"hingedreht" werden kann. Dies in der zu sehen.

#### Roboterlogik

Um diesen Algorithmus als Roboter umsetzen zu können, benutzen wir eine
Schleife, die einen Code so lange wiederholt, bis der Roboter einen
Ausgang gefunden hat. Die Startposition muss vom Nutzer eingegeben
werden und wird dann als aktuelle Position gesetzt. Der beschreift diese
Vorgehensweiße. Es wird auch klar, dass der Roboter einen Ausgang
gefunden hat, falls sich seine eigene Position sich nicht mehr im Feld
befindet.

Um zu wissen, in welche Richtung sich der Roboter drehen muss, nutzen
wir die Ultraschallsensoren. Diese messen den Abstand zu den Wänden.
Falls diese eine bestimmte Mindestdistanz betragen, kann davon
ausgegangen werden dass sich in diese Richtung keine Wand befindet. Die
jeweilige Drehrichtung kann aus der entnommen werden.


| Sensor links       | Sensor vorne      | Sensor rechts    | Aktion        |
| :--------- | :--------- | :--------- | :-------------------- |
| \-         | \-         | keine Wand | nach Rechts drehen    |
| \-         | keine Wand | Wand       | Keine Drehung         |
| keine Wand | Wand       | Wand       | nach Links drehen     |
| Wand       | Wand       | Wand       | 2x nach Rechts drehen |

Entscheidungstabelle



Zusätzlich wird der verwendet, um den Weg wie in zu optimieren. Dabei
wird an jeder neuen Position zuerst ermittelt, ob sich Wände um den
Roboter befinden. Ist dies nicht der Fall, so setzt der Roboter einen
Pfeil am jeweiligen Nachbarfeld, der zum Roboter zeigt.

## Ergebnis

Nun wird ein Test durchgeführt. Das Labyrinth wird so aufgebaut, wie es
in den Beispielen aus gezeigt wurde. Der finale Aufbau ist in zu sehen.

Labyrinth Beispielaufbau Draufsicht

<img src="images/labyrinth-komplett.jpg" width="400">


Nach Durchlaufen des ersten Programms wird überprüft, ob dieser alle
Felder richtig abspeichert. Alle Ergebnisse werden vor dem Übertragen
per Bluetooth am Display ausgegeben. Das Ausgegebene Feld-Array wird in
gezeigt.


| <span style="color: blue">4</span> |  <span style="color: blue">4</span>  |                 3                  |                 4                  | 4 |
| ------------ | ---------------- |---------------- |---------------- |---------------- |
|                 0                  |  <span style="color: blue">1</span>  |                 3                  |                 2                  | 1 |
|                 2                  |  <span style="color: blue">1</span>  | <span style="color: blue">4</span> | <span style="color: blue">4</span> | 1 |
|                 0                  | <span style="color: orange">2</span> | <span style="color: blue">2</span> | <span style="color: blue">1</span> | 1 |

Richtungen im Array


| **Richtung** | **Zahl im Feld** |
| -----------: | :--------------- |
|       Nichts | 0                |
|         Oben | 1                |
|       Rechts | 2                |
|        Unten | 3                |
|        Links | 4                |
|    Code Ende | 5                |

Richtungen Legende


Nun kann mittels der der abgespeicherte Weg in identifiziert werden.
Achtet man in auf die orange eingefärbte Startposition und auf die blau
eingefärbten Felder, so wird klar, dass der Roboter 1 die Richtigen
korrekt ermittelt hat.

Somit wird anschließend der Weg im Programm virtuell durchlaufen, um die
zu übergebende Zeichenkette ermitteln zu können. Diese endet mit der
Zahl \(5\), damit der zweite Roboter weiß, dass er am Ende angekommen
ist. Folgendes überträgt der Roboter an den zweiten Roboter:
\["2214411445"\]

Der zweite Roboter muss dann manuell auf der Startposition vom ersten
Roboter platziert werden. Anschließend fährt dieser die Richtungen der
Zeichenkette ab. Der optimierte Weg ist dann auch ersichtlich kürzer als
der des ersten Roboters.

## Fazit

Im Schluss soll noch auf Probleme während des Arbeitens und auf
Verbesserungsmöglichkeiten eingegangen werden.

### Probleme

Probleme ergaben sich beim Programmieren des Roboters. Nicht nur die
Mechanik des Roboters, sondern auch die Programmiersprache NXC hat uns
oft Sorgen bereitet

#### 90-Grad Drehungen

Im Programmcode sind einige Funktionen gegeben, die eine Drehung des
Roboters ermöglichen. Jedoch kann keine dieser sicherstellen, dass der
Roboter genau 90-Grad Kurven fahren kann. Auch wenn die Abweichung bei
den Kurven nur sehr minimal ist, summieren sich die Fehler und beim
Durchfahren des Labyrinths muss die Drehrichtung des Roboters immer
wieder manuell angepasst werden.

#### Double Array

Ein weiteres großes Problem trat beim Speichern vom Double-Array auf.
Hier gab der Roboter immer seltsame Werte zurück, wenn man auf diese
zugreifen wollte. Letztendlich mussten wir das zweidimensionales Array
in einem eindimensionalen Arrays speichern, da ein zweidimensionales
nicht funktionierte. Auf den Fehler wurden wir erst durch eine Eintrag
im Stackoverflow Forum aufmerksam.  Hier schrieb ein Nutzer, dass
zweidimensionale Arrays in NXC nicht existieren. Was jedoch sehr seltsam
ist, da diese laut offizieller Dokumentation  funktionieren müssten.

### Verbesserungsmöglichkeiten

Die vorher erwähnten Probleme bei den 90-Grad Drehungen könnten
verbessert werden. Eine Möglichkeit hierbei wäre das Einbauen eines
Gyrosensors. Dabei könnte man während des Drehen des Roboters
kontrollieren, ob dieser die Drehung perfekt ausgeführt hat, und falls
nicht, diese anpassen.