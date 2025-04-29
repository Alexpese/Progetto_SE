# Accordatore Fai Da Te
Il progetto consiste nella costruzione di un accordatore per chitarra (di default si intende accordata a 440Hz (nota A4))

<h2>Descrizione</h2>
All'accensione dell'accordatore, viene presa come nota iniziale il Mi alto (circa 329 Hz) e il microfono inzia ad "ascoltare", in base all'accordatura
della corda (una volta ricevuto il segnale dal microfono e elaborato) viene acceso un led che indica se la corda è da allentare, tirare oppure la nota è adatta
(Una frequenza viene considerata adatta quando è in un certo range (entro 1Hz di differenza massima)); se la nota non è adatta viene acceso un led rosso (il led viene scelto in base all'operazione da effettuare: uno se nota troppo bassa, l'altro se nota troppo alta), altrimenti viene acceso il led verde
Per cambiare nota (e quindi corda) si possono usare i due pulsanti (uno per "scendere" e uno per "salire"), quando viene cambiata la nota viene anche riprodotto il suono che corrisponde a questa e viene mostrato sullo schermo del led la nota e il numero di corda corrispondente
(Siccome il rilevamento della nota dovrebbe essere continuo, il cambio di nota potrebbe essere gestito similmente ad un interrupt)

<h2>Componenti</h2>
- Microfono (Max9814 o Max4466)<br>
- Display LCD (TC1602A-1T)<br>
- Altoparlante (Passive Buzzer)<br>
- Pulsanti (x2)<br>
- Led (2 rossi e 1 verde)<br>
