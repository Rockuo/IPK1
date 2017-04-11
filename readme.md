##Název
ftrestd
##Přehled
ftrestd [OPTION] 
##Popis
Vytvoří server, na portu 6677 a s root složkou v umístění programu

-p PORT změní port na hodnotu PORT

-r ROOT změní root složku na ROOT

##Implementace
Implementave se skládá ze 3 částí, knihovny genericFunctions
 v níž jsou funkce pro práci se soubory, sockety, a pro parsování hlavičky
 do struktury Header. Dále je zde hlavičkový soubor structures.h, obsahující typy se
 kterými se v programu pracuje. Samotný program má pouze 2 funkce, funkci zajišťující 
 výpis obsahu složky a hlavní funkce v níž se iterativně poslouchá na zadaném portu a 
 Při ustanovení připojení se zavolá funkce z genericFunctions, pro získání hlavičky
 a následně je získáno tělo, po získání potřebných dat zesocketu je provedena operace 
 pomocí základních předdefinovaných funkcí knihoven jako např. unistd.h

#AUTOR
Richard Bureš

#SEE ALSO
ftrest

##Název
ftrest
##Přehled
ftrest  COMMAND REMOTE-PATH [LOCAL-PATH]
##Popis
Pošle request na server na andrese REMOTE-PATH žádající o operaci COMMAND

#Operace
del: smaže soubor určený REMOTE-PATH na serveru 

get: zkopíruje soubor z REMOTE-PATH do aktuálního lokálního adresáře či na místo určené pomocí LOCAL-PATH je-li uvedeno

put: zkopíruje soubor z LOCAL-PATH do adresáře REMOTE-PATH 

lst:  vypíše obsah vzdáleného adresáře na standardní výstup (formát bude stejný jako výstup z příkazu ls)

mkd: vytvoří adresář specifikovaný v REMOTE-PATH na serveru

rmd: odstraní adresář specifikovaný V REMOTE-PATH ze serveru

##Implementace
Implementave se skládá ze 3 částí, knihovny genericFunctions
 a hlavičkového souboru structures.h, viz ftrestd. Program nejprve rozparsuje
 předané argumenty, nastaví sy stav na jeden z možných stavů ustanovených v enum 
 actionType. Poté sestaví hlavičku. Pokud se jedná o operaci PUT tak získá soubor a vloží ho, jinak
 je v hlavičce Content-length vždy 0. Následně Je sestaveno spojení a obsah je odeslán.
 Program pak už jen čeká na odpověď, která je-li v pořádku, dokoná operaci, a následně se ukončí.


#AUTOR
Richard Bureš

#SEE ALSO
ftrest