Sistem Inteligent de Monitorizare a Tunelului

Acest proiect implementeaza un sistem de monitorizare a unui tunel rutier utilizand un microcontroler Arduino Mega 2560 si senzori pentru detectia vehiculelor, a gazelor periculoase si a fumului. Sistemul este dezvoltat pentru a optimiza siguranta si eficienta traficului intr-un mediu controlat.
Caracteristici principale

    Monitorizarea traficului: Detecteaza intrarea si iesirea vehiculelor pe fiecare sens de circulatie, folosind senzori PIR.
    Numararea vehiculelor: Contorizeaza masinile prezente in tunel, cu o limita maxima de 3 vehicule pe sens.
    Alertare in caz de pericol: Detecteaza fum si gaze periculoase, activand automat un LED de alerta.
    Control manual al alertelor: Utilizatorul poate activa manual o stare de alerta printr-un buton de panica.
    Feedback vizual: LED-uri de diverse culori indica starea traficului si nivelul de siguranta.
    Monitorizare in timp real: Informatiile sunt afisate in Serial Monitor, permitand diagnosticarea rapida a starii tunelului.

Componente utilizate
Hardware

    Arduino Mega 2560 – microcontrolerul principal.
    Senzori PIR – pentru detectarea vehiculelor la intrare si iesire.
    Potentiometre – pentru simularea gazelor si fumului.
    LED-uri:
        Alb – indica prezenta vehiculelor in tunel.
        Verde – indica intrarea unei masini.
        Rosu – indica iesirea unei masini.
        Galben – semnaleaza o stare de alerta.
    Buton de panica – pentru activarea manuala a starii de urgenta.

Software

    Arduino IDE – pentru dezvoltare si upload-ul codului pe microcontroler.
    FreeRTOS – gestionarea multitasking a proceselor prin intermediul unui sistem de operare in timp real.
    Serial Monitor – pentru monitorizarea si diagnosticarea starii tunelului.

Structura aplicatiei

Sistemul functioneaza pe baza mai multor task-uri FreeRTOS, fiecare avand un rol bine definit:

    Entry Check Task – detecteaza intrarea vehiculelor si actualizeaza contorul.
    Exit Check Task – detecteaza iesirea vehiculelor si decrementeaza contorul.
    Incident Monitoring Task – verifica senzorii de gaz si fum, gestionand starea de alerta.
    Panic Button Task – monitorizeaza activarea manuala a unei alerte prin buton.
    Display Task – afiseaza constant informatii despre numarul de vehicule si alertele active.

Fluxul de functionare

    Cand un vehicul intra in tunel, senzorul PIR detecteaza miscarea si creste contorul vehiculelor.
    LED-ul alb ramane aprins cat timp exista vehicule in tunel.
    Daca numarul maxim de vehicule este atins, intrarea este blocata.
    In cazul detectarii fumului sau gazelor periculoase, se activeaza LED-ul galben de alerta.
    In cazul unei situatii de urgenta, utilizatorul poate activa butonul de panica pentru a alerta administratorii.
    Pe masura ce vehiculele ies din tunel, contorul se actualizeaza, iar LED-ul alb se stinge cand nu mai sunt vehicule in tunel.

Testare si validare

Proiectul a fost testat riguros in scenarii diverse pentru a asigura functionarea corecta:

    Detectarea vehiculelor: Senzorii PIR au fost calibrati pentru a evita detectarile false.
    Numararea precisa: Au fost implementate mecanisme pentru evitarea incrementarilor multiple nedorite.
    Starea de alerta: Pragurile senzorilor de gaz si fum au fost ajustate pentru a declansa corect LED-ul de alerta.
    Butonul de panica: A fost verificata activarea corecta si revenirea la starea normala dupa dezactivare.

Posibile imbunatatiri

    Adaugarea unui ecran LCD pentru afisarea locala a informatiilor despre trafic si alerte.
    Integrarea unui sistem de notificare prin SMS sau e-mail in caz de alerta.
    Extinderea functionalitatilor pentru monitorizarea tunelurilor cu mai multe benzi de circulatie.
