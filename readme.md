##Název
ftrestd
##Přehled
ftrestd [OPTION] 
##Popis
Vytvoří server, na portu 6677 a s root složkou v umístění programu

-p PORT změní port na hodnotu PORT

-r ROOT změní root složku na ROOT

#AUTOR
Richard Bureš

#SEE ALSO
ftrest

##Název
frestd
##Přehled
frestd  COMMAND REMOTE-PATH [LOCAL-PATH]
##Popis
Pošle request na server na andrese REMOTE-PATH žádající o operaci COMMAND

#Operace
del: smaže soubor určený REMOTE-PATH na serveru 

get: zkopíruje soubor z REMOTE-PATH do aktuálního lokálního adresáře či na místo určené pomocí LOCAL-PATH je-li uvedeno

put: zkopíruje soubor z LOCAL-PATH do adresáře REMOTE-PATH 

lst:  vypíše obsah vzdáleného adresáře na standardní výstup (formát bude stejný jako výstup z příkazu ls)

mkd: vytvoří adresář specifikovaný v REMOTE-PATH na serveru

rmd: odstraní adresář specifikovaný V REMOTE-PATH ze serveru


#AUTOR
Richard Bureš

#SEE ALSO
ftrest