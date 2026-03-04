Hei, dette er vår ide om hva bilen vår skal få til!

1. Følge en linje - både rett frem, men også svinge
2. Observere hindringer i veien - stoppe opp (evt. kjører forbi) 
3. Skjermen skal vise batteristatus og hvor langt bilen kan     kjøre før den må lades 

Plan: 
- En while løkke bilen er låst i (kontinuerlig kjøre calibrate funksjon), så lenge den er i while løkken skal bilen spinne rundt sin egen akse, når den hopper ut av while-løkken skal bilen følge en linje
- En del av koden skal få bilen til å følge en linje (hva skal motoren gjøre om verdiene er mindre/større/lik 2000?)
- En del av koden måp beregne batteristatus og beregne hvor langt bilen kan kjøre før den må lade 
- En del av koden styrer hva som står på skjermen 
- En del av koden observerer hindringer og får bilen til å stoppe opp (Bilen må ikke bråbremse, da dette ødelegger motoren)

Motor: 
- LOW er forover, HIGH er bakover
- Digital pin 15 - kontrollerer den høyre motoren 
- Digital pin 16 - kontrollerer venstre motor 

Ideer til fortsettelsen på prosjektet: 
Rasberry Pi:
- Informasjon blir lagret i "skyen", klarer vi å for eksempel lagre distansen bilen har kjørt,  batteribruk eller klarer å sende ut en varsling når den hører på hindringer
- Kan vi sende kommandoer fra Rasberry Pi til bilen i nåtid?
Hindring: 
- Få zumo-bilen til å stoppe opp for hindringer (evt. at den klarer å kjøre forbi/snu 180 grader)
Høyreregel:
- Om vi får til å koble opp sensorer på siden av bilen, er planen å få bilen til å stoppe opp for andre biler som kommer kjørende fra høyre 
Rødt/grønt lys: 
- IR-sensorene klarer å se forskjell på rødt og grønt - om vi holder et rødt ark foran bilen skal den forstå at den skal stoppe opp 
Fartsendring: 
- Bilen skal ut ifra linjene (fargekoder/strekkoder) på bakken forstå når den skal øke/senke farten 
Skjerm: 
- Står på skjermen når den møter på hindringer/rødt lys


--------------------------------------------------------------------------------------------------------------------------------------------

Målet i kjernedelen er et fungerende, distribuert Smart City-system med lokale robotfunksjoner og skybasert overvåkning/styring.

Hver robot håndterer linjefølging og hinderunngåelse lokalt. AVR kjører hard sanntidsaktuering og sikkerhet, mens ESP32 håndterer lokal beslutning og planlegging. På skysiden kjører Raspberry Pi Mosquitto og Node-RED. Robotene publiserer telemetri over MQTT, og Node-RED viser sanntidsstatus i dashboard og kan sende enkle overstyringskommandoer (GO/HOLD/STOP). Mosquitto er kun broker, Node-RED kjører skylogikk.

Navigasjon modelleres som en graf: kryss og lader er noder, og teipveier er kanter. Grafen ligger på ESP32 som node-/kanttabell (med kostnad og manøvertype), og rute beregnes node-til-node (f.eks. A*). Roboten får en destinasjonsnode og følger ruten kant-for-kant.
ROAD_MODE: følg teip.
NODE_MODE: ved stopplinje/kryss, les AprilTag for node-ID og velg neste kant.

Teip brukes for kjøreadferd på vei (stopp/sakte/normal/hurtigsoner), mens AprilTags brukes for nodeidentitet og absolutt referanse ved beslutningspunkter. Roboten trenger ikke kjøre over taggen; taggene plasseres i et nodefelt som ligger innenfor det nedovervendte kameraets synsfelt ved stoppposisjon.

Når roboten kjører inn i en tilnærmingssone for et kryss, leser ESP32 kryss-tag og oppdaterer lokal pose, og ber om passeringstillatelse. Hvis GO kommer i tide og banen er fri, passerer roboten kontrollert uten full stopp. Hvis HOLD, eller manglende svar innen x ms før stopplinje, stopper den og venter. Ved skyutfall brukes lokal fallback (høyreregel/nærhetsregel). Etter passering publiseres kryss-forlatt-status over MQTT.

Batteri er simulert og estimeres fra encoder-basert distanse. Ved lavt batteri rutes roboten til ladesonen. Ankomst bekreftes med AprilTag; ESP32 sjekker avstand og orientering mot dockingmål og sender deretter stoppkommando til AVR. Ladetilstand holdes til 80 %, og batteri-/ladestatus publiseres til MQTT og vises i Node-RED.

Styringsprioritet på roboten bør være: nødstopp > hinderstopp > lading > kryssbeslutning > normal linjefølging

hvordan den skal kjøre i kryss
fysisk trafikk lys styrt av node-red (bare for visualisering)
realistisk simulering av batteri
starte på parkeringsplass for å kunne velge hvor den skal kjøre i starten
endring på fart med forskjellige teip som detekteres av linjesensorene
finne ut om sensor for hindringer fungerer
planlegging av hvordan byen skal se ut, de forskjellige stedene
finne ut om velocity control og lokale regler for traffikk fungerer
legge til skjerm på zumoene for å vise deler av informasjonen i node-red lokalt
finne ut om alle tre zumoene fungerer og har det de trenger