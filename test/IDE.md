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
