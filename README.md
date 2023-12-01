# CentraleAllarmeArduino
Centrale di allarme realizzata con arduino (arduino come mainboard)
Questo vuole essere solamente un progetto personale, non da intendere come sicuro e non mi assumo alcuna responsabilita di ogni danno derivato dall' utilizzo di questo progetto.
la mia esperienza, la centrale e ancora in funzione oggi a distanza di 3 anni.

Lista Hardware:
arduino uno r3
12v to 5v regulator (step down) (ho utilizzato il circuito di un alimentatore usb 5v per accendisigari auto, 12v).
1 IRF520 MOSFET Driver Module.
5 resistori 10k ohm (1 se non si usa internal pullup, il codice non considera input_pullup).
2 batterie leak acid 12v.
1 carica batterie idoneo per la leak acid, ne ho scelto uno che eroga 14v.
Sensori volumetrici 12v
Sirena per allarme 12v (settata con positivo a mancare, ergo se non riceve 12v utilizza la sua batteria leak acid interna per alimentare l' altoparlante).
Cavetteria, consiglio cavo per allarme 1x4, solitamente hanno 1 rosso e 1 nero da 0.75 e ulteriori 2 cavi a colore misto (magari verde e giallo) di sezione piu piccola, il cavo per allarme nello specifico e' schermato.
Un sistema di controllo remoto, io ho optato per una centralina radio universale 4ch esterna con radiocomando 433mhz (ATTENZIONE SCARSISSIMA SICUREZZA, OPTARE PER ALTRO SISTEMA), questa centralina governa 4 rele' che utilizzo come fossero pushbutton

Funzionamento generale:
CENTRALINA RADIO
ch1 attiva allarme
ch2 disattiva allarme o pulsante panico
ch3 pulsante panico (suona allarme forzatamente)


Il sensore volumetrico ha un suo stato N.A. se non alimentato, quando alimentato questo stato passa a N.C, quando rileva un movimento e per la durata del tale apre il contatto, noi intercettiamo questa apertura e cascasse il mondo non spegneremo l'allarme! :)

la sirena di allarme generalmente ha un selettore dipswitch per selezionare il funzionamento, ho settato il dipswitch per la modalita' suona con "positivo a mancare".

Montaggio:
nel montare la sirena ricorda di non allacciare la batteria interna, altrimenti... suona.

dopo aver posato i cavi e installato sirena e volumetrici nelle volute posizioni proseguire come segue.

i volumetrici vanno collegati, 12v cavo positivo 12v, comune o negativo, con comune o negativo scelto nel nostro cablaggio.
prendere dunque i 2 cavetti puliti (no alimentazione) e collegarli come si preferisce nei morsetti comune normalmente chiuso (chiuso se alimenato 12v e nessun rilevamento). 
Alla sirena collegare positivo e comune nei corrispettivi morsetti. se preferisci, utilizzare i 2 cavetti puliti per il tamper antimanomissione se presente nella scheda della sirena, ancora non collegare morsetti alla batteria interna

spostiamoci alla centrale arduino, prendere il caricabatteria 14v collegare il positivo al positivo dei volumetrici, il negativo al negativo dei volumetrici
collegate anche il negativo o comune della sirena
alimentare anche lo stepdown 12v-5v per alimentare il nostro arduino
collegare i 2 cavetti puliti del volumetrico in questa maniera 1 cavetto al 5v dell'arduino, l'altro cavetto al pin di lettura del volumetrico, puoi modificare il codice per la realizzazione di zone, il progetto ne implementa una sola, se vuoi comunque collegare molteplici volumetrici collega l'uscita di uno all'ingesso di un altro in serie, quindi prendere l' ultimo cavetto dell' ultimo volumetrico e collegarlo al pin di lettura arduino (qui va un resistore)

collegare il segnale driver dal pin alimentazione sirena (nel progetto arduino) dell'arduino al pin signal del nostro mosfet, collegare dunque il carico dal mosfet al positivo della sirena, collegare i 14v del caricabatteria all' assorbimento dello stesso, i comuni possono stare insieme, in quanto tensione 0v.

a questo punto collegare anche la batteria 12v al positivo 14v e al comune dello stesso, a questo punto il sistema viene alimentato, anche senza la presa di corrente.
adesso arduino segnala il mosfet e alimenta la sirena, che puo essere collegata alla sua batteria.

come funziona il codice
il codice legge tensione sul pin del volumetrico, se questa manca e lo staso dell'allarme e impostato su attivo, toglie alimetazione sulla sirena, che comincia a suonare, dopo un tempo massimo che per legge italiana e di 2 minuti e 30 secondi, rialimenta la sirena, ma non disattiva l'allarme che rimane impostato su on, se viene premuto il pulsante panico, toglie alimentazione alla sirena per lo stesso lasso di tempo, disattivare l'allarme spegne la sirena (la rialimenta), la centralina radio 4 canali viene letta come fosse dei pushbutton.

per qualsiasi informazione o chiarimento, contattatemi a bada.nino95@gmail.com
