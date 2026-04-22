#!/bin/bash #lo script sarà eseguito coll'interprete bash che si trova in /bin/bash 

case $# in 
        0) echo Ecco i contenuti del directory corrente $PWD; ls;; 
#$PWD è la variabile che contiene il percorso della directory corrente        
        1) head $1;; #head mostra le prime 10 righe del file indicato da $1.
        2) if diff $1 $2 > /dev/null; then
                echo i due file sono identici
        else
            echo i due file sono diversi
        fi ;;
#diff $1 $2 confronta i contenuti dei file $1 e $2.
#L’output di diff viene rediretto a /dev/null per non mostrarlo.
#Se i file sono identici, diff restituisce codice di uscita 0
        *) echo $0: troppi parametri ;;
esac    