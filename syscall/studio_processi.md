Prima Ora: L'infrastruttura del File System (i-node e Superblocco)

1) Che cos'è esattamente un i-node e quali metadati fondamentali contiene? Focalizzati sulla differenza tra i dati del file (contenuto) e i metadati (permessi, proprietario, tempi)

   Un i-node è la rappresentazione di un file sul disco (ovvero nel file system). I metadati fondamentali che contiene sono: il tipo del file, i permessi (r-w-x), la dimensione del file, gli identificativi di proprietà (UID per User ID e GID per Group ID), dettagli sugli indirizzi dei blocchi.
 
2) Come Unix gestisce i file di grandi dimensioni se un i-node ha spazio solo per pochi indirizzi diretti ? Approfondisci il concetto di blocchi indiretti singoli, doppi e tripli.
Se un i-node ha spazio solo per pochi indirizzi diretti allora sfrutta i blocchi di indirizzi indiretti (singoli, doppi e tripli)

Approfondimento sugli Indirizzi Indiretti

Quando il file supera la dimensione coperta dai 10 blocchi diretti, Unix utilizza una struttura a "cascata" per mappare ulteriori dati:

Single Indirect (Indiretto Singolo): L'i-node contiene un puntatore a un blocco speciale sul disco. Questo blocco non contiene dati del file, ma una lista di indirizzi che puntano a ulteriori blocchi dati.
Double Indirect (Indiretto Doppio): Per file ancora più grandi, l'i-node punta a un blocco che contiene indirizzi di blocchi indiretti singoli .Ognuno di questi blocchi indiretti singoli punta poi a sua volta a una serie di blocchi dati.
Triple Indirect (Indiretto Triplo): È l'ultimo livello di espansione. L'i-node punta a un blocco che contiene indirizzi di blocchi indiretti doppi, creando tre livelli di passaggio prima di arrivare ai dati effettivi.

3) Qual è la funzione del Superblocco e perché è considerato l'elemento più critico del File System? Cerca cosa succede se viene danneggiato e quali informazioni globali memorizza
 Un superblocco contiene varie informazioni relative al File System:
 -Dimensione del file system
 -Numero di blocchi liberi, lista di blocchi liberi e lista di i-node liberi, l'indirizzo del prossimo blocco libero da assegnare
 -Dimensione lista i-node, numero di i-node liberi, lista di i-node liberi ...
 E' l'elemento più critico perché se dannegiato renderebbe inaccessibili i dati sui file.
Meccanismo di protezione (Dettaglio importante): Proprio perché è così critico, il sistema non ne conserva una sola copia. Le fonti specificano che vengono fatte diverse copie del superblocco in vari blocchi del disco a intervalli regolari, proprio per permettere il recupero dei dati in caso di danneggiamento di una delle copie

4) In che modo una directory associa un "nome file" al suo contenuto fisico? Osserva come le directory siano file speciali che mappano nomi a numeri di i-node.
Una directory è un i-node che che associa uno o più blocchi dati al cui interno c'è una coppia contenente il nome del file

5) Cosa succede nell'i-node quando viene invocata la SC unlink()? Distingui tra la rimozione di un link e la cancellazione effettiva dei blocchi dati dal disco.



Seconda Ora: Le Chiamate di Sistema e i Descrittori

Cosa restituisce esattamente la open() in caso di successo e dove viene memorizzato questo valore? Studia la tabella dei descrittori di file nella user area del processo.
   In caso di successo, la open() restituisce un intero che è il descrittore del file appena aperto.
   Il fd viene memorizzato nella tabella dei descrittori di file (nella user area del processo). Il valore restituito (fd) costituisce l'indice di una posizione libera all'interno della tabella dei descrittori di file.
   

2. Quali sono i passaggi interni che il kernel compie durante una open("/usr/sp/mbox")? Segui il percorso dalla   root directory fino al recupero dell'i-node finale.

3. Come interagiscono i flag O_CREAT, O_TRUNC e O_EXCL? Capisci in quali scenari l'apertura di un file fallisce o sovrascrive dati esistenti. Risposta: questi flag sono tutti modalità di apertura o accesso al file.
O_CREAT se il file non esiste, viene creato (funziona solo su file regolari)
O_TRUNC se il file esiste, viene sovrascritto, cioè i dati sopra vengono cancellati e sostituiti da quelli che scriverò
O_EXCL se il file esiste, l'apertura fallisce, si dà errore


4. In che modo il parametro umask influenza i permessi di un file appena creato? Prova a capire la logica bit a bit: perm & ~umask.

5. Qual è il significato del valore di ritorno di read()? Analizza i tre casi: valore positivo (byte letti), zero (fine file) e -1 (errore).

6. Perché la close() non garantisce che i dati siano stati scritti fisicamente sul disco? Rifletti sulla differenza tra la chiusura del descrittore e il flushing del buffer cache del kernel.

7. Perché è considerato un errore grave testare errno dopo aver chiamato una funzione di stampa come printf()? Ricorda che le SC successive sovrascrivono il codice d'errore.

8. Quando dovresti preferire le SC POSIX (open, read) rispetto alla libreria standard (fopen, fread)? Valuta il compromesso tra controllo dei blocchi e l'efficienza del buffering in spazio utente.
