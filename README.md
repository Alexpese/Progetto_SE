# <h1>Accordatore Fai Da Te</h1>
Il progetto consiste nella costruzione di un accordatore per chitarra (di default si intende accordata a 440Hz (nota A4))

<h2>Descrizione</h2>
All'accensione dell'accordatore, viene presa come nota iniziale il Mi basso (circa 82,41 Hz) e il microfono inzia ad "ascoltare", in base all'accordatura della corda (una volta ricevuto il segnale dal microfono e elaborato) viene acceso un led che indica se la corda è da allentare, tirare oppure la nota è adatta (una frequenza viene considerata adatta quando è in un certo range (+- 5Hz)); se la nota non è adatta viene acceso un led rosso (il led viene scelto in base all'operazione da effettuare: uno se nota troppo bassa, l'altro se nota troppo alta), altrimenti viene acceso il led verde.
Per cambiare nota (e quindi corda) si usa un pulsante, che cambia la nota (e quindi la corda) a salire, per poi ripartire dal Mi basso (l'accordatura si intende quella standard: E2, A2, D3, G3, B3, E4)

Il rilevamento della frequenza è effettuato tramite autocorrelazione poichè (dopo aver effettuato delle prove) usare FFT era poco efficace (non veniva rilevata la frequenza corretta (o quanto meno "adatta")).
A causa delle limitazioni dell'hardware (il microfono non è professionale e ha dimensioni ridotte) sono state effettuate delle modifiche a livello software sulle frequenze rilevate (ad alcune note è stata adeguata la frequenza, comparando la frequenza rilevata con il microfono con quella della nota suonata (da una chitarra già accordata)

L'accordatore ha incorporato un metronomo che, in base al tempo scelto (tempo di base 60BPM), emette un suono (in questo caso la nota DO) con una certa frequenza. Il tempo può essere cambiato tramite un tastierino numerico (Membrane Switch), tramite una sequenza di 3 numeri (le lettere e i caratteri speciali non vengono rilevati) che corrisponde al tempo BPM desiderato. 

<h2>Componenti</h2>
- Microfono (Max9814): Usato per la rilevazione dell'accordatura dello strumento<br>
- Display LCD (TC1602A-1T): Usato per l'interazione con l'utente, mostra (stampato a video) la corda che si sta accordando (e la nota corrispondente)<br>
- Altoparlante (Passive Buzzer): Usato per la riproduzione nella nota corrente a cui si sta effettuando la comparazione (durante l'accordatura) e per riprodurre il battito del metronomo<br>
- Pulsanti (x2)<br>
- Led (2 rossi e 1 verde)<br>
- Arduino Mega R3 (Scelto per la necessità di avere più pin di un Aruino Uno)<br>
- Membrane Switch: Usato per la scelta del tempo del metronomo<br>
